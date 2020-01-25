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
#include <chrono> 
#include <math.h>
#include <limits>
#include <ctime>
#include <mpi.h>
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

float* Merge(float* Aleft, float* Aright, int lleft, int lright){
	float* output_array = new float[lleft+lright];
	int i = 0;
	int j = 0;

	for(int k=0; k<lleft+lright; k++){
		if(i<lleft && j<lright){
			if (Aleft[i] <= Aright[j]){
				output_array[k] = Aleft[i];
				i++;
			}else{
				output_array[k] = Aright[j];
				j++;
			}
		}else{
			if(i<lleft){
				output_array[k] = Aleft[i];
				i++;
			}else if(j<lright){
				output_array[k] = Aright[j];
				j++;
			}
		}
	}
	delete [] Aleft;
	delete [] Aright;
	return(output_array);
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
	int pid, size;
	MPI_Comm comm = MPI_COMM_WORLD;

	MPI_Init(&argc, &argv);

	MPI_Comm_rank(comm, &pid);
	MPI_Comm_size(comm, &size);

	if(pid == 0){
		//cout << "for number of processors: " << size << endl;
	}

	//for(int ti = 0; ti<5;ti++){
		MPI_Barrier(comm);
		//auto startTime = high_resolution_clock::now();
		double start = MPI_Wtime();

		int* scatterSizes = new int[size];
		int* displacements = new int[size];
		displacements[0] = 0;
		
		if(N%size == 0){
			for(int i=0; i<size; i++){
				scatterSizes[i] = N/size;
				if(i!=0){
					displacements[i] = displacements[i-1]+scatterSizes[i-1];	
				}
				
			}
		}else{
			for(int i=0; i<size; i++){
				if(i%2==0){
					scatterSizes[i] = (int)(N/size);	
				}else{
					scatterSizes[i] = (ceil)((float)N/size);	
				}
				if(i!=0){
					displacements[i] = displacements[i-1]+scatterSizes[i-1];	
				}
			}
		}
		
		/*
		//sanity check
		if(pid==0){
			int sum = 0;
			for(int i=0;i<size;i++){
				sum = sum + scatterSizes[i];
			}
			if(sum!=N){
				cout << "Error: " << sum << endl;
			}
		}
		*/
		float* input_data;

		if(pid == 0){
			input_data = load_data("merge_data.txt");
		}
		float* sub_data = new float[scatterSizes[pid]];
		int sub_length = scatterSizes[pid];	
		MPI_Scatterv(input_data, scatterSizes, displacements, MPI_FLOAT, sub_data, sub_length, MPI_FLOAT, 0, comm);

		int noIter = ceil(log2(size));

		SortInPlace(sub_data,0,sub_length-1);
		
		for(int i = 0; i< noIter; i++){
			int a = pow(2,(i+1));
			int b = pow(2,i);
			if(pid%a == 0){
				if(pid+pow(2,i)<size){
					int sub_length_received;
					MPI_Recv(&sub_length_received, 1, MPI_INT, pid+pow(2,i), 0, comm, MPI_STATUS_IGNORE);
					float* sub_data_received = new float[sub_length_received];
					MPI_Recv(sub_data_received, sub_length_received, MPI_FLOAT, pid+pow(2,i), 1, comm, MPI_STATUS_IGNORE);

					float* new_sub_data;
					int new_sub_length = sub_length+sub_length_received;
					new_sub_data = Merge(sub_data, sub_data_received, sub_length, sub_length_received);
					sub_data = new_sub_data;
					sub_length = new_sub_length;
				}
			}else{
				if(pid%b == 0){
					MPI_Send(&sub_length, 1, MPI_INT, pid-pow(2,i), 0, comm);
					MPI_Send(sub_data, sub_length, MPI_FLOAT, pid-pow(2,i), 1, comm);	
					delete [] sub_data;
				}
			}
		}

		/*
		if(pid==0){
			//sanity check
			int answer = 1;
			for(int i=1; i<N; i++){
				if(sub_data[i] < sub_data[i-1]){
					answer = 0;
				}
			}
			if(answer==0){
				cout << "Error: sanity check" << endl;
			}
			//print(sub_data,"outputp.txt");
		}*/

		MPI_Barrier(comm);
		//auto stopTime = high_resolution_clock::now();  //done by me
		double stop = MPI_Wtime();
		//auto duration = duration_cast<milliseconds>(stopTime - startTime); //done by me
		if(pid==0){
			//cout<< "time taken: " << duration.count() << endl;
			cout<< (stop - start)*1000 << endl;
		}
		delete [] scatterSizes;
		delete [] displacements;

		if(pid==0){
			print(sub_data,"outputp.txt");
		}
	//}

	MPI_Finalize();
	return 0;
}