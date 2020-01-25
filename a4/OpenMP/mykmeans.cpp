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
#define N 20000
using namespace std::chrono; 
using namespace std;

class Classification{
public:
	int noIterations;
	float minDistanceSq;
	float** clusters;
};

int** load_data(string input_file){
	int** input_data = new int*[N];
	for (int i=0; i<N; i++){
		input_data[i] = new int[2];
		for(int j=0; j<2; j++){
			input_data[i][j] = 0;
		}
	}

	ifstream inputdata(input_file); //stores all the input data
	if (inputdata.fail()) {
		throw(logic_error("Error : File not Found for Loading"));
	}

	string line;
	string linedata;

	int index = 0;
	
	while (getline(inputdata, line)){
		stringstream ss(line);
		getline(ss,linedata,',');
		input_data[index][0] = stoi(linedata); 
		getline(ss,linedata,',');
		input_data[index][1] = stoi(linedata); 

		index++;
	}
	return(input_data);
}

float SqEuclidDist(float x1, float y1, float x2, float y2){
	float distsq = pow((x1-x2),2) + pow((y1-y2),2);
	return(distsq);
}

/*
//defined points as initial means
float** initialMeans(){
	float** start = new float*[20];
	for (int i=0; i<20; i++){
		start[i] = new float[2];
		for(int j=0; j<2; j++){
			start[i][j] = 0;
		}
	}
	for(int i=0; i<5; i++){
		start[i][0] = -500*(i-5);
		start[i][1] = 300;    
	}
	for(int i=5; i<10; i++){
		start[i][0] = -500*(i-4);
		start[i][1] = 300;    
	}
	for(int i=10; i<15; i++){
		start[i][0] = -500*(i-15);
		start[i][1] = -300;    
	}
	for(int i=15; i<20; i++){
		start[i][0] = -500*(i-14);
		start[i][1] = -300;    
	}
	return(start);
}

//first 20 elements of the data as initial means
float** initialMeans1(int** data){
	float** start = new float*[20];
	for (int i=0; i<20; i++){
		start[i] = new float[2];
		for(int j=0; j<2; j++){
			start[i][j] = data[i][j];
		}
	}
	return(start);
}

//uniformly chooses centroid from the data
float** initialMeans2(int** data){
	srand(time(NULL));
	float** start = new float*[20];
	for (int i=0; i<20; i++){
		start[i] = new float[2];
		int randNo = (rand() %N);
		for(int j=0; j<2; j++){
			start[i][j] = data[randNo][j];
		}
	}
	return(start);
}

//takes the initial centroid as the centroid of every 1000 elements in the data
float** initialMeans3(int** data){
	float** start = new float*[20];
	for (int i=0; i<20; i++){
		start[i] = new float[2];
		for(int k = 1000*i ; k < (1000*i + 1000); k++){
			for(int j=0; j<2; j++){
				start[i][j] = start[i][j] + data[k][j];
			}
		}
		start[i][0] = start[i][0]/1000;
		start[i][1] = start[i][1]/1000;
	}
	return(start);
}

float** kmeanspp(int** data){
	srand(time(NULL));
	//initialisation
	float** start = new float*[20];
	for (int i=0; i<20; i++){
		start[i] = new float[2];
		for(int j=0; j<2; j++){
			start[i][j] = 0;
		}
	}

	//first centroid
	int randNo = (rand() %N);
	start[0][0] = data[randNo][0];
	start[0][1] = data[randNo][1];

	float* weights = new float[N];
	float* cumweights = new float[N];

	for(int ci=1; ci<20; ci++){
		for(int i =0; i<N; i++){
			weights[i] = SqEuclidDist(data[i][0],data[i][1],start[0][0],start[0][1]);
			float dummy = 0;
			for(int j=0;j<ci;j++){
				dummy = SqEuclidDist(data[i][0],data[i][1],start[j][0],start[j][1]);
				if(dummy<weights[i]){
					weights[i] = dummy;
				}
			}
		}

		cumweights[0] = weights[0];
		for(int i=1; i<N; i++){
			cumweights[i] = cumweights[i-1] + weights[i];
		}

		randNo = rand();
		randNo = randNo%(int(cumweights[N-1]))+1;
		for(int i=1; i<N; i++){
			if(randNo<=cumweights[i]){
				start[ci][0] = data[i][0];
				start[ci][1] = data[i][1];
				break;
			}
		}
	}

	return(start);
}
*/

float** initialMeans4(){
	float** y = new float*[20];
	for (int i=0; i<20; i++){
		y[i] = new float[2];
		for(int j=0; j<2; j++){
			y[i][j] = 0;
		}
	}
	y[0][0]=2182, y[0][1]=224;
	y[1][0]=-245, y[1][1]= 491;
	y[2][0]=2614, y[2][1]=-281;
	y[3][0]=-1566, y[3][1]=530;
	y[4][0]=2865, y[4][1]=-222;
	y[5][0]=-2955, y[5][1]=493;
	y[6][0]=-962, y[6][1]=-360;
	y[7][0]=2562, y[7][1]=261;
	y[8][0]=-2141, y[8][1]=-510;
	y[9][0]=-2858, y[9][1]=-244;
	y[10][0]=-2645, y[10][1]=-407;
	y[11][0]=-2232, y[11][1]=328;
	y[12][0]=1534, y[12][1]=-259;
	y[13][0]=2787, y[13][1]=-257;
	y[14][0]=-1598, y[14][1]=-509;
	y[15][0]=2648, y[15][1]=-483;
	y[16][0]=2950, y[16][1]=-304;
	y[17][0]=2095, y[17][1]=449;
	y[18][0]=-2826, y[18][1]=-478;
	y[19][0]=2315, y[19][1]=489;
	return(y);
}

float** initialMeans5(){
	float** y = new float*[20];
	for (int i=0; i<20; i++){
		y[i] = new float[2];
		for(int j=0; j<2; j++){
			y[i][j] = 0;
		}
	}
	y[0][0]=-20624, y[0][1]=-6119;
	y[1][0]=20761, y[1][1]=-8440;
	y[2][0]=-35693, y[2][1]=9959;
	y[3][0]=-32098, y[3][1]=-9044;
	y[4][0]=20009, y[4][1]=-6447;
	y[5][0]=7407, y[5][1]=-7040;
	y[6][0]=47883, y[6][1]=-8378;
	y[7][0]=34028, y[7][1]=-9822;
	y[8][0]=7764, y[8][1]=-5008;
	y[9][0]=60466, y[9][1]=7114;
	y[10][0]=-34228, y[10][1]=-6265;
	y[11][0]=7041, y[11][1]=8689;
	y[12][0]=-61658, y[12][1]=-10042;
	y[13][0]=-63294, y[13][1]=-7637;
	y[14][0]=21135, y[14][1]=-7623;
	y[15][0]=47283, y[15][1]=7978;
	y[16][0]=22481, y[16][1]=-7685;
	y[17][0]=-33246, y[17][1]=-9708;
	y[18][0]=-8101, y[18][1]=-6684;
	y[19][0]=32267, y[19][1]=6058;
	return(y);
}

Classification Classify(int** input_data, float** means){
	Classification varC;
	float** answer = new float*[20];
	for (int i=0; i<20; i++){
		answer[i] = new float[4];
		answer[i][0] = i+1;
		for(int j=1; j<4; j++){
			answer[i][j] = 0;
		}
	}

	int itertaken = 0;
	int* ClusterIndex = new int[N]; //stores cluster index of each input

	for(int noIter = 0; noIter<100; noIter++){

		//assignment
		int change = 0;
		int* changeAdd = new int[20]; //keeps track of how many items are added in each cluster
		for(int i = 0 ; i<20; i++){
			changeAdd[i] = 0;
		}
		for(int i = 0; i < N; i++){
			float min = std::numeric_limits<double>::infinity();
			int minClusterIndex = 0;
			for(int j = 0; j < 20; j++){
				float distsq = SqEuclidDist(input_data[i][0],input_data[i][1],means[j][0],means[j][1]);
				if(distsq < min){
					min = distsq;
					minClusterIndex = j+1;
				}
			}
			if(ClusterIndex[i] != minClusterIndex){
				change = 1;
			}
			/*
			//take out the item from it's old cluster
			if(ClusterIndex[i]!=0){
				answer[ClusterIndex[i]-1][1] = answer[ClusterIndex[i]-1][1]-1;
			}
			//update Cluster Index
			ClusterIndex[i] = minClusterIndex;
			//add the item to new cluster
			answer[ClusterIndex[i]-1][1] = answer[ClusterIndex[i]-1][1] + 1;
			*/
			ClusterIndex[i] = minClusterIndex;
		}

		for(int i = 0 ; i<N; i++){
			changeAdd[ClusterIndex[i]-1] = changeAdd[ClusterIndex[i]-1] + 1;
		}

		for(int i = 0 ; i<20; i++){
			answer[i][1] = changeAdd[i];
		}

		//sanity check
		int sum = 0;
		for(int i = 0; i < 20 ; i++){
			sum = sum + answer[i][1];
		}
		if(sum!=N){
			cout << "Error" << endl;
			break;
		}


		if(change==0){
			break;
		}

		//update means
		for(int i = 0; i<20 ; i++){
			float xcordsum = 0;
			float ycordsum = 0;
			for(int j=0; j<N; j++){
				if(ClusterIndex[j] == answer[i][0]){
					xcordsum = xcordsum + input_data[j][0];
					ycordsum = ycordsum + input_data[j][1];
				}
			}
			answer[i][2] = xcordsum/answer[i][1];
			answer[i][3] = ycordsum/answer[i][1];
			means[i][0] = answer[i][2];
			means[i][1] = answer[i][3];
		}

		//update no of iterations
		itertaken = itertaken+1;
	}	

	//sum of distances
	float sum = 0;
	float distsq = 0;
	for(int i = 0; i<N; i++){
		for(int j = 1; j<=20 ; j++){
			if(ClusterIndex[i] == j){
				distsq = SqEuclidDist(input_data[i][0],input_data[i][1],answer[j-1][2],answer[j-1][3]);
				sum = sum + (distsq);
			}
		}
	}

	varC.noIterations = itertaken;
	varC.minDistanceSq = sum;
	varC.clusters = answer;
	return(varC);
}

/*
Classification replicate(int noTimes, int** input_data){
	float** start = kmeanspp(input_data);
	Classification answer = Classify(input_data,start);
	for(int i = 1; i<noTimes; i++){
		float** start = kmeanspp(input_data);
		Classification answer1 = Classify(input_data,start);
		if(answer1.minDistanceSq < answer.minDistanceSq){
			answer = answer1;
		}
	}	
	return(answer);
}*/

void print(float** answer, string output_file) 
{
	ofstream file(output_file);
	if(file.is_open()){
		for(int i = 0; i < 20; i++){
			file << answer[i][0] << ", " << answer[i][1] << ", " << answer[i][2] << ", " << answer[i][3] << endl;
		}
		file.close();
	}
}

int main(void){
	int** input_data = load_data("data_20k.csv");

	
	float** start = initialMeans4();
	/*
	cout << "time taken for sequential" << endl;
	for(int i=0; i<5; i++){
		float** start = initialMeans4();
		auto startTime = high_resolution_clock::now();
		Classification answer = Classify(input_data,start);
		auto stopTime = high_resolution_clock::now();  //done by me
		auto duration = duration_cast<milliseconds>(stopTime - startTime); //done by me
		cout<< duration.count() << endl;
	}

	cout << endl;*/
	
	auto startTime = high_resolution_clock::now();
	Classification answer = Classify(input_data,start);
	auto stopTime = high_resolution_clock::now();  //done by me
	auto duration = duration_cast<milliseconds>(stopTime - startTime); //done by me
	//cout<< "time taken: " << duration.count() << endl;
	//cout<< "iterations taken: " << answer.noIterations << endl;
	//cout << "Total distance: " << answer.minDistanceSq << endl;
	for(int i=0; i<20;i++){
		cout<< answer.clusters[i][0] << ", " << answer.clusters[i][1] << ", " << answer.clusters[i][2] << ", " << answer.clusters[i][3] << endl;
	}
}