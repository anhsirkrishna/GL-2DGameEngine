/******************************************************************************/
/*!
/*File   ComponentFactory.cpp
/*Author Krishna Pillai
/*Email: krishna.pillai@digipen.edu
/*Date   01/27/2022
/*  Component Factory implementation file
/* DigiPen Institute of Technology © 2022
/******************************************************************************/

#include "ComponentFactory.h"

ComponentFactory::ComponentFactory() {
	creator_map.insert(std::pair<std::string, ComponentCreator*>("TRANSFORM", new TransformCreator));
	creator_map.insert(std::pair<std::string, ComponentCreator*>("GLQUAD", new GLQuadCreator));
	creator_map.insert(std::pair<std::string, ComponentCreator*>("COLLIDER", new ColliderCreator));
	creator_map.insert(std::pair<std::string, ComponentCreator*>("MOVEMENT", new MovementCreator));
	creator_map.insert(std::pair<std::string, ComponentCreator*>("CONTROLLER", new ControllerCreator));
}

ComponentFactory::~ComponentFactory() {
	for (std::pair<std::string, ComponentCreator*> element : creator_map)
	{
		delete element.second;
	}
}

Component* ComponentFactory::Create(std::string component_name, json json_object) {
	return creator_map.at(component_name)->Create(json_object);
}