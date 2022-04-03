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
#include "Movement.h"
#include "Camera.h"

#include <SDL.h>
#include <cmath>
#include <iostream>

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

float CameraController::GetFollowObjectPosX(float xOffset) {
	SDL_assert(p_follow_object != nullptr);

	Transform* follow_transform = static_cast<Transform*>(p_follow_object->HasComponent("TRANSFORM"));
	return follow_transform->GetPosCoord(0) + xOffset;
}

float CameraController::GetFollowObjectPosY(float yOffset) {
	SDL_assert(p_follow_object != nullptr);

	Transform* follow_transform = static_cast<Transform*>(p_follow_object->HasComponent("TRANSFORM"));
	return follow_transform->GetPosCoord(1) + yOffset;
}

float CameraController::GetFollowObjectVelX() {
	SDL_assert(p_follow_object != nullptr);

	Movement* follow_movement = static_cast<Movement*>(p_follow_object->HasComponent("MOVEMENT"));
	return follow_movement->GetHorizontalVelocity();
}

float CameraController::GetFollowObjectVelY() {
	SDL_assert(p_follow_object != nullptr);

	Movement* follow_movement = static_cast<Movement*>(p_follow_object->HasComponent("MOVEMENT"));
	return follow_movement->GetVerticalVelocity();
}

float CameraController::GetFollowObjectScaleX() {
	SDL_assert(p_follow_object != nullptr);

	Transform* follow_transform = static_cast<Transform*>(p_follow_object->HasComponent("TRANSFORM"));
	return follow_transform->GetScaleX();
}


void CameraController::SetCameraPos(float x_, float y_, float z_) {
	p_camera->position = glm::vec4(x_, y_, p_camera->position.z, 0.0f);
}

void CameraController::ChangeCameraZ(float forward, float step) {

	if (forward > 0.0f)
		p_camera->position.z -= step;
	else
		p_camera->position.z += step;

	std::cout << p_camera->position.z << std::endl;

}


float lerp(float x, float y, float t) {
	return x * (1.f - t) + y * t;
}


float CameraController::LerpX(float x_, float step)
{

	if (!lerpingX)
	{
		tX = 0.0f;
		start_x = p_camera->position.x;
		goal_x = x_;
		lerpingX = true;

		return lerp(start_x, goal_x, tX);
	}
	else
	{
		tX += step;

		if (tX > 1.0f)
			lerpingX = false;

		return lerp(start_x, x_, tX);
	}
}

float CameraController::LerpY(float y_, float step)
{

	if (!lerpingY)
	{
		tY = 0.0f;
		start_y = p_camera->position.y;
		goal_y = y_;
		lerpingY = true;
		return lerp(start_y, goal_y, tY);
	}
	else
	{
		tY += step;
		if (tY > 1.0f)
			lerpingY = false;

		return lerp(start_y, y_, tY);
	}
}
