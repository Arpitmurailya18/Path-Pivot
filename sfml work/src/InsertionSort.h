// ===================================================================================
// == FILE: src/InsertionSort.h ==
// ===================================================================================
//
// AUTHOR: Arpit Jatav
//
// DESCRIPTION: Header file for the Insertion Sort algorithm. Defines the state
// object and function prototypes required for the step-by-step visualization.
//
// ===================================================================================
#ifndef INSERTIONSORT_H
#define INSERTIONSORT_H

#include <SFML/Graphics.hpp>
#include <vector>

/**
 * @brief Holds all state information for an Insertion Sort in progress.
 *
 * This struct contains the counters for the outer and inner loops (i, j), the 'key'
 * being inserted, flags for controlling the visualization, and counters for
 * tracking statistics. The `keyPickedUp` flag helps manage the two-phase nature
 * of each step (finding the position, then inserting).
 */
struct InsertionSortState {
    int i = 1;             // The index of the element to be inserted into the sorted portion.
    int j = 0;             // The index used to scan backwards through the sorted portion.
    int key = 0;           // The value of the element currently being inserted.
    bool isSorted = false; // True when the entire array is sorted.
    bool keyPickedUp = false; // A flag to manage the state within a single pass.
    int currentLine = 0;   // The current line of pseudocode to highlight.

    // --- Statistics ---
    unsigned long long comparisons = 0;
    unsigned long long arrayAccesses = 0;
};

/**
 * @brief Performs a single step of the Insertion Sort algorithm.
 * @param bars The vector of sf::RectangleShape for visualization.
 * @param arr The integer array being sorted.
 * @param state The current state of the Insertion Sort (passed by reference).
 */
void insertionSortStep(std::vector<sf::RectangleShape>& bars, std::vector<int>& arr, InsertionSortState& state);

/**
 * @brief Resets the Insertion Sort state to its default values for a new sort.
 * @param state The Insertion Sort state object to reset (passed by reference).
 */
void resetInsertionSort(InsertionSortState& state);

#endif // INSERTIONSORT_H