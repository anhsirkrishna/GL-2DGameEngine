/******************************************************************************/
/*!
/*File   ComponentCreator.h
/*Author Krishna Pillai
/*Email: krishna.pillai@digipen.edu
/*Date   01/27/2022
/*  Component creator header file
/* DigiPen Institute of Technology © 2022
/******************************************************************************/

#pragma once
#include "Component.h"
#include <nlohmann/json.hpp>

using json = nlohmann::json;

/*Base class for component creator
* Contains a single method called Create()
* that creates and returns an instance of the specified Component
*/
class ComponentCreator {
public:
	virtual Component* Create(json json_object);
};

//Creates instance of GLQuad component
class GLQuadCreator : public ComponentCreator {
public:
	virtual Component* Create(json json_object);
};

//Creates instance of Transform component
class TransformCreator : public ComponentCreator {
public:
	virtual Component* Create(json json_object);
};

// Creates instance of Movement component
class MovementCreator : public ComponentCreator {
public:
	virtual Component* Create(json json_object);
};

// Creates instance of Collider component
class ColliderCreator : public ComponentCreator {
public:
	virtual Component* Create(json json_object);
};

//Creates instance of Controller component
class ControllerCreator: public ComponentCreator {
  public:
	virtual Component* Create(json json_object);
};

//Creates instance of Animation component
class AnimationCreator : public ComponentCreator {
public:
	virtual Component* Create(json json_object);
};

//Creates instance of ParticleEffect component
class ParticleEffectCreator : public ComponentCreator {
public:
	virtual Component* Create(json json_object);
};

//Creates instance of Tilemap component
class TilemapCreator : public ComponentCreator {
public:
	virtual Component* Create(json json_object);
};

//Creates instance of Behavior component
class BehaviorCreator : public ComponentCreator {
public:
	virtual Component* Create(json json_object);
};

//Creates instance of Projectile component
class ProjectileCreator : public ComponentCreator {
public:
	virtual Component* Create(json json_object);
};

//Creates instance of Hitbox component
class HitboxCreator : public ComponentCreator {
public:
	virtual Component* Create(json json_object);
};

//Creates instance of Hurtbox component
class HurtboxCreator : public ComponentCreator {
public:
	virtual Component* Create(json json_object);
};

//Creates instance of CameraController component
class CameraControllerCreator : public ComponentCreator {
public:
	virtual Component* Create(json json_object);
};

//Creates instance of DependentObjects component
class DependantObjectsCreator : public ComponentCreator {
public:
	virtual Component* Create(json json_object);
};

//Creates instance of Button UI component
class UIButtonCreator : public ComponentCreator {
public:
	virtual Component* Create(json json_object);
};

//Creates instance of Panel UI component
class UIPanelCreator : public ComponentCreator {
public:
	virtual Component* Create(json json_object);
};

//Creates instance of Text UI component
class UITextCreator : public ComponentCreator {
public:
	virtual Component* Create(json json_object);
};

// Creates instance of Pathfinder component
class PathfinderCreator : public ComponentCreator {
public:
	virtual Component* Create(json json_object);
};

// Creates instance of JumpTriggerReceiverBox component
class JumpTriggerReceiverBoxCreator : public ComponentCreator {
public:
	virtual Component* Create(json json_object);
};

// Creates instance of JumpTriggerBox component
class JumpTriggerBoxCreator : public ComponentCreator {
public:
	virtual Component* Create(json json_object);
};

// Creates instance of Health component
class HealthCreator : public ComponentCreator {
public:
	virtual Component* Create(json json_object);
};