// ===================================================================================
// == FILE: src/Dijkstra.h ==
// ===================================================================================
//
// AUTHOR: Arpit Jatav
//
// DESCRIPTION: Header file for Dijkstra's pathfinding algorithm. Defines the state
// objects and function prototypes required for the step-by-step visualization,
// specifically for finding the lowest-cost path in a weighted grid.
//
// ===================================================================================
#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "Grid.h"
#include <vector>
#include <map>
#include <queue>

/**
 * @brief A node wrapper for Dijkstra's priority queue.
 *
 * Contains a pointer to a grid node and its associated cost from the start.
 * The overloaded '>' operator allows the priority queue to function as a min-heap,
 * always providing the node with the lowest known cost.
 */
struct DijkstraNode {
    Node* node;
    int cost;

    bool operator>(const DijkstraNode& other) const {
        return cost > other.cost;
    }
};

/**
 * @brief Holds all state information for a Dijkstra's search in progress.
 */
struct DijkstraState {
    // The "open set" of nodes to be evaluated, prioritized by lowest cost from start.
    std::priority_queue<DijkstraNode, std::vector<DijkstraNode>, std::greater<DijkstraNode>> openSet;
    
    // Maps a node to the node from which it was reached. Used to reconstruct the path.
    std::map<Node*, Node*> parentMap;

    // Maps a node to its known cost from the start node.
    std::map<Node*, int> costMap;

    // --- State Flags ---
    bool isSearching = false;
    bool isComplete = false;
    bool noPathExists = false;

    // --- Visualization & Stats ---
    int currentLine = 0;
    int nodesVisited = 0;
    int pathCost = 0;
};

/**
 * @brief Performs a single step of Dijkstra's algorithm.
 * @param grid The main pathfinding grid (passed by reference).
 * @param state The current state of the Dijkstra's search (passed by reference).
 * @param isDiagonal A boolean flag to enable/disable 8-directional movement.
 */
void dijkstraStep(Grid& grid, DijkstraState& state, bool isDiagonal);

/**
 * @brief Resets the Dijkstra's state to its default values for a new search.
 * @param state The Dijkstra's state object to reset (passed by reference).
 */
void resetDijkstra(DijkstraState& state);

#endif // DIJKSTRA_H