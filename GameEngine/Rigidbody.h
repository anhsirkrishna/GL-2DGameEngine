/******************************************************************************/
/*!
/*File   Rigidbody.h
/*Author Sreyash Raychaudhuri
/*Email: srey.raychaudhuri@digipen.edu
/*Date   01/27/2022
/*  Rigidbody Component class encapsulation header file
/* DigiPen Institute of Technology © 2022
/******************************************************************************/

#pragma once

#include "Component.h"
#include "Transform.h"

#include <glm.hpp>

class Rigidbody : public Component {
public:

	//default ctor for Rigidbody
	Rigidbody();

	//Returns the force value
	float GetForce();

	//Returns the mass value
	float GetMass();

	//Returns the gravity value
	float GetGravity();

	//Returns the friction value
	float GetFriction();

	//Returns the velocity
	glm::vec4 GetVelocity();

	//Sets the velocity
	void SetVelocity(glm::vec4 new_velocity);

	/* Update the velocity according to the given paramaters
	* arg 1: index 0 or 1 corresponding to x or y axis for glm vec
	* arg 2: add (1) or subtract (-1)
	* arg 3: use force or friction
	*/
	void UpdateVelocity(int coord_axis_index, int add_or_sub, 
				std::string forceType);

	// Update the transform componenent of the owner game object
	void UpdateTransform();

	void Link();

private:
	float gravity;
	float force;
	float friction;
	float mass;
	glm::vec4 velocity;
	Transform* p_owner_transform;
};

