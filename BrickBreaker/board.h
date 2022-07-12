#pragma once
#include "config.h"
#include "util.h"
#include "GameObject.h"

class Board : public GameObject, public Collidable
{
	float speed;
	float pos_x, pos_y;
	float width, height;
	float color[3];
	int lifes;
	bool lost_life = false;
public:
	Board();
	void draw() override ;
	void update() override;
	void init()  override;
	void setColor(float r, float g, float b);
	void decreaseLife() { lifes--; lost_life = true; }
	float getX() { return pos_x; }
	float getY() { return pos_y; }
	int getLifes() { return lifes; }
	Disk** getCollisionHull() const override;	
};