walk_distance = 300 -- when player is this close, enemy will react
attack_distance = 150
height_diff = 5
walk_speed = 50
time_between_shots = 60
time_till_can_switch_state = 30

jump_speed = 150

die_event_id = 6

timer = timer + adder
timer_2 = timer_2 + adder


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


if received_event then
	received_event = false
	if hit_event then
		Die()
	end
	if jump_event then
		jump_event = false
		play_sound("jump.wav")
		Jump()
	end
end