//
// Created by ariyaltharun on 10/18/25.
//
#include <ball.hpp>
#include <logger.hpp>

/* Constructor */
Ball::Ball(Vector3 initPosition) {
    /* Note: For Coordinate Plane Reference for this Game
     * Positive X axis towards right
     * Positive Y axis downwards
     * Positive Z axis into the screen */
    LOGI("Initializing Ball Object");
    radius = 25.0f;
    gravity = 9.8f;
    position = {initPosition.x, initPosition.y, -radius};
    accelSensor = new AccelerometerSensor();
    velocity = {0.0f, 0.0f, 0.0f};
    acceleration = 20.0f;
    gameCamera = GameCamera::getInstance(position);
    grid = new Grid();

    gameStatus = "PLAYING";
}

/* Draw Ball on the screen */
void Ball::draw() {
    DrawSphere(position, radius, PURPLE);
}

void Ball::restrictBallMovement() {
    if (position.x < 0.0f) {
        position.x = std::max<float>(0.0f, position.x);
        velocity.x = 0.0f;
    }
    if (position.y < 0.0f) {
        position.y = std::max<float>(0.0f, position.y);
        velocity.y = 0.0f;
    }
    if (position.x > grid->getXLimit()) {
        position.x = std::min<float>(position.x, grid->getXLimit());
        velocity.x = 0.0f;
    }
    if (position.y > grid->getYLimit()) {
        position.y = std::min<float>(position.y, grid->getYLimit());
        velocity.y = 0.0f;
    }
}

bool Ball::isOnGrid() {
    int cellSz = grid->getCellSz();
    int c = position.x/cellSz, r = position.y/cellSz;
    DrawText(TextFormat("Grid: (%d, %d)", c, r), 10, 30, 10, PURPLE);
    // Logic to say, whether you win or loose
    if (c == (int)grid->getEndPos().x && r == (int)grid->getEndPos().y && fabs(velocity.x) <= 0.1f && fabs(velocity.y) <= 0.1f)
        gameStatus = "WIN";
    if (position.z > 0) gameStatus = "LOOSE";
    if (grid->grid[r][c] == 0 || position.z >= 0) {
        DrawText("Ball is off Grid", 10, 20, 10, RED);
        velocity.z += 20 * gravity * GetFrameTime();
        position.z += velocity.z * GetFrameTime();
        return false;
    }
    velocity.z = 0.0f;
    position.z = -radius;
    DrawText("Ball is on Grid", 10, 20, 10, GREEN);
    return true;
}

/* Update ball position */
void Ball::move() {
    // Move the ball
    moveAlongAxis(velocity.x, position.x, -accelSensor->getSensorValues().x);
    moveAlongAxis(velocity.y, position.y, accelSensor->getSensorValues().y);
    DrawText(TextFormat("Position: (%f, %f, %f)", position.x, position.y, position.z), 10, 10, 10, RED);
    // Restrict the ball movement within the grid
    restrictBallMovement();
    isOnGrid();
    // Update the camera Positon
    gameCamera->updateCamera(position);
}

/* Move the ball along the axis */
void Ball::moveAlongAxis(float &velocityAxis, float &positionAxis, float accelSenAxis) {
    /*
     * velocity = (change in displacement) / (change in time)
     * acceleration = (change in velocity) / (change in time)
     *
     * Magnitude denotes how much the position to be changes
     * Sign denotes which direction to move
     * Velocity Decay = ??? | What is stopping velocity
     *
     * Move the ball only if the ball center is above the block
     *
     * */
    if (position.z <= 0 && fabs(accelSenAxis) >= 1.2f && gameStatus == "PLAYING") {
        velocityAxis += (20*accelSenAxis) * GetFrameTime();
        LOGD("Ball is moving");
        positionAxis += velocityAxis * GetFrameTime();
    } else {
        if (fabs(velocityAxis) > 0.1f) {
            LOGD("Ball is stopping");
            velocityAxis *= 0.98; // Decay Velocity
            positionAxis += velocityAxis * GetFrameTime();
        } else {
            velocityAxis = 0.0f;
        }
    }
}

std::string Ball::getGameStatus() {
    return gameStatus;
}

/* Destructor */
Ball::~Ball() = default;
