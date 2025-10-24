//
// Created by ariyaltharun on 10/18/25.
//
#include <grid.hpp>
#include <logger.hpp>

Grid::Grid() {
    LOGI("Initialize grid object");
    row = 10;
    col = 11;
    cellSz = 50;
    xLimit = col*cellSz;
    yLimit = row*cellSz;
    grid = std::vector(row, std::vector<int>(col, 0));
    designGrid();
    initializeStartEnd();
}

void Grid::draw() {
    for (int r=0; r<row; r++) {
        for (int c=0; c<col; c++) {
            Color color;
            // DrawRectangle(c*cellSz, r*cellSz, cellSz, cellSz, YELLOW);
            if (grid[r][c] == 0) continue;
            switch (grid[r][c]) {
                case 1:
                    color = BROWN;
                    break;
                case 2:
                    color = BLUE;
                    break;
                case 3:
                    color = GREEN;
                    break;
            }
            DrawCube(Vector3{ (float)c*cellSz+cellSz/2.0f, (float)r*cellSz+cellSz/2.0f, 5.0f }, (float)cellSz, (float)cellSz, 10.0f, color);
        }
    }
}

void Grid::designGrid() {
    LOGI("Designing Game Grid");
    grid = {
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0},
            {0, 0, 0, 0, 1, 1, 1, 1, 3, 1, 0},
            {0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0},
            {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0},
            {0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0},
            {0, 0, 0, 1, 2, 1, 0, 0, 0, 0, 0},
            {0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0},
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    };
}

void Grid::initializeStartEnd() {
    LOGI("Initializing start and end point in the grid");
    for (int r=0; r<row; r++)
        for (int c=0; c<col; c++)
            if (grid[r][c] == 2)
                startPos = { c * cellSz + cellSz/2.0f, r * cellSz + cellSz/2.0f, 0.0f };
            else if (grid[r][c] == 3)
                endPos = { (float)c, (float)r, 0.0f };
}

int Grid::getCellSz() {
    return cellSz;
}

Vector3 Grid::getStartPos() {
    return startPos;
}

Vector3 Grid::getEndPos() {
    return endPos;
}

int Grid::getXLimit() {
    return xLimit-1;
}

int Grid::getYLimit() {
    return yLimit-1;
}

Grid::~Grid() {

}
