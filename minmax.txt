#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric> // for std::accumulate
#include <random> // for random generation
#include <omp.h>
#include <chrono>
using namespace std;

void sequential(vector<int> arr){
    auto start = chrono::high_resolution_clock::now();
    int min = arr[0];
    // find min
    for(auto ele:arr){
        if(min > ele){
            min=ele;
        }
    }
    
    int max = arr[0];
    // find max
    for(auto ele:arr){
        if(max < ele){
            max=ele;
        }
    }
    
    long long sum=0;
    // find sum
    for(auto ele:arr){
        sum += ele;
    }
    
    int avg = sum / arr.size();
    
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    cout<<"Time required is - "<<duration.count()<<endl;
    cout<<"Min - "<<min<<endl;
    cout<<"max - "<<max<<endl;
    cout<<"sum - "<<sum<<endl;
    cout<<"avg - "<<avg<<endl;
    
}

void parallel(vector<int> arr){
    auto start = chrono::high_resolution_clock::now();
    int min = arr[0];
    
    #pragma omp parallel for reduction(min:min)
    for(auto ele:arr){
        if(min > ele){
            min=ele;
        }
    }
    
    int max = arr[0];
    #pragma omp parallel for reduction(max:max)
    for(auto ele:arr){
        if(max < ele){
            max=ele;
        }
    }
    
    long long sum=0;
    #pragma omp parallel for reduction(+:sum)
    for(auto ele:arr){
        sum += ele;
    }
    
    int avg = sum / arr.size();
    
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    cout<<"Time required is - "<<duration.count()<<endl;
    cout<<"Min - "<<min<<endl;
    cout<<"max - "<<max<<endl;
    cout<<"sum - "<<sum<<endl;
    cout<<"avg - "<<avg<<endl;
    
}



int main() {
    // Define vector size
   int n=1000;
   
      // Create a random number generator with a proper seed
    std::random_device rd;
    std::mt19937 gen(rd()); // Mersenne Twister engine
    std::uniform_int_distribution<int> dis(1, 1000); // Random numbers between 1 and 1000

    // Create and fill a vector with random numbers
    vector<int> arr(n);
    for (int i = 0; i < n; i++) {
        arr[i] = dis(gen); // Get a random number from the distribution
    }

   sequential(arr);
   parallel(arr);
   return 0;
}
