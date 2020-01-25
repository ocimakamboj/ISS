#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int* load_data(char* input_file){
	static int answer[20000];
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

int reduction(int* input_data){
	int N = 20000;
	int sum = 0;
	for(int i=0; i<N; i++){
		sum = sum + input_data[i];
	}
	return(sum);
}

int main(){
	char* name = "reduce_data.txt";
	int* input_data = load_data(name);
	printf("%d\n", reduction(input_data));
}
/*
int* load_data3(){
	static int answer[20000];
	FILE* fp = fopen("reduce_data.txt","r"); 
	int element;

	int i = 0;
	while( fscanf(fp, "%i", &element) != EOF )
    {
        answer[i] = element;
        //printf("%d\n", answer[i]);
        i++;
    }
    fclose(fp);
    return(answer);
}*/