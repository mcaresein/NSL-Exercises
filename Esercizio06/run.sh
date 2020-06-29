#!/bin/bash
echo "Helper per lanciare la simulazione"
echo "Fase di equilibrazione di 250 blocchi divisa in 5 restart"
cp input_false.dat input.dat
./Monte_Carlo_ISING_1D.exe > /dev/null;

cp input_true.dat input.dat
for (( i = 0; i < 4; i++ )); do
   ./Monte_Carlo_ISING_1D.exe > /dev/null;
   sleep 1
done

echo "Lunga simulazione all'equilibrio."
cp input_long.dat input.dat
./Monte_Carlo_ISING_1D.exe > /dev/null;
