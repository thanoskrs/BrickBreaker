#pragma once
#include "util.h"
#include <string>

class PowerUp
{
	float pos_x, pos_y;
	float size = 25.0f;
	bool enable = false;
	float rotation = 0.0f;

protected:
	std::string powerup;

public:
	void update();
	void draw();
	void setPosX(float x) { pos_x = x; }
	void setPosY(float y) { pos_y = y; }
	void setEnable(bool b) { enable = b; }
	float getPosX() { return pos_x; }
	float getPosY() { return pos_y; }
	bool getEnable() { return enable; }
	Disk getCollisionHull() const;
};