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
#include "MemoryManager.h"
#include "Behavior.h"
#include "Projectile.h"
#include "Hitbox.h"
#include "Hurtbox.h"
#include "CameraController.h"
#include "DependantObjects.h"
#include "Button.h"
#include "UIPanel.h"
#include "UIText.h"
#include "Pathfinder.h"
#include "EnemyProjectile.h"
#include "JumpTriggerReceiverBox.h"
#include "JumpTriggerBox.h"
#include "Health.h"

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

Component* BehaviorCreator::Create(json json_object) {
	Behavior* new_behavior = new Behavior;
	new_behavior->Serialize(json_object);
	return static_cast<Component*>(new_behavior);
}

Component* ProjectileCreator::Create(json json_object) {
	Projectile* new_projectile = new Projectile;
	new_projectile->Serialize(json_object);
	return static_cast<Component*>(new_projectile);
}

Component* HitboxCreator::Create(json json_object) {
	Hitbox* new_hitbox = new Hitbox;
	new_hitbox->Serialize(json_object);
	return static_cast<Component*>(new_hitbox);
}

Component* HurtboxCreator::Create(json json_object) {
	Hurtbox* new_hurtbox = new Hurtbox;
	new_hurtbox->Serialize(json_object);
	return static_cast<Component*>(new_hurtbox);
}

Component* CameraControllerCreator::Create(json json_object) {
	CameraController* new_camera_c = new CameraController;
	new_camera_c->Serialize(json_object);
	return static_cast<Component*>(new_camera_c);
}

Component* DependantObjectsCreator::Create(json json_object) {
	DependantObjects* new_dep_o = new DependantObjects;
	new_dep_o->Serialize(json_object);
	return static_cast<Component*>(new_dep_o);
}

Component* UIButtonCreator::Create(json json_object) {
	Button* new_button_obj = new Button;
	new_button_obj->Serialize(json_object);
	return static_cast<Component*>(new_button_obj);
}

Component* UIPanelCreator::Create(json json_object) {
	UIPanel* new_panel_obj = new UIPanel;
	new_panel_obj->Serialize(json_object);
	return static_cast<Component*>(new_panel_obj);
}

Component* UITextCreator::Create(json json_object) {
	UIText* new_text_obj = new UIText;
	new_text_obj->Serialize(json_object);
	return static_cast<Component*>(new_text_obj);
}

Component* PathfinderCreator::Create(json json_object) {
	Pathfinder* new_pf = new Pathfinder;
	new_pf->Serialize(json_object);
	return static_cast<Component*>(new_pf);
}

Component* EnemyProjectileCreator::Create(json json_object)
{
	EnemyProjectile* new_ep = new EnemyProjectile;
	new_ep->Serialize(json_object);
	return static_cast<Component*>(new_ep);
}

Component* JumpTriggerReceiverBoxCreator::Create(json json_object)
{
	JumpTriggerReceiverBox* new_jtrb = new JumpTriggerReceiverBox;
	new_jtrb->Serialize(json_object);
	return static_cast<Component*>(new_jtrb);

}

Component* JumpTriggerBoxCreator::Create(json json_object)
{
	JumpTriggerBox* new_jtb = new JumpTriggerBox;
	new_jtb->Serialize(json_object);
	return static_cast<Component*>(new_jtb);
}

Component* HealthCreator::Create(json json_object)
{
	Health* new_health = new Health;
	new_health->Serialize(json_object);
	return static_cast<Component*>(new_health);
}
