/******************************************************************************/
/* !
/* File   Projectile.h
/* Author Krishna Pillai
/* Email: krishna.pillai@digipen.edu
/* Date   02/28/2022
/* Projectile Component header file
/* DigiPen Institute of Technology © 2022
/******************************************************************************/
#pragma once

#include "Component.h"
#include <glm.hpp>

class GameObject;
class Transform;

class Projectile : public Component {
private:
	std::unordered_map<std::string, std::vector<GameObject*>> p_instances;
	std::unordered_map<std::string, unsigned int> instance_count;
	std::unordered_map<std::string, unsigned int> last_used_instance;
	std::unordered_map<std::string, glm::vec3> spawn_offset;
	std::unordered_map<std::string, bool> recycle;
	std::string instance_file;
	Transform* p_owner_transform;

	/* Get the index of the instance that was last used
	*  Returns: int
	*/
	int GetLastUsedInstance();
public:
	// default constructor
	Projectile();

	//Link other components
	virtual void Link();

	/*No action required here since serialization is
	* being done during ChangeState()
	* Returns: void
	*/
	void Serialize(json json_object);

	/*Spawns a projectile by enabling one that is 
	* currently disabled in the instance list
	*/
	void Spawn();

	/*Changes the state of the component
	* Checks if state has already been instantiated
	* If it hasn't then it creates game objects for
	* the number of instances of the projectile.
	* Expects a dict with the following key:values
	* "instance_def": "Obj_def" for the projectile object
	* "instance_count": the number of instances
	* Returns: void
	*/
	virtual void ChangeState(json json_object);
};