#include <numeric>
#include <array>
#include <utility>
#include <algorithm>

#include "SortingAlgo.h"
#include "Engine.h"

void SortingAlgo::bubbleSort(Engine* obj)
{
    for (int i = 0; i < obj->m_numbers.size(); i++)
    {
        for (int j = 0; j < obj->m_numbers.size() - i - 1; j++)
        {
            obj->handleEvents();
            if (!obj->m_running)
            {
                return;
            }

            obj->m_leftIndex = j;
            obj->m_rightIndex = j + 1;

            if (obj->m_numbers[j] > obj->m_numbers[j + 1])
            {
                std::swap(obj->m_numbers[j], obj->m_numbers[j + 1]);
            }
            {
                std::lock_guard<std::mutex> lokc(obj->mtx);
                obj->updateAvailable = true;
            }
            obj->cv.notify_all();
            {
                std::unique_lock<std::mutex> lock(obj->mtx);
                obj->cv.wait(lock, [&] {return !obj->updateAvailable; });
            }
        }
    }

    obj->resetColorIdx();
    obj->Draw();
    obj->m_sorted = true;
}

void SortingAlgo::selectionSort(Engine* obj)
{
    for (int i = 0; i < obj->m_numbers.size() - 1; i++)
    {
        int minIdx = i;
        for (int j = i + 1; j < obj->m_numbers.size(); j++)
        {
            obj->handleEvents();
            if (!obj->m_running)
            {
                return;
            }
            if (obj->m_numbers[j] < obj->m_numbers[minIdx])
            {
                minIdx = j;
            }
        }
        obj->m_leftIndex = i;
        obj->m_rightIndex = minIdx;
        std::swap(obj->m_numbers[i], obj->m_numbers[minIdx]);
        
        {
            std::lock_guard<std::mutex> lokc(obj->mtx);
            obj->updateAvailable = true;
        }
        obj->cv.notify_all();
        {
            std::unique_lock<std::mutex> lock(obj->mtx);
            obj->cv.wait(lock, [&] {return !obj->updateAvailable; });
        }
    }

    obj->resetColorIdx();
    obj->Draw();
    obj->m_sorted = true;
}

void SortingAlgo::insertionSort(Engine* obj)
{
    for (int i = 1; i < obj->m_numbers.size(); i++)
    {
        obj->m_pivotIndex = i;
        int key = obj->m_numbers[i];
        int j = i - 1;
        while (j >= 0 && obj->m_numbers[j] > key)
        {
            obj->handleEvents();
            if (!obj->m_running)
            {
                return;
            }
            obj->m_numbers[j + 1] = obj->m_numbers[j];
            obj->m_leftIndex = j;
            obj->m_rightIndex = j + 1;
            
            {
                std::lock_guard<std::mutex> lokc(obj->mtx);
                obj->updateAvailable = true;
            }
            obj->cv.notify_all();
            {
                std::unique_lock<std::mutex> lock(obj->mtx);
                obj->cv.wait(lock, [&] {return !obj->updateAvailable; });
            }

            j--;
        }
        obj->m_numbers[j + 1] = key;
        
        {
            std::lock_guard<std::mutex> lokc(obj->mtx);
            obj->updateAvailable = true;
        }
        obj->cv.notify_all();
        {
            std::unique_lock<std::mutex> lock(obj->mtx);
            obj->cv.wait(lock, [&] {return !obj->updateAvailable; });
        }
    }

    obj->resetColorIdx();
    obj->Draw();
    obj->m_sorted = true;
}

void SortingAlgo::binaryInsertionSort(Engine* obj)
{
    for (int i = 1; i < obj->m_numbers.size(); i++)
    {
        int key = obj->m_numbers[i];
        int left = 0;
        int right = i - 1;

        // Binary search to find the correct position to insert the key
        while (left <= right)
        {
            int mid = left + (right - left) / 2;
            if (obj->m_numbers[mid] <= key)
                left = mid + 1;
            else
                right = mid - 1;
        }

        // Shift elements to make space for the key
        for (int j = i - 1; j >= left; j--)
        {
            obj->handleEvents();
            if (!obj->m_running)
            {
                return;
            }
            obj->m_numbers[j + 1] = obj->m_numbers[j];
            obj->m_leftIndex = j;
            obj->m_rightIndex = j + 1;
            
            {
                std::lock_guard<std::mutex> lokc(obj->mtx);
                obj->updateAvailable = true;
            }
            obj->cv.notify_all();
            {
                std::unique_lock<std::mutex> lock(obj->mtx);
                obj->cv.wait(lock, [&] {return !obj->updateAvailable; });
            }
        }
        obj->m_numbers[left] = key;
        obj->m_pivotIndex = left;
        
        {
            std::lock_guard<std::mutex> lokc(obj->mtx);
            obj->updateAvailable = true;
        }
        obj->cv.notify_all();
        {
            std::unique_lock<std::mutex> lock(obj->mtx);
            obj->cv.wait(lock, [&] {return !obj->updateAvailable; });
        }
    }

    obj->resetColorIdx();
    obj->Draw();
    obj->m_sorted = true;
}

void SortingAlgo::shellSort(Engine* obj)
{
    int n = obj->m_numbers.size();

    // Start with a large gap and reduce the gap
    for (int gap = n / 2; gap > 0; gap /= 2)
    {
        // Perform a gapped insertion sort for this gap size
        for (int i = gap; i < n; i++)
        {
            int temp = obj->m_numbers[i];
            int j;
            for (j = i; j >= gap && obj->m_numbers[j - gap] > temp; j -= gap)
            {
                obj->handleEvents();
                if (!obj->m_running)
                {
                    return;
                }
                obj->m_numbers[j] = obj->m_numbers[j - gap];
                obj->m_leftIndex = j;
                obj->m_rightIndex = j - gap;
                
                {
                    std::lock_guard<std::mutex> lokc(obj->mtx);
                    obj->updateAvailable = true;
                }
                obj->cv.notify_all();
                {
                    std::unique_lock<std::mutex> lock(obj->mtx);
                    obj->cv.wait(lock, [&] {return !obj->updateAvailable; });
                }
            }
            obj->m_numbers[j] = temp;
            obj->m_pivotIndex = j;
            
            {
                std::lock_guard<std::mutex> lokc(obj->mtx);
                obj->updateAvailable = true;
            }
            obj->cv.notify_all();
            {
                std::unique_lock<std::mutex> lock(obj->mtx);
                obj->cv.wait(lock, [&] {return !obj->updateAvailable; });
            }
        }
    }

    obj->resetColorIdx();
    obj->Draw();
    obj->m_sorted = true;
}

void SortingAlgo::heapify(Engine* obj, std::vector<int>& arr, int n, int i)
{
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;

    if (l < n && arr[l] > arr[largest])
    {
        largest = l;
    }
    if (r < n && arr[r] > arr[largest])
    {
        largest = r;
    }

    if (largest != i)
    {
        obj->m_leftIndex = i;
        obj->m_rightIndex = largest;
        std::swap(arr[i], arr[largest]);
        {
            std::lock_guard<std::mutex> lokc(obj->mtx);
            obj->updateAvailable = true;
        }
        obj->cv.notify_all();
        {
            std::unique_lock<std::mutex> lock(obj->mtx);
            obj->cv.wait(lock, [&] {return !obj->updateAvailable; });
        }
        heapify(obj, arr, n, largest);
    }
}

void SortingAlgo::heapSort(Engine* obj)
{
    int n = obj->m_numbers.size();

    for (int i = n / 2 - 1; i >= 0; i--)
    {
        obj->handleEvents();
        if (!obj->m_running)
        {
            return;
        }
        heapify(obj, obj->m_numbers, n, i);
    }

    for (int i = n - 1; i > 0; i--)
    {
        obj->handleEvents();
        if (!obj->m_running)
        {
            return;
        }
        obj->m_leftIndex = 0;
        obj->m_rightIndex = i;
        std::swap(obj->m_numbers[0], obj->m_numbers[i]);
        {
            std::lock_guard<std::mutex> lokc(obj->mtx);
            obj->updateAvailable = true;
        }
        obj->cv.notify_all();
        {
            std::unique_lock<std::mutex> lock(obj->mtx);
            obj->cv.wait(lock, [&] {return !obj->updateAvailable; });
        }
        heapify(obj, obj->m_numbers, i, 0);
    }

    obj->resetColorIdx();
    obj->Draw();
    obj->m_sorted = true;
}

int SortingAlgo::partition(Engine* obj, std::vector<int>& arr, int low, int high)
{
    int pivot = arr[high];
    int i = low - 1;

    obj->m_rightIndex = high;

    for (int j = low; j < high; j++)
    {
        if (arr[j] < pivot)
        {
            i++;
            std::swap(arr[i], arr[j]);
            obj->m_leftIndex = i;
            obj->m_pivotIndex = j;
            {
                std::lock_guard<std::mutex> lokc(obj->mtx);
                obj->updateAvailable = true;
            }
            obj->cv.notify_all();
            {
                std::unique_lock<std::mutex> lock(obj->mtx);
                obj->cv.wait(lock, [&] {return !obj->updateAvailable; });
            }
        }
    }
    obj->m_leftIndex = i + 1;
    std::swap(arr[i + 1], arr[high]);
    {
        std::lock_guard<std::mutex> lokc(obj->mtx);
        obj->updateAvailable = true;
    }
    obj->cv.notify_all();
    {
        std::unique_lock<std::mutex> lock(obj->mtx);
        obj->cv.wait(lock, [&] {return !obj->updateAvailable; });
    }
    return i + 1;
}

void SortingAlgo::quickSort(Engine* obj, std::vector<int>& arr, int low, int high)
{
    if (low < high)
    {
        obj->handleEvents();
        if (!obj->m_running)
        {
            return;
        }
        int pivot = partition(obj, arr, low, high);
        quickSort(obj, arr, low, pivot - 1);
        quickSort(obj, arr, pivot + 1, high);
    }

    obj->resetColorIdx();
    obj->Draw();
    obj->m_sorted = true;
}

void SortingAlgo::merge(Engine* obj, std::vector<int>& arr, int l, int m, int r)
{
    int n1 = m - l + 1;
    int n2 = r - m;

    std::vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; ++i)
    {
        L[i] = arr[l + i];
    }
    for (int j = 0; j < n2; ++j)
    {
        R[j] = arr[m + 1 + j];
    }

    int i = 0, j = 0, k = l;
    obj->m_pivotIndex = m;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j])
        {
            obj->m_leftIndex = k;
            obj->m_rightIndex = i;
            arr[k++] = L[i++];
        }
        else
        {
            obj->m_leftIndex = k;
            obj->m_rightIndex = j;
            arr[k++] = R[j++];
        }
        {
            std::lock_guard<std::mutex> lokc(obj->mtx);
            obj->updateAvailable = true;
        }
        obj->cv.notify_all();
        {
            std::unique_lock<std::mutex> lock(obj->mtx);
            obj->cv.wait(lock, [&] {return !obj->updateAvailable; });
        }
    }

    while (i < n1)
    {
        obj->m_leftIndex = k;
        obj->m_rightIndex = i;
        arr[k++] = L[i++];
        {
            std::lock_guard<std::mutex> lokc(obj->mtx);
            obj->updateAvailable = true;
        }
        obj->cv.notify_all();
        {
            std::unique_lock<std::mutex> lock(obj->mtx);
            obj->cv.wait(lock, [&] {return !obj->updateAvailable; });
        }
    }
    while (j < n2)
    {
        obj->m_leftIndex = k;
        obj->m_rightIndex = j;
        arr[k++] = R[j++];
        {
            std::lock_guard<std::mutex> lokc(obj->mtx);
            obj->updateAvailable = true;
        }
        obj->cv.notify_all();
        {
            std::unique_lock<std::mutex> lock(obj->mtx);
            obj->cv.wait(lock, [&] {return !obj->updateAvailable; });
        }
    }
}

void SortingAlgo::mergeSort(Engine* obj, std::vector<int>& arr, int l, int r)
{
    if (l < r)
    {
        obj->handleEvents();
        if (!obj->m_running)
        {
            return;
        }
        int m = l + (r - l) / 2;
        mergeSort(obj, arr, l, m);
        mergeSort(obj, arr, m + 1, r);
        merge(obj, arr, l, m, r);
    }

    obj->resetColorIdx();
    obj->Draw();
    obj->m_sorted = true;
}

void SortingAlgo::countingSort(Engine* obj)
{
    /*
      In Counting Sort, the sorting primarily happens during the phase where elements are placed back into the
      original array (arr) from the output array based on their counts.
      Counting Sort is efficient for sorting integers when the range of input data is not significantly larger
      than the number of elements.
      It counts the number of occurrences of each value and uses this information to place elements in sorted order.
    */
    int maxVal = *max_element(obj->m_numbers.begin(), obj->m_numbers.end());
    int minVal = *min_element(obj->m_numbers.begin(), obj->m_numbers.end());
    int range = maxVal - minVal + 1;

    std::vector<int> count(range), output(obj->m_numbers.size());
    for (int i = 0; i < obj->m_numbers.size(); ++i)
    {
        ++count[obj->m_numbers[i] - minVal];
    }

    for (int i = 1; i < range; ++i)
    {
        count[i] += count[i - 1];
    }

    for (int i = obj->m_numbers.size() - 1; i >= 0; --i)
    {
        output[count[obj->m_numbers[i] - minVal] - 1] = obj->m_numbers[i];
        --count[obj->m_numbers[i] - minVal];
    }

    for (int i = 0; i < obj->m_numbers.size(); ++i)
    {
        obj->m_leftIndex = i;
        obj->m_numbers[i] = output[i];
        {
            std::lock_guard<std::mutex> lokc(obj->mtx);
            obj->updateAvailable = true;
        }
        obj->cv.notify_all();
        {
            std::unique_lock<std::mutex> lock(obj->mtx);
            obj->cv.wait(lock, [&] {return !obj->updateAvailable; });
        }
    }

    obj->resetColorIdx();
    obj->Draw();
    obj->m_sorted = true;
}

void SortingAlgo::countingSortByDigit(Engine* obj, std::vector<int>& arr, int exp)
{
    int n = arr.size();
    std::vector<int> output(n);
    std::array<int, 10> count = { 0 };

    for (int i = 0; i < n; ++i)
    {
        ++count[(arr[i] / exp) % 10];
    }

    for (int i = 1; i < 10; ++i)
    {
        count[i] += count[i - 1];
    }

    for (int i = n - 1; i >= 0; --i)
    {
        output[count[(arr[i] / exp) % 10] - 1] = arr[i];
        --count[(arr[i] / exp) % 10];
    }

    for (int i = 0; i < n; ++i)
    {
        obj->m_rightIndex = i;
        arr[i] = output[i];
        {
            std::lock_guard<std::mutex> lokc(obj->mtx);
            obj->updateAvailable = true;
        }
        obj->cv.notify_all();
        {
            std::unique_lock<std::mutex> lock(obj->mtx);
            obj->cv.wait(lock, [&] {return !obj->updateAvailable; });
        }
    }
}

void SortingAlgo::radixSort(Engine* obj)
{
    int maxVal = *max_element(obj->m_numbers.begin(), obj->m_numbers.end());

    for (int exp = 1; maxVal / exp > 0; exp *= 10)
    {
        countingSortByDigit(obj, obj->m_numbers, exp);
    }

    obj->resetColorIdx();
    obj->Draw();
    obj->m_sorted = true;
}

void SortingAlgo::bucketSort(Engine* obj)
{
    int n = obj->m_numbers.size();
    std::vector<std::vector<int>> buckets(n);

    // Place each element in its bucket
    for (int i = 0; i < n; ++i)
    {
        int bucketIndex = n * obj->m_numbers[i] / (INT_MAX + 1LL);
        buckets[bucketIndex].push_back(obj->m_numbers[i]);
    }

    // Sort individual buckets
    for (int i = 0; i < n; ++i)
    {
        std::sort(buckets[i].begin(), buckets[i].end());
    }

    // Concatenate sorted buckets into arr
    int idx = 0;
    for (int i = 0; i < n; ++i)
    {
        obj->m_leftIndex = i;
        for (int j = 0; j < buckets[i].size(); ++j)
        {
            obj->m_rightIndex = j;
            obj->m_pivotIndex = idx;
            obj->m_numbers[idx++] = buckets[i][j];
            {
                std::lock_guard<std::mutex> lokc(obj->mtx);
                obj->updateAvailable = true;
            }
            obj->cv.notify_all();
            {
                std::unique_lock<std::mutex> lock(obj->mtx);
                obj->cv.wait(lock, [&] {return !obj->updateAvailable; });
            }
        }
    }

    obj->resetColorIdx();
    obj->Draw();
    obj->m_sorted = true;
}