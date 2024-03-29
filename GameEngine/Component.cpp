#include "Component.h"

//Constructor that creates a named component
Component::Component(std::string _name) : name(_name), p_owner_object(NULL) {}

//Returns the name of the component
const std::string& Component::GetName() {
	return name;
}

//Sets a GameObject as the owner of the component instance
void Component::SetOwner(GameObject* owner_object) {
	p_owner_object = owner_object;
}

//Returns a pointer to the owner GameObject
GameObject* Component::GetOwner() {
	return p_owner_object;
}

//Base Update method. Does nothing.
void Component::Update() {

}

//Base Draw method. Does nothing.
void Component::Draw(ShaderProgram* p_program) {

}

/*Changes the state of the component
* Individuals components have logic to do this
* Returns: void
*/
void Component::ChangeState(json json_object) {

}

//Base Link method. Does nothing.
//Individual components will have logic that links itself with other dependant components
void Component::Link() {

}

/*Handles the event sent to this component
* Individuals components have logic to do this
* Returns: void
*/
void Component::HandleEvent(TimedEvent* p_event) {

}

/*Resets the components attributes to its default values
* Individuals components have logic to do this
* Returns: void
*/
void Component::Reset() {

}