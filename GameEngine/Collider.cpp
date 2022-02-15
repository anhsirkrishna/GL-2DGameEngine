#include "Collider.h"
#include "GameObjectManager.h"
#include "Transform.h"
#include "Movement.h"
#include "Util.h"

#include <iostream>

Collider::Collider() : Component("COLLIDER"), 
					   p_owner_transform(nullptr), 
					   p_owner_movement(nullptr), 
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


/* Update collider position
 * values are relative to the Transform position
 */
void Collider::UpdateColliderPosition()
{
	glm::vec4 pos = p_owner_transform->GetPosition();
	
	/* Set this for now. 
	 * Collider pos can be relative to transform in any way you want
	*/
	col_pos.x = pos.x + (pos.z / 2.0f);
	col_pos.y = pos.y + (pos.w / 2.0f);
	col_pos.z = pos.z / 2.0f;
	col_pos.w = pos.w / 2.0f;
}


// Get collider position
glm::vec4 Collider::GetColliderPosition()
{
	return col_pos;
}

// Update function for Collider
void Collider::Update()
{
	// Update collider pos
	UpdateColliderPosition();

	std::vector<GameObject*> game_object_list = p_game_obj_manager->game_object_list;

	// Iterate over all other game objects
	for (auto i = game_object_list.begin(); i < game_object_list.end(); i++) {

		GameObject* game_object_other = *i;

		/* 1. Make sure a game object exists at index i
		   2. The two game objects being compared for collision are not the same
		*/
		if (GetOwner() != game_object_other && game_object_other->HasComponent("COLLIDER")) {

			//glm::vec4 pos_1 = static_cast<Transform*>(other_game_object->HasComponent("TRANSFORM"))->GetPosition();
			glm::vec4 col_pos_other = static_cast<Collider*>(game_object_other->HasComponent("COLLIDER"))->GetColliderPosition();

			/* If the two game objects collide.
			 * The statements in this block will be executed only if
			 * 1) A collision occurs
			 * 2) The body collided with has a Movement Component and
			 * 3) The pos of collider of other object has been initialized
			 */

			if (AABB(col_pos, col_pos_other) && 
				GetOwner()->HasComponent("MOVEMENT") && 
				col_pos_other != glm::vec4(0)) {

				/* Get the velocity and position of this game object
				 * Have to modify them
				 */
				glm::vec4 vel_this = p_owner_movement->GetVelocity();
				glm::vec4 pos_this = p_owner_transform->GetPosition();

				// Need the position of the game object collided with
				Transform* other_transform = static_cast<Transform*>(game_object_other->HasComponent("TRANSFORM"));

				glm::vec4 pos_other = other_transform->GetPosition();

					/* If the player touches the BOTTOM of a platform:
					* If the distance of overlap between the top of the first object and 
					* bottom of the second object is less than a certain distance (here 5) - 
					* then move the first object a bit below the other object by a small offset
					*/

					if ((col_pos_other.y + col_pos_other.w) > (col_pos.y - col_pos.w) &&
						(col_pos_other.y + col_pos_other.w) - (col_pos.y - col_pos.w) <= 5) {

						pos_this.y = pos_other.y + pos_other.w + 0.5f;
						vel_this.y = 0;
					}

					/* Player touches the TOP of a platform - similar logic as previous
					 * This time, disable gravity for this body
					 * Otherwise force of gravity and this collision resolution effect will compete.
					 * This makes the body jitter oddly
					 */

					if ((col_pos.y + col_pos.w) > (col_pos_other.y - col_pos_other.w) &&
						(col_pos.y + col_pos.w) - (col_pos_other.y - col_pos_other.w) <= 5) {

						pos_this.y = pos_other.y - pos_this.w - 0.5f;
						vel_this.y = 0;

						p_owner_movement->SetGravityUsage(false);
					}

					// Player touches the LEFT of the other object. Similar logic to prev.
					if ((col_pos.x + col_pos.z) > (col_pos_other.x - col_pos_other.z) &&
						(col_pos.x + col_pos.z) - (col_pos_other.x - col_pos_other.z) <= 5) {

						pos_this.x = pos_other.x - pos_this.z - 0.5f;
						vel_this.x = 0;
					}
					
					// Player touches the RIGHT of the other object. Similar logic to prev.
					if ((col_pos_other.x + col_pos_other.z) > (col_pos.x - col_pos.z) &&
						(col_pos_other.x + col_pos_other.z) - (col_pos.x - col_pos.z) <= 5) {

						pos_this.x = pos_other.x + pos_other.z + 0.5f;
						vel_this.x = 0;
					}

					p_owner_transform->SetPosition(pos_this);
					p_owner_movement->SetVelocity(vel_this);

			}
			else {
				// If this object is left or right of the other object, activate gravity
				if (((col_pos.x + col_pos.z) < (col_pos_other.x - col_pos_other.z) ||
					(col_pos.x - col_pos.z) > (col_pos_other.x + col_pos_other.z)) &&
					GetOwner()->HasComponent("MOVEMENT")) {

					p_owner_movement->SetGravityUsage(true);
				}
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
