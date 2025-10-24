//
// Created by ariyaltharun on 10/18/25.
//

#ifndef BALL_BALANCING_GAME_GRID_HPP
#define BALL_BALANCING_GAME_GRID_HPP

#include <vector>
#include <raylib.h>

class Grid {
private:
    int row = 0; // Denotes Y Axis
    int col = 0; // Denotes X Axis
    int cellSz = 0;
    void initializeStartEnd();
    Vector3 startPos;
    Vector3 endPos;
    int xLimit, yLimit;
public:
    std::vector<std::vector<int>> grid;
    Grid();
    void designGrid();
    void draw();
    Vector3 getStartPos();
    Vector3 getEndPos();
    int getXLimit();
    int getYLimit();
    int getCellSz();
    ~Grid();
};

#endif //BALL_BALANCING_GAME_GRID_HPP
