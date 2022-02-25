if is_pressed(15) then move(200.0) end
if is_pressed(13) then move(-200.0) end
if is_pressed(12) then jump(-200.0) end
if is_released(15) or is_released(13) then
	set_grav(true)
	move(0.0)
end
