//==================== INCLUDES ====================
#include <iostream>
#include <chrono>
#include <vector>
#include <algorithm>

//==================== NAMESPCAES ====================
using namespace std;
using namespace std::chrono;

//==================== GLOBALS ====================
enum SORT_TYPES{STD_SORT       = 0,
                BUBBLE_SORT    = 1,
                MERGE_SORT     = 2,
                INSERTION_SORT = 3,
                SELECTION_SORT = 4,
                QUICK_SORT     = 5,
                BUCKET_SORT    = 6 
                };

//==================== FORWARD FUNCTION DECL ====================
void fillVector(int fillValue, vector<int> & vec);
void determineSort(SORT_TYPES name, vector<int> vec);
void printVector(vector<int> vec);

void bubbleSort(vector<int> vec);

void mergeSort(vector<int> &vec, int p, int r);
void merge(vector<int> &vec, int p, int q, int r);

void insertionSort(vector<int> vec);

void selectionSort(vector<int> vec);

void quickSort(vector<int> vec, int, int);
int partition(vector<int> vec, int, int);

//==================== MAIN ====================
int main(){
    for(int i = 100; i <= 1000; i += 100){
        int sortingSize = i;
        vector<int> unsortedVector;
        fillVector(sortingSize, unsortedVector);
        
        determineSort(STD_SORT, unsortedVector);
        determineSort(BUBBLE_SORT, unsortedVector);
        determineSort(MERGE_SORT, unsortedVector);
        determineSort(INSERTION_SORT, unsortedVector);
        determineSort(SELECTION_SORT, unsortedVector);
        determineSort(QUICK_SORT, unsortedVector);
        determineSort(BUCKET_SORT, unsortedVector);
    }

    return 0;
}

//==================== PRINT VECTOR ====================
void printVector(vector<int> vec){
    for(auto x : vec){
        cout << x << " ";
    }
    cout << endl;

    return;
}

//==================== DETERMINE SORT ====================
void determineSort(SORT_TYPES name, vector<int> vec){
    string sortName;
    auto start = high_resolution_clock::now();
    ///SEE BELOW FOR tempVecMerge/////
    vector<int> tempVecMerge = vec;
    /////////////////////////////////


    switch(name){
        case STD_SORT:
            sort(vec.begin(), vec.end());
            sortName = "Std Sort";
            break;
        case BUBBLE_SORT:
            sortName = "Bubble sort";
            bubbleSort(vec);
            break;
        case MERGE_SORT:
            sortName = "Merge sort";
            //since merge is recursive it needs the vec as a reference
            //have to make a copy of vec before hand
            mergeSort(tempVecMerge, 0, static_cast<int>(tempVecMerge.size() - 1));
            break;
        case INSERTION_SORT:
            sortName = "Insertion sort";
            insertionSort(vec);
            break;
        case SELECTION_SORT:
            sortName = "Selection sort";
            selectionSort(vec);
            break;
        case QUICK_SORT:
            sortName = "Quick sort";
            quickSort(vec, 0, static_cast<int>(vec.size() - 1));
            break;
        case BUCKET_SORT:
            sortName = "Bucket sort";
            break;
        default:
            break;
    }

    auto finish = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(finish - start);
    cout << sortName << "\t" << duration.count() << " nanoseconds" << "\t\t" << vec.size() << " items" << endl;

    return;
}

//==================== QUICK SORT ====================
int partition(vector<int> vec, int start, int end){
    int pivot = vec[start];
    int count = 0;

    for(int i = start + 1; i <= end; i++){
        if(vec[i] <= pivot){
            count++;
        }
    }

    int pivotIndex = start + count;
    swap(vec[pivotIndex], vec[start]);

    int i = start, j = end;

    while(i < pivotIndex && j > pivotIndex){
        while(vec[i] <= pivot){
            i++;
        }

        while(vec[j] > pivot){
            j--;
        }

        if(i < pivotIndex && j > pivotIndex){
            swap(vec[i++], vec[j--]);
        }
    }

    return pivotIndex;
}

void quickSort(vector<int> vec, int start, int end){
    vector<int> items = vec;
    if(start >= end){ return; }

    int p = partition(items, start, end);
    quickSort(items, start, p - 1);
    quickSort(items, p+2, end);

    return;
}

//==================== BUBBLE SORT ====================
void bubbleSort(vector<int> vec){
    long unsigned int step, i;

    for(step = 0; step < vec.size() - 1; ++step){
        int swapped = 0;
        for(i = 0; i < vec.size() - step - 1; ++i){
            if(vec[i] > vec[i + 1]){
                int temp = vec[i];
                vec[i] = vec[i + 1];
                vec[i + 1] = temp;

                swapped = 1;
            }
        }

        if(swapped == 0){
            break;
        }
    }

    return;
}


//==================== MERGE SORT ====================
void mergeSort(vector<int>& vec, int p, int r) {
    if (p < r) {
        int q = p + (r - p) / 2;

        mergeSort(vec, p, q);
        mergeSort(vec, q + 1, r);

        merge(vec, p, q, r);
    }
}

void merge(vector<int>& vec, int p, int q, int r) {
    int n1 = q - p + 1;
    int n2 = r - q;
    vector<int> L(n1);
    vector<int> R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = vec[p + i];
    for (int j = 0; j < n2; j++)
        R[j] = vec[q + 1 + j];

    int i = 0, 
        j = 0, 
        k = p;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            vec[k] = L[i];
            i++;
        } else {
            vec[k] = R[j];
            j++;
        }
        
        k++;
    }
}

//==================== INSERTION SORT ====================
void insertionSort(vector<int> vec){
    for (size_t i = 1; i < vec.size(); ++i) {
        int k = vec[i];
        size_t j = i - 1;
        while (j > 0 and vec[j] > k) {
            vec[j+1] = vec[j];
            j--;
        }
        vec[j+1] = k;
    }

    return;
}

//==================== SELECTION SORT ====================
void selectionSort(vector<int> vec){
    for (size_t i = 0; i < vec.size() - 2; ++i) {
        size_t minIndex = i;
        for (size_t j =i+1; j < vec.size() - 1; ++j) {
            if (vec[j] < vec[minIndex]) {
                minIndex = j;
            }
        }
        if (minIndex != i) {
            int temp = vec[i];
            vec[i] = vec[minIndex];
            vec[minIndex] = temp;
        } 
    }

    return;
}


//==================== FILL VECTOR ====================
void fillVector(int fillValue, vector<int> & vec){
    vec.erase(vec.begin(), vec.end());

    for(int i = 0; i < fillValue; i++){
        int item = rand() % 1000;
        vec.push_back(item);
    }
    return;
}