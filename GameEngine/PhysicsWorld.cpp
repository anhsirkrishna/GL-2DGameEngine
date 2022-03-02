#include "PhysicsWorld.h"
#include "FrameRateController.h"
#include "GameObjectManager.h"
#include <iostream>

glm::vec4 gravity = glm::vec4(0, 200, 0, 0);

/* Load all store all game objects that can be affected by physics
 * Basically, adding them to the physics world */
void PhysicsWorld::Init()
{
	auto game_object_list = p_game_obj_manager->game_object_list;

	for (int i = 0; i < game_object_list.size(); i++) {
		if (game_object_list[i]->HasComponent("COLLIDER")) {
			physics_game_objects.push_back(game_object_list[i]);
		}
	}
}

void PhysicsWorld::Reload() {

	/*for (int i = 0; i < physics_game_objects.size(); i++) {
		delete physics_game_objects[i];
	}*/

	physics_game_objects.clear();
	
	auto game_object_list = p_game_obj_manager->game_object_list;

	for (int i = 0; i < game_object_list.size(); i++) {
		if (game_object_list[i]->HasComponent("COLLIDER")) {
			physics_game_objects.push_back(game_object_list[i]);
		}
	}
}

// Function to add a game object that can be affected by in-game physics
void PhysicsWorld::AddPhysicsGameObject(GameObject* physics_object) {
	physics_game_objects.push_back(physics_object);
}

// Function to remove a game object that in the physics world
void PhysicsWorld::RemovePhysicsGameObject(GameObject* physics_object)
{
	auto it = std::find(physics_game_objects.begin(), physics_game_objects.end(), physics_object);
	if (it != physics_game_objects.end()) {
		physics_game_objects.erase(it);
	}
}

// Euler integration. Update velocities and positions 
void PhysicsWorld::Integrate()
{
	float dt = p_framerate_controller->GetPrevLoopDeltaTime() / 1000.0f;

	for (GameObject* p : physics_game_objects) {
		if (!p->enabled)
			continue;
		if (p->HasComponent("MOVEMENT")) {

			Transform* p_transf = static_cast<Transform*>(p->HasComponent("TRANSFORM"));
			Movement* p_mov = static_cast<Movement*>(p->HasComponent("MOVEMENT"));

			auto p_pos = p_transf->GetPosition();
			auto p_vel = p_mov->GetVelocity();


			// Gravity acts on the body if downward movement is not locked
			if (!p_mov->dirLocks.down_lock) {
				p_vel += gravity * dt;
			}

			// Update pos based on velocity
			p_pos += p_vel * dt;

			p_mov->SetVelocity(p_vel);
			p_transf->SetPosition(p_pos);
		}
	}
}


// Check for AABB collisions between two colliders
bool PhysicsWorld::AABB(Collider* collider_0, Collider* collider_1) {

	auto col_pos_0 = collider_0->GetColliderPosition();
	auto col_pos_1 = collider_1->GetColliderPosition();

	if ((col_pos_0.x - col_pos_0.z) > (col_pos_1.x + col_pos_1.z)) {
		return false;
	}

	if ((col_pos_0.x + col_pos_0.z) < (col_pos_1.x - col_pos_1.z)) {
		return false;
	}

	if ((col_pos_0.y + col_pos_0.w) < (col_pos_1.y - col_pos_1.w)) {
		return false;
	}

	if ((col_pos_0.y - col_pos_0.w) > (col_pos_1.y + col_pos_1.w)) {
		return false;
	}

	return true;
}

// Detect any possible collision between two objects
void PhysicsWorld::DetectAndRecordCollisions()
{
	for (auto i = physics_game_objects.begin(); i != physics_game_objects.end(); i++) {

		/* Only check for a collision with another object
		 * if the game object has a movement component 
		 */
		if ((*i)->HasComponent("MOVEMENT")) {

			for (auto j = physics_game_objects.begin(); j != physics_game_objects.end(); j++) {

				if (*i == *j) {
					continue;
				}
				else {

					Collider* collider_a = static_cast<Collider*>((*i)->HasComponent("COLLIDER"));
					Collider* collider_b = static_cast<Collider*>((*j)->HasComponent("COLLIDER"));

					collider_a->UpdateColliderPosition();
					collider_b->UpdateColliderPosition();

					if (AABB(collider_a, collider_b)) {

						Collision* collision = new Collision();
						collision->collider_a = collider_a;
						collision->collider_b = collider_b;

						collision_list.push_back(collision);
					}
				}
			}
		}
	}
}

void PhysicsWorld::ResolveCollisions()
{
	for (auto& c : collision_list) {

		/* The first game object has the movement component
		/* This is the one whose velocity is going to be reset and 
		 * position (collider position and hence also transform pos) will be updated
		 */
		Movement* mov_a = static_cast<Movement*>(c->collider_a->GetOwner()->HasComponent("MOVEMENT"));

		glm::vec4 vel = mov_a->GetVelocity();

		glm::vec4 col_pos_a = c->collider_a->GetColliderPosition();
		glm::vec4 col_pos_b = c->collider_b->GetColliderPosition();


		/* If the moving object touches the BOTTOM of a platform:
		* If the distance of overlap between the top of the first object and
		* bottom of the second object is less than a certain distance (here 5) -
		* then move the first object a bit below the other object by a small offset
		*/

		if ((col_pos_b.y + col_pos_b.w) > (col_pos_a.y - col_pos_a.w) &&
			(col_pos_b.y + col_pos_b.w) - (col_pos_a.y - col_pos_a.w) <= 10) {

			col_pos_a.y = col_pos_b.y + col_pos_b.w + col_pos_a.w + 0.5f;
			vel.y = 0;

			/* Store a reference to the collider of
			 * the object being touched above (conceptually)
			 * in the moving object's collider. Lock upward movement
			 */
			c->collider_a->colliders_touching.above = c->collider_b;
			mov_a->dirLocks.up_lock = true;
		}

		/* Moving object touches the TOP of a platform - similar logic as previous
		 * This time, disable gravity for this body
		 * Otherwise force of gravity and this collision resolution effect will compete.
		 * This makes the body jitter oddly
		 */

		if ((col_pos_a.y + col_pos_a.w) > (col_pos_b.y - col_pos_b.w) &&
			(col_pos_a.y + col_pos_a.w) - (col_pos_b.y - col_pos_b.w) <= 10) {

			col_pos_a.y = col_pos_b.y - col_pos_b.w - col_pos_a.w - 0.5f;
			vel.y = 0;

			/* Store a reference to the collider of
			 * the object being touched beneath (conceptually)
			 * in the moving object's collider. Lock downward movement
			 */
			c->collider_a->colliders_touching.beneath = c->collider_b;
			mov_a->dirLocks.down_lock = true;
		}

		// Player touches the LEFT of the other object. Similar logic to prev.
		if ((col_pos_a.x + col_pos_a.z) > (col_pos_b.x - col_pos_b.z) &&
			(col_pos_a.x + col_pos_a.z) - (col_pos_b.x - col_pos_b.z) <= 10) {

			col_pos_a.x = col_pos_b.x - col_pos_b.z - col_pos_a.z - 0.5f;
			vel.x = 0;

			/* Store a reference to the collider of
			 * the object being touched on the right (conceptually)
			 * in the moving object's collider. Lock rightward movement
			 */
			c->collider_a->colliders_touching.right = c->collider_b;
			mov_a->dirLocks.right_lock = true;
			
		}

		// Player touches the RIGHT of the other object. Similar logic to prev.
		if ((col_pos_b.x + col_pos_b.z) > (col_pos_a.x - col_pos_a.z) &&
			(col_pos_b.x + col_pos_b.z) - (col_pos_a.x - col_pos_a.z) <= 10) {

			col_pos_a.x = col_pos_b.x + col_pos_b.z + col_pos_a.z + 0.5f;
			vel.x = 0;


			/* Store a reference to the collider of
			 * the object being touched on the left (conceptually)
			 * in the moving object's collider. Lock left movement
			 */
			c->collider_a->colliders_touching.left = c->collider_b;
			mov_a->dirLocks.left_lock = true;
		}

		c->collider_a->SetColliderPosition(col_pos_a);
		c->collider_a->UpdateTransformPosition();

		mov_a->SetVelocity(vel);
	}

	/* Delete all Collision Objects and then clear the Collision Queue.
	 * We are done dealing resolving all collisions
	 */

	for (int i = 0; i < collision_list.size(); i++) {
		delete collision_list[i];
	}

	collision_list.clear();
}

void PhysicsWorld::UnlockMovements() {
	Collider* p_owner_collider;
	Movement* p_owner_movement;

	for (auto i = physics_game_objects.begin(); i != physics_game_objects.end(); i++) {
		p_owner_movement = static_cast<Movement*>((*i)->HasComponent("MOVEMENT"));
		if (p_owner_movement != nullptr) {
			p_owner_collider = static_cast<Collider*>((*i)->HasComponent("COLLIDER"));
			if (p_owner_collider != nullptr) {
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
						p_owner_movement->dirLocks.up_lock = false;
						p_owner_collider->colliders_touching.above = nullptr;
					}
				}

				// If standing on another object
				if (p_owner_collider->colliders_touching.beneath != nullptr) {

					glm::vec4 col_pos_other = p_owner_collider->colliders_touching.beneath->GetColliderPosition();

					/* If far enough above the object OR
					 * left or right of that touching object -  and hence not touching anymore
					 */
					if ((col_pos_other.y - col_pos_other.w) - (col_pos.y + col_pos.w) > 2.0f ||
						((col_pos.x + col_pos.z) < (col_pos_other.x - col_pos_other.z) ||
							(col_pos.x - col_pos.z) > (col_pos_other.x + col_pos_other.z))) {
						// Unlock downward movement. And not touching anything beneath it anymore
						p_owner_movement->dirLocks.down_lock = false;
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
					if ((col_pos_other.x - col_pos_other.z) - (col_pos.x + col_pos.z) > 1.5f ||
						((col_pos.y - col_pos.w) > (col_pos_other.y + col_pos_other.w)) ||
						((col_pos.y + col_pos.w) < (col_pos_other.y - col_pos_other.w))) {

						p_owner_movement->dirLocks.right_lock = false;
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

						p_owner_movement->dirLocks.left_lock = false;
						p_owner_collider->colliders_touching.left = nullptr;
					}
				}
			}
		}
	}
}
