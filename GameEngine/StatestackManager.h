/******************************************************************************/
/*!
/*File   StatestackManager.h
/*Author Krishna Pillai
/*Email: krishna.pillai@digipen.edu
/*Date   01/25/2022
/*  Statestack Manager header file
/* DigiPen Institute of Technology © 2022
/******************************************************************************/

#pragma once
#include <vector> 

class BaseState;
class ShaderProgram;

class StateStackManager {
public:
	void Update();
	void Render();
	void Clear();
	void Push(BaseState* p_state);
	void Pop();
	~StateStackManager();
private:
	std::vector<BaseState*> state_stack;
	std::vector<BaseState*> to_delete;
};

extern StateStackManager* p_statestack_manager;