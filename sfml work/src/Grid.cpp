#include "Grid.h"

const sf::Color EMPTY_COLOR = sf::Color::White;
const sf::Color START_COLOR = sf::Color::Green;
const sf::Color END_COLOR = sf::Color::Red;
const sf::Color WALL_COLOR = sf::Color(50, 50, 50);
const sf::Color VISITED_COLOR = sf::Color(173, 216, 230);
const sf::Color PATH_COLOR = sf::Color::Yellow;
const sf::Color WEIGHT_COLOR = sf::Color(188, 143, 143);
const sf::Color GRID_LINE_COLOR = sf::Color(200, 200, 200);
//Define colors, including one for the new Weight node

/**
 * @brief ** UPDATED CONSTRUCTOR **
 * Now accepts a starting position (x, y).
 */
Grid::Grid(int x, int y, int width, int height, int nodeSize) : gridX(x), gridY(y), nodeSize(nodeSize) {
    this->rows = height / nodeSize;
    this->cols = width / nodeSize;

    nodes.resize(rows);
    for (int i = 0; i < rows; ++i) {
        nodes[i].resize(cols);
        for (int j = 0; j < cols; ++j) {
            nodes[i][j].row = i;
            nodes[i][j].col = j;
            nodes[i][j].shape.setSize(sf::Vector2f(nodeSize, nodeSize));
            // ** UPDATED POSITIONING **
            nodes[i][j].shape.setPosition(gridX + j * nodeSize, gridY + i * nodeSize);
            nodes[i][j].shape.setOutlineThickness(1);
            nodes[i][j].shape.setOutlineColor(GRID_LINE_COLOR);
            setNodeType(i, j, NodeType::Empty);
        }
    }
}

void Grid::draw(sf::RenderWindow& window) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            window.draw(nodes[i][j].shape);
        }
    }
}

/**
 * @brief ** UPDATED MOUSE HANDLING **
 * Now correctly calculates the row and column based on the grid's position.
 */
void Grid::handleMouseInput(sf::RenderWindow& window, bool weightsEnabled) {
    if (sf::Mouse::getPosition(window).x > 1030) return;

    int mouseRow = (sf::Mouse::getPosition(window).y - gridY) / nodeSize;
    int mouseCol = (sf::Mouse::getPosition(window).x - gridX) / nodeSize;

    if (mouseRow < 0 || mouseRow >= rows || mouseCol < 0 || mouseCol >= cols) return;

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && weightsEnabled) {
            if (nodes[mouseRow][mouseCol].type == NodeType::Empty) {
                setNodeType(mouseRow, mouseCol, NodeType::Weight);
            }
        }
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
             if (nodes[mouseRow][mouseCol].type == NodeType::Empty) {
                setNodeType(mouseRow, mouseCol, NodeType::Wall);
            }
        }
        else {
            if (nodes[mouseRow][mouseCol].type == NodeType::Empty) {
                if (!startNode) {
                    setNodeType(mouseRow, mouseCol, NodeType::Start);
                } else if (!endNode) {
                    setNodeType(mouseRow, mouseCol, NodeType::End);
                }
            }
        }
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
        if (nodes[mouseRow][mouseCol].type == NodeType::Empty) {
            setNodeType(mouseRow, mouseCol, NodeType::Wall);
        }
    }
}

// ... (reset, resetWalls, clearPath, setNodeType functions remain the same)
void Grid::reset() {
    startNode = nullptr;
    endNode = nullptr;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            setNodeType(i, j, NodeType::Empty);
        }
    }
}

void Grid::resetWalls() {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (nodes[i][j].type == NodeType::Wall) {
                setNodeType(i, j, NodeType::Empty);
            }
        }
    }
}

void Grid::clearPath() {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            sf::Color currentColor = nodes[i][j].shape.getFillColor();
            if (currentColor != EMPTY_COLOR && currentColor != START_COLOR &&
                currentColor != END_COLOR && currentColor != WALL_COLOR &&
                currentColor != WEIGHT_COLOR)
            {
                if (nodes[i][j].cost > 1) {
                    setNodeType(i, j, NodeType::Weight);
                } else {
                    setNodeType(i, j, NodeType::Empty);
                }
            }
        }
    }
}

void Grid::clearWeights() {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (nodes[i][j].type == NodeType::Weight) {
                setNodeType(i, j, NodeType::Empty);
            }
        }
    }
}

// Fills the entire grid with walls to be "carved" out.
void Grid::fillWithWalls() {
    startNode = nullptr;
    endNode = nullptr;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            setNodeType(i, j, NodeType::Wall);
        }
    }
}

// A simple bounds check.
bool Grid::isValid(int r, int c) {
    return r >= 0 && r < rows && c >= 0 && c < cols;
}

void Grid::finalizeMaze() {
    for (auto& row : nodes) {
        for (auto& node : row) {
            if (node.type == NodeType::Visited) {
                setNodeType(node.row, node.col, NodeType::Empty);
            }
        }
    }
}

void Grid::clearMaze() {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            // If the node is NOT the start or end point, reset it to empty.
            if (&nodes[i][j] != startNode && &nodes[i][j] != endNode) {
                setNodeType(i, j, NodeType::Empty);
            }
        }
    }
}

/**
 * @brief Sets the type of a specific node and updates its visual and logical state.
 *
 * This is a critical helper function that handles all node transformations. It updates
 * the node's type, color, and cost. It also correctly manages the main startNode and
 * endNode pointers for the grid, ensuring there is only ever one of each.
 *
 * @param row The row index of the node to change.
 * @param col The column index of the node to change.
 * @param type The new NodeType to assign to the node.
 */
void Grid::setNodeType(int row, int col, NodeType type) {
    // Before changing the node, check if it's currently the start or end node.
    // If it is, we must nullify the main pointer to it to prevent dangling pointers.
    if (&nodes[row][col] == startNode) {
        startNode = nullptr;
    }
    if (&nodes[row][col] == endNode) {
        endNode = nullptr;
    }

    // Set the node's new type and reset its movement cost to the default value.
    nodes[row][col].type = type;

    // Use a switch statement to handle the specific logic for each node type.
    switch (type) {
        case NodeType::Empty:
            nodes[row][col].shape.setFillColor(EMPTY_COLOR);
            nodes[row][col].cost = 1;
            break;
        case NodeType::Start:
            nodes[row][col].shape.setFillColor(START_COLOR);
            nodes[row][col].cost = 1;
            startNode = &nodes[row][col];  // Assign this node as the grid's official start point.
            break;
        case NodeType::End:
            nodes[row][col].shape.setFillColor(END_COLOR);
            nodes[row][col].cost = 1;
            endNode = &nodes[row][col];    // Assign this node as the grid's official end point.
            break;
        case NodeType::Wall:
            nodes[row][col].shape.setFillColor(WALL_COLOR);
            nodes[row][col].cost = 1;
            break;
        case NodeType::Visited:
            break;
        case NodeType::Path:
            nodes[row][col].shape.setFillColor(PATH_COLOR);
            break;
        case NodeType::Weight:
            nodes[row][col].shape.setFillColor(WEIGHT_COLOR);
            nodes[row][col].cost = 5;      // Weighted "mud" nodes have a higher movement cost.
            break;
    }
}
