/******************************************************************************/
/* !
/* File:   PhysicsWorld.h
/* Author: Srey Raychaudhuri
/* Email:  srey.raychaudhuri@digipen.edu
/* Date:   02/25/2022
/* Physics World class encapsulation header file
/* DigiPen Institute of Technology © 2022
/******************************************************************************/
#include <string>
#include <vector>

class Collider;
class Movement;
class GameObject;

enum side {
	BOTTOM = 0,
	TOP,
	LEFT,
	RIGHT,
	NONE
};

/* Struct that stores
   1) pointers to the two colliding bodies 
   2) on which side of the second object it's colliding
   3) penetration depth on collision
*/
struct Collision {
	Collider* collider_a;
	Movement* movement_a;
	Collider* collider_b;
	side side_of_b;
	float penetration_depth;
};

// Info needed to be returned after AABB collision check
struct AABBResult {
	bool colliding;
	side side_of_b;
	float penetration_depth;
};

#pragma once
class PhysicsWorld
{
private:
	// stores all game objects that can be affected by physics
	std::vector<GameObject*> physics_game_objects;
	std::vector<Collision*> collision_list;

public:
	PhysicsWorld() {};
	~PhysicsWorld() {};

	// For loading game objects into the entity list
	void Init();

	// Function to add a physics game object 
	void AddPhysicsGameObject(GameObject* physics_object);
	
	// Function to remove a physics game object
	void RemovePhysicsGameObject(GameObject* physics_object);

	// Function to Update Dynamics
	void Integrate();

	// To check for AABB collision between two given colliders
	AABBResult AABB(Collider* collider_0, Collider* collider_1);

	// Check for a potential collision
	void DetectAndRecordCollisions();
	
	// Resolve all recorded collisions
	void ResolveCollisions();

	// Reloads physics objects on new level load
	void Reload();
};

extern PhysicsWorld* p_physics_world;