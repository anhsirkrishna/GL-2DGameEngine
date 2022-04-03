/******************************************************************************/
/*!
/*File   CameraController.h
/*Author Krishna Pillai
/*Email: krishna.pillai@digipen.edu
/*Date   01/27/2022
/*  CameraController header file
/* DigiPen Institute of Technology © 2022
/******************************************************************************/

#pragma once
#include "Component.h"

#include <glm.hpp>

class GameObject;

class CameraController : public Component {
private:
	GameObject* p_follow_object;
	std::string follow_object_name;
	float tX;
	float tY;
	float start_x;
	float start_y;
	float goal_x;
	float goal_y;
	bool lerpingX = false;
	bool lerpingY = false;
public:
	CameraController();
	virtual void Serialize(json json_object);
	virtual void Link();
	float GetFollowObjectPosX(float xOffset);
	float GetFollowObjectPosY(float yOffset);
	void ChangeCameraZ(float forward, float step);
	void SetCameraPos(float x_, float y_, float z_);
	float LerpX(float x_, float step);
	float LerpY(float y_, float step);
	float GetFollowObjectVelX();
	float GetFollowObjectVelY();
	float GetFollowObjectScaleX();
};