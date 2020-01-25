class IDictionary
{
public:
	virtual void insert(int , char* ) = 0;
 	virtual char* lookup(int) = 0;
 	virtual void printTable(char* ) = 0;
};