#include "Behavior.h"
#include "LuaManager.h"
// default constructor
Behavior::Behavior() : Component("BEHAVIOR") {

}

// loads lua script to run on Update()
void Behavior::LoadScript()
{
	p_lua_manager->RegInputFunctions(lua_state);
	p_lua_manager->RegPlayerFunctions(lua_state, GetOwner());


	std::string file = "..\\Resources\\Scripts\\player_move.lua";

	script_result = lua_state.load_file(file);


}

// runs the current script once per frame
void Behavior::Update()
{
	/*lua_state.script(R"(
			if is_pressed(15) then move(200.0) end
			if is_pressed(13) then move(-200.0) end
			if is_pressed(12) then jump(-200.0) end
			if is_released(15) or is_released(13) then
				set_grav(true)
				move(0.0)
			end
			)");*/

	script_result();

}

//Serialize method. Nothing to do for Transform component.
void Behavior::Serialize(json json_object)
{

}

//Links related components
void Behavior::Link()
{
	
}