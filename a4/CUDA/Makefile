CC=nvcc

reduction.o: reduction.cu
	$(CC) -o reduction.o reduction.cu
	$(CC) -o reductionNS.o reductionNS.cu

clean:
	rm -r reduction.o
	rm -r reductionNS.o

