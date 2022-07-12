#pragma once
#include "config.h"
#include "graphics.h"
#include "util.h"
#include "GameObject.h"

#define BRICK_WIDTH CANVAS_WIDTH/10
#define BRICK_HEIGHT CANVAS_HEIGHT/15


class Brick : public Collidable
{
	float pos_x, pos_y;
	float color[3];
public:
	void draw();
	void init(const float& x, const float& y, const float& r, const float& g, const float& b);
	float getX() { return pos_x; }
	float getY() { return pos_y; }
	static float getWidth()  { return (float)BRICK_WIDTH; }
	static float getHeight() { return (float) BRICK_HEIGHT; }
	static void draw(Brick** bricks);
	Disk** getCollisionHull() const override;
};

