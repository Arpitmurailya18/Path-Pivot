// ===================================================================================
// == FILE: src/SelectionSort.h ==
// ===================================================================================
//
// AUTHOR: Arpit Jatav
//
// DESCRIPTION: Header file for the Selection Sort algorithm. Defines the state
// object and function prototypes required for the step-by-step visualization.
//
// ===================================================================================
#ifndef SELECTIONSORT_H
#define SELECTIONSORT_H

#include <SFML/Graphics.hpp>
#include <vector>

/**
 * @brief Holds all state information for a Selection Sort in progress.
 *
 * This struct contains the counters for the outer and inner loops (i, j), an index
 * to track the current minimum, flags for controlling the visualization, and
 * counters for tracking statistics. The `findingMin` flag manages the two-phase
 * nature of each pass (finding the minimum, then swapping).
 */
struct SelectionSortState {
    int i = 0;             // The boundary of the sorted portion of the array.
    int j = 1;             // The iterator for scanning the unsorted portion.
    int min_idx = 0;       // The index of the smallest element found so far in the current pass.
    bool isSorted = false; // True when the entire array is sorted.
    bool findingMin = true;// A flag to manage the state within a single pass.
    int currentLine = 0;   // The current line of pseudocode to highlight.

    // --- Statistics ---
    unsigned long long comparisons = 0;
    unsigned long long arrayAccesses = 0;
};

/**
 * @brief Performs a single step of the Selection Sort algorithm.
 * @param bars The vector of sf::RectangleShape for visualization.
 * @param arr The integer array being sorted.
 * @param state The current state of the Selection Sort (passed by reference).
 */
void selectionSortStep(std::vector<sf::RectangleShape>& bars, std::vector<int>& arr, SelectionSortState& state);

/**
 * @brief Resets the Selection Sort state to its default values for a new sort.
 * @param state The Selection Sort state object to reset (passed by reference).
 */
void resetSelectionSort(SelectionSortState& state);

#endif // SELECTIONSORT_H