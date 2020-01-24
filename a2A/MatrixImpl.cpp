#include"IMatrix.h"
#include<iostream>

using namespace std;


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/**
 * 2D Matrix Implementation usign Arrays
 */
class ArrayMatrix: public IMatrix
{  
	/*#######################################################
	 # Define your own data structures here 
	 #######################################################*/

	private: 
		// TODO
		//selfstart
		int noRows;
		int noColumns;
		float** matrix;
		int counterAppend; //to keep track of how many rows have been appended;
		int nnz; //to keep track of non zero elements
   		//selfend
	public:
		// TODO
   
   
	/**
     * Default constructor is required.
	 * You may optionally define additional constructors as well.
     */
	ArrayMatrix()
	{ 
		// TODO: Provide implementation for default constructor
		//selfstart
		noRows = 0;
		noColumns = 0;
		counterAppend = 0;
		nnz = 0;
		//selfend
	}
 
 
	/*#######################################################
 	  # Override functions defined in IMatrix interface
	  #######################################################*/

	/** Override the init function defined in the IMatrix interface */
	void init (int N, int M)
 	{ 
		// TODO: Provide implementation for init using array
		//selfstart
		if (noRows > 0 || noColumns > 0){
			throw(logic_error("Error : Matrix already Initialised"));
		}else{
			noRows = N;
			noColumns = M;
			matrix = new float*[N];
			for (int i=0; i<N; i++){
				matrix[i] = new float[M];
				for(int j=0; j<M; j++){
					matrix[i][j] = 0;
				}
			}
		}
		//selfend
	}

	
	/** Override the append function defined in the IMatrix interface */
   void append (float* row_vals)
	{ 
	    // TODO: Provide implementation for append using array
	    //selfstart
		if (counterAppend == noRows){
			throw(out_of_range("Error : Cannot append more rows"));
		}else{
			for(int j=0; j<noColumns ;j++ ){
				matrix[counterAppend][j] = row_vals[j];
				if(row_vals[j] != 0){
					nnz++;
				}
			}
			counterAppend++;
		}
	    //selfend
	}

	
	/** Override the get function defined in the IMatrix interface */
	float get(int i, int j)
	{ 
	    // TODO: Provide implementation for get using array
	    //selfstart
	    if(i>=noRows || j>=noColumns || i<0 || j<0){
	    	throw(out_of_range("Error : invalid input for get function"));
	    }else{
	    	return(matrix[i][j]);
	    }
	    //selfend
	}

	
	/**
	  * This returns the number of rows in the matrix based on init()
	  */
	int row_count()
	{
		// TODO: Provide implementation using array
		//selfstart
		return(noRows);
		//selfend
	}
	
	/**
	  * This returns the number of columns in the matrix based on init()
	  */
	int col_count() 
	{
		// TODO: Provide implementation using array
		//selfstart
		return(noColumns);
		//selfend
	}

	
	/**
	  * This returns the number of non zero values in the matrix
	  */
	int nnz_count()
	{
		// TODO: Provide implementation using array
		//selfstart
		return(nnz);
		//selfend
	}

	/*#######################################################
	 # Optionally define any other private utility functions here
	 #######################################################*/
	
	// TODO
	//selfstart
	void print(){
		for(int i=0; i<noRows; i++){
			for (int j=0; j<noColumns; j++){
				cout << matrix[i][j] << ", ";
			}
			cout << endl;
		}
	}
	/*
	void set(int i, int j, float v)
	{ 
	    //selfstart
	    if(i>=noRows || j>=noColumns){
	    	throw(out_of_range("Error : invalid input for set function"));
	    }else{
	    	matrix[i][j] = v;
	    }
	    //selfend
	}*/
	//selfend
};

/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

/**
 * 2D Matrix Implementation usign CSR
 */
class CSRMatrix: public IMatrix
{  	

	/*#######################################################
	 # Define your own data structures here 
	 #######################################################*/

	private: 
		// TODO
		//selfstart
		int noRows;
		int noColumns;
		int nnz;//no of non zero elements;
		float* Aelements;//stores non zero elements
		int* IArow;//cumulative count of non zero elements in rows
		int* JAcolumn;//column index of non zero elements
		int counterAppend;//to keep track of how many rows have been appended;
   		//selfend
	public:
		// TODO
   
   
	/**
     * Default constructor is required.
	 * You may optionally define additional constructors as well.
     */
	CSRMatrix()
	{ 
		// TODO: Provide implementation for default constructor
		//selfstart
		noRows = 0;
		noColumns = 0;
		nnz = 0;
		counterAppend = 0;
		//selfend
	}
 
 
	/*#######################################################
 	  # Override functions defined in IMatrix interface
	  #######################################################*/

	/** Override the init function defined in the IMatrix interface */
	void init (int N, int M)
 	{ 
		// TODO: Provide implementation for init using csr
		//selfstart
		if(noRows>0 || noColumns>0){
			throw(logic_error("Error : Matrix already Initialised"));
		}else{
			noRows = N;
			noColumns = M;
			Aelements = NULL;
			IArow = new int[N+1];
			for(int i=0; i<=N; i++){
				IArow[i] = 0;
			}
			JAcolumn = NULL;
		}
		//selfend
	}
	

	/** Override the append function defined in the IMatrix interface */
   void append (float* row_vals)
	{ 
	    // TODO: Provide implementation for append using csr
	    //selfstart
	    if (counterAppend == noRows){
			throw(out_of_range("Error : Cannot append more rows"));
		}else{
			int nnzNew = nnz;
			for(int i=0; i<noColumns; i++){
				if(row_vals[i]!=0){
					nnzNew++;
				}
			}
			for(int i = counterAppend + 1; i<=noRows; i++){
				IArow[i] = nnzNew;
			}

			float* AelementsNew = new float[nnzNew];
			int* JAcolumnNew = new int[nnzNew];

			for(int i = 0; i<nnz; i++){
				AelementsNew[i] = Aelements[i];
				JAcolumnNew[i] = JAcolumn[i];
			}

			delete [] Aelements;
			delete [] JAcolumn;

			int index = nnz;
			for(int j = 0; j<noColumns; j++){
				if(row_vals[j] != 0){
					AelementsNew[index] = row_vals[j];
					JAcolumnNew[index] = j;
					index++; 
				}
			}
			Aelements = AelementsNew;
			JAcolumn = JAcolumnNew;
			nnz = nnzNew;
			counterAppend++;
		}
		//selfend
	}
	
	/** Override the get function defined in the IMatrix interface */
	float get(int i, int j)
	{ 
	    // TODO: Provide implementation for get using csr
	    //selfstart
	    if(i>=noRows || j>=noColumns || i<0 || j<0){
	    	throw(out_of_range("Error : invalid input for get function"));
	    }
	    else{
	    	float answer = 0;
	    	int start = IArow[i];
	    	int end = IArow[i+1] - 1;
	    	int index = BinarySearch(JAcolumn, start, end, j);
	    	if(index != -1){
	    		answer = Aelements[index];
	    	}
	    	/*
	    	for(int index = IArow[i]; index < IArow[i+1] ; index++){
	    		if(JAcolumn[index] == j){
	    			answer = Aelements[index];
	    		}
	    	}
	    	*/
	    	return(answer);
	    }
		//selfend
	}

	/**
	  * This returns the number of rows in the matrix based on init()
	  */
	int row_count()
	{
		// TODO: Provide implementation using csr
		//selfstart
		return(noRows);
		//selfend
	}
	
	/**
	  * This returns the number of columns in the matrix based on init()
	  */
	int col_count() 
	{
		// TODO: Provide implementation using csr
		//selfstart
		return(noColumns);
		//selfend
	}

	
	/**
	  * This returns the number of non zero values in the matrix
	  */
	int nnz_count()
	{
		// TODO: Provide implementation using csr
		//selfstart
		return(nnz);
		//selfend
	}

	
	/*#######################################################
	 # Optionally define any other private utility functions here
	 #######################################################*/
	
	// TODO
	//selfstart
	//takes an array input and finds v between start and end, returns the index
	int BinarySearch(int *Array, int start, int end, int value){
		if(start<=end){
			int mid = (int)(start+end)/2;
			if(Array[mid] == value){
				return(mid);
			}else if(Array[mid] > value){
				return BinarySearch(Array, start, mid - 1, value);
			}else{
				return BinarySearch(Array, mid+1, end, value);
			}
		}
		return(-1);
	}

	void print(){
		cout << "array A of non zero elements: " << endl;
		for(int i=0; i<nnz; i++){
			cout << Aelements[i] << ", ";
		}
		cout<<endl;
		cout << "array JA of columns of non zero elements: " << endl;
		for(int i=0; i<nnz; i++){
			cout << JAcolumn[i] << ", ";
		}
		cout<<endl;
		cout << "array IA of cumulative row count: " << endl;
		for(int i=0; i<=noRows; i++){
			cout << IArow[i] << ", ";
		}
		cout<<endl;

	}
	//selfend

	
};