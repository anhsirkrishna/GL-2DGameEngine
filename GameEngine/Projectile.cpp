/******************************************************************************/
/* !
/* File   Projectile.cpp
/* Author Krishna Pillai
/* Email: krishna.pillai@digipen.edu
/* Date   02/28/2022
/* Projectile Component implementation file
/* DigiPen Institute of Technology © 2022
/******************************************************************************/
#include "Projectile.h"
#include "Transform.h"
#include "GameObjectFactory.h"
#include "GameObjectManager.h"
#include "EventManager.h"
#include "PhysicsWorld.h"

Projectile::Projectile() : Component("PROJECTILE"), p_instances(), 
						   instance_count(0), last_used_instance(0),
							p_owner_transform(nullptr), instance_file() {
}

/*Serializes this component by creating all the required
* number of instances of the projectile.
* Expects a dict with the following key:values
* "instance_def": "Obj_def" for the projectile object
* "instance_count": the number of instances
* Returns: void
*/
void Projectile::Serialize(json json_object) {
	instance_file = json_object["instance_def"].get<std::string>();
	instance_count = json_object["instance_count"].get<int>();

	std::string projectile_name = "_Projectile";
	GameObjectFactory go_factory;
	GameObject* new_object;
	for (unsigned int i = 0; i < instance_count; i++) {
		new_object = go_factory.CreateGameObject(projectile_name, instance_file);
		new_object->enabled = false;
		p_game_obj_manager->AddGameObject(new_object);
		p_instances.push_back(new_object);
	}
}

void Projectile::Link() {
	p_owner_transform = static_cast<Transform*>(GetOwner()->HasComponent("TRANSFORM"));
}

void Projectile::Spawn() {
	GameObject* new_projectile = p_instances[GetLastUsedInstance()];
	new_projectile->enabled = true;

	Transform* projectile_transform = static_cast<Transform*>(new_projectile->HasComponent("TRANSFORM"));
	projectile_transform->SetScale(p_owner_transform->GetScaleX(), p_owner_transform->GetScaleY());
	glm::vec4 new_position = p_owner_transform->GetPosition();
	new_position.x += 30;
	new_position.y += 30;
	projectile_transform->SetPosition(new_position);
}

int Projectile::GetLastUsedInstance() {
	//Should usually return immediately 
	for (unsigned int i = last_used_instance; i < instance_count; i++) {
		if (!p_instances[i]->enabled) {
			last_used_instance = i;
			return last_used_instance;
		}
	}

	//Otherwise perform linear search
	for (unsigned int i = 0; i < last_used_instance; i++) {
		if (!p_instances[i]->enabled) {
			last_used_instance = i;
			return last_used_instance;
		}
	}

	last_used_instance = 0;
	return last_used_instance;
}