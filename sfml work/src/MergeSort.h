// ===================================================================================
// == FILE: src/MergeSort.h ==
// ===================================================================================
//
// AUTHOR: Arpit Jatav
//
// DESCRIPTION: Header file for the iterative Merge Sort algorithm. Defines the state
// objects and function prototypes required for a step-by-step visualization that
// simulates the classic recursive "divide and conquer" approach.
//
// ===================================================================================
#ifndef MERGESORT_H
#define MERGESORT_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <stack>

/**
 * @brief Represents a single merge operation for two sorted subarrays.
 *
 * In our iterative simulation of the recursive Merge Sort, each "job" contains the
 * boundaries of the subarrays to be merged and the current indices for the merge process.
 */
struct MergeJob {
    int left;  // The starting index of the left subarray.
    int mid;   // The ending index of the left subarray.
    int right; // The ending index of the right subarray.

    // --- State for the merge process itself ---
    int i; // Current index scanning through the left subarray.
    int j; // Current index scanning through the right subarray.
    int k; // Current index for placing elements in the main array.
};

/**
 * @brief Holds all state information for an iterative Merge Sort in progress.
 */
struct MergeSortState {
    // A stack of MergeJob objects is used to simulate the call stack of the recursive algorithm.
    std::stack<MergeJob> jobs;
    
    // Merge Sort requires a temporary array of the same size as the original for merging.
    std::vector<int> tempArray;
    
    // --- State Flags ---
    bool isSorting = false;
    bool isSorted = false;
    int currentLine = 0;

    // --- Statistics ---
    unsigned long long comparisons = 0;
    unsigned long long arrayAccesses = 0;
};

void mergeSortStep(std::vector<sf::RectangleShape>& bars, std::vector<int>& arr, MergeSortState& state);
void resetMergeSort(MergeSortState& state, int arrSize);

#endif // MERGESORT_H