#include "PlayState.h"
#include "PhysicsWorld.h"
#include "LevelManager.h"
#include "EventManager.h"
#include "GameObjectManager.h"
#include "InputManager.h"
#include "ControlSchemeManager.h"
#include "LuaManager.h"
#include "GameManager.h"
#include "GraphicsManager.h"
#include "ShaderProgram.h"
#include "StatestackManager.h"
#include "AudioManager.h"
#include "EditorState.h"
#include "PauseState.h"
#include "Transform.h"
#include "Health.h"

#include <SDL.h>

/*Initialize the play state
* Create the level and all the objects
* within the level
*/
PlayState::PlayState() : player_obj(nullptr) {
	p_physics_world->Init();
	p_level_manager->LoadLevel(p_game_manager->Level());
	for (auto& go : p_game_obj_manager->game_object_list) {
		if (go->GetName() == "player_character") {
			player_obj = go;
			continue;
		}
			
		if (go->GetName().find("enemy") != std::string::npos)
			enemy_obj_health_list.push_back(static_cast<Health*>(go->HasComponent("HEALTH")));

	}

	p_audio_manager->Play("bass.wav");
}

/*Deletes the play state by
* clearing the level and cleaning
* up all the objects
*/
PlayState::~PlayState() {
	enemy_obj_health_list.clear();
	p_event_manager->ClearSubscribeList();
	p_game_obj_manager->Cleanup();
}

/*Enters the state from another state
* Doesn't need any special action
* Returns: void
*/
void PlayState::Enter() {
	
}

/*Basic update call for the PlayState
* Calls Update on all the gameobjects
* Calls Update on all the required managers
* Returns: void
*/
void PlayState::Update() {
	p_input_manager->Update();
	p_control_scheme_manager->Update();

	if (p_input_manager->isKeyTriggered(SDL_SCANCODE_TAB)) {
		p_statestack_manager->Push(new EditorState());
		return;
	}

	if (p_input_manager->isKeyTriggered(SDL_SCANCODE_ESCAPE) || p_input_manager->isControllerButtonTriggered(SDL_CONTROLLER_BUTTON_START)) {
		p_statestack_manager->Push(new PauseState());
		return;
	}

	p_physics_world->Integrate();
	p_physics_world->DetectAndRecordCollisions();
	p_physics_world->ResolveCollisions();
	
	p_lua_manager->Update();
	p_event_manager->Update();
	p_audio_manager->Update();

	p_game_obj_manager->Update();
	
	if (p_input_manager->isKeyTriggered(SDL_SCANCODE_K)) {
		for (auto& enemy_health : enemy_obj_health_list) {
			enemy_health->Die();
		}
	}

	if (p_input_manager->isKeyTriggered(SDL_SCANCODE_PERIOD)) {
		p_event_manager->QueueTimedEvent(new TimedEvent(EventID::pickup, true));
	}

	if (CheckWinCondition()) {
		for (auto& enemy_health : enemy_obj_health_list) {
			enemy_health->Reset();
		}
		p_event_manager->QueueTimedEvent(new TimedEvent(EventID::win, true));
	}

	if (p_input_manager->isQuit())
		p_game_manager->Quit();
}

/*Calls the render/draw call for all the objects
* Returns: void
*/
void PlayState::Render() {
	ShaderProgram* p_shader_program = p_graphics_manager->GetActiveShader();
	p_shader_program->Use();

	//Redraw the scene every frame
	p_game_obj_manager->Draw(p_shader_program);
	p_shader_program->Unuse();

	p_graphics_manager->PostProcess();
	p_graphics_manager->DrawGBuffer();
}

void PlayState::Reset() {
	enemy_obj_health_list.clear();
	p_audio_manager->StopAllSounds();

	p_audio_manager->Play("bass.wav");

	p_level_manager->ReloadLevel();
	for (auto& go : p_game_obj_manager->game_object_list) {
		if (go->GetName() == "player_character")
			player_obj = go;

		if (go->GetName().find("enemy") != std::string::npos)
			enemy_obj_health_list.push_back(static_cast<Health*>(go->HasComponent("HEALTH")));
	}
	Transform* transform_comp = static_cast<Transform*>(player_obj->HasComponent("TRANSFORM"));
	glm::vec4 new_position(68, 200, -1, 0);
	transform_comp->SetPosition(new_position);
}

/*Exits the state into another state
* Doesn't need any special action
* Returns: void
*/
void PlayState::Exit() {
	
}

/*Check if all the enemy objects are dead
* if they are dead then mark as win
* Return : bool
*/
bool PlayState::CheckWinCondition() {
	bool win = true;
	for (auto& enemy_health : enemy_obj_health_list) {
		win &= enemy_health->IsDead();
	}

	return win;
}