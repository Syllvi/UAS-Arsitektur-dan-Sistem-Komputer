#pragma once
#include "grid.h"
#include <vector>
#include <omp.h>
using namespace std;

class OpenMPBFS{
public:
    static int run(Grid &g){
        vector<vector<int>> visited(g.rows, vector<int>(g.cols, 0));

        vector<Point> frontier;
        frontier.push_back(g.start);
        visited[g.start.x][g.start.y] = 1;

        int dx[4] = {1, -1, 0, 0};
        int dy[4] = {0, 0, 1, -1};
        
        bool found = false;

        while (!frontier.empty() && !found){
            vector<Point> next;

            int max_threads = omp_get_max_threads();
            vector<vector<Point>> thread_local_frontiers(max_threads);

            #pragma omp parallel
            {
                int tid = omp_get_thread_num();
                
                #pragma omp for nowait
                for (int i = 0; i < (int)frontier.size(); i++){
                    if (found) continue;

                    Point cur = frontier[i];

                    if (cur.x == g.goal.x && cur.y == g.goal.y){
                        found = true;
                        continue;
                    }

                    for (int d = 0; d < 4; d++){
                        int nx = cur.x + dx[d];
                        int ny = cur.y + dy[d];

                        if (g.isValid(nx, ny)){
                            if (!visited[nx][ny]){
                                int already_visited;
                                #pragma omp atomic capture
                                {
                                    already_visited = visited[nx][ny];
                                    visited[nx][ny] = 1;
                                }

                                if (already_visited == 0) {
                                    thread_local_frontiers[tid].push_back({nx, ny});
                                }
                            }
                        }
                    }
                }
            }

            size_t total_size = 0;
            for (int i = 0; i < max_threads; i++){
                total_size += thread_local_frontiers[i].size();
            }
            next.reserve(total_size);

            for (int i = 0; i < max_threads; i++){
                next.insert(next.end(), thread_local_frontiers[i].begin(), thread_local_frontiers[i].end());
            }

            frontier = move(next);
        }

        return visited[g.goal.x][g.goal.y];
    }
};