#include<bits/stdc++.h>
#include <cstdio>
#include <iostream>
#include <thread>
#include <algorithm>
#include <chrono>
using namespace std;
using namespace chrono;

struct Node {
    int priority;
    char pname;
    int burst_time;	//burst time
    int arrival_time;	//arrival time
    int restime=0;	//suspended for time
    int ctime=0;	//completion time
    int wtime=0;	//waiting time        

    int degree; 
    Node *child;
    Node *sibling;
    Node *parent;
};

Node* newNode(char name, int key, int arrival_time, int burst_time){
    Node *temp = new Node;
    temp->pname = name;
    temp->priority = key;
    temp->arrival_time = arrival_time;
    temp->burst_time = burst_time;
    temp->degree = 0;
    temp->child = temp->parent = temp->sibling = NULL;
    return temp;
}

bool sort_a_t(Node* a,Node* b){
    return a->arrival_time < b->arrival_time; 
}

class minHeap{
    private:
    
    public:
    int length;
    
    vector<Node*> heap;
    minHeap(){
   	 //constructor to initialise length of empty heap
   	 length=0;
    }

    void swap(int i, int j){
   	 //swap elements at index i and j
   	 Node* temp = heap[i];
   	 heap[i] = heap[j];
   	 heap[j] = temp;
    }

    void bottom_up_heapify(){
   	 //heapify after insertion
   	 int parent, i=length;
   	 while(i!=1){
   		 parent = (i/2);
   		 if(heap[parent-1]->priority > heap[i-1]->priority){
   			 swap(parent-1, i-1);
   			 i = parent;
   		 }
   		 else{
   			 break;
   		 }
   	 }
    }

    void top_down_heapify(){
   	 //heapify after deletion
   	 int i=1, j;
   	 int left, right;
   	 while(i<length){
   		 left = 2*i;
   		 right = 2*i + 1;
   		 if(left<length and right <= length){
   			 j = heap[left-1]->priority <= heap[right-1]->priority ? left-1 : right-1;
   			 if(heap[j]->priority < heap[i-1]->priority){
   				 swap(j, i-1);
   				 i = j+1;
   			 }
   			 else break;    
   		 }
   		 else if(left == length){
   			 if(heap[left-1]->priority < heap[i-1]->priority){
   				 swap(left-1, i-1);
   				 i = left;
   			 }
   			 else break;
   		 }
   		 else break;
   	 }
    }
    
    void display(){
   	 //display elements of max heap
   	 int i;
   	 cout << "\n";
   	 for(i=0;i<length;i++){
   		 cout << heap[i]->pname << "("<< heap[i]->priority<<", "<<heap[i]->burst_time<<") ";
   	 }
   	 cout << "\n";
    }

	void insert(char pname, int priority, int at, int bt){
	   	 //insert element into heap
		 Node *n = newNode(pname, priority, at, bt);
	   	 heap.push_back(n);
	   	 length += 1;
	   	 bottom_up_heapify(); 
	}

    void delete_(){
   	 //delete root from heap
   	 int i = length-1;
   	 if(i==-1){
   	 	return;
   	 }
   	 swap(0, i);
   	 length -= 1;
   	 top_down_heapify();
    }
    
    Node * getMin(){
        return heap.front();
    }
};

void displayQueue(list<Node*> q){
	cout << "Job_name\tArrival_Time\tPriority\tBurst_time\n";
    for(auto x:q){
        cout << x->pname << "\t\t"<< x->arrival_time << "\t\t"<< x->priority << "\t\t"<< x->burst_time << "\n";
    }
    cout << "\n";  	
}

int main(){   
    auto start = high_resolution_clock::now();
	
    minHeap H;
    list<Node*> rq;
    list<Node*> CPU;
    char jobName;
    int temp[3];

    int n=0;
    
    FILE* file = fopen("jobs.txt", "r");

    
    while(fscanf(file, "%c\t%d\t%d\t%d\n", &jobName, &temp[0], &temp[1], &temp[2]) != EOF){
    	n++;

    	//Node* newNode(name, prio, arrival_time, burst_time);
    	rq.push_back(newNode(jobName, temp[0], temp[1], temp[2]));
    }
    fclose(file);
    //auto start = high_resolution_clock::now();
    rq.sort(sort_a_t);
	
	displayQueue(rq);
	
	int time=0;
	list<Node*>::iterator x;
	Node *min ;
    list<int> e;
	while(!rq.empty()){
		x = rq.begin();
		//H.display();		
		while(x!=rq.end()){
			if((*x)->arrival_time > time) break;
			if((*x)->arrival_time==time){
				//int insert(int _head, char name, int key, int arrival_time, int burst_time)
                H.insert((*x)->pname, (*x)->priority, (*x)->arrival_time, (*x)->burst_time);
				x = rq.erase(x);
			}
		}
		min = H.getMin();
        
        if (!CPU.empty() && CPU.back()->pname == min->pname){
            min->burst_time -= 1;
            e.back()++;
        }
        else{
            min->burst_time -= 1;
            CPU.push_back(min);
            e.push_back(time+1);
        }
        if (min->burst_time == 0){
            H.delete_();
        }

		time++;
	}

	while (H.length){
        min = H.getMin();
        time += min->burst_time;
        if (!CPU.empty() && CPU.back()->pname == min->pname){
         	
            e.back()+=min->burst_time;
        }
        else{
     
            CPU.push_back(min);
            e.push_back(time);
        }  
        min->burst_time = 0;      
        H.delete_();
        //H.display();
        
    }
	
    list<Node*>:: iterator it1;
    list<int>:: iterator it2;
    it1 = CPU.begin();
    it2 = e.begin();
    cout << "|   " ;
    while (it1!= CPU.end()){
        cout<<(*it1)->pname<<"\t|   ";
        it1++; //it2++;
    }
    cout << "\n0\t";
    while(it2!= e.end()){
        
    	cout <<(*it2) << "\t";
        
    	it2++; 
    }
    cout << "\n";
	//displayQueue(rq);
	auto end = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(end - start).count();

    // Print the processing time
    cout << "Processing time: " << duration << " nanoseconds" << std::endl;
   // */
    return 0;
 
}
