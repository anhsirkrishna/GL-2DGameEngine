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

class GameObject;
class Transform;

class Projectile : public Component {
private:
	std::vector<GameObject*> p_instances;
	std::string instance_file;
	unsigned int instance_count;
	unsigned int last_used_instance;
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

	/*Serializes this component by creating all the required
	* number of instances of the projectile.
	* Expects a dict with the following key:values
	* "instance_def": "Obj_def" for the projectile object
	* "instance_count": the number of instances
	* Returns: void
	*/
	void Serialize(json json_object);

	/*Spawns a projectile by enabling one that is 
	* currently disabled in the instance list
	*/
	void Spawn();
};