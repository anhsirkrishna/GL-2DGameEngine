/******************************************************************************/
/*!
/*File   ComponentFactory.h
/*Author Krishna Pillai
/*Email: krishna.pillai@digipen.edu
/*Date   01/27/2022
/*  Component Factory header file
/* DigiPen Institute of Technology © 2022
/******************************************************************************/

#pragma once

#include "ComponentCreator.h"

class ComponentFactory {
public:
	ComponentFactory();
	~ComponentFactory();
	Component* Create(std::string component_name, json json_object);
private:
	std::unordered_map<std::string, ComponentCreator*> creator_map;
};
