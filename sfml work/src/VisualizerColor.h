// ===================================================================================
// == FILE: src/VisualizerColor.h ==
// ===================================================================================
//
// AUTHOR: Arpit Jatav
//
// DESCRIPTION: This header file declares global constants for the colors used
// throughout the sorting visualizations. By using the 'extern' keyword, we can
// define these colors once in the main C++ file and then share them across all
// other files (like BubbleSort.cpp, SelectionSort.cpp, etc.) without causing
// linker errors. This creates a single source of truth for the color palette.
//
// ===================================================================================
#ifndef VISUALIZER_COLOR_H
#define VISUALIZER_COLOR_H

#include <SFML/Graphics.hpp>

// These color constants are defined in the main application file (e.g., Day19.cpp).
// The `extern` keyword tells the compiler that these variables exist somewhere else,
// allowing other .cpp files that include this header to use them.

extern const sf::Color BAR_DEFAULT_COLOR; // The default color for bars in the array.
extern const sf::Color BAR_COMPARE_COLOR; // Color for bars being actively compared.
extern const sf::Color BAR_SWAP_COLOR;    // Color for bars that are being swapped.
extern const sf::Color BAR_SORTED_COLOR;  // Color for bars that are in their final, sorted position.
extern const sf::Color BAR_MIN_COLOR;     // Color for highlighting the current minimum element (e.g., in Selection Sort).

#endif // VISUALIZER_COLOR_H