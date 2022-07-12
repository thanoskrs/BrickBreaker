#pragma once
#include "config.h"
#include <string>
#include "GameObject.h"


class Ball : public GameObject
{
	float pos_x, pos_y;
	float speed_x, speed_y;
	float size;
	float speed;
	bool ball_fall = false;
	bool classic_ball = true;
	bool blue_ball = false, purple_ball = false, football = false, tennis_ball = false;
	float sound_volume = 0.8f;
	std::string ball = "ball.png";
public:
	typedef enum { UP_LEFT, UP_RIGHT, DOWN_LEFT, DOWN_RIGHT } ball_dir;
	ball_dir direction;

	Ball();
	void draw() override;
	void update() override;
	void init() override;
	void ballSound();
	void setX(float x) { pos_x = x; }
	void setY(float y) { pos_y = y; }
	void setSpeedX(float x) { speed_x = x; }
	void setSpeedY(float y) { speed_y = y; }
	void setSoundVolume(float v) { v > 0.f && v < 1.f ? sound_volume = v : sound_volume = 1.0f; }
	void setBallFall(bool b) { ball_fall = b; }
	void setBall(std::string b) { ball = b; }
	float getX() { return pos_x; }
	float getY() { return pos_y; }
	bool getBallFall() { return ball_fall; }
	Disk getCollisionHull() const;
	void setBlueBall(bool b) { blue_ball = b; }
	void setPurpleBall(bool b) { purple_ball = b; }
	void setFootBall(bool b) { football = b; }
	void setTennisBall(bool b) { tennis_ball = b; }
	bool blueBall() { return blue_ball; }
	bool purpleBall() { return purple_ball; }
	bool footBall() { return football; }
	bool tennisBall() { return tennis_ball; }
};