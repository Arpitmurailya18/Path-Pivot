// ===================================================================================
// == FILE: src/MazeGenerator.cpp ==
// ===================================================================================
//
// AUTHOR: Arpit Jatav
//
// DESCRIPTION: Implements the step-by-step logic for the Randomized Depth-First
// Search (DFS) maze generation algorithm.
//
// ===================================================================================
#include "MazeGenerator.h"
#include <random>
#include <algorithm>

/**
 * @brief Performs one step of the Randomized DFS maze generation.
 * This version directly carves empty paths into a grid filled with walls.
 */
void mazeStep(Grid& grid, MazeGeneratorState& state) {
    // If generation is not active or the stack is empty, the maze is complete.
    if (!state.isGenerating || state.stack.empty()) {
        state.isGenerating = false;
        return;
    }

    // 1. Get the current cell from the top of the stack.
    Node* current = state.stack.top();
    
    // 2. Find all unvisited neighbors that are two cells away.
    //    Checking two cells away is the key to creating paths with walls in between.
    std::vector<Node*> neighbors;
    int r = current->row;
    int c = current->col;
    int dr[] = {-2, 2, 0, 0}; // Check up, down, left, right by 2 cells.
    int dc[] = {0, 0, -2, 2};

    for(int i = 0; i < 4; ++i) {
        int new_r = r + dr[i];
        int new_c = c + dc[i];
        // A valid neighbor must be within the grid and must still be a wall.
        if (grid.isValid(new_r, new_c) && grid.nodes[new_r][new_c].type == NodeType::Wall) {
            neighbors.push_back(&grid.nodes[new_r][new_c]);
        }
    }

    // 3. If unvisited neighbors were found...
    if (!neighbors.empty()) {
        // ...choose one of them randomly.
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(neighbors.begin(), neighbors.end(), g);
        Node* next = neighbors[0];

        // 4. Carve a path between the current cell and the chosen neighbor.
        //    This involves turning both the neighbor cell and the wall between them into empty paths.
        int wall_r = r + (next->row - r) / 2;
        int wall_c = c + (next->col - c) / 2;
        
        grid.setNodeType(wall_r, wall_c, NodeType::Empty);
        grid.setNodeType(next->row, next->col, NodeType::Empty);

        // 5. Push the chosen neighbor to the stack to become the new current cell.
        state.stack.push(next);
    } else {
        // 6. If there are no unvisited neighbors, this path is a dead end. Backtrack.
        //    Pop the current cell from the stack to return to the previous cell.
        state.stack.pop();
    }
}

/**
 * @brief Resets the maze generator state for a new maze.
 */
void resetMazeGenerator(MazeGeneratorState& state) {
    state.isGenerating = false;
    // Efficiently clear the stack by swapping it with an empty one.
    std::stack<Node*> empty;
    state.stack.swap(empty);
}