#include "OptionsState.h"
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
OptionsState::OptionsState() {
	start_index = p_ui_obj_manager->game_object_list.size();
	GameObjectFactory().CreateUI("Options_UI");
}

/*Deletes the play state by
* clearing the level and cleaning
* up all the objects
*/
OptionsState::~OptionsState() {
}

/*Enters the state from another state
* Doesn't need any special action
* Returns: void
*/
void OptionsState::Enter() {
	SDL_ShowCursor(SDL_ENABLE);
}

/*Basic update call for the OptionsState
* Calls Update on all the gameobjects
* Calls Update on all the required managers
* Returns: void
*/
void OptionsState::Update() {
	p_input_manager->Update();
	p_control_scheme_manager->Update();

	p_lua_manager->Update();
	p_event_manager->Update();

	if (p_input_manager->isKeyTriggered(SDL_SCANCODE_ESCAPE)) {
		p_statestack_manager->Pop();
		return;
	}

	p_ui_obj_manager->Update(start_index);

	if (p_input_manager->isQuit())
		p_game_manager->Quit();
}

/*Calls the render/draw call for all the objects
* Returns: void
*/
void OptionsState::Render() {
	ShaderProgram* p_shader_program = p_graphics_manager->GetActiveShader();
	p_ui_obj_manager->Draw(p_shader_program);
}

/*Exits the state into another state
* Doesn't need any special action
* Returns: void
*/
void OptionsState::Exit() {
	p_statestack_manager->Pop();
}