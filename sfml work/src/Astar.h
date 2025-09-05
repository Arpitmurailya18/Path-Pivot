// ===================================================================================
// FILE: src/AStar.h
//
// AUTHOR: Arpit Jatav
//
// DESCRIPTION: Header file for the A* pathfinding algorithm. Defines the state
// objects and function prototypes required for the step-by-step visualization.
// ===================================================================================

#ifndef ASTAR_H
#define ASTAR_H

#include "Grid.h"
#include <vector>
#include <map>
#include <queue> // For the priority queue

/**
 * @brief A node wrapper for the A* priority queue.
 *
 * Contains a pointer to a grid node and its associated F-cost (G-cost + H-cost).
 * The overloaded '>' operator allows the priority queue to function as a min-heap,
 * always providing the node with the lowest F-cost.
 */
struct AStarNode {
    Node* node;
    int fCost;

    // Overload the > operator for the priority queue (min-heap)
    bool operator>(const AStarNode& other) const {
        return fCost > other.fCost;
    }
};

/**
 * @brief Holds all state information for an A* search in progress.
 */

struct AStarState {
    // The "open set" of nodes to be evaluated, prioritized by lowest F-cost.
    std::priority_queue<AStarNode, std::vector<AStarNode>, std::greater<AStarNode>> openSet;

    // Maps a node to the node from which it was reached. Used to reconstruct the path.
    std::map<Node*, Node*> parentMap;

    // Maps a node to its G-cost (the known cost from the start node).
    std::map<Node*, int> gCost; // Cost from start to node

    bool isSearching = false;
    bool isComplete = false;
    bool noPathExists = false;
    int currentLine = 0;
    int nodesVisited = 0; // ** NEW **
    int pathCost = 0;     // ** NEW **
};

// Function prototypes
int calculateHeuristic(Node* a, Node* b);
void aStarStep(Grid& grid, AStarState& state, bool isDiagonal);
void resetAStar(AStarState& state);

#endif // ASTAR_H