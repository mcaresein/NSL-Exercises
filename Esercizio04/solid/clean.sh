#!/bin/fish
rm -rf *epot*
rm -rf *ekin*
rm -rf *etot*
rm -rf *temp*
rm old.0 old.final
rm -rf frames/*.xyz
cp config.fcc config.0
cat input_false.dat > input.dat
