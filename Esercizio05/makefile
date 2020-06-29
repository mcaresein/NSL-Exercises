CC = icpc
CFLAGS = -Wall -O3 --std=c++11 -isysroot /Library/Developer/CommandLineTools/SDKs/MacOSX10.14.sdk

main.exe : main.o random.o metropolisRW.o metropolis1s.o metropolis2p.o statistics.o
	$(CC) main.o random.o metropolisRW.o metropolis1s.o metropolis2p.o statistics.o -o main.exe
main.o : main.cpp
	$(CC) -c main.cpp -o main.o $(CFLAGS)
random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)
metropolisRW.o : metropolisRW.cpp metropolisRW.h
	$(CC) -c metropolisRW.cpp -o metropolisRW.o $(CFLAGS)
metropolis1s.o : metropolis1s.cpp metropolis1s.h
	$(CC) -c metropolis1s.cpp -o metropolis1s.o $(CFLAGS)
metropolis2p.o : metropolis2p.cpp metropolis2p.h
	$(CC) -c metropolis2p.cpp -o metropolis2p.o $(CFLAGS)
statistics.o : statistics.cpp statistics.h
	$(CC) -c statistics.cpp -o statistics.o $(CFLAGS)

clean :
	rm *.o main.exe seed.out
