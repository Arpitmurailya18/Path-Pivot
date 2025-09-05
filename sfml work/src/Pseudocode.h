// ===================================================================================
// == FILE: src/Pseudocode.h ==
// ===================================================================================
//
// AUTHOR: Arpit Jatav
//
// DESCRIPTION: Defines the PseudocodeManager class, which is responsible for
// storing and providing the pseudocode text for every algorithm in the
// visualizer. This acts as a centralized "database" for all pseudocode content.
//
// ===================================================================================

#ifndef PSEUDOCODE_H
#define PSEUDOCODE_H

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <map>

/**
 * @brief A manager class to store and retrieve pseudocode for all algorithms.
 *
 * This class uses a map to associate an algorithm's name (as a string) with a
 * vector of strings, where each string is a single line of its pseudocode. The
 * pseudocode for every algorithm is hardcoded directly in the constructor for simplicity.
 */

class PseudocodeManager {
public:
    // A map where the key is the algorithm's name (e.g., "Bubble Sort") and the
    // value is a vector of strings containing each line of its pseudocode.
    std::map<std::string, std::vector<std::string>> pseudocodes;

    /**
     * @brief Constructs the PseudocodeManager and initializes all pseudocode data.
     * When a PseudocodeManager object is created, this constructor immediately
     * populates the `pseudocodes` map with all the necessary algorithm text.
     */
    PseudocodeManager() {
        // --- Sorting Algorithm Pseudocode ---
        pseudocodes["Bubble Sort"] = {
            "procedure bubbleSort(A)",
            " n = length(A)",
            " repeat",
            "  swapped = false",
            "  for i = 1 to n-1 do",
            "   if A[i-1] > A[i] then",
            "    swap(A[i-1], A[i])",
            "    swapped = true",
            "   end if",
            "  end for",
            "  n = n - 1",
            " until not swapped",
            "end procedure"
        };

        pseudocodes["Selection Sort"] = {
            "procedure selectionSort(A)",
            " n = length(A)",
            " for i = 0 to n - 1 do",
            "  minIndex = i",
            "  for j = i + 1 to n do",
            "   if A[j] < A[minIndex] then",
            "    minIndex = j",
            "   end if",
            "  end for",
            "  swap(A[i], A[minIndex])",
            " end for",
            "end procedure"
        };
        
        pseudocodes["Insertion Sort"] = {
            "procedure insertionSort(A)",
            " for i = 1 to length(A) - 1 do",
            "  key = A[i]",
            "  j = i - 1",
            "  while j >= 0 and A[j] > key do",
            "   A[j+1] = A[j]",
            "   j = j - 1",
            "  end while",
            "  A[j+1] = key",
            " end for",
            "end procedure"
        };

        pseudocodes["Merge Sort"] = {
            "procedure mergeSort(A)",
            " for curr_size = 1 to n-1 by 2*curr_size",
            "  for left_start = 0 to n-1 by 2*curr_size",
            "   mid = ...",
            "   right_end = ...",
            "   merge(A,left_start,mid,right_end)",
            "  end for",
            " end for",
            "end procedure"
        };

        pseudocodes["Quick Sort"] = {
            "procedure quickSort(A,low,high)",
            " if low < high",
            "  p = partition(A, low, high)",
            "  quickSort(A, low, p - 1)",
            "  quickSort(A, p + 1, high)",
            " end if",
            "end procedure",
            "procedure partition(A,low,high)",
            " pivot = A[high]",
            " i = low - 1",
            " for j = low to high - 1",
            "  if A[j] < pivot",
            "   i++",
            "   swap(A[i], A[j])",
            "  end if",
            " end for",
            " swap(A[i+1], A[high])",
            " return i + 1",
            "end procedure"
        };
        // --- Pathfinding Algorithm Pseudocode ---
        pseudocodes["BFS"] = {
            "procedure BFS(graph,start,end)",
            " let Q be a queue",
            " Q.enqueue(start)",
            " mark start as visited",
            " while Q is not empty do",
            "  current = Q.dequeue()",
            "  if current is end then",
            "   return PathFound",
            "  end if",
            "  for each neighbor of current do",
            "   if neighbor is not visited then",
            "     mark neighbor as visited",
            "     Q.enqueue(neighbor)",
            "   end if",
            "  end for",
            " end while",
            " return PathNotFound",
            "end procedure"
        };
        
        pseudocodes["DFS"] = {
            "procedure DFS(graph,start,end)",
            " let S be a stack",
            " S.push(start)",
            " while S is not empty do",
            "  current = S.pop()",
            "  if current is not visited then",
            "    mark current as visited",
            "    if current is end then",
            "      return PathFound",
            "    end if",
            "    for each neighbor of current do",
            "      S.push(neighbor)",
            "    end for",
            "  end if",
            " end while",
            " return PathNotFound",
            "end procedure"
        };

         pseudocodes["A* Search"] = {
            "procedure A*(start, goal)",
            " openSet.add(start)",
            " while openSet is not empty",
            "  current = node in openSet with lowest fCost",
            "  if current == goal",
            "    return PathFound",
            "  end if",
            "  for each neighbor of current",
            "   tentative_gCost = gCost[current] + 1",
            "   if tentative_gCost < gCost[neighbor]",
            "    parent[neighbor] = current",
            "    gCost[neighbor] = tentative_gCost",
            "    fCost[neighbor] = gCost + heuristic",
            "    openSet.add(neighbor)",
            "   end if",
            "  end for",
            " end while",
            " return PathNotFound",
            "end procedure"
        };

        pseudocodes["Dijkstra"] = {
            "procedure Dijkstra(graph, start, end)",
            " dist[source] = 0",
            " create vertex priority queue Q",
            " while Q is not empty",
            "  u = vertex in Q with min distance",
            "  remove u from Q",
            "  for each neighbor v of u",
            "   alt = dist[u] + length(u, v)",
            "   if alt < dist[v]",
            "    dist[v] = alt",
            "    prev[v] = u",
            "   end if",
            "  end for",
            " end while",
            "end procedure"
        };
    }
};

#endif // PSEUDOCODE_H
