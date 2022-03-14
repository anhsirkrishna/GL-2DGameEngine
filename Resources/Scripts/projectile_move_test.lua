projectile_speed = 50;

move(projectile_speed * get_scale_x())

if received_event then
	if impact_event then
		disable_obj()
	end
end