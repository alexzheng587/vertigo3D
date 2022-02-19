#pragma once
#include "common.hpp"
#include <vector>
#include <array>
#include <unordered_map>
#include "../ext/stb_image/stb_image.h"

// Player component
struct Player
{

};

// Single Vertex Buffer element for non-textured meshes (coloured.vs.glsl & chicken.vs.glsl)
struct ColoredVertex
{
	vec3 position;
	vec3 color;
};

// Single Vertex Buffer element for textured sprites (textured.vs.glsl)
struct TexturedVertex
{
	vec3 position;
	vec2 texcoord;
};

// Mesh datastructure for storing vertex and index buffers
struct Mesh
{
	static bool loadFromOBJFile(std::string obj_path, std::vector<ColoredVertex>& out_vertices, std::vector<uint16_t>& out_vertex_indices, vec2& out_size);
	vec2 original_size = {1,1};
	std::vector<ColoredVertex> vertices;
	std::vector<uint16_t> vertex_indices;
};

enum class BOX_ANIMATION {
	STILL = 0,
	UP = 1,
	DOWN = 2,
	LEFT = 3,
	RIGHT = 4
};

// Set each character to their placement in the alphabet for easy conversion from the CSV to TileState
enum class TileState
{
	S = 18,
	F = 5,
	V = 21,
	O = 14,
	E = 4,
};

struct Tile
{
	BOX_ANIMATION status = BOX_ANIMATION::STILL;
	glm::mat4 model;
	int degrees = 0;
	TileState tileState = TileState::E;
};

// represents the entire cube
// front -> left -> right -> top -> bottom -> back
struct Cube
{
	bool loadFromExcelFile(std::string filename);
	std::array<std::vector<std::vector<Tile>>, 6> faces;
	int size = 0;
};

struct Motion
{
	bool interpolate = false; // 0 for interpolation, 1 for extrapolation
	vec3 velocity = {0, 0, 0}; // Used if extrapolating
	vec3 destination = {0, 0, 0}; // Used if interpolating
	float remaining_time = 0; // Used if interpolating

	// For rendering 3d coordinates to 2d screen
	vec3 origin = {0, 0, 0};
	vec2 x_vector = {cos(radians(30.0f)), sin(radians(30.0f))};
	vec2 y_vector = {0, 1};
	vec2 z_vector = {cos(radians(60.0f)), sin(radians(60.0f))};
	vec3 position = {0, 0, 0};
	
	vec2 scale = {10, 10};
};

/**
 * The following enumerators represent global identifiers refering to graphic
 * assets. For example TEXTURE_ASSET_ID are the identifiers of each texture
 * currently supported by the system.
 *
 * So, instead of referring to a game asset directly, the game logic just
 * uses these enumerators and the RenderRequest struct to inform the renderer
 * how to structure the next draw command.
 *
 * There are 2 reasons for this:
 *
 * First, game assets such as textures and meshes are large and should not be
 * copied around as this wastes memory and runtime. Thus separating the data
 * from its representation makes the system faster.
 *
 * Second, it is good practice to decouple the game logic from the render logic.
 * Imagine, for example, changing from OpenGL to Vulkan, if the game logic
 * depends on OpenGL semantics it will be much harder to do the switch than if
 * the renderer encapsulates all asset data and the game logic is agnostic to it.
 *
 * The final value in each enumeration is both a way to keep track of how many
 * enums there are, and as a default value to represent uninitialized fields.
 */

enum class TEXTURE_ASSET_ID {
	BOX = 0,
	EXPLORER = BOX + 1,
	TEXTURE_COUNT = EXPLORER + 1
};
const int texture_count = (int)TEXTURE_ASSET_ID::TEXTURE_COUNT;

enum class EFFECT_ASSET_ID {
	BOX = 0,
	TEXTURED = BOX + 1,
	EFFECT_COUNT = TEXTURED + 1
};
const int effect_count = (int)EFFECT_ASSET_ID::EFFECT_COUNT;

enum class GEOMETRY_BUFFER_ID {
	CUBE = 0,
	SPRITE = 1,
	GEOMETRY_COUNT = SPRITE + 1
};
const int geometry_count = (int)GEOMETRY_BUFFER_ID::GEOMETRY_COUNT;

struct RenderRequest {
	TEXTURE_ASSET_ID used_texture = TEXTURE_ASSET_ID::TEXTURE_COUNT;
	EFFECT_ASSET_ID used_effect = EFFECT_ASSET_ID::EFFECT_COUNT;
	GEOMETRY_BUFFER_ID used_geometry = GEOMETRY_BUFFER_ID::GEOMETRY_COUNT;
};

