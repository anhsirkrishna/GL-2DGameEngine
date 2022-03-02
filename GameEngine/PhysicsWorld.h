/******************************************************************************/
/* !
/* File:   PhysicsWorld.h
/* Author: Srey Raychaudhuri
/* Email:  srey.raychaudhuri@digipen.edu
/* Date:   02/25/2022
/* Physics World class encapsulation header file
/* DigiPen Institute of Technology © 2022
/******************************************************************************/

#include "GameObjectManager.h"
#include "Collider.h"
#include "Movement.h"
#include "Transform.h"


// Struct that stores pointers to the two colliding bodies
struct Collision {
	Collider* collider_a;
	Collider* collider_b;
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
	bool AABB(Collider* collider_0, Collider* collider_1);

	// Check for a potential collision
	void DetectAndRecordCollisions();
	
	// Resolve all recorded collisions
	void ResolveCollisions();

	// Reloads physics objects on new level load
	void Reload();
};

extern PhysicsWorld* p_physics_world;
