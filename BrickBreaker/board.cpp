#include "board.h"
#include "graphics.h"
#include "config.h"

void Board::update()
{
	if (pos_x > width/2 && graphics::getKeyState(graphics::SCANCODE_LEFT))
	{
		pos_x -= graphics::getDeltaTime() / 2.0f;
	}
	if (pos_x < CANVAS_WIDTH - width/2 && graphics::getKeyState(graphics::SCANCODE_RIGHT))
	{
		pos_x += graphics::getDeltaTime() / 2.0f;
	}
	if (lost_life)
	{
		pos_x = CANVAS_WIDTH / 2.f;
		pos_y = CANVAS_HEIGHT - CANVAS_HEIGHT / 25.f;
		lost_life = false;
	}

}

void Board::draw()
{
	graphics::Brush br;
	br.fill_color[0] = color[0];
	br.fill_color[1] = color[1];
	br.fill_color[2] = color[2];
	br.outline_opacity = 0.0f;
	graphics::drawRect(pos_x, pos_y, width, height, br);

}

void Board::init()
{
	speed = 5.0f;
	pos_x = CANVAS_WIDTH / 2.f;
	pos_y = CANVAS_HEIGHT - CANVAS_HEIGHT / 25.f;
	width = CANVAS_WIDTH / 8.0f;
	height = CANVAS_HEIGHT / 50.f;
	lifes = 3 ;
}

void Board::setColor(float r, float g, float b)
{
	color[0] = r;
	color[1] = g;
	color[2] = b;
}
Disk** Board::getCollisionHull() const
{
	Disk* d1 = new Disk{};
	d1->cx = pos_x - 3 * width / 7.0f;
	d1->cy = pos_y;
	d1->radius = 12.0f;

	Disk* d2 = new Disk{};
	d2->cx = pos_x - 2 * width / 7.0f;
	d2->cy = pos_y;
	d2->radius = 12.0f;

	Disk* d3 = new Disk{};
	d3->cx = pos_x - width / 7.0f;
	d3->cy = pos_y;
	d3->radius = 12.0f;

	Disk* d4 = new Disk{};
	d4->cx = pos_x;
	d4->cy = pos_y;
	d4->radius = 12.0f;

	Disk* d5 = new Disk{};
	d5->cx = pos_x + width / 7.0f;
	d5->cy = pos_y;
	d5->radius = 12.0f;

	Disk* d6 = new Disk{};
	d6->cx = pos_x + 2 * width / 7.0f;
	d6->cy = pos_y;
	d6->radius = 12.0f;

	Disk* d7 = new Disk{};
	d7->cx = pos_x + 3 * width / 7.0f;
	d7->cy = pos_y;
	d7->radius = 12.0f;

	Disk** disks = new Disk * [7];
	disks[0] = d1;
	disks[1] = d2;
	disks[2] = d3;
	disks[3] = d4;
	disks[4] = d5;
	disks[5] = d6;
	disks[6] = d7;

	return disks;
}


Board::Board()
{
	init();
}