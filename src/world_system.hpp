#pragma once

// internal
#include "common.hpp"

// stlib
#include <vector>
#include <random>

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_mixer.h>

#include "render_system.hpp"

// Container for all our entities and game logic. Individual rendering / update is
// deferred to the relative update() methods
class WorldSystem
{
public:
	WorldSystem();

	// Creates a window
	GLFWwindow* create_window();

	// starts the game
	void init(RenderSystem* renderer);

	// Releases all associated resources
	~WorldSystem();

	// Should the game be over ?
	bool is_over()const;
private:
	// Input callback functions
	void on_key(int key, int, int action, int mod);

	// OpenGL window handle
	GLFWwindow* window;

	// Game state
	RenderSystem* renderer;
	Cube cube;

	// music references
	// Mix_Music* background_music;
	// Mix_Chunk* chicken_dead_sound;
	// Mix_Chunk* chicken_eat_sound;
};
