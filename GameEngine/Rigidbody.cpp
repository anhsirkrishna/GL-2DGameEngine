#include "Rigidbody.h"
#include "InputManager.h"
#include "GameObject.h"

#include <SDL.h>
#include <glm.hpp>

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

//Sets the new velocity value
void Rigidbody::SetVelocity(glm::vec4 new_velocity)
{
	velocity = new_velocity;
}

/* Update the velocity according to the given paramaters
* arg 1: index 0 or 1 corresponding to x or y axis for glm vec
* arg 2: add (1) or subtract (-1)
* arg 3: use force or friction
* arg 4:dt that should be calculated using the frame rate controller
*/
void Rigidbody::UpdateVelocity(int coord_axis_index, int add_or_sub,
					   std::string forceType, float delta_time)
{
	if (forceType == "FORCE") {
		velocity[coord_axis_index] += add_or_sub * (force / mass) * delta_time;
	}
	else {
		velocity[coord_axis_index] += add_or_sub * (friction / mass) * delta_time;
	}
	
	glm::vec4 pos = p_owner_transform->GetPosition();

	pos.x += velocity.x * delta_time;
	pos.y += velocity.y * delta_time;

	p_owner_transform->SetPosition(pos);
	/*new_position.x += velocity.x * delta_time;
	new_position.y += velocity.y * delta_time;*/
}

void Rigidbody::Link()
{
	p_owner_transform = static_cast<Transform*>(GetOwner()->HasComponent("TRANSFORM"));
}


