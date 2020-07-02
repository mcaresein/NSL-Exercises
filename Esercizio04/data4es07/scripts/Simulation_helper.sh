#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

#explicitly exit if a command fails
set -e

echo -e "${GREEN}Helper to run simulation at different temperatures"

[ ! -f $(dirname $(pwd))/MolDyn ] &&  echo -e "${RED}I need to be in a subfolder w.r.t the MolDyn executable.\n A safe choice is to name the subfolder as the phase to be simulated" && exit 1

echo -e "${GREEN}I perform 5 short simulation with restart and then a long one"
echo -e "${GREEN}Output files are in the form output*vstemp ${RED}"

echo -e "${RED} Did you check simulation parameters in input files? ${NC}"
read -p 'Press a key to continue' c

[ ! -f input_false.dat ] && echo -e "${RED}Gimme input_false.dat ${NC}" &&  exit 1

./clean.sh && ../Moldyn

[ ! -f input_true.dat ] && echo -e "${RED}Gimme input_true.dat ${NC}" &&  exit 1


for (( i = 0; i < 5; i++ )); do
   ./restart.sh && ../Moldyn
done

[ ! -f input_true_measurement.dat ] && echo -e "${RED}Gimme input_true_measurement.dat ${NC}" &&  exit 1


./restart.sh && cp input_true_measurement.dat input.dat && ../Moldyn

echo -e "${GREEN}Done!  ${NC}"
