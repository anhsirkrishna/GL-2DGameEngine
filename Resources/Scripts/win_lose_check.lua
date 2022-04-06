if received_event then
	received_event = false
	if lose_event then
		statestack_push_lose()
	end
end