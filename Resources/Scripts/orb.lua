--Constants
attack_action = 3
fire_action = 5
ice_action = 6
earth_action = 7

pressed = 0
triggered = 1
released = 2
inactive = 3

timer_till_can_shoot = 20

timer = timer + adder

if timer == 60*3 then
	adder = -1
end

if timer == 0 then
	adder = 1
end

function ThrowProjectile()
	spawn_projectile()

	if get_state() == "FIRE" then
		play_sound("fireball.wav")
	elseif get_state() == "EARTH" then
		play_sound("dirt.wav")
	elseif get_state() == "ICE" then
		play_sound("shield.wav")
	end


end

--player characters scale. object_index = 0
follow_obj_scale_x = get_dependant_obj_scale_x(0)
follow_obj_scale_y = get_dependant_obj_scale_y(0)
set_transform_scale(follow_obj_scale_x, follow_obj_scale_y)

--player characters x position. object_index = 0
follow_obj_x = get_dependant_obj_pos_x(0)

--player characters y position. object_index = 0
follow_obj_y = get_dependant_obj_pos_y(0)

--Get the current xpos
x_coord = follow_obj_x + 20
y_coord = follow_obj_y + 30

--Move offset a little bit more when character is facing left
if follow_obj_scale_x < 0 then
	x_coord = x_coord + 5
end

--Set the fire orb pos to the position + offset
--new_x_coord = x_coord + ((timer - 60*1.5)/ 60)
new_x_coord = x_coord + ((timer - 90) / 16) 

--new_y_coord = y_coord + ((timer - 90) / 7) 
set_pos_coord(new_x_coord, 0)
set_pos_coord(y_coord, 1)

if check_action_state(fire_action, triggered) then
	--Only enable fire power if powerup is collected
	if power_count > 1 then
		change_state("FIRE")
	end
elseif check_action_state(ice_action, triggered) then
	--Only enable ice power if powerup is collected
	if power_count > 2 then
		change_state("ICE")
	end
elseif check_action_state(earth_action, triggered) then
	--Only enable earth power if powerup is collected
	if power_count > 0 then
		change_state("EARTH")
	end
end

if check_action_state(attack_action, triggered) then
	if get_state() ~= "NONE" then
		--Set the z coord to 0 before spawning projectile 
		--to ensure that the projectile is spawned at z=0
		set_pos_coord(0, 2)
		ThrowProjectile()
	end
end

if received_event then
	received_event = false
	if pickup_event then
		pickup_event = false
		power_count = power_count + 1
		if power_count == 1 then
			change_state("EARTH")
		elseif power_count == 2 then
			change_state("FIRE")
		elseif power_count == 3 then
			change_state("ICE")
		end
	end
end

--zaxis movement
new_z_coord = ((timer - 90) / 180) + 3
set_pos_coord(new_z_coord * adder, 2)