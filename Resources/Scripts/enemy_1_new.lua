--walk_distance = 200 -- when player is this close, enemy will react
min_horiz_dist = 150
min_vertical_dist = 100
walk_speed = 50
time_between_shots = 60
time_till_can_switch_state = 40

jump_speed = 200

die_event_id = 6

timer = timer + adder
timer_2 = timer_2 + adder

--if timer > time_between_shots then 
--	timer = 0
--end

if timer_2 > time_till_can_switch_state then
	if get_vertical_velocity() == 0 and get_state() == "JUMP" then
		change_state("WALK")
	end
end

function Die()
	if get_state() ~= "DIE" then
		die_health()
		play_sound("hurt.wav")
		change_state("DIE")
		move(0)
		set_transform_scale(1 * hit_direction, 1)  --Make sure the sprite is facing right
		--Send an event id 5, with a delay of 0 ms, with broadcast=false
		send_event(die_event_id, 0, false)
	end
end

function Jump()
	if get_state() ~= "JUMP" and get_state() ~= "DIE" then
		play_sound("jump.wav")
		timer_2 = 0
		change_state("JUMP")
		jump(jump_speed)
	end
end

if get_state() == "ATTACK" then
	if is_animation_completed() == true then
		--play_sound("fireball.wav")
		change_state("IDLE")
	end
end


-- first check if the vertical dist between the player and an enemy is small enough
if (get_dependant_obj_pos_y(0) > get_pos_coord(1) and get_dependant_obj_pos_y(0) - get_pos_coord(1) < min_vertical_dist) or 
   (get_pos_coord(1) > get_dependant_obj_pos_y(0) and get_pos_coord(1) - get_dependant_obj_pos_y(0) < min_vertical_dist) then


	-- If player is on LEFT of the enemy and close enough to player, attack.
	if get_dependant_obj_pos_x(0) < get_pos_coord(0) and 
		get_pos_coord(0) - get_dependant_obj_pos_x(0) < min_horiz_dist and
		get_state() == "IDLE" and timer >= time_between_shots then
			spawn_projectile()
			timer = 0
			change_state("ATTACK")
			set_transform_scale(-1, 1) 
			move(0)
	end

	-- -- If player is on RIGHT of the enemy and close enough to player, attack.
	if get_dependant_obj_pos_x(0) > get_pos_coord(0) and 
		get_dependant_obj_pos_x(0) - get_pos_coord(0) < min_horiz_dist and
		(get_state() == "IDLE") and timer >= time_between_shots then
			spawn_projectile()	
			timer = 0
			set_transform_scale(1, 1)
			change_state("ATTACK")
			move(0)
	end
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