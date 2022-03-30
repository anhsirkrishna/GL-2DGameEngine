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
#include "MemoryManager.h"

ComponentFactory::ComponentFactory() {
	creator_map.insert(std::pair<std::string, ComponentCreator*>("TRANSFORM", new TransformCreator));
	creator_map.insert(std::pair<std::string, ComponentCreator*>("GLQUAD", new GLQuadCreator));
	creator_map.insert(std::pair<std::string, ComponentCreator*>("COLLIDER", new ColliderCreator));
	creator_map.insert(std::pair<std::string, ComponentCreator*>("MOVEMENT", new MovementCreator));
	creator_map.insert(std::pair<std::string, ComponentCreator*>("CONTROLLER", new ControllerCreator));
	creator_map.insert(std::pair<std::string, ComponentCreator*>("ANIMATION", new AnimationCreator));
	creator_map.insert(std::pair<std::string, ComponentCreator*>("PARTICLE_EFFECT", new ParticleEffectCreator));
	creator_map.insert(std::pair<std::string, ComponentCreator*>("TILEMAP", new TilemapCreator));
	creator_map.insert(std::pair<std::string, ComponentCreator*>("BEHAVIOR", new BehaviorCreator));
	creator_map.insert(std::pair<std::string, ComponentCreator*>("PROJECTILE", new ProjectileCreator));
	creator_map.insert(std::pair<std::string, ComponentCreator*>("HITBOX", new HitboxCreator));
	creator_map.insert(std::pair<std::string, ComponentCreator*>("HURTBOX", new HurtboxCreator));
	creator_map.insert(std::pair<std::string, ComponentCreator*>("CAMERA_CONTROLLER", new CameraControllerCreator));
	creator_map.insert(std::pair<std::string, ComponentCreator*>("DEPENDANT_OBJECTS", new DependantObjectsCreator));
	creator_map.insert(std::pair<std::string, ComponentCreator*>("BUTTON", new UIButtonCreator));
	creator_map.insert(std::pair<std::string, ComponentCreator*>("UIPANEL", new UIPanelCreator));
	creator_map.insert(std::pair<std::string, ComponentCreator*>("UITEXT", new UITextCreator));
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