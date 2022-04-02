
if received_event then
	received_event = false
	if hit_event then
		--if hit then despawn the mirror
		delayed_disable_obj(150)
	end
end