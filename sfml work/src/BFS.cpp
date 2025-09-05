// ===================================================================================
// == FILE: src/BFS.cpp ==
// ===================================================================================
//
// AUTHOR: Arpit Jatav
//
// DESCRIPTION: Implements the step-by-step logic for the Breadth-First Search
// (BFS) pathfinding algorithm, including the live path visualization and
// statistics tracking.
//
// ===================================================================================
#include "BFS.h"

/**
 * @brief A helper function to visualize the current path being explored.
 * * This function is called in every step of the search to create the "live path"
 * effect. It first clears any previous yellow path back to the "visited" color,
 * then traces backwards from the current node to the start, drawing the new path.
 * * @param grid The main pathfinding grid.
 * @param currentNode The node the algorithm is currently processing.
 * @param parentMap The map used to trace the path backwards.
 */
void drawCurrentPath(Grid& grid, Node* currentNode, std::map<Node*, Node*>& parentMap) {
    // 1. Clear any old path nodes back to 'visited' color.
    for (int i = 0; i < grid.rows; ++i) {
        for (int j = 0; j < grid.cols; ++j) {
            if (grid.nodes[i][j].type == NodeType::Path) {
                grid.nodes[i][j].type = NodeType::Visited;
                grid.nodes[i][j].shape.setFillColor(sf::Color(173, 216, 230)); // Visited color
            }
        }
    }

    // 2. Draw the new path from the start to the current node in yellow.
    Node* tracer = currentNode;
    while (tracer != grid.startNode && tracer != nullptr) {
        if (tracer != grid.endNode) { // Don't recolor the red end node
            tracer->type = NodeType::Path;
            tracer->shape.setFillColor(sf::Color::Yellow);
        }
        tracer = parentMap[tracer];
    }
}


void bfsStep(Grid& grid, BFSState& state, bool isDiagonal) {
    if (!state.isSearching || state.isComplete) return;

    // If the queue is empty, it means we've explored every reachable node.
    if (state.queue.empty()) {
        state.noPathExists = true;
        state.isSearching = false;
        state.isComplete = true;
        state.currentLine = 16;
        return;
    }
    
    // 1. Get the next node to process from the front of the queue.
    state.currentLine = 4;
    Node* current = state.queue.front();
    state.queue.pop();
    state.nodesVisited++; // Increment the statistics counter.
    state.currentLine = 5;

    // Update the visual representation of the current path.
    drawCurrentPath(grid, current, state.parentMap);
    
    // 2. Check if the current node is the destination.
    state.currentLine = 6;
    if (current == grid.endNode) {
        // If the path is found, calculate the final path cost by tracing backwards.
        Node* tracer = grid.endNode;
        while(tracer != nullptr) {
            state.pathCost += tracer->cost;
            tracer = state.parentMap[tracer];
        }
        state.isComplete = true;
        state.isSearching = false;
        state.currentLine = 7;
        return;
    }

    // 3. Explore the neighbors of the current node.
    int r = current->row;
    int c = current->col;
    int dr[] = {-1, 1, 0, 0, -1, -1, 1, 1}; // Deltas for up, down, left, right, and diagonals
    int dc[] = {0, 0, -1, 1, -1, 1, -1, 1};

    state.currentLine = 9;
    int numDirections = isDiagonal ? 8 : 4; // Check 8 directions for diagonal, 4 otherwise.
    for (int i = 0; i < numDirections; ++i) {
        int new_r = r + dr[i];
        int new_c = c + dc[i];

        // Ensure the neighbor is within the grid boundaries.
        if (new_r >= 0 && new_r < grid.rows && new_c >= 0 && new_c < grid.cols) {
            Node& neighbor = grid.nodes[new_r][new_c];
            
            // Check for the end node here as well for immediate completion.
            state.currentLine = 10;
            if (&neighbor == grid.endNode) {
                state.parentMap[&neighbor] = current;
                drawCurrentPath(grid, &neighbor, state.parentMap);
                // Calculate final path cost.
                Node* tracer = &neighbor;
                while(tracer != nullptr) {
                    state.pathCost += tracer->cost;
                    tracer = state.parentMap[tracer];
                }
                state.isComplete = true;
                state.isSearching = false;
                state.currentLine = 7;
                return;
            }

            // If the neighbor is an unvisited empty square, process it.
            if (neighbor.type == NodeType::Empty) {
                state.currentLine = 11;
                neighbor.type = NodeType::Visited; // Mark as visited to avoid re-processing.
                neighbor.shape.setFillColor(sf::Color(173, 216, 230));
                state.parentMap[&neighbor] = current; // Record the path.
                state.queue.push(&neighbor);          // Add to the queue to visit later.
                state.currentLine = 12;
            }
        }
    }
}

void resetBFS(BFSState& state) {
    state.isSearching = false;
    state.isComplete = false;
    state.noPathExists = false;
    state.parentMap.clear();
    std::queue<Node*> empty;
    state.queue.swap(empty);
    state.currentLine = 0;
    state.nodesVisited = 0;
    state.pathCost = 0;
}