#pragma once

#include "Component.h"

#include <glm.hpp>

class Rigidbody : public Component {
public:

	//default ctor for Rigidbody
	Rigidbody();

	//Returns the force
	float GetForce();

	//Returns the mass
	float GetMass();

	//Returns the gravity value
	float GetGravity();

	//Returns the friction value
	float GetFriction();

	//Returns the velocity
	glm::vec4 GetVelocity();

	//Sets the velocity
	void SetVelocity(glm::vec4 new_velocity);

private:
	float gravity;
	float force;
	float friction;
	float mass;
	glm::vec4 velocity;
};

