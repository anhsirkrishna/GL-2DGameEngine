#pragma once

#include <string>
#include <vector>
#include "StateManager.h"

//Forward declaration
class Component;
class ShaderProgram;
class TimedEvent;

/*
* A GameObject class.
* Essentially just a list of components. 
* The GameObject itself does NOT define any behavior.
* All behavior needs to be contained in components.
*/
class GameObject {
public:
	//Index keeps track of the index with respect to the list of game objects
	unsigned int index;
	StateManager state_manager;

	//Creates a named game object
	GameObject(std::string object_name);

	~GameObject();
	
	//Returns the name of the Game object
	std::string GetName();

	const std::string& GetNameRef() const;

	//Checks if the game object has a particular component. Returns it if present
	Component* HasComponent(std::string component_name) const;

	//Adds a component to the list of components of the game object
	void AddComponent(Component* component);

	//Calls the Update() function for each of its components
	void Update();

	/*
	* Calls the Link() function for each of its components
	* Should be called only after all the components have been added to the game object
	*/
	void LinkComponents();

	//Calls the Draw() function for each of its components
	void Draw(ShaderProgram* program);

	/*Handles an event sent to this object
	* by passing the event onto all its components
	* Returns: void
	*/
	void HandleEvent(TimedEvent* p_event);

	// is_active getters and setters
	void SetActive(bool val);
	bool IsActive() const;

	//Disable game object
	void Disable();

	//Enable game object
	void Enable();
	
	/*Disable the object after a time delay
	* Returns: void
	*/
	void DelayedDisable(double time_delay);
private:
	std::string name;
	std::vector<Component*> component_list;
	bool is_active;
};