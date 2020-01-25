#include <iostream>
#include <string.h>
#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <iomanip>
#include"DictionaryImpl.cpp"
#include <fstream>
#include <sstream>
#include <list>
#include <utility> 
#include <chrono> 
#include <math.h>
using namespace std::chrono; 

using namespace std;

//The following function takes as input a tsv file, the size of the list and reads the input file 
//and inserts them in the hash_table implementation of the dictionary
IDictionary* readandinsert_hash(char* input_file, int size){
	IDictionary* dictionary = new HashTableImpl(size);
	ifstream inputdata(input_file); //stores all the input data
	if (inputdata.fail()) {
		throw(logic_error("Error : File not Found for Loading"));
	}
	string line;
	
	while (getline(inputdata, line)){
		stringstream ss(line);
		string linedata;
		getline(ss,linedata,'\t');
		int key = stoi(linedata);
		getline(ss,linedata,'\t');
		char* value = new char[linedata.size()+1];
		copy(linedata.begin(),linedata.end(),value);
		value[linedata.size()]='\0';
		dictionary -> insert(key, value);	
	}
	return(dictionary);	
}

//The following function takes as input a tsv file, the size of the list and reads the input file 
//and inserts them in the array implementation of the dictionary
IDictionary* readandinsert_array(char* input_file, int size){
	IDictionary* dictionary = new ArrayDictImpl(size);
	ifstream inputdata(input_file); //stores all the input data
	if (inputdata.fail()) {
		throw(logic_error("Error : File not Found for Loading"));
	}
	string line;
	
	while (getline(inputdata, line)){
		stringstream ss(line);
		string linedata;
		getline(ss,linedata,'\t');
		int key = stoi(linedata);
		getline(ss,linedata,'\t');
		char* value = new char[linedata.size()+1];
		copy(linedata.begin(),linedata.end(),value);
		value[linedata.size()]='\0';
		dictionary -> insert(key, value);	
	}
	return(dictionary);	
}

//The following function takes as input a tsv file, the size of the list and reads the input file 
//and inserts them in the BSearch implementation of the dictionary
IDictionary* readandinsert_bsearch(char* input_file, int size){
	IDictionary* dictionary = new BSearchDictImpl(size);
	ifstream inputdata(input_file); //stores all the input data
	if (inputdata.fail()) {
		throw(logic_error("Error : File not Found for Loading"));
	}
	string line;
	
	while (getline(inputdata, line)){
		stringstream ss(line);
		string linedata;
		getline(ss,linedata,'\t');
		int key = stoi(linedata);
		getline(ss,linedata,'\t');
		char* value = new char[linedata.size()+1];
		copy(linedata.begin(),linedata.end(),value);
		value[linedata.size()]='\0';
		dictionary -> insert(key, value);	
	}
	return(dictionary);	
}

//The following function takes as input a file, containing the keys to be looked up,
//a IDictionary pointer which is the dictionary to be looked up in,
//returns a list of keys from the input file, and the corredponding values.
list<pair<int,char*> > lookup_result(char* lookup_file, IDictionary* table){
	list<pair<int,char*> > list;
	pair<int,char*> varP;
	ifstream inputdata(lookup_file); //stores all the input data
	if (inputdata.fail()) {
		throw(logic_error("Error : Input File not Found"));
	}
	string line;
	
	while (getline(inputdata, line)){
		int key = stoi(line);
		char* value = table -> lookup(key);
		varP.first = key;
		varP.second = value;
		list.push_back(varP);
	}
	return(list);
}

//The following function takes as input a list of key value pairs, 
//which is the the lookup result, and prints it to an output file
//returns the lookup count
int printOutput(list<pair<int,char*> > lookup_result, char* output_file){
	int ans = 0;
	pair<int,char*> varP;
	ofstream file(output_file);
	if(file.is_open()){
		for(list<pair<int,char*> > :: iterator it = lookup_result.begin(); it!=lookup_result.end(); it++){
			ans++;
			varP = *it;
			file << varP.first << '\t' << varP.second << endl;
		}
		file.close();
	}
	return(ans);
}

int main(int n, char *argv[])
{
	int size = atoi(argv[2]);
	if(strcmp("hash", argv[1])==0)
	{
		cout << "hash,"<<size<<",";
		auto start = high_resolution_clock::now(); 
		IDictionary* table = readandinsert_hash(argv[3], size);
		auto stop = high_resolution_clock::now();  
		auto duration = duration_cast<milliseconds>(stop - start);
		cout << duration.count(); 

		start = high_resolution_clock::now(); 
		list<pair<int,char*> > lookup_list = lookup_result(argv[4], table);
		stop = high_resolution_clock::now();  
		duration = duration_cast<milliseconds>(stop - start); 
	
		int lookup_count = printOutput(lookup_list,argv[5]);
		cout << "," << lookup_count << "," << duration.count() << endl;
	}
    else if( strcmp("array", argv[1])==0)
	{
		cout << "array,"<<size<<",";
		auto start = high_resolution_clock::now(); 
		IDictionary* table = readandinsert_array(argv[3], size);
		auto stop = high_resolution_clock::now();  
		auto duration = duration_cast<milliseconds>(stop - start);
		cout << duration.count(); 

		start = high_resolution_clock::now(); 
		list<pair<int,char*> > lookup_list = lookup_result(argv[4], table);
		stop = high_resolution_clock::now();  
		duration = duration_cast<milliseconds>(stop - start); 
	
		int lookup_count = printOutput(lookup_list,argv[5]);
		cout << "," << lookup_count << "," << duration.count() << endl;
	}
    else if( strcmp("bsearch", argv[1])==0 )
	{
		cout << "bsearch,"<<size<<",";
        auto start = high_resolution_clock::now(); 
		IDictionary* table = readandinsert_bsearch(argv[3], size);
		auto stop = high_resolution_clock::now();  
		auto duration = duration_cast<milliseconds>(stop - start);
		cout << duration.count(); 

		start = high_resolution_clock::now(); 
		list<pair<int,char*> > lookup_list = lookup_result(argv[4], table);
		stop = high_resolution_clock::now();  
		duration = duration_cast<milliseconds>(stop - start); 
	
		int lookup_count = printOutput(lookup_list,argv[5]);
		cout << "," << lookup_count << "," << duration.count() << endl;
	}
   	else 
		cout<<"[main] invalid input parameters. Valid usage is..."<<endl;
	return 0;
    
}