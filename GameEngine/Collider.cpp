#include "Collider.h"
#include "CollisionChecker.h"
#include "GameObjectManager.h"

#include <iostream>

Collider::Collider() : Component("COLLIDER"), p_owner_transform(nullptr), p_owner_rigidbody(nullptr)
{}

void Collider::Update()
{
	glm::vec4 pos_0 = p_owner_transform->GetPosition();

	// Iterate over all other game objects
	for (int i = 0; i < 50; i++) {

		GameObject* other_game_object = p_game_obj_manager->game_object_list[i];

		/* 1. Make sure a game object exists at index i
		   2. The two game objects being compared for collision are not the same
		*/
		if (other_game_object != nullptr && GetOwner() != other_game_object) {

			glm::vec4 pos_1 = static_cast<Transform*>(other_game_object->HasComponent("TRANSFORM"))->GetPosition();

			// if the two game objects collide
			if (Colliding(pos_0, pos_1)) {

				// -------------- Gameplay logic ----------------

				/* This section below is where you can add code for 
				 * desired collision checks (and responses)
				 * The given example is one between the player and another object */

				// ----------------------------------------------

				if (GetOwner()->GetName() == "demo_obj" && other_game_object->GetName() == "demo_obj_2") {

					glm::vec4 vel_0 = p_owner_rigidbody->GetVelocity();

					// Player touches the bottom of a platform
					if (pos_0.y < (pos_1.y + pos_1.w) && ((pos_1.y + pos_1.w) - pos_0.y) < 2) {
						pos_0.y = pos_1.y + pos_0.w + 1; vel_0.y = 0;
					}

					// Player touches the top of a platform
					if ((pos_0.y + pos_0.w) > pos_1.y && ((pos_0.y + pos_0.w) - pos_1.y) < 2) {
						pos_0.y = pos_1.y - pos_0.w - 1; vel_0.y = 0;
						p_owner_rigidbody->SetGravityUsage(false);
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
					p_owner_rigidbody->SetVelocity(vel_0);
				}
			}
			else {
				if (GetOwner()->GetName() == "demo_obj" && (
					(pos_0.x + pos_0.z) < pos_1.x ||
					pos_0.x > (pos_1.x + pos_1.z))) {

					p_owner_rigidbody->SetGravityUsage(true);
				}
			}
		}
	}
}

// Link other components to this one
void Collider::Link()
{
	p_owner_transform = static_cast<Transform*>(GetOwner()->HasComponent("TRANSFORM"));
	p_owner_rigidbody = static_cast<Rigidbody*>(GetOwner()->HasComponent("RIGIDBODY"));
}
