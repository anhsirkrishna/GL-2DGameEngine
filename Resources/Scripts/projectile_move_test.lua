projectile_speed = 50;

move(projectile_speed * get_scale_x())

if received_event then
	if impact_event then
		move(0)
		particle_burst(100)
		--disable after half a second
		delayed_disable_obj(500)
	end
end