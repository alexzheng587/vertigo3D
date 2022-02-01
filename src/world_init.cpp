#include "world_init.hpp"
#include "tiny_ecs_registry.hpp"

Entity createPlayer()
{
	auto entity = Entity();

	// Store a reference to the potentially re-used mesh object
	// Mesh& mesh = renderer->getMesh(GEOMETRY_BUFFER_ID::CHICKEN);
	// registry.meshPtrs.emplace(entity, &mesh);

	// Create and (empty) Chicken component to be able to refer to all eagles
	// registry.players.emplace(entity);
	// registry.renderRequests.insert(
	// 	entity,
	// 	{ TEXTURE_ASSET_ID::TEXTURE_COUNT, // TEXTURE_COUNT indicates that no txture is needed
	// 		EFFECT_ASSET_ID::EFFECT_COUNT,
	// 		GEOMETRY_BUFFER_ID::GEOMETRY_COUNT });

	return entity;
}

Entity createBox()
{
	Entity entity = Entity();

	// Store a reference to the potentially re-used mesh object (the value is stored in the resource cache)
	registry.renderRequests.insert(
		entity,
		{ TEXTURE_ASSET_ID::BOX,
		 EFFECT_ASSET_ID::BOX,
		 GEOMETRY_BUFFER_ID::CUBE });

	return entity;
}