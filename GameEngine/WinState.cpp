#include "WinState.h"
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

#include <SDL.h>


/*Initialize the play state
* Create the level and all the objects
* within the level
*/
WinState::WinState() {
	GameObjectFactory().CreateUI("Win_UI");
}

/*Deletes the play state by
* clearing the level and cleaning
* up all the objects
*/
WinState::~WinState() {
	p_ui_obj_manager->Cleanup();
}

/*Enters the state from another state
* Doesn't need any special action
* Returns: void
*/
void WinState::Enter() {
	SDL_ShowCursor(SDL_ENABLE);
}

/*Basic update call for the WinState
* Calls Update on all the gameobjects
* Calls Update on all the required managers
* Returns: void
*/
void WinState::Update() {
	p_input_manager->Update();
	p_control_scheme_manager->Update();

	p_lua_manager->Update();
	p_event_manager->Update();

	p_ui_obj_manager->Update();

	if (p_input_manager->isQuit())
		p_game_manager->Quit();
}

/*Calls the render/draw call for all the objects
* Returns: void
*/
void WinState::Render() {
	ShaderProgram* p_shader_program = p_graphics_manager->GetActiveShader();
	p_ui_obj_manager->Draw(p_shader_program);
}

/*Exits the state into another state
* Doesn't need any special action
* Returns: void
*/
void WinState::Exit() {
	p_statestack_manager->Reset();
	p_statestack_manager->PushNewFadeOutState();
}