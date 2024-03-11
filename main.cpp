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
            break;
        case INSERTION_SORT:
            sortName = "Insertion sort";
            break;
        case SELECTION_SORT:
            sortName = "Selection sort";
            break;
        case QUICK_SORT:
            sortName = "Quick sort";
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

//==================== FILL VECTOR ====================
void fillVector(int fillValue, vector<int> & vec){
    vec.erase(vec.begin(), vec.end());

    for(int i = 0; i < fillValue; i++){
        int item = rand() % 1000;
        vec.push_back(item);
    }
    return;
}