--Constants
projectile_speed = 100;

--Move the projectile in the correct direction
move(projectile_speed * get_scale_x())

if received_event then
	received_event = false
	if impact_event then
		impact_event = false
		--Particle burst effect
		particle_burst(100)

		--disable after half a second for a lingering effect
		delayed_disable_obj(300)
	end
end