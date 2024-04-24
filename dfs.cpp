#include <iostream>
#include <omp.h>
#include <chrono>
#include <vector>
#include <stack>
using namespace std;
class Graph{
    private:
    int size;
    vector<vector<int>> adjList;
    
    public:
    Graph(int v){
        size = v;
        adjList.resize(v);
    }
    
    void addEdge(int u, int v) {
        adjList[u].push_back(v);
        adjList[v].push_back(u); // Undirected graph
    }
    
    void DFS(int start) {
        vector<bool> visited(size,false);
        stack<int> stk;
        stk.push(start);
        
        while(!stk.empty()){
            int current = stk.top();
            stk.pop();
            
            if( !visited[current] ){
                
                cout<<"visited - "<<current<<endl;
                visited[current]=true;
                
                for(auto children:adjList[current]){
                    
                    if(!visited[children]){
                        stk.push(children);
                    }
                    
                }
            }
        }
        
    }
    
    void parallelDFS(int start){
    vector<int> visited(size,false);
    stack<int> stk;
    stk.push(start);
    
    #pragma omp parallel 
    {
        while(!stk.empty()){
            int current;
            #pragma omp critical
            {
                if( !stk.empty() ){
                    current = stk.top();
                    stk.pop();
                }else{
                    current = -1;
                }
                
            }
            
            
            if(current !=-1 && !visited[current]){
                visited[current] = 1;
                cout<<"Visited - "<<current<<endl;
                
                //visit children
                #pragma omp critical
                {
                    for(auto neighbour:adjList[current]){
                        if(!visited[neighbour]){
                            stk.push(neighbour);
                        }
                    }
                }
            }
        }
    }
}


    
};



int main()
{
      // Create a graph with 7 vertices
    Graph graph(7);

    // Adding edges to the graph to make it undirected
    graph.addEdge(0, 1);
    graph.addEdge(0, 2);
    graph.addEdge(1, 3);
    graph.addEdge(1, 4);
    graph.addEdge(2, 5);
    graph.addEdge(2, 6);
    
    
    auto nstart = chrono::high_resolution_clock::now();
    graph.DFS(0);
    auto nend = chrono::high_resolution_clock::now();
    chrono::duration<double> nduration = nend - nstart;
    cout<<"Time required for serial is "<<nduration.count()<<endl;
    
    auto start = chrono::high_resolution_clock::now();
    graph.parallelDFS(0);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    cout<<"Time required for parallel is "<<duration.count();

    return 0;
}
