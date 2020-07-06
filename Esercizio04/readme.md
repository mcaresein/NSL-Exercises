This folder contains the code Moldyn. There are 3 input files:
- input_false.dat has restart set false.
- input_true.dat has restart set true.
- input_true_measurement.dat has restart set true and runs a longer simulation for measurements pourpose.
The code reads input.dat, so each input as to be copied or concatenated to input.dat.  

Each phase subfolder has all 3 inputs, with custom thermodynamic parameters for the specific phase.

There are some helpers:
- clean.sh cleans all output and set input_false.dat as input.dat
- restart.sh set restart to true and run a simulation. 

Each phase subfolder has both scripts.

The code is meant to be used in this way:
1) `cd <phase folder>`
2) `./clean.sh`
3) `../Moldyn`
4) `./restart.sh` until ready for measurement
5) `cp input_true_measurement.dat input.dat && ../Moldyn` 
