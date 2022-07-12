#pragma once
#include "powerups.h"
#include <string>

class Coin : public PowerUp
{
public:
	Coin()
	{
		powerup = "coin.png";
	}
};
