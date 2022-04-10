#include "PhysicsWorld.h"
#include "FrameRateController.h"
#include "GameObjectManager.h"
#include "Collider.h"
#include "Movement.h"
#include "Transform.h"
#include <iostream>

glm::vec4 gravity = glm::vec4(0, 200, 0, 0);

/* Load all store all game objects that can be affected by physics
 * Basically, adding them to the physics world */
void PhysicsWorld::Init()
{
	auto game_object_list = p_game_obj_manager->game_object_list;

	for (int i = 0; i < game_object_list.size(); i++) {
		if (game_object_list[i]->HasComponent("COLLIDER") || game_object_list[i]->HasComponent("MOVEMENT")) {
			physics_game_objects.push_back(game_object_list[i]);
		}
	}
}

void PhysicsWorld::Reload() {

	physics_game_objects.clear();
	
	auto game_object_list = p_game_obj_manager->game_object_list;

	for (int i = 0; i < game_object_list.size(); i++) {
		if (game_object_list[i]->HasComponent("COLLIDER") || game_object_list[i]->HasComponent("MOVEMENT")) {
			physics_game_objects.push_back(game_object_list[i]);
		}
	}
}

// Function to add a game object that can be affected by in-game physics
void PhysicsWorld::AddPhysicsGameObject(GameObject* physics_object) {
	if (physics_object->HasComponent("MOVEMENT") || physics_object->HasComponent("COLLIDER")) {
		physics_game_objects.push_back(physics_object);
	}
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

	for (GameObject* p : p_game_obj_manager->game_object_list) {
		if (!p->IsActive())
			continue;
		Movement* p_mov = static_cast<Movement*>(p->HasComponent("MOVEMENT"));
		if (p_mov) {
			Transform* p_transf = p_mov->p_owner_transform;

			auto p_pos = p_transf->GetPosition();
			auto p_vel = p_mov->GetVelocity();

			// Apply gravity
			if (p_mov->GetGravity()) {
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
AABBResult PhysicsWorld::AABB(Collider* collider_0, Collider* collider_1) {

	auto col_pos_0 = collider_0->GetColliderPosition();
	auto col_pos_1 = collider_1->GetColliderPosition();

	std::vector<AABBResult> collision_info;
	float penetration_depth;

	// If col box 0 is on the right of col box 1
	if ((col_pos_0.x - col_pos_0.z) > (col_pos_1.x + col_pos_1.z)) {
		return { false, NONE, FLT_MAX};
	}
	else {
		// Colliding! on the RIGHT of col box 1
		collision_info.push_back({ true, RIGHT, (col_pos_1.x + col_pos_1.z) - (col_pos_0.x - col_pos_0.z)});
	}
		
	// If col box 0 is on the left of col box 1
	if ((col_pos_0.x + col_pos_0.z) < (col_pos_1.x - col_pos_1.z)) {
		return { false, NONE, FLT_MAX};
	}
	else {
		// Colliding! on the LEFT of col box 1
		collision_info.push_back({ true, LEFT, (col_pos_0.x + col_pos_0.z) - (col_pos_1.x - col_pos_1.z)});
	}

	// If col box 0 is above col box 1
	if ((col_pos_0.y + col_pos_0.w) < (col_pos_1.y - col_pos_1.w)) {
		return { false, NONE, FLT_MAX};
	}
	else {
		// Colliding! on the TOP of col box 1
		collision_info.push_back({ true, TOP, (col_pos_0.y + col_pos_0.w) - (col_pos_1.y - col_pos_1.w) });
	}

	// If col box 0 is below col box 1
	if ((col_pos_0.y - col_pos_0.w) > (col_pos_1.y + col_pos_1.w)) {
		return { false, NONE, FLT_MAX};
	}
	else {
		// Colliding! on the BOTTOM of col box 1
		collision_info.push_back({ true, BOTTOM, (col_pos_1.y + col_pos_1.w) - (col_pos_0.y - col_pos_0.w)});
	}

	// Collected info on collisions on all sides. 
	// Store and resolve for the side where the penetration depth is the least
	AABBResult result = { true, NONE, FLT_MAX};
	float min_pen_depth = FLT_MAX;
	for (int i = 0; i < collision_info.size(); i++) {
		if (collision_info[i].penetration_depth < min_pen_depth) {
			min_pen_depth = collision_info[i].penetration_depth;
			result = collision_info[i];
		}
	}

	return result;

}

// Detect any possible collision between two objects
void PhysicsWorld::DetectAndRecordCollisions()
{
	Movement* movement_a;
	Collider* collider_a;
	Collider* collider_b;
	for (auto i = physics_game_objects.begin(); i != physics_game_objects.end(); i++) {

		if (!(*i)->IsActive())
			continue;
		/* Only check for a collision with another object
		 * if the game object has a movement component 
		 */

		movement_a = static_cast<Movement*>((*i)->HasComponent("MOVEMENT"));
		if (movement_a == NULL)
			continue;

		collider_a = static_cast<Collider*>((*i)->HasComponent("COLLIDER"));
		if ( collider_a) {

			if (collider_a->IsEnabled() == false)
				continue;

			collider_a->UpdateColliderPosition();
			for (auto j = physics_game_objects.begin(); j != physics_game_objects.end(); j++) {
				if (!(*j)->IsActive())
					continue;

				if (*i == *j) {
					continue;
				}
				else {
					collider_b = static_cast<Collider*>((*j)->HasComponent("COLLIDER"));
					if (collider_b) {

						if (collider_b->IsEnabled() == false)
							continue;

						AABBResult collision_result = AABB(collider_a, collider_b);

						if (collision_result.colliding) {

							Collision* collision = new Collision();
							collision->collider_a = collider_a;
							collision->movement_a = movement_a;
							collision->collider_b = collider_b;
							collision->penetration_depth = collision_result.penetration_depth;
							collision->side_of_b = collision_result.side_of_b;

							collision_list.push_back(collision);
						}
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
		Movement* mov_a = c->movement_a;
		Transform* transf_a = mov_a->p_owner_transform;

		glm::vec4 vel = mov_a->GetVelocity();

		glm::vec4 col_pos_a = c->collider_a->GetColliderPosition();
		glm::vec4 col_pos_b = c->collider_b->GetColliderPosition();

		float min_depth = std::max(c->penetration_depth - 0.1f, 0.0f);

		// Moving object touches the BOTTOM of the other object
		switch (c->side_of_b)
		{
		case BOTTOM:
			col_pos_a.y = col_pos_b.y + col_pos_b.w + col_pos_a.w + min_depth;
			vel.y = 0;
			break;
		case TOP:
			col_pos_a.y = col_pos_b.y - col_pos_b.w - col_pos_a.w - min_depth;
			vel.y = 0;
			break;
		case LEFT:
			col_pos_a.x = col_pos_b.x - col_pos_b.z - col_pos_a.z - min_depth;
			vel.x = 0;
			break;
		case RIGHT:
			col_pos_a.x = col_pos_b.x + col_pos_b.z + col_pos_a.z + min_depth;
			vel.x = 0;
			break;
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