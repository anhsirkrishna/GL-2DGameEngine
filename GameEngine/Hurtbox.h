/******************************************************************************/
/* !
/* File   Hurtbox.h
/* Author Krishna Pillai
/* Email: krishna.pillai@digipen.edu
/* Date   02/28/2022
/* Hurtbox Component header file
/* DigiPen Institute of Technology © 2022
/******************************************************************************/

#pragma once

#include "Component.h"

#include <glm.hpp>

class Transform;

class Hurtbox : public Component {
private:
	glm::vec2 offset;
	glm::vec2 dimensions;
	Transform* p_owner_transform;
	bool enabled;
public:
	Hurtbox();

	/*Inits a Hurtbox component
	* Expects a dict with the following key:values
	* offset : float[2]
	* dimensions : float[2]
	* Returns : void
	*/
	void Serialize(json json_object);

	/*Update the component
	* moves to the offset
	* checks owner tranform scale
	*/
	virtual void Update();

	virtual void Link();

	/*Returns a vec4 containing the position,
	* width and height
	* Returns : vec4
	*/
	glm::vec4 GetHurtboxPos();

	/*Handles events 
	* Returns: void
	*/
	virtual void HandleEvent(TimedEvent* p_event);

	/*Resets the hurtbox*/
	virtual void Reset();
	
	//Returns enabled state
	bool IsEnabled();
};