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
	* arg 4:dt that should be calculated using the frame rate controller
	*/
	void UpdateVelocity(int coord_axis_index, int add_or_sub, 
				std::string forceType, float delta_time);

	void Link();

private:
	float gravity;
	float force;
	float friction;
	float mass;
	glm::vec4 velocity;
	Transform* p_owner_transform;
};

