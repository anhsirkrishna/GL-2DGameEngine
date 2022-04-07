


function Destroy()
	set_pos_coord(-100000, 1)  --Make sure the sprite is facing right
end


if received_event then
	received_event = false
	if hit_event then
		hit_event = false
		Destroy()
	end
end