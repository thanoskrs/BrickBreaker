#include "brick.h"

void Brick::draw()
{
	graphics::Brush br;
	br.outline_opacity = 1.f;
	br.fill_opacity = 1.f;
	br.fill_color[0] = color[0];
	br.fill_color[1] = color[1];
	br.fill_color[2] = color[2];
	br.outline_color[0] = 0.0f;
	br.outline_color[1] = 0.0f;
	br.outline_color[2] = 0.045f;
	graphics::drawRect(pos_x, pos_y, BRICK_WIDTH, BRICK_HEIGHT, br);
}


void Brick::draw(Brick** bricks)
{
	for (int i = 0; i < 50; i++)
	{
		// if a brick is destroyed, we don't draw it
		if (bricks[i] == nullptr) {
			continue;
		}
		// draw one by tne the bricks 
		bricks[i]->draw();
	}
}


Disk** Brick::getCollisionHull() const
{
	Disk* d1 = new Disk{};
	d1->cx = pos_x - BRICK_WIDTH / 3.0f;
	d1->cy = pos_y;
	d1->radius = 25.f;
	
	Disk* d2 = new Disk{};
	d2->cx = pos_x;
	d2->cy = pos_y;
	d2->radius = 25.f;

	Disk* d3 = new Disk{};
	d3->cx = pos_x + BRICK_WIDTH / 3.0f;;
	d3->cy = pos_y;
	d3->radius = 25.f;

	
	Disk** disks = new Disk * [3];
	disks[0] = d1;
	disks[1] = d2;
	disks[2] = d3;

	return disks;
}

void Brick::init(const float& x, const float& y, const float& r, const float& g, const float& b)
{
	pos_x = x;
	pos_y = y;
	color[0] = r;
	color[1] = g;
	color[2] = b;
}


