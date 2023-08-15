#include<bits/stdc++.h>
#include <iostream>
#include <cstdio>
#include <chrono>
#include <thread>
#include <algorithm>
using namespace std;
using namespace chrono;

struct Node {
    float priority;
    int pname;
    float burst_time;	//burst time
    int arrival_time;	//arrival time    
    float wTime;
    int degree; 
    Node *child;
    Node *sibling;
    Node *parent;
};

Node* newNode(int name, int arrival_time, int burst_time){
    Node *temp = new Node;
    temp->pname = name;
    temp->priority = 0;
    temp->arrival_time = arrival_time;
    temp->burst_time = burst_time;
    temp->degree = 0;
    temp->wTime = 0;
    temp->child = temp->parent = temp->sibling = NULL;
    return temp;
}

void displayList(list<Node*> _heap){
    for(auto x:_heap){
		//cout << x->priority << " ";
        cout << x->pname << " ";
    }
    cout << "\n";  
}

Node* mergeBinomialTrees(Node *b1, Node *b2) {
    if (b1->priority > b2->priority)
        swap(b1, b2);

    b2->parent = b1;
    b2->sibling = b1->child;
    b1->child = b2;
    b1->degree++;
    return b1;
}

list<Node*> unionBionomialHeap(list<Node*> l1, list<Node*> l2) {
    list<Node*> _new;
    list<Node*>::iterator it = l1.begin();
    list<Node*>::iterator ot = l2.begin();
    while (it!=l1.end() && ot!=l2.end())
    {
        if((*it)->degree <= (*ot)->degree)
        {
            _new.push_back(*it);
            it++;
        }
        else
        {
            _new.push_back(*ot);
            ot++;
        }
    }

    while (it != l1.end()) {
        _new.push_back(*it);
        it++;
    }

    while (ot!=l2.end())
    {
        _new.push_back(*ot);
        ot++;
    }
    return _new;
}

list<Node*> adjust(list<Node*> _heap) {
    if (_heap.size() <= 1)
        return _heap;
    list<Node*> new_heap;
    list<Node*>::iterator it1,it2,it3;
    it1 = it2 = it3 = _heap.begin();

    if (_heap.size() == 2)
    {
        it2 = it1;
        it2++;
        it3 = _heap.end();
        /*
        it1 = heap[0]
        it2 = heap[1]
        it3 = heap[1]
        */
    }
    else
    {
        it2++;
        it3=it2;
        it3++;
        /*
        it1 = heap[0]
        it2 = heap[1]
        it3 = heap[2]
        */    
    }
    
    while (it1 != _heap.end()) {
        if (it2 == _heap.end()) //if heap.size = 2 in 1st iteration
			it1++;

        else if ((*it1)->degree < (*it2)->degree) {
            it1++;
            it2++;
            if(it3!=_heap.end())
                it3++;
        }

        else if (it3!=_heap.end() && (*it1)->degree == (*it2)->degree && (*it1)->degree == (*it3)->degree){
            it1++;
            it2++;
            it3++;
        }

        else if ((*it1)->degree == (*it2)->degree)
        {
            Node *temp;
            *it1 = mergeBinomialTrees(*it1,*it2);
            it2 = _heap.erase(it2);
            if(it3 != _heap.end())
                it3++;
        }
    }
    return _heap;
}

list<Node*> insertATreeInHeap(list<Node*> _heap, Node *tree) {
    list<Node*> temp;
    temp.push_back(tree);
    temp = unionBionomialHeap(_heap,temp);
    return adjust(temp);
}

list<Node*> removeMinFromTreeReturnBHeap(Node *tree) {
    list<Node*> heap;
    Node *temp = tree->child;
    Node *lo;

    while (temp) {
        lo = temp;
        temp = temp->sibling;
        lo->sibling = NULL;
        heap.push_front(lo);
    }
    return heap;
}

list<Node*> insert(list<Node*> _head, int name, int arrival_time, int burst_time) {
    Node *temp = newNode(name, arrival_time, burst_time);
    return insertATreeInHeap(_head,temp);
}

Node* getMin(list<Node*> _heap) {
    list<Node*>::iterator it = _heap.begin();
    Node *temp = *it;
    while (it != _heap.end()) {
        if ((*it)->priority < temp->priority)
            temp = *it;
        it++;
    }
    return temp;
}

list<Node*> extractMin(list<Node*> _heap)
{
    list<Node*> new_heap,lo;
    Node *temp;

    temp = getMin(_heap);
    list<Node*>::iterator it;
    it = _heap.begin();
    while (it != _heap.end())
    {
        if (*it != temp)
        {
            new_heap.push_back(*it);
        }
        it++;
    }
    lo = removeMinFromTreeReturnBHeap(temp);
    new_heap = unionBionomialHeap(new_heap,lo);
    new_heap = adjust(new_heap);
    return new_heap;
}

void printTree(Node *h) {
    while (h)
    {
        cout << h->pname<<"("<<h->priority<<","<<h->burst_time<<") ";
        printTree(h->child);
        h = h->sibling;
    }
}
void heapify(Node*, int, Node*);
void traverseTree(Node *h, Node *min) {
    while (h){
        if (h!=min){ 
            h->wTime++;
            h->priority = 0 - (h->wTime/h->burst_time);
		}
        //heapify(h,1,min);
        traverseTree(h->child, min);
        h = h->sibling;
    }
    
}
void printHeap(list <Node*> _heap) {
    list<Node*> ::iterator it;
    it = _heap.begin();
    while (it != _heap.end()) {
        printTree(*it);
        it++;
    }
    cout << "\n";
}

void displayQueue(list<Node*> q){
	cout << "Job_name\tArrival_Time\tPriority\tBurst_time\n";
    for(auto x:q){
        cout << x->pname << "\t\t"<< x->arrival_time << "\t\t"<< x->priority << "\t\t"<< x->burst_time << "\n";
    }
    cout << "\n";  	
}


bool sort_a_t(Node* a,Node* b){
    return a->arrival_time < b->arrival_time; 
}

void heapify(Node*_heap, int flag, Node* min){
    Node *y;
    Node *s;
    Node *z;
    y = _heap;
    z = y->parent;
    /*
    list<Node*> ::iterator it;
    it = _heap.begin();
	while (it != _heap.end()) {
	    y = (*it);
	    z = y->parent; */
	    while (z != NULL && y -> priority < z -> priority) {
	            // swap contents
	            s = y;

	            y->priority = z->priority;
	            y->pname = z->pname;
	            y->burst_time = z->burst_time;
	            y->arrival_time  = z->arrival_time;
	            y->wTime = z->wTime;
	            z->priority = s->priority;
	            z->pname = s->pname;
	            z -> burst_time =  s-> burst_time;
	            z -> arrival_time = s->arrival_time;
	            z -> wTime = s->wTime;
	            cout <<z->pname;
	    }        
	    /*
	    it++;
	}
    */
}

void updateHeap(list<Node*> _heap, int flag, Node *min) {
    list<Node*> ::iterator it;
    it = _heap.begin();
    while (it != _heap.end() /*&& *it == min*/) {
        traverseTree(*it, min);
        heapify((*it), flag, min);
        it++;
    }
}
int main(){
	auto start = high_resolution_clock::now();
    list<Node*> rq;
    list<Node*> _heap;
    list<Node*> CPU;
    int jobName;
    int temp[2];
    int n=0;
    Node* y;
    FILE* file = fopen("jobs.txt", "r");
    
    while(fscanf(file, "%d\t%d\t%d\n", &jobName, &temp[0], &temp[1]) != EOF){
    	n++;
    	//Node* newNode(name, prio, arrival_time, burst_time);
    	rq.push_back(newNode(jobName, temp[0], temp[1]));
    }
    fclose(file);
    rq.sort(sort_a_t);
	int time=0, flag = 1;
	list<Node*>:: iterator x;
	Node *min ;
    list<int> e;
    e.push_back(0);
	while(!rq.empty()){
		x = rq.begin();

		while(x!=rq.end()){
			if((*x)->arrival_time > time) break;
			if((*x)->arrival_time==time){
				//list<Node*> insert(list<Node*> _head, char name, int priority, int arrival_time, int burst_time)
                //y = *x;
                //y->wTime++;

				_heap = insert(_heap, (*x)->pname, (*x)->arrival_time, (*x)->burst_time);
				x = rq.erase(x);
			}
		}
		min = getMin(_heap);
        
        if (!CPU.empty() && CPU.back()->pname == min->pname){
            
            min->burst_time -= 1;
            e.back()++;
            min->wTime = 0;//changes made 
            min->priority = 0;
        }
        else{
            min->burst_time -= 1;
            CPU.push_back(min);
            e.push_back(time+1);
            min->wTime = 0;//changes made 
            min->priority = 0;
        }
        //heapify(min, flag, min);
        
        if (min->burst_time == 0)
            extractMin(_heap);
        
        heapify(min, flag, min);
        updateHeap(_heap,flag, min);
        if(flag == 1) flag = 0;
		time++;
	}
   // auto start = high_resolution_clock::now();
    while (!_heap.empty()){
        min = getMin(_heap);
        time ++;
        if (!CPU.empty() && CPU.back()->pname == min->pname){

            min->burst_time -= 1;
            e.back()++;
            min->wTime = 0;//changes made 
            min->priority = 0;       
        }
        else{
            
            min->burst_time -= 1;
            CPU.push_back(min);
            e.push_back(time);
            min->wTime = 0;//changes made 
            min->priority = 0;
        }
        heapify(min, flag, min);
        if(min->burst_time == 0) _heap = extractMin(_heap);
    }
	
    list<Node*>:: iterator it1;
    list<int>:: iterator it2;
    it1 = CPU.begin();
    it2 = e.begin();
    cout << "\n\tStarts at\tPID\tSuspended at\n\n" ;
    while (it1!= CPU.end()){
        cout<<"\t"<< (*it2) << "\t\t" << (*it1)->pname<<"\t\t"<< (*(++it2)) << endl;
        it1++; 
        
    }
	auto end = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(end - start).count();

    // Print the processing time
    cout << "\nProcessing time: " << duration << " nanoseconds" << std::endl;
    return 0;
}