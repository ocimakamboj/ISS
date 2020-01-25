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
#include <math.h>
#include <limits>
#include <ctime>
#define N 20000
using namespace std;


__global__ void reduce(int *g_idata, int* g_odata)
{
	static __shared__ int sdata[1024];

	// load shared mem
	unsigned int tid = threadIdx.x;
	unsigned int i =   blockIdx.x*blockDim.x + threadIdx.x;

	if(tid<544){
		for(int j=0; j<20 ; j++){
			sdata[tid] = sdata[tid] + g_idata[i+1024*j];
		}
	}else{
		for(int j=0; j<19 ; j++){
			sdata[tid] = sdata[tid] + g_idata[i+1024*j];
		}
	}
	
	__syncthreads();

	// do reduction in shared mem
	for(unsigned int s=1; s < blockDim.x; s *= 2) {
		if ((tid % (2*s)) == 0){
			sdata[tid] += sdata[tid + s];
		}
		__syncthreads();
	}

	// write result for this block to global mem
	if (tid == 0){
		g_odata[blockIdx.x] = sdata[tid];
	}
}

int* load_data1(){
	int* input_data = new int[N];

	for(int i=0;i<N;i++){
		input_data[i] = i;
		
	}
	return(input_data);
}

int* load_data(const char* input_file){
	int* answer = new int[N];
	FILE* fp = fopen(input_file,"r"); 
	int element;

	int i = 0;
	while( fscanf(fp, "%d", &element) != EOF )
    {
        answer[i] = element;
        i++;
    }
    fclose(fp);
    return(answer);
}

int main(){
	cudaEvent_t start, stop;
 	float time;
 	cudaEventCreate(&start);
 	cudaEventCreate(&stop);

	int* h_idata;
	int* h_odata; /* host data*/
	int* d_idata;
	int* d_odata; /* device data*/

	int numThreadsperBlock = 1024;
	int numBlocks =  1;
	int size1 = N * sizeof( int );
	int size2 = numBlocks * sizeof(int);

	cudaMalloc( (void **) &d_idata, size1 );
	cudaMalloc( (void **) &d_odata, size2);
	h_idata = (int *)malloc( size1 );
	h_odata = (int *)malloc( size2);

	const char* name = "reduce_data.txt";

	h_idata = load_data(name);
	
	/* copying inputs to device memory */
	cudaMemcpy(d_idata, h_idata, size1, cudaMemcpyHostToDevice) ;
	cudaMemcpy(d_odata, h_odata, size2, cudaMemcpyHostToDevice) ;

	dim3 dimBlock(numThreadsperBlock, 1, 1); dim3 dimGrid(numBlocks, 1, 1);

	cudaEventRecord(start, 0);
	reduce<<< dimGrid, dimBlock >>>(d_idata, d_odata);
	cudaEventRecord(stop, 0);
 	cudaEventSynchronize(stop);

	int result;
	cudaMemcpy( h_odata, d_odata, size2, cudaMemcpyDeviceToHost );
	cudaMemcpy( &result, d_odata, size2, cudaMemcpyDeviceToHost );
	cout << "sum is " << result << endl;

	cudaEventElapsedTime(&time, start, stop);
 	printf ("Time for the kernel: %f ms\n", time);

	free(h_idata);
	free(h_odata);
	cudaFree(d_idata);
	cudaFree(d_odata);
}