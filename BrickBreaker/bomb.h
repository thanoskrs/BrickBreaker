#pragma once
#include "powerups.h"
#include <string>

class Bomb : public PowerUp
{
public:
	Bomb()
	{
		powerup = "bomb.png";
	}
};