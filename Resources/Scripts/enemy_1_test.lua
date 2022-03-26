walk_distance = 300 -- when player is this close, enemy will react
attack_distance = 150
height_diff = 5
walk_speed = 50
time_between_shots = 60
time_till_can_switch_state = 30

jump_speed = 150

die_event_id = 5

timer = timer + adder
timer_2 = timer_2 + adder

if timer > time_between_shots then 
	timer = 0
end

if timer_2 > time_till_can_switch_state then
	if get_vertical_velocity() == 0 and get_state() == "JUMP" then
		change_state("IDLE")
		timer_2 = 0
	end
end

function Die()
	if get_state() ~= "DIE" then
		change_state("DIE")
		move(0)
		set_transform_scale(1 * hit_direction, 1)  --Make sure the sprite is facing right
		--Send an event id 5, with a delay of 0 ms, with broadcast=false
		send_event(5, 0, false)
	end
end

function Jump()
	if get_state() ~= "JUMP" and get_state() ~= "DIE" then
		change_state("JUMP")
		jump(jump_speed)
	end
end

if get_state() == "ATTACK" then
	if is_animation_completed() == true then
		change_state("IDLE")
	end
end





---- If player is on left of the enemy

-- If player is far enough but not too close, run towards player
if get_dependant_obj_pos_x(0) < get_pos_coord(0) and 
	get_pos_coord(0) - get_dependant_obj_pos_x(0) < walk_distance and
	get_pos_coord(0) - get_dependant_obj_pos_x(0) > attack_distance and get_state() == "IDLE" then
		change_state("WALK")
		set_transform_scale(-1, 1) 
		move(-walk_speed)
end

-- If close enough to player, attack.
if get_dependant_obj_pos_x(0) < get_pos_coord(0) and 
	get_pos_coord(0) - get_dependant_obj_pos_x(0) < attack_distance and
	(get_state() == "IDLE" or  get_state() == "WALK") and timer >= time_between_shots then
		spawn_projectile()
		change_state("ATTACK")
		set_transform_scale(-1, 1) 
		move(0)
end

---- Same as above except when player is on the right
if get_dependant_obj_pos_x(0) > get_pos_coord(0) and 
	get_dependant_obj_pos_x(0) - get_pos_coord(0) < walk_distance and
	get_dependant_obj_pos_x(0) - get_pos_coord(0) > attack_distance and get_state() == "IDLE" then
		change_state("WALK")
		set_transform_scale(1, 1) 
		move(walk_speed)
end

-- If close enough to player, attack.
if get_dependant_obj_pos_x(0) > get_pos_coord(0) and 
	get_dependant_obj_pos_x(0) - get_pos_coord(0) < attack_distance and
	(get_state() == "IDLE" or  get_state() == "WALK") and timer >= time_between_shots then
		spawn_projectile()	
		set_transform_scale(1, 1)
		change_state("ATTACK")
		--spawn_enemy_projectile()
		move(0)
end


if received_event then
	received_event = false
	if hit_event then
		hit_event = false
		Die()
	end
	if jump_event then
		jump_event = false
		Jump()
	end
end