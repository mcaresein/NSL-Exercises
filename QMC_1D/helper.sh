#!/bin/bash

T_MIN=.1
T_MAX=10.1
T_SEQ=.3

for i in $(seq $T_MIN $T_SEQ $T_MAX); do
	./qmc1d
	mkdir -p t_$i
	cp *.dat t_$i/
	sed -i '2 s/'$i'/'$(echo "$i+$T_SEQ" |bc|awk '{printf "%f", $0}')'/' input.dat
done	

