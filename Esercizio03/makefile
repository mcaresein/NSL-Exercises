CC = icpc
CFLAGS = -Wall -O3 --std=c++11 -isysroot /Library/Developer/CommandLineTools/SDKs/MacOSX10.14.sdk

main.exe : main.o random.o gbm.o European.o statistics.o
	$(CC) main.o random.o gbm.o European.o statistics.o -o main.exe
main.o : main.cpp
	$(CC) -c main.cpp -o main.o $(CFLAGS)
random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)
European.o : European.cpp European.h
	$(CC) -c European.cpp -o European.o $(CFLAGS)
gbm.o : gbm.cpp gbm.h
	$(CC) -c gbm.cpp -o gbm.o $(CFLAGS)
statistics.o : statistics.cpp statistics.h
	$(CC) -c statistics.cpp -o statistics.o $(CFLAGS)

clean :
	rm *.o main.exe seed.out
