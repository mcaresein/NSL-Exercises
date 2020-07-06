This is a base code to sample wavefunction by means of the metropolis algorithm with gaussian and uniform distributions of the proposed moves.
The code is structured as follow:
1) metropolisRW: is a base class for a metropolis algorithm for a random walk. It has some utility methods and force any derived to implement a gaussian step and a uniform step;
2) metropolis1s: derives from RW. implements proposition of new moves with gaussian and uniform distribution, and has 1s hydrogen squared wavefunction as acceptance probability;
2) metropolis2p: derives from RW. implements proposition of new moves with gaussian and uniform distribution, and has 2p hydrogen squared wavefunction as acceptance probability;

The code is meant to be run as `./main.exe > /dev/null`, which prints to sderr the acceptance rate.
It prints in stdout istantaneous values of R.
It saves Block Averaged values of R in the file specified to the method SaveBlockAverageVariance of the class statistics.

