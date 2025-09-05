// ===================================================================================
// == FILE: src/BubbleSort.cpp ==
// ===================================================================================
//
// AUTHOR: Arpit Jatav
//
// DESCRIPTION: Implements the step-by-step logic for the Bubble Sort algorithm,
// including the visualization of comparisons and swaps, and statistics tracking.
//
// ===================================================================================
#include "BubbleSort.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include "VisualizerColor.h"


using namespace sf;
using namespace std;

// Define colors for visualization
const Color BAR_DEFAULT_COLOR = Color(108, 52, 97);
const Color BAR_COMPARE_COLOR = Color::Yellow;
const Color BAR_SWAP_COLOR = Color::Red;
const Color BAR_SORTED_COLOR = Color::Green;

void bubbleSortStep(std::vector<sf::RectangleShape>& bars, std::vector<int>& arr, BubbleSortState& state) {
    // If the sort is already complete, do nothing.
    if (state.isSorted) { state.currentLine = 12; return; }

    state.currentLine = 2; // repeat
    // Reset colors for the unsorted part of the array before the next comparison.
    for (size_t k = 0; k < arr.size() - state.i; ++k) {
        bars[k].setFillColor(BAR_DEFAULT_COLOR);
    }

    // This block runs when one full pass of the inner loop (j) is complete.
    if (state.j >= arr.size() - state.i - 1) {
        state.currentLine = 10; // n = n - 1
        // The last element of the pass is now in its correct sorted position.
        bars[arr.size() - 1 - state.i].setFillColor(BAR_SORTED_COLOR);
        
        // --- Early Exit Optimization ---
        // If an entire pass was completed with no swaps, the array is already sorted.
        if (!state.swapped) {
            state.isSorted = true;
            for (auto& bar : bars) bar.setFillColor(BAR_SORTED_COLOR);
            return;
        }
        
        // Reset for the next pass.
        state.swapped = false;
        state.i++;
        state.j = 0;
        state.currentLine = 3; // swapped = false
    }

    // Check if all passes are complete.
    if (state.i >= arr.size() - 1) {
        state.isSorted = true;
        for (auto& bar : bars) bar.setFillColor(BAR_SORTED_COLOR);
        return;
    }
    
    // 1. Highlight the two elements being compared.
    state.currentLine = 4; // for i = 1 to n-1
    bars[state.j].setFillColor(BAR_COMPARE_COLOR);
    bars[state.j + 1].setFillColor(BAR_COMPARE_COLOR);
    
    // 2. Perform the comparison.
    state.currentLine = 5; // if A[i-1] > A[i]
    state.comparisons++;
    state.arrayAccesses += 2; // For the two reads in the comparison.
    
    if (arr[state.j] > arr[state.j + 1]) {
        // 3. If elements are out of order, perform a swap.
        state.currentLine = 6; // swap
        
        state.arrayAccesses += 4; // For the two reads and two writes in the swap.
        std::swap(arr[state.j], arr[state.j + 1]);

        // Update the visual bars to reflect the swap.
        sf::Vector2f size1 = bars[state.j].getSize();
        sf::Vector2f size2 = bars[state.j + 1].getSize();
        bars[state.j].setSize({size1.x, size2.y});
        bars[state.j].setPosition(bars[state.j].getPosition().x, 600 - size2.y);
        bars[state.j + 1].setSize({size2.x, size1.y});
        bars[state.j + 1].setPosition(bars[state.j + 1].getPosition().x, 600 - size1.y);

        bars[state.j].setFillColor(BAR_SWAP_COLOR);
        bars[state.j + 1].setFillColor(BAR_SWAP_COLOR);
        state.swapped = true; // Mark that a swap occurred in this pass.
        state.currentLine = 7; // swapped = true
    }

    // Move to the next pair of elements.
    state.j++;
}

void resetBubbleSort(BubbleSortState& state) {
    state.i = 0;
    state.j = 0;
    state.swapped = false;
    state.isSorted = false;
    state.currentLine = 0;
    state.comparisons = 0;
    state.arrayAccesses = 0;
}