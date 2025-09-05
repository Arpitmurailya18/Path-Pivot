// ===================================================================================
// == FILE: src/DFS.cpp ==
// ===================================================================================
//
// AUTHOR: Arpit Jatav
//
// DESCRIPTION: Implements the step-by-step logic for the Depth-First Search
// (DFS) pathfinding algorithm, including the live path visualization and
// statistics tracking.
//
// ===================================================================================
#include "DFS.h"

/**
 * @brief A helper function to visualize the current path being explored by DFS.
 *
 * This function is responsible for the "live path" effect. It clears the previous
 * yellow path and then traces backwards from the current node to draw the new path.
 *
 * @param grid The main pathfinding grid.
 * @param currentNode The node the algorithm is currently processing.
 * @param parentMap The map used to trace the path backwards.
 */
void drawCurrentDFSPath(Grid& grid, Node* currentNode, std::map<Node*, Node*>& parentMap) {
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
        if (tracer != grid.endNode) {
            tracer->type = NodeType::Path;
            tracer->shape.setFillColor(sf::Color::Yellow);
        }
        tracer = parentMap[tracer];
    }
}


void dfsStep(Grid& grid, DFSState& state, bool isDiagonal) {
    if (!state.isSearching || state.isComplete) {
        return;
    }

    // If the stack is empty, it means we've explored every possible branch.
    state.currentLine = 3; // while S is not empty do
    if (state.stack.empty()) {
        state.noPathExists = true;
        state.isSearching = false;
        state.isComplete = true;
        state.currentLine = 15; // return PathNotFound
        return;
    }

    // 1. Get the next node to process from the top of the stack.
    Node* current = state.stack.top();
    state.stack.pop();
    state.nodesVisited++; // Increment the statistics counter.
    state.currentLine = 4; // current = S.pop()

    // Mark as visited if it's not already. This prevents getting stuck in cycles.
    if (current->type == NodeType::Empty) {
        state.currentLine = 6; // mark current as visited
        current->type = NodeType::Visited;
        current->shape.setFillColor(sf::Color(173, 216, 230));
    }
    
    // 2. Visualize the current exploration path.
    drawCurrentDFSPath(grid, current, state.parentMap);

    // 3. Find and process all valid neighbors.
    int r = current->row;
    int c = current->col;
    int dr[] = {-1, 1, 0, 0, -1, -1, 1, 1};
    int dc[] = {0, 0, -1, 1, -1, 1, -1, 1}; 

    state.currentLine = 10; // for each neighbor of current do
    int numDirections = isDiagonal ? 8 : 4;
    for (int i = 0; i < numDirections; ++i) {
        int new_r = r + dr[i];
        int new_c = c + dc[i];

        // Ensure the neighbor is within the grid boundaries.
        if (new_r >= 0 && new_r < grid.rows && new_c >= 0 && new_c < grid.cols) {
            Node& neighbor = grid.nodes[new_r][new_c];

            // If we found the end node, the search is complete.
            state.currentLine = 7; // if current is endNode then
            if (&neighbor == grid.endNode) {
                state.parentMap[&neighbor] = current;
                drawCurrentDFSPath(grid, &neighbor, state.parentMap);
                
                // Calculate the final path cost by tracing backwards.
                Node* tracer = &neighbor;
                while(tracer != nullptr) {
                    state.pathCost += tracer->cost;
                    tracer = state.parentMap[tracer];
                }

                state.isComplete = true;
                state.isSearching = false;
                state.currentLine = 8; // return PathFound
                return;
            }

            // If the neighbor is an unvisited empty square, push it to the stack to explore next.
            if (neighbor.type == NodeType::Empty) {
                state.parentMap[&neighbor] = current;
                state.stack.push(&neighbor);
                state.currentLine = 11; // S.push(neighbor)
            }
        }
    }
}


void resetDFS(DFSState& state) {
    state.isSearching = false;
    state.isComplete = false;
    state.noPathExists = false;
    state.parentMap.clear();
    state.currentLine = 0;
    state.nodesVisited = 0;
    state.pathCost = 0;
    
    // Efficiently clear the stack.
    std::stack<Node*> empty;
    state.stack.swap(empty);
}