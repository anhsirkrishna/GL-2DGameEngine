/******************************************************************************/
/*!
/*File   BaseState.h
/*Author Krishna Pillai
/*Email: krishna.pillai@digipen.edu
/*Date   01/25/2022
/*  BaseState header file
/* DigiPen Institute of Technology © 2022
/******************************************************************************/

#pragma once

class ShaderProgram;

class BaseState {
public:
	virtual ~BaseState() {}
	virtual void Enter() {}
	virtual void Exit() {}
	virtual void Update() {}
	virtual void Render() {}
	virtual void Reset() {}
};