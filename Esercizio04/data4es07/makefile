CC = g++
CFLAGS = -Wall -O3 --std=c++11

Moldyn : Moldyn_NVE.o
	$(CC) Moldyn_NVE.o -o Moldyn
Moldyn_NVE.o : Moldyn_NVE.cpp Moldyn_NVE.h
	$(CC) -c Moldyn_NVE.cpp -o Moldyn_NVE.o $(CFLAGS)
clean :
	rm *.o Moldyn
wipe :
	./clean.sh
