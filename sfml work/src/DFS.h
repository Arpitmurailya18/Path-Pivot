// ===================================================================================
// == FILE: src/DFS.h ==
// ===================================================================================
//
// AUTHOR: Arpit Jatav
//
// DESCRIPTION: Header file for the Depth-First Search (DFS) pathfinding algorithm.
// Defines the state object and function prototypes required for the step-by-step
// visualization of the algorithm.
//
// ===================================================================================
#ifndef DFS_H
#define DFS_H

#include "Grid.h"
#include <stack>
#include <vector>
#include <map>

/**
 * @brief Holds all state information for a Depth-First Search in progress.
 *
 * This struct contains the stack for managing nodes to visit (LIFO - Last-In, First-Out),
 * a map to reconstruct the path, and various flags and counters for controlling
 * the visualization and tracking statistics.
 */
struct DFSState {
    // The core data structure for DFS, ensuring a depth-first exploration.
    std::stack<Node*> stack;

    // Maps a node to the node from which it was reached. Used to reconstruct the path.
    std::map<Node*, Node*> parentMap;

    // --- State Flags ---
    bool isSearching = false;  // True while the algorithm is actively running.
    bool isComplete = false;   // True when the algorithm has finished.
    bool noPathExists = false; // True if the stack becomes empty before the end is found.

    // --- Visualization & Stats ---
    int currentLine = 0;   // The current line of pseudocode to highlight.
    int nodesVisited = 0;  // A counter for the total number of nodes processed.
    int pathCost = 0;      // The total cost of the final path found.
};

/**
 * @brief Performs a single step of the DFS algorithm.
 * @param grid The main pathfinding grid (passed by reference).
 * @param state The current state of the DFS search (passed by reference).
 * @param isDiagonal A boolean flag to enable/disable 8-directional movement.
 */
void dfsStep(Grid& grid, DFSState& state, bool isDiagonal);

/**
 * @brief Resets the DFS state to its default values for a new search.
 * @param state The DFS state object to reset (passed by reference).
 */
void resetDFS(DFSState& state);

#endif // DFS_H