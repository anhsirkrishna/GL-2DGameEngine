/******************************************************************************/
/*!
/*File: LuaManager.cpp
/*Author: Brady Menendez
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

// default constructor
LuaManager::LuaManager() {

}

// registers input functions from the input manager
void LuaManager::RegInputFunctions(sol::state& state) {
	state.set_function("is_pressed", &InputManager::isKeyPressed, p_input_manager);
	state.set_function("is_released", &InputManager::isKeyReleased, p_input_manager);
}

// registers player movement functions from the Movement component
void LuaManager::RegPlayerFunctions(sol::state& state, GameObject* obj) {

	Component* move_comp = obj->HasComponent("MOVEMENT");

	if (move_comp != nullptr)
	{
		Movement* move = dynamic_cast<Movement*>(move_comp);

		state.set_function("move", &Movement::MoveHorizontally, move);
		state.set_function("jump", &Movement::Jump, move);
		state.set_function("set_grav", &Movement::SetGravityUsage, move);
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

