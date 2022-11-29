#include <iostream>
#include <time.h>
#include <pthread.h>
using namespace std;

#define MAX 20
#define THREAD_MAX 4

int a[MAX];
int part = 0;

void merge(int array[], int const left, int const mid, int const right)
{
    auto const subArrayOne = mid - left + 1;
    auto const subArrayTwo = right - mid;
 
    auto *leftArray = new int[subArrayOne], *rightArray = new int[subArrayTwo];
 
    for (auto i = 0; i < subArrayOne; i++)
        leftArray[i] = array[left + i];
    for (auto j = 0; j < subArrayTwo; j++)
        rightArray[j] = array[mid + 1 + j];
 
    auto indexOfSubArrayOne = 0, indexOfSubArrayTwo = 0;
    int indexOfMergedArray = left;
 
    while (indexOfSubArrayOne < subArrayOne && indexOfSubArrayTwo < subArrayTwo) {
        if (leftArray[indexOfSubArrayOne]
            <= rightArray[indexOfSubArrayTwo]) {
            array[indexOfMergedArray]
                = leftArray[indexOfSubArrayOne];
            indexOfSubArrayOne++;
        }
        else {
            array[indexOfMergedArray]
                = rightArray[indexOfSubArrayTwo];
            indexOfSubArrayTwo++;
        }
        indexOfMergedArray++;
    }
    
    while (indexOfSubArrayOne < subArrayOne) {
        array[indexOfMergedArray]
            = leftArray[indexOfSubArrayOne];
        indexOfSubArrayOne++;
        indexOfMergedArray++;
    }
    
    while (indexOfSubArrayTwo < subArrayTwo) {
        array[indexOfMergedArray]
            = rightArray[indexOfSubArrayTwo];
        indexOfSubArrayTwo++;
        indexOfMergedArray++;
    }
    delete[] leftArray;
    delete[] rightArray;
}

void mergeSort(int array[], int const begin, int const end)
{
    if (begin >= end)
        return;
 
    auto mid = begin + (end - begin) / 2;
    mergeSort(array, begin, mid);
    mergeSort(array, mid + 1, end);
    merge(array, begin, mid, end);
}

void printArray(int A[], int size)
{
    for (auto i = 0; i < size; i++)
        cout << A[i] << " ";
}

// Merge Sort using multithreading
void mergeForThread(int low, int mid, int high)
{
    int* left = new int[mid - low + 1];
    int* right = new int[high - mid];
 
    // n1 is size of left part and n2 is size
    // of right part
    int n1 = mid - low + 1, n2 = high - mid, i, j;
 
    // storing values in left part
    for (i = 0; i < n1; i++)
        left[i] = a[i + low];
 
    // storing values in right part
    for (i = 0; i < n2; i++)
        right[i] = a[i + mid + 1];
 
    int k = low;
    i = j = 0;
 
    // merge left and right in ascending order
    while (i < n1 && j < n2) {
        if (left[i] <= right[j])
            a[k++] = left[i++];
        else
            a[k++] = right[j++];
    }
 
    // insert remaining values from left
    while (i < n1) {
        a[k++] = left[i++];
    }
 
    // insert remaining values from right
    while (j < n2) {
        a[k++] = right[j++];
    }
}

void merge_sort(int low, int high)
{
    int mid = low + (high - low) / 2;
    if (low < high) {
        merge_sort(low, mid);
        merge_sort(mid + 1, high);
        mergeForThread(low, mid, high);
    }
}

// thread function for multi-threading
void* merge_sort(void* arg)
{
    // which part out of 4 parts
    int thread_part = part++;
 
    // calculating low and high
    int low = thread_part * (MAX / 4);
    int high = (thread_part + 1) * (MAX / 4) - 1;
 
    // evaluating mid point
    int mid = low + (high - low) / 2;
    if (low < high) {
        merge_sort(low, mid);
        merge_sort(mid + 1, high);
        mergeForThread(low, mid, high);
    }
}


int main()
{
    int arr[] = { 12, 11, 13, 5, 6, 7 };
    clock_t t1, t2;

    t1 = clock();
    auto arr_size = sizeof(arr) / sizeof(arr[0]);
 
    cout << "Given array is \n";
    printArray(arr, arr_size);
 
    mergeSort(arr, 0, arr_size - 1);
 
    t2 = clock();
    cout << "\nSorted array is \n";
    printArray(arr, arr_size);
    cout << "Time taken without multithreading: " << (t2 - t1) / (double)CLOCKS_PER_SEC << endl;


    // Multithreading
    for (int i = 0; i < MAX; i++)
        a[i] = rand() % 100;
 
    clock_t t3, t4;
 
    t3 = clock();
    pthread_t threads[THREAD_MAX];
 
    // creating 4 threads
    for (int i = 0; i < THREAD_MAX; i++)
        pthread_create(&threads[i], NULL, merge_sort,
                                        (void*)NULL);
 
    // joining all 4 threads
    for (int i = 0; i < 4; i++)
        pthread_join(threads[i], NULL);
 
    // merging the final 4 parts
    merge(0, (MAX / 2 - 1) / 2, MAX / 2 - 1);
    merge(MAX / 2, MAX/2 + (MAX-1-MAX/2)/2, MAX - 1);
    merge(0, (MAX - 1)/2, MAX - 1);
 
    t4 = clock();
 
    // displaying sorted array
    cout << "Sorted array: ";
    for (int i = 0; i < MAX; i++)
        cout << a[i] << " ";
 
    // time taken by merge sort in seconds
    cout << "Time taken with multithreading: " << (t4 - t3) / (double)CLOCKS_PER_SEC << endl;


    return 0;
}