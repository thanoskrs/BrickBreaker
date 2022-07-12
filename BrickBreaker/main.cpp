#include "graphics.h"
#include "config.h"
#include "game.h"
#include <fstream>



void update(float ms)
{
    Game* game = reinterpret_cast<Game*>(graphics::getUserData());
    game->update();
}

void draw()
{
    Game* game = reinterpret_cast<Game*>(graphics::getUserData());
    game->draw();
}

int main()
{
    Game brickBreaker;
   
	graphics::createWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Brick Breaker");

    graphics::setUserData(&brickBreaker);
   
    graphics::setDrawFunction(draw);
    graphics::setUpdateFunction(update);

    graphics::setCanvasSize(CANVAS_WIDTH, CANVAS_HEIGHT);
    graphics::setCanvasScaleMode(graphics::CANVAS_SCALE_WINDOW );

    brickBreaker.init();
 	graphics::startMessageLoop();

    std::ofstream saveData("playersData.txt", std::ios::out);
    if (!saveData)
    {
        exit(EXIT_FAILURE);
    }
    else
    {
        saveData << brickBreaker.getCoins();
    }
	return 0;
}