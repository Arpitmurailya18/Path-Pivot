// ===================================================================================
// FILE: main.cpp
//
// AUTHOR: Arpit Jatav
//
// DESCRIPTION: The core application file for the "Path & Pivot" algorithm
// visualizer. This file contains the main application loop, handles all SFML
// window events, manages the application state (Home, Sorting, Pathfinding),
// and draws all UI elements to the screen.
// ===================================================================================

#include <bits/stdc++.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include "src/BubbleSort.h"
#include "src/SelectionSort.h"
#include "src/InsertionSort.h"
#include "src/VisualizerColor.h"
#include "src/MergeSort.h"
#include "src/QuickSort.h"
#include "src/Grid.h"
#include "src/BFS.h"
#include "src/DFS.h"
#include "src/Astar.h"
#include "src/Dijkstra.h"
#include "src/MazeGenerator.h"
#include "src/Pseudocode.h"
#include "src/Homepage.h"


using namespace sf;
using namespace std;

enum class Mode {Home, Sorting, Pathfinding};


/**
 * @brief Creates a new Button object with a centered label.
 * @param x The top-left x-coordinate of the button.
 * @param y The top-left y-coordinate of the button.
 * @param w The width of the button.
 * @param h The height of the button.
 * @param text The string to display on the button's label.
 * @param font The Font to use for the label.
 * @return A new Button struct object.
 */

struct Button
{
    RectangleShape shape;
    Text label;
    Color defaultColor;
    Color hoverColor;
};

Button createButton(float x, float y, float w, float h, const string text, Font &font) {
    Button button;
    button.shape.setSize({w, h});
    button.shape.setOrigin(w / 2.0f, h / 2.0f);
    button.shape.setPosition(x + w / 2.0f, y + h / 2.0f);
    button.defaultColor = Color(80, 80, 150);
    button.hoverColor = Color(120, 120, 180);
    button.shape.setFillColor(button.defaultColor);
    button.shape.setOutlineColor(Color::White);
    button.shape.setOutlineThickness(2);

    button.label.setFont(font);
    button.label.setString(text);
    button.label.setCharacterSize(20);
    button.label.setFillColor(Color::White);
    FloatRect textRect = button.label.getLocalBounds();
    button.label.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    button.label.setPosition(x + w / 2.0f, y + h / 2.0f);
    return button;
}

// Function to reposition button
void reposition(Button& button, float newX, float newY) {
    float buttonWidth = button.shape.getSize().x;
    float buttonHeight = button.shape.getSize().y;

    // Set the new position, accounting for the centered origin
    button.shape.setPosition(newX + buttonWidth / 2.0f, newY + buttonHeight / 2.0f);
    button.label.setPosition(newX + buttonWidth / 2.0f, newY + buttonHeight / 2.0f);
}


/**
 * @brief Represents a single UI dropdown menu, holding its visual components and state.
 *
 * This struct contains all the necessary SFML objects and state flags to manage
 * a dropdown, including its options, animation, and whether it's open or closed.
 */
struct Dropdown
{
    RectangleShape box;         // The main, visible background box of the dropdown.
    Text selected;              // The text that displays the currently selected item.
    vector<Text> options;       // A list of all the Text objects for each available option.
    bool expanded = false;      // A flag to track if the dropdown menu is currently open.
    Clock animationClock;       // An SFML clock to time the fade-in animation.
    float animationDuration = 0.2f; // The duration of the fade-in effect in seconds.
};

/**
 * @brief Clears and repopulates a Dropdown's list of options from a vector of strings.
 *
 * This function takes a Dropdown object by reference, clears out any old options
 * it might have, and then creates a new list of Text objects based on the
 * provided vector of strings.
 *
 * @param dd The Dropdown object to be populated (passed by reference).
 * @param font The Font to use for creating the new text options.
 * @param options A vector of strings, where each string will become a new option.
 */
void populateDropdown(Dropdown &dd, Font& font, const vector<string>& options){
    // First, clear out any old options to ensure a clean state.
    dd.options.clear();

    // Loop through each string provided in the options vector.
    for (const auto& option : options) {
        // Create a new Text object for this option.
        Text opt;
        opt.setFont(font);

        opt.setString(option);
        opt.setCharacterSize(18);
        opt.setFillColor(Color::Black);

        // Position the new option vertically below the previous one.
        opt.setPosition(1060, 230 + dd.options.size() * 35);

        // Add the newly created text object to the dropdown's list of options.
        dd.options.push_back(opt);
    }
}


// ===================================================================================
// == Global Variables for Sorting Mode ==
// ===================================================================================
// These variables hold the data and state for the sorting visualization. They are
// global to be accessible throughout the application's lifecycle.

vector<int> arr;            // The underlying integer array that is being sorted.
vector<RectangleShape> bars; // The visual representation (bars) of the array.
vector<int> arr_backup;     // A backup of the original, unsorted array for the "Reset" button.

int arrSize = 50;                // The current number of elements in the array (controlled by the size slider).
int cellSize = 21;               // The current cell size for the pathfinding grid.

// Variables to hold the dynamically calculated dimensions of the bars.
float barSpacing;
float barWidth;
float barSpacing_backup;
float barWidth_backup;

/**
 * @brief Generates a new, random array of integers and creates its visual representation.
 *
 * This function clears any existing array data, then generates a new set of random
 * integer heights based on the global 'arrSize'. It dynamically calculates the width
 * and spacing of the visual bars to ensure they always fit perfectly within the
 * designated screen area. Finally, it creates a backup of the new array for the
 * "Reset" functionality.
 */
void generatearr(){
    // Clear the main data and visual vectors to start fresh.
    arr.clear();
    bars.clear();

    // Set up a high-quality random number generator for the bar heights.
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> distrib(10, 400);

    // --- Dynamic Bar Calculation ---
    // These calculations ensure the bars always fit perfectly, no matter the array size.
    float totalTrackWidth = 945.0f; // The total pixel width of the drawing area.
    float startX = 50.0f;           // The starting X-coordinate for the first bar.

    // Calculate the total space (bar + gap) available for each element.
    barSpacing = totalTrackWidth / arrSize;
    // Calculate the visible width of the bar, leaving a small gap for visual clarity.
    barWidth = barSpacing * 0.8f;

    // Loop to generate the data and create the corresponding visual bar.
    for (int i = 0; i < arrSize; i++) {
        int height = distrib(gen);
        arr.push_back(height);

        RectangleShape bar(Vector2f(barWidth, height));
        bar.setPosition(startX + i * barSpacing, 600 - height);  // Position from the bottom up.
        bar.setFillColor(BAR_DEFAULT_COLOR);    
        bar.setOutlineColor(Color(50, 50, 50));
        bar.setOutlineThickness(1);
        bars.push_back(bar);
    }

    // Create a backup of the newly generated, unsorted state.
    // This is crucial for the "Reset" button, which reverts to this state.
    arr_backup = arr; 
    barSpacing_backup = barSpacing;
    barWidth_backup = barWidth;
}

// ===================================================================================
// == Core Application State and Resources ==
// ===================================================================================
// These global objects hold the state and resources for the entire application.

// --- General UI and Resource Managers ---
Sprite logoSprite;                    // The sprite for the "Path & Pivot" logo on the home screen.
PseudocodeManager pseudoManager;      // Manages loading and displaying pseudocode for all algorithms.


// --- State Objects for Sorting Algorithms ---
// Each object holds the current state (e.g., counters, flags) for its respective sorting algorithm.
BubbleSortState bubbleState;
SelectionSortState selectionState;
InsertionSortState insertionState;
MergeSortState mergeState;
QuickSortState quickState;

// --- Resources and State Objects for Pathfinding & Maze Generation ---
// These objects manage the grid and the state of each pathfinding or maze generation algorithm.
Grid pathfindingGrid(0, 60, 1029, 567, 21);
BFSState bfsState;
DFSState dfsState;
AStarState aStarState;
DijkstraState dijkstraState;
MazeGeneratorState mazeState;

int main()
{
    generatearr();

    bool draggingSlider = false;
    bool draggingSizeSlider = false;

    bool isPlaying = false;
    bool wasPlayingBeforeFocusLoss = false;
    int currentStep = 0;
    int hoveredIndex = -1;
    float animationDelay = 0.5f;
    bool isGeneratingMaze = false; 
    Clock stepClock;

    // ===================================================================================
    // == SFML Window and Asset Initialization ==
    // ===================================================================================

    // --- Window Creation ---
    // Create the main application window with a resolution of 1280x720 and a standard title bar.
    RenderWindow window(VideoMode(1280, 720), "SFML Window", Style::Default);
    // Cap the frame rate at 60 FPS to ensure consistent performance and prevent unnecessary CPU usage.
    window.setFramerateLimit(60);

    
    // --- Asset Loading ---
    // Load all necessary external resources like fonts and textures from files.
    // It's crucial to check if each asset loaded successfully to prevent crashes.

    Font font;
    Font font1;
    if (!font.loadFromFile("Arial.ttf"))
    {
        // If the primary font fails to load, print an error and exit the program.
        cerr << "Error loading font" << endl;
        return -1;
    }

    if(!font1.loadFromFile("BAUHS93.TTF")){
        // Handle error for the secondary font.
        cerr << "Error loading font" << endl;
        return -1;
    }

    Texture logoTexture;
    if (!logoTexture.loadFromFile("logo.png")) {
        // Handle error for the logo image file.
        cerr << "Error: Could not load logo.png" << endl;
    }


    // --- Sprite Configuration ---
    // Configure the logo sprite after its texture has been loaded.
    logoSprite.setTexture(logoTexture);
    // To properly scale and position the logo from its center, we must first set its origin.
    Vector2u textureSize = logoTexture.getSize();
    logoSprite.setOrigin(textureSize.x / 2.0f, textureSize.y / 2.0f);
    // Scale the sprite down to an appropriate size for the UI.
    logoSprite.setScale(0.05f, 0.05f);
    // Position the logo's newly defined center point at the desired coordinates.
    logoSprite.setPosition(30, 31);

    Mode currentMode = Mode::Home;  // App state
    HomeScreen homeScreen(font);


    // ===================================================================================
    // == UI Elements: Header Gradient Background ==
    // ===================================================================================
    // This creates a subtle decorative gradient at the top of the window by stacking
    // many thin, 1-pixel-high rectangles, each with a slightly different color.
    vector<RectangleShape> gradientBars;

    int barHeight = 60;  // The total height of the gradient area in pixels.

    for (int i = 0; i < barHeight; i++)
    {
        // Create a thin, full-width rectangle for this line of the gradient.
        RectangleShape bar(Vector2f(1280, 1));

        // Calculate a subtle, light-blueish gradient color.
        // The color changes slightly with each vertical pixel `i`.
        int r = 180 + i / 2;
        int g = 200 + i / 3;
        int b = 255;

        bar.setFillColor(Color(r, g, b));
        bar.setPosition(0, i);     // Position this bar one pixel below the previous one.
        gradientBars.push_back(bar);
    }


    // ===================================================================================
    // == UI Elements: Header Text & Status Bar ==
    // ===================================================================================

    // --- Main Application Title ---
    Text title;
    title.setFont(font);
    title.setString("Path & Pivot");
    title.setCharacterSize(32);
    title.setFillColor(Color::Black);
    title.setStyle(Text::Bold);
    title.setPosition(50, 10);

    // --- "Build by" and Author Name ---
    Text buildby;
    buildby.setFont(font);
    buildby.setString("Build by");
    buildby.setCharacterSize(14);
    buildby.setFillColor(Color::Black);
    buildby.setPosition(250, 28);

    Text author;
    author.setFont(font1);
    author.setString("Arpit jatav");
    author.setCharacterSize(16);
    author.setFillColor(Color::White);
    author.setOutlineColor(Color::Black);
    author.setOutlineThickness(0.5);
    author.setPosition(305, 25);

    // --- Dynamic Status Bar ---
    // This text object is updated throughout the program to give feedback to the user.
    Text status;
    status.setFont(font);
    status.setString("Welcome! Please select a mode.");
    status.setCharacterSize(20);
    status.setFillColor(Color::Black);
    status.setPosition(700, 18);

    // ===================================================================================
    // == UI Elements: Main Side Panel ==
    // ===================================================================================
    // This section initializes all the primary UI components located in the right-hand
    // side panel, including the background, mode selection, and algorithm dropdown.

    // --- Side Panel Background ---
    // A simple rectangle that serves as the background for the entire control panel.
    RectangleShape sidePanel;
    sidePanel.setSize(Vector2f(250, 720));
    sidePanel.setFillColor(Color(240, 240, 240));
    sidePanel.setPosition(1030, 0);

    // --- Mode Selection Buttons ---
    // These buttons allow the user to switch between the main application modes.
    Button homeModeBtn = createButton(1050, 20, 200, 40, "Home", font);
    Button sortModeBtn = createButton(1050, 70, 200, 40, "Sorting", font);
    Button pathModeBtn = createButton(1050, 120, 200, 40, "Pathfinding", font);

    // --- Algorithm Selection Dropdown ---
    // Initializes the main dropdown menu used for selecting specific algorithms.
    Dropdown algorithmDropdown;
    algorithmDropdown.box.setSize({200, 40});
    algorithmDropdown.box.setFillColor(Color(220, 220, 220));
    algorithmDropdown.box.setOutlineColor(Color(100, 100, 100));
    algorithmDropdown.box.setOutlineThickness(2);
    algorithmDropdown.box.setPosition(1050, 170);

    // Configure the text that displays the currently selected item.
    algorithmDropdown.selected.setFont(font);
    algorithmDropdown.selected.setCharacterSize(18);
    algorithmDropdown.selected.setFillColor(Color::Black);
    algorithmDropdown.selected.setPosition(1060, 178);

    // --- Dropdown Content ---
    // Define the lists of algorithms that will populate the dropdown in each mode.
    vector<string> sortingAlgos = {"Bubble Sort", "Selection Sort", "Insertion Sort", "Merge Sort", "Quick Sort"};
    vector<string> pathfindingAlgos = {"BFS", "DFS", "A* Search", "Dijkstra"};

    algorithmDropdown.selected.setString("Select Algorithm");

    // ===================================================================================
    // == UI Elements: Statistics Display Panels ==
    // ===================================================================================
    // These text objects are used to display performance metrics for the algorithms
    // on the side panel after a visualization is complete.

    // --- Pathfinding Statistics Panel ---
    // This panel shows the results of a pathfinding search.
    Text statsTitle("Statistics:", font, 20);
    statsTitle.setStyle(Text::Bold);
    statsTitle.setFillColor(Color::Black);
    statsTitle.setPosition(1050, 500-30);

    // Displays the name of the completed pathfinding algorithm (e.g., "A* Search").
    Text statsAlgoNameText("", font, 20);
    statsAlgoNameText.setStyle(Text::Bold);
    statsAlgoNameText.setFillColor(Color(0, 139, 139));
    statsAlgoNameText.setPosition(1160, 500-30);

    // Displays the total number of nodes the algorithm visited to find the path.
    Text nodesVisitedText("Nodes Visited: 0", font, 16);
    nodesVisitedText.setFillColor(Color::Black);
    nodesVisitedText.setPosition(1050, 530-30);

    // Displays the total cost of the final path (accounts for weighted nodes).
    Text pathCostText("Path Cost: 0", font, 16);
    pathCostText.setFillColor(Color::Black);
    pathCostText.setPosition(1050, 555-30);

    // --- Sorting Statistics Panel ---
    // This panel shows the performance results of a sorting algorithm.
    Text statsTitleSorting("Statistics:", font, 20);
    statsTitleSorting.setStyle(Text::Bold);
    statsTitleSorting.setFillColor(Color::Black);
    statsTitleSorting.setPosition(1050-20, 470);

    // Displays the name of the completed sorting algorithm (e.g., "Merge Sort").
    Text statsAlgoNameSorting("", font, 20);
    statsAlgoNameSorting.setStyle(Text::Bold);
    statsAlgoNameSorting.setFillColor(Color(0, 139, 139)); // Dark Cyan
    statsAlgoNameSorting.setPosition(1160-20-5, 470);

    // Displays the total number of comparisons made by the algorithm.
    Text comparisonsText("Comparisons: 0", font, 16);
    comparisonsText.setFillColor(Color::Black);
    comparisonsText.setPosition(1050-20, 500);

    // Displays the total number of times the algorithm read from or wrote to the array.
    Text accessesText("Array Accesses: 0", font, 16);
    accessesText.setFillColor(Color::Black);
    accessesText.setPosition(1050-20, 525);


    // == UI Elements: Checkboxes & Toggles ==
    // ===================================================================================
    // These UI elements allow the user to toggle specific features on and off.

    // --- Pseudocode Toggle ---
    // This checkbox controls the visibility of the pseudocode display on the side panel.
    RectangleShape checkBox(Vector2f(20, 20));
    checkBox.setPosition(1050, 645-30);
    checkBox.setFillColor(Color::White);
    checkBox.setOutlineColor(Color::Black);
    checkBox.setOutlineThickness(2);

    // This boolean flag is the core state variable for this feature.
    // The rest of the application checks this flag to decide whether to draw the pseudocode.
    bool showPseudocode = false;
    Text checkLabel;
    checkLabel.setFont(font);
    checkLabel.setString("Show Pseudocode");
    checkLabel.setCharacterSize(18);
    checkLabel.setFillColor(Color::Black);
    checkLabel.setPosition(1080, 643-30);

    // --- Diagonal Movement Toggle (Pathfinding Only) ---
    // This checkbox allows the user to enable or disable 8-directional diagonal movement
    // for pathfinding algorithms.
    RectangleShape diagonalBox(Vector2f(20, 20));
    diagonalBox.setPosition(1050, 675-30);
    diagonalBox.setFillColor(Color::White);
    diagonalBox.setOutlineColor(Color::Black);
    diagonalBox.setOutlineThickness(2);

    // This boolean flag is passed to the pathfinding step functions to control their behavior.
    bool isDiagonal = false;
    Text diagonalLabel;
    diagonalLabel.setFont(font);
    diagonalLabel.setString("Diagonals");
    diagonalLabel.setCharacterSize(18);
    diagonalLabel.setFillColor(Color::Black);
    diagonalLabel.setPosition(1080, 673-30);

    // ===================================================================================
    // == UI Elements: Control Panel Background ==
    // ===================================================================================
    // This creates the gradient background for the bottom control bar. It provides
    // a clean visual area that groups all the user controls together.
    vector<RectangleShape> controlBars;
    int cbarheight = 93;

    // Define the start (top) and end (bottom) colors for the gradient.
    Color startColor(210, 210, 210);
    Color endColor(235, 235, 235);
    for(int i = 0; i<cbarheight; i++){
        RectangleShape bar(Vector2f(1030, 1));

        // --- Linear Interpolation for a Smooth Gradient ---
        // Calculate our current position within the gradient as a percentage (0.0 to 1.0).
        float progress = static_cast<float>(i) / cbarheight;

        // Blend the start and end colors based on our current progress.
        // This technique is called Linear Interpolation (Lerp).
        int r = startColor.r + (endColor.r - startColor.r) * progress;
        int g = startColor.g + (endColor.g - startColor.g) * progress;
        int b = startColor.b + (endColor.b - startColor.b) * progress;
        bar.setFillColor(Color(r, g, b));
        bar.setPosition(0, 627 + i);
        controlBars.push_back(bar);
    }


    // This section initializes all the interactive UI elements for the application,
    // including control buttons, sliders, and color indicator legends.

    // --- Control Button Initialization ---
    // These buttons are created here but are repositioned and shown/hidden later
    // based on the current application mode (Sorting or Pathfinding).
    Button playbtn = createButton(50, 640, 120, 40, "Play", font);
    Button newArrayBtn = createButton(200, 640, 120, 40, "New Array", font);
    Button resetBtn = createButton(350, 640, 120, 40, "Reset", font);
    Button clearmazeBtn = createButton(160-20, 640, 120, 40, "Clear Maze", font);
    Button pathClearBtn = createButton(500, 640, 120, 40, "Clear Path", font);
    Button mazeGenBtn = createButton(650, 640, 120, 40, "Maze", font);

    /**
     * @brief A lambda function to handle the visual feedback for button hovering.
     *
     * This function's sole purpose is to update a button's appearance (color and scale)
     * when the mouse is over it. All click logic is handled separately in the event loop
     * for a cleaner separation of concerns.
     */
    auto handleButtonHover = [&](Button &btn) {
        Vector2i mousePos = Mouse::getPosition(window);

        if (btn.shape.getGlobalBounds().contains(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y))) {
            // Apply hover effect: change color and slightly increase size.
            btn.shape.setFillColor(btn.hoverColor);
            btn.shape.setScale(1.05f, 1.05f);
            btn.label.setScale(1.05f, 1.05f);
        } else {
            // Revert to default appearance.
            btn.shape.setFillColor(btn.defaultColor);
            btn.shape.setScale(1.0f, 1.0f); 
            btn.label.setScale(1.0f, 1.0f); 
        }
    };



    // --- Slider Initialization ---

    // -- Speed Control Slider --
    Text speed;
    speed.setFont(font);
    speed.setString("Speed: 1.00x");
    speed.setCharacterSize(16);
    speed.setFillColor(Color::Black);
    speed.setPosition(500+150+70-10-5-1, 628);

    RectangleShape sliderTrack;  // The gray background track.
    sliderTrack.setSize(Vector2f(308, 6));
    sliderTrack.setFillColor(Color(180, 180, 180)); 
    sliderTrack.setPosition(500+150+70-10-5-1, 660);

    CircleShape sliderKnob(13);  // The draggable knob.
    sliderKnob.setFillColor(Color(80, 80, 150));
    sliderKnob.setPosition(751.4f, 650.5f);  // Initial position for 1.00x speed.

    RectangleShape sliderFill;  // The colored bar that fills the track.
    sliderFill.setSize(Vector2f(sliderKnob.getPosition().x - sliderTrack.getPosition().x, sliderTrack.getSize().y));
    sliderFill.setFillColor(sliderKnob.getFillColor());
    sliderFill.setPosition(sliderTrack.getPosition());

    // -- Array Size Control Slider (Sorting Mode Only) --
    Text sizeLabel("Array Size:", font, 16);
    sizeLabel.setFillColor(Color::Black);
    sizeLabel.setPosition(500+150+70-10-5-1, 628+50-5-1);

    RectangleShape sizeSliderTrack;
    sizeSliderTrack.setSize(Vector2f(308, 6));
    sizeSliderTrack.setFillColor(Color(180, 180, 180));
    sizeSliderTrack.setPosition(500+150+70-10-5-1, 660+50-5-1);

    CircleShape sizeSliderKnob(13);
    sizeSliderKnob.setFillColor(Color(80, 80, 150));
    sizeSliderKnob.setPosition(837.8f, 694.5f);  // Initial position for array size 50.

    RectangleShape sizeSliderFill;
    sizeSliderFill.setSize(Vector2f(sizeSliderKnob.getPosition().x - sizeSliderTrack.getPosition().x, sizeSliderTrack.getSize().y));
    sizeSliderFill.setFillColor(sizeSliderKnob.getFillColor());
    sizeSliderFill.setPosition(sizeSliderTrack.getPosition());


    // --- Color Indicator Legends ---

    // -- Sorting Mode Legend --
    float indicatorY = 690.0f;
    // Comparison Indicator
    CircleShape compbox(10);
    compbox.setPosition(60, indicatorY);
    compbox.setFillColor(Color::Yellow);
    
    Text complabel;
    complabel.setFont(font);
    complabel.setString("Comparing");
    complabel.setCharacterSize(16);
    complabel.setFillColor(Color::Black);
    complabel.setPosition(85, indicatorY - 1);
    
    // Swapping Indicator
    CircleShape swapbox(10);
    swapbox.setPosition(190, indicatorY);
    swapbox.setFillColor(Color::Red);
    
    Text swaplabel;
    swaplabel.setFont(font);
    swaplabel.setString("Swapping");
    swaplabel.setCharacterSize(16);
    swaplabel.setFillColor(Color::Black);
    swaplabel.setPosition(215, indicatorY - 1);
    
    // Writing/Default Indicator
    CircleShape writepbox(10);
    writepbox.setPosition(320, indicatorY);
    writepbox.setFillColor(Color(108, 52, 97));
    
    Text writeplabel;
    writeplabel.setFont(font);
    writeplabel.setString("Writing");
    writeplabel.setCharacterSize(16);
    writeplabel.setFillColor(Color::Black);
    writeplabel.setPosition(345, indicatorY - 1);
    
    // Sorted Indicator
    CircleShape sortbox(10);
    sortbox.setPosition(430, indicatorY);
    sortbox.setFillColor(Color::Green);
    
    Text sortlabel;
    sortlabel.setFont(font);
    sortlabel.setString("Sorted");
    sortlabel.setCharacterSize(16);
    sortlabel.setFillColor(Color::Black);
    sortlabel.setPosition(455, indicatorY - 1);

    // -- Pathfinding Mode Legend --
    float indicatorYpathfinding = 690.0f;
    
    // Start Point Indicator
    CircleShape startbox(10);
    startbox.setPosition(50, indicatorYpathfinding);
    startbox.setFillColor(Color::Green);
    
    Text startlabel;
    startlabel.setFont(font);
    startlabel.setString("Start");
    startlabel.setCharacterSize(16);
    startlabel.setFillColor(Color::Black);
    startlabel.setPosition(75, indicatorYpathfinding - 1);
    
    // End Point Indicator
    CircleShape endbox(10);
    endbox.setPosition(145, indicatorYpathfinding);
    endbox.setFillColor(Color::Red);
    
    Text endlabel;
    endlabel.setFont(font);
    endlabel.setString("End");
    endlabel.setCharacterSize(16);
    endlabel.setFillColor(Color::Black);
    endlabel.setPosition(170, indicatorYpathfinding - 1);
    
    // Wall Indicator
    CircleShape wallbox(10);
    wallbox.setPosition(235, indicatorYpathfinding);
    wallbox.setFillColor(Color::Black);
    
    Text walllabel;
    walllabel.setFont(font);
    walllabel.setString("Wall");
    walllabel.setCharacterSize(16);
    walllabel.setFillColor(Color::Black);
    walllabel.setPosition(260, indicatorYpathfinding - 1);
    
    // Path Indicator
    CircleShape pathbox(10);
    pathbox.setPosition(325, indicatorYpathfinding);
    pathbox.setFillColor(Color::Yellow);
    
    Text pathlabel;
    pathlabel.setFont(font);
    pathlabel.setString("Path");
    pathlabel.setCharacterSize(16);
    pathlabel.setFillColor(Color::Black);
    pathlabel.setPosition(350, indicatorYpathfinding - 1);
    
    // Visited Cell Indicator
    CircleShape visitbox(10);
    visitbox.setPosition(415, indicatorYpathfinding);
    visitbox.setFillColor(Color(173, 216, 230));
    
    Text visitlabel;
    visitlabel.setFont(font);
    visitlabel.setString("Visited");
    visitlabel.setCharacterSize(16);
    visitlabel.setFillColor(Color::Black);
    visitlabel.setPosition(440, indicatorYpathfinding - 1);

    CircleShape osetbox(10);
    osetbox.setPosition(505, indicatorYpathfinding);
    osetbox.setFillColor(Color(200, 255, 200));

    Text osetlabel;
    osetlabel.setFont(font);
    osetlabel.setString("Open Set");
    osetlabel.setCharacterSize(16);
    osetlabel.setFillColor(Color::Black);
    osetlabel.setPosition(530, indicatorYpathfinding - 1);

    CircleShape mudbox(10);
    mudbox.setPosition(625, indicatorYpathfinding);
    mudbox.setFillColor(Color(188, 143, 143));

    Text mudlabel;
    mudlabel.setFont(font);
    mudlabel.setString("Weight");
    mudlabel.setCharacterSize(16);
    mudlabel.setFillColor(Color::Black);
    mudlabel.setPosition(650, indicatorYpathfinding - 1);


    // ===================================================================================
    // == Main Application Loop ==
    // ===================================================================================
    // This is the heart of the "Path & Pivot" visualizer. The loop runs continuously
    // at a capped 60 FPS until the user closes the window.
    //
    // Each iteration of the loop follows a standard real-time application structure:
    //   1. EVENT HANDLING:   Process all user input (mouse clicks, key presses, etc.).
    //   2. UPDATE LOGIC:     Update the state of the current visualization and UI.
    //   3. DRAWING:          Render the current state to the screen.

    while (window.isOpen())
    {
        // --- Event Handling ---
        // Create a single Event object that will be reused to process all user input.
        Event event;
        while (window.pollEvent(event))
        {
            // This event triggers when the user clicks another window or minimizes this one.
            if (event.type == sf::Event::LostFocus) {
                // First, remember the current state so we can restore it later.
                wasPlayingBeforeFocusLoss = isPlaying;
                // Always pause the simulation when the window is not active.
                isPlaying = false;
            }

            // This event triggers when the user clicks back on the window.
            if (event.type == sf::Event::GainedFocus) {
                // Only resume automatically if the simulation was running right before focus was lost.
                // This prevents un-pausing an algorithm the user had manually paused.
                if (wasPlayingBeforeFocusLoss) {
                    isPlaying = true;
                }
            }
            // --- Mouse Position & Slider Event Handling ---
            Vector2i mousePos = Mouse::getPosition(window);

            // This block handles the START of a drag action.
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left) {
                // Check if the click was on the speed slider.
                if (sliderKnob.getGlobalBounds().contains(mousePos.x, mousePos.y) || 
                    sliderTrack.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    
                    draggingSlider = true;
                    // Immediately jump to the clicked position
                    float newX = static_cast<float>(mousePos.x);
                    newX = max(704.0f, min(newX, 1004.0f)); // Clamp position
                    sliderKnob.setPosition(newX, sliderKnob.getPosition().y);
                }

                // Check if the click was on the size slider.
                if (sizeSliderKnob.getGlobalBounds().contains(mousePos.x, mousePos.y) ||
                    sizeSliderTrack.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    draggingSizeSlider = true;
                    float newX = static_cast<float>(mousePos.x);
                    newX = max(704.0f, min(newX, 1004.0f));
                    sizeSliderKnob.setPosition(newX, sizeSliderKnob.getPosition().y);
                }
            }

            if (event.type == Event::MouseButtonPressed)
            {
                if (sliderKnob.getGlobalBounds().contains(mousePos.x, mousePos.y))
                {
                    draggingSlider = true;
                }
                if(sizeSliderKnob.getGlobalBounds().contains(mousePos.x, mousePos.y))
                {
                    draggingSizeSlider = true;
                }
            }
            // This block handles the END of a drag action.
            if (event.type == Event::MouseButtonReleased)
            {
                draggingSlider = false;
                draggingSizeSlider = false;
            }

            // This block handles the CONTINUOUS drag movement.
            if (draggingSlider)
            {
                float newX = mousePos.x;
                newX = max(704.0f, min(newX, 1004.0f));
                // Clamp the knob's position to the track's boundaries.
                sliderKnob.setPosition(newX, sliderKnob.getPosition().y);
            }

            if(draggingSizeSlider)
            {
                float newX = mousePos.x;
                newX = max(704.0f, min(newX, 1004.0f));
                sizeSliderKnob.setPosition(newX, sizeSliderKnob.getPosition().y);
            }

            float sizeSliderValue = (sizeSliderKnob.getPosition().x - 704.0f) / 300.0f;
            int previousArrSize = arrSize;
            int previousCellSize = cellSize;
            if (currentMode == Mode::Sorting) {
                int minBars = 10;
                int maxBars = 100;
                arrSize = minBars + sizeSliderValue * (maxBars - minBars);
                sizeLabel.setString("Array Size: " + to_string(arrSize));
                if (arrSize != previousArrSize) {
                    generatearr();
                    resetBubbleSort(bubbleState);
                    resetSelectionSort(selectionState);
                    resetInsertionSort(insertionState);
                    resetMergeSort(mergeState, arr.size());
                    resetQuickSort(quickState, arr.size());
                    isPlaying = false;
                }
            }

            // ===================================================================================
            // == Primary Mouse Click Event Handler ==
            // ===================================================================================
            // This large block handles every left-mouse-click that occurs in the application.
            // It uses an if/else if chain to ensure only one action is taken per click.
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
            {
                Vector2i mousePos = Mouse::getPosition(window);

                // --- Side Panel: Mode Switching Buttons ---
                // These buttons change the main state of the application.
                if (homeModeBtn.shape.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    // Only switch to Home mode if we are not already there.
                    if (currentMode != Mode::Home) {
                        currentMode = Mode::Home;
                        status.setString("Welcome! Please select a mode.");
                        generatearr();
                        resetBubbleSort(bubbleState);
                        resetSelectionSort(selectionState);
                        resetInsertionSort(insertionState);
                        resetMergeSort(mergeState, arr.size());
                        resetQuickSort(quickState, arr.size());

                        pathfindingGrid.reset();
                        resetBFS(bfsState);
                        resetDFS(dfsState);
                        resetAStar(aStarState);
                        resetDijkstra(dijkstraState);
                        isPlaying = false;
                    }

                }
                else if (sortModeBtn.shape.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    // Only switch to Sorting mode if we are not already there.
                    if (currentMode != Mode::Sorting) {
                        currentMode = Mode::Sorting;
                        // Clean up any leftover pathfinding data before switching.
                        pathfindingGrid.reset();
                        resetBFS(bfsState);
                        resetDFS(dfsState);
                        resetAStar(aStarState);
                        resetDijkstra(dijkstraState);
                        // Populate the dropdown with the correct algorithms for this mode.
                        populateDropdown(algorithmDropdown, font, sortingAlgos);
                        algorithmDropdown.selected.setString("Select Algorithm");
                        status.setString("Switched to Sorting Mode.");
                        isPlaying = false;
                    }
                }
                else if (pathModeBtn.shape.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    // Only switch to Pathfinding mode if we are not already there.
                    if (currentMode != Mode::Pathfinding) {
                        currentMode = Mode::Pathfinding;
                        // Clean up any leftover sorting data before switching.
                        generatearr();  // This also resets all sorting states and backups.
                        // Populate the dropdown with the correct algorithms for this mode.
                        resetBubbleSort(bubbleState);
                        resetSelectionSort(selectionState);
                        resetInsertionSort(insertionState);
                        resetMergeSort(mergeState, arr.size());
                        resetQuickSort(quickState, arr.size());
                        populateDropdown(algorithmDropdown, font, pathfindingAlgos);
                        algorithmDropdown.selected.setString("Select Algorithm");
                        status.setString("Place Start, End, and Walls.");
                        isPlaying = false;
                    }
                }

                // --- Side Panel: Algorithm Dropdown ---
                // This logic handles opening, closing, and selecting from the dropdown.
                if (algorithmDropdown.box.getGlobalBounds().contains(mousePos.x, mousePos.y))
                {
                    // Toggle the dropdown's expanded state and restart its animation clock.
                    algorithmDropdown.expanded = !algorithmDropdown.expanded;
                    if (algorithmDropdown.expanded) {
                        algorithmDropdown.animationClock.restart();
                    }
                }
                // Check if any option is clicked
                else if (algorithmDropdown.expanded)
                {   
                    // If the dropdown is open, check if an option was clicked.
                    bool optionClicked = false;
                    for (auto &opt : algorithmDropdown.options) {
                        if (opt.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                            string oldAlgo = algorithmDropdown.selected.getString();
                            string newAlgo = opt.getString();
                            // Only reset the environment if a *new* algorithm is selected.
                            if (oldAlgo != newAlgo) {
                                algorithmDropdown.selected.setString(newAlgo);
                                isPlaying = false;
                                status.setString("Algorithm: " + newAlgo);
                                
                                // If we are in pathfinding mode, a new selection resets everything.
                                if (currentMode == Mode::Pathfinding) {
                                    pathfindingGrid.clearPath();
                                    resetBFS(bfsState);
                                    resetDFS(dfsState);
                                    resetAStar(aStarState);
                                    resetDijkstra(dijkstraState);
                                    // If switching from a weighted to unweighted algo, clear the mud.
                                    bool wasWeighted = (oldAlgo == "A* Search" || oldAlgo == "Dijkstra");
                                    bool isUnweighted = (newAlgo == "BFS" || newAlgo == "DFS");
                                    if (wasWeighted && isUnweighted) {
                                        pathfindingGrid.clearWeights();
                                    }
                                }else if (currentMode == Mode::Sorting) {
                                    // In sorting mode, we reset the array and all states.
                                    arr = arr_backup;
                                    barSpacing = barSpacing_backup;
                                    barWidth = barWidth_backup;
                                    bars.clear();
                                    // Recreate the visual bars from the restored data.
                                    for(size_t i = 0; i < arr.size(); ++i) {
                                        RectangleShape bar(Vector2f(barWidth, arr[i]));
                                        bar.setPosition(50.0f + i * barSpacing, 600 - arr[i]);
                                        bar.setFillColor(BAR_DEFAULT_COLOR);
                                        bar.setOutlineColor(Color(50, 50, 50));
                                        bar.setOutlineThickness(1);
                                        bars.push_back(bar);
                                    }
                                    // Reset all sorting algorithm states.
                                    resetBubbleSort(bubbleState);
                                    resetSelectionSort(selectionState);
                                    resetInsertionSort(insertionState);
                                    resetMergeSort(mergeState, arr.size());
                                    resetQuickSort(quickState, arr.size());
                                }
                            }
                            algorithmDropdown.expanded = false; // Always close the dropdown after a click.
                            optionClicked = true;
                            break;
                        }
                    }
                    // If the click was outside an expanded dropdown, close it.
                    if (!optionClicked) algorithmDropdown.expanded = false;
                }
                else
                {
                    algorithmDropdown.expanded = false; // close if clicked outside
                }

                // --- Controle Buttons ---
                // This logic handles all the buttons in the bottom control panel.
                if (playbtn.shape.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    string selectedAlgo = algorithmDropdown.selected.getString();

                    // 1. First, check if an algorithm is even selected.
                    if (selectedAlgo == "Select Algorithm") {
                        status.setString("Please select an algorithm first!");
                    }
                    // 2. Next, if in pathfinding mode, check if the grid is ready.
                    else if (currentMode == Mode::Pathfinding && (!pathfindingGrid.startNode || !pathfindingGrid.endNode)) {
                        status.setString("Place both Start and End nodes!");
                    }
                    // 3. If everything is ready, check if the algorithm is already finished.
                    else {
                         bool isFinished = false;
                        if (currentMode == Mode::Sorting) {
                            if ((selectedAlgo == "Bubble Sort" && bubbleState.isSorted) ||
                                (selectedAlgo == "Selection Sort" && selectionState.isSorted) ||
                                (selectedAlgo == "Insertion Sort" && insertionState.isSorted) ||
                                (selectedAlgo == "Merge Sort" && mergeState.isSorted) ||
                                (selectedAlgo == "Quick Sort" && quickState.isSorted)) {
                                isFinished = true;
                            }
                        } else { // Pathfinding
                            if ((selectedAlgo == "BFS" && bfsState.isComplete) ||
                                (selectedAlgo == "DFS" && dfsState.isComplete) ||
                                (selectedAlgo == "A* Search" && aStarState.isComplete) ||
                                (selectedAlgo == "Dijkstra" && dijkstraState.isComplete)) {
                                isFinished = true;
                            }
                        }
                        // 4. Only if the algorithm is NOT finished can we toggle play/pause.
                        if (!isFinished) {
                            isPlaying = !isPlaying;
                        }
                        
                        // 5. If we just started playing, initialize the algorithm's state.
                        if (isPlaying) {
                            if (currentMode == Mode::Sorting) {
                                if (selectedAlgo == "Merge Sort" && !mergeState.isSorting) {
                                    resetMergeSort(mergeState, arr.size());
                                    mergeState.tempArray = arr;
                                    stack<MergeJob> reversed_jobs;
                                    while(!mergeState.jobs.empty()){ reversed_jobs.push(mergeState.jobs.top()); mergeState.jobs.pop(); }
                                    mergeState.jobs = reversed_jobs;
                                    mergeState.isSorting = true;
                                }
                                if (selectedAlgo == "Quick Sort" && !quickState.isSorting) {
                                    resetQuickSort(quickState, arr.size());
                                    quickState.isSorting = true;
                                }
                            } else { // Pathfinding Mode
                                if (pathfindingGrid.startNode && pathfindingGrid.endNode) {
                                    // Check if we are starting a NEW search, not resuming.
                                    bool isNewSearch = false;
                                    if (selectedAlgo == "BFS" && !bfsState.isSearching) isNewSearch = true;
                                    if (selectedAlgo == "DFS" && !dfsState.isSearching) isNewSearch = true;
                                    if (selectedAlgo == "A* Search" && !aStarState.isSearching) isNewSearch = true;
                                    if (selectedAlgo == "Dijkstra" && !dijkstraState.isSearching) isNewSearch = true;
    
                                    if(isNewSearch) {
                                        pathfindingGrid.clearPath();
                                        if (selectedAlgo == "BFS") {
                                            resetBFS(bfsState);
                                            bfsState.queue.push(pathfindingGrid.startNode);
                                            bfsState.isSearching = true;
                                            status.setString("Searching with BFS...");
                                        } else if(selectedAlgo == "DFS") {
                                            resetDFS(dfsState);
                                            dfsState.stack.push(pathfindingGrid.startNode);
                                            dfsState.isSearching = true;
                                            status.setString("Searching with DFS...");
                                        } else if (selectedAlgo == "A* Search") {
                                            resetAStar(aStarState);
                                            for(auto& row : pathfindingGrid.nodes) for(auto& node : row) aStarState.gCost[&node] = numeric_limits<int>::max();
                                            aStarState.gCost[pathfindingGrid.startNode] = 0;
                                            int hCost = calculateHeuristic(pathfindingGrid.startNode, pathfindingGrid.endNode);
                                            aStarState.openSet.push({pathfindingGrid.startNode, hCost});
                                            aStarState.isSearching = true;
                                            status.setString("Searching with A*...");
                                        } else if (selectedAlgo == "Dijkstra") {
                                            resetDijkstra(dijkstraState);
                                            for(auto& row : pathfindingGrid.nodes) for(auto& node : row) dijkstraState.costMap[&node] = numeric_limits<int>::max();
                                            dijkstraState.costMap[pathfindingGrid.startNode] = 0;
                                            dijkstraState.openSet.push({pathfindingGrid.startNode, 0});
                                            dijkstraState.isSearching = true;
                                            status.setString("Searching with Dijkstra...");
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                // --- Control Panel: Other Buttons ---
                if (currentMode==Mode::Sorting && newArrayBtn.shape.getGlobalBounds().contains(mousePos.x, mousePos.y)){
                    isPlaying=false;
                    generatearr();  // create new random heights
                    resetBubbleSort(bubbleState);
                    resetSelectionSort(selectionState);
                    resetInsertionSort(insertionState);
                    resetMergeSort(mergeState, arr.size());
                    resetQuickSort(quickState, arr.size());
                    status.setString("Array generated. Select an algorithm.");
                }

                if (resetBtn.shape.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    isPlaying = false;
                    currentStep = 0;
                    if(currentMode == Mode::Sorting){
                        arr = arr_backup;
                        barSpacing = barSpacing_backup;
                        barWidth = barWidth_backup;
                        bars.clear();
                        for(size_t i = 0; i < arr.size(); ++i) {
                            RectangleShape bar(Vector2f(barWidth, arr[i]));
                            bar.setPosition(50.0f + i * barSpacing, 600 - arr[i]);
                            bar.setFillColor(BAR_DEFAULT_COLOR);
                            bar.setOutlineColor(Color(50, 50, 50));
                            bar.setOutlineThickness(1);
                            bars.push_back(bar);
                        }
                        resetBubbleSort(bubbleState);
                        resetSelectionSort(selectionState);
                        resetInsertionSort(insertionState);
                        resetMergeSort(mergeState, arr.size());
                        resetQuickSort(quickState, arr.size());
                        status.setString("Array reset. Select an algorithm.");
                    }else{
                        pathfindingGrid.reset();
                        resetBFS(bfsState);
                        resetDFS(dfsState);
                        resetAStar(aStarState);
                        resetDijkstra(dijkstraState);
                        status.setString("Grid reset. Place Start and End.");
                    }
                }

                if (currentMode == Mode::Pathfinding && pathClearBtn.shape.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    isPlaying = false;
                    pathfindingGrid.clearPath();
                    resetBFS(bfsState);
                    resetDFS(dfsState);
                    resetAStar(aStarState);
                    resetDijkstra(dijkstraState);
                    status.setString("Path cleared.");
                }

                if (currentMode == Mode::Pathfinding && clearmazeBtn.shape.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    isPlaying = false;
                    pathfindingGrid.clearMaze();
                    resetBFS(bfsState);
                    resetDFS(dfsState);
                    resetAStar(aStarState);
                    resetDijkstra(dijkstraState);
                    status.setString("Maze cleared. Ready for new search.");
                }
                if (currentMode == Mode::Pathfinding && mazeGenBtn.shape.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    isPlaying = false;
                    resetBFS(bfsState);
                    resetDFS(dfsState);
                    resetAStar(aStarState);
                    resetDijkstra(dijkstraState);
            
                    // Prepare grid for generation
                    pathfindingGrid.fillWithWalls();
                    resetMazeGenerator(mazeState);
            
                    // Pick a random starting point and set it to Empty
                    int startRow = (rand() % (pathfindingGrid.rows / 2)) * 2;
                    int startCol = (rand() % (pathfindingGrid.cols / 2)) * 2;
                    
                    pathfindingGrid.setNodeType(startRow, startCol, NodeType::Empty);
                    mazeState.stack.push(&pathfindingGrid.nodes[startRow][startCol]);
                    
                    isGeneratingMaze = true;
                    mazeState.isGenerating = true;
                    status.setString("Generating maze...");
                }

            }

            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
            {
                Vector2i mousePos = Mouse::getPosition(window);

                if (checkBox.getGlobalBounds().contains(mousePos.x, mousePos.y))
                {
                    showPseudocode = !showPseudocode;
                    checkBox.setFillColor(showPseudocode ? Color::Green : Color::White);
                }

                if (diagonalBox.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    isDiagonal = !isDiagonal;
                    diagonalBox.setFillColor(isDiagonal ? Color::Green : Color::White);
                    isPlaying = false;
                    pathfindingGrid.clearPath();

                    resetBFS(bfsState);
                    resetDFS(dfsState);
                    resetAStar(aStarState);
                    resetDijkstra(dijkstraState);
                    status.setString("Settings changed.");
                }
            }

            // Algo names highlighting
            if (event.type == Event::MouseMoved)
            {
                Vector2f mousePos(event.mouseMove.x, event.mouseMove.y);
                hoveredIndex = -1;

                for (size_t i = 0; i < algorithmDropdown.options.size(); i++)
                {
                    if (algorithmDropdown.options[i].getGlobalBounds().contains(mousePos))
                    {
                        hoveredIndex = i;
                        break;
                    }
                }
            }

            for (size_t i = 0; i < algorithmDropdown.options.size(); i++)
            {
                if ((int)i == hoveredIndex)
                {
                    algorithmDropdown.options[i].setFillColor(Color::Blue);
                    algorithmDropdown.options[i].setOutlineThickness(0.3);
                    algorithmDropdown.options[i].setOutlineColor(Color::Black);
                }
                else
                {
                    algorithmDropdown.options[i].setFillColor(Color::Black);
                    algorithmDropdown.options[i].setOutlineThickness(0);
                }

                window.draw(algorithmDropdown.options[i]);
            }

            switch (event.type)
            {
            case Event::Closed:
                window.close();
                break;
            case Event::KeyPressed:
                if (event.key.code == Keyboard::Escape)
                    window.close();
                break;
            default:
                break;
            }
        }

        // ===================================================================================
        // == UPDATE LOGIC (Runs Every Frame) ==
        // ===================================================================================
        // This section is the "brain" of the application and runs on every single frame,
        // regardless of whether the user has provided any input in that frame. Its job is
        // to update the state of the application and UI based on current settings and
        // ongoing processes.
        //
        // Key responsibilities in this block include:
        //  - Running continuous animations (like the home screen waves and button hovers).
        //  - Executing the main algorithm stepping logic when 'isPlaying' is true.
        //  - Synchronizing UI elements (like sliders and statistics) with internal state variables.
        //  - Managing the appearance of dynamic UI elements (e.g., the Play/Pause button).
        //

        handleButtonHover(homeModeBtn);
        handleButtonHover(sortModeBtn);
        handleButtonHover(pathModeBtn);
        Color activeColor = Color(0, 139, 139);
        if (currentMode == Mode::Home){
            homeModeBtn.shape.setFillColor(activeColor);
            sortModeBtn.shape.setFillColor(sortModeBtn.defaultColor);
            pathModeBtn.shape.setFillColor(pathModeBtn.defaultColor);
            homeScreen.update();
        } else if (currentMode == Mode::Sorting){
            homeModeBtn.shape.setFillColor(homeModeBtn.defaultColor);
            sortModeBtn.shape.setFillColor(activeColor);
            pathModeBtn.shape.setFillColor(pathModeBtn.defaultColor);
        } else{ 
            homeModeBtn.shape.setFillColor(homeModeBtn.defaultColor);
            sortModeBtn.shape.setFillColor(sortModeBtn.defaultColor);
            pathModeBtn.shape.setFillColor(activeColor);
        }

        // Apply to all buttons
        handleButtonHover(playbtn);
        playbtn.shape.setFillColor(isPlaying ? Color::Green : playbtn.defaultColor);
        playbtn.label.setString(isPlaying ? "Pause" : "Play");

        if (currentMode == Mode::Pathfinding && !isPlaying) {
            string selectedAlgo = algorithmDropdown.selected.getString();
            bool allowWeights = (selectedAlgo == "A* Search" || selectedAlgo == "Dijkstra");
            pathfindingGrid.handleMouseInput(window, allowWeights);
        }

       // ===================================================================================
       // == UI State Update: Sliders ==
       // ===================================================================================
       // This section runs every frame to ensure the application's state (like speed
       // and array size) is always perfectly in sync with the visual UI sliders.
   
       // --- Speed Slider Update ---
       // This block translates the pixel position of the speed slider's knob into a
       // speed multiplier for the algorithm animations.


        // 1. Calculate the slider's progress as a value from 0.0 to 1.0.
        float sliderValue = (sliderKnob.getPosition().x - 704.0f) / 300.0f;

        // 2. Apply an easing curve by squaring the value. This makes the slider
        // much more sensitive at the slow end, giving you finer control.
        float easedSliderValue = sliderValue;
        // 3. Define the minimum and maximum speed multipliers.
        float minSpeed = 0.25f;
        float maxSpeed = 5.0f;

        // 4. Map the eased progress to the desired speed range.
        float currentSpeed = minSpeed + easedSliderValue * (maxSpeed - minSpeed);

        // // 4. Calculate the animation delay. For our "turbo mode," this is used
        // //    to determine how many steps to run per frame.
        // animationDelay = 0.05f / currentSpeed;

        // 5. Update the UI text label, formatting the speed to two decimal places for a clean look.
        stringstream ss;
        ss << fixed << setprecision(2) << currentSpeed;
        speed.setString("Speed: " + ss.str() + "x");
        // 6. Update the visual "fill" bar to match the knob's position.
        float fillWidth = sliderKnob.getPosition().x - sliderTrack.getPosition().x;
        if (fillWidth < 0) fillWidth = 0;   // Prevent negative width.
        sliderFill.setSize(Vector2f(fillWidth, sliderTrack.getSize().y));

        // --- Array Size Slider Update (Only affects Sorting Mode) ---
        // This block translates the size slider's position into the number of bars to be sorted.

        // 1. Calculate the size slider's progress as a value from 0.0 to 1.0.
        float sizeSliderValue = (sizeSliderKnob.getPosition().x - 704.0f) / 300.0f;

        // 2. Define the minimum and maximum array sizes.
        int minBars = 10;
        int maxBars = 100;
        // 3. Map the 0.0-1.0 progress to the desired array size range.
        arrSize = minBars + sizeSliderValue * (maxBars - minBars);

        // 4. Update the UI text label with the current array size.
        sizeLabel.setString("Array Size: " + to_string(arrSize));

        // 5. Update the visual "fill" bar to match the knob's position.
        float sizeFillWidth = sizeSliderKnob.getPosition().x - sizeSliderTrack.getPosition().x;
        if (sizeFillWidth < 0) sizeFillWidth = 0;

        sizeSliderFill.setSize(Vector2f(sizeFillWidth, sizeSliderTrack.getSize().y));

        // --- Algorithm Stepping & Maze Generation ("Turbo Mode") ---
        // This section runs the core logic of the visualizations every frame. It uses a
        // "steps-per-frame" approach to allow for very high-speed animations that are
        // not limited by the application's 60 FPS cap.

        // A. Main Algorithm Visualization Loop
        if (isPlaying) {
            // This lambda function neatly contains your original core algorithm-calling logic.
            // We define it once here so we can call it from either the slow-speed or fast-speed path.
            auto runSingleStep = [&]() {
                string selectAlgo = algorithmDropdown.selected.getString();
                
                if(currentMode == Mode::Sorting){
                    if (selectAlgo == "Bubble Sort") bubbleSortStep(bars, arr, bubbleState);
                    else if (selectAlgo == "Selection Sort") selectionSortStep(bars, arr, selectionState);
                    else if (selectAlgo == "Insertion Sort") insertionSortStep(bars, arr, insertionState);
                    else if(selectAlgo == "Merge Sort") mergeSortStep(bars, arr, mergeState);
                    else if(selectAlgo == "Quick Sort") quickSortStep(bars, arr, quickState);
                    
                    bool sortIsComplete = false;
                    if (selectAlgo == "Bubble Sort" && bubbleState.isSorted) sortIsComplete = true;
                    else if (selectAlgo == "Selection Sort" && selectionState.isSorted) sortIsComplete = true;
                    else if (selectAlgo == "Insertion Sort" && insertionState.isSorted) sortIsComplete = true;
                    else if (selectAlgo == "Merge Sort" && mergeState.isSorted) sortIsComplete = true;
                    else if (selectAlgo == "Quick Sort" && quickState.isSorted) sortIsComplete = true;
                    if (sortIsComplete) {
                        isPlaying = false; 
                        status.setString("Sorting complete!"); 
                    }
                } else {
                    if (selectAlgo == "BFS") {
                        bfsStep(pathfindingGrid, bfsState, isDiagonal);
                        if (bfsState.noPathExists) { status.setString("No path found!"); isPlaying = false; }
                        if (bfsState.isComplete && !bfsState.noPathExists) { status.setString("Path found!"); isPlaying = false; }
                    } else if(selectAlgo == "DFS") {
                        dfsStep(pathfindingGrid, dfsState, isDiagonal);
                        if(dfsState.noPathExists) { status.setString("No path found!"); isPlaying = false; }
                        if (dfsState.isComplete && !dfsState.noPathExists) { status.setString("Path found!"); isPlaying = false; }
                    } else if(selectAlgo == "A* Search") {
                        aStarStep(pathfindingGrid, aStarState, isDiagonal);
                        if(aStarState.noPathExists) { status.setString("No path found!"); isPlaying = false; }
                        if (aStarState.isComplete && !aStarState.noPathExists) { status.setString("Path found!"); isPlaying = false; }
                    } else if(selectAlgo == "Dijkstra") {
                        dijkstraStep(pathfindingGrid, dijkstraState, isDiagonal);
                        if (dijkstraState.noPathExists) { status.setString("No path found!"); isPlaying = false; }
                        if (dijkstraState.isComplete && !dijkstraState.noPathExists) { status.setString("Path found!"); isPlaying = false; }
                    }
                }
            };
    
            // --- HYBRID SPEED CONTROL ---
            // CASE 1: Speed is less than 1.0x (Slowing Down)
            // We use a time-based delay to skip frames, which is the way to truly slow down.
            if (currentSpeed < 1.0f) {
                // Calculate the required delay between steps. At 0.25x speed, this will run one step every 4 frames.
                float requiredDelay = (1.0f / 60.0f) / currentSpeed; 
                if (stepClock.getElapsedTime().asSeconds() >= requiredDelay) {
                    runSingleStep();
                    stepClock.restart();
                }
            }
            // CASE 2: Speed is 1.0x or greater (Speeding Up)
            // We use "steps-per-frame" method.
            else {
                int stepsPerFrame = static_cast<int>(currentSpeed);
                for (int i = 0; i < stepsPerFrame; ++i) {
                    // If the algorithm finishes mid-frame, we must stop immediately.
                    if (!isPlaying) break; 
                    runSingleStep();
                }
            }
        }
        // B. Maze Generation Loop
        if (isGeneratingMaze) {
            // Run multiple steps per frame for a fast but still visible generation animation.
            for (int i = 0; i < 10; ++i) { 
                if (mazeState.isGenerating) {
                    mazeStep(pathfindingGrid, mazeState);
                } else {
                    break;  // Stop early if generation is complete.
                }
            }
            
            // When the generator finishes, finalize the maze and update the UI status.
            if (!mazeState.isGenerating) {
                isGeneratingMaze = false;
                status.setString("Maze generated. Place Start/End.");
            }
        }

        // ===================================================================================
        // == UI State Update: Button Hovers & Statistics Panels ==
        // ===================================================================================
        // This section updates dynamic UI elements that need to be checked on every frame.
    
        // --- Button Hover Visuals ---
        // This must run every frame to continuously check the mouse position against
        // all interactive buttons and apply the appropriate visual feedback.

        handleButtonHover(newArrayBtn);
        handleButtonHover(resetBtn);
        handleButtonHover(clearmazeBtn);
        handleButtonHover(pathClearBtn);
        handleButtonHover(mazeGenBtn);

        // --- Sorting Statistics Panel Update ---
        // Checks the state of the current sorting algorithm and updates the stats display accordingly.
        if (currentMode == Mode::Sorting) {
            string selectedAlgo = algorithmDropdown.selected.getString();

            // 1. Set the name of the algorithm in the stats panel.
            if (selectedAlgo != "Select Algorithm") {
                statsAlgoNameSorting.setString(selectedAlgo);
            } else {
                statsAlgoNameSorting.setString("");  // Clear the name if none is selected.
            }

            // 2. Check if the currently selected algorithm has finished sorting.
            bool sortIsComplete = false;
            if (selectedAlgo == "Bubble Sort" && bubbleState.isSorted) sortIsComplete = true;
            else if (selectedAlgo == "Selection Sort" && selectionState.isSorted) sortIsComplete = true;
            else if (selectedAlgo == "Insertion Sort" && insertionState.isSorted) sortIsComplete = true;
            else if (selectedAlgo == "Merge Sort" && mergeState.isSorted) sortIsComplete = true;
            else if (selectedAlgo == "Quick Sort" && quickState.isSorted) sortIsComplete = true;

            // 3. If the sort is complete, retrieve and display its final stats.
            if (sortIsComplete) {
                unsigned long long totalComparisons = 0;
                unsigned long long totalAccesses = 0;

                // Get stats from the appropriate state object.
                if (selectedAlgo == "Bubble Sort") {
                    totalComparisons = bubbleState.comparisons;
                    totalAccesses = bubbleState.arrayAccesses;
                } else if (selectedAlgo == "Selection Sort") {
                    totalComparisons = selectionState.comparisons;
                    totalAccesses = selectionState.arrayAccesses;
                } else if (selectedAlgo == "Insertion Sort") {
                    totalComparisons = insertionState.comparisons;
                    totalAccesses = insertionState.arrayAccesses;
                } else if (selectedAlgo == "Merge Sort") {
                    totalComparisons = mergeState.comparisons;
                    totalAccesses = mergeState.arrayAccesses;
                } else if (selectedAlgo == "Quick Sort") {
                    totalComparisons = quickState.comparisons;
                    totalAccesses = quickState.arrayAccesses;
                }
                
                // Update the UI text elements with the final numbers.
                comparisonsText.setString("Comparisons: " + to_string(totalComparisons));
                accessesText.setString("Array Accesses: " + to_string(totalAccesses));

            } else {
                // If no sort is complete (or none is selected), show the default "0" values.
                comparisonsText.setString("Comparisons: 0");
                accessesText.setString("Array Accesses: 0");
            }
        }

        // --- Pathfinding Statistics Panel Update ---
        // Checks the state of the current pathfinding algorithm and updates the stats display.
        if (currentMode == Mode::Pathfinding) {
            string selectedAlgo = algorithmDropdown.selected.getString();

            // 1. Set the algorithm name in the panel.
            if (selectedAlgo != "Select Algorithm") {
                statsAlgoNameText.setString(selectedAlgo);
            } else {
                statsAlgoNameText.setString("");
            }

            // 2. Check for completion and update the stats text accordingly.
            if (selectedAlgo == "BFS" && bfsState.isComplete && selectedAlgo != "Select Algorithm") {
                nodesVisitedText.setString("Nodes Visited: " + to_string(bfsState.nodesVisited));
                pathCostText.setString("Path Cost: " + to_string(bfsState.pathCost));
            } else if (selectedAlgo == "DFS" && dfsState.isComplete && selectedAlgo != "Select Algorithm") {
                nodesVisitedText.setString("Nodes Visited: " + to_string(dfsState.nodesVisited));
                pathCostText.setString("Path Cost: " + to_string(dfsState.pathCost));
            } else if (selectedAlgo == "A* Search" && aStarState.isComplete && selectedAlgo != "Select Algorithm") {
                nodesVisitedText.setString("Nodes Visited: " + to_string(aStarState.nodesVisited));
                pathCostText.setString("Path Cost: " + to_string(aStarState.pathCost));
            } else if (selectedAlgo == "Dijkstra" && dijkstraState.isComplete && selectedAlgo != "Select Algorithm") {
                nodesVisitedText.setString("Nodes Visited: " + to_string(dijkstraState.nodesVisited));
                pathCostText.setString("Path Cost: " + to_string(dijkstraState.pathCost));
            } else{
                // If no search is complete, show the default "0" values.
                nodesVisitedText.setString("Nodes Visited: 0");
                pathCostText.setString("Path Cost: 0");
            }
        }

       // This is the final part of the UPDATE LOGIC section.
       // It's responsible for updating the home screen's continuous animations.
        if (currentMode == Mode::Home) {
            homeScreen.update();
        }

        // ===================================================================================
        // == DRAWING (Render the current state to the screen) ==
        // ===================================================================================
        // This section is responsible for drawing every visible element to the window.
        // It follows a strict back-to-front order to ensure correct layering.
    
        // 1. Clear the previous frame to start with a fresh canvas.
        window.clear(Color(245, 245, 245));

        // 2. Draw the main visualization area based on the current mode.
        // --- Draw Main Visualization Area ---
        if (currentMode == Mode::Home) {
            homeScreen.draw(window);
        }else if(currentMode == Mode::Sorting) {
            // Draw the main sorting bars and the background for the control panel.
            for (auto &bar : bars) window.draw(bar);
            for (auto &bar : controlBars) window.draw(bar);

            // Dynamically reposition the control buttons for this mode.
            reposition(playbtn, 50, 640);
            reposition(resetBtn, 350, 640);

            // Draw the controls for sorting. 
            window.draw(playbtn.shape);
            window.draw(playbtn.label);
            window.draw(newArrayBtn.shape);
            window.draw(newArrayBtn.label);
            window.draw(resetBtn.shape);
            window.draw(resetBtn.label);
            window.draw(sliderTrack);
            window.draw(sliderFill);
            window.draw(sliderKnob);
            window.draw(sizeLabel);
            window.draw(sizeSliderTrack);
            window.draw(sizeSliderFill);
            window.draw(sizeSliderKnob);
            window.draw(speed);

        }else {// Pathfinding Mode
            // Draw the main pathfinding grid and the control panel background.
            pathfindingGrid.draw(window);
            for (auto &bar : controlBars) window.draw(bar);

            // Dynamically reposition the control buttons for this mode.
            reposition(playbtn, 10, 640);
            reposition(resetBtn, 310-20-20, 640);
            reposition(pathClearBtn, 460-20-20-20, 640);
            reposition(mazeGenBtn, 610-20-20-20-20, 640);
            
            // Draw the controls for pathfinding.
            window.draw(playbtn.shape);
            window.draw(playbtn.label);
            window.draw(clearmazeBtn.shape);
            window.draw(clearmazeBtn.label);
            window.draw(resetBtn.shape);
            window.draw(resetBtn.label);
            window.draw(pathClearBtn.shape);
            window.draw(pathClearBtn.label);
            window.draw(mazeGenBtn.shape);
            window.draw(mazeGenBtn.label);
            window.draw(sliderTrack);
            window.draw(sliderFill);
            window.draw(sliderKnob); 
            window.draw(speed);
        }

        // 3. Draw the static overlay UI (headers, side panel, etc.) on top of everything.
        // --- Draw Static Overlay UI ---
        for (auto &bar : gradientBars)
        {
            window.draw(bar);
        }
        window.draw(logoSprite);
        window.draw(title);
        window.draw(buildby);
        window.draw(author);
        window.draw(status);
        window.draw(sidePanel);

        // Draw the main mode-switching buttons.
        window.draw(homeModeBtn.shape);
        window.draw(homeModeBtn.label);
        window.draw(sortModeBtn.shape);
        window.draw(sortModeBtn.label);
        window.draw(pathModeBtn.shape);
        window.draw(pathModeBtn.label);

        // 4. Draw mode-specific UI elements on the side panel.
        //    These are only shown when not on the home screen.
        if(currentMode != Mode::Home){
            // Draw the dropdown menu and checkboxes.
            window.draw(checkBox);
            window.draw(checkLabel);
            window.draw(algorithmDropdown.box);
            window.draw(algorithmDropdown.selected);
            
            // If the dropdown is expanded, draw its options with a fade-in animation.
            if (algorithmDropdown.expanded)
            {
                // Calculate animation progress (0.0 to 1.0).
                float elapsed = algorithmDropdown.animationClock.getElapsedTime().asSeconds();
                float progress = min(1.0f, elapsed / algorithmDropdown.animationDuration);
    
                Uint8 alpha = static_cast<Uint8>(progress * 255);
    
                for (size_t i = 0; i < algorithmDropdown.options.size(); ++i) {
                    Color baseColor = ((int)i == hoveredIndex) ? Color::Blue : Color::Black;
                    algorithmDropdown.options[i].setFillColor(Color(baseColor.r, baseColor.g, baseColor.b, alpha));
    
                    window.draw(algorithmDropdown.options[i]);
                }
            }

            // --- Draw Statistics Panels (if pseudocode is hidden) ---
            // The stats panels are hidden when pseudocode is visible to prevent overlap.
            if (currentMode == Mode::Pathfinding && !showPseudocode) {
                window.draw(statsTitle);
                window.draw(statsAlgoNameText);
                window.draw(nodesVisitedText);
                window.draw(pathCostText);
            }

            if (currentMode == Mode::Sorting && !showPseudocode) {
                window.draw(statsTitleSorting);
                window.draw(statsAlgoNameSorting);
                window.draw(comparisonsText);
                window.draw(accessesText);
            }
        }

        // 5. Draw the dynamic color legends and the pseudocode panel on top of the side panel.
        string selectAlgo = algorithmDropdown.selected.getString();
        if(currentMode == Mode::Sorting){
            // Draw the color legend for sorting algorithms.
            window.draw(sortbox);
            window.draw(sortlabel);
            window.draw(writepbox);
            window.draw(writeplabel);
            window.draw(compbox);
            window.draw(complabel);
            window.draw(swapbox);
            window.draw(swaplabel);
        }else if(currentMode == Mode::Pathfinding){
            // Draw the color legend for pathfinding algorithms.
            window.draw(pathbox);
            window.draw(pathlabel);
            window.draw(visitbox);
            window.draw(visitlabel);
            window.draw(wallbox);
            window.draw(walllabel);
            window.draw(endbox);
            window.draw(endlabel);
            window.draw(startbox);
            window.draw(startlabel);
            window.draw(diagonalBox);
            window.draw(diagonalLabel);
            if(selectAlgo == "A* Search"){
                window.draw(osetbox);
                window.draw(osetlabel);
                window.draw(mudbox);
                window.draw(mudlabel);
            }else if(selectAlgo == "Dijkstra"){
                window.draw(osetbox);
                window.draw(osetlabel);
                window.draw(mudbox);
                window.draw(mudlabel);   
            }
            
        }

        window.setFramerateLimit(60);

        // --- Draw Pseudocode Panel (if enabled) ---
        if (showPseudocode && currentMode != Mode::Home) {
            string selectedAlgo = algorithmDropdown.selected.getString();
            if (pseudoManager.pseudocodes.count(selectedAlgo) && !algorithmDropdown.expanded) {
                int activeLine = 0;

                // Get the active line from the correct state
                if (selectedAlgo == "Bubble Sort") activeLine = bubbleState.currentLine;
                else if (selectedAlgo == "Selection Sort") activeLine = selectionState.currentLine;
                else if (selectedAlgo == "Insertion Sort") activeLine = insertionState.currentLine;
                else if (selectedAlgo == "Merge Sort") activeLine = mergeState.currentLine;
                else if (selectedAlgo == "Quick Sort") activeLine = quickState.currentLine;
                else if (selectedAlgo == "BFS") activeLine = bfsState.currentLine;
                else if (selectedAlgo == "DFS") activeLine = dfsState.currentLine;
                else if (selectedAlgo == "A* Search") activeLine = aStarState.currentLine;
                else if (selectedAlgo == "Dijkstra") activeLine = dijkstraState.currentLine;

                auto& lines = pseudoManager.pseudocodes[selectedAlgo];
                for (size_t i = 0; i < lines.size(); ++i) {
                    Text lineText(lines[i], font, 14);
                    lineText.setPosition(1030, 220 + i * 20);
                    
                    // Highlight the currently executing line of pseudocode in red.
                    if ((int)i == activeLine) {
                        lineText.setFillColor(Color::Red);
                        lineText.setStyle(Text::Bold);
                    } else {
                        lineText.setFillColor(Color::Black);
                    }
                    window.draw(lineText);
                }
            }
        }

        // 6. Finally, display everything that has been drawn in this frame.
        window.display();
    }

    return 0;
}

