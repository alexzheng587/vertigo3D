#version 330 core

// Outputs colors in RGBA
out vec4 FragColor;

// Inputs the texture coordinates from the Vertex Shader
in vec3 texCoord;

// Gets the Texture Unit from the main function
uniform samplerCube tex0;

void main()
{
	FragColor = texture(tex0, texCoord);
}