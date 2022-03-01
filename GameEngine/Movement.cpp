#include "Movement.h"
#include "InputManager.h"
#include "FrameRateController.h"
#include "GameObject.h"
#include "Transform.h"
#include "Collider.h"

#include <SDL.h>
#include <glm.hpp>

Movement::Movement() : Component("MOVEMENT"), p_owner_transform(nullptr), p_owner_collider(nullptr),
					mass(5), velocity(glm::vec4(0)), gravity_on(false), dirLocks({false, false, false, false}) {}

// Returns the mass value
float Movement::GetMass() {
	return mass;
}

// Returns the velocity value
glm::vec4 Movement::GetVelocity()
{
	return velocity;
}

// Sets the new velocity value
void Movement::SetVelocity(glm::vec4 new_velocity)
{
	velocity = new_velocity;
}

// Just check if any directional locks can be disabled if in case enabled
void Movement::Update()
{
	glm::vec4 col_pos = p_owner_collider->GetColliderPosition();


	// If beneath another object
	if (p_owner_collider->colliders_touching.above != nullptr) {

		glm::vec4 col_pos_other = p_owner_collider->colliders_touching.above->GetColliderPosition();

		/* If far enough below the object OR
		 * left or right of that touching object - and hence not touching anymore
		 */
		if ((col_pos.y - col_pos.w) - (col_pos_other.y + col_pos_other.w) > 2.0f ||
		   ((col_pos.x + col_pos.z) < (col_pos_other.x - col_pos_other.z) ||
			(col_pos.x - col_pos.z) > (col_pos_other.x + col_pos_other.z))) {

			// Unlock downward movement. And not touching anything beneath it anymore
			dirLocks.up_lock = false;
			p_owner_collider->colliders_touching.above = nullptr;
		}
	}

	// If standing on another object
	if (p_owner_collider->colliders_touching.beneath != nullptr) {

		glm::vec4 col_pos_other = p_owner_collider->colliders_touching.beneath->GetColliderPosition();

		/* If far enough above the object OR
		 * left or right of that touching object -  and hence not touching anymore 
		 */
		if  ((col_pos_other.y - col_pos_other.w) - (col_pos.y + col_pos.w) > 2.0f ||
			((col_pos.x + col_pos.z) < (col_pos_other.x - col_pos_other.z) ||
			(col_pos.x - col_pos.z) > (col_pos_other.x + col_pos_other.z))) {

			// Unlock downward movement. And not touching anything beneath it anymore
			dirLocks.down_lock = false;
			p_owner_collider->colliders_touching.beneath = nullptr;
		}
	}

	// If touching a body on the right
	if (p_owner_collider->colliders_touching.right != nullptr) {

		glm::vec4 col_pos_other = p_owner_collider->colliders_touching.right->GetColliderPosition();

		/* If far enough left from the object, OR 
		 * above or below the touching object on the right
		 * Release right ward movement lock. (Can move right now)
		 */
		if  ((col_pos_other.x - col_pos_other.z) - (col_pos.x + col_pos.z) > 1.5f ||
			((col_pos.y - col_pos.w) > (col_pos_other.y + col_pos_other.w)) ||
			((col_pos.y + col_pos.w) < (col_pos_other.y - col_pos_other.w))) {

			dirLocks.right_lock = false;
			p_owner_collider->colliders_touching.right = nullptr;
		}
	}

	// If touching a body on the left
	if (p_owner_collider->colliders_touching.left != nullptr) {

		glm::vec4 col_pos_other = p_owner_collider->colliders_touching.left->GetColliderPosition();


		/* If far enough right from the object, OR
         * above or below the touching object on the right
		 * Release leftward movement lock. (Can move left now)
		 */
		if ((col_pos.x - col_pos.z) - (col_pos_other.x + col_pos_other.z) > 1.5f ||
			((col_pos.y - col_pos.w) > (col_pos_other.y + col_pos_other.w)) ||
			((col_pos.y + col_pos.w) < (col_pos_other.y - col_pos_other.w))) {

			dirLocks.left_lock = false;
			p_owner_collider->colliders_touching.left = nullptr;
		}
	}
}


// Sets the x velocity
void Movement::MoveHorizontally(float vel_x)
{
	// trying to move leftward and left lock is disabled and 
	// trying to move rightward and right lock is disabled, then only set the velocity
	if (!(vel_x < 0 && dirLocks.left_lock) && !(vel_x > 0 && dirLocks.right_lock)) {
		velocity.x = vel_x;
	}

}


// Gives a -ve y velocity. Min is -10.0f
void Movement::Jump(float vel_y)
{
	// Has to have a minimum upward velocity of 10.0f;
	if (vel_y > -10.0f) {
		velocity.y = -100.0f;
	}
	else {
		velocity.y = vel_y;
	}

}

// Serialize method. Nothing to do for the movement component
void Movement::Serialize(json json_object)
{
	gravity_on = json_object["gravity_on"].get<bool>();
}

// Stores references to other components
void Movement::Link()
{
	p_owner_transform = static_cast<Transform*>(GetOwner()->HasComponent("TRANSFORM"));
	p_owner_collider = static_cast<Collider*>(GetOwner()->HasComponent("COLLIDER"));
}


