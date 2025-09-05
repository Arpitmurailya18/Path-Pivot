// ===================================================================================
// == FILE: src/Dijkstra.cpp ==
// ===================================================================================
//
// AUTHOR: Arpit Jatav
//
// DESCRIPTION: Implements the step-by-step logic for Dijkstra's pathfinding
// algorithm, including handling of weighted nodes and statistics tracking.
//
// ===================================================================================
#include "Dijkstra.h"
#include <limits>

// Define specific colors for visualizing visited nodes, distinguishing between normal and weighted ("mud") nodes.
const sf::Color VISITED_COLOR = sf::Color(173, 216, 230);
const sf::Color VISITED_WEIGHT_COLOR = sf::Color(135, 168, 182); // A "muddy blue"

void drawCurrentDijkstraPath(Grid& grid, Node* currentNode, std::map<Node*, Node*>& parentMap);


void dijkstraStep(Grid& grid, DijkstraState& state, bool isDiagonal) {
    if (!state.isSearching || state.isComplete) return;

    state.currentLine = 3; // while Q is not empty
    if (state.openSet.empty()) {
        state.noPathExists = true;
        state.isSearching = false;
        state.isComplete = true;
        state.currentLine = 14; // end procedure
        return;
    }

    // 1. Get the node with the lowest cost from the priority queue.
    state.currentLine = 4; // u = vertex in Q with min distance
    Node* current = state.openSet.top().node;
    state.openSet.pop();
    state.nodesVisited++;
    state.currentLine = 5; // remove u from Q

    // 2. Immediately check if we've reached the end.
    // This is done first to prevent the end node's color from ever changing.
    if (current == grid.endNode) {
        drawCurrentDijkstraPath(grid, current, state.parentMap);
        // The final path cost is simply the cost recorded in our map for the end node.
        state.pathCost = state.costMap[current];
        state.isComplete = true;
        state.isSearching = false;
        return;
    }

    // Optimization: If we've already processed this node, skip it.
    // This can happen if a node is added to the queue multiple times with different costs.
    if (current->type == NodeType::Visited) {
        return;
    }

    // 3. Mark the current node as visited.
    // We use a special color if the visited node is a high-cost "mud" node.
    if (current->type != NodeType::Start) {
        current->type = NodeType::Visited;
        if (current->cost > 1) {
            current->shape.setFillColor(VISITED_WEIGHT_COLOR);
        } else {
            current->shape.setFillColor(VISITED_COLOR);
        }
    }

    // Update the live path visualization.
    drawCurrentDijkstraPath(grid, current, state.parentMap);

    // 4. Explore all valid neighbors.
    int r = current->row;
    int c = current->col;
    int dr[] = {-1, 1, 0, 0, -1, -1, 1, 1};
    int dc[] = {0, 0, -1, 1, -1, 1, -1, 1};

    state.currentLine = 6; // for each neighbor
    int numDirections = isDiagonal ? 8 : 4;
    for (int i = 0; i < numDirections; ++i) {
        int new_r = r + dr[i];
        int new_c = c + dc[i];

        if (new_r >= 0 && new_r < grid.rows && new_c >= 0 && new_c < grid.cols) {
            Node& neighbor = grid.nodes[new_r][new_c];

            if (neighbor.type == NodeType::Wall) {
                continue; // Skip walls.
            }

            // 5. Calculate the cost to reach this neighbor through the current node.
            state.currentLine = 7; // alt = dist[u] + length(u, v)
            int newCost = state.costMap[current] + neighbor.cost;

            // 6. If this path is cheaper than any previously found path, record it.
            state.currentLine = 8; // if alt < dist[v]
            if (newCost < state.costMap[&neighbor]) {
                state.currentLine = 9; // dist[v] = alt
                state.costMap[&neighbor] = newCost;
                state.currentLine = 10; // prev[v] = u
                state.parentMap[&neighbor] = current;
                state.openSet.push({&neighbor, newCost});

                // Visually mark the neighbor as being in the "open set".
                if (neighbor.type != NodeType::End && neighbor.type != NodeType::Start) {
                    neighbor.shape.setFillColor(sf::Color(200, 255, 200));
                }
            }
        }
    }
}

void resetDijkstra(DijkstraState& state) {
    state.isSearching = false;
    state.isComplete = false;
    state.noPathExists = false;
    state.parentMap.clear();
    state.costMap.clear();
    state.currentLine = 0;
    state.nodesVisited = 0;
    state.pathCost = 0;
    
    // Efficiently clear the priority queue.
    std::priority_queue<DijkstraNode, std::vector<DijkstraNode>, std::greater<DijkstraNode>> empty;
    state.openSet.swap(empty);
}

void drawCurrentDijkstraPath(Grid& grid, Node* currentNode, std::map<Node*, Node*>& parentMap) {
    for (int i = 0; i < grid.rows; ++i) {
        for (int j = 0; j < grid.cols; ++j) {
            if (grid.nodes[i][j].type == NodeType::Path) {
                grid.nodes[i][j].type = NodeType::Visited;
                // When clearing the old path, revert to the correct "visited" color.
                if(grid.nodes[i][j].cost > 1) {
                    grid.nodes[i][j].shape.setFillColor(VISITED_WEIGHT_COLOR);
                } else {
                    grid.nodes[i][j].shape.setFillColor(VISITED_COLOR);
                }
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