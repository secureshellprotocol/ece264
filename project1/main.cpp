//James Ryan 2026
/*	Project 1
 *	Takes an input file describing a line of Foobar/Foobar derivative creatures
 *	and writes an output file describing each creatures name and their strength
*/

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <new>
#include <sstream>

using namespace std;

/************
 *	Classes	*
 ***********/

class Foobar{
	private:
		int 	pos;
		string 	name;
	protected:	
		int 	get_pos();
	public:
		Foobar(string n, int p = 0);
		
		virtual int	get_strength();
		void		set_pos(int p);
		string		get_name();
};

Foobar::Foobar(string n, int p){
	pos = p;
	name = n;
}

//	position getter
int Foobar::get_pos(){
	return pos;
}

//	position setter
void Foobar::set_pos(int p){
	pos = p;
	return;
}

//	name getter
string Foobar::get_name(){
	return name;
}

//	strength getter
int Foobar::get_strength(){
	return pos;
}

//	Inherits from Foobar
class Foo:public Foobar{
	public:
		Foo(string n, int p = 0):Foobar(n, p){}

		virtual int	get_strength();
};

//	strength getter -- position in line * 3
int Foo::get_strength(){
	return this->get_pos()*3;
}

//	Inherits from Foobar
class Bar:public Foobar{
	public:
		Bar(string n, int p = 0):Foobar(n, p){}	
		
		virtual int	get_strength();
};

//	strength getter -- position in line + 15
int Bar::get_strength(){
	return this->get_pos()+15;
}

/****************
 *	Functions	*
 ***************/

/*	readInput
 *	Takes a vector of Foobar ptrs and an input filename as arguments
 *	Using a known file format of:
 *		<foo type>	<foo name>
 *	this creates a vector of foobar's correlating to the
 *	input file contents
 *	returns 0 on success, -1 on failure
 */
int readInput(vector<Foobar*> *line, string file){
	ifstream readin(file);
	
	//err: file dne
	if(!readin){ 
		return -1;	
	}
	
	//err: line dne
	if(line == NULL){
		return -1;
	}

	string token;
	while(getline(readin, token)){
		string type, name;
		stringstream s(token);

		s >> type;
		s >> name;
		
		if(type == "foobar"){
			line->push_back(new Foobar(name));
		}
		if(type == "foo"){
			line->push_back(new Foo(name));
		}
		if(type == "bar"){
			line->push_back(new Bar(name));
		}
	}

	return 0;
}

/*	foobarCleaning
 *	Takes a vector of Foobar ptrs as arguments
 *	Cleans each foobar, assigning them a line position
 *	returns 0 on success, -1 on failure
 */
int foobarCleaning(vector<Foobar*> *line){
	//err: line dne
	if(line == NULL){
		return -1;
	}

	//from size-1 to 0
	for(int i = line->size()-1; i >= 0; i--){
		line->at(i)->set_pos(line->size() - i);
	}

	return 0;
}

/*	writeToFile
 *	Takes a vector of Foobar ptrs and an output file name as arguments
 * 	Writes a nicely formatted line of Foobars & Co. to a file
 * 	Output format:
 * 		<foobar name> <foobar strength>
 */
int writeToFile(vector<Foobar*> *line, string file){
	ofstream readout(file);
	
	//err: file dne
	if(!readout){ 
		return -1;	
	}
	
	//err: line dne
	if(line == NULL){
		return -1;
	}

	for(int i = 0; i < line->size(); i++){
		readout << line->at(i)->get_name();
		readout << " ";
		readout << line->at(i)->get_strength();
		readout << endl;
		
		//free that foobar
		delete line->at(i);
		line->at(i) = nullptr;
	}
	line->clear();

	return 0;
}

int main(){
	//prompt for file
	string input_file, output_file;
	cout << "input" << endl;
	cin >> input_file;
	cout << "output" << endl;
	cin >> output_file;

	//prep line of foobars, commence cleaning
	vector<Foobar*> line;

	readInput(&line, input_file);
	foobarCleaning(&line);
	writeToFile(&line, output_file);

	return 0;
}
