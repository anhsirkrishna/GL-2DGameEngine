--Constants
left_action = 0
right_action = 1
jump_action = 2
attack_action = 3

pressed = 0
triggered = 1
released = 2
inactive = 3

walk_speed = 50;
jump_speed = -80;

function MoveCharacterRight()
	--Only move if character is already walking or idle
	if get_state() == "IDLE" or get_state() == "WALK" then
		move(walk_speed) --Move with a velocity of "walk_speed"
		change_state("WALK")
		set_transform_scale(1, 1)  --Make sure the sprite is facing right
	end
end

function MoveCharacterLeft()
	--Only move if character is already walking or idle
	if get_state() == "IDLE" or get_state() == "WALK" then
		move(-1*walk_speed) --Move with a velocity of "walk_speed"
		change_state("WALK")
		set_transform_scale(-1, 1)  --Make sure the sprite is facing right
	end
end

function JumpCharacter()
	--Cannot double jump so make sure character not jumping/falling
	if get_state() ~= "JUMP" and get_state() ~= "FALL" then
		jump(jump_speed)
		change_state("JUMP")
	end
end

function ThrowProjectile()
	if get_state() == "IDLE" then
		spawn_projectile()
	end
end


if check_action_state(left_action, inactive) and 
	check_action_state(right_action, inactive) and 
	check_action_state(jump_action, inactive) then
	if get_downlock() == true then
		change_state("IDLE")
		move(0)
	end
end

--If right was triggered
if check_action_state(right_action, pressed) then
	MoveCharacterRight()
end

--If left was triggered
if check_action_state(left_action, pressed) then
	MoveCharacterLeft()
end

--If right was released while left was pressed
if check_action_state(right_action, released) and check_action_state(left_action, pressed) then
	MoveCharacterLeft()
end

--If left was released while right was pressed
if check_action_state(right_action, released) and check_action_state(left_action, pressed) then
    MoveCharacterRight()
end

--If jump button was triggered
if check_action_state(jump_action, triggered) then
	JumpCharacter()
end

--If character is falling
if get_vertical_velocity() > 0 then
	change_state("FALL")
elseif get_vertical_velocity() == 0 then
	if get_state() == "FALL" then
		change_state("IDLE")
	end
end

if check_action_state(attack_action, triggered) then
	ThrowProjectile()
end
