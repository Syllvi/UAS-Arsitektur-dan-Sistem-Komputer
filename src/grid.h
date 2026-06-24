#pragma once
#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>

using namespace std;

struct Point{
    int x, y;
};

class Grid{
public:
    int rows, cols;
    vector<vector<int>> grid;

    Point start, goal;

    Grid(int r, int c) : rows(r), cols(c){
        grid.resize(rows, vector<int>(cols, 0));
    }

    void generateRandom(double obstacleProb = 0.2){
        for (int i = 0; i < rows; i++){
            for (int j = 0; j < cols; j++){
                if (i == j || i == j + 1 || i == j - 1){
                    grid[i][j] = 0; 
                } else{
                    grid[i][j] = ((i * 31 + j * 17) % 100 < 20) ? 1 : 0;
                }
            }
        }

        start = {0, 0};
        goal = {rows - 1, cols - 1};

        grid[start.x][start.y] = 0;
        grid[goal.x][goal.y] = 0;
    }

    bool isValid(int x, int y) {
        return x >= 0 && y >= 0 && x < rows && y < cols && grid[x][y] == 0;
    }

    void print() {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                cout << grid[i][j] << " ";
            }
            cout << "\n";
        }
    }
};