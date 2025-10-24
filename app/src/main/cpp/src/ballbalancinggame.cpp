//
// Created by ariyaltharun on 10/19/25.
//

#include <ballbalancing.hpp>
#include <android/input.h>
#include "android_native_app_glue.h"
#include <android/window.h>

BallBalancingGame::BallBalancingGame() {
    // Android App Config to keep the screen on always when this app is opened
    android_app *app = GetAndroidApp();
    ANativeActivity_setWindowFlags(app->activity, AWINDOW_FLAG_KEEP_SCREEN_ON, 0);
    // Game code start from here
    LOGI("Ball Balancing Game Starting..... !!!");
    SetConfigFlags(FLAG_FULLSCREEN_MODE);
    InitWindow(0, 0, "Hi");
    LOGI("Target Frames Per Second is set to: 60");
    SetTargetFPS(60);
    LOGI("Width = %d, Height = %d", SCREEN_WIDTH, SCREEN_HEIGHT);
    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();
    gameState = GameState::START_SCREEN;
}

void BallBalancingGame::startScreen() {
    LOGI("Start Screen!!");
    touch = {0.0f, 0.0f};
    Rectangle start_option = {static_cast<float>(0.1*screenWidth), static_cast<float>(0.4*screenHeight), static_cast<float>(0.8*screenWidth), static_cast<float>(0.07*screenHeight)};
    Rectangle level_option = {static_cast<float>(0.1*screenWidth), static_cast<float>(0.5*screenHeight), static_cast<float>(0.8*screenWidth), static_cast<float>(0.07*screenHeight)};
    Rectangle quit_option = {static_cast<float>(0.1*screenWidth), static_cast<float>(0.6*screenHeight), static_cast<float>(0.8*screenWidth), static_cast<float>(0.07*screenHeight)};
    while (gameState == GameState::START_SCREEN) {
        /* Handle Events */
        listenForTouchPoints();
        if (CheckCollisionPointRec(touch, start_option)) {
            DrawText("Starting Game", 10, 30, 20, BLUE);
            gameState = GameState::GAMEPLAY_SCREEN;
            gamePlay(0);
        } else if (CheckCollisionPointRec(touch, level_option)) {
            DrawText("Options levels: Yet to Implement", 10, 30, 20, BLUE);
            gameState = GameState::LEVEL_SCREEN;
            levelsScreen();
        } else if (CheckCollisionPointRec(touch, quit_option)) {
            DrawText("Quiting Game...", 10, 30, 20, BLUE);
            break;
        }
        /* Draw Graphics */
        BeginDrawing();
        {
            DrawText(TextFormat("Touch at: %f, %f", touch.x, touch.y), 10, 10, 20, RED);
            // Start Option Button
            DrawRectangleRec(start_option, DARKGREEN);
            DrawText("Start Game", 0.3*screenWidth, 0.42*screenHeight, 50, RAYWHITE);
            // Level Option Button
            DrawRectangleRec(level_option, DARKBLUE);
            DrawText("Select Levels", 0.25*screenWidth, 0.52*screenHeight, 50, RAYWHITE);
            // Quit Option Button
            DrawRectangleRec(quit_option, RED);
            DrawText("Quit Game", 0.33*screenWidth, 0.62*screenHeight, 50, RAYWHITE);
            // Footer Text
            DrawText("Made with  by Ariyal with Raylib", 0.25*screenWidth, 0.95*screenHeight, 25, RAYWHITE);
        }
        EndDrawing();
        // Reset Touch
        touch = {0, 0};
    }
}

void BallBalancingGame::levelsScreen() {
    // TODO: Add Levels Screen and Navigate to specific level
    std::vector<Rectangle> rects;
    float levelBoxSz = 0.2*screenWidth;
    rects.push_back({ 0.1f * screenWidth, 0.3f * screenWidth, levelBoxSz, levelBoxSz });
    rects.push_back({ 0.4f * screenWidth, 0.3f * screenWidth, levelBoxSz, levelBoxSz });
    rects.push_back({ 0.7f * screenWidth, 0.3f * screenWidth, levelBoxSz, levelBoxSz });
    int levelDisplay = 1;
    while (gameState == GameState::LEVEL_SCREEN) {
        /* Handle the input */
        listenForTouchPoints();
        // List for level selection
        for (int level = 1; level <= rects.size(); level++) {
            if (CheckCollisionPointRec(touch, rects[level - 1])) {
                gameState = GameState::GAMEPLAY_SCREEN;
                gamePlay(level);
            }
        }
        // Draw the level screen
        BeginDrawing();
        {
            DrawText("Select Levels", 0.13f * screenWidth, 0.05f * screenHeight, 80, BLUE);
            levelDisplay = 1;
            for (Rectangle rect: rects) {
                DrawRectangleRec(rect, GRAY);
                DrawText(TextFormat("%d", levelDisplay++), rect.x + 0.3*levelBoxSz, rect.y + 0.2*levelBoxSz, 100, RED);
            }
        }
        EndDrawing();
        // Reset Touch
        touch = {0.0f, 0.0f};
    }
}

void BallBalancingGame::gamePlay(int level) {
    /* Initialize game object for game level `level` */
    grid = new Grid();
    gameCamera = GameCamera::getInstance(grid->getStartPos());
    ball = new Ball(grid->getStartPos());
    startTime = 0;
    /* Game Loop */
    while (gameState == GameState::GAMEPLAY_SCREEN) {
        /* Update Movable object in game */
        ball->move();
        // After five seconds of game end, return to start screen
        if (startTime > 0 && ((int)GetTime() - startTime) >= 5) {
            gameState = GameState::START_SCREEN;
            startScreen();
            LOGI("Moving to start screen");
            exit(-1);
        }
        /* Draw the objects */
        BeginDrawing();
        ClearBackground(BLACK);
        {
            BeginMode3D(*gameCamera->getCamera());
            {
                grid->draw();
                ball->draw();
            }
            EndMode3D();
            // Conditions to check when you loose or win the game
            if (ball->getGameStatus() == "LOOSE") {
                DrawText("You LOST the game", 10, 40, 10, RED);
                Rectangle prompt = {static_cast<float>(0.1*screenWidth), static_cast<float>(0.4*screenHeight), static_cast<float>(0.8*screenWidth), static_cast<float>(0.07*screenHeight)};
                DrawRectangleRec(prompt, RED);
                DrawText("YOU LOST", 0.33 * screenWidth, 0.42 * screenHeight, 50, RAYWHITE);
                if (startTime == 0) startTime = (int)GetTime();
            } else if (ball->getGameStatus() == "WIN") {
                DrawText("You WON the game", 10, 40, 10, GREEN);
                Rectangle prompt = {static_cast<float>(0.1*screenWidth), static_cast<float>(0.4*screenHeight), static_cast<float>(0.8*screenWidth), static_cast<float>(0.07*screenHeight)};
                DrawRectangleRec(prompt, GREEN);
                DrawText("YOU WON", 0.33 * screenWidth, 0.42 * screenHeight, 50, RAYWHITE);
                if (startTime == 0) startTime = (int)GetTime();
            }
        }
        EndDrawing();
    }
}

void BallBalancingGame::run() {
    /* All Game states should be managed here */
    startScreen();
}

/* Method to listen for touch points and store in variable vector touch */
void BallBalancingGame::listenForTouchPoints() {
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        touch = {(float)GetMouseX(), (float)GetMouseY()};
    }
}

BallBalancingGame::~BallBalancingGame() {
    /* Clean up game resource */
    LOGI("Cleaning up the game");
    LOGD("Closing Raylib Window");
    CloseWindow();
    LOGD("Deleting ball and grid object");
    delete(ball);
    delete(grid);
    LOGI("Exiting game with status code zero (0)");
    exit(0);
}
