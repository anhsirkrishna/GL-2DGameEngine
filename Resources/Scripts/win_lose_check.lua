if received_event then
	received_event = false
	if lose_event then
		lose_event = false
		statestack_push_lose()
	end

	if win_event then
		win_event = false
		statestack_push_win()
	end
end