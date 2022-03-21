
--player characters x position
follow_obj_x = get_follow_obj_x()

--player characters y position
follow_obj_y = get_follow_obj_y()

--Set the camera pos as the same
set_camera_pos(follow_obj_x, follow_obj_y, 1)

--Clamp the camera to the limits of the scene
clamp_camera()