#!/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

echo -e "${GREEN}Helper to run simulation at different temperatures"
echo -e "${RED}I need the script run in the same folder i'm run from"
echo -e "${GREEN}T ranges from 0.2 to 3 in 100 steps"
echo -e "${GREEN}Output files are in the form output*vstemp ${RED}"
rm -i output*vstemp

echo "# Temperature ----- Mean Value ----- Variance" >  output_enevstemp
echo "# Temperature ----- Mean Value ----- Variance" >  output_magvstemp
echo "# Temperature ----- Mean Value ----- Variance" >  output_heatvstemp
echo "# Temperature ----- Mean Value ----- Variance" >  output_chivstemp

#tstep=0.028;
tstep=0.056
new_temperature=0.2;
for (( Ts = 1; Ts < 51; Ts++ )); do
   new_temperature=$(echo "$new_temperature+$tstep" | bc);

   echo -e "${GREEN}Simulazione a T = $new_temperature ${NC}"
   sed  -i '1 s/^.*$/'$new_temperature'/' input_false.dat
   sed  -i '1 s/^.*$/'$new_temperature'/' input_true.dat
   sed  -i '1 s/^.*$/'$new_temperature'/' input_long.dat
   ./clean.sh
   ./run.sh

   awk 'END{print '$new_temperature', $3, $4}' output.ene.0 >> output_enevstemp
   awk 'END{print '$new_temperature', $3, $4}' output.heat.0 >> output_heatvstemp
   awk 'END{print '$new_temperature', $3, $4}' output.chi.0 >> output_chivstemp

done

echo -e "${GREEN}Restart with h=0.02"
tstep=0.056
new_temperature=0.2;
for (( Ts = 1; Ts < 51; Ts++ )); do
   new_temperature=$(echo "$new_temperature+$tstep" | bc);

   echo -e "${GREEN}Simulazione a T = $new_temperature ${NC}"
   sed  -i '1 s/^.*$/'$new_temperature'/' input_false_h.dat
   sed  -i '1 s/^.*$/'$new_temperature'/' input_true_h.dat
   sed  -i '1 s/^.*$/'$new_temperature'/' input_long_h.dat
   ./clean.sh
   ./run.sh

   awk 'END{print '$new_temperature', $3, $4}' output.mag.0 >> output_magvstemp


done
