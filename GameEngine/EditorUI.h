/******************************************************************************/
/*!
/*File: Editor.h
/*Author: Brady Menendez
/*Email: brady.m@digipen.edu
/*Date   1/12/2022
/*  Definition of the Editor Class
*
/* DigiPen Institute of Technology © 2022
/******************************************************************************/
#pragma once
typedef struct SDL_Window;
typedef void* SDL_GLContext;
class GameObjectManager;

/*
Custom ImGui Editor class
*/
class Editor
{
public:
	void Init(SDL_Window* window, SDL_GLContext context) const;
	void NewFrame() const;
	void Render();
	void Cleanup() const;
};

extern Editor* p_editor;

