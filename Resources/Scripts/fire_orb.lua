--Constants
fire_action = 5
ice_action = 6
earth_action = 7

pressed = 0
triggered = 1
released = 2
inactive = 3

timer = timer + adder

if timer == 60*3 then
	adder = -1
end

if timer == 0 then
	adder = 1
end

--player characters x position. object_index = 0
follow_obj_x = get_dependant_obj_pos_x(0)

--player characters y position. object_index = 0
follow_obj_y = get_dependant_obj_pos_y(0)

--Get the current xpos
x_coord = follow_obj_x + 20
y_coord = follow_obj_y + 30

--Set the fire orb pos to the position + offset
--new_x_coord = x_coord + ((timer - 60*1.5)/ 60)
new_x_coord = x_coord + ((timer - 90) / 16) 

--new_y_coord = y_coord + ((timer - 90) / 7) 
set_pos_coord(new_x_coord, 0)
set_pos_coord(y_coord, 1)

new_z_coord = ((timer - 90) / 180) + 3

set_pos_coord(new_z_coord * adder, 2)

if check_action_state(fire_action, triggered) then
	change_state("FIRE")
elseif check_action_state(ice_action, triggered) then
	change_state("ICE")
elseif check_action_state(earth_action, triggered) then
	change_state("EARTH")
end
