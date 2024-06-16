#ifndef SORTINGALGO_H
#define SORTINGALGO_H

#include <vector>

// Forward declaration of Engine class to avoid circular dependency
class Engine;

class SortingAlgo
{
public:
    SortingAlgo() {}

    void bubbleSort(Engine* obj);
    void selectionSort(Engine* obj);
    void insertionSort(Engine* obj);
    void binaryInsertionSort(Engine* obj);
    void shellSort(Engine* obj);
    void quickSort(Engine* obj, std::vector<int>& arr, int low, int high);
    void mergeSort(Engine* obj, std::vector<int>& arr, int l, int r);
    void heapSort(Engine* obj);
    void countingSort(Engine* obj);
    void radixSort(Engine* obj);
    void bucketSort(Engine* obj);

private:
    void merge(Engine* obj, std::vector<int>& arr, int l, int m, int r);
    int  partition(Engine* obj, std::vector<int>& arr, int low, int high);
    void countingSortByDigit(Engine* obj, std::vector<int>& arr, int exp);
    void heapify(Engine* obj, std::vector<int>& arr, int n, int i);
};

#endif // SORTINGALGO_H