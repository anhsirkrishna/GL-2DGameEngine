#include "EnemyProjectile.h"
#include "GameObjectFactory.h"
#include "GameObjectManager.h"
#include "PhysicsWorld.h"
#include "Hitbox.h"

EnemyProjectile::EnemyProjectile() : Component("ENEMYPROJECTILE"), instance_count(0), instance_def(""),
	instance_index(0), p_owner_transform(nullptr), spawn_offset(glm::vec3(0)), proj_stay_duration(1000.0f)
{
}

void EnemyProjectile::CreateAndStoreInstances()
{
	// Create a set number of instances of the projectile
	for (int i = 0; i < instance_count; i++) {
		std::string projectile_name = "Enemy_Projectile_" + std::to_string(i);
		GameObjectFactory go_factory;
		GameObject* new_object;
		new_object = go_factory.CreateGameObject(projectile_name, instance_def);
		new_object->SetActive(false); // Not active yet
		p_game_obj_manager->AddGameObject(new_object);
		projectiles.push_back(new_object);
		proj_timers.push_back(proj_stay_duration);
		p_physics_world->AddPhysicsGameObject(new_object);
	}
}

void EnemyProjectile::Update()
{
	// Keep looping through timers for each projectile
	for (int i = 0; i < projectiles.size(); i++) {
		
		// if projectile is active, decrement timer
		if (projectiles[i]->IsActive()) {
			proj_timers[i] -= 10.0f;

			// timer for that projectile is down to 0 inactivate it.
			if (proj_timers[i] < 0) {
				projectiles[i]->SetActive(false);
			}
		}
	}
}

void EnemyProjectile::Serialize(json json_object)
{
	instance_count = json_object["instance_count"].get<int>();
	instance_def = json_object["instance_def"].get<std::string>();

	auto offset = json_object["spawn_offset"].get<std::vector<int>>();
	spawn_offset = glm::vec3(offset[0], offset[1], offset[2]);

	CreateAndStoreInstances();
}

void EnemyProjectile::Link()
{
	p_owner_transform = static_cast<Transform*>(GetOwner()->HasComponent("TRANSFORM"));
}

void EnemyProjectile::SpawnProjectile()
{
	// Get projectile instance. If not activated yet, activate it. 
	GameObject* new_projectile = projectiles[instance_index];
	if (!new_projectile->IsActive()) {
		instance_index = (instance_index + 1) % instance_count;
		proj_timers[instance_index] = proj_stay_duration;
		static_cast<Hitbox*>(new_projectile->HasComponent("HITBOX"))->DisableForABit();
		new_projectile->SetActive(true);
		new_projectile->ResetComponents();
		Transform* projectile_transform = static_cast<Transform*>(new_projectile->HasComponent("TRANSFORM"));
		projectile_transform->SetScale(p_owner_transform->GetScaleX(), p_owner_transform->GetScaleY());
		glm::vec4 new_position = p_owner_transform->GetPosition();
		new_position.x += spawn_offset.x;
		new_position.y += spawn_offset.y;
		new_position.z += spawn_offset.z;
		projectile_transform->SetPosition(new_position);
	}
}

