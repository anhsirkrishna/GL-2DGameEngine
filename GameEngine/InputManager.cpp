#include "InputManager.h"
#include "memory.h"
#include "Camera.h"
#include <SDL.h>

#include <iostream>

/*
* Default Ctor for InputManager
* Intializes required data structures.
* Intializes the game controller if found
*/
InputManager::InputManager() : quit(false) {

	memset(current_state, 0, 512 * sizeof * current_state);
	memset(prev_state, 0, 512 * sizeof * prev_state);

	memset(current_button_state, 0, SDL_CONTROLLER_BUTTON_MAX * sizeof * current_button_state);
	memset(prev_button_state, 0, SDL_CONTROLLER_BUTTON_MAX * sizeof * prev_button_state);

	memset(current_axis_state, 0, SDL_CONTROLLER_AXIS_MAX * sizeof * current_axis_state);
	memset(prev_axis_state, 0, SDL_CONTROLLER_AXIS_MAX * sizeof * prev_axis_state);

	prev_mouse_state = mouse_state = 0;
}

//Cleanup for Input Manager. Closes game controller if opened.
InputManager::~InputManager() {
	//Close the controller
	SDL_GameControllerClose(p_controller);
}

// To check for controller
void InputManager::CheckForController()
{
	// Check for joysticks
	if (SDL_NumJoysticks() > 0)
	{
		//Load joystick
		p_controller = SDL_GameControllerOpen(0);
		if (p_controller == NULL)
		{
			SDL_Log("Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError());
		}
	}
	else {
		SDL_Log("No game controllers detected.\n");
	}
}


/*
* Calls SDL_PollEvent() which polls events from the queue
* Updates Keyboard and Mouse states
*/
void InputManager::Update() {

	int numberOfItems = 0;
	SDL_Event e;

	while (SDL_PollEvent(&e) != 0)
	{ 
		// Scroll wheel controls (put in a function?)
		if (e.type == SDL_MOUSEWHEEL)
		{
			if (e.wheel.y > 0) // scroll up
			{
				p_camera->ProcessZoom(CameraMovement::CAM_ZOOMIN);
			}
			else if (e.wheel.y < 0) // scroll down
			{
				p_camera->ProcessZoom(CameraMovement::CAM_ZOOMOUT);
			}
		}

		//User requests quit from application window
		if (e.type == SDL_QUIT)
		{
			quit = true;
		}
	}


	Uint8 new_button_state[SDL_CONTROLLER_BUTTON_MAX];
	int new_axis_state[SDL_CONTROLLER_AXIS_MAX];

	// Getting states of controller buttons
	for (Uint8 i = 0; i < SDL_CONTROLLER_BUTTON_MAX; i++) {
		new_button_state[i] = SDL_GameControllerGetButton(p_controller, SDL_GameControllerButton(i));
	}
	memcpy(prev_button_state, current_button_state, SDL_CONTROLLER_BUTTON_MAX * sizeof * current_button_state);
	memcpy(current_button_state, new_button_state, SDL_CONTROLLER_BUTTON_MAX * sizeof * current_button_state);

	// Getting states of controller joystick axes
	int temp;
	for (Uint8 i = 0; i < SDL_CONTROLLER_AXIS_MAX; i++) {
		 temp = SDL_GameControllerGetAxis(p_controller, SDL_GameControllerAxis(i));

		 new_axis_state[i] = (temp < -JOYSTICK_DEAD_ZONE) ? -1 :
			 ((temp > JOYSTICK_DEAD_ZONE) ? 1 : 0);
	}

	memcpy(prev_axis_state, current_axis_state, SDL_CONTROLLER_AXIS_MAX * sizeof * current_axis_state);
	memcpy(current_axis_state, new_axis_state, SDL_CONTROLLER_AXIS_MAX * sizeof * current_axis_state);

	const Uint8* currentKeyStates = SDL_GetKeyboardState(&numberOfItems);
	if (numberOfItems > 512)
		numberOfItems = 512;
	memcpy(prev_state, current_state, 512 * sizeof * current_state);
	memcpy(current_state, currentKeyStates, 512 * sizeof * current_state);

	//Get mouse position
	prev_mouse_state = mouse_state;
	mouse_state = SDL_GetMouseState(&mouse_x, &mouse_y);
}

/*
* Checks if a particular key is being pressed in the current frame
* Returns bool : True if pressed
*/
bool InputManager::isKeyPressed(unsigned int keyScanCode) {
	if (current_state[keyScanCode])
		return true;

	return false;
}

/*
* Checks if a particular key is released in the current frame
* Implying that the key was pressed in the previous frame
* Returns bool : True if released
*/
bool InputManager::isKeyReleased(unsigned int keyScanCode) {
	if (prev_state[keyScanCode] && (current_state[keyScanCode] == 0))
		return true;

	return false;
}

/*
* Checks if a particular key was pressed in the current frame
* Implying that the key was NOT pressed in the previous frame
* Returns bool : True if triggered
*/
bool InputManager::isKeyTriggered(unsigned int keyScanCode) {
	if (prev_state[keyScanCode] == 0 && current_state[keyScanCode])
		return true;

	return false;
}

/*
* Checks if a particular mouse button is being pressed in the current frame
* Returns bool : True if pressed
*/
bool InputManager::isMousePressed(unsigned int mouse_scan_code) {
	if ((mouse_state & mouse_scan_code) != 0)
		return true;

	return false;
}


/*
* Checks if a particular mouse button is released in the current frame
* Implying that the mouse button was pressed in the previous frame
* Returns bool : True if released
*/
bool InputManager::isMouseReleased(unsigned int mouse_scan_code) {
	if ((prev_mouse_state & mouse_scan_code) != 0 && ((mouse_state & mouse_scan_code) == 0))
		return true;

	return false;
}


/*
* Checks if a particular mouse button was pressed in the current frame
* Implying that the mouse button was NOT presssed in the previous frame
* Returns bool : True if triggered
*/
bool InputManager::isMouseTriggered(unsigned int mouse_scan_code) {
	if (((prev_mouse_state & mouse_scan_code) == 0) && ((mouse_state & mouse_scan_code) != 0))
		return true;

	return false;
}

/*
* Checks if user quit the application
* Returns bool : True if quitting
*/
bool InputManager::isQuit() {
	return quit;
}

/*
* Gets the horizontal state of the left analog stick on a controller
* Returns int : -1 if stick is tilted left, 1 if tilted right, 0 if neutral
*/
int InputManager::getLeftStickHorizontal() {
	int horizontal = SDL_GameControllerGetAxis(p_controller, SDL_CONTROLLER_AXIS_LEFTX);
	if (horizontal < -JOYSTICK_DEAD_ZONE)
		return -1;
	else if (horizontal > JOYSTICK_DEAD_ZONE)
		return 1;

	return 0;
}

/*
* Gets the vertical state of the left analog stick on a controller
* Returns int : -1 if stick is tilted down, 1 if tilted up, 0 if neutral
*/
int InputManager::getLeftStickVertical() {
	int vertical = SDL_GameControllerGetAxis(p_controller, SDL_CONTROLLER_AXIS_LEFTY);
	if (vertical < -JOYSTICK_DEAD_ZONE)
		return -1;
	else if (vertical > JOYSTICK_DEAD_ZONE)
		return 1;

	return 0;
}

/*
* Checks if a particular controller button is being pressed in the current frame
* Returns bool : True if pressed
*/
bool InputManager::isControllerButtonPressed(int button_code) {
	if (current_button_state[button_code])
		return true;
		
	return false;
}


/*
* Checks if a particular controller button is being triggered in the current frame
* Returns bool : True if triggered
*/
bool InputManager::isControllerButtonTriggered(int button_code) {
	if (current_button_state[button_code] && !prev_button_state[button_code])
		return true;

	return false;
}

/*
* Checks if a particular controller button is released in the current frame
* Returns bool : True if released
*/
bool InputManager::isControllerButtonReleased(int button_code)
{
	if (!current_button_state[button_code] && prev_button_state[button_code])
		return true;

	return false;

}

/*
* Checks if controller joystick is being released in the current frame
* Returns bool : True if released
*/
bool InputManager::isControllerAxisReleased(int axis_code)
{
	if (prev_axis_state[axis_code] != current_axis_state[axis_code])
		return true;

	return false;
}


// Gets value of the axis state for the joytick in the current frame
int InputManager::getAxisValueAt(int axis_code)
{
	return current_axis_state[axis_code];
}
