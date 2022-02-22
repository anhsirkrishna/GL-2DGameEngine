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

/*
Custom ImGui Editor class
*/
class Editor
{
public:

	float last_frame_fps;

	void Init() const;
	void NewFrame() const;
	void DebuggerWindow();
	void GameObjectWindow();
	void Render();
	void Cleanup() const;
};

extern Editor* p_editor;

