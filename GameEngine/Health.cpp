#include "Health.h"

Health::Health() : Component("HEALTH"), health(0)
{
}

void Health::DecrementHealth()
{
	health--;
}

void Health::Serialize(json json_object)
{
	starting_health = json_object["health_val"].get<int>();
	health = starting_health;
}

int Health::GetHealth()
{
	return health;
}

bool Health::IsDead() {
	return health <= 0;
}

void Health::Die() {
	health = 0;
}

void Health::Reset() {
	health = starting_health;
}
