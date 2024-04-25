#include <iostream>
#include <omp.h>
#include <vector>
#include <chrono>
#include <algorithm>
using namespace std;

// Function to merge two halves
void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    vector<int> leftarr(n1) , rightarr(n2);
    
    // generate left array from left -> n1 
    for(int i=0;i<n1;i++){
        leftarr[i] = arr[left+i];
    }
    
 // generate right array from mid+1 -> n2
    for(int i=0;i<n1;i++){
        rightarr[i] = arr[mid+1+i];
    }
    
    int i=0 , j=0 , k=left;
    
    while(i<n1 && j<n2){
        if(leftarr[i] < rightarr[j]){
            arr[k]=leftarr[i];
            i++;
        }else{
            arr[k]=rightarr[j];
            j++;
        }
        k++;
    }
    
    while(i<n1){
        arr[k]=leftarr[i];
        i++;
        k++;
    }
    while(j<n2){
        arr[k]=rightarr[j];
        j++;
        k++;
    }
}

void mergeSort(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        // Recursive calls for each half
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        // Merge the sorted halves
        merge(arr, left, mid, right);
    }
}


// Parallel Merge Sort
void parallelMergeSort(vector<int>& arr, int left, int right) {
    if (left < right) {
        
        int mid = left + (right - left) / 2;
        
            #pragma omp parallel sections
            {
                #pragma omp section
                parallelMergeSort(arr, left, mid);

                #pragma omp section
                parallelMergeSort(arr, mid + 1, right);
            }

        merge(arr, left, mid, right);
    }
}


int main ()
{
  int n = 40;
  vector < int >arr (n);
  
  for(int i=0; i<n; i++){
      arr[i] = rand() % 100;
  }
  
  
    auto start = chrono::high_resolution_clock::now();
    mergeSort(arr,0,n-1);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    cout<<duration.count()<<"seconds"<<endl;

  
    auto nstart = chrono::high_resolution_clock::now();
    parallelMergeSort(arr,0,n-1);
    auto nend = chrono::high_resolution_clock::now();
    chrono::duration<double> nduration = nend - nstart;
    cout<<nduration.count()<<"seconds"<<endl;
    
}
