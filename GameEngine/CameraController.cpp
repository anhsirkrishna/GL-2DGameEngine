/******************************************************************************/
/*!
/*File   CameraController.cpp
/*Author Krishna Pillai
/*Email: krishna.pillai@digipen.edu
/*Date   01/27/2022
/*  CameraController implementation file
/* DigiPen Institute of Technology © 2022
/******************************************************************************/

#include "CameraController.h"
#include "GameObject.h"
#include "GameObjectManager.h"
#include "Transform.h"
#include "Camera.h"

#include <SDL.h>

CameraController::CameraController() : Component("CAMERA_CONTROLLER"), 
		p_follow_object(nullptr), follow_object_name() {}

void CameraController::Serialize(json json_object) {
	follow_object_name = json_object["follow_object_name"].get<std::string>();
}

void CameraController::Link() {
	for (unsigned int i = 0; i < p_game_obj_manager->game_object_list.size(); ++i) {
		if (p_game_obj_manager->game_object_list[i]->GetName() == follow_object_name) {
			p_follow_object = p_game_obj_manager->game_object_list[i];
			return;
		}
	}
}

float CameraController::GetFollowObjectPosX() {
	SDL_assert(p_follow_object != nullptr);

	Transform* follow_transform = static_cast<Transform*>(p_follow_object->HasComponent("TRANSFORM"));
	return follow_transform->GetPosCoord(0);
}

float CameraController::GetFollowObjectPosY() {
	SDL_assert(p_follow_object != nullptr);

	Transform* follow_transform = static_cast<Transform*>(p_follow_object->HasComponent("TRANSFORM"));
	return follow_transform->GetPosCoord(1);
}

void CameraController::SetCameraPos(float x_, float y_, float z_) {
	p_camera->position = glm::vec4(x_, y_, z_, 0.0f);
}