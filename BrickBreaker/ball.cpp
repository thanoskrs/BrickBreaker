#include "ball.h"
#include "config.h"
#include "graphics.h"

void Ball::ballSound()
{
	graphics::playSound(std::string(ASSET_PATH) + "hit.mp3", sound_volume);
}


void Ball::draw()
{
	graphics::Brush br;
	br.texture = std::string(ASSET_PATH) + ball;
	br.outline_opacity = 0.f;
	graphics::drawRect(pos_x, pos_y, size, size, br);
}


void Ball::update()
{
	if (pos_y >= CANVAS_HEIGHT - size / 2)
	{
		ballSound();
		pos_y = CANVAS_HEIGHT - CANVAS_HEIGHT / 25.f - 18.5f;
		ball_fall = true;
	}
	if (pos_x >= CANVAS_WIDTH - size / 2)
	{
		ballSound();
		if (direction == UP_RIGHT)
			direction = UP_LEFT;
		if (direction == DOWN_RIGHT)
			direction = DOWN_LEFT;
		
	}
	else if (pos_y <= 38)
	{
		ballSound();
		if (direction == UP_LEFT)
			direction = DOWN_LEFT;
		if (direction == UP_RIGHT)
			direction = DOWN_RIGHT;
	}
	else if (pos_x <= size /2)
	{
		ballSound();
		if (direction == DOWN_LEFT)
			direction = DOWN_RIGHT;
		if (direction == UP_LEFT)
			direction = UP_RIGHT;
	}
	
	speed = graphics::getDeltaTime() / 3.0f;

	if (direction == UP_RIGHT) {
		pos_x += speed * speed_x <= speed ? speed * speed_x : speed;
		pos_y -= speed * speed_y <= speed ? speed * speed_y : speed;
	} 
	else if (direction == UP_LEFT)
	{
		pos_x -= speed * speed_x <= speed ? speed * speed_x : speed;
		pos_y -= speed * speed_y <= speed ? speed * speed_y : speed;
	}
	else if (direction == DOWN_LEFT)
	{
		pos_x -= speed * speed_x <= speed ? speed * speed_x : speed;
		pos_y += speed * speed_y <= speed ? speed * speed_y : speed;
	}
	else if (direction == DOWN_RIGHT)
	{
		pos_x += speed * speed_x <= speed ? speed * speed_x : speed;
		pos_y += speed * speed_y <= speed ? speed * speed_y : speed;
	}
}

void Ball::init()
{
	speed = 0.0f;
	speed_x = 1.0f;
	speed_y = 1.0f;
	pos_x = CANVAS_WIDTH / 2.f;
	pos_y = CANVAS_HEIGHT - CANVAS_HEIGHT / 25.f - 18.5f;
	size = 25.f;
}

Disk Ball::getCollisionHull() const
{
	Disk disk;
	disk.cx = pos_x;
	disk.cy = pos_y;
	disk.radius = size / 2;
	return disk;
}

Ball::Ball()
{
	init();
}