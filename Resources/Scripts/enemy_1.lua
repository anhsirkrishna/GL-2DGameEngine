die_event_id = 5

function Die()
	if get_state() ~= "DIE" then
		change_state("DIE")
		set_transform_scale(1 * hit_direction, 1)  --Make sure the sprite is facing right
		--Send an event id 5, with a delay of 0 ms, with broadcast=false
		send_event(5, 0, false)
	end
end

if received_event then
	if hit_event then
		Die()
	end
end

if check_action_state(3, 1) then
	spawn_projectile()
end