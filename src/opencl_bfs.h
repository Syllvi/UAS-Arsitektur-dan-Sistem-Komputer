#pragma once
#include "grid.h"
#include <vector>
#include <iostream>

#ifdef USE_OPENCL
    #ifdef __APPLE__
        #include <OpenCL/cl.h>
    #else
        #include <CL/cl.h>
    #endif
    #define OPENCL_ENABLED 1
#else
    #define OPENCL_ENABLED 0
#endif

using namespace std;

class OpenCLBFS{
public:
    static const char* kernelSource;

    static int run(Grid &g){
        int gridSize = g.rows * g.cols;
        vector<int> h_visited(gridSize, 0);
        vector<int> h_frontier(gridSize, 0);

        vector<int> h_grid_flat(gridSize, 0);
        for(int i = 0; i < g.rows; i++){
            for(int j = 0; j < g.cols; j++){
                h_grid_flat[i * g.cols + j] = g.grid[i][j];
            }
        }

        int start_idx = g.start.x * g.cols + g.start.y;
        int goal_idx = g.goal.x * g.cols + g.goal.y;

        h_frontier[start_idx] = 1;
        h_visited[start_idx] = 1;

#if OPENCL_ENABLED
        cl_int err;
        cl_platform_id platform;
        clGetPlatformIDs(1, &platform, NULL);

        cl_device_id device;
        clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);

        cl_context context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
        cl_command_queue queue = clCreateCommandQueueWithProperties(context, device, 0, &err);

        cl_program program = clCreateProgramWithSource(context, 1, &kernelSource, NULL, &err);
        clBuildProgram(program, 1, &device, NULL, NULL, NULL);
        cl_kernel kernel = clCreateKernel(program, "persistent_bfs_kernel", &err);

        cl_mem d_grid = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int) * gridSize, h_grid_flat.data(), &err);
        cl_mem d_visited = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(int) * gridSize, h_visited.data(), &err);
        cl_mem d_frontier = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(int) * gridSize, h_frontier.data(), &err);
        cl_mem d_next_frontier = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(int) * gridSize, NULL, &err);
        cl_mem d_global_flag = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(int), NULL, &err);

        int host_changed = 1;

        while (host_changed > 0) {
            host_changed = 0;
            clEnqueueWriteBuffer(queue, d_global_flag, CL_TRUE, 0, sizeof(int), &host_changed, 0, NULL, NULL);

            int zero = 0;
            clEnqueueFillBuffer(queue, d_next_frontier, &zero, sizeof(int), 0, sizeof(int) * gridSize, 0, NULL, NULL);

            clSetKernelArg(kernel, 0, sizeof(cl_mem), &d_grid);
            clSetKernelArg(kernel, 1, sizeof(cl_mem), &d_visited);
            clSetKernelArg(kernel, 2, sizeof(cl_mem), &d_frontier);
            clSetKernelArg(kernel, 3, sizeof(cl_mem), &d_next_frontier);
            clSetKernelArg(kernel, 4, sizeof(cl_mem), &d_global_flag);
            clSetKernelArg(kernel, 5, sizeof(int), &g.rows);
            clSetKernelArg(kernel, 6, sizeof(int), &g.cols);
            clSetKernelArg(kernel, 7, sizeof(int), &goal_idx);
            
            size_t global_size = gridSize;
            clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global_size, NULL, 0, NULL, NULL);

            clEnqueueReadBuffer(queue, d_global_flag, CL_TRUE, 0, sizeof(int), &host_changed, 0, NULL, NULL);

            cl_mem temp = d_frontier;
            d_frontier = d_next_frontier;
            d_next_frontier = temp;
        }

        clEnqueueReadBuffer(queue, d_visited, CL_TRUE, sizeof(int) * goal_idx, sizeof(int), &h_visited[goal_idx], 0, NULL, NULL);

        clReleaseMemObject(d_grid);
        clReleaseMemObject(d_visited);
        clReleaseMemObject(d_frontier);
        clReleaseMemObject(d_next_frontier);
        clReleaseMemObject(d_global_flag);
        clReleaseKernel(kernel);
        clReleaseProgram(program);
        clReleaseCommandQueue(queue);
        clReleaseContext(context);

        return h_visited[goal_idx];
#else
        cerr << "OpenCL fallback: OpenCL is not enabled via macro definitions.\n";
        return 0;
#endif
    }
};

const char* OpenCLBFS::kernelSource = R"(
__kernel void persistent_bfs_kernel(__global const int* grid,
                                    __global int* visited,
                                    __global int* frontier,
                                    __global int* next_frontier,
                                    __global int* global_flag,
                                    int rows, int cols, int goal_idx){
    int id = get_global_id(0);
    int gridSize = rows * cols;

    int r = id / cols;
    int c = id % cols;

    int dr[4] = {1, -1, 0, 0};
    int dc[4] = {0, 0, 1, -1};

    for (int step = 0; step < (rows + cols); step++){
        if (id == 0){
            *global_flag = 0;
        }
        barrier(CLK_GLOBAL_MEM_FENCE);

        if (visited[goal_idx] == 1) return;

        if (id < gridSize && frontier[id] == 1){
            for (int d = 0; d < 4; d++){
                int nr = r + dr[d];
                int nc = c + dc[d];

                if (nr >= 0 && nr < rows && nc >= 0 && nc < cols){
                    int nid = nr * cols + nc;

                    if (grid[nid] == 0 && visited[nid] == 0){
                        if (atomic_cmpxchg(&visited[nid], 0, 1) == 0){
                            next_frontier[nid] = 1;
                            atomic_xchg(global_flag, 1);
                        }
                    }
                }
            }
        }

        barrier(CLK_GLOBAL_MEM_FENCE);

        if (id < gridSize) {
            frontier[id] = next_frontier[id];
            next_frontier[id] = 0;
        }
        
        barrier(CLK_GLOBAL_MEM_FENCE);

        if (*global_flag == 0) break;
    }
}
)";