/******************************************************************************/
/*!
/*File   ComponentCreator.cpp
/*Author Krishna Pillai
/*Email: krishna.pillai@digipen.edu
/*Date   01/27/2022
/*  Component creator implementation file
/* DigiPen Institute of Technology © 2022
/******************************************************************************/

#include "ComponentCreator.h"
#include "GLQuad.h"
#include "Transform.h"
#include "Movement.h"
#include "Collider.h"
#include "Controller.h"
#include "Animation.h"
#include "ParticleEffect.h"
#include "Tilemap.h"

/*Componentcreator create method
* Creates a new instance of the component in question.
* Calls the serialize method on the instance and returns it
*/
Component* ComponentCreator::Create(json json_object) {
	return NULL;
}

/*Componentcreator create method
* Creates a new instance of the component in question.
* Calls the serialize method on the instance and returns it
*/
Component* GLQuadCreator::Create(json json_object) {
	GLQuad* new_glquad = new GLQuad;
	new_glquad->Serialize(json_object);
	return static_cast<Component*>(new_glquad);
}

Component* TransformCreator::Create(json json_object) {
	Transform* new_transform = new Transform;
	new_transform->Serialize(json_object);
	return static_cast<Component*>(new_transform);
}

Component* MovementCreator::Create(json json_object) {
	Movement* new_movement = new Movement;
	new_movement->Serialize(json_object);
	return static_cast<Component*>(new_movement);
}

Component* ColliderCreator::Create(json json_object)
{
	Collider* new_collider = new Collider;
	new_collider->Serialize(json_object);
	return static_cast<Component*>(new_collider);
}

Component* ControllerCreator::Create(json json_object)
{
	Controller* new_controller = new Controller;
	new_controller->Serialize(json_object);
	return static_cast<Component*>(new_controller);
}

Component* AnimationCreator::Create(json json_object) {
	Animation* new_animation = new Animation;
	new_animation->Serialize(json_object);
	return static_cast<Component*>(new_animation);
}

Component* ParticleEffectCreator::Create(json json_object) {
	ParticleEffect* new_particle_effect = new ParticleEffect;
	new_particle_effect->Serialize(json_object);
	return static_cast<Component*>(new_particle_effect);
}

Component* TilemapCreator::Create(json json_object) {
	Tilemap* new_tilemap = new Tilemap;
	new_tilemap->Serialize(json_object);
	return static_cast<Component*>(new_tilemap);
}