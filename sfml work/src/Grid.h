// ===================================================================================
// FILE: src/Grid.h
//
// AUTHOR: Arpit Jatav
//
// DESCRIPTION: Defines the Grid class, which manages the 2D grid of nodes for the
// pathfinding mode. It handles the creation, drawing, user interaction (placing
// walls, start/end nodes), and state of the pathfinding environment.
// ===================================================================================
#ifndef GRID_H
#define GRID_H

#include <SFML/Graphics.hpp>
#include <vector>

enum class NodeType { Empty, Start, End, Wall, Visited, Path, Weight };

struct Node {
    sf::RectangleShape shape;
    NodeType type = NodeType::Empty;
    int row, col;
    int cost = 1;
};

/**
 * @brief Manages the 2D grid for the pathfinding visualizer.
 */

class Grid {
public:
    // ** UPDATED CONSTRUCTOR **
    Grid(int x, int y, int width, int height, int nodeSize);

    void draw(sf::RenderWindow& window);
    void handleMouseInput(sf::RenderWindow& window, bool weightsEnabled);
    void reset();
    void resetWalls();
    void clearMaze();
    void clearPath();
    void clearWeights();
    void fillWithWalls();
    bool isValid(int r, int c);

    /**
     * @brief Sets the type of a specific node and updates its visual state.
     * @param row The row index of the node to change.
     * @param col The column index of the node to change.
     * @param type The new NodeType to assign to the node.
     */
    void setNodeType(int row, int col, NodeType type);
    void finalizeMaze();

    std::vector<std::vector<Node>> nodes;
    Node* startNode = nullptr;
    Node* endNode = nullptr;
    int rows, cols;

private:
    int nodeSize;
    int gridX, gridY; // Store the top-left position
};

#endif // GRID_H