#pragma once
#include "graphics.h"
#include "brick.h"
#include "board.h"
#include "ball.h"
#include "coin.h"
#include "bomb.h"
#include <unordered_map>
#include <string>
#include <chrono>

class Game
{
	typedef enum { STATUS_MENU, STATUS_PLAY, STATUS_SETTINGS } status_t;
	typedef enum { COLOR_BLUE, COLOR_PURPLE, COLOR_GREEN, COLOR_YELLOW, COLOR_GRAY} color_t;

	Brick** bricks = nullptr;
	Board* board = nullptr;
	Ball* ball = nullptr;

	std::unordered_map<int, Coin> coins;
	std::unordered_map<int, Bomb> bombs;

	status_t status = STATUS_MENU;
	color_t color = COLOR_BLUE;

	std::chrono::steady_clock::time_point sec;

	int score = 0, bricks_left =50;
	int bonus = 0;
	int saved_coins;

	float music_volume = 0.4, pos_x_music = 480.0f;
	float sound_volume = 0.8f, pos_x_sound = 500.0f;
	float r = 0.25f, g = 0.35f, b = 0.7f;

	bool pause = false, start = false, music = false;
	bool collision = false;
	bool finish = false;
	bool intialization = false;
	bool payment = false;
	bool not_enough_coins = false;
	bool bomb_col = false;
	bool coin_col = false;
	
	std::string current_ball;

	void updateMenu();
	void updateGame();
	void updateSettings();

	void drawMenu();
	void drawGame();
	void drawSettings();

	void initGame();

	void chooseGameColor(float& x, const float& y, float r, float g, float b);
	void showBall(float& x, float& y, std::string image);
	void checkBrickCollision();
	void checkBoardCollision();
	void checkCoinCollision();
	void checkBombCollision();

	void playMusic();
	void stopMusic();

public:
	void update();
	void draw();
	void init();
	float getCoins() { return saved_coins; }
	Game() {}
	~Game();
};