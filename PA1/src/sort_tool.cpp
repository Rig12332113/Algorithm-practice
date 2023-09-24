// **************************************************************************
//  File       [sort_tool.cpp]
//  Author     [Yu-Hao Ho]
//  Synopsis   [The implementation of the SortTool Class]
//  Modify     [2020/9/15 Mu-Ting Wu]
// **************************************************************************

#include "sort_tool.h"
#include<iostream>


// Constructor
SortTool::SortTool() {}

// Insertsion sort method
void SortTool::InsertionSort(vector<int>& data) {
    // Function : Insertion sort
    // TODO : Please complete insertion sort code here
    for(int current = 1; current < data.size(); current++){
        int index = current - 1, temp = data[current];
        while(index >= 0 && data[index] > temp){
            data[index + 1] = data[index];
            index--;
        }
        data[index + 1] = temp;
    }
}

// Quick sort method
void SortTool::QuickSort(vector<int>& data){
    QuickSortSubVector(data, 0, data.size() - 1);
}
// Sort subvector (Quick sort)
void SortTool::QuickSortSubVector(vector<int>& data, int low, int high) {
    // Function : Quick sort subvector
    // TODO : Please complete QuickSortSubVector code here
    // Hint : recursively call itself
    //        Partition function is needed
    if (low < high){
        int middle = Partition(data, low, high);
        QuickSortSubVector(data, low, middle - 1);
        QuickSortSubVector(data, middle + 1, high);
    }

}

int SortTool::Partition(vector<int>& data, int low, int high) {
    // Function : Partition the vector 
    // TODO : Please complete the function
    // Hint : Textbook page 171
    int largepartstart = low;
    int pivot = data[(low + high) / 2];
    data.erase(data.begin() + (low + high) / 2);
    for(int i = low; i < high; i++){
        if (data[i] <= pivot){
            swap(data[i], data[largepartstart]);
            largepartstart++;
        }
    }
    data.insert(data.begin() + largepartstart, pivot);
    return largepartstart;
}

// Merge sort method
void SortTool::MergeSort(vector<int>& data){
    MergeSortSubVector(data, 0, data.size() - 1);
}

// Sort subvector (Merge sort)
void SortTool::MergeSortSubVector(vector<int>& data, int low, int high) {
    // Function : Merge sort subvector
    // TODO : Please complete MergeSortSubVector code here
    // Hint : recursively call itself
    //        Merge function is needed
    if (low == high)
        return;
    int middle = (low + high) / 2;
    MergeSortSubVector(data, low, middle);
    MergeSortSubVector(data, middle + 1, high);
    Merge(data, low, middle, middle + 1, high);
}

// Merge
void SortTool::Merge(vector<int>& data, int low, int middle1, int middle2, int high) {
    // Function : Merge two sorted subvector
    // TODO : Please complete the function
    vector<int> temp1;
    temp1.assign(data.begin() + low, data.begin() + middle1 + 1);
    vector<int> temp2;
    temp2.assign(data.begin() + middle2, data.begin() + high + 1);
    int index1 = 0, index2 = 0, size1 = temp1.size(), size2 = temp2.size();
    for(int i = 0; index1 < size1 || index2 < size2; i++){
        if (index1 >= size1){
            data[low + i] = temp2[index2];
            index2++;
        }
        else if (index2 >= size2){
            data[low + i] = temp1[index1];
            index1++;
        }
        else{
            int compare = temp1[index1] < temp2[index2] ? 1 : 0;
            data[low + i] = compare ? temp1[index1] : temp2[index2];
            if (compare)
                index1++;
            else
                index2++;
        }
    }
    temp1.resize(0);
    temp2.resize(0);
}

// Heap sort method
void SortTool::HeapSort(vector<int>& data) {
    // Build Max-Heap
    BuildMaxHeap(data);
    // 1. Swap data[0] which is max value and data[i] so that the max value will be in correct location
    // 2. Do max-heapify for data[0]
    int size = data.size();
    for (int i = data.size() - 1; i >= 1; i--) {
        swap(data[0],data[i]);
        size--;
        MaxHeapify(data, 0, size);
    }
}

//Max heapify
void SortTool::MaxHeapify(vector<int>& data, int root, int size) {
    // Function : Make tree with given root be a max-heap if both right and left sub-tree are max-heap
    // TODO : Please complete max-heapify code here
    int leftchild = 2 * root + 1, rightchild = 2 * root + 2;
    if (leftchild >= size && rightchild >= size){
        return;
    }
    else if (rightchild >= heapSize){
        if (data[leftchild] >= data[root])
            swap(data[leftchild], data[root]);
    }
    else{
        int biggerone = data[leftchild] > data[rightchild] ? leftchild : rightchild;
        if (data[root] < data[biggerone]){
            swap(data[root], data[biggerone]);
            MaxHeapify(data, biggerone, size);
        }
    }
}

//Build max heap
void SortTool::BuildMaxHeap(vector<int>& data) {
    heapSize = data.size(); // initialize heap size
    // Function : Make input data become a max-heap
    // TODO : Please complete BuildMaxHeap code here
    for(int i = heapSize - 1; i >= 0; i--)
        MaxHeapify(data, i, heapSize);
}
