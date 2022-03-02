
function Die()
	if get_state() ~= "DIE" then
		change_state("DIE")
		set_transform_scale(1 * hit_direction, 1)  --Make sure the sprite is facing right
	end
end

if received_event then
	if hit_event then
		Die()
	end
end