/******************************************************************************/
/*!
/*File   DependantObjects.cpp
/*Author Krishna Pillai
/*Email: krishna.pillai@digipen.edu
/*Date   01/27/2022
/*  DependantObjects implementation file
/* DigiPen Institute of Technology © 2022
/******************************************************************************/

#include "DependantObjects.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "Transform.h"
#include "Camera.h"

#include <SDL.h>

DependantObjects::DependantObjects() : Component("DEPENDANT_OBJECTS"),
p_dependant_objects(), dependant_object_names() {}

void DependantObjects::Serialize(json json_object) {
	dependant_object_names = json_object["dependant_object_names"].get<std::vector<std::string>>();
}

void DependantObjects::Link() {
	for (unsigned int i = 0; i < p_game_obj_manager->game_object_list.size(); ++i) {
		for (auto& object_name : dependant_object_names) {
			if (p_game_obj_manager->game_object_list[i]->GetName() == object_name) {
				p_dependant_objects.push_back(p_game_obj_manager->game_object_list[i]);
			}
		}
	}
}

float DependantObjects::GetDependantObjectPosX(int obj_index) {
	SDL_assert(p_dependant_objects[obj_index] != nullptr);

	Transform* follow_transform = 
		static_cast<Transform*>(p_dependant_objects[obj_index]->HasComponent("TRANSFORM"));
	return follow_transform->GetPosCoord(0);
}

float DependantObjects::GetDependantObjectPosY(int obj_index) {
	SDL_assert(p_dependant_objects[obj_index] != nullptr);

	Transform* follow_transform = 
		static_cast<Transform*>(p_dependant_objects[obj_index]->HasComponent("TRANSFORM"));
	return follow_transform->GetPosCoord(1);
}

float DependantObjects::GetDependantObjectScaleX(int obj_index) {
	SDL_assert(p_dependant_objects[obj_index] != nullptr);

	Transform* follow_transform =
		static_cast<Transform*>(p_dependant_objects[obj_index]->HasComponent("TRANSFORM"));
	return follow_transform->GetScaleX();
}

float DependantObjects::GetDependantObjectScaleY(int obj_index) {
	SDL_assert(p_dependant_objects[obj_index] != nullptr);

	Transform* follow_transform =
		static_cast<Transform*>(p_dependant_objects[obj_index]->HasComponent("TRANSFORM"));
	return follow_transform->GetScaleY();
}

bool DependantObjects::IsObjectDependant(GameObject* dependant_obj) {
	for (auto& p_obj : p_dependant_objects)
	{
		if (dependant_obj == p_obj)
			return true;
	}
	return false;
}
