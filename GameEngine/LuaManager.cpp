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
}

// registers player movement functions from the Movement component
void LuaManager::RegObjectFunctions(sol::state& state, GameObject* obj) {

	state.set_function("get_state", &StateManager::GetCurrentState, &obj->state_manager);
	state.set_function("get_prev_state", &StateManager::GetPrevState, &obj->state_manager);
	state.set_function("change_state", &StateManager::ChangeState, &obj->state_manager);
	state.set_function("enable_obj", &GameObject::Enable, obj);
	state.set_function("disable_obj", &GameObject::Disable, obj);

	Component* comp = obj->HasComponent("MOVEMENT");

	if (comp != nullptr) {
		Movement* move = dynamic_cast<Movement*>(comp);

		state.set_function("move", &Movement::MoveHorizontally, move);
		state.set_function("jump", &Movement::Jump, move);
		state.set_function("get_vertical_velocity", &Movement::GetVerticalVelocity, move);
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
}

void LuaManager::RegEvents(sol::state& state, TimedEvent* p_event) {
	if (p_event == nullptr) {
		state["received_event"] = false;
	}
	else {
		state["received_event"] = true;
		switch (p_event->event_id) {
			case EventID::hit:
				state["hit_event"] = true;
				state["hit_direction"] = static_cast<HitEvent*>(p_event)->direction;
			case EventID::impact:
				state["impact_event"] = true;
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

