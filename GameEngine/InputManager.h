#pragma once
#include <SDL_stdinc.h>
#include <SDL_gamecontroller.h>
#include <vector>

//Analog joystick dead zone
const int JOYSTICK_DEAD_ZONE = 8000;

class InputManager {
public:
	/*
	* Default Ctor for InputManager
	* Intializes required data structures.
	* Intializes the game controller if found
	*/
	InputManager();

	//Cleanup for Input Manager. Closes game controller if opened.
	~InputManager();

	/*
	* Calls SDL_PollEvent() which polls events from the queue
	* Updates Keyboard and Mouse states
	*/
	void Update();

	/*
	* Checks if a particular key is being pressed in the current frame
	* Returns bool : True if pressed
	*/
	bool isKeyPressed(unsigned int keyScanCode);

	/*
	* Checks if a particular key is released in the current frame
	* Implying that the key was pressed in the previous frame
	* Returns bool : True if released
	*/
	bool isKeyReleased(unsigned int keyScanCode);

	/*
	* Checks if a particular key was pressed in the current frame
	* Implying that the key was NOT pressed in the previous frame
	* Returns bool : True if triggered
	*/
	bool isKeyTriggered(unsigned int keyScanCode);

	/*
	* Checks if a particular mouse button is being pressed in the current frame
	* Returns bool : True if pressed
	*/
	bool isMouseTriggered(unsigned int mouseScanCode);

	/*
	* Checks if a particular mouse button is released in the current frame
	* Implying that the mouse button was pressed in the previous frame
	* Returns bool : True if released
	*/
	bool isMouseReleased(unsigned int mouseScanCode);

	/*
	* Checks if a particular mouse button was pressed in the current frame
	* Implying that the mouse button was NOT presssed in the previous frame
	* Returns bool : True if triggered
	*/
	bool isMousePressed(unsigned int mouseScanCode);

	/*
	* Checks if user quit the application
	* Returns bool : True if quitting
	*/
	bool isQuit();

	/*
	* Gets the horizontal state of the left analog stick on a controller
	* Returns int : -1 if stick is tilted left, 1 if tilted right, 0 if neutral
	*/
	int getLeftStickHorizontal();

	/*
	* Gets the vertical state of the left analog stick on a controller
	* Returns int : -1 if stick is tilted down, 1 if tilted up, 0 if neutral
	*/
	int getLeftStickVertical();

	/*
	* Checks if a particular controller button is being pressed in the current frame
	* Returns bool : True if pressed
	*/
	bool isControllerButtonPressed(int button_code);

	/*
	* Checks if a particular controller button is being triggered in the current frame
	* Returns bool : True if triggered
	*/
	bool isControllerButtonTriggered(int button_code);

	/*
	* Checks if a particular controller button is being released in the current frame
	* Returns bool : True if released
	*/
	bool isControllerButtonReleased(int button_code);

	///*
	//* Checks if a particular controller joystick is being pushed in the current frame
	//* Returns bool : True if helf
	//*/
	//bool isControllerAxisHeld(int button_code);

	/*
	* Checks if a particular controller joystick is being released in the current frame
	* Returns bool : True if released
	*/
	bool isControllerAxisReleased(int axis_code);

	int getAxisValueAt(int axis_code);

public:
	int mouse_x, mouse_y;
private:
private:
	Uint8 current_state[512];
	Uint8 prev_state[512];

	Uint8 current_button_state[20];
	Uint8 prev_button_state[20];

	int current_axis_state[8];
	int prev_axis_state[8];

	Uint32 mouse_state;
	Uint32 prev_mouse_state;
	SDL_GameController* p_controller;
	bool quit;
};

//extern declaration for use accross the project
extern InputManager* p_input_manager;
