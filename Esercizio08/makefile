CC = icpc
CFLAGS = -Wall -O3 --std=c++11 -isysroot /Library/Developer/CommandLineTools/SDKs/MacOSX10.14.sdk

main.exe : main.o random.o metropolisRW.o metropolis_trial.o statistics.o
	$(CC) main.o random.o metropolisRW.o metropolis_trial.o statistics.o -o main.exe
main.o : main.cpp
	$(CC) -c main.cpp -o main.o $(CFLAGS)
random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)
metropolisRW.o : metropolisRW.cpp metropolisRW.h
	$(CC) -c metropolisRW.cpp -o metropolisRW.o $(CFLAGS)
metropolis_trial.o : metropolis_trial.cpp metropolis_trial.h
	$(CC) -c metropolis_trial.cpp -o metropolis_trial.o $(CFLAGS)
statistics.o : statistics.cpp statistics.h
	$(CC) -c statistics.cpp -o statistics.o $(CFLAGS)

clean :
	rm *.o main.exe seed.out
