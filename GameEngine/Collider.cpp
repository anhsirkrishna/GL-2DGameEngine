#include "Collider.h"
#include "CollisionChecker.h"
#include "GameObjectManager.h"
#include "Transform.h"
#include "Movement.h"
#include "Util.h"

#include <iostream>

Collider::Collider() : Component("COLLIDER"), p_owner_transform(nullptr), 
					   p_owner_movement(nullptr) {}

// Nothing to do for the Collider component
void Collider::Serialize(json json_object) {
}

void Collider::Update()
{
	glm::vec4 pos_0 = p_owner_transform->GetPosition();
	std::vector<GameObject*> game_object_list = p_game_obj_manager->game_object_list;

	// Iterate over all other game objects
	for (auto i = game_object_list.begin(); i < game_object_list.end(); i++) {

		GameObject* other_game_object = *i;

		/* 1. Make sure a game object exists at index i
		   2. The two game objects being compared for collision are not the same
		*/
		if (GetOwner() != other_game_object) {

			glm::vec4 pos_1 = static_cast<Transform*>(other_game_object->HasComponent("TRANSFORM"))->GetPosition();

			// if the two game objects collide
			if (AABB(pos_0, pos_1)) {

				// -------------- Gameplay logic ----------------

				/* This section below is where you can add code for 
				 * desired collision checks (and responses)
				 * The given example is one between the player and another object */

				// ----------------------------------------------

				glm::vec4 vel_0 = p_owner_movement->GetVelocity();

					// Player touches the bottom of a platform
					if (pos_0.y < (pos_1.y + pos_1.w) && ((pos_1.y + pos_1.w) - pos_0.y) < 2) {
						pos_0.y = pos_1.y + pos_0.w + 1; vel_0.y = 0;
					}

					// Player touches the top of a platform
					if ((pos_0.y + pos_0.w) > pos_1.y && ((pos_0.y + pos_0.w) - pos_1.y) < 2) {
						pos_0.y = pos_1.y - pos_0.w - 1; vel_0.y = 0;
					p_owner_movement->SetGravityUsage(false);
					}

					// Player touches the left of the other object
					if ((pos_0.x + pos_0.z) > pos_1.x && ((pos_0.x + pos_0.z) - pos_1.x) < 2) {
						pos_0.x = pos_1.x - pos_0.z - 1; vel_0.x = 0;
					}

					// Player touches the right of the other object
					if (pos_0.x < (pos_1.x + pos_1.z) && ((pos_1.x + pos_1.z) - pos_0.x) < 2) {
						pos_0.x = pos_1.x + pos_0.z + 1; vel_0.x = 0;
					}

					p_owner_transform->SetPosition(pos_0);
				p_owner_movement->SetVelocity(vel_0);

			}
			else {
				if ((pos_0.x + pos_0.z) < pos_1.x ||
					pos_0.x > (pos_1.x + pos_1.z)) {

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
