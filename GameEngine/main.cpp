#include <Windows.h>
#include <SDL.h>
#include <GL\glew.h>
#include <SDL_opengl.h>
#include <GL\GLU.h>
#include <SDL_image.h>

#include "GameDefs.h"
#include "ShaderProgram.h"
#include "GameObjectManager.h"
#include "InputManager.h"
#include "GameManager.h"
#include "Matrix3D.h"
#include "GLQuad.h"
#include "Transform.h"

/*
* Few default global values. These extern variables are declared in GameDefs.h
* so they can be accessed anywhere in the project
*/
unsigned int WORLD_WIDTH = 720;
unsigned int WORLD_HEIGHT = 480;

unsigned int WINDOW_WIDTH = 720;
unsigned int WINDOW_HEIGHT = 480;

unsigned int DEFAULT_FRAMERATE = 60;

/*
* Global managers. These extern variables are decaled in their respective 
* header files so they can be accessed anywhere in the project.
*/
GameObjectManager* p_game_obj_manager;
GameManager* p_game_manager;
InputManager* p_input_manager;

/*
* Global variables to handle SDL window and Open GL Context
*/
SDL_Window* gp_sdl_window;
SDL_GLContext gp_gl_context;

/*
* Macro used to check for OpenGL errors.
* Use this often to ensure all the OpenGL calls go through cleanly 
* without any errors
*/
#define CHECKERROR {GLenum err = glGetError(); if (err != GL_NO_ERROR) { SDL_Log("OpenGL error (at line Main.cpp:%d): %s\n", __LINE__, glewGetErrorString(err));} }


/*
* Function to create all the global managers
*/
void CreateManagers() {
	p_game_obj_manager = new GameObjectManager();
	p_game_manager = new GameManager();
	p_input_manager = new InputManager();
}

/*
* Cleanup the global managers
*/
void DeleteManagers() {
	p_game_obj_manager->Cleanup();
	delete p_game_obj_manager;
	delete p_game_manager;
	delete p_input_manager;
}


/*
* Function to initalize SDL and Open GL and SDL_Image
* Sets the OpenGL Version
* Creates an SDL Window
* Associates an OpenGL Context
* Init GLEW framework
* Init PNG loading using SDL_image
* Returns: bool - True if initialization was a success, else False
*/
bool SDL_GL_Init() {

	int error = 0;
	// Initialize SDL
	if ((error = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER)) < 0)
	{
		SDL_Log("Couldn't initialize SDL, error %i\n", error);
		return false;
	}

	//Use OpenGL 3.1 core
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	gp_sdl_window = SDL_CreateWindow("GameEngine",		// window title
		SDL_WINDOWPOS_UNDEFINED,					// initial x position
		SDL_WINDOWPOS_UNDEFINED,					// initial y position
		WINDOW_WIDTH,								// width, in pixels
		WINDOW_HEIGHT,								// height, in pixels
		SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

	// Check that the window was successfully made
	if (NULL == gp_sdl_window)
	{
		// In the event that the window could not be made...
		SDL_Log("Could not create window: %s\n", SDL_GetError());
		return false;
	}

	//Create context
	gp_gl_context = SDL_GL_CreateContext(gp_sdl_window);
	if (gp_gl_context == NULL)
	{
		SDL_Log("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
		return false;
	}
	else
	{
		//Initialize GLEW
		GLenum glew_error = glewInit();
		if (glew_error != GLEW_OK)
		{
			SDL_Log("Error initializing GLEW! %s\n", glewGetErrorString(glew_error));
			return false;
		}

		//Use Vsync
		if (SDL_GL_SetSwapInterval(1) < 0)
		{
			SDL_Log("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
			return false;
		}

		//Initialize PNG loading
		int img_flags = IMG_INIT_PNG;
		if (!(IMG_Init(img_flags) & img_flags))
		{
			SDL_Log("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
			return false;
		}
	}

	return true;
}

/*
* Function to Close the application
* Destroys the SDL window and quits SDL
* Frees the Console if debug mode
* Returns : void 
*/
void CloseProgram() {
	// Close and destroy the window
	SDL_DestroyWindow(gp_sdl_window);

	// Quit SDL subsystems
	SDL_Quit();

	#ifdef DEBUG
		FreeConsole();
	#endif // DEBUG

}

/*
* Creates a OpenGL Shader program
* Adds individual shaders and compiles them
* Sets the input variables for the shader program
* Returns: Pointer to ShaderProgram Oject 
*/
ShaderProgram* GL_Program_init() {
	//ShaderProgram is a util class to encapsulate common methods and attributes for ShaderPrograms
	ShaderProgram* p_shader_program = new ShaderProgram();
	p_shader_program->AddShader("final.vert", GL_VERTEX_SHADER);
	p_shader_program->AddShader("final.frag", GL_FRAGMENT_SHADER);

	glBindAttribLocation(p_shader_program->program_id, 0, "in_position"); //Attrib location 0 will always be used for position coordinates
	glBindAttribLocation(p_shader_program->program_id, 1, "in_color"); //Attrib location 1 will always be used for colors
	glBindAttribLocation(p_shader_program->program_id, 2, "in_texcoords"); //Attric location 2 will always be used for texture coordinates
	p_shader_program->LinkProgram();
	CHECKERROR;

	return p_shader_program;
}


int main(int argc, char* args[])
{
	#ifdef DEBUG
		//This call creates a console window that SDL can log to for debugging.
		AllocConsole();
	#endif // DEBUG

	//Init SDL and OpenGL
	if (SDL_GL_Init())
		SDL_Log("Initialization Complete");
	else {
		SDL_Log("Initialization Failed");
		return 1;
	}

	//Create all the global managers
	CreateManagers();

	//Create the Shader Program
	ShaderProgram* p_shader_program = GL_Program_init();

	//Temporary bit of code for demo purposes.
	//Clean this up as soon as a serializer is in place
	GameObject* new_game_object = new GameObject("demo_obj");
	GLQuad* new_quad = new GLQuad();
	new_quad->CreateDemo();
	new_game_object->AddComponent(new_quad);
	Transform* new_transform = new Transform();
	new_game_object->AddComponent(new_transform);
	new_game_object->LinkComponents();
	SDL_Rect new_pos;
	p_game_obj_manager->AddGameObject(new_game_object);

	//Main Game loop 
	//The status of the game is maintained by the GameManager
	while (p_game_manager->Status())
	{
		p_game_obj_manager->Update();
		p_input_manager->Update();

		if (p_input_manager->isQuit())
			p_game_manager->Quit();

		//Following lines are test code. Remove ASAP
		if (p_input_manager->getLeftStickHorizontal() != 0) {
			new_pos = new_transform->GetPosition();
			new_pos.x = new_pos.x + (3 * p_input_manager->getLeftStickHorizontal());
			new_transform->SetPosition(new_pos);
		}
		if (p_input_manager->getLeftStickVertical() != 0) {
			new_pos = new_transform->GetPosition();
			new_pos.y = new_pos.y + (3 * p_input_manager->getLeftStickVertical());
			new_transform->SetPosition(new_pos);
		}
		if (p_input_manager->isControllerButtonPressed(SDL_CONTROLLER_BUTTON_A)) {
			new_transform->SetRotation(0);
		}
		if (p_input_manager->isControllerButtonPressed(SDL_CONTROLLER_BUTTON_B)) {
			new_transform->SetRotation(new_transform->GetRotation() + 0.3);
		}
		if (p_input_manager->isControllerButtonPressed(SDL_CONTROLLER_BUTTON_X)) {
			new_transform->SetRotation(new_transform->GetRotation() - 0.3);
		}

		//The following bit of code should be moved into a GameStateManager or and individual game State
		p_shader_program->Use();
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Matrix3D orthoGraphProj = OrthographicProj(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0, 1.0);
		GLuint loc = glGetUniformLocation(p_shader_program->program_id, "orthoGraphProj");
		glUniformMatrix4fv(loc, 1, GL_FALSE, orthoGraphProj.GetMatrixP());

		loc = glGetUniformLocation(p_shader_program->program_id, "mode");
		glUniform1i(loc, 0);

		CHECKERROR;
		//Redraw the scene every frame
		p_game_obj_manager->Draw(p_shader_program);
		CHECKERROR;
		p_shader_program->Unuse();

		SDL_GL_SwapWindow(gp_sdl_window);
	}

	DeleteManagers();
	CloseProgram();
	return 0;
}