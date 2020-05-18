/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/
#include <vector>
using namespace std;

typedef vector< vector<double> > matrix;

//parameters, observables
const int m_props=4;
int n_props=4;
int iv,ik,it,ie;
double stima_pot, stima_kin, stima_etot, stima_temp;

//configuration
const int m_part=108;
double x[m_part],y[m_part],z[m_part],xold[m_part],yold[m_part],zold[m_part];
double vx[m_part],vy[m_part],vz[m_part];

// thermodynamical state
int npart;
double energy,temp,vol,rho,box,rcut;

// simulation
int nconf, conf_length;
int nstep, iprint, seed;
double delta;
bool restart;

// averages
double acc,att;
int blocks_number;
int blocks_length;
//vector< vector<double> > sum_prog_props;
//vector< vector<double> > sum2_prog_props;
//vector< vector<double> > error_prog_props;
//vector< vector<double> > ave_props;
//vector< vector<double> > ave2_props;
//vector<double>  sum_props;

//functions
void Input(void);
void Move(void);
void Measure(vector<double>& );
void ConfFinal(void);
void ConfXYZ(int);
double Force(int, int);
double Pbc(double);
void WriteAverages(void);
double Error(double, double, int);
void BlockAverages(int, matrix&, matrix&, matrix&, vector<double>&);
/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/
