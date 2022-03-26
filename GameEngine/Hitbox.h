/******************************************************************************/
/* !
/* File   Hitbox.h
/* Author Krishna Pillai
/* Email: krishna.pillai@digipen.edu
/* Date   02/28/2022
/* Hitbox Component header file
/* DigiPen Institute of Technology © 2022
/******************************************************************************/

#pragma once

#include "Component.h"

#include <glm.hpp>

class Transform;

class Hitbox : public Component {
private:
	glm::vec2 offset;
	glm::vec2 dimensions;
	Transform* p_owner_transform;
	bool enabled;
	float timer_till_enabled;
	bool CheckCollision(glm::vec4 curr_position, GameObject*& collided_object);
public:
	Hitbox();

	/*Inits a hitbox component
	* Expects a dict with the following key:values
	* offset : float[2]
	* dimensions : float[2]
	* Returns : void
	*/
	void Serialize(json json_object);

	/*Update the component
	* moves to the offset 
	* checks owner tranform scale
	* checks collisions with hurtboxes
	* sends hit events
	*/
	virtual void Update();

	virtual void Link();

	void DisableForABit();
};