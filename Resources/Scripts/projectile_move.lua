--Constants
projectile_speed = 100;

--Move the projectile in the correct direction
if impact_event then
	move(0)
else
	--move projectile only if it hasn't impacted
	move(projectile_speed * get_scale_x())
end

if received_event then
	received_event = false
	if impact_event then
		particle_burst(1500)
		delayed_disable_obj(150)
	end
end