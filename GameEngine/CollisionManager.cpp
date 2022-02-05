#include "CollisionManager.h"
#include "GameObjectManager.h"
#include "Transform.h"
#include "Rigidbody.h"

#include <iostream>

/* To check if two game objects are colliding. (AABB)
 * Returns true if colliding, else returns false
 */
bool CollisionManager::Colliding(glm::vec4 pos_0, glm::vec4 pos_1) {

	if (pos_0.x  > (pos_1.x + pos_1.z)) {
		return false;
	}

	if ((pos_0.x + pos_0.z) < pos_1.x) {
		return false;
	}

	if (pos_0.y < (pos_1.y - pos_1.w)) {
		return false;
	}

	if ((pos_0.y - pos_0.w) > pos_1.y) {
		return false;
	}

	return true;
}

// Function to constantly keep checking for any possible collisions
void CollisionManager::Update() {

	// Iterate over all game objects - for the first game object
	for (int i = 0; i < 50; i++) {
		GameObject* game_object_0 = p_game_obj_manager->game_object_list[i];

		// Make sure a game object exists at index i
		if (game_object_0 != nullptr) {

			glm::vec4 pos_0 = static_cast<Transform*>(game_object_0->HasComponent("TRANSFORM"))->GetPosition();

			// Iterate over all game objects again - this time for the second game object
			for (int j = 0; j < 50; j++) {

				GameObject* game_object_1 = p_game_obj_manager->game_object_list[j];

				/* 1. Make sure a game object exists at index j &
				   2. The two game objects being compared for collision are not the same
				*/
				if (game_object_1 != nullptr && game_object_0 != game_object_1) {

					glm::vec4 pos_1 = static_cast<Transform*>(game_object_1->HasComponent("TRANSFORM"))->GetPosition();

					// if the two game objects collide
					if (Colliding(pos_0, pos_1)) {
							
						/* This section below is where you can add code for desired collision checks (and responses)
						*  The given example is one between the player and another object */
						if (game_object_0->GetName() == "demo_obj" && game_object_1->GetName() == "demo_obj_2") {

							std::cout << "Collided!" << std::endl;
							static_cast<Rigidbody*>(game_object_0->HasComponent("RIGIDBODY"))->SetVelocity(glm::vec4(0));
						}
					}
				}
			}
		}
	}
}
