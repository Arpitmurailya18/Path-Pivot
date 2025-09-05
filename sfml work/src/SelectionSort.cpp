// ===================================================================================
// == FILE: src/SelectionSort.cpp ==
// ===================================================================================
//
// AUTHOR: Arpit Jatav
//
// DESCRIPTION: Implements the step-by-step logic for the Selection Sort algorithm,
// including the visualization of finding the minimum element and swapping it
// into place, as well as statistics tracking.
//
// ===================================================================================
#include <vector> 
#include <SFML/Graphics.hpp>
#include "SelectionSort.h"
#include "VisualizerColor.h"

// Note: These extern declarations are only needed if VisualizerColor.h is not included.
// It's better practice to include the header.
extern const sf::Color BAR_DEFAULT_COLOR;
extern const sf::Color BAR_COMPARE_COLOR;
extern const sf::Color BAR_SWAP_COLOR;
extern const sf::Color BAR_SORTED_COLOR;

void selectionSortStep(std::vector<sf::RectangleShape>& bars, std::vector<int>& arr, SelectionSortState& state) {
    // If the sort is already complete, do nothing.
    if (state.isSorted) { state.currentLine = 11; return; }

    state.currentLine = 2; // for i = 0 to n - 1
    // Reset colors for the unsorted part of the array, leaving the sorted part green.
    for (size_t k = state.i; k < arr.size(); ++k) {
        bars[k].setFillColor(BAR_DEFAULT_COLOR);
    }
    for (size_t k = 0; k < state.i; ++k) {
        bars[k].setFillColor(BAR_SORTED_COLOR);
    }

    // The algorithm works in two phases per outer loop iteration (i):
    // 1. `findingMin = true`: Scan the unsorted part to find the index of the minimum element.
    // 2. `findingMin = false`: Swap the found minimum element with the element at index `i`.

    if (state.findingMin) {
        // --- Phase 1: Find the minimum element ---
        state.currentLine = 4; // for j = i + 1 to n
        if (state.j < arr.size()) {
            // Highlight the current element being checked and the current minimum.
            bars[state.j].setFillColor(BAR_COMPARE_COLOR);
            bars[state.min_idx].setFillColor(BAR_COMPARE_COLOR);
            state.currentLine = 5; // if A[j] < A[minIndex]
            
            state.comparisons++;
            state.arrayAccesses += 2; // For the two reads in the comparison.
            
            // If we find a new minimum, update our index.
            if (arr[state.j] < arr[state.min_idx]) {
                state.currentLine = 6; // minIndex = j
                state.min_idx = state.j;
            }
            state.j++; // Move to the next element.
        } else {
            // If we've scanned the entire unsorted part, switch to the swap phase.
            state.findingMin = false;
        }
    } else {
        // --- Phase 2: Swap the minimum element into its correct sorted position ---
        state.currentLine = 9; // swap(A[i], A[minIndex])

        state.arrayAccesses += 4; // 2 reads and 2 writes for the swap.
        std::swap(arr[state.min_idx], arr[state.i]);

        // Update the visual bars to reflect the swap.
        sf::Vector2f size1 = bars[state.i].getSize();
        sf::Vector2f size2 = bars[state.min_idx].getSize();
        bars[state.i].setSize({size1.x, size2.y});
        bars[state.i].setPosition(bars[state.i].getPosition().x, 600 - size2.y);
        bars[state.min_idx].setSize({size2.x, size1.y});
        bars[state.min_idx].setPosition(bars[state.min_idx].getPosition().x, 600 - size1.y);
        
        // The element at `i` is now sorted.
        bars[state.i].setFillColor(BAR_SORTED_COLOR);

        // Move the sorted boundary forward.
        state.i++;
        
        // Check if the entire array is now sorted.
        if (state.i >= arr.size() - 1) {
            state.isSorted = true;
            for(auto& bar : bars) bar.setFillColor(BAR_SORTED_COLOR);
            return;
        }

        // Reset for the next pass.
        state.min_idx = state.i;
        state.j = state.i + 1;
        state.findingMin = true;
        state.currentLine = 3; // minIndex = i
    }
}

void resetSelectionSort(SelectionSortState& state) {
    state.i = 0;
    state.j = 1;
    state.min_idx = 0;
    state.isSorted = false;
    state.findingMin = true;
    state.currentLine = 0;
    state.comparisons = 0;
    state.arrayAccesses = 0;
}