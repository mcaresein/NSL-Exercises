This code contains a Variational MonteCarlo code to find the ground of a 1d quantum particle in a double well, with a trial function as a sum of shifted gaussians.
The code is structured as follow:
1) metropolisRW: is a base class for a metropolis algorithm for a random walk. It has some utility methods and force any derived to implement a gaussian step and a uniform step;
2) metropolis_trial: derives from RW. implements proposition of new moves with uniform distribution, and has trial squared wavefunction as acceptance probability;

The code is meant to be run by the script VariateParameters.sh, which save in the file energies.dat(append, not overwrite)

**Warning**: it rapidly eat your disk space by saving the optimal wavefunction found for each of the pair of the parameters. 

QMC_1D contains data from a PIGS/PIMC code for comparison.
vmc_data contains data shown in the notebook
