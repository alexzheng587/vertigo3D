// internal
#include "physics_system.hpp"
#include "world_init.hpp"

// standard
#include <math.h> 
#define PI 3.14159265

void PhysicsSystem::step(float elapsed_ms)
{

	// Move bug based on how much time has passed, this is to (partially) avoid
	// having entities move at different speed based on the machine.
	auto& motion_registry = registry.motions;
	for (uint i = 0; i < motion_registry.size(); i++)
	{
		Motion& motion = motion_registry.components[i];
		Entity entity = motion_registry.entities[i];
		float step_seconds = elapsed_ms / 1000.f;

		if (motion.interpolate){
			if (elapsed_ms > motion.remaining_time){
				motion.position = motion.destination;
				motion.remaining_time = 0;
			}
			else{
				motion.position = (motion.destination - motion.position) * (elapsed_ms / motion.remaining_time);
				motion.remaining_time -= elapsed_ms;
			}
		}
		else{
			motion.position = motion.position + motion.velocity * step_seconds;
		}
	}
}