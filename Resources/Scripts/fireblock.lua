


function Destroy()

	play_sound("fireball_hit.wav")

	delayed_disable_obj(0)
end


if received_event then
	received_event = false
	if hit_event then
		hit_event = false
		Destroy()
	end
end