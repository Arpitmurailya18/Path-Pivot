// ===================================================================================
// == FILE: src/QuickSort.h ==
// ===================================================================================
//
// AUTHOR: Arpit Jatav
//
// DESCRIPTION: Header file for the iterative Quick Sort algorithm. Defines the state
// objects and function prototypes required for a step-by-step visualization that
// simulates the classic recursive "divide and conquer" approach using a job stack.
//
// ===================================================================================
#ifndef QUICKSORT_H
#define QUICKSORT_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <stack>

/**
 * @brief Represents a subarray that needs to be partitioned.
 * In our iterative simulation, each "job" on the stack contains the start (low)
 * and end (high) indices of a segment of the array to be sorted.
 */
struct QuickSortJob {
    int low;
    int high;
};

/**
 * @brief Holds all state information for an iterative Quick Sort in progress.
 */
struct QuickSortState {
    // A stack of QuickSortJob objects is used to simulate the call stack of the recursive algorithm.
    std::stack<QuickSortJob> jobs;
    
    // --- State Flags ---
    bool isSorted = false;
    bool isSorting = false;

    // --- State for the current partition step ---
    bool needsPartition = true; // True when a new partition job needs to be started.
    int pivot = 0;              // The value of the pivot element for the current partition.
    int i = 0;                  // The "wall" or index of the smaller element.
    int j = 0;                  // The main iterator that scans through the subarray.
    int current_low = 0;        // The low bound of the current partition job.
    int current_high = 0;       // The high bound of the current partition job.
    int currentLine = 0;        // The current line of pseudocode to highlight.

    // --- Statistics ---
    unsigned long long comparisons = 0;
    unsigned long long arrayAccesses = 0;
};

void quickSortStep(std::vector<sf::RectangleShape>& bars, std::vector<int>& arr, QuickSortState& state);
void resetQuickSort(QuickSortState& state, int arrSize);

#endif // QUICKSORT_H