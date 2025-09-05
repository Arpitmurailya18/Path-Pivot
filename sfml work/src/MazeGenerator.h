// ===================================================================================
// == FILE: src/MazeGenerator.h ==
// ===================================================================================
//
// AUTHOR: Arpit Jatav
//
// DESCRIPTION: Header file for the Randomized Depth-First Search (DFS) maze
// generator. Defines the state object and function prototypes required for the
// step-by-step creation of a random maze.
//
// ===================================================================================
#ifndef MAZEGENERATOR_H
#define MAZEGENERATOR_H

#include "Grid.h"
#include <stack>
#include <vector>

/**
 * @brief Holds all state information for a maze generation process in progress.
 *
 * This struct uses a stack to manage the path of the Randomized DFS algorithm,
 * simulating a "carver" that moves through a solid grid of walls.
 */
struct MazeGeneratorState {
    // The stack holds the current path of the carver. The top of the stack is the current cell.
    std::stack<Node*> stack;
    
    // A flag to track if the generation animation is currently active.
    bool isGenerating = false;
};

/**
 * @brief Performs a single step of the maze generation algorithm.
 * @param grid The main pathfinding grid (passed by reference).
 * @param state The current state of the maze generator (passed by reference).
 */
void mazeStep(Grid& grid, MazeGeneratorState& state);

/**
 * @brief Resets the maze generator state to its default values for a new maze.
 * @param state The maze generator state object to reset (passed by reference).
 */
void resetMazeGenerator(MazeGeneratorState& state);

#endif // MAZEGENERATOR_H