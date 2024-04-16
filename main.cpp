//==================================================
//==================== INCLUDES ====================
//==================================================

#include <iostream>
#include <iomanip>
#include <chrono>
#include <vector>
#include <algorithm>

//==================================================
//==================== NAMESPACES ==================
//==================================================

using namespace std;
using namespace std::chrono;

//==================================================
//==================== GLOBALS =====================
//==================================================

enum SORT_TYPES{STD_SORT       = 0,
                BUBBLE_SORT    = 1,
                MERGE_SORT     = 2,
                INSERTION_SORT = 3,
                SELECTION_SORT = 4,
                QUICK_SORT     = 5,
                BUCKET_SORT    = 6 
                };

//==================================================

// true to find the one minute time for a sort
// loop start/increment needs manually tweaked to find it in a reasonable time
bool PRINT_MINUTE = false; // 📑

// true to calculate an average sort time
bool PRINT_AVERAGES = true; // 📑

// how many vectors to average
const int NUM_VECTORS = 10;

// the size of the vectors to average
const int SIZES[] = { 32, 64, 128, 256, 512, 1024 };

// the sort types to use in averaging; note std sort is not needed here
const SORT_TYPES ENUMS[] = { BUBBLE_SORT, MERGE_SORT, 
                             INSERTION_SORT, SELECTION_SORT, 
                             QUICK_SORT, BUCKET_SORT };

const string LINE = "------------------------------------------------------------";

//==================================================
//============= FUNCTION DECLARATIONS ==============
//==================================================

void fillVector(int fillValue, vector<int> & vec);
void determineSort(SORT_TYPES name, vector<int> vec);
void printVector(vector<int> vec);

long int averageSort(SORT_TYPES name, vector<int> vec);
bool minuteSort(SORT_TYPES name, vector<int> vec);

void bubbleSort(vector<int> vec);

void mergeSort(vector<int> &vec, int p, int r);
void merge(vector<int> &vec, int p, int q, int r);

void insertionSort(vector<int> vec);

void selectionSort(vector<int> vec);

void bucketSort(vector<int> vec);

void quickSort(vector<int> vec, int, int);
int partition(vector<int> vec, int, int);

//==================================================
//==================== MAIN ========================
//==================================================

int main() {

    if (PRINT_MINUTE) {
        bool sortComplete = false;        
        //======================================================
        // These need manually adjusted to cut down time/memory
        //======================================================
        /* Since each iteration takes ~1 minute,
           I'm starting with a large increment then refining
           the initial value and reducing the increment 
           until I can get a result very close to 60s.
        */
        int initial   = 750'000'000;
        int increment = 10'000'000;

        for (int j = initial; not sortComplete; j+=increment) {
            cout << "j = " << j << endl;
            vector<int> unsortedVector;
            // cout << "max size: " << unsortedVector.max_size() << endl; // 2.3Q elements
            fillVector(j, unsortedVector);
            //===========================================================
            // Ranked approximately slowest to fastest
            //===========================================================
            // sortComplete = minuteSort(BUBBLE_SORT,    unsortedVector);
            // sortComplete = minuteSort(QUICK_SORT,     unsortedVector);
            // sortComplete = minuteSort(SELECTION_SORT, unsortedVector);
            // sortComplete = minuteSort(INSERTION_SORT, unsortedVector);
            // sortComplete = minuteSort(MERGE_SORT,     unsortedVector);
            // sortComplete = minuteSort(BUCKET_SORT,    unsortedVector);
            sortComplete = minuteSort(STD_SORT,       unsortedVector);

            if (not sortComplete) {
                unsortedVector.clear();
            }

        }
    }
    
    //==================================================
    
    if (PRINT_AVERAGES) {
        cout << LINE << endl;
        cout << "Finding the mean of " << NUM_VECTORS << " vectors..." << endl;
        cout << LINE << endl;
        // for each sort
        for (int e = 0; e < 6; e++) {
            // for each size
            for (int s = 0; s < 6; s++) {
                vector<long int> stdTimes;
                vector<long int> sortTimes;

                // average N>=10 vectors and compare to std sort
                for (int i = 0; i < NUM_VECTORS; i++) {
                    vector<int> unsortedVector;
                    fillVector(SIZES[s], unsortedVector);
                    stdTimes.push_back(averageSort(STD_SORT, unsortedVector));
                    sortTimes.push_back(averageSort(ENUMS[e], unsortedVector));
                }
                // std sort results
                long int stdSum = 0;
                for (long int j: stdTimes) {
                    stdSum += j;
                }
                cout << setw(14) << right << "Std Sort" << ":\t";
                cout << "mean = " << setw(8) << right << stdSum/stdTimes.size() << " nanoseconds";
                cout << "\titems = " << setw(4) << left << SIZES[s] << endl;

                // test sort results
                string sortName = "";
                switch (ENUMS[e]) {
                    case BUBBLE_SORT:
                        sortName = "Bubble Sort"; break;
                    case MERGE_SORT:
                        sortName = "Merge Sort"; break;
                    case INSERTION_SORT:
                        sortName = "Insertion Sort"; break;
                    case SELECTION_SORT:
                        sortName = "Selection Sort"; break;
                    case QUICK_SORT:
                        sortName = "Quick Sort"; break;
                    case BUCKET_SORT:
                        sortName = "Bucket Sort"; break;
                    case STD_SORT:
                    default:
                        sortName = "Std Sort"; break;
                }
                long int sortSum = 0;
                for (long int k: sortTimes) {
                    sortSum += k;
                }
                cout << setw(14) << right << sortName << ":\t";
                cout << "mean = " << setw(8) << right << sortSum/sortTimes.size() << " nanoseconds";
                cout << "\titems = " << setw(4) << left << SIZES[s] << endl;
            }

        }

    }
    
    cout << LINE << endl;
    //==================================================
    
    for (int i = 100; i <= 1000; i += 100) {
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

    cout << LINE << endl;
    return 0;
}

//==================================================
//================ PRINT VECTOR ====================
//==================================================

void printVector(vector<int> vec){
    for(auto x : vec){
        cout << x << " ";
    }
    cout << endl;

    return;
}

//==================================================
//================ AVERAGE SORTS ===================
//==================================================

long int averageSort(SORT_TYPES name, vector<int> vec) {
    string sortName = "";
    auto start = high_resolution_clock::now();
    vector<int> tempVecMerge = vec;

    switch(name){
        case STD_SORT:
            sort(vec.begin(), vec.end());
            sortName = "Std Sort"; break;
        case BUBBLE_SORT:
            sortName = "Bubble Sort";
            bubbleSort(vec); break;
        case MERGE_SORT:
            sortName = "Merge Sort";
            mergeSort(tempVecMerge, 0, static_cast<int>(tempVecMerge.size() - 1)); break;
        case INSERTION_SORT:
            sortName = "Insertion Sort";
            insertionSort(vec); break;
        case SELECTION_SORT:
            sortName = "Selection Sort";
            selectionSort(vec); break;
        case QUICK_SORT:
            sortName = "Quick Sort";
            quickSort(vec, 0, static_cast<int>(vec.size() - 1)); break;
        case BUCKET_SORT:
            sortName = "Bucket Sort";
            bucketSort(vec); break;
        default: break;
    }

    auto finish   = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(finish - start);

    return duration.count();
}

//==================================================
//=============== 1 MINUTE SORT ====================
//==================================================

bool minuteSort(SORT_TYPES name, vector<int> vec) {
    string sortName = "";
    auto start = high_resolution_clock::now();
    vector<int> tempVecMerge = vec;

    switch(name){
        case STD_SORT:
            sort(vec.begin(), vec.end());
            sortName = "Std Sort"; break;
        case BUBBLE_SORT:
            sortName = "Bubble sort";
            bubbleSort(vec); break;
        case MERGE_SORT:
            sortName = "Merge sort";
            mergeSort(tempVecMerge, 0, static_cast<int>(tempVecMerge.size() - 1)); break;
        case INSERTION_SORT:
            sortName = "Insertion sort";
            insertionSort(vec); break;
        case SELECTION_SORT:
            sortName = "Selection sort";
            selectionSort(vec); break;
        case QUICK_SORT:
            sortName = "Quick sort";
            quickSort(vec, 0, static_cast<int>(vec.size() - 1)); break;
        case BUCKET_SORT:
            sortName = "Bucket sort";
            bucketSort(vec); break;
        default:
            break;
    }

    auto finish   = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(finish - start);
    auto minute   = seconds(60);

    cout << setw(14) << right << sortName << ":\t";
    cout << setw(8) << right << duration.count() << " seconds" << "\t";
    cout << setw(4) << right << vec.size() << " items" << endl;

    if (duration.count() >= minute.count()) {
        return true;
    }
    return false;
}

//==================================================
//================ DETERMINE SORT ==================
//==================================================

void determineSort(SORT_TYPES name, vector<int> vec){
    string sortName = "";
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
            bucketSort(vec);
            break;
        default:
            break;
    }

    auto finish = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(finish - start);
    cout << setw(14) << right << sortName << ":\t";
    cout << setw(8) << right << duration.count() << " nanoseconds" << "\t";
    cout << setw(4) << right << vec.size() << " items" << endl;

    return;
}

//==================================================
//================== QUICK SORT ====================
//==================================================

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

//==================================================

void quickSort(vector<int> vec, int start, int end){
    vector<int> items = vec;
    if(start >= end){ return; }

    int p = partition(items, start, end);
    quickSort(items, start, p - 1);
    quickSort(items, p+2, end);

    return;
}

//==================================================
//================= BUBBLE SORT ====================
//==================================================

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

//==================================================
//================== MERGE SORT ====================
//==================================================

void mergeSort(vector<int>& vec, int p, int r) {
    if (p < r) {
        int q = p + (r - p) / 2;

        mergeSort(vec, p, q);
        mergeSort(vec, q + 1, r);

        merge(vec, p, q, r);
    }
}

//==================================================

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
    while (i < n1 and j < n2) {
        if (L[i] <= R[j]) {
            vec[k] = L[i];
            i++;
        } else {
            vec[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        vec[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        vec[k] = R[j];
        j++;
        k++;
    }
}

//==================================================
//================= INSERTION SORT =================
//==================================================

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

//==================================================
//============== SELECTION SORT ====================
//==================================================

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

//==================================================
//================= BUCKET SORT ====================
//==================================================

void bucketSort(vector<int> vec) {
    if (vec.empty()){
        return;
    }

    int max = *max_element(vec.begin(), vec.end());

    vector<vector<int>> buckets(max + 1);

    for (size_t i = 0; i < vec.size(); ++i) {
        buckets[vec[i]].push_back(vec[i]);
    }

    int index = 0;
    for (size_t i = 0; i < buckets.size(); ++i) {
        sort(buckets[i].begin(), buckets[i].end());
        for (size_t j = 0; j < buckets[i].size(); ++j) {
            vec[index++] = buckets[i][j];
        }
    }
}

//==================================================
//================= FILL VECTOR ====================
//==================================================

void fillVector(int fillValue, vector<int> & vec){
    vec.erase(vec.begin(), vec.end());

    for(int i = 0; i < fillValue; i++){
        int item = rand() % 1000;
        vec.push_back(item);
    }
    return;
}

//==================================================