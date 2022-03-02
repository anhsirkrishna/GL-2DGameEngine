/******************************************************************************/
/*!
/*File: LevelManager.cpp
/*Author: Brady Menendez
/*Email: brady.m@digipen.edu
/*Date   3/1/2022
/*  Implementation of the LevelManager Class
*
/* DigiPen Institute of Technology © 2022
/******************************************************************************/
#include "LevelManager.h"
#include "GameObjectManager.h"
#include "GameObjectFactory.h"
#include "PhysicsWorld.h"
#include "LuaManager.h"

// loads a level using the gameobject factory and reloads all
// managers that need be.
void LevelManager::LoadLevel(int level) {

	std::string level_file = ".\\Levels\\Level_" + std::to_string(level) + ".json";
	std::ifstream level_data(level_file);

	// missing level file check
	if (!level_data.good())
	{
		level_data.close();
		return;
	}
	level_data.close();

	p_game_obj_manager->Cleanup();
	GameObjectFactory go;
	go.CreateLevel(level);
	current_level = level;
	p_physics_world->Reload();
	p_lua_manager->LoadBehaviorScripts();
}

// reloads the current level
void LevelManager::ReloadLevel() {

	LoadLevel(current_level);
}