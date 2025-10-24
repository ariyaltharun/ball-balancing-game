//
// Created by ariyaltharun on 10/19/25.
//

#pragma once

#include <raylib.h>
#include <logger.hpp>
#include <camera.hpp>
#include <ball.hpp>
#include <grid.hpp>

#define SCREEN_WIDTH GetScreenWidth()
#define SCREEN_HEIGHT GetScreenHeight()

enum GameState {
    START_SCREEN,
    LEVEL_SCREEN,
    GAMEPLAY_SCREEN
};

class BallBalancingGame {
private:
    /* Screen Height & Width */
    int screenHeight, screenWidth;
    /* Game Object */
    Ball *ball{};
    GameCamera *gameCamera{};
    Grid *grid{};

    int gameState;
    void startScreen();
    void levelsScreen();
    void gamePlay(int level);

    /* detect and store touch points */
    Vector2 touch;
    void listenForTouchPoints();
public:
    BallBalancingGame();
    void run();
    ~BallBalancingGame();
};