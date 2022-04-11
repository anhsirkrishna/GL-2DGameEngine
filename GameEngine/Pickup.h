/******************************************************************************/
/* !
/* File   Pickup.h
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
class DependantObjects;
class Hurtbox;

class Pickup : public Component {
private:
	glm::vec2 offset;
	glm::vec2 dimensions;
	bool enabled;
	Transform* p_owner_transform;
	DependantObjects* p_owner_dep_obj;
	GameObject* p_pickup_character;
	Hurtbox* p_pickup_character_hurtbox;
public:
	Pickup();

	/*Inits a Pickup component
	* Expects a dict with the following key:values
	* offset : float[2]
	* dimensions : float[2]
	* Returns : void
	*/
	void Serialize(json json_object);

	/*Update the component
	* moves to the offset
	* checks owner tranform scale
	* checks collisions with player character
	* sends pickup events
	*/
	virtual void Update();

	bool CheckCollision(glm::vec4 curr_position);

	virtual void Link();

	virtual void Reset();

	virtual void HandleEvent(TimedEvent* p_event);
};