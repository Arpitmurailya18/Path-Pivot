# Path & Pivot - An Interactive Algorithm Visualizer

Path & Pivot is an interactive algorithm visualizer designed to demonstrate the real-time execution and behavior of classic sorting and pathfinding algorithms.

The project was originally developed as a **C++/SFML desktop application** and has been extended with a **browser-based version** that allows users to interact with the visualizer directly without installing the desktop application.

> **Live Demo:** [Path & Pivot](https://arpitmurailya18.github.io/Path-Pivot/)

---

## Preview - Windows Desktop version

### Sorting Mode Visualization

![Sorting Mode Screenshot](./assest/Sorting.gif)

*A snapshot of Merge Sort in action.*

### Pathfinding Mode Visualization

![Pathfinding Mode Screenshot](./assest/Pathfinding.gif)

*A* Search finding a path through a user-generated maze.

---

# Features

## Sorting Mode

The sorting visualizer provides step-by-step visualization of the following algorithms:

- Bubble Sort
- Selection Sort
- Insertion Sort
- Merge Sort
- Quick Sort

### Sorting Features

- Generate new arrays
- Adjustable visualization speed
- Start / Pause / Resume
- Reset
- Real-time comparisons
- Swap visualization
- Sorted-element visualization
- Comparison statistics
- Array access statistics
- Algorithm pseudocode
- Current pseudocode line highlighting

---

## Pathfinding Mode

The pathfinding visualizer provides interactive visualization of:

- Breadth-First Search (BFS)
- Depth-First Search (DFS)
- Dijkstra's Algorithm
- A* Search

### Pathfinding Features

- Generate random mazes
- Create custom walls
- Add weighted nodes
- Set custom start point
- Set custom end point
- Optional diagonal traversal
- Adjustable visualization speed
- Start / Pause / Resume
- Clear Visualization
- Reset
- Visited-node visualization
- Open-set visualization
- Final path visualization
- Visited-node statistics
- Path-cost statistics
- Algorithm pseudocode
- Current pseudocode line highlighting

---

# Interactive Visualization

The visualizer uses different colors to make algorithm execution easier to understand.

### Pathfinding Color Legend

| Color | Meaning |
|---|---|
| Green | Start / Final Path |
| Red | End |
| Gray | Wall |
| Purple | Visited Node |
| Orange | Open Set |
| Yellow | Weighted Node |

---

# Tools & Technologies

## Desktop Application

- **Language:** C++
- **Graphics Library:** SFML
- **Compiler:** g++ / MinGW
- **Build Environment:** Windows

## Web Application

- **HTML**
- **CSS**
- **JavaScript**
- **Deployment:** GitHub Pages

## Development Tools

- Git
- GitHub
- Visual Studio Code

---

# Project Structure

```text
Path-Pivot/
│
├── Algo Visualizer/
│   └── sfml work/
│       ├── include/
│       ├── src/
│       ├── assets/
│       └── ...
│
├── web/
│   ├── index.html
│   ├── style.css
│   ├── script.js
│   └── path-pivot-logo.png
│
└── README.md
```
# Installation & Setup
Path & Pivot can be used in two ways:
1. Desktop Application (C++/SFML)
2. Web Application

## Desktop Application
The desktop version is the original C++/SFML implementation.
Option A - Download the Pre-Built Application
This is the easiest way to run Path & Pivot on Windows.

Requirements
- Windows
- No compiler required
- No SFML installation required when using the pre-built release

Installation
1. Go to the Releases page.
2. Download the latest .zip release.
3. Extract the downloaded ZIP file.
4. Open the extracted folder.
5. Run: main.exe

The application will start directly.
Note: The pre-built desktop application is intended for Windows.

## Web Application
The web version allows you to use Path & Pivot directly in a browser.
No C++, SFML, or MinGW installation is required.
Live Demo
You can use the web version directly: https://arpitmurailya18.github.io/Path-Pivot/

# How to Use
When a user visits the web application for the first time, an instruction panel explains how the visualizer works.

## Sorting Mode
1. Select a sorting algorithm.
2. Generate a new array.
3. Adjust the visualization speed.
4. Click Start.
5. Use Pause / Resume to control execution.
6. Use Reset to start over.

## Pathfinding Mode
1. Select a pathfinding algorithm.
2. Generate a new maze or create your own grid.
3. Set the start node.
4. Set the end node.
5. Add walls or weighted nodes if required.
6. Enable diagonal traversal if required.
7. Adjust the visualization speed.
8. Click Start.
9. Use Pause / Resume when required.
10. Use Clear Visualization to remove the search result while keeping the grid.
11. Use Reset to recreate the pathfinding grid.

# Pseudocode Visualization
Path & Pivot displays the pseudocode corresponding to the selected algorithm.
During execution, the currently active pseudocode line is highlighted so users can connect the visual animation with the underlying algorithmic steps.

# Why Path & Pivot?
Path & Pivot was created to make fundamental algorithms easier to understand through interactive visualization.

Instead of only reading algorithm implementations, users can observe:
- How elements move during sorting
- How comparisons and swaps occur
- Which nodes are explored during pathfinding
- How BFS, DFS, Dijkstra, and A* search through a grid
- How weighted nodes affect pathfinding
- How different traversal options affect the search
- How algorithm execution corresponds to its pseudocode
- How algorithm behavior changes with different inputs

# Future Improvements
Possible future improvements include:
- Additional sorting algorithms
- Additional graph algorithms
- More maze-generation techniques
- Algorithm complexity comparison
- Additional visualization customization
- Improved mobile interaction

# Author
**Arpit Jatav**
Computer Science / Software Engineering Student

# License
This project is intended for educational and learning purposes.
