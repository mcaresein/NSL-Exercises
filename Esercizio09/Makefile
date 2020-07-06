CC = g++
CFLAGS = -Wall -O3 --std=c++11

GeneticTSP : main.o statistics.o genetic.o random.o
	$(CC) random.o genetic.o statistics.o main.o -o GeneticTSP
main.o : main.cpp
	$(CC) -c main.cpp -o main.o $(CFLAGS)
statistics.o : statistics.cpp statistics.h
	$(CC) -c statistics.cpp -o statistics.o $(CFLAGS)
genetic.o : genetic.cpp genetic.h
	$(CC) -c genetic.cpp -o genetic.o $(CFLAGS)
random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)
clean :
	rm *.o GeneticTSP seed.out
