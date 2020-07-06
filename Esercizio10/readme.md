Every subfolder contains each own code. They're similar in structure, the only difference being in the parallelization capability for the Exercise10.2.
I refer to the [readme](https://github.com/mcaresein/NSL-Exercises/blob/master/Esercizio09/readme.md) in Exercise09 for the provided methods.

This code implements simulated annealing in order to solve TSP problem.
An object roadbook is annealed through progressive cooling. The number of moves per temperature is set in the input file. 
The code unevenly scales temperatures keeping the number of moves per temperatures fixed.

The parallel code have to be run with the number of paths multiple of the number of MPI process created. 
The program prevent the user to run it otherwise, in order to maximize load balance between processes and avoid syncronization issues.
