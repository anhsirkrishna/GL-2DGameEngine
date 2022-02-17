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