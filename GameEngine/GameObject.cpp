#include "GameObject.h"
#include "Component.h"
#include "Events.h"
#include "EventManager.h"

//Creates a named game object
GameObject::GameObject(std::string object_name) : name(object_name), index(0), is_active(true) {
	state_manager.SetOwner(this);
}

GameObject::~GameObject() {
	for (unsigned int i = 0; i < component_list.size(); ++i) {
		delete component_list[i];
	}
	component_list.clear();
}

//Returns the name of the Game object
const std::string& GameObject::GetName() {
	return name;
}

const std::string& GameObject::GetNameRef() const
{
	return name;
}

//Checks if the game object has a particular component. Returns it if present
Component* GameObject::HasComponent(const std::string& component_name) const {
	for (auto component : component_list) {
		if (component->GetName() == component_name)
			return component;
	}

	return NULL;
}

//Adds a component to the list of components of the game object
void GameObject::AddComponent(Component* component) {
	component_list.push_back(component);
	component->SetOwner(this);
}


//Calls the Update() function for each of its components
void GameObject::Update() {
	if (IsActive()) {
		for (auto component : component_list) {
		component->Update();
		}
	}
}

/*
* Calls the Link() function for each of its components
* Should be called only after all the components have been added to the game object
*/
void GameObject::LinkComponents() {
	for (auto component : component_list) {
		component->Link();
	}
}

//Calls the Draw() function for each of its components
void GameObject::Draw(ShaderProgram* program) {
	if (IsActive()) {
		for (auto component : component_list) {
			component->Draw(program);
		}
	}
}

/*Handles an event sent to this object 
* by passing the event onto all its components
* Returns: void
*/
void GameObject::HandleEvent(TimedEvent* p_event) {
	if (IsActive()) {
		for (auto component : component_list) {
			component->HandleEvent(p_event);
		}
	}

	if (p_event->event_id == EventID::disable)
		Disable();
}

//Disable game object
void GameObject::Disable() {
	is_active = false;
}

//Enable game object
void GameObject::Enable() {
	is_active = true;
}

void GameObject::SetActive(bool val)
{
	is_active = val;
}

bool GameObject::IsActive() const
{
	return is_active;
}

void GameObject::ResetComponents() {
	for (auto component : component_list) {
		component->Reset();
	}
}

//Disable game object after a time delay
void GameObject::DelayedDisable(double time_delay) {
	TimedEvent* new_event = new TimedEvent(EventID::disable, false, this);
	new_event->SetTime(time_delay);
	p_event_manager->QueueTimedEvent(new_event);
}


