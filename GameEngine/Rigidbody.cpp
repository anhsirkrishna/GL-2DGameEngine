#include "Rigidbody.h"

Rigidbody::Rigidbody() : Component("RIGIDBODY"), gravity(100), friction(250), force(500), mass(5), velocity(glm::vec4(0)) {}

//Returns the force value
float Rigidbody::GetForce(){
	return force;
}

//Returns the mass value
float Rigidbody::GetMass() {
	return mass;
}

//Returns the gravity value
float Rigidbody::GetGravity()
{
	return gravity;
}

//Returns the friction value
float Rigidbody::GetFriction()
{
	return friction;
}

//Returns the velocity value
glm::vec4 Rigidbody::GetVelocity()
{
	return velocity;
}

void Rigidbody::SetVelocity(glm::vec4 new_velocity)
{
	velocity = new_velocity;
}


