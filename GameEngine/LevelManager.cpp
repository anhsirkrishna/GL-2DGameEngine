/******************************************************************************/
/*!
/*File: LevelManager.cpp
/*Author: Brady Menendez
/*Email: brady.m@digipen.edu
/*Date   3/1/2022
/*  Implementation of the LevelManager Class
*
/* DigiPen Institute of Technology � 2022
/******************************************************************************/
#include "LevelManager.h"
#include "GameObjectManager.h"
#include "GameObjectFactory.h"
#include "PhysicsWorld.h"
#include "LuaManager.h"
#include "Transform.h"
#include <fstream>
#include <algorithm>
#include <iterator>

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
	curr_obj_map = go.CreateLevel(level);
	current_level = level;
	p_physics_world->Reload();
	p_lua_manager->LoadBehaviorScripts();
}

// reloads the current level
void LevelManager::ReloadLevel() {

	LoadLevel(current_level);
}


void LevelManager::SaveSingle() {

	json j_obj_map(curr_obj_map);
	std::string level_file = ".\\Levels\\Level_" + std::to_string(current_level) + ".json";
	std::ofstream o(level_file);
	o << std::setw(4) << j_obj_map << std::endl;
}

// saves the current level
void LevelManager::SaveLevel() {

	for (auto& it : curr_obj_map)
	{
		auto res = std::find_if(p_game_obj_manager->game_object_list.begin(),
			p_game_obj_manager->game_object_list.end(),
			[&](GameObject* x) {return x->GetName() == it.first; });

		if (res != p_game_obj_manager->game_object_list.end())
		{
			Component* trans_comp = (*res)->HasComponent("TRANSFORM");

			if (trans_comp != nullptr)
			{
				Transform* trans_obj = dynamic_cast<Transform*>(trans_comp);

				std::vector<float> f_vector = { trans_obj->GetPosCoord(0),
												trans_obj->GetPosCoord(1),
												trans_obj->GetPosCoord(2),
												trans_obj->GetPosCoord(3)};

				it.second["starting_position"] = json(f_vector);

			}
		}
	}


	json j_obj_map(curr_obj_map);

	std::string level_file = ".\\Levels\\Level_" + std::to_string(current_level) + ".json";

	std::ofstream o(level_file);
	o << std::setw(4) << j_obj_map << std::endl;
}