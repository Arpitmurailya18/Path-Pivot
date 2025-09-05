// ===================================================================================
// == FILE: src/InsertionSort.cpp ==
// ===================================================================================
//
// AUTHOR: Arpit Jatav
//
// DESCRIPTION: Implements the step-by-step logic for the Insertion Sort algorithm,
// including the visualization of shifting elements and statistics tracking.
//
// ===================================================================================
#include <SFML/Graphics.hpp>
#include "InsertionSort.h"
#include "VisualizerColor.h"

void insertionSortStep(std::vector<sf::RectangleShape>& bars, std::vector<int>& arr, InsertionSortState& state) {
    if (state.isSorted) { state.currentLine = 10; return; }

    state.currentLine = 1; // for i = 1 to length(A) - 1
    // Visually update the sorted portion of the array.
    for (int k = 0; k < state.i; ++k) {
        bars[k].setFillColor(BAR_SORTED_COLOR);
    }
    for (size_t k = state.i; k < arr.size(); ++k) {
        bars[k].setFillColor(BAR_DEFAULT_COLOR);
    }

    // Phase 1: Pick up the key from the unsorted portion.
    // This happens once at the beginning of each outer loop iteration (i).
    if (!state.keyPickedUp) {
        state.currentLine = 2; // key = A[i]
        state.key = arr[state.i];
        state.arrayAccesses++; // Read arr[i] to get the key.

        state.currentLine = 3; // j = i - 1
        state.j = state.i - 1;
        state.keyPickedUp = true;
    }

    // Highlight the key's original position and the element it's being compared against.
    bars[state.i].setFillColor(BAR_COMPARE_COLOR);
    if (state.j >= 0) {
        bars[state.j].setFillColor(BAR_COMPARE_COLOR);
    }

    // Phase 2: Scan backwards through the sorted portion and shift elements.
    state.currentLine = 4; // while j >= 0 and A[j] > key
    
    bool conditionMet = false;
    if (state.j >= 0) {
        state.comparisons++;
        state.arrayAccesses++; // Read arr[j] for the comparison.
        if (arr[state.j] > state.key) {
            conditionMet = true;
        }
    }

    if (conditionMet) {
        // If the sorted element is greater than the key, shift it to the right.
        state.currentLine = 5; // A[j+1] = A[j]
        arr[state.j + 1] = arr[state.j];
        state.arrayAccesses += 2; // 1 read and 1 write.

        // Update the visual bar to show the shift.
        bars[state.j + 1].setSize({bars[state.j].getSize().x, bars[state.j].getSize().y});
        bars[state.j + 1].setPosition(bars[state.j + 1].getPosition().x, 600 - bars[state.j].getSize().y);
        bars[state.j + 1].setFillColor(BAR_SWAP_COLOR);

        state.currentLine = 6; // j = j - 1
        state.j--;
    } else {
        // Phase 3: Insert the key into its correct position.
        // This happens when we've found the right spot (or reached the beginning).
        state.currentLine = 8; // A[j+1] = key
        arr[state.j + 1] = state.key;
        state.arrayAccesses++; // Write the key into the array.

        // Update the visual bar for the inserted key.
        float barWidth = bars[state.j + 1].getSize().x;
        bars[state.j + 1].setSize({barWidth, (float)state.key});
        bars[state.j + 1].setPosition(bars[state.j + 1].getPosition().x, 600 - (float)state.key);
        bars[state.j + 1].setFillColor(BAR_SORTED_COLOR);

        // Move to the next element in the unsorted portion.
        state.i++;

        // Check if the entire array is now sorted.
        if (state.i >= arr.size()) {
            state.isSorted = true;
            for(auto& bar : bars) bar.setFillColor(BAR_SORTED_COLOR);
            return;
        }
        // Reset the flag to pick up the next key in the following step.
        state.keyPickedUp = false;
    }
}

void resetInsertionSort(InsertionSortState& state) {
    state.i = 1;
    state.j = 0;
    state.isSorted = false;
    state.keyPickedUp = false;
    state.currentLine = 0;
    state.comparisons = 0;
    state.arrayAccesses = 0;
}