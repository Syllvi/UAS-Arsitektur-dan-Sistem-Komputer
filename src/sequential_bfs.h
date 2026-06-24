#pragma once
#include "grid.h"
#include <queue>
#include <vector>
#include <fstream>
#include <string>
using namespace std;

class SequentialBFS{
public:
    static int run(Grid &g){
        int gridSize = g.rows * g.cols;
        vector<int> visited(gridSize, 0);
        vector<int> parent(gridSize, -1); 

        queue<int> q;
        int start_idx = g.start.x * g.cols + g.start.y;
        int goal_idx = g.goal.x * g.cols + g.goal.y;

        q.push(start_idx);
        visited[start_idx] = 1;

        int dx[4] = {1, -1, 0, 0};
        int dy[4] = {0, 0, 1, -1};

        bool found = false;

        while (!q.empty()){
            int cur_idx = q.front();
            q.pop();

            if (cur_idx == goal_idx){
                found = true;
                break;
            }

            int r = cur_idx / g.cols;
            int c = cur_idx % g.cols;

            for (int i = 0; i < 4; i++){
                int nr = r + dx[i];
                int nc = c + dy[i];

                if (nr >= 0 && nr < g.rows && nc >= 0 && nc < g.cols && g.grid[nr][nc] == 0){
                    int n_idx = nr * g.cols + nc;
                    if (!visited[n_idx]) {
                        visited[n_idx] = 1;
                        parent[n_idx] = cur_idx; 
                        q.push(n_idx);
                    }
                }
            }
        }

        if (found){
            ofstream fout("path.txt");
            fout << g.rows << " " << g.cols << "\n";
            
            for (int i = 0; i < g.rows; i++){
                for (int j = 0; j < g.cols; j++) {
                    fout << g.grid[i][j] << (j == g.cols - 1 ? "" : " ");
                }
                fout << "\n";
            }

            vector<string> path_coordinates;
            int curr = goal_idx;
            while (curr != -1){
                int r = curr / g.cols;
                int c = curr % g.cols;
                path_coordinates.push_back(to_string(r) + " " + to_string(c));
                curr = parent[curr];
            }

            fout << path_coordinates.size() << "\n";
            for (int i = path_coordinates.size() - 1; i >= 0; i--){
                fout << path_coordinates[i] << "\n";
            }
            fout.close();
            return 1;
        }

        return 0;
    }
};