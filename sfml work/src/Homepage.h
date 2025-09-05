// ===================================================================================
// == FILE: src/Homepage.h ==
// ===================================================================================
//
// AUTHOR: Arpit Jatav
//
// DESCRIPTION: Header file for the HomeScreen class. This class manages all the
// elements and animations for the main landing page of the application,
// including the background wave effect and instructional text.
//
// ===================================================================================
#ifndef HOMESCREEN_H
#define HOMESCREEN_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath> // For sin() function

/**
 * @brief Manages the state, animation, and drawing of the application's home screen.
 *
 * This class encapsulates all the SFML objects and logic required to render the
 * main menu, including its dynamic background and user instructions.
 */
class HomeScreen {
public:
    /**
     * @brief Constructs the HomeScreen object.
     * @param font The primary font used for all text elements.
     */
    HomeScreen(sf::Font& font);

    /**
     * @brief Updates the state of animations on the home screen.
     * This should be called once per frame in the main application loop.
     */
    void update();

    /**
     * @brief Draws all home screen elements to the specified window.
     * @param window The sf::RenderWindow to draw to.
     */
    void draw(sf::RenderWindow& window);

private:
    // --- UI Text and Layout Elements ---
    sf::Text title;
    sf::Text subtitle;
    sf::Text sortingTitle;
    sf::Text sortingInstructions;
    sf::Text pathfindingTitle;
    sf::Text pathfindingInstructions;
    sf::RectangleShape separator; // A decorative line under the main title.

    // --- Background Animation ---
    std::vector<sf::RectangleShape> backgroundBars; // The bars that create the wave effect.
    sf::Clock animationClock;                     // A timer for the wave and boat animations.

    // --- Floating Boat Animation ---
    sf::ConvexShape paperBoat; // The shape for the floating paper boat.
    float boatXPosition;       // The current horizontal position of the boat.
};

#endif // HOMESCREEN_H