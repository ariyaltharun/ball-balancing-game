//
// Created by ariyaltharun on 10/18/25.
//
#pragma once

#include <raylib.h>
#include <accelerometerSensor.hpp>
#include <camera.hpp>
#include <grid.hpp>

class Ball {
private:
    Vector3 position{};
    Vector3 velocity{};
    float radius;
    float acceleration;
    float gravity;
    void moveAlongAxis(float &velocityAxis, float &positionAxis, float accelSenAxis);
    AccelerometerSensor *accelSensor;
    GameCamera *gameCamera;
    void restrictBallMovement();
    bool isOnGrid();
    std::string gameStatus;
public:
    Grid *grid = nullptr;
    Ball(Vector3 initPosition);
    void draw();
    void move();
    std::string getGameStatus();
    ~Ball();
};
