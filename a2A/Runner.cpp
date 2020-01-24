#include <iostream>
#include <string.h>
#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include <iomanip>
#include"MatrixImpl.cpp"
//selfstart
#include <fstream>
#include <sstream>
#include <list>
#include <utility> 
#include <chrono> 
#include <math.h>
using namespace std::chrono; 
//selfend

using namespace std;

//selfstart
//The following piece of code was taken from Stackoverflow, this includes the function parseLine, getValue(), and getValue1().
int parseLine(char* line){
    // This assumes that a digit will be found and the line ends in " Kb".
    int i = strlen(line);
    const char* p = line;
    while (*p <'0' || *p > '9') p++;
    line[i-3] = '\0';
    i = atoi(p);
    return i;
}

int getValue(){ //Note: this value is in KB!
    FILE* file = fopen("/proc/self/status", "r");
    int result = -1;
    char line[128];

    while (fgets(line, 128, file) != NULL){
        if (strncmp(line, "VmSize:", 7) == 0){
            result = parseLine(line);
            break;
        }
    }
    fclose(file);
    return result;
}

int getValue1(){ //Note: this value is in KB!
    FILE* file = fopen("/proc/self/status", "r");
    int result = -1;
    char line[128];

    while (fgets(line, 128, file) != NULL){
        if (strncmp(line, "VmRSS:", 6) == 0){
            result = parseLine(line);
            break;
        }
    }
    fclose(file);
    return result;
}
//selfend

/**
  * Runner code that serves as harness for invoking various functions required 
  * to be implemented for this assignment.
  * You may modify this code, but need to retain the input parameter signature and 
  * output requirements specified in the assignment.
  */  
  
/////////////////////////////////////////////////////////////////////////////

IMatrix* load_arr(char* input_file) 
{
	// TODO: Code for loading matrix from input_file into array impl and returning object
	//selfstart
	IMatrix* matrix = new ArrayMatrix();
	
	ifstream inputdata(input_file); //stores all the input data
	if (inputdata.fail()) {
		throw(logic_error("Error : File not Found for Loading"));
	}
	string line;
	getline(inputdata, line); //stores one line at a time from inputdata to line
	stringstream ss(line); //scans the line
	string linedata;
	getline(ss,linedata,'\t'); //stores the elements of linedata until tab is encountered
	int noRows = stoi(linedata);
	getline(ss,linedata,'\t');
	int noColumns = stoi(linedata);
	
	matrix->init(noRows,noColumns);
	int rowcounter = 0; //to keep track of number of rows in the input file
	
	while (getline(inputdata, line)){
		stringstream ss(line);
		string linedata;
		float* row = new float[noColumns];
		int index = 0;

		while (getline(ss,linedata,'\t')){
			row[index] = stof(linedata);
			index++;
		}
		if (index != noColumns){
			throw(logic_error("Error : inconsistent number of columns in input file"));
		}
		rowcounter++;
		try{
			matrix->append(row);
		}catch(out_of_range){
			throw(logic_error("Error : inconsistent number of rows in input file. rows more than specified"));
		}
		delete [] row;
	}
	
	if(rowcounter != noRows){
		throw(logic_error("Error : inconsistent number of rows in input file"));
	}
	return(matrix);
	//selfend

}

IMatrix* load_csr(char* input_file) 
{
	// TODO: Code for loading matrix from input_file into CSR impl and returning object
	//selfstart
	IMatrix* matrix = new CSRMatrix();
	
	ifstream inputdata(input_file);
	if (inputdata.fail()) {
		throw(logic_error("Error : File not Found"));
	}
	string line;
	getline(inputdata, line);
	stringstream ss(line);
	string linedata;
	getline(ss,linedata,'\t');
	int noRows = stoi(linedata);
	getline(ss,linedata,'\t');
	int noColumns = stoi(linedata);

	matrix->init(noRows,noColumns);
	int rowcounter = 0;
	
	while (getline(inputdata, line)){
		stringstream ss(line);
		string linedata;
		float* row = new float[noColumns];
		int index = 0;

		while (getline(ss,linedata,'\t')){
			row[index] = stof(linedata);
			index++;
		}
		if (index != noColumns){
			throw(logic_error("Error : inconsistent number of columns in input file"));
		}
		rowcounter++;
		try{
			matrix->append(row);
		}catch(out_of_range){
			throw(logic_error("Error : inconsistent number of rows in input file. rows more than specified"));
		}
		delete[] row;
	}
	if(rowcounter != noRows){
		throw(logic_error("Error : inconsistent number of rows in input file. rows less than specified"));
	}
	return(matrix);
	//selfend

}

IMatrix* init_arr(int rows, int cols) 
{
	// TODO: Code for initializing an empty matrix using array impl with rows and cols as 
	// dimensions, and returning the object
	//selfstart
	IMatrix* matrix = new ArrayMatrix();
	matrix -> init(rows,cols);
	return(matrix);
	//selfend
}


IMatrix* init_csr(int rows, int cols) 
{
	// TODO: Code for initializing an empty matrix using CSR impl with rows and cols as 
	// dimensions, and returning the object
	//selfstart
	IMatrix* matrix = new CSRMatrix();
	matrix -> init(rows,cols);
	return(matrix);
	//selfend
}


void print_mat(IMatrix* mat, char* output_file) 
{
	// TODO: print matrix as TSV to otput_file
	//selfstart
	ofstream file(output_file);
	if(file.is_open()){
		int noRows = mat->row_count();
		int noColumns = mat->col_count();
		file << noRows << '\t' << noColumns << endl;

		for(int i = 0; i < noRows; i++){
			for(int j = 0; j < noColumns; j++){
				float value = trunc((mat -> get(i,j))*1000)/1000;
				//float value = mat -> get(i,j);
				if(j<(noColumns-1)){
					file << value << '\t';
				}else{
					file << value << endl;
				}	
			}
		}
		file.close();
	}
	//selfend
}

/////////////////////////////////////////////////////////////////////////////

void load(char* mat_type, char* input_file, char* output_file)
{
	// TODO: any other validation?
	
	IMatrix* mat1;

	if (strcmp("array", mat_type)==0) // TODO: time this region and print "load,array,output_file,time_millisec"
    { 
    	auto start = high_resolution_clock::now(); //done by me
		mat1 = load_arr(input_file);
		auto stop = high_resolution_clock::now();  //done by me
		auto duration = duration_cast<milliseconds>(stop - start); //done by me
		cout << "load,array," << output_file << "," << duration.count() << "_millisec" <<endl; //done by me
    }  
	else if (strcmp("csr", mat_type)==0) // TODO: time this region and print "load,csr,output_file,time_millisec"
	{  
		auto start = high_resolution_clock::now(); //done by me
		mat1 = load_csr(input_file);
		auto stop = high_resolution_clock::now();  //done by me
		auto duration = duration_cast<milliseconds>(stop - start); //done by me
		cout << "load,csr," << output_file << "," << duration.count() << "_millisec" <<endl; //done by me
	}
	else
		cout<<"[load] invalid matrix type input seen: "<<mat_type<<endl;

	// store the loaded matrix mat1 in output file given by output_file
	print_mat(mat1, output_file);
	

	return;
}

/////////////////////////////////////////////////////////////////////////////

void add(IMatrix* mat1, IMatrix* mat2, IMatrix* mat3) 
{	
	// TODO: Code for adding the mat1 and mat2 and storing in a third matrix mat3
	//selfstart
	int noRows = mat1 -> row_count();
	int noColumns = mat1 -> col_count();
	float* row = new float[noColumns]; 
	
	for(int i=0; i<noRows; i++){
		for(int j=0; j<noColumns; j++){
			row[j] = mat1 -> get(i,j) + mat2 -> get(i,j);
		}
		mat3 -> append(row);
	}
	delete [] row;
	//selfend
	
	return;
}

void add(char* mat_type, char* input_file1, char* input_file2, char* output_file)
{ 
	// TODO: any other validation?
	
	//IMatrix* mat1, mat2, mat3; original line changed because of compile error
	//selfstart
	IMatrix * mat1,* mat2,* mat3;
	//selfend

	if (strcmp("array", mat_type)==0) 
    { 
		mat1 = load_arr(input_file1);
		mat2 = load_arr(input_file2);
		
		// TODO: any other validation?
		//selfstart
		int noRowsM1 = mat1 -> row_count();
		int noColumnsM1 = mat1 -> col_count();
		int noRowsM2 = mat2 -> row_count();
		int noColumnsM2 = mat2 -> col_count();
		if(noRowsM1!=noRowsM2 || noColumnsM1!=noColumnsM2){
			throw(logic_error("Error : Dimenensions not compatible for addition"));
		}
		//selfend
		
		mat3 = init_arr(mat1->row_count(), mat1->col_count());
    }  
	else if (strcmp("csr", mat_type)==0)
	{
		mat1 = load_csr(input_file1);
		mat2 = load_csr(input_file2);

		// TODO: any other validation?
		//selfstart
		int noRowsM1 = mat1 -> row_count();
		int noColumnsM1 = mat1 -> col_count();
		int noRowsM2 = mat2 -> row_count();
		int noColumnsM2 = mat2 -> col_count();
		if(noRowsM1!=noRowsM2 || noColumnsM1!=noColumnsM2){
			throw(logic_error("Error : Dimenensions not compatible for addition"));
		}
		//selfend
		
		mat3 = init_csr(mat1->row_count(), mat1->col_count());
	}
	else {
		cout<<"[add] invalid matrix type input seen: "<<mat_type<<endl;
		return;
	}
	
	// TODO: time this method and print "add,mat_type,output_file,time_millisec"
	
	auto start = high_resolution_clock::now(); //done by me
	add(mat1, mat2, mat3);
	auto stop = high_resolution_clock::now();  //done by me
	auto duration = duration_cast<milliseconds>(stop - start); //done by me
	cout << "add,"<<mat_type<< "," << output_file << "," << duration.count() << "_millisec" <<endl; //done by me
	
	// store output matrix mat3 in file given by output_file
	print_mat(mat3, output_file);

	return;
}

/////////////////////////////////////////////////////////////////////////////

void multiply(IMatrix* mat1, IMatrix* mat2, IMatrix* mat3) 
{	
	
	// TODO: Code for adding the mat1 and mat2 and storing in a third matrix mat3
	//selfstart
	int noColumns = mat2 -> col_count();
	float* row = new float[noColumns];

	for(int i=0; i<mat1 -> row_count(); i++){	
		for(int i = 0; i<noColumns; i++){
			row[i] = 0;
		}
		for(int j=0; j<mat2 -> col_count(); j++){
			for(int k=0; k<mat1 -> col_count(); k++){
				row[j] = row[j] + mat1->get(i,k)*mat2->get(k,j);
			}
		}
		mat3 -> append(row);
	}
	delete [] row;
	//selfend
	
	return;
}

void multiply(char* mat_type, char* input_file1, char* input_file2, char* output_file)
{
	// TODO: any other validation?

	//IMatrix* mat1, mat2, mat3; original line changed because of compile error
	//selfstart
	IMatrix * mat1,* mat2,* mat3;
	//selfend

	if (strcmp("array", mat_type)==0) 
    { 
		mat1 = load_arr(input_file1);
		mat2 = load_arr(input_file2);

		// TODO: any other validation?
		//selfstart
		int noColumnsM1 = mat1 -> col_count();
		int noRowsM2 = mat2 -> row_count();
		if(noColumnsM1!=noRowsM2){
			throw(logic_error("Error : Dimenensions not compatible for multiplication"));
		}
		//selfend
		// TODO: init output matrix mat3 with arr impl
		//selfstart
		mat3 = init_arr(mat1->row_count(), mat2->col_count());
		//selfend
	}  
	else if (strcmp("csr", mat_type)==0)
	{
		mat1 = load_csr(input_file1);
		mat2 = load_csr(input_file2);

		// TODO: any other validation?
		//selfstart
		int noColumnsM1 = mat1 -> col_count();
		int noRowsM2 = mat2 -> row_count();
		if(noColumnsM1!=noRowsM2){
			throw(logic_error("Error : Dimenensions not compatible for multiplication"));
		}
		//selfend
		// TODO: init output matrix mat3 with csr impl
		//selfstart
		mat3 = init_csr(mat1->row_count(), mat2->col_count());
		//selfend
	}
	else {
		cout<<"[multiply] invalid matrix type input seen: "<<mat_type<<endl;
		return;
	}
	
	// TODO: time this method and print "multiply,csr,output_file,time_millisec"

	auto start = high_resolution_clock::now(); //done by me
	multiply(mat1, mat2, mat3);
	auto stop = high_resolution_clock::now();  //done by me
	auto duration = duration_cast<milliseconds>(stop - start); //done by me
	cout << "multiply,"<<mat_type<< "," << output_file << "," << duration.count() << "_millisec" <<endl; //done by me
	
	// store output matrix mat3 in file given by output_file
	print_mat(mat3, output_file);

	return;
}

/////////////////////////////////////////////////////////////////////////////

void bfs(char* input_file, char* root_id, char* output_file)
{
 
	// TODO: any validation?
	
	IMatrix* mat1;
	// TODO: Define a List ADT traverse_list to store output.
	//selfstart
	list<pair<int,list<int> > > traverse_list;
	//selfend

	// TODO
	mat1 = load_csr(input_file);
	//selfstart
	if(mat1 -> row_count() != mat1 -> col_count()){
		throw(logic_error("Error : Invalid input. Rows not equal to Columns"));
	}
	//selfend

	//selfstart
	pair<int,list<int> > varP; //variable for pair type
	list<int> Queue; //Queue for BFS
	int noVertices = mat1 -> row_count();
	int * visited = new int[noVertices]; //array for flagging vertices that have been visited
	for(int j=0; j<noVertices; j++){
		visited[j] = 0;
	}
	int * level = new int[noVertices]; //array for storing levels
	for(int j=0; j<noVertices; j++){
		level[j] = 0;
	}
	int source = (root_id[0]) - '0';
	if(source < 0 || source>=noVertices){
		throw(logic_error("Error : invalid input for source vertex"));
	}
	//initialising BFS
	Queue.push_back(source);
	visited[source] = 1;
	level[source] = 0;
	varP.first = 0;
	varP.second.push_back(source);
	//selfend

	{	
		// TODO: time this region and print "bfs,csr,output_file,time_millisec"
		// TODO: Code for doing BFS on the matrix starting with vertex present in row "root_id"
		// TODO: Add traversed items into traverse_list
		//selfstart
		auto start = high_resolution_clock::now(); //done by me
		while(!Queue.empty()){
			int v = Queue.front();
			Queue.pop_front();
			for(int j=0; j<noVertices; j++){
				if(mat1 -> get(v,j) == 1){
					int neighbour = j;
					if(visited[neighbour]==0){
						Queue.push_back(neighbour);
						visited[neighbour] = 1;
						level[neighbour] = level[v] + 1;
						if(varP.first == level[neighbour]){
							varP.second.push_back(neighbour);
						}else{
							varP.second.sort();
							traverse_list.push_back(varP);
							varP.first = level[neighbour];
							varP.second.clear();
							varP.second.push_back(neighbour);
						}
					}
				}
			}
		}
		traverse_list.push_back(varP);
		auto stop = high_resolution_clock::now();  //done by me
		auto duration = duration_cast<milliseconds>(stop - start); //done by me
		cout << "bfs,csr," << output_file << "," << duration.count() << "_millisec" <<endl; //done by me
		//selfend
	}
	
	// TODO: store traversal output present in traverse_list into file given by output_file
	//selfstart
	ofstream file(output_file);
	if(file.is_open()){
		for(list<pair<int,list<int> > > :: iterator it = traverse_list.begin(); it!=traverse_list.end(); it++){
			varP = *it;
			file << varP.first;
			for(list<int> :: iterator it2 = varP.second.begin(); it2!=varP.second.end(); it2++){
				file << ", " << *it2;
			}
			file << endl;
		}
		file.close();
	}
	delete [] visited;
	delete [] level;
	//selfend
	

	return;

}


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

int main(int n, char *argv[])
{

	if(strcmp("load", argv[1])==0)
	{
		load(argv[2], argv[3], argv[4]);
		
	}
    else if( strcmp("add", argv[1])==0)
	{
		add(argv[2], argv[3], argv[4], argv[5]);
	}
    else if( strcmp("multiply", argv[1])==0 )
	{
        multiply(argv[2], argv[3], argv[4], argv[5]);
	}
    else if(strcmp("bfs", argv[1])==0)
	{
        bfs(argv[2], argv[3], argv[4]);
	} else 
		cout<<"[main] invalid input parameters. Valid usage is..."<<endl;
	
	/*cout << "virtual memory used : " << getValue()/1000 << " mb" << endl;	
	cout << "physical memory used : " << getValue1()/1000 << " mb"  << endl;*/	
	return 0;
    
}

