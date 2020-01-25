#include <iostream>
#include <string.h>
#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <list>
#include <utility> 
#include <algorithm> 
#include <chrono> 
#include <math.h>
#include <limits>
#include <ctime>
#define N 100000
using namespace std::chrono; 
using namespace std;

float* load_data(string input_file){
	float* input_data = new float[N];

	ifstream inputdata(input_file); //stores all the input data
	if (inputdata.fail()) {
		throw(logic_error("Error : File not Found for Loading"));
	}

	string line;

	int index = 0;
	
	while (getline(inputdata, line)){
		input_data[index] = stoi(line); 

		index++;
	}
	return(input_data);
}

void MergeInPlace(float* input_array, int n1, int n2, int n3){
	int length = n3-n1+1;
	int lleft = n2-n1+1;
	int lright = length - lleft;

	float* Aleft = new float[lleft];
	for(int i=0; i<lleft; i++){
		Aleft[i] = input_array[n1+i];
	}
	float* Aright = new float[lright];
	for(int i=0; i<lright; i++){
		Aright[i] = input_array[n1+lleft+i];
	}
	int i = 0;
	int j = 0;

	for(int k=0; k<lleft+lright; k++){
		if(i<lleft && j<lright){
			if (Aleft[i] <= Aright[j]){
				input_array[n1+k] = Aleft[i];
				i++;
			}else{
				input_array[n1+k] = Aright[j];
				j++;
			}
		}else{
			if(i<lleft){
				input_array[n1+k] = Aleft[i];
				i++;
			}else if(j<lright){
				input_array[n1+k] = Aright[j];
				j++;
			}
		}
	}
	delete [] Aleft;
	delete [] Aright;
}

void SortInPlace(float* input_array, int n1, int n2){
	int length = n2 - n1 + 1;
	if(length==2){
		if(input_array[n1]>input_array[n2]){
			float dummy = input_array[n1];
			input_array[n1] = input_array[n2];
			input_array[n2] = dummy;
		}
	}
	else if(length>2){
		int lleft;
		int lright;
		if(length%2 == 0){
			lleft = length/2;
			lright = length - lleft;
		}else{
			lleft = (length+1)/2;
			lright = length - lleft;
		}
		SortInPlace(input_array,n1,n1+lleft-1);
		SortInPlace(input_array,n1+lleft,n2);
		MergeInPlace(input_array,n1,n1+lleft-1,n2);
	}
}

void print(float* answer, string output_file) 
{
	ofstream file(output_file);
	if(file.is_open()){
		for(int i = 0; i < N; i++){
			file << answer[i] << endl;
		}
		file.close();
	}
}

int main(int argc, char** argv){

	/*for(int i=0; i<5; i++){
		auto startTime = high_resolution_clock::now();
		float* input_data = load_data("merge_data.txt");
		SortInPlace(input_data,0,99999);
		auto stopTime = high_resolution_clock::now();  //done by me
		auto duration = duration_cast<milliseconds>(stopTime - startTime); //done by me
		cout<< duration.count() << endl;
	}*/
	/*
	auto startTime = high_resolution_clock::now();
	float* input_data = load_data("merge_data.txt");
	SortInPlace(input_data,0,99999);
	auto stopTime = high_resolution_clock::now();  //done by me
	auto duration = duration_cast<milliseconds>(stopTime - startTime); //done by me
	cout<< duration.count() << endl;
	*/
	//print(input_data,"output.txt");

	/*
	//sanity check
	int answer = 1;
	for(int i=1; i<N; i++){
		if(input_data[i] < input_data[i-1]){
			answer = 0;
		}
	}

	if(answer==0){
		cout << "Error: Sanity Check" << endl;
	}
	cout << "built in " << endl;
	for(int i=0; i<5;i++){*/
		auto startTime = high_resolution_clock::now();
		float* input_data1 = load_data("merge_data.txt");
		sort(input_data1,input_data1+100000);
		auto stopTime = high_resolution_clock::now();  //done by me
		auto duration = duration_cast<milliseconds>(stopTime - startTime); //done by me
		cout<< duration.count() << endl;
	//}


	print(input_data1,"output.txt");	

	return 0;
}