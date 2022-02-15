// Header
#include "world_system.hpp"
#include "world_init.hpp"

// stlib
#include <cassert>
#include <sstream>

#include "tiny_ecs_registry.hpp"

// Create the bug world
WorldSystem::WorldSystem()
{

}

WorldSystem::~WorldSystem() {
	// Destroy music components
	// if (background_music != nullptr)
	// 	Mix_FreeMusic(background_music);
	// if (chicken_dead_sound != nullptr)
	// 	Mix_FreeChunk(chicken_dead_sound);
	// if (chicken_eat_sound != nullptr)
	// 	Mix_FreeChunk(chicken_eat_sound);
	// Mix_CloseAudio();

	// Destroy all created components
	registry.clear_all_components();

	// Close the window
	glfwDestroyWindow(window);
}

// Debugging
namespace {
	void glfw_err_cb(int error, const char *desc) {
		fprintf(stderr, "%d: %s", error, desc);
	}
}

// World initialization
// Note, this has a lot of OpenGL specific things, could be moved to the renderer
GLFWwindow* WorldSystem::create_window() {
	///////////////////////////////////////
	// Initialize GLFW
	glfwSetErrorCallback(glfw_err_cb);
	if (!glfwInit()) {
		fprintf(stderr, "Failed to initialize GLFW");
		return nullptr;
	}

	//-------------------------------------------------------------------------
	// If you are on Linux or Windows, you can change these 2 numbers to 4 and 3 and
	// enable the glDebugMessageCallback to have OpenGL catch your mistakes for you.
	// GLFW / OGL Initialization
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	glfwWindowHint(GLFW_RESIZABLE, 0);

	// Create the main window (for rendering, keyboard, and mouse input)
	window = glfwCreateWindow(window_width_px, window_height_px, "Vertigo Game", nullptr, nullptr);
	if (window == nullptr) {
		fprintf(stderr, "Failed to glfwCreateWindow");
		return nullptr;
	}

	// Setting callbacks to member functions (that's why the redirect is needed)
	// Input is handled using GLFW, for more info see
	// http://www.glfw.org/docs/latest/input_guide.html
	glfwSetWindowUserPointer(window, this);
	auto key_redirect = [](GLFWwindow* wnd, int _0, int _1, int _2, int _3) { ((WorldSystem*)glfwGetWindowUserPointer(wnd))->on_key(_0, _1, _2, _3); };
	glfwSetKeyCallback(window, key_redirect);

	//////////////////////////////////////
	// Loading music and sounds with SDL
	// if (SDL_Init(SDL_INIT_AUDIO) < 0) {
	// 	fprintf(stderr, "Failed to initialize SDL Audio");
	// 	return nullptr;
	// }
	// if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
	// 	fprintf(stderr, "Failed to open audio device");
	// 	return nullptr;
	// }

	// background_music = Mix_LoadMUS(audio_path("music.wav").c_str());
	// chicken_dead_sound = Mix_LoadWAV(audio_path("chicken_dead.wav").c_str());
	// chicken_eat_sound = Mix_LoadWAV(audio_path("chicken_eat.wav").c_str());

	// if (background_music == nullptr || chicken_dead_sound == nullptr || chicken_eat_sound == nullptr) {
	// 	fprintf(stderr, "Failed to load sounds\n %s\n %s\n %s\n make sure the data directory is present",
	// 		audio_path("music.wav").c_str(),
	// 		audio_path("chicken_dead.wav").c_str(),
	// 		audio_path("chicken_eat.wav").c_str());
	// 	return nullptr;
	// }

	return window;
}

void WorldSystem::init(RenderSystem* renderer_arg) {
	this->renderer = renderer_arg;
	// TODO: implement level select (maybe here)
	cube.loadFromExcelFile(level_path("level2.csv"));
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < cube.size; j++) {
			for (int k = 0; k < cube.size; k++) {
				if (!(cube.faces[i][j][k].tileState == TileState::E))
					createTile(cube.faces[i][j][k]);
			}
		}
	}

	// // Playing background music indefinitely
	// Mix_PlayMusic(background_music, -1);
	// fprintf(stderr, "Loaded music\n");
}

// Should the game be over ?
bool WorldSystem::is_over() const {
	return bool(glfwWindowShouldClose(window));
}

// On key callback
void WorldSystem::on_key(int key, int, int action, int mod) {
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// key is of 'type' GLFW_KEY_
	// action can be GLFW_PRESS GLFW_RELEASE GLFW_REPEAT
	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	if (key == GLFW_KEY_UP) {
		for (Tile& t : registry.tiles.components) {
			if (t.status == BOX_ANIMATION::STILL)
				t.status = BOX_ANIMATION::UP;
		}
	}

	else if (key == GLFW_KEY_DOWN) {
		for (Tile& t : registry.tiles.components) {
			if (t.status == BOX_ANIMATION::STILL)
				t.status = BOX_ANIMATION::DOWN;
		}
	}

	else if (key == GLFW_KEY_LEFT) {
		for (Tile& t : registry.tiles.components) {
			if (t.status == BOX_ANIMATION::STILL)
				t.status = BOX_ANIMATION::LEFT;
		}
	}

	else if (key == GLFW_KEY_RIGHT) {
		for (Tile& t : registry.tiles.components) {
			if (t.status == BOX_ANIMATION::STILL)
				t.status = BOX_ANIMATION::RIGHT;
		}
	}
}
