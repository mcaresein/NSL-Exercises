#!/bin/bash
# useful definitions
echoerr() { echo -e "$@" 1>&2; }
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

echored() { echoerr  "${RED}$@${NC}"; }
echogreen() { echoerr "${GREEN}$@"${NC}; }
echored() { echoerr "${NC}$@${NC}"; }

#explicitly exit if a command fails
set -e

#main

echogreen Helper to run simulation at different parameters mu and sigma

[ ! -f main.exe ] &&  echored Executable not found! Exit && exit 1

MU_MAX=0.9
MU_MIN=0.3
MU_STEP=0.01

SIGMA_MAX=0.9
SIGMA_MIN=0.5
SIGMA_STEP=0.01

for mu in `seq $MU_MIN $MU_STEP $MU_MAX`
do
   for sigma in `seq $SIGMA_MIN $SIGMA_STEP $SIGMA_MAX`
   do
      echogreen  Simulation with parameters mu = $mu and sigma = $sigma
      ./main.exe $mu $sigma 1 > output_$mu\_$sigma
      echo $mu $sigma $(tail -1 energy) >> energies.dat 
   done
done
RESULTS=($(awk '{print $4, $1, $2}' energies.dat | sort -n | head -1))

echogreen Optimal parameters are: mu=${RESULTS[1]}, sigma=${RESULTS[2]}
echogreen Done!
