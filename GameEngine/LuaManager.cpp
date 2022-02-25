#include "LuaManager.h"

#include <iostream>
#include <SDL.h>
#include "InputManager.h"
#include "GameObject.h"
#include "Movement.h"
#include "GameObjectManager.h"
#include "Behavior.h"

void input_checker(SDL_Scancode a)
{
	SDL_Log("AAAAAAAAA\n");
	
	bool test = p_input_manager->isKeyPressed(a);
	SDL_Log(std::to_string(test).c_str());
}

void my_function(bool a, std::string key) {
    // Create a string with the letter 'D' "a" times,
    // append it to 'b'

	if(a)
		SDL_Log(key.c_str());
}

LuaManager::LuaManager() {

	lua_state.set_function("is_pressed", &InputManager::isKeyPressed, p_input_manager);
	lua_state.set_function("is_released", &InputManager::isKeyReleased, p_input_manager);
	lua_state.set_function("boop", &my_function);
	//lua_state.set("obj", p_input_manager);
	//lua_state.set_function("CheckInput", &input_checker);

}

void LuaManager::RegInputFunctions(sol::state& state)
{
	state.set_function("is_pressed", &InputManager::isKeyPressed, p_input_manager);
	state.set_function("is_released", &InputManager::isKeyReleased, p_input_manager);
}

void LuaManager::RegPlayerFunctions(sol::state& state, GameObject* obj)
{

	Component* move_comp = obj->HasComponent("MOVEMENT");

	if (move_comp != nullptr)
	{
		Movement* move = dynamic_cast<Movement*>(move_comp);

		state.set_function("move", &Movement::MoveHorizontally, move);
		state.set_function("jump", &Movement::Jump, move);
		state.set_function("set_grav", &Movement::SetGravityUsage, move);
	}

}

void LuaManager::LoadBehaviorScripts()
{
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


LuaManager::~LuaManager() {

}

void LuaManager::Update() {

	lua_state.script(R"(
			boop(is_pressed(26), 'W')
			boop(is_pressed(4), 'A')
			boop(is_pressed(22), 'S')
			boop(is_pressed(7), 'D')
			)");

}

void LuaManager::Cleanup() {

}

