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

class GameObject;

class CameraController : public Component {
private:
	GameObject* p_follow_object;
	std::string follow_object_name;
public:
	CameraController();
	virtual void Serialize(json json_object);
	virtual void Link();
	float GetFollowObjectPosX();
	float GetFollowObjectPosY();
	void SetCameraPos(float x_, float y_, float z_);
};