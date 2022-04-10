time_till_health_disable = 120

timer = timer + adder

function Die()
	delayed_disable_obj(0)
end

if received_event then
	received_event = false
	if hit_event and timer > time_till_health_disable then
		hit_event = false
		decr_health()	
		timer = 0
		if get_health() == 0 then
			Die()
		end
	end
end