#include "Collider.h"
#include "GameObjectManager.h"
#include "Transform.h"
#include "Movement.h"
#include "Util.h"

#include <iostream>

Collider::Collider() : Component("COLLIDER"), 
					   p_owner_transform(nullptr), 
					   p_owner_movement(nullptr),
					   collider_touching(nullptr),
					   pos_offset(glm::vec4(0)),
					   col_pos(glm::vec4(0)) 
{}


// Check for AABB collisions
// pos_0 and pos_1 are collider positions
bool Collider::AABB(glm::vec4 pos_0, glm::vec4 pos_1) {

	if ((pos_0.x - pos_0.z) > (pos_1.x + pos_1.z)) {
		return false;
	}

	if ((pos_0.x + pos_0.z) < (pos_1.x - pos_1.z)) {
		return false;
	}

	if ((pos_0.y + pos_0.w) < (pos_1.y - pos_1.w)) {
		return false;
	}

	if ((pos_0.y - pos_0.w) > (pos_1.y + pos_1.w)) {
		return false;
	}

	return true;
}


// Nothing to do for the Collider component
void Collider::Serialize(json json_object) {
}

// Set collider offset values
void Collider::SetColliderOffsets(glm::vec4 offsets)
{
	pos_offset = offsets;
}

/* Update transform position after 
 * updating collider positions
 */
void Collider::UpdateTransformPosition()
{
	glm::vec4 new_pos = p_owner_transform->GetPosition();

	new_pos.x = col_pos.x - pos_offset.x - pos_offset.z / 2.0f;
	new_pos.y = col_pos.y - pos_offset.y - pos_offset.w / 2.0f;

	p_owner_transform->SetPosition(new_pos);
}


/* Update collider position
 * values are relative to the Transform position
 */
void Collider::UpdateColliderPosition()
{
	glm::vec4 pos = p_owner_transform->GetPosition();
	
	col_pos.x = pos.x + pos_offset.x + pos_offset.z / 2.0f;
	col_pos.y = pos.y + pos_offset.y + pos_offset.w / 2.0f;
	col_pos.z = pos_offset.z / 2.0f;
	col_pos.w = pos_offset.w / 2.0f;
}


// Get collider position
glm::vec4 Collider::GetColliderPosition()
{
	return col_pos;
}

// Update function for Collider
void Collider::Update()
{
	UpdateColliderPosition();

	// Get the list of game objects
	std::vector<GameObject*> game_object_list = p_game_obj_manager->game_object_list;

	// We want to do collision checks and response only if the owner object has a Movement component
	if (GetOwner()->HasComponent("MOVEMENT")) {

		// Iterate over all other game objects
		for (auto i = game_object_list.begin(); i < game_object_list.end(); i++) {

			GameObject* game_object_other = *i;

			/* 1. Make sure a game object exists at index i
			   2. The two game objects being compared for collision are not the same
			*/
			if (GetOwner() != game_object_other && game_object_other->HasComponent("COLLIDER")) {

				Collider* collider_other = static_cast<Collider*>(game_object_other->HasComponent("COLLIDER"));
				glm::vec4 col_pos_other = collider_other->GetColliderPosition();

				/* If the two game objects collide.
				 * The statements in this block will be executed only if
				 * 1) A collision occurs
				 * 2) The pos of collider of other object has been initialized
				 */

				if (AABB(col_pos, col_pos_other) && 
					col_pos_other != glm::vec4(0)) {

;					/* Get the velocity and position of this game object
					 * Have to modify them
					 */
					glm::vec4 vel_this = p_owner_movement->GetVelocity();


					// Need the position of the game object collided with
					Transform* other_transform = static_cast<Transform*>(game_object_other->HasComponent("TRANSFORM"));
					glm::vec4 pos_other = other_transform->GetPosition();

					/* If the player touches the BOTTOM of a platform:
					* If the distance of overlap between the top of the first object and
					* bottom of the second object is less than a certain distance (here 5) -
					* then move the first object a bit below the other object by a small offset
					*/

					if ((col_pos_other.y + col_pos_other.w) > (col_pos.y - col_pos.w) &&
						(col_pos_other.y + col_pos_other.w) - (col_pos.y - col_pos.w) <= 10) {

						col_pos.y = col_pos_other.y + col_pos_other.w + col_pos.w + 0.5f;
						vel_this.y = 0;
					}

					/* Player touches the TOP of a platform - similar logic as previous
					 * This time, disable gravity for this body
					 * Otherwise force of gravity and this collision resolution effect will compete.
					 * This makes the body jitter oddly
					 */

					if ((col_pos.y + col_pos.w) > (col_pos_other.y - col_pos_other.w) &&
						(col_pos.y + col_pos.w) - (col_pos_other.y - col_pos_other.w) <= 10) {

						col_pos.y = col_pos_other.y - col_pos_other.w - col_pos.w - 0.5f;
						vel_this.y = 0;

						p_owner_movement->SetGravityUsage(false);
					}

					// Player touches the LEFT of the other object. Similar logic to prev.
					if ((col_pos.x + col_pos.z) > (col_pos_other.x - col_pos_other.z) &&
						(col_pos.x + col_pos.z) - (col_pos_other.x - col_pos_other.z) <= 10) {

						col_pos.x = col_pos_other.x - col_pos_other.z - col_pos.z - 0.5f;
						vel_this.x = 0;
					}

					// Player touches the RIGHT of the other object. Similar logic to prev.
					if ((col_pos_other.x + col_pos_other.z) > (col_pos.x - col_pos.z) &&
						(col_pos_other.x + col_pos_other.z) - (col_pos.x - col_pos.z) <= 10) {

						col_pos.x = col_pos_other.x + col_pos_other.z + col_pos.z + 0.5f;
						vel_this.x = 0;
					}

					/* Store a reference to the collider of 
					 * the object being touched (conceptually)
					 */
					collider_touching = collider_other;

					UpdateTransformPosition();
					p_owner_movement->SetVelocity(vel_this);

				}
			}
		}

		/* If gravity is being used. No need for a ref
		 * to the collider being touched. E.g. case when jump is pressed,
		 * not touching the collider anymore */
		if (p_owner_movement->GetGravityUsage()) {
			collider_touching = nullptr;
		}

		// If standing on another object
		if (collider_touching != nullptr) {

			glm::vec4 col_pos_other = collider_touching->GetColliderPosition();

			//If left or right of that touching object -  and hence not touching anymore
			if (((col_pos.x + col_pos.z) < (col_pos_other.x - col_pos_other.z) ||
				(col_pos.x - col_pos.z) > (col_pos_other.x + col_pos_other.z))) {

				p_owner_movement->SetGravityUsage(true);
				collider_touching = nullptr;
			}
		}
	}
}

// Link other components to this one
void Collider::Link()
{
	p_owner_transform = static_cast<Transform*>(GetOwner()->HasComponent("TRANSFORM"));
	p_owner_movement = static_cast<Movement*>(GetOwner()->HasComponent("MOVEMENT"));
}
