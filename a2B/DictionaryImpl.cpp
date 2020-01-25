#include"IDictionary.h"
#include <list>
#include <utility>
#include <iostream>
#include <string.h>
#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <algorithm> 

using namespace std;

class HashTableImpl: public IDictionary { 
	private:
		int noBuckets;
		list<pair<int,char*> > *hashtable;
		int counter;

	public: 
	HashTableImpl(int capacity) {  
		noBuckets = capacity;
		hashtable = new list<pair<int,char*> >[capacity];
		counter = 0;
	}

	int hashFunction(int key){
		return(key%noBuckets);
	}

	void insert(int key, char* value){
		if(counter >= noBuckets){
			//throw(out_of_range){"Error : Size is less than the elements being inserted"};
		}
		int bucket = hashFunction(key);
		pair<int,char*> varP;
		varP.first = key;
		varP.second = value; 
		hashtable[bucket].push_back(varP);
		counter++;
	}

 	char* lookup(int key){
 		int bucket = hashFunction(key);
 		pair<int,char*> varP;
 		for(list<pair<int,char*> > :: iterator it = hashtable[bucket].begin(); it!=hashtable[bucket].end(); it++){
			varP = *it;
			if(varP.first == key){
				return(varP.second);
			}
		}
		char* value = new char[1];
		value[0] = '\0';
		return(value);
 	}

 	void printTable(char* output_file){
 		pair<int,char*> varP;
 		ofstream file(output_file);
		if(file.is_open()){
			for(int i = 0; i<noBuckets; i++){
				for(list<pair<int,char*> > :: iterator it = hashtable[i].begin(); it!=hashtable[i].end(); it++){
					varP = *it;
					file << varP.first;
					file << ", " << varP.second;
					file << "||";
				}
				file << endl;
			}
			file.close();
		}
 	}
};

class ArrayDictImpl : public IDictionary {
	private:
		int size;
		pair<int,char*> *myArray;
		int counter; // to keep track of how many elements have been inserted

	public:
	ArrayDictImpl(int capacity) {
		size = capacity;
		counter = 0;
		myArray = new pair<int,char*>[capacity];
	}

	void insert(int key, char* value){
		if(counter == size){
			throw(out_of_range){"Error : Size is less than the elements being inserted"};
		}
		pair<int,char*> varP;
		varP.first = key;
		varP.second = value; 
		myArray[counter] = varP;
		counter++;
	}

	char* lookup(int key){
		pair<int,char*> varP;
		for(int i = 0; i < size ; i++){
			varP = myArray[i];
			if(varP.first == key){
				return(varP.second);
			}
		}
		char* value = new char[1];
		value[0] = '\0';
		return(value);
	}

 	void printTable(char* output_file){
 		pair<int,char*> varP;
 		ofstream file(output_file);
		if(file.is_open()){
			for(int i = 0; i < size ; i++){
				varP = myArray[i];
				file << varP.first << ", " << varP.second << endl;
			}
		}
 	}

};

class BSearchDictImpl : public IDictionary {
	private:
		int size;
		pair<int,char*> *myArray;
		int counter; // to keep track of how many elements have been inserted
		int flagSort; //1 means array is sorted, 0 means array is not sorted
	public:
	BSearchDictImpl(int capacity) {
		size = capacity;
		counter = 0;
		flagSort = 0;
		myArray = new pair<int,char*>[capacity];
	}

	void insert(int key, char* value){
		if(counter == size){
			throw(out_of_range){"Error : Size is less than the elements being inserted"};
		}
		pair<int,char*> varP;
		varP.first = key;
		varP.second = value; 
		myArray[counter] = varP;
		counter++;
		if(counter==size){
			mySort();
			flagSort = 1;
		}
	}

	char* lookup(int key){
		if(flagSort == 0){
			mySort();
			flagSort = 1;
		}
		return(BinarySearch(myArray, 0, size, key));
	}

 	void printTable(char* output_file){
 		pair<int,char*> varP;
 		ofstream file(output_file);
		if(file.is_open()){
			for(int i = 0; i < size ; i++){
				varP = myArray[i];
				file << varP.first << ", " << varP.second << endl;
			}
		}
 	}

 	static bool compare(pair<int,char*> p1, pair<int,char*> p2){
		if(p1.first < p2.first){
			return(true);
		}
		return(false);
	}

	void mySort() {
		sort(myArray, myArray + size, compare);
	}

	char* BinarySearch(pair<int,char*> *Array, int start, int end, int value){
		if(start<=end){
			int mid = (int)(start+end)/2;
			if(Array[mid].first == value){
				return(Array[mid].second);
			}else if(Array[mid].first > value){
				return BinarySearch(Array, start, mid - 1, value);
			}else{
				return BinarySearch(Array, mid+1, end, value);
			}
		}
		char* ans = new char[1];
		ans[0] = '\0';
		return(ans);
	}
};