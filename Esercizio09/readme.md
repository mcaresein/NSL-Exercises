This folder contains GeneticTSP code.

There is a self explaining input file. This code is quite verbose and can be forced to display only important informations doing `./GeneticTSP > /dev/null`.
If stdout is not redirected, it will print  crossover and/or mutation every pair of paths has undergone.
I heartly suggest to set verbosity to 1 only for debug pourposes.

---
This code provides the following objects declared in genetic.h and implemented genetic.cpp :
1) Path: it's a class derived from vector, which implements all the requested mutations and some utility methods;
2) Roadbook: it's a class derived from path; it's a collection of paths, and basically implements the crossover and mutation method between its data members;
3) City: simple struct containing coordinates of a city;
4) Sehenswurdigkeiten( which is something like 'list of point of interests' in German): is a list of cities; among it's method there are calculation of $L^1$ and $L^2$ distances;

Moreover, there are some helper functions in the file main.cpp; expecially, there is the selection operator.
