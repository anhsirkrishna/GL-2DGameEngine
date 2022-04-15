/******************************************************************************/
/* !
/* File   Health.h
/* Author Sreyash Raychaudhuri
/* Email: srey.raychaudhuri@digipen.edu
/* Date   01/27/2022
/* Pathfinder Component encapsulation header file
/* DigiPen Institute of Technology © 2022
/******************************************************************************/

#pragma once
#include "Component.h"

class Health : public Component
{
private:
    int health;
	int starting_health;
public:
    Health();
    void Serialize(json json_object);

    void DecrementHealth();
    int GetHealth();
    bool IsDead();
    void Die();
	virtual void Reset();
};

