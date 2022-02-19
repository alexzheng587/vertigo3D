#pragma once

#include "common.hpp"
#include "render_system.hpp"

const float EXPLORER_BB_WIDTH = 0.4f * window_height_px / 3;
const float EXPLORER_BB_HEIGHT = 0.4f * window_height_px / 3;

// the tile
Entity createTile(Tile tile);
// the player
Entity createPlayer(RenderSystem* renderer, vec3 origin, vec3 pos);


