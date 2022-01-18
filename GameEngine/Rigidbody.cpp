#include "Rigidbody.h"

Rigidbody::Rigidbody() : Component("RIGIDBODY"), gravity(10), force(100), mass(5), velocity_x(0), velocity_y(0) {}

//Returns the force value
float Rigidbody::GetForce(){
	return force;
}

//Returns the mass value
float Rigidbody::GetMass() {
	return mass;
}

//Returns the velocity value
float Rigidbody::GetVelocityX()
{
	return velocity_x;
}

float Rigidbody::GetVelocityY()
{
	return velocity_y;
}

float Rigidbody::SetVelocityX(float vel_x)
{
	velocity_x = vel_x;
}

float Rigidbody::SetVelocityY(float vel_y)
{
	velocity_y = vel_y;
}

//Returns the gravity value
float Rigidbody::GetGravity()
{
	return gravity;
}


