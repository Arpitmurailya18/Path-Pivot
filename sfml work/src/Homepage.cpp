// ===================================================================================
// == FILE: src/Homepage.cpp ==
// ===================================================================================
//
// AUTHOR: Arpit Jatav
//
// DESCRIPTION: Implements the logic for the HomeScreen class, including the
// constructor to initialize all UI elements and the update/draw methods to
// handle the animations and rendering.
//
// ===================================================================================
#include "Homepage.h"

HomeScreen::HomeScreen(sf::Font& font) {
    // --- Background Bars Initialization ---
    // This loop creates the vertical bars for the background wave animation.
    int numBars = 100;
    float barWidth = 1030.0f / numBars;
    for (int i = 0; i < numBars; ++i) {
        sf::RectangleShape bar;
        bar.setSize(sf::Vector2f(barWidth, 200));
        bar.setPosition(i * barWidth, 0);
        
        // Use a base color and create a subtle transparency gradient across the bars.
        int r = 70;
        int g = 130;
        int b = 180;
        int alpha = 30 + (i * 40 / numBars); // Vary alpha from 30 to 70 for a nice effect.
        bar.setFillColor(sf::Color(r, g, b, alpha)); 
        
        backgroundBars.push_back(bar);
    }

    // --- Paper Boat Initialization ---
    // The boat is created using a ConvexShape, which allows for custom polygons.
    paperBoat.setPointCount(7);
    paperBoat.setPoint(0, sf::Vector2f(0, 20));
    paperBoat.setPoint(1, sf::Vector2f(60, 20));
    paperBoat.setPoint(2, sf::Vector2f(50, 35));
    paperBoat.setPoint(3, sf::Vector2f(10, 35));
    paperBoat.setPoint(4, sf::Vector2f(0, 20));
    paperBoat.setPoint(5, sf::Vector2f(30, 0));
    paperBoat.setPoint(6, sf::Vector2f(30, 20));
    paperBoat.setFillColor(sf::Color(139, 69, 19)); // A "wood brown" color.
    paperBoat.setOutlineColor(sf::Color(92, 51, 23)); // A darker outline.
    paperBoat.setOutlineThickness(2);
    boatXPosition = -100.0f; // Start off-screen to the left for a smooth entrance.


    // --- Typography and Layout Initialization ---
    // This section sets up all the static text and layout elements for the home screen.
    title.setFont(font);
    title.setString("Interactive Algorithm Visualizer");
    title.setCharacterSize(60);
    title.setStyle(sf::Text::Bold);
    title.setFillColor(sf::Color(45, 55, 72));
    sf::FloatRect titleRect = title.getLocalBounds();
    title.setOrigin(titleRect.left + titleRect.width / 2.0f, titleRect.top + titleRect.height / 2.0f);
    title.setPosition(1030 / 2.0f, 120);

    separator.setSize(sf::Vector2f(600, 2));
    separator.setFillColor(sf::Color(200, 200, 200));
    separator.setOrigin(300, 1);
    separator.setPosition(1030 / 2.0f, 190);

    subtitle.setFont(font);
    subtitle.setString("Choose a mode from the panel on the right to begin.");
    subtitle.setCharacterSize(22);
    subtitle.setFillColor(sf::Color(100, 100, 100));
    sf::FloatRect subtitleRect = subtitle.getLocalBounds();
    subtitle.setOrigin(subtitleRect.left + subtitleRect.width / 2.0f, subtitleRect.top + subtitleRect.height / 2.0f);
    subtitle.setPosition(1030 / 2.0f, 220);

    sortingTitle.setFont(font);
    sortingTitle.setString("Sorting Mode");
    sortingTitle.setCharacterSize(30);
    sortingTitle.setStyle(sf::Text::Bold);
    sortingTitle.setFillColor(sf::Color::Black);
    sortingTitle.setPosition(100, 300);

    sortingInstructions.setFont(font);
    sortingInstructions.setString(
        "- Watch classic sorting algorithms in action.\n"
        "- Control the speed with the slider.\n"
        "- Reset the array to generate a new dataset."
    );
    sortingInstructions.setCharacterSize(20);
    sortingInstructions.setFillColor(sf::Color(50, 50, 50));
    sortingInstructions.setPosition(100, 350);
    sortingInstructions.setLineSpacing(1.3f);

    pathfindingTitle.setFont(font);
    pathfindingTitle.setString("Pathfinding Mode");
    pathfindingTitle.setCharacterSize(30);
    pathfindingTitle.setStyle(sf::Text::Bold);
    pathfindingTitle.setFillColor(sf::Color::Black);
    pathfindingTitle.setPosition(600, 300);

    pathfindingInstructions.setFont(font);
    pathfindingInstructions.setString(
        "- See how algorithms find the shortest path.\n"
        "- Left-click to place Start and End nodes.\n"
        "- Right-click or Shift+Click to draw walls.\n"
        "- W-key+Click for weight nodes.\n"
        "- Use the buttons to clear the path or the maze."
    );
    pathfindingInstructions.setCharacterSize(20);
    pathfindingInstructions.setFillColor(sf::Color(50, 50, 50));
    pathfindingInstructions.setPosition(600, 350);
    pathfindingInstructions.setLineSpacing(1.3f);
}

void HomeScreen::update() {
    // This function runs every frame to update the state of the animations.
    float time = animationClock.getElapsedTime().asSeconds();
    
    // 1. Update the background wave animation.
    for (size_t i = 0; i < backgroundBars.size(); ++i) {
        // A sine wave is used to create a smooth, continuous up-and-down motion.
        // The time and the bar's index are used as input to create the wave effect.
        float sine_input = time * 2.0f + (float)i * 0.2f;
        float height_offset = std::sin(sine_input) * 20.0f; // 20 pixels of vertical movement.
        float newHeight = 150.0f + height_offset;
        
        backgroundBars[i].setSize(sf::Vector2f(backgroundBars[i].getSize().x, newHeight));
        backgroundBars[i].setPosition(backgroundBars[i].getPosition().x, 720 - newHeight); // Anchor to the bottom.
    }

    // 2. Update the floating boat animation.
    boatXPosition += 20.0f * 0.016f; // Move at a consistent speed (approx. 20 pixels/sec).
    if (boatXPosition > 1030) {
        boatXPosition = -50.0f; // Reset to the left when it goes off-screen.
    }

    // To make the boat float, we find which bar it's "on top of".
    int barIndex = static_cast<int>(boatXPosition / backgroundBars[0].getSize().x);
    if (barIndex >= 0 && barIndex < backgroundBars.size()) {
        // Get the Y position of that bar and place the boat on top of it.
        float waveY = backgroundBars[barIndex].getPosition().y;
        paperBoat.setPosition(boatXPosition, waveY - 30); // "- 30" adjusts for the boat's height.
    } else {
        // Use a default position if the boat is off-screen.
        paperBoat.setPosition(boatXPosition, 720 - 150 - 30);
    }
}

void HomeScreen::draw(sf::RenderWindow& window) {
    // The drawing order is important for correct layering (back to front).
    
    // 1. Draw the background elements first.
    for (const auto& bar : backgroundBars) {
        window.draw(bar);
    }
    window.draw(paperBoat);

    // 2. Draw the text and UI elements on top of the background.
    window.draw(title);
    window.draw(separator);
    window.draw(subtitle);
    window.draw(sortingTitle);
    window.draw(sortingInstructions);
    window.draw(pathfindingTitle);
    window.draw(pathfindingInstructions);
}