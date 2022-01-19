#pragma once

#include "Component.h"

class Rigidbody : public Component {
public:

	//default ctor for Rigidbody
	Rigidbody();

	//Returns the force
	float GetForce();

	//Returns the mass
	float GetMass();

	//Returns the velocity along the x-axis
	float GetVelocityX();

	//Returns the velocity along the y-axis
	float GetVelocityY();

	//Returns the gravity value
	float GetGravity();

	//Sets the velocity along the x-axis
	void SetVelocityX(float vel_x);

	//Sets the velocity along the y-axis
	void SetVelocityY(float vel_y);

private:
	float gravity;
	float force;
	float mass;
	float velocity_x, velocity_y;
};

