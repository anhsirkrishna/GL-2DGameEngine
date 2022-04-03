
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
x_step = 0.08
y_step = 0.03

target_x = target_x + forward_mod * get_follow_obj_scalex()

lerp_x = lerp_cam_x(target_x, x_step)
lerp_y = lerp_cam_y(target_y, y_step)
set_camera_pos(lerp_x, lerp_y, 1)	

if is_pressed(82) then
	change_cam_z(1.0, 1.0)
end

if is_pressed(81) then
	change_cam_z(0.0, 1.0)
end

--Clamp the camera to the limits of the scene
clamp_camera()