#pragma once

#include "Component.h"
#include "Transform.h"
#include <glm.hpp>

class EnemyProjectile : public Component
{
private:
	std::vector<GameObject*> projectiles;
	std::vector<float> proj_timers;
	std::string instance_def;
	int instance_count;
	int instance_index;
	glm::vec3 spawn_offset;
	Transform* p_owner_transform;
	float proj_stay_duration;

public:
	EnemyProjectile();

	// Create a no. of projectiles (= intance_count) and store them in a vector
	void CreateAndStoreInstances();

	// Spawn Projectile 
	void SpawnProjectile();

	/* Update function for this component:
	 * Check to see if a projectile should be disabled
	 */
	void Update();

	// JSON Serialization
	void Serialize(json json_object);

	// Store references to other components
	void Link();
};

