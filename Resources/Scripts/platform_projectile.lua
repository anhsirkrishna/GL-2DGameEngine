--Constants
projectile_speed = 100
platform_lifetime = 120

activate_event_id = 8

timer = timer + adder

if timer > 0 and timer < platform_lifetime then
	movement_disable_gravity()
end

if timer == platform_lifetime then
	timer = -120
	--Send an event id 6(activate event), with a delay of 0 ms, with broadcast=false
	send_event(activate_event_id, 0, false)
	--disable after 1.5 seconds
	delayed_disable_obj(1500)
end

if received_event then
	received_event = false
	if activate_event then
		activate_event = false
		collider_disable()
		movement_enable_gravity()
	end
end