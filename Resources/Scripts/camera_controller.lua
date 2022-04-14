up_arrow_scancode = 82
down_arrow_scancode = 81
scancode_9 = 38

--Set the window width and height
set_window_dimensions(WINDOW_WIDTH, WINDOW_HEIGHT)

if is_released(scancode_9) then
	toggle_unlocked()
end


if is_unlocked() then
	goto exit
end


--player characters x position
follow_obj_x = get_follow_obj_x(20.0)

--player characters y position
follow_obj_y = get_follow_obj_y(15.0)

target_x = follow_obj_x
target_y = follow_obj_y

--velocity can be used for physics based lerping, velocity change 
--too sudden to matter as of now
vel_x = get_follow_obj_velx()
vel_y = get_follow_obj_vely()

forward_mod = 25
x_step = 0.04
y_step = 0.08

lerping = false

if is_flipped_scalex() or not is_done_lerping() then
	
	target_x = target_x + forward_mod * get_follow_obj_scalex()
	lerp_x = lerp_cam_x(target_x, x_step)
	set_camera_pos(lerp_x, target_y, 1)
else
	target_x = target_x + forward_mod * get_follow_obj_scalex()
	set_camera_pos(target_x, target_y, 1)
end

if is_pressed(up_arrow_scancode) then
	change_cam_z(1.0, 1.0)
end

if is_pressed(down_arrow_scancode) then
	change_cam_z(0.0, 1.0)
end

--Clamp the camera to the limits of the scene
clamp_camera()

::exit::