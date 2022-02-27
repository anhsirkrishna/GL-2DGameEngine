-- if D is pressed, go right
if is_pressed(7) then move(200.0) end
-- if A is pressed, go left
if is_pressed(4) then move(-200.0) end
-- if SPACE is pressed, jump
if is_pressed(44) then 
	set_grav(true)
	jump(-100.0) 
end
-- if A or D are released, stop moving
if is_released(7) or is_released(4) then move(0.0) end
