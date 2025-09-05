// ===================================================================================
// == FILE: src/QuickSort.cpp ==
// ===================================================================================
//
// AUTHOR: Arpit Jatav
//
// DESCRIPTION: Implements the step-by-step logic for an iterative Quick Sort
// algorithm, including the Lomuto partition scheme, visualization, and
// statistics tracking.
//
// ===================================================================================
#include "QuickSort.h"
#include "VisualizerColor.h"

/**
 * @brief Performs a single step of the iterative Quick Sort algorithm.
 *
 * This function processes one step of the current partition job. It either sets up a
 * new partition, compares an element to the pivot, or swaps elements. Once a
// partition is complete, it places the pivot and pushes new jobs for the
 * sub-partitions onto the stack.
 */
void quickSortStep(std::vector<sf::RectangleShape>& bars, std::vector<int>& arr, QuickSortState& state) {
    if (state.isSorted || !state.isSorting) { state.currentLine = 15; return; }

    state.currentLine = 1; // if low < high
    // If the job stack is empty and the last partition is done, the sort is complete.
    if (state.jobs.empty() && state.needsPartition) {
        state.isSorted = true;
        state.isSorting = false;
        for (auto& bar : bars) bar.setFillColor(BAR_SORTED_COLOR);
        return;
    }

    // Phase 1: Set up a new partition job from the stack.
    // This happens once at the beginning of each new partition.
    if (state.needsPartition) {
        state.currentLine = 2; // p = partition(...)
        QuickSortJob currentJob = state.jobs.top();
        state.jobs.pop();

        state.current_low = currentJob.low;
        state.current_high = currentJob.high;
        state.pivot = arr[state.current_high];
        state.arrayAccesses++; // For reading the pivot value.
        state.i = state.current_low - 1;
        state.j = state.current_low;
        state.needsPartition = false;
        state.currentLine = 8; // pivot = A[high]
    }

    // Reset bar colors for the current step, leaving sorted bars untouched.
    for (size_t k = 0; k < bars.size(); ++k) {
        if (bars[k].getFillColor() != BAR_SORTED_COLOR) {
            bars[k].setFillColor(BAR_DEFAULT_COLOR);
        }
    }
    // Highlight the pivot and the iterators for the current partition.
    if (!state.needsPartition) {
        bars[state.current_high].setFillColor(BAR_COMPARE_COLOR); // Pivot
        if (state.i >= state.current_low) bars[state.i].setFillColor(BAR_COMPARE_COLOR); // Wall 'i'
        if (state.j < state.current_high) bars[state.j].setFillColor(BAR_COMPARE_COLOR); // Iterator 'j'
    }

    // Phase 2: The main partitioning loop.
    // This continues until the iterator 'j' reaches the pivot.
    state.currentLine = 10; // for j = low to high - 1
    if (state.j < state.current_high) {
        state.currentLine = 11; // if A[j] < pivot
        
        state.comparisons++;
        state.arrayAccesses++; // For reading arr[j].
        
        // If the current element is smaller than the pivot...
        if (arr[state.j] < state.pivot) {
            state.i++; // ...move the "wall" forward...
            state.currentLine = 12; // i++
            
            state.arrayAccesses += 4; // ...and swap the elements.
            std::swap(arr[state.i], arr[state.j]);
            state.currentLine = 13; // swap(A[i], A[j])

            // Update the visual bars to reflect the swap.
            sf::Vector2f size1 = bars[state.i].getSize();
            sf::Vector2f size2 = bars[state.j].getSize();
            bars[state.i].setSize({size1.x, size2.y});
            bars[state.i].setPosition(bars[state.i].getPosition().x, 600 - size2.y);
            bars[state.j].setSize({size2.x, size1.y});
            bars[state.j].setPosition(bars[state.j].getPosition().x, 600 - size1.y);
            bars[state.i].setFillColor(BAR_SWAP_COLOR);
            bars[state.j].setFillColor(BAR_SWAP_COLOR);
        }
        state.j++; // Move to the next element.
    }
    // Phase 3: The partition is complete. Place the pivot.
    else {
        int pivot_final_index = state.i + 1;
        
        state.arrayAccesses += 4; // For the final swap of the pivot into place.
        std::swap(arr[pivot_final_index], arr[state.current_high]);
        state.currentLine = 16; // swap(A[i+1], A[high])

        // Update visual bars for the final pivot placement.
        sf::Vector2f size1 = bars[pivot_final_index].getSize();
        sf::Vector2f size2 = bars[state.current_high].getSize();
        bars[pivot_final_index].setSize({size1.x, size2.y});
        bars[pivot_final_index].setPosition(bars[pivot_final_index].getPosition().x, 600 - size2.y);
        bars[state.current_high].setSize({size2.x, size1.y});
        bars[state.current_high].setPosition(bars[state.current_high].getPosition().x, 600 - size1.y);

        // The pivot is now in its final, sorted position.
        bars[pivot_final_index].setFillColor(BAR_SORTED_COLOR);

        // Phase 4: Create new jobs for the sub-partitions to the left and right of the pivot.
        state.currentLine = 3; // quickSort(A, low, p - 1)
        if (state.current_low < pivot_final_index - 1) {
            state.jobs.push({state.current_low, pivot_final_index - 1});
        } else if (state.current_low == pivot_final_index - 1) {
            // If the sub-partition has only one element, it's already sorted.
            bars[state.current_low].setFillColor(BAR_SORTED_COLOR);
        }

        state.currentLine = 4; // quickSort(A, p + 1, high)
        if (pivot_final_index + 1 < state.current_high) {
            state.jobs.push({pivot_final_index + 1, state.current_high});
        } else if (pivot_final_index + 1 == state.current_high) {
            bars[pivot_final_index + 1].setFillColor(BAR_SORTED_COLOR);
        }
        
        // Signal that the next step should start a new partition job from the stack.
        state.needsPartition = true;
    }
}

/**
 * @brief Resets the Quick Sort state and prepares the initial job.
 */
void resetQuickSort(QuickSortState& state, int arrSize) {
    state.isSorted = false;
    state.isSorting = false;
    state.needsPartition = true;
    state.current_low = 0;
    state.current_high = arrSize - 1;
    state.currentLine = 0;
    state.comparisons = 0;
    state.arrayAccesses = 0;

    // Efficiently clear the jobs stack.
    while (!state.jobs.empty()) state.jobs.pop();

    // If there's more than one element, push the first job (the entire array) to the stack.
    if (arrSize > 1) {
        state.jobs.push({0, arrSize - 1});
    } else {
        state.isSorted = true;
    }
}