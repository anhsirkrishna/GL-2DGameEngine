if received_event then
	received_event = false
	if pickedup_event then
		pickedup_event = false
		particle_burst(1000)
		delayed_disable_obj(1000)
	end
end