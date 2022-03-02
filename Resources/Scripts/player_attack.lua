--Constants
attack_action = 3 

pressed = 0
triggered = 1
released = 2
inactive = 3

if check_action_state(attack_action, 1) then
	spawn_projectile()
end