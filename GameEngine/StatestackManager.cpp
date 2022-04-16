/******************************************************************************/
/*!
/*File   StatestackManager.cpp
/*Author Krishna Pillai
/*Email: krishna.pillai@digipen.edu
/*Date   01/25/2022
/*  Statestack Manager implementation file
/* DigiPen Institute of Technology © 2022
/******************************************************************************/

#include "StateStackManager.h"
#include "BaseState.h"
#include "GraphicsManager.h"
#include "LoseState.h"
#include "PlayState.h"
#include "FadeOutState.h"
#include "WinState.h"
#include "ConfirmState.h"

void StateStackManager::Update() {
	state_stack.back()->Update();
	while (to_delete.size() > 0)
	{
		delete to_delete.back();
		to_delete.pop_back();
	}
}

void StateStackManager::Render() {
	p_graphics_manager->ClearBuffer(glm::vec4(0.0/255, 0.0/255, 1.0/255, 1));

	for (auto state : state_stack) {
		state->Render();
	}

	p_graphics_manager->SwapBuffers();
}
void StateStackManager::Clear() {
	while (to_delete.size() > 0)
	{
		delete to_delete.back();
		to_delete.pop_back();
	}
	while (state_stack.size() > 0)
	{
		delete state_stack.back();
		state_stack.pop_back();
	}
}

void StateStackManager::Push(BaseState* p_state) {
	state_stack.push_back(p_state);
	state_stack.back()->Enter();
}

void StateStackManager::Pop() {
	BaseState* popped_state = state_stack.back();
	to_delete.push_back(popped_state);
	state_stack.pop_back();
	popped_state->Exit();
}

StateStackManager::~StateStackManager() {
	Clear();
}

void StateStackManager::PushLoseState() {
	Push(new LoseState());
}

void StateStackManager::PushWinState() {
	Push(new WinState());
}

void StateStackManager::PushNewGameState() {
	Push(new PlayState());
}

void StateStackManager::PushNewFadeOutState() {
	Push(new FadeOutState());
}

void StateStackManager::PushConfirmState() {
	Push(new ConfirmState());
}

/*Reset the state at the top of the stack
* Returns void
*/
void StateStackManager::Reset() {
	state_stack.back()->Reset();
}
