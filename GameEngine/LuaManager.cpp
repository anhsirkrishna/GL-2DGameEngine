/******************************************************************************/
/*!
/*File: LuaManager.cpp
/*Author: Brady Menendez
*		  Krishna Pillai - Integrate control scheme manager
/*Email: brady.m@digipen.edu
/*Date   2/24/2022
/*  Implementation of the Lua Manager Class
*
/* DigiPen Institute of Technology © 2022
/******************************************************************************/
#include "LuaManager.h"
#include "InputManager.h"
#include "GameObject.h"
#include "Movement.h"
#include "Behavior.h"
#include "GameObjectManager.h"
#include "ControlSchemeManager.h"
#include "StateManager.h"
#include "Transform.h"
#include "Events.h"
#include "Projectile.h"
#include "GraphicsManager.h"
#include "CameraController.h"
#include "Camera.h"
#include "Animation.h"
#include "ParticleEffect.h"
#include "EventManager.h"
#include "DependantObjects.h"
#include "Collider.h"
#include "StatestackManager.h"
#include "Health.h"

#include <SDL.h>

// default constructor
LuaManager::LuaManager() {

}

// registers input functions from the input manager
void LuaManager::RegGlobals(sol::state& state) {
	state.set_function("is_pressed", &InputManager::isKeyPressed, p_input_manager);
	state.set_function("is_released", &InputManager::isKeyReleased, p_input_manager);
	state.set_function("check_action_state", 
		&ControlSchemeManager::CheckActionState, p_control_scheme_manager);
	state.set_function("log_msg", &LuaManager::LogMessage, this);

	state["WINDOW_WIDTH"] = p_graphics_manager->window_width;
	state["WINDOW_HEIGHT"] = p_graphics_manager->window_height;

	state.set_function("clamp_camera", &Camera::ClampCameraPosition, p_camera);
	state.set_function("move_camera", &Camera::ProcessKeyboardInput, p_camera);

	state.set("timer", 0);
	state.set("adder", 1);

	state.set_function("statestack_pop", &StateStackManager::Pop, p_statestack_manager);
	state.set("timer_2", 2);
}

// registers player movement functions from the Movement component
void LuaManager::RegObjectFunctions(sol::state& state, GameObject* obj) {

	state.set_function("get_state", &StateManager::GetCurrentState, &obj->state_manager);
	state.set_function("get_prev_state", &StateManager::GetPrevState, &obj->state_manager);
	state.set_function("change_state", &StateManager::ChangeState, &obj->state_manager);
	state.set_function("enable_obj", &GameObject::Enable, obj);
	state.set_function("disable_obj", &GameObject::Disable, obj);
	state.set_function("delayed_disable_obj", &GameObject::DelayedDisable, obj);

	Component* comp = obj->HasComponent("BEHAVIOR");
	if (comp != nullptr) {
		Behavior* behavior = dynamic_cast<Behavior*>(comp);
		state.set_function("send_event", &Behavior::SendEvent, behavior);
	}

	comp = obj->HasComponent("MOVEMENT");
	if (comp != nullptr) {
		Movement* move = dynamic_cast<Movement*>(comp);

		state.set_function("move", &Movement::MoveHorizontally, move);
		state.set_function("jump", &Movement::Jump, move);
		state.set_function("get_vertical_velocity", &Movement::GetVerticalVelocity, move);
		state.set_function("movement_enable_gravity", &Movement::EnableGravity, move);
		state.set_function("movement_disable_gravity", &Movement::DisableGravity, move);
	}

	comp = obj->HasComponent("COLLIDER");
	if (comp != nullptr) {
		Collider* collider = dynamic_cast<Collider*>(comp);
		state.set_function("collider_enable", &Collider::Enable, collider);
		state.set_function("collider_disable", &Collider::Disable, collider);
		state.set_function("collider_is_enabled", &Collider::IsEnabled, collider);
	}

	comp = obj->HasComponent("TRANSFORM");
	if (comp != nullptr) {
		Transform* transform = dynamic_cast<Transform*>(comp);
		state.set_function("set_transform_scale", &Transform::SetScale, transform);
		state.set_function("get_scale_x", &Transform::GetScaleX, transform);
		state.set_function("set_pos_coord", &Transform::SetPosCoord, transform);
		state.set_function("get_pos_coord", &Transform::GetPosCoord, transform);
	}

	comp = obj->HasComponent("PROJECTILE");
	if (comp != nullptr) {
		Projectile* projectile = dynamic_cast<Projectile*>(comp);
		state.set_function("spawn_projectile", &Projectile::Spawn, projectile);
	}

	comp = obj->HasComponent("CAMERA_CONTROLLER");
	if (comp != nullptr) {
		CameraController* camera_c = dynamic_cast<CameraController*>(comp);
		state.set_function("get_follow_obj_x", 
			&CameraController::GetFollowObjectPosX, camera_c);
		state.set_function("get_follow_obj_y",
			&CameraController::GetFollowObjectPosY, camera_c);
		state.set_function("set_camera_pos",
			&CameraController::SetCameraPos, camera_c);
	}

	comp = obj->HasComponent("ANIMATION");
	if (comp != nullptr) {
		Animation* animation = dynamic_cast<Animation*>(comp);
		state.set_function("is_animation_completed", &Animation::Completed, animation);
	}

	comp = obj->HasComponent("PARTICLE_EFFECT");
	if (comp != nullptr) {
		ParticleEffect* particle_effect = dynamic_cast<ParticleEffect*>(comp);
		state.set_function("particle_burst", &ParticleEffect::BurstEffect, particle_effect);
		state.set_function("reset_particles", &ParticleEffect::ResetParticles, particle_effect);
	}

	comp = obj->HasComponent("DEPENDANT_OBJECTS");
	if (comp != nullptr) {
		DependantObjects* dependant_objects = dynamic_cast<DependantObjects*>(comp);
		state.set_function("get_dependant_obj_pos_x", &DependantObjects::GetDependantObjectPosX, dependant_objects);
		state.set_function("get_dependant_obj_pos_y", &DependantObjects::GetDependantObjectPosY, dependant_objects);
		state.set_function("get_dependant_obj_scale_x", &DependantObjects::GetDependantObjectScaleX, dependant_objects);
		state.set_function("get_dependant_obj_scale_y", &DependantObjects::GetDependantObjectScaleY, dependant_objects);
	}

	//comp = obj->HasComponent("ENEMYPROJECTILE");
	//if (comp != nullptr) {
	//	EnemyProjectile* enemy_projectile = dynamic_cast<EnemyProjectile*>(comp);
	//	state.set_function("spawn_enemy_projectile", &EnemyProjectile::SpawnProjectile, enemy_projectile);
	//}

	comp = obj->HasComponent("HEALTH");
	if (comp != nullptr) {
		Health* health = dynamic_cast<Health*>(comp);
		state.set_function("get_health", &Health::GetHealth, health);
		state.set_function("decr_health", &Health::DecrementHealth, health);
	}
}

void LuaManager::RegEvents(sol::state& state, TimedEvent* p_event) {
	if (p_event == nullptr) {
		state["received_event"] = false;
	}
	else {
		state["received_event"] = true;
		state["hit_event"] = false;
		state["impact_event"] = false;
		state["activate_event"] = false;
		state["jump_event"] = false;
		switch (p_event->event_id) {
			case EventID::hit:
				state["hit_event"] = true;
				state["hit_direction"] = static_cast<HitEvent*>(p_event)->direction;
				break;
			case EventID::impact:
				state["impact_event"] = true;
				break;
			case EventID::activate:
				state["activate_event"] = true;
				break;
			case EventID::jump:
				state["jump_event"] = true;
		}
	}
}



// reads and loads scripts for all gameobjects with the behavior component
void LuaManager::LoadBehaviorScripts() {
	for (auto& obj : p_game_obj_manager->game_object_list)
	{
		Component* bhv_comp = obj->HasComponent("BEHAVIOR");

		if (bhv_comp != nullptr)
		{
			Behavior* move = dynamic_cast<Behavior*>(bhv_comp);
			move->LoadScript();
		}
	}
}

// update
void LuaManager::Update() {

}

// cleanup
void LuaManager::Cleanup() {

}

// deconstructor
LuaManager::~LuaManager() {

}

void LuaManager::LogMessage(std::string log_str) {
	SDL_Log(log_str.c_str());
}