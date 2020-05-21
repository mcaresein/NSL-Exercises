#include <vector>

int NUMBER_OF_CITIES;
int NUMBER_OF_GENERATIONS=100; 
int NUMBER_OF_PATHS=10;
int MOVES_PER_TEMPERATURE;
double T_MIN;
double T_MAX;
int T_STEPS;

std::vector< std::vector<double> > x_of_cities, y_of_cities;
std::vector<double> probabilities;
double beta, temperature;

int accepted, attempted;
int iprint;
