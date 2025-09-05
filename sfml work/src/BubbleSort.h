// ===================================================================================
// == FILE: src/BubbleSort.h ==
// ===================================================================================
//
// AUTHOR: Arpit Jatav
//
// DESCRIPTION: Header file for the Bubble Sort algorithm. Defines the state
// object and function prototypes required for the step-by-step visualization.
//
// ===================================================================================
#ifndef BUBBLESORT_H
#define BUBBLESORT_H

#include <SFML/Graphics.hpp>
#include <vector>

/**
 * @brief Holds all state information for a Bubble Sort in progress.
 *
 * This struct contains the counters for the outer and inner loops (i, j),
 * flags for controlling the visualization, and counters for tracking statistics.
 * The `swapped` flag is used for an optimization that allows the sort to
 * terminate early if a pass is completed with no swaps.
 */
struct BubbleSortState {
    int i = 0;             // Counter for the outer loop (number of passes completed).
    int j = 0;             // Counter for the inner loop (current comparison index).
    bool swapped = false;  // Flag for the early-exit optimization.
    bool isSorted = false; // True when the entire array is sorted.
    int currentLine = 0;   // The current line of pseudocode to highlight.

    // --- Statistics ---
    unsigned long long comparisons = 0;   // Total number of comparisons made.
    unsigned long long arrayAccesses = 0; // Total number of array reads/writes.
};

/**
 * @brief Performs a single step of the Bubble Sort algorithm.
 * @param bars The vector of sf::RectangleShape for visualization.
 * @param arr The integer array being sorted.
 * @param state The current state of the Bubble Sort (passed by reference).
 */
void bubbleSortStep(std::vector<sf::RectangleShape>& bars, std::vector<int>& arr, BubbleSortState& state);

/**
 * @brief Resets the Bubble Sort state to its default values for a new sort.
 * @param state The Bubble Sort state object to reset (passed by reference).
 */
void resetBubbleSort(BubbleSortState& state);

#endif // BUBBLESORT_H