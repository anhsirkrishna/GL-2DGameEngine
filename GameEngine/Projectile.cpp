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
							p_owner_transform(nullptr), instance_file(),
							recycle() {
}

/*No action required here since serialization is 
* being done during ChangeState()
* Returns: void
*/
void Projectile::Serialize(json json_object) {
	std::string current_state = "DEFAULT";

	//State hasn't been instantiated
	if (p_instances.find(current_state) == p_instances.end()) {
		instance_file = json_object["instance_def"].get<std::string>();
		instance_count[current_state] = json_object["instance_count"].get<int>();
		auto offset = json_object["spawn_offset"].get<std::vector<float>>();
		spawn_offset[current_state] = glm::vec3(offset[0], offset[1], offset[2]);

		std::string projectile_name = "_Projectile";
		GameObjectFactory go_factory;
		GameObject* new_object;
		for (unsigned int i = 0; i < instance_count[current_state]; i++) {
			new_object = go_factory.CreateGameObject(projectile_name, instance_file);
			new_object->SetActive(false);
			p_game_obj_manager->AddGameObject(new_object);
			p_instances[current_state].push_back(new_object);
			p_physics_world->AddPhysicsGameObject(new_object);
		}
	}
}

void Projectile::Link() {
	p_owner_transform = static_cast<Transform*>(GetOwner()->HasComponent("TRANSFORM"));
}

void Projectile::Spawn() {
	std::string current_state = GetOwner()->state_manager.GetCurrentState();

	if (p_instances.find(current_state) == p_instances.end()) {
		current_state = "DEFAULT";
	}

	GameObject* new_projectile = p_instances[current_state][GetLastUsedInstance()];
	if (new_projectile->IsActive() && !recycle[current_state])
		return;
	new_projectile->SetActive(true);
	new_projectile->ResetComponents();
	
	Transform* projectile_transform = static_cast<Transform*>(new_projectile->HasComponent("TRANSFORM"));
	projectile_transform->SetScale(p_owner_transform->GetScaleX(), p_owner_transform->GetScaleY());
	glm::vec4 new_position = p_owner_transform->GetPosition();

	if (p_owner_transform->GetScaleX() > 0)
		new_position.x += (spawn_offset[current_state].x/2);
	else
		new_position.x -= (spawn_offset[current_state].x*4);

	new_position.y += (spawn_offset[current_state].y * p_owner_transform->GetScaleY());

	new_position.z += spawn_offset[current_state].z;
	projectile_transform->SetPosition(new_position);
}

int Projectile::GetLastUsedInstance() {
	std::string current_state = GetOwner()->state_manager.GetCurrentState();
	//Should usually return immediately 
	for (unsigned int i = last_used_instance[current_state]; i < instance_count[current_state]; i++) {
		if (!p_instances[current_state][i]->IsActive()) {
			last_used_instance[current_state] = i;
			return last_used_instance[current_state];
		}
	}

	//Otherwise perform linear search
	for (unsigned int i = 0; i < last_used_instance[current_state]; i++) {
		if (!p_instances[current_state][i]->IsActive()) {
			last_used_instance[current_state] = i;
			return last_used_instance[current_state];
		}
	}

	last_used_instance[current_state] = 0;
	return last_used_instance[current_state];
}

/*Changes the state of the component
* Checks if state has already been instantiated
* If it hasn't then it creates game objects for 
* the number of instances of the projectile.
* Expects a dict with the following key:values
* "instance_def": "Obj_def" for the projectile object
* "instance_count": the number of instances
* Returns: void
*/
void Projectile::ChangeState(json json_object) {
	std::string current_state = GetOwner()->state_manager.GetCurrentState();

	//State hasn't been instantiated
	if (p_instances.find(current_state) == p_instances.end()) {
		instance_file = json_object["instance_def"].get<std::string>();
		instance_count[current_state] = json_object["instance_count"].get<int>();
		auto offset = json_object["spawn_offset"].get<std::vector<float>>();
		spawn_offset[current_state] = glm::vec3(offset[0], offset[1], offset[2]);

		recycle[current_state] = json_object["recycle"].get<bool>();

		std::string projectile_name = "_Projectile";
		GameObjectFactory go_factory;
		GameObject* new_object;
		for (unsigned int i = 0; i < instance_count[current_state]; i++) {
			new_object = go_factory.CreateGameObject(projectile_name, instance_file);
			new_object->SetActive(false);
			p_game_obj_manager->AddGameObject(new_object);
			p_instances[current_state].push_back(new_object);
			p_physics_world->AddPhysicsGameObject(new_object);
		}
	}
}