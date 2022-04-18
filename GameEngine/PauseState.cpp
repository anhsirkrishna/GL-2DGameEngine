#include "PauseState.h"
#include "PhysicsWorld.h"
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
#include "GameObjectFactory.h"
#include "AudioManager.h"

#include <SDL.h>


/*Initialize the play state
* Create the level and all the objects
* within the level
*/
PauseState::PauseState() {
	
}

/*Deletes the play state by
* clearing the level and cleaning
* up all the objects
*/
PauseState::~PauseState() {
	p_ui_obj_manager->Cleanup();
}

/*Enters the state from another state
* Doesn't need any special action
* Returns: void
*/
void PauseState::Enter() {
	GameObjectFactory().CreateUI("Pause_menu");
	
}

/*Basic update call for the PauseState
* Calls Update on all the gameobjects
* Calls Update on all the required managers
* Returns: void
*/
void PauseState::Update() {
	p_input_manager->Update();
	p_control_scheme_manager->Update();

	SDL_ShowCursor(SDL_ENABLE);

	p_lua_manager->Update();
	p_event_manager->Update();

	if (p_input_manager->isKeyTriggered(SDL_SCANCODE_ESCAPE)) {
		p_statestack_manager->Pop();
		return;
	}

	p_ui_obj_manager->Update();

	if (p_input_manager->isQuit())
		p_game_manager->Quit();
}

/*Calls the render/draw call for all the objects
* Returns: void
*/
void PauseState::Render() {
	ShaderProgram* p_shader_program = p_graphics_manager->GetActiveShader();
	p_ui_obj_manager->Draw(p_shader_program);
}

/*Exits the state into another state
* Doesn't need any special action
* Returns: void
*/
void PauseState::Exit() {
	
}