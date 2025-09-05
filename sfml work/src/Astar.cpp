// ===================================================================================
// FILE: src/AStar.cpp
//
// AUTHOR: Arpit Jatav
//
// DESCRIPTION: Implements the step-by-step logic for the A* pathfinding algorithm.
// ===================================================================================

#include "AStar.h"
#include <cmath> // For heuristic calculation (abs)
#include <limits> // For infinity

/**
 * @brief Calculates the Manhattan distance heuristic between two nodes.
 * @param a The starting node.
 * @param b The ending node.
 * @return The H-cost, an integer estimate of the distance.
 */
int calculateHeuristic(Node* a, Node* b) {
    return std::abs(a->row - b->row) + std::abs(a->col - b->col);
}

void drawCurrentAStarPath(Grid& grid, Node* currentNode, std::map<Node*, Node*>& parentMap);


/**
 * @brief Performs a single step of the A* search algorithm.
 *
 * This function represents one iteration of the main A* loop. It selects the node
 * with the lowest F-cost from the open set, evaluates its neighbors, and updates their
 * costs if a cheaper path is found.
 *
 * @param grid The main pathfinding grid (passed by reference).
 * @param state The current state of the A* search (passed by reference).
 * @param allowDiagonals A boolean flag to enable/disable 8-directional movement.
 */
void aStarStep(Grid& grid, AStarState& state, bool isDiagonal) {
    if (!state.isSearching || state.isComplete) return;

    state.currentLine = 2; // while openSet is not empty
    if (state.openSet.empty()) {
        // If the open set is empty, no path exists.
        state.noPathExists = true;
        state.isSearching = false;
        state.isComplete = true;
        state.currentLine = 17; // return PathNotFound
        return;
    }

    state.currentLine = 3; // current = node with lowest fCost

    // 1. Get the node with the lowest F-cost from the priority queue.
    Node* current = state.openSet.top().node;
    state.openSet.pop();
    state.nodesVisited++; // ** NEW **

    drawCurrentAStarPath(grid, current, state.parentMap);

    state.currentLine = 4; // if current == goal
    if (current == grid.endNode) {
        state.pathCost = state.gCost[current]; // ** NEW **
        state.isComplete = true;
        state.isSearching = false;
        state.currentLine = 5; // return PathFound
        return;
    }

    if (current->type != NodeType::Start) {
        current->type = NodeType::Visited;
        current->shape.setFillColor(sf::Color(173, 216, 230));
    }

    int r = current->row;
    int c = current->col;
    int dr[] = {-1, 1, 0, 0, -1, -1, 1, 1};
    int dc[] = {0, 0, -1, 1, -1, 1, -1, 1};

    state.currentLine = 7; // for each neighbor
    int numDirections = isDiagonal ? 8 : 4;

    // 2. Explore neighbors.
    for (int i = 0; i < numDirections; ++i) {
        int new_r = r + dr[i];
        int new_c = c + dc[i];

        if (new_r >= 0 && new_r < grid.rows && new_c >= 0 && new_c < grid.cols) {
            Node& neighbor = grid.nodes[new_r][new_c];

            if (neighbor.type == NodeType::Wall || neighbor.type == NodeType::Visited) {
                continue;
            }

            state.currentLine = 8; // tentative_gCost = ...
            // 3. Calculate the G-cost to this neighbor through the current node.
            int tentative_gCost = state.gCost[current] + neighbor.cost;

            state.currentLine = 9; // if tentative_gCost < gCost

            // 4. If this path is cheaper than any previous path, update and record it.
            if (tentative_gCost < state.gCost[&neighbor]) {
                state.currentLine = 10; // parent[neighbor] = current
                state.parentMap[&neighbor] = current;
                state.currentLine = 11; // gCost[neighbor] = ...
                state.gCost[&neighbor] = tentative_gCost;
                int hCost = calculateHeuristic(&neighbor, grid.endNode);
                int fCost = tentative_gCost + hCost;
                state.currentLine = 12; // fCost[neighbor] = ...
                state.openSet.push({&neighbor, fCost});
                state.currentLine = 13; // openSet.add(neighbor)

                if (neighbor.type != NodeType::End && neighbor.type != NodeType::Start) {
                    neighbor.shape.setFillColor(sf::Color(200, 255, 200));
                }
            }
        }
    }
}

void resetAStar(AStarState& state) {
    state.isSearching = false;
    state.isComplete = false;
    state.noPathExists = false;
    state.parentMap.clear();
    state.gCost.clear();
    state.currentLine = 0;
    state.nodesVisited = 0; // ** NEW **
    state.pathCost = 0;     // ** NEW **
    std::priority_queue<AStarNode, std::vector<AStarNode>, std::greater<AStarNode>> empty;
    state.openSet.swap(empty);
}

void drawCurrentAStarPath(Grid& grid, Node* currentNode, std::map<Node*, Node*>& parentMap) {
    for (int i = 0; i < grid.rows; ++i) {
        for (int j = 0; j < grid.cols; ++j) {
            if (grid.nodes[i][j].type == NodeType::Path) {
                grid.nodes[i][j].type = NodeType::Visited;
                grid.nodes[i][j].shape.setFillColor(sf::Color(173, 216, 230));
            }
        }
    }
    Node* tracer = currentNode;
    while (tracer != grid.startNode && tracer != nullptr) {
        if (tracer != grid.endNode) {
            tracer->type = NodeType::Path;
            tracer->shape.setFillColor(sf::Color::Yellow);
        }
        tracer = parentMap[tracer];
    }
}
