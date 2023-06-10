//	James Ryan 2026
/*	Project 2 - ECE264, Cooper Union
 *
 *	Takes in a text file of a list of commands and accordingly
 *	configures a new/existing stack or queue as specified
 */

#include <iostream>
#include <string>
#include <fstream>
#include <list>
#include <sstream>
#include <vector>
#include <new>

using namespace std;

template <typename T> 
class SimpleList{
	private:

	struct Node{
		T		data;
		Node*	next;
	};
	
	Node* 	head;
	Node*	tail;

	int 	size;
	string 	name;

	/*	SL obj		top (0)					(n-1)			bottom (n)
	 *	head* -->	--------------			~-----			-----------------
	 *				|	T	| next* ---> ..	| next*	--->	|	T	| void	|
	 *				--------------			~-----			-----------------
	 *	tail* --- ...									...	--^
	 *
	 	empty list means zero nodes, no dummy node.
	 *
	 *	size = n + 1. so empty is size 0, one node is size 1. w
	 */

	protected:
		//catch-all empty node condition:
		//	inits head and tail simultaneously
		void emptyCase(T d){
			head = tail = new Node;
			head->data = d;
			size++;
				
			return;
		}
		//add node before node 0 (top node)
		void prepend(T d){		
			//edge: empty
			if(head == NULL){
				emptyCase(d);

				return;
			}
			
			Node* n = new Node;
			n->next = head;
			n->data = d;
			head = n;
			size++;

			return;
		}
		//add node after node n (bottom node)
		void append(T d){		
			//edge: empty
			if(head == NULL){
				emptyCase(d);		

				return;
			}
			
			Node* n = new Node;
			n->next = nullptr;
			n->data = d;
			tail->next = n;
			tail = n;
			size++;

			return;
		}
		//reassign head to node 1 and rm node 0
		void rmTop(T* d){
			//err: empty
			//	no data to return -- nullptr it is		
			if(head == NULL){
				d = nullptr;
				return;
			}

			Node *n = head;
			head = head->next;
			*d = n->data;
			delete n;
			size--;

			return;
		}
	
	public:
		//reserved for Stack and Queue
		virtual void push(T d) = 0;
		virtual void pop(T* d) = 0;
		
		//returns name
		string getName(){
			return name;
		}
		//returns size
		int getSize(){
			return size;
		}

		//constructs a list of size 0 with no nodes
		//no sentinel/dummy nodes
		SimpleList(string n){
			head = tail = nullptr;
			size = 0;
			name = n;
		}
};

template <typename T> 
class Queue:public SimpleList<T>{
	public:
		//nodes are added to the end
		void push(T d){
			this->append(d);
			return;
		}
		//nodes are rm'd from the start
		void pop(T* d){
			this->rmTop(d);
			return;
		}

		Queue(string n):SimpleList<T>(n){}
};

template <typename T> 
class Stack:public SimpleList<T>{
	public:
		//nodes are added to the start
		void push(T d){
			this->prepend(d);
			return;
		}
		//nodes are rm'd from the start
		void pop(T* d){
			this->rmTop(d);
			return;	
		}

		Stack(string n):SimpleList<T>(n){}
};

//searches for a SimpleList/deriv. in a vector
//	nullptr - none found
//	otherwise return the found SimpleList
template <typename T> 
SimpleList<T>* search(list<SimpleList<T>*> &list, string n){
	for(auto i = list.begin(); i != list.end(); i++){ //iterates through vector
		if((*i)->getName() == n){ //checks that current linked list's name
			return *i; //returns the list address
		}
	}

	return nullptr; //returns no address if dne
}

//creates a queue or stack of type T.
//adds it to linked list containing type T queues and stacks.
//	0 - success
//	1 - failure
template <typename T>
int create(list<SimpleList<T> *> &listSL, string n, string a){
	//err: list exists already
	if(search(listSL, n) != nullptr){
		return 1;
	}

	//create temp ptr and properly alloc mem for it
	SimpleList<T> *pSL;
	if(a == "stack"){
		pSL = new Stack<T>(n);
	}
	else {
		pSL = new Queue<T>(n);
	}
					
	listSL.push_front(pSL);
	return 0;
}

//doesnt work. unused
//	left for historical purposes.
template <typename T>
int processPush(list<SimpleList<T> *> &listSL, string n, string a){
	//err: list dne
	SimpleList<T> *pSL;
	if((pSL = search(listSL, n)) == nullptr){
		return 1;
	}
	
	//fetch data and alloc node on named list
	if(is_same<T, int>::value){
		pSL->push(stoi(a));
		return 0;
	}
	else if(is_same<T, double>::value){
		pSL->push(stod(a));
		return 0;
	}
	pSL->push(a);
	return 0;
}

//mega-function which processes a text file of commands and writes all output
//to a text file.
void parseAndWrite(string i, string o){
	list<SimpleList<int> *> listSLi;	//holds all int queues and stacks
    list<SimpleList<double> *> listSLd;	//holds all double queues and stacks
	list<SimpleList<string> *> listSLs;	//holds all string queues and stacks
	
	ifstream readin(i);
	ofstream readout(o);
	string line;
	
	while(getline(readin, line)){
		//initializing process
		string c; //command var
		string n; //name var
		string a; //action var
		char d; //datatype var

		readout << "PROCESSING COMMAND: " << line << '\n';

		stringstream ss(line);
		ss >> c >> n >> a;
		d = n[0];

		//I could not find anything about removing the starting char from name
		//in the project specifications.
		//	it seems silly to keep it, since I organize everything in a list
		//	which differentiates by type, so I'm removing it.
		n.erase(0,1);
	
		if(c == "create"){
			switch(d){
				case 'i':{
					//err: list exists already
					if(create<int>(listSLi, n, a)){
						readout << "ERROR: This name already exists!\n";
					}
					break;
				}
				case 'd':{
					//err: list exists already
					if(create<double>(listSLd, n, a)){
						readout << "ERROR: This name already exists!\n";
					}
					break;
				}
				case 's':{
					//err: list exists already
					if(create<string>(listSLs, n, a)){
						readout << "ERROR: This name already exists!\n";
					}
					break;
				}
			}
		}
		else if(c == "push"){
			switch(d){
				case 'i':{
					if(processPush<int>(listSLi, n, a)){
						readout << "ERROR: This list does not exist!\n";
					}
					break;
				}
				case 'd':{
					//err: list dne
					SimpleList<double> *pSLd;
					if((pSLd = search(listSLd, n)) == nullptr){
						readout << "ERROR: This name does not exist!\n";
						break;
					}
	
					//fetch data and alloc node on named list
					pSLd->push(stod(a));
					break;
				}
				case 's':{
					//err: list dne
					SimpleList<string> *pSLs;
					if((pSLs = search(listSLs, n)) == nullptr){
						readout << "ERROR: This name does not exist!\n";
						break;
					}
	
					//fetch data and alloc node on named list
					pSLs->push(a);
					break;	
				}
			}
		}
		else if(c == "pop"){
			switch(d){
				case 'i':{
					//err: list dne
					SimpleList<int> *pSLi;
					if((pSLi = search(listSLi, n)) == nullptr){
						readout << "ERROR: This name does not exist!\n";
						break;
					}
					//err: list empty
					else if(pSLi->getSize() == 0){
						readout << "ERROR: This list is empty!\n";
						break;
					}
					
					//fetch data and return to output
					int data = -1;
					pSLi->pop(&data);
					readout << "Value popped: " << data << '\n';
					break;
				}
				case 'd':{
					//err: list dne
					SimpleList<double> *pSLd;
					if((pSLd = search(listSLd, n)) == nullptr){
						readout << "ERROR: This name does not exist!\n";
						break;
					}
					//err: list empty
					else if(pSLd->getSize() == 0){
						readout << "ERROR: This list is empty!\n";
						break;
					}
					
					//fetch data and return to output
					double data = -1;
					pSLd->pop(&data);
					readout << "Value popped: " << data << '\n';
					break;				
				}
				case 's':{
					//err: list dne
					SimpleList<string> *pSLs;
					if((pSLs = search(listSLs, n)) == nullptr){
						readout << "ERROR: This name does not exist!\n";
						break;
					}
					//err: list empty
					else if(pSLs->getSize() == 0){
						readout << "ERROR: This list is empty!\n";
						break;
					}
					
					//fetch data and return to output
					string data = "-1";
					pSLs->pop(&data);
					readout << "Value popped: " << data << '\n';
					break;
				}
			}
		}
	}	

	//garbage collection
	listSLi.clear();
	listSLd.clear();
	listSLs.clear();

	readin.close();
	readout.close();
	return;
}

int main(){
	//init io
	string input_file, output_file;
	cout << "input file name: " << endl;
	cin >> input_file;
	cout << "output file name: " << endl;
	cin >> output_file;

	//run test
	parseAndWrite(input_file, output_file);

	return 0;
}
