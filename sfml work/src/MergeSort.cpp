// ===================================================================================
// == FILE: src/MergeSort.cpp ==
// ===================================================================================
//
// AUTHOR: Arpit Jatav
//
// DESCRIPTION: Implements the step-by-step logic for an iterative Merge Sort
// algorithm, including the pre-calculation of merge "jobs" to simulate
// recursion, visualization, and statistics tracking.
//
// ===================================================================================
#include "MergeSort.h"
#include "VisualizerColor.h"
#include <algorithm> // For std::min

/**
 * @brief Performs a single step of the iterative Merge Sort algorithm.
 *
 * This function processes one step of the current MergeJob at the top of the stack.
 * It compares and merges one element at a time from the tempArray back into the main
 * array. Once a job is complete, it copies the newly sorted segment back to the
 * tempArray for future merges and pops the job from the stack.
 */
void mergeSortStep(std::vector<sf::RectangleShape>& bars, std::vector<int>& arr, MergeSortState& state) {
    if (state.isSorted || !state.isSorting) { state.currentLine = 7; return; }

    state.currentLine = 2; // for curr_size...
    // If the job stack is empty, the entire sort is complete.
    if (state.jobs.empty()) {
        state.isSorted = true;
        state.isSorting = false;
        for (auto& bar : bars) bar.setFillColor(BAR_SORTED_COLOR);
        return;
    }

    state.currentLine = 3; // for left_start...
    MergeJob& currentJob = state.jobs.top();

    // Highlight the entire range of the current merge operation.
    for (size_t barIdx = 0; barIdx < bars.size(); ++barIdx) {
        if (barIdx >= (size_t)currentJob.left && barIdx <= (size_t)currentJob.right) {
             bars[barIdx].setFillColor(BAR_COMPARE_COLOR);
        } else {
             bars[barIdx].setFillColor(BAR_DEFAULT_COLOR);
        }
    }

    // --- Core Merge Logic ---
    // This series of if/else if blocks merges one element per step.
    state.currentLine = 6; // merge(...)
    if (currentJob.i <= currentJob.mid && currentJob.j <= currentJob.right) {
        state.comparisons++;
        state.arrayAccesses += 2; // For reading from tempArray.

        if (state.tempArray[currentJob.i] <= state.tempArray[currentJob.j]) {
            arr[currentJob.k] = state.tempArray[currentJob.i];
            state.arrayAccesses += 2; // 1 read from temp, 1 write to main.
            currentJob.i++;
        } else {
            arr[currentJob.k] = state.tempArray[currentJob.j];
            state.arrayAccesses += 2; // 1 read from temp, 1 write to main.
            currentJob.j++;
        }
        currentJob.k++;
    }
    // If the right subarray is exhausted, copy any remaining elements from the left.
    else if (currentJob.i <= currentJob.mid) {
        arr[currentJob.k] = state.tempArray[currentJob.i];
        state.arrayAccesses += 2;
        currentJob.i++;
        currentJob.k++;
    }
    // If the left subarray is exhausted, copy any remaining elements from the right.
    else if (currentJob.j <= currentJob.right) {
        arr[currentJob.k] = state.tempArray[currentJob.j];
        state.arrayAccesses += 2;
        currentJob.j++;
        currentJob.k++;
    }
    // If both subarrays are exhausted, the merge for this job is complete.
    else {
        // Copy the newly sorted segment from the main array back to the temp array.
        // This is crucial for the next level of merges.
        for (int i = currentJob.left; i <= currentJob.right; ++i) {
            state.tempArray[i] = arr[i];
            state.arrayAccesses += 2;
            // Update the visual bars to reflect the sorted segment.
            bars[i].setSize({bars[i].getSize().x, (float)arr[i]});
            bars[i].setPosition(bars[i].getPosition().x, 600 - (float)arr[i]);
            bars[i].setFillColor(BAR_SORTED_COLOR);
        }
        // This job is done, move to the next one.
        state.jobs.pop();
    }
}

/**
 * @brief Resets the Merge Sort state and pre-calculates all merge jobs.
 *
 * This function prepares the state for a new sort. It uses a bottom-up approach
 * to determine all the merge operations that will be needed and pushes them
 * onto a stack. The step function then consumes this stack of jobs.
 */
void resetMergeSort(MergeSortState& state, int arrSize) {
    state.isSorted = false;
    state.isSorting = false;
    state.tempArray.assign(arrSize, 0);
    state.currentLine = 0;
    state.comparisons = 0;
    state.arrayAccesses = 0;

    while (!state.jobs.empty()) state.jobs.pop();

    // --- Pre-calculation of Merge Jobs ---
    // These loops simulate the "divide" part of the "divide and conquer" strategy.
    // They iterate through different subarray sizes (curr_size) and positions (left_start)
    // to determine all the merges that need to happen, from small to large.
    for (int curr_size = 1; curr_size <= arrSize - 1; curr_size = 2 * curr_size) {
        for (int left_start = 0; left_start < arrSize - 1; left_start += 2 * curr_size) {
            int mid = std::min(left_start + curr_size - 1, arrSize - 1);
            int right_end = std::min(left_start + 2 * curr_size - 1, arrSize - 1);

            MergeJob job;
            job.left = left_start;
            job.mid = mid;
            job.right = right_end;
            job.i = left_start;
            job.j = mid + 1;
            job.k = left_start;
            state.jobs.push(job);
        }
    }
}