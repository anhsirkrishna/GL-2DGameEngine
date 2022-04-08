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
#include "EditorState.h"
#include "PauseState.h"

#include <SDL.h>

/*Initialize the play state
* Create the level and all the objects
* within the level
*/
PlayState::PlayState() {
	p_physics_world->Init();
	p_level_manager->LoadLevel(p_game_manager->Level());
}

/*Deletes the play state by
* clearing the level and cleaning
* up all the objects
*/
PlayState::~PlayState() {
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

	p_game_obj_manager->Update();

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
	p_level_manager->ReloadLevel();
}

/*Exits the state into another state
* Doesn't need any special action
* Returns: void
*/
void PlayState::Exit() {
	
}