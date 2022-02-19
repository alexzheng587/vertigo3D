#include "world_init.hpp"
#include "tiny_ecs_registry.hpp"

Entity createPlayer(RenderSystem* renderer, vec3 origin, vec3 pos) {
	auto entity = Entity();

	Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::SPRITE);
	registry.meshPtrs.emplace(entity, &mesh);

	// Setting initial motion values
	Motion& motion = registry.motions.emplace(entity);
	motion.interpolate = true;
	motion.origin = origin;
	motion.position = pos;
	motion.velocity = { 0.f, 0.f , 0.f };
	motion.scale = vec2({ EXPLORER_BB_WIDTH, EXPLORER_BB_HEIGHT });

	Player& explorer = registry.players.emplace(entity);

	registry.renderRequests.insert(
		entity,
		{
			TEXTURE_ASSET_ID::EXPLORER,
			EFFECT_ASSET_ID::TEXTURED,
			GEOMETRY_BUFFER_ID::SPRITE
		}
	);
	return entity;
}

Entity createTile(Tile tile)
{
	Entity entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	registry.tiles.insert(entity, tile);
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::BOX,
		 EFFECT_ASSET_ID::BOX,
		 GEOMETRY_BUFFER_ID::CUBE });

	return entity;
}