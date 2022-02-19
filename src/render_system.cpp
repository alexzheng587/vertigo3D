// internal
#include "render_system.hpp"
#include <SDL.h>

#include "tiny_ecs_registry.hpp"

void RenderSystem::drawTexturedMesh(Entity entity, const mat4 &projection, const mat4 &view)
{
	assert(registry.renderRequests.has(entity));
	const RenderRequest &render_request = registry.renderRequests.get(entity);

	const GLuint used_effect_enum = (GLuint)render_request.used_effect;
	assert(used_effect_enum != (GLuint)EFFECT_ASSET_ID::EFFECT_COUNT);
	const GLuint program = (GLuint)effects[used_effect_enum];

	// Setting shaders
	glUseProgram(program);
	gl_has_errors();

	assert(render_request.used_geometry != GEOMETRY_BUFFER_ID::GEOMETRY_COUNT);
	const GLuint vbo = vertex_buffers[(GLuint)render_request.used_geometry];
	const GLuint ibo = index_buffers[(GLuint)render_request.used_geometry];

	// Setting vertex and index buffers
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	gl_has_errors();

	mat4 model = mat4(1.0f);

	// Input data location as in the vertex buffer
	if (render_request.used_effect == EFFECT_ASSET_ID::BOX)
	{
		GLint in_position_loc = glGetAttribLocation(program, "aPos");
		GLint in_texcoord_loc = glGetAttribLocation(program, "aTex");
		gl_has_errors();
		assert(in_texcoord_loc >= 0);

		glEnableVertexAttribArray(in_position_loc);
		glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE,
							  sizeof(TexturedVertex), (void *)0);
		gl_has_errors();

		glEnableVertexAttribArray(in_texcoord_loc);
		// remember to change this if tex0's type changes vec2/vec3
		glVertexAttribPointer(
			in_texcoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex),
			(void *)sizeof(
				vec3)); // note the stride to skip the preceeding vertex position

		// Enabling and binding texture to slot 0
		glActiveTexture(GL_TEXTURE0);
		gl_has_errors();

		assert(registry.renderRequests.has(entity));
		GLuint texture_id =
			texture_gl_handles[(GLuint)registry.renderRequests.get(entity).used_texture];

		// use 2d
		glBindTexture(GL_TEXTURE_2D, texture_id);
		gl_has_errors();

		// handle rotation
		Tile& boxRotate = registry.tiles.get(entity);
		switch (boxRotate.status) {
			case BOX_ANIMATION::UP:
				boxRotate.model = rotate(glm::mat4(1.0f), (float)radians(-1.0f), vec3(1.0f, 0.0f, 0.0f)) * boxRotate.model;
				boxRotate.degrees++;
				break;
			case BOX_ANIMATION::DOWN:
				boxRotate.model = rotate(glm::mat4(1.0f), (float)radians(1.0f), vec3(1.0f, 0.0f, 0.0f)) * boxRotate.model;
				boxRotate.degrees++;
				break;
			case BOX_ANIMATION::LEFT:
				boxRotate.model = rotate(glm::mat4(1.0f), (float)radians(-1.0f), vec3(0.0f, 1.0f, 0.0f)) * boxRotate.model;
				boxRotate.degrees++;
				break;
			case BOX_ANIMATION::RIGHT:
				boxRotate.model = rotate(glm::mat4(1.0f), (float)radians(1.0f), vec3(0.0f, 1.0f, 0.0f)) * boxRotate.model;
				boxRotate.degrees++;
				break;
		}
		if (boxRotate.degrees == 90)
		{
			boxRotate.degrees = 0;
			boxRotate.status = BOX_ANIMATION::STILL;
		}
		model = boxRotate.model;
	}
	// Input data location as in the vertex buffer
	else if (render_request.used_effect == EFFECT_ASSET_ID::TEXTURED)
	{
		GLint in_position_loc = glGetAttribLocation(program, "in_position");
		GLint in_texcoord_loc = glGetAttribLocation(program, "in_texcoord");
		gl_has_errors();
		assert(in_texcoord_loc >= 0);

		glEnableVertexAttribArray(in_position_loc);
		glVertexAttribPointer(in_position_loc, 3, GL_FLOAT, GL_FALSE,
			sizeof(TexturedVertex), (void*)0);
		gl_has_errors();

		glEnableVertexAttribArray(in_texcoord_loc);
		glVertexAttribPointer(
			in_texcoord_loc, 2, GL_FLOAT, GL_FALSE, sizeof(TexturedVertex),
			(void*)sizeof(
				vec3)); // note the stride to skip the preceeding vertex position

		// Enabling and binding texture to slot 0
		glActiveTexture(GL_TEXTURE0);
		gl_has_errors();

		assert(registry.renderRequests.has(entity));
		GLuint texture_id =
			texture_gl_handles[(GLuint)registry.renderRequests.get(entity).used_texture];

		glBindTexture(GL_TEXTURE_2D, texture_id);
		gl_has_errors();


		assert(registry.motions.has(entity));
		Motion motion = registry.motions.get(entity);
		// TODO: Figure out how to translate
		
	}
	else
	{
		assert(false && "Type of render request not supported");
	}

	// Get number of indices from index buffer, which has elements uint16_t
	GLint size = 0;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	gl_has_errors();

	GLsizei num_indices = size / sizeof(uint16_t);
	// GLsizei num_triangles = num_indices / 3;

	GLint currProgram;
	glGetIntegerv(GL_CURRENT_PROGRAM, &currProgram);
	// Setting uniform values to the currently bound program
	GLuint model_loc = glGetUniformLocation(currProgram, "model");
	glUniformMatrix4fv(model_loc, 1, GL_FALSE, (float *)&model);
	GLuint view_loc = glGetUniformLocation(currProgram, "view");
	glUniformMatrix4fv(view_loc, 1, GL_FALSE, (float *)&view);
	GLuint projection_loc = glGetUniformLocation(currProgram, "proj");
	glUniformMatrix4fv(projection_loc, 1, GL_FALSE, (float *)&projection);
	gl_has_errors();
	// Drawing of num_indices/3 triangles specified in the index buffer
	glDrawElements(GL_TRIANGLES, num_indices, GL_UNSIGNED_SHORT, nullptr);
	gl_has_errors();
}

// draw the intermediate texture to the screen
void RenderSystem::drawToScreen()
{
	// Clearing backbuffer
	int w, h;
	glfwGetFramebufferSize(window, &w, &h); // Note, this will be 2x the resolution given to glfwCreateWindow on retina displays
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, w, h);
	glDepthRange(0, 10);
	glClearColor(0, 0, 0, 1.0);
	glClearDepth(1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	gl_has_errors();
	// Enabling alpha channel for textures
	glDisable(GL_BLEND);
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);

	// Draw the screen texture on the quad geometry
	glBindBuffer(GL_ARRAY_BUFFER, vertex_buffers[(GLuint)GEOMETRY_BUFFER_ID::CUBE]);
	glBindBuffer(
		GL_ELEMENT_ARRAY_BUFFER,
		index_buffers[(GLuint)GEOMETRY_BUFFER_ID::CUBE]); // Note, GL_ELEMENT_ARRAY_BUFFER associates
																	 // indices to the bound GL_ARRAY_BUFFER
	gl_has_errors();

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_2D, off_screen_render_buffer_color);
	gl_has_errors();
	// Draw

	// Get number of indices from index buffer, which has elements uint16_t
	GLint size = 0;
	glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
	gl_has_errors();

	GLsizei num_indices = size / sizeof(uint16_t);

	glDrawElements(
		GL_TRIANGLES, num_indices, GL_UNSIGNED_SHORT,
		nullptr); // one triangle = 3 vertices; nullptr indicates that there is
				  // no offset from the bound index buffer
	gl_has_errors();
}

// Render our game world
// http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-14-render-to-texture/
void RenderSystem::draw()
{
	// Getting size of window
	int w, h;
	glfwGetFramebufferSize(window, &w, &h); // Note, this will be 2x the resolution given to glfwCreateWindow on retina displays

	// First render to the custom framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	gl_has_errors();
	// Clearing backbuffer
	glViewport(0, 0, w, h);
	glDepthRange(0.00001, 10);
	glClearColor(0, 0, 0, 1.0);
	glClearDepth(10.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST); // native OpenGL does not work with a depth buffer
							  // and alpha blending, one would have to sort
							  // sprites back to front
	gl_has_errors();
	mat4 projection_3D = createProjectionMatrix(w, h);
	mat4 view = createViewMatrix();
	// Draw all textured meshes that have a position and size component
	for (Entity entity : registry.renderRequests.entities)
	{
		// Note, its not very efficient to access elements indirectly via the entity
		// albeit iterating through all Sprites in sequence. A good point to optimize
		drawTexturedMesh(entity, projection_3D, view);
	}

	// Truely render to the screen
	// drawToScreen();

	// flicker-free display with a double buffer
	glfwSwapBuffers(window);
	gl_has_errors();
}

mat4 RenderSystem::createViewMatrix()
{
	mat4 view = lookAt(vec3(6.0f, 6.0f, 6.0f),
							vec3(0.0f, 0.0f, 0.0f),
							vec3(0.0f, 1.0f, 0.0f));
	return view;
}

mat4 RenderSystem::createProjectionMatrix(int width, int height)
{
	mat4 proj = mat4(1.0f);
	proj = perspective(radians(45.0f), (float)width / height, 0.1f, 100.0f);
	return proj;
}