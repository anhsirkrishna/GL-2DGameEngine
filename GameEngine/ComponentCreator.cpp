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
