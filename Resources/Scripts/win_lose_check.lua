if received_event then
	log_msg("received_event")
	received_event = false
	if lose_event then
		log_msg("lose_event")
		statestack_push_lose()
	end
end