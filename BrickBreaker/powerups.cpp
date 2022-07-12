#include "powerups.h"
#include "graphics.h""
#include "config.h"

void PowerUp::update()
{
	pos_y += 2.0f;
	if (rotation > 360.0f)
		rotation = 0.0f;
}

void PowerUp::draw()
{
	graphics::Brush br;
	br.outline_opacity = 0.0f;
	br.texture = std::string(ASSET_PATH) + powerup;
	rotation += 20.0f;
	graphics::setOrientation(rotation);
	graphics::drawRect(pos_x, pos_y, size, size, br);
	br.texture = "";
	graphics::resetPose();
}

Disk PowerUp::getCollisionHull() const
{
	Disk disk;
	disk.cx = pos_x;
	disk.cy = pos_y;
	disk.radius = size / 2.0f;
	return disk;
}

