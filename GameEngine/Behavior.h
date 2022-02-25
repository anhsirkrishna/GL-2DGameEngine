#pragma once
#pragma once

#include "Component.h"

#include <sol/sol.hpp>


class GLQuad;

class Behavior : public Component {

	std::string script_name;
	sol::state lua_state;
	sol::load_result script_result;

public:

	// default constructor
	Behavior();

	// loads lua script to run on Update()
	void LoadScript();

	// runs the current script once per frame
	void Update();

	//Serialize method. Nothing to do for Transform component.
	void Serialize(json json_object);

	//Links related components
	virtual void Link();
	
};