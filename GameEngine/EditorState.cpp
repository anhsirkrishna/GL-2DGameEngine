#include "EditorState.h"
#include "EditorUI.h"
#include "InputManager.h"
#include "ControlSchemeManager.h"
#include "LuaManager.h"
#include "StatestackManager.h"
#include "GameObjectManager.h"

#include <SDL.h>

/*Initialize the play state
* Create the level and all the objects
* within the level
*/
EditorState::EditorState() {
}

/*Deletes the play state by
* clearing the level and cleaning
* up all the objects
*/
EditorState::~EditorState() {
}

/*Enters the state from another state
* Doesn't need any special action
* Returns: void
*/
void EditorState::Enter() {
	p_editor->NewFrame();
	SDL_ShowCursor(SDL_ENABLE);
}

/*Basic update call for the EditorState
* Calls Update on all the gameobjects
* Calls Update on all the required managers
* Returns: void
*/
void EditorState::Update() {
	p_input_manager->Update();
	p_control_scheme_manager->Update();

	if (p_input_manager->isKeyTriggered(SDL_SCANCODE_ESCAPE)) {
		p_statestack_manager->Pop();
		return;
	}

	p_lua_manager->Update();

	p_editor->NewFrame();

	p_game_obj_manager->Update();
}

/*Calls the render/draw call for all the objects
* Returns: void
*/
void EditorState::Render() {
	//ImGui Render
	p_editor->Render();
}

/*Exits the state into another state
* Doesn't need any special action
* Returns: void
*/
void EditorState::Exit() {

}