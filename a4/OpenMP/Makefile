CC=g++ -std=c++11
XCC= -fopenmp

mykmeans.o: mykmeans.cpp
	$(CC) -o mykmeans.o mykmeans.cpp
	$(CC) $(XCC) -o kmeansparallel_stat.o kmeansparallel_stat.cpp
	$(CC) $(XCC) -o kmeansparallel_dyn.o kmeansparallel_dyn.cpp

clean:
	rm -r mykmeans.o
	rm -r kmeansparallel_stat.o
	rm -r kmeansparallel_dyn.o

