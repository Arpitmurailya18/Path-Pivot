// ===================================================================================
// == FILE: src/BFS.h ==
// ===================================================================================
//
// AUTHOR: Arpit Jatav
//
// DESCRIPTION: Header file for the Breadth-First Search (BFS) pathfinding algorithm.
// Defines the state object and function prototypes required for the step-by-step
// visualization of the algorithm.
//
// ===================================================================================
#ifndef BFS_H
#define BFS_H
#include "Grid.h"
#include <queue>
#include <map>

/**
 * @brief Holds all state information for a Breadth-First Search in progress.
 *
 * This struct contains the queue for managing nodes to visit, a map to reconstruct
 * the path, and various flags and counters for controlling the visualization and
// tracking statistics.
 */
struct BFSState {
    // The core data structure for BFS, ensuring a level-by-level search.
    std::queue<Node*> queue;
    
    // Maps a node to the node from which it was reached. Used to reconstruct the path.
    std::map<Node*, Node*> parentMap;
    
    // --- State Flags ---
    bool isSearching = false;  // True while the algorithm is actively running.
    bool isComplete = false;   // True when the algorithm has finished (found a path or not).
    bool noPathExists = false; // True if the queue becomes empty before the end is found.

    // --- Visualization & Stats ---
    int currentLine = 0;   // The current line of pseudocode to highlight.
    int nodesVisited = 0;  // A counter for the total number of nodes processed.
    int pathCost = 0;      // The total cost of the final path found.
};

/**
 * @brief Performs a single step of the BFS algorithm.
 * @param grid The main pathfinding grid (passed by reference).
 * @param state The current state of the BFS search (passed by reference).
 * @param isDiagonal A boolean flag to enable/disable 8-directional movement.
 */
void bfsStep(Grid& grid, BFSState& state, bool isDiagonal);

/**
 * @brief Resets the BFS state to its default values for a new search.
 * @param state The BFS state object to reset (passed by reference).
 */
void resetBFS(BFSState& state);
#endif // BFS_H