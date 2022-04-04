/******************************************************************************/
/*!
/*File   DependantObjects.h
/*Author Krishna Pillai
/*Email: krishna.pillai@digipen.edu
/*Date   01/27/2022
/*  DependantObjects header file
/* DigiPen Institute of Technology © 2022
/******************************************************************************/

#pragma once
#include "Component.h"

#include <vector>

class GameObject;

class DependantObjects : public Component {
private:
	std::vector<GameObject*> p_dependant_objects;
	std::vector<std::string> dependant_object_names;
public:
	DependantObjects();
	virtual void Serialize(json json_object);
	virtual void Link();
	float GetDependantObjectPosX(int obj_index);
	float GetDependantObjectPosY(int obj_index);
	float GetDependantObjectScaleX(int obj_index);
	float GetDependantObjectScaleY(int obj_index);
	/*Checks if a particular object is part 
	* of the dependant object list
	* Returns: void
	*/
	bool IsObjectDependant(GameObject* dependant_obj);
};