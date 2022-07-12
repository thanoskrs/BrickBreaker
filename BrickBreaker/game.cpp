#include "game.h"
#include "graphics.h"
#include "config.h"
#include "brick.h"
#include "util.h"
#include <fstream>
#include <time.h>

void Game::update()
{
	if (status == STATUS_MENU)
	{
		updateMenu();
	}
	else if (status == STATUS_PLAY)
	{
		updateGame();
	}
	else
	{
		updateSettings();
	}
}

void Game::draw()
{
	graphics::Brush br;
	br.fill_color[0] = 0.0f;
	br.fill_color[1] = 0.0f;
	br.fill_color[2] = 0.045f;
	br.outline_opacity = 0.0f;

	// draw backround
	graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br);

	graphics::MouseState ms;
	graphics::getMouseState(ms);
	
	if (status == STATUS_MENU) {
		drawMenu();
	}
	else if (status == STATUS_PLAY) {
		drawGame();
	}
	else
	{
		drawSettings();
	}
}

void Game::init()
{
	initGame();
	graphics::setFont(std::string(ASSET_PATH) + "bitstream.ttf");
	std::ifstream data("playersData.txt", std::ios::in);
	if (!data)
	{
		saved_coins = 0;
		exit(EXIT_FAILURE);
	}
	else
	{
		std::string text;
		std::getline(data, text);
		saved_coins = std::stoi(text);
	}

}

void Game::initGame()
{
	if (bricks == nullptr)
	{
		bricks = new  Brick * [50];
	}
	if (board == nullptr)
	{
		board = new Board();
	}
	if (ball == nullptr)
	{
		ball = new Ball();
	}

	if (color == COLOR_BLUE)
	{
		r = 0.25;
		g = 0.35f;
		b = 0.7f;
	}
	else if (color == COLOR_PURPLE)
	{
		r = 1.0f;
		g = 0.15f;
		b = 0.5f;
	}
	else if (color == COLOR_GREEN)
	{
		r = 0.0f;
		g = 1.0f;
		b = 0.15f;
	}
	else if (color == COLOR_YELLOW)
	{
		r = 0.85;
		g = 1.0f;
		b = 0.15f;
	}
	else if (color == COLOR_GRAY)
	{
		r = 0.1;
		g = 0.1;
		b = 0.1;
	}


	float x = Brick::getWidth() / 2.0f, y = Brick::getHeight() * 1.2f;
	for (int i = 0; i < 50; i++)
	{
		Brick* brick = new Brick();
		bricks[i] = brick;
		bricks[i]->init(x, y, r, g, b);
		if ((i + 1) % 10 == 0) {
			x = Brick::getWidth() / 2.0f;
			y = y + Brick::getHeight();
			if (color == COLOR_BLUE || color == COLOR_PURPLE)
			{
				g += 0.125f;
			}
			else if (color == COLOR_GREEN || color == COLOR_YELLOW)
			{
				b += 0.125f;
			}
			else if (color == COLOR_GRAY)
			{
				r += 0.125f;
				g += 0.125f;
				b += 0.125f;
			}
		}
		else {
			x += Brick::getWidth();
		}
	}

	board->setColor(r, g, b);

	for (int i = 0; i < 10; i++)
	{
		int random = std::rand() % 50;
		Coin coin = Coin();
		coins[random] = coin;
		coins[random].setPosX(bricks[random]->getX());
		coins[random].setPosY(bricks[random]->getY());
	}

	for (int i = 0; i < 5; i++)
	{
		int random = std::rand() % 50;
		if (coins.find(random) == coins.end())
		{
			Bomb bomb = Bomb();
			bombs[random] = bomb;
			bombs[random].setPosX(bricks[random]->getX());
			bombs[random].setPosY(bricks[random]->getY());
		}
	}

	intialization = true;
}

void Game::updateMenu()
{
	if (music)
	{
		stopMusic();
		music = false;
	}

	float coordinates[3][3];
	coordinates[0][0] = CANVAS_WIDTH / 2.0f;
	coordinates[1][0] = CANVAS_WIDTH / 2.0f;

	coordinates[0][1] = CANVAS_HEIGHT / 2.0f - 50.0f;
	coordinates[1][1] = coordinates[0][1] + 50.0f;

	graphics::MouseState ms;
	graphics::getMouseState(ms);

	if (ms.cur_pos_x > coordinates[0][0] - 100 && ms.cur_pos_x < coordinates[0][0] + 100
		&& ms.cur_pos_y > coordinates[0][1] - 25 && ms.cur_pos_y < coordinates[0][1] + 25)
	{

		if (ms.button_left_released)
		{
			status = STATUS_PLAY;
			if(!intialization) 
				initGame();
		}
	}

	if (ms.cur_pos_x > coordinates[1][0] - 100 && ms.cur_pos_x < coordinates[1][0] + 100
		&& ms.cur_pos_y > coordinates[1][1] - 25 && ms.cur_pos_y < coordinates[1][1] + 25)
	{
		if (ms.button_left_released)
			status = STATUS_SETTINGS;
	}
}

void Game::updateGame()
{
	if (!finish)
	{

		//check collisions if start
		if (start && !pause)
		{
			ball->update();
			checkBoardCollision();
			checkBrickCollision();
			checkCoinCollision();
			checkBombCollision();
			for (auto iter = coins.begin(); iter != coins.end(); iter++)
			{
				if (iter->second.getEnable())
				{
					iter->second.update();
				}
			}

			for (auto iter = bombs.begin(); iter != bombs.end(); iter++)
			{
				if (iter->second.getEnable())
				{
					iter->second.update();
				}
			}
			if (bomb_col) {
				while(board->getLifes() > 0)
					board->decreaseLife();
			}

		}
		if (!start)
		{
			ball->setX(board->getX());
			ball->setY(board->getY() - 18.5f);
			if (graphics::getKeyState(graphics::SCANCODE_SPACE))
			{
				ball->direction = Ball::UP_RIGHT;
				start = true;
			}
			for (auto iter = coins.begin(); iter != coins.end();)
			{
				if (iter->second.getEnable())
				{
					coins.erase(iter++);
				}
				else
				{
					++iter;
				}
			}
			for (auto iter = bombs.begin(); iter != bombs.end();)
			{
				if (iter->second.getEnable())
				{
					bombs.erase(iter++);
				}
				else
				{
					++iter;
				}
			}
		}
		if (!pause)
		{
			if (ball->getBallFall())
			{
				board->decreaseLife();
				bonus = 0;
				ball->setBallFall(false);
				start = false;
			}
			board->update();
		}

		if (pause)
		{
			stopMusic();
			music = false;
		}

		graphics::MouseState ms;
		graphics::getMouseState(ms);

		if (!finish)
		{
			// pause game and play 
			if (!pause)
			{
				if (ms.cur_pos_x >= 20 && ms.cur_pos_x <= 50
					&& ms.cur_pos_y >= 5 && ms.cur_pos_y <= 20)
				{
					if (ms.button_left_released)
						pause = true;
				}

				if (!music)
				{
					playMusic();
					music = true;
				}
			}
			else
			{
				if (ms.cur_pos_x >= 20 && ms.cur_pos_x <= 50
					&& ms.cur_pos_y >= 5 && ms.cur_pos_y <= 20)
				{
					if (ms.button_left_released)
						pause = false;
				}
			}
		}
	}

	if (finish)
	{
		stopMusic();
	}
	graphics::MouseState ms;
	graphics::getMouseState(ms);

	// go to main menu
	if (ms.cur_pos_x >= 100 && ms.cur_pos_x <= 180
		&& ms.cur_pos_y >= 5 && ms.cur_pos_y <= 20)
	{

		if (ms.button_left_released)
		{
			status = STATUS_MENU;
			score = 0;
			bricks_left = 50;
			start = false;
			bonus = 0;
			finish = false;
			collision = false;
			// we dont delete the ball because we want to handle the sound of ball from settings 
			delete board;
			board = nullptr;
			for (int i = 0; i < 50; i++)
			{
				if (bricks[i])
					delete bricks[i];
				bricks[i] = nullptr;
			}
			delete bricks;
			bricks = nullptr;
			coins.clear();
			bombs.clear();
			bomb_col = false;
			intialization = false;
		}
	}
}

void Game::updateSettings()
{
	if (music)
	{
		stopMusic();
		music = false;
	}

	graphics::MouseState ms;
	graphics::getMouseState(ms);

	if (!payment)
	{
		//choose color for bricks and board
		if (ms.cur_pos_y >= 140.0f && ms.cur_pos_y <= 170.0f)
		{
			if (ms.cur_pos_x > 320.0f && ms.cur_pos_x < 380.0f)
			{
				if (ms.button_left_released)
				{
					color = COLOR_BLUE;
				}
			}
			else if (ms.cur_pos_x > 380.0f && ms.cur_pos_x < 440.0f)
			{
				if (ms.button_left_released)
				{
					color = COLOR_PURPLE;
				}
			}
			else if (ms.cur_pos_x > 440.0f && ms.cur_pos_x < 500.0f)
			{
				if (ms.button_left_released)
				{
					color = COLOR_GREEN;
				}
			}
			else if (ms.cur_pos_x > 500.0f && ms.cur_pos_x < 560.0f)
			{
				if (ms.button_left_released)
				{
					color = COLOR_YELLOW;
				}
			}
			else if (ms.cur_pos_x > 560.0f && ms.cur_pos_x < 620.0f)
			{
				if (ms.button_left_released)
				{
					color = COLOR_GRAY;
				}
			}
		}

		//choose ball
		if (ms.cur_pos_y >= 225.0f && ms.cur_pos_y <= 255.0f)
		{
			if (ms.cur_pos_x > 355.0 && ms.cur_pos_x < 385.0f)
			{
				if (ms.button_left_released)
				{
					ball->setBall("ball.png");
				}
			}
			else if (ms.cur_pos_x > 405.0f && ms.cur_pos_x < 435.0f)
			{
				if (ms.button_left_released)
				{
					if (ball->blueBall())
					{
						ball->setBall("blue_ball.png");
					}
					else
					{
						current_ball = "blue_ball.png";
						if (saved_coins >= 5)
						{
							payment = true;
						}
						else
						{
							not_enough_coins = true;
							auto start = std::chrono::steady_clock::now();
							sec = start;
						}
					}
				}
			}
			else if (ms.cur_pos_x > 455.0f && ms.cur_pos_x < 485.0f)
			{
				if (ms.button_left_released)
				{
					if (ball->purpleBall())
					{
						ball->setBall("purple_ball.png");
					}
					else
					{
						current_ball = "purple_ball.png";
						if (saved_coins >= 5)
						{
							payment = true;
						}
						else
						{
							not_enough_coins = true;
							auto start = std::chrono::steady_clock::now();
							sec = start;
						}
					}
				}
			}
			else if (ms.cur_pos_x > 505.0f && ms.cur_pos_x < 535.0f)
			{
				if (ms.button_left_released)
				{
					if (ball->footBall())
					{
						ball->setBall("football.png");
					}
					else
					{
						current_ball = "football.png";
						if (saved_coins >= 5)
						{
							payment = true;
						}
						else
						{
							not_enough_coins = true;
							auto start = std::chrono::steady_clock::now();
							sec = start;
							
						}
					}
				}
			}
			else if (ms.cur_pos_x > 555.0f && ms.cur_pos_x < 585.0f)
			{
				if (ms.button_left_released)
				{
					if (ball->tennisBall())
					{
						ball->setBall("tennisball.png");
					}
					else
					{
						current_ball = "tennisball.png";
						if (saved_coins >= 5)
						{
							payment = true;
						}
						else
						{
							not_enough_coins = true;
							auto start = std::chrono::steady_clock::now();
							sec = start;
						}
					}
				}
			}
		}


		// music volume
		if (ms.cur_pos_x >= 365.0f && ms.cur_pos_x <= 565.0f
			&& ms.cur_pos_y >= 370.0f && ms.cur_pos_y <= 390.0f)
		{
			if (ms.dragging)
				pos_x_music = ms.cur_pos_x;
		}

		music_volume = (pos_x_music - 365) / 200;

		// sound volume
		if (ms.cur_pos_x >= 365.0f && ms.cur_pos_x <= 565.0f
			&& ms.cur_pos_y >= 315.0f && ms.cur_pos_y <= 335.0f)
		{
			if (ms.dragging)
				pos_x_sound = ms.cur_pos_x;
		}

		sound_volume = (pos_x_sound - 365.0f) / 200.0f;
		ball->setSoundVolume(sound_volume);

		if (ms.cur_pos_x >= 20 && ms.cur_pos_x <= 100
			&& ms.cur_pos_y >= 5 && ms.cur_pos_y <= 20)
		{
			if (ms.button_left_released)
				status = STATUS_MENU;
		}
	}
	else if (payment)
	{
		if (ms.cur_pos_y >= CANVAS_HEIGHT / 2.0f + 5.0f && ms.cur_pos_y <= CANVAS_HEIGHT / 2.0f + 35.0f)
		{
			if (ms.cur_pos_x >= CANVAS_WIDTH / 2.0f - 40.0f - 15.0f && ms.cur_pos_x <= CANVAS_WIDTH / 2.0f - 40.0f + 15.0f)
			{
				if (ms.button_left_released)
				{
					ball->setBall(current_ball);
					if (current_ball == "blue_ball.png")
					{
						ball->setBlueBall(true);
					}
					else if (current_ball == "purple_ball.png")
					{
						ball->setPurpleBall(true);
					}
					else if (current_ball == "football.png")
					{
						ball->setFootBall(true);
					}
					else if (current_ball == "tennisball.png")
					{
						ball->setTennisBall(true);
					}
					saved_coins -= 5;
					payment = false;
				}
			}
			else if (ms.cur_pos_x >= CANVAS_WIDTH / 2.0f + 40.0f - 15.0f && ms.cur_pos_x <= CANVAS_WIDTH / 2.0f + 40.0f + 15.0f)
			{
				if (ms.button_left_released)
					payment = false;
			}
		}
	}
}

void Game::playMusic()
{
	graphics::playMusic(std::string(ASSET_PATH) + "basic_music.mp3", music_volume, true);
}

void Game::stopMusic()
{
	graphics::stopMusic();
}

Game::~Game()
{
	if (board)
		delete board;
	if (ball)
		delete ball;
	if (bricks)
	{
		for (int i = 0; i < 50; i++)
		{
			if (bricks[i])
				delete bricks[i];
		}
		delete[] bricks;
	}
}

void Game::drawMenu()
{
	graphics::Brush br;
	br.fill_color[0] = 0.8f;
	br.fill_color[1] = 1.0f;
	br.fill_color[2] = 1.0f;

	br.outline_color[0] = 0.0f;
	br.outline_color[1] = 0.0f;
	br.outline_color[2] = 0.045f;
	br.outline_opacity = 1.0f;

	float coordinates[2][2] = {};

	coordinates[0][0] = CANVAS_WIDTH / 2.0f;
	coordinates[1][0] = CANVAS_WIDTH / 2.0f;

	coordinates[0][1] = CANVAS_HEIGHT / 2.0f - 50.0f;
	coordinates[1][1] = coordinates[0][1] + 50.0f;

	graphics::drawRect(coordinates[0][0], coordinates[0][1], 200, 50, br);
	graphics::drawRect(coordinates[1][0], coordinates[1][1], 200, 50, br);

	br.fill_color[0] = 0.02f;
	br.fill_color[1] = 0.05f;
	br.fill_color[2] = 0.1f;


	float text_coor_y[2] = {};
	float text_coor_x = CANVAS_WIDTH / 2.0f - 50.0f;;
	text_coor_y[0] = CANVAS_HEIGHT / 2.0f - 40.0f;
	text_coor_y[1] = text_coor_y[0] + 50.0f;


	char PLAY[5];
	sprintf_s(PLAY, "PLAY");
	graphics::drawText(text_coor_x, text_coor_y[0], 30, PLAY, br);

	char SETTINGS[9];
	sprintf_s(SETTINGS, "SETTINGS");
	graphics::drawText(text_coor_x -20 , text_coor_y[1], 30, SETTINGS, br);


	graphics::MouseState ms;
	graphics::getMouseState(ms);

	if (ms.cur_pos_x > coordinates[0][0] - 100 && ms.cur_pos_x < coordinates[0][0] + 100
		&& ms.cur_pos_y > coordinates[0][1] - 25 && ms.cur_pos_y < coordinates[0][1] + 25)
	{
		br.fill_color[0] = 0.6f;
		br.fill_color[1] = 0.15f;
		br.fill_color[2] = 0.8f;
		graphics::drawText(text_coor_x, text_coor_y[0], 30, PLAY, br);
	}

	if (ms.cur_pos_x > coordinates[1][0] - 100 && ms.cur_pos_x < coordinates[1][0] + 100
		&& ms.cur_pos_y > coordinates[1][1] - 25 && ms.cur_pos_y < coordinates[1][1] + 25)
	{
		br.fill_color[0] = 0.6f;
		br.fill_color[1] = 0.15f;
		br.fill_color[2] = 0.8f;
		graphics::drawText(text_coor_x - 20, text_coor_y[1], 30, SETTINGS, br);
	}

}

void Game::drawGame()
{
	board->draw();

	Brick::draw(bricks);

	ball->draw();
	for (auto iter = coins.begin(); iter != coins.end(); iter++)
	{
		if (iter->second.getEnable())
		{
			iter->second.draw();
		}
	}
	for (auto iter = bombs.begin(); iter != bombs.end(); iter++)
	{
		if (iter->second.getEnable())
		{
			iter->second.draw();
		}
	}


	graphics::Brush br;
	br.fill_color[0] = 1.0f;
	br.fill_color[1] = 1.0f;
	br.fill_color[2] = 1.0f;

	// print "main menu" choice
	char MENU[15];
	sprintf_s(MENU, "main   menu");
	graphics::drawText(100, 20, 15, MENU, br);

	//coins
	br.texture = std::string(ASSET_PATH) + "coin.png";
	br.outline_opacity = 0.0f;
	graphics::drawRect(605.0f, 15.0f, 20.0f, 20.0f, br);
	char coins_info[10];
	sprintf_s(coins_info, "x   %i", saved_coins);
	graphics::drawText(620.0f, 20.0f, 15.0f, coins_info, br);

	//lifes 
	br.texture = std::string(ASSET_PATH) + "heart.png";
	graphics::drawRect(680.0f, 15.0f, 20.0f, 20.0f, br);
	char lifes[10];
	sprintf_s(lifes, "x   %i", board->getLifes());
	graphics::drawText(695.0f, 20.0f, 15.0f, lifes, br);
	br.texture = "";

	// print score
	char SCORE[20];
	sprintf_s(SCORE, "SCORE :   %i", score);
	graphics::drawText(750, 20, 15, SCORE, br);

	// print how many bricks left to win
	char BRICKS_LEFT[20];
	sprintf_s(BRICKS_LEFT, "BRICKS  LEFT :   %i", bricks_left);
	graphics::drawText(850, 20, 15, BRICKS_LEFT, br);

	if (bricks_left == 0)
	{
		finish = true;
		graphics::drawText(450, 500, 25, "YOU    WON ! ! !", br);
	}
	if (board->getLifes() == 0)
	{
		finish = true;
		graphics::drawText(450, 500, 25, "YOU    LOST . . .", br);
	}

	graphics::MouseState ms;
	graphics::getMouseState(ms);

	if (!finish)
	{

		if (collision && bonus > 1)
		{
			graphics::Brush br;
			br.fill_color[0] = 1.0f;
			br.fill_color[1] = 1.0f;
			br.fill_color[2] = 1.0f;
			char printBonus[40];
			sprintf_s(printBonus, "+   %i   BONUS", 100 * (bonus - 1));
			graphics::drawText(450, 450, 20, printBonus, br);
		}
		if (graphics::getDeltaTime() >= 100.0f)
		{
			collision = false;
		}

		// pause game and play


		if (!pause) {
			char PAUSE[10];
			sprintf_s(PAUSE, "pause");
			graphics::drawText(20, 20, 15, PAUSE, br);
			if (ms.cur_pos_x >= 20 && ms.cur_pos_x <= 50
				&& ms.cur_pos_y >= 5 && ms.cur_pos_y <= 20)
			{
				br.fill_color[0] = 1.0f;
				br.fill_color[1] = 1.0f;
				br.fill_color[2] = 0.6f;
				graphics::drawText(20, 20, 15, PAUSE, br);
			}
		}
		else
		{
			char PLAY[10];
			sprintf_s(PLAY, "play");
			graphics::drawText(20, 20, 15, PLAY, br);
			if (ms.cur_pos_x >= 20 && ms.cur_pos_x <= 50
				&& ms.cur_pos_y >= 5 && ms.cur_pos_y <= 20)
			{
				br.fill_color[0] = 1.0f;
				br.fill_color[1] = 1.0f;
				br.fill_color[2] = 0.6f;
				graphics::drawText(20, 20, 15, PLAY, br);
			}
		}
	}

	// go to main menu
	if (ms.cur_pos_x >= 100 && ms.cur_pos_x <= 180
		&& ms.cur_pos_y >= 5 && ms.cur_pos_y <= 20)
	{
		br.fill_color[0] = 1.0f;
		br.fill_color[1] = 1.0f;
		br.fill_color[2] = 0.6f;
		graphics::drawText(100, 20, 15, MENU, br);
	}

	if (!start && board->getLifes() >= 1)
	{
		br.fill_color[0] = 0.0f;
		br.fill_color[1] = 0.0f;
		br.fill_color[2] = 0.045f;
		br.fill_opacity = 0.45f;
		br.outline_opacity = 0.0f;
		graphics::drawRect(CANVAS_WIDTH / 2.0f, CANVAS_HEIGHT / 2.0f, CANVAS_WIDTH, CANVAS_HEIGHT, br);
		br.fill_color[0] = 1.0f;
		br.fill_color[1] = 1.0f;
		br.fill_color[2] = 1.0f;
		br.fill_opacity = 1.0f;
		graphics::drawText(350, 400, 20, "PRESS    SPACE    TO    START", br);
	}
}

void Game::drawSettings()
{
	graphics::Brush br;
	graphics::MouseState ms;

	if (!payment)
	{
		br.fill_color[0] = 0.8f;
		br.fill_color[1] = 1.0f;
		br.fill_color[2] = 1.0f;

		br.outline_color[0] = 0.0f;
		br.outline_color[1] = 0.0f;
		br.outline_color[2] = 0.045f;
		br.outline_opacity = 1.0f;

		char MENU[15];
		sprintf_s(MENU, "main   menu");
		graphics::drawText(20, 20, 15, MENU, br);

		br.texture = std::string(ASSET_PATH) + "coin.png";
		br.outline_opacity = 0.0f;
		graphics::drawRect(605.0f, 15.0f, 20.0f, 20.0f, br);
		char coins_info[10];
		sprintf_s(coins_info, "x   %i", saved_coins);
		graphics::drawText(620.0f, 20.0f, 15.0f, coins_info, br);
		br.texture = "";
		br.outline_opacity = 1.0f;

		// let the play choose color for board
		char info[30];
		sprintf_s(info, "BOARD    COLOR");
		graphics::drawText(400, 125, 20, info, br);

		float x = 350.0f;
		float y = 155.0f;

		chooseGameColor(x, y, 0.25f, 0.35f, 0.7f);
		chooseGameColor(x, y, 1.0f, 0.15f, 0.5f);
		chooseGameColor(x, y, 0.0f, 1.0f, 0.15f);
		chooseGameColor(x, y, 0.85f, 1.0f, 0.15);
		chooseGameColor(x, y, 0.1, 0.1f, 0.1f);


		// let the player choose color for ball
		br.fill_color[0] = 0.8f;
		br.fill_color[1] = 1.0f;
		br.fill_color[2] = 1.0f;
		sprintf_s(info, "CHOOSE    BALL");
		graphics::drawText(405, 210, 20, info, br);


		x = 370.0f;
		y = 240.0f;

		//br.outline_opacity = 0.0f;
		showBall(x, y, "ball.png");
		showBall(x, y, "blue_ball.png");
		showBall(x, y, "purple_ball.png");
		showBall(x, y, "football.png");
		showBall(x, y, "tennisball.png");
		br.texture = "";

		// sound
		br.fill_color[0] = 0.8f;
		br.fill_color[1] = 1.0f;
		br.fill_color[2] = 1.0f;
		sprintf_s(info, "SOUND");
		graphics::drawText(435, 300, 20, info, br);

		br.outline_color[0] = 1.f;
		br.outline_color[1] = 1.f;
		br.outline_color[2] = 1.f;
		br.outline_opacity = 1.f;
		graphics::drawLine(365, 325, 565, 325, br);

		br.fill_color[0] = 1.0f;
		br.fill_color[1] = 1.0f;
		br.fill_color[2] = 0.f;
		br.outline_opacity = 0.f;
		graphics::drawRect(pos_x_sound, 325, 5, 15, br);


		//music
		br.fill_color[0] = 0.8f;
		br.fill_color[1] = 1.0f;
		br.fill_color[2] = 1.0f;
		sprintf_s(info, "MUSIC");
		graphics::drawText(435, 355, 20, info, br);

		br.outline_color[0] = 1.f;
		br.outline_color[1] = 1.f;
		br.outline_color[2] = 1.f;
		br.outline_opacity = 1.f;
		graphics::drawLine(365, 380, 565, 380, br);

		br.fill_color[0] = 1.0f;
		br.fill_color[1] = 1.0f;
		br.fill_color[2] = 0.f;
		br.outline_opacity = 0.f;
		graphics::drawRect(pos_x_music, 380, 5, 15, br);

		graphics::MouseState ms;
		graphics::getMouseState(ms);

		if (ms.cur_pos_x >= 20 && ms.cur_pos_x <= 100
			&& ms.cur_pos_y >= 5 && ms.cur_pos_y <= 20)
		{
			br.fill_color[0] = 1.0f;
			br.fill_color[1] = 1.0f;
			br.fill_color[2] = 0.6f;
			graphics::drawText(20, 20, 15, MENU, br);
		}

		br.fill_opacity = 0.0f;
		br.outline_color[0] = 1.0f;
		br.outline_color[1] = 1.0f;
		br.outline_color[2] = 1.0f;
		br.outline_width = 4.0f;
		br.outline_opacity = 1.0f;

		//choose color
		if (ms.cur_pos_y >= 140.0f && ms.cur_pos_y <= 170.0f)
		{
			if (ms.cur_pos_x > 320.0f && ms.cur_pos_x < 380.0f)
			{
				graphics::drawRect(350.0f, 155.0f, 60.0f, 30.0f, br);
			}
			else if (ms.cur_pos_x > 380.0f && ms.cur_pos_x < 440.0f)
			{
				graphics::drawRect(410.0f, 155.0f, 60.0f, 30.0f, br);
			}
			else if (ms.cur_pos_x > 440.0f && ms.cur_pos_x < 500.0f)
			{
				graphics::drawRect(470.0f, 155.0f, 60.0f, 30.0f, br);
			}
			else if (ms.cur_pos_x > 500.0f && ms.cur_pos_x < 560.0f)
			{
				graphics::drawRect(530.0f, 155.0f, 60.0f, 30.0f, br);
			}
			else if (ms.cur_pos_x > 560.0f && ms.cur_pos_x < 620.0f)
			{
				graphics::drawRect(590.0f, 155.0f, 60.0f, 30.0f, br);
			}
		}

		//choose ball
		if (ms.cur_pos_y >= 225.0f && ms.cur_pos_y <= 255.0f)
		{
			if (ms.cur_pos_x > 355.0 && ms.cur_pos_x < 385.0f)
			{
				graphics::drawRect(370.0f, 240.0f, 30.0f, 30.0f, br);
			}
			else if (ms.cur_pos_x > 405.0f && ms.cur_pos_x < 435.0f)
			{
				graphics::drawRect(420.0f, 240.0f, 30.0f, 30.0f, br);
			}
			else if (ms.cur_pos_x > 455.0f && ms.cur_pos_x < 485.0f)
			{
				graphics::drawRect(470.0f, 240.0f, 30.0f, 30.0f, br);
			}
			else if (ms.cur_pos_x > 505.0f && ms.cur_pos_x < 535.0f)
			{
				graphics::drawRect(520.0f, 240.0f, 30.0f, 30.0f, br);
			}
			else if (ms.cur_pos_x > 555.0f && ms.cur_pos_x < 585.0f)
			{
				graphics::drawRect(570.0f, 240.0f, 30.0f, 30.0f, br);
			}
		}

		if (not_enough_coins)
		{
			br.fill_color[0] = 1.0f;
			br.fill_color[1] = 1.0f;
			br.fill_color[2] = 1.0f;
			br.fill_opacity = 1.0f;
			
			char message[30];
			sprintf_s(message, "NOT  ENOUGH  COINS");
			graphics::drawText(400.0f, 280.0f, 10, message, br);
			auto end = std::chrono::steady_clock::now();
			std::chrono::duration<double> elapsed_secs = end - sec;
			if (elapsed_secs.count() >= 1.0)
			{
				not_enough_coins = false;
			}
		}

	}

	if (payment && saved_coins >= 5)
	{
		br.fill_color[0] = 1.0f;
		br.fill_color[1] = 0.9f;
		br.fill_color[2] = 0.7f;
		br.fill_opacity = 1.0f;
		br.outline_opacity = 0.0f;
		graphics::drawRect(CANVAS_WIDTH / 2.0f, CANVAS_HEIGHT / 2.0f, 150, 80, br);

		br.texture = std::string(ASSET_PATH) + "coin.png";
		br.outline_opacity = 0.0f;
		graphics::drawRect(CANVAS_WIDTH / 2.0f - 20.0f, CANVAS_HEIGHT / 2.0f - 25.0f, 20.0f, 20.0f, br);
		char coins_requested[10];
		sprintf_s(coins_requested, "x   5");
		br.fill_color[0] = 0.0f;
		br.fill_color[1] = 0.0f;
		br.fill_color[1] = 0.0f;
		graphics::drawText(CANVAS_WIDTH / 2.0f, CANVAS_HEIGHT / 2.0f - 20.0f, 15.0f, coins_requested, br);

		br.fill_color[0] = 1.0f;
		br.fill_color[1] = 0.9f;
		br.fill_color[2] = 0.7f;

		br.texture = std::string(ASSET_PATH) + "choose.png";
		graphics::drawRect(CANVAS_WIDTH / 2.0f - 40.0f, CANVAS_HEIGHT / 2.0f + 20.0f, 30.0f, 30.0f, br);
		br.texture = std::string(ASSET_PATH) + "reject.png";
		graphics::drawRect(CANVAS_WIDTH / 2.0f + 40.0f, CANVAS_HEIGHT / 2.0f + 20.0f, 30.0f, 30.0f, br);
		br.texture = "";

		graphics::getMouseState(ms);
		if (ms.cur_pos_y >= CANVAS_HEIGHT / 2.0f + 5.0f && ms.cur_pos_y <= CANVAS_HEIGHT / 2.0f + 35.0f)
		{

			br.fill_opacity = 0.0f;
			br.outline_color[0] = 1.0f;
			br.outline_color[0] = 1.0f;
			br.outline_color[0] = 1.0f;
			br.outline_width = 3.0f;
			br.outline_opacity = 1.0f;
			
			if (ms.cur_pos_x >= CANVAS_WIDTH / 2.0f - 40.0f - 15.0f && ms.cur_pos_x <= CANVAS_WIDTH / 2.0f - 40.0f + 15.0f)
			{
				graphics::drawRect(CANVAS_WIDTH / 2.0f - 40.0f, CANVAS_HEIGHT / 2.0f + 20.0f, 30.0f, 30.0f, br);
			}
			else if (ms.cur_pos_x >= CANVAS_WIDTH / 2.0f + 40.0f - 15.0f && ms.cur_pos_x <= CANVAS_WIDTH / 2.0f + 40.0f + 15.0f)
			{
				graphics::drawRect(CANVAS_WIDTH / 2.0f + 40.0f, CANVAS_HEIGHT / 2.0f + 20.0f, 30.0f, 30.0f, br);
			}
		}
	}
}

void Game::checkBrickCollision()
{
	if (!ball || !bricks)
	{
		return;
	}

	Disk d1 = ball->getCollisionHull();
	Disk* d2[50][3]; 
	for (int i = 0; i < 50; i++)
	{
		if (bricks[i])
		{
			Disk** disks = bricks[i]->getCollisionHull();
			d2[i][0] = disks[0];
			d2[i][1] = disks[1];
			d2[i][2] = disks[2];
		}
	}
	for (int i = 0; i < 50; i++)
	{
		if (!(bricks[i] == nullptr))
		{
			float dx1 = d1.cx - d2[i][0]->cx;
			float dy1 = d1.cy - d2[i][0]->cy;
			float dx2 = d1.cx - d2[i][1]->cx;
			float dy2 = d1.cy - d2[i][1]->cy;
			float dx3 = d1.cx - d2[i][2]->cx;
			float dy3 = d1.cy - d2[i][2]->cy;

			if (sqrt(dx1 * dx1 + dy1 * dy1) < d1.radius + d2[i][0]->radius ||
				sqrt(dx2 * dx2 + dy2 * dy2) < d1.radius + d2[i][0]->radius ||
				sqrt(dx3 * dx3 + dy3 * dy3) < d1.radius + d2[i][0]->radius)
			{
				if (ball->getY() <= bricks[i]->getY() + BRICK_HEIGHT / 2.0f
					&& ball->getY() >= bricks[i]->getY() - BRICK_HEIGHT / 2.0f)
				{
					if (ball->direction == Ball::UP_LEFT)
					{
						ball->direction = Ball::UP_RIGHT;
					}
					else if (ball->direction == Ball::DOWN_LEFT)
					{
						ball->direction = Ball::DOWN_RIGHT;
					}
					else if (ball->direction == Ball::DOWN_RIGHT)
					{
						ball->direction = Ball::DOWN_LEFT;
					}
					else if (ball->direction == Ball::UP_RIGHT)
					{
						ball->direction = Ball::UP_LEFT;
					}
				}
				else if (ball->getY() > bricks[i]->getY())
				{
					if (ball->direction == Ball::UP_RIGHT)
					{
						ball->direction = Ball::DOWN_RIGHT;
					}
					else if (ball->direction == Ball::UP_LEFT)
					{
						ball->direction = Ball::DOWN_LEFT;
					}
				}
				else if (ball->getY() < bricks[i]->getY())
				{
					if (ball->direction == Ball::DOWN_RIGHT)
					{
						ball->direction = Ball::UP_RIGHT;
					}
					else if (ball->direction == Ball::DOWN_LEFT)
					{
						ball->direction = Ball::UP_LEFT;
					}
				}

				collision = true;
				if (!(coins.find(i) == coins.end()))
				{
					coins[i].setEnable(true);
				}
				if (!(bombs.find(i) == bombs.end()))
				{
					bombs[i].setEnable(true);
				}
				
				ball->ballSound();
				delete bricks[i];
				bricks[i] = nullptr;
				delete d2[i][0];
				delete d2[i][1];
				delete d2[i][2];
				bricks_left--;
				score = score + 50 + bonus * 100;
				bonus++;
				break;
			}
		}
	}
	for (int i = 0; i < 50; i++)
	{
		if (bricks[i])
		{
			delete d2[i][0];
			delete d2[i][1];
			delete d2[i][2];
		}
	}
}

void Game::checkBoardCollision()
{
	if (!ball || !board)
	{
		return;
	}

	Disk d1 = ball->getCollisionHull();
	Disk** d2 = board->getCollisionHull();

	for (int i = 0; i < 7; i++)
	{
		float dx1 = d1.cx - d2[i]->cx;
		float dy1 = d1.cy - d2[i]->cy;
		if (sqrt(dx1 * dx1 + dy1 * dy1) < d1.radius + d2[i]->radius)
		{
			if (ball->getX() <= board->getX())
			{
				ball->direction = Ball::UP_LEFT;
				ball->setSpeedX((board->getX() - ball->getX()) / 50.0f);
				ball->setSpeedY(-(board->getX() - ball->getX()) / 50.0f + 2.0f);

			}
			else if (ball->getX() > board->getX())
			{
				ball->direction = Ball::UP_RIGHT;
				ball->setSpeedX((ball->getX() - board->getX()) / 50.0f);
				ball->setSpeedY(-(board->getX() - ball->getX()) / 50.0f + 2.0f);
			}
			bonus = 0;
			break;
		}	
	}
	delete d2[0];
	delete d2[1];
	delete d2[2];
	delete[] d2;
}

void Game::checkCoinCollision()
{
	for (auto iter = coins.begin(); iter != coins.end();) 
	{
		if (iter->second.getEnable())
		{
			bool col = false;
			if (iter->second.getPosY() >= board->getY())
			{
				coins.erase(iter++);
				break;
			}

			Disk d1 = iter->second.getCollisionHull();
			Disk** d2 = board->getCollisionHull();

			for (int i = 0; i < 7; i++)
			{
				float dx1 = d1.cx - d2[i]->cx;
				float dy1 = d1.cy - d2[i]->cy;
				if (sqrt(dx1 * dx1 + dy1 * dy1) < d1.radius + d2[i]->radius)
				{
					graphics::playSound(std::string(ASSET_PATH) + "coin_drop.mp3", sound_volume);
					saved_coins += 1;
					coins.erase(iter++);
					coin_col = true;
					break;
				}
			}
			delete d2[0];
			delete d2[1];
			delete d2[2];
			delete[] d2;
			if (coin_col)
			{
				coin_col = false;
				break;
			}
		}
		++iter;
	}
}

void Game::checkBombCollision()
{
	for (auto iter = bombs.begin(); iter != bombs.end();)
	{
		if (iter->second.getEnable())
		{
			bool col = false;
			if (iter->second.getPosY() >= board->getY())
			{
				bombs.erase(iter++);
				break;
			}

			Disk d1 = iter->second.getCollisionHull();
			Disk** d2 = board->getCollisionHull();

			for (int i = 0; i < 7; i++)
			{
				float dx1 = d1.cx - d2[i]->cx;
				float dy1 = d1.cy - d2[i]->cy;
				if (sqrt(dx1 * dx1 + dy1 * dy1) < d1.radius + d2[i]->radius)
				{
					graphics::playSound(std::string(ASSET_PATH) + "bomb_sound.mp3", sound_volume);
					bombs.erase(iter++);
					bomb_col = true;
					break;
				}
			}
			delete d2[0];
			delete d2[1];
			delete d2[2];
			delete[] d2;
			if (bomb_col)
			{
				break;
			}
		}
		++iter;
	}
}

void Game::chooseGameColor(float& x, const float& y, float r, float g, float b)
{
	graphics::Brush br;
	br.fill_color[0] = r;
	br.fill_color[1] = g;
	br.fill_color[2] = b;
	graphics::drawRect(x, y, 60, 30, br);
	x += 60;
}

void Game::showBall(float& x, float& y, std::string image)
{
	graphics::Brush br;
	br.texture = std::string(ASSET_PATH) + image;
	graphics::drawRect(x, y, 30.0f, 30.0f, br);
	x += 50;
}
