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
	move(walk_speed) --Move with a velocity of "walk_speed"
	set_transform_scale(1, 1)  --Make sure the sprite is facing right
end

function MoveCharacterLeft()
	--Only move if character is already walking or idle
	move(-1*walk_speed) --Move with a velocity of "walk_speed"
	set_transform_scale(-1, 1)  --Make sure the sprite is facing right
end

function JumpCharacter()
	--Cannot double jump so make sure character not jumping/falling
	jump(jump_speed)
end

function ThrowProjectile()
	spawn_projectile()
end


if check_action_state(left_action, inactive) and 
	check_action_state(right_action, inactive) and 
	check_action_state(jump_action, inactive) then
		move(0)
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

if check_action_state(attack_action, triggered) then
	ThrowProjectile()
end
