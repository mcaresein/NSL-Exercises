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

int n_props=5; const int nbins=100; const int m_props=105;
int iv,ik,it,ie, ip, igofr;
double stima_epot, stima_ekin, stima_etot, stima_temp, stima_pres, g_hist[nbins];

// averages
double blk_av[m_props],blk_norm,accepted,attempted;
double glob_av[m_props],glob_av2[m_props];
double err_epot, err_ekin, err_temp, err_etot, err_pres,err_gdir;


//configuration
const int m_part=108;
double x[m_part],y[m_part],z[m_part],xold[m_part],yold[m_part],zold[m_part];
double vx[m_part],vy[m_part],vz[m_part];

// thermodynamical state
int npart;
double energy,temp,vol,rho,box,rcut, bin_size;

// simulation
int nstep, nblocks, neq, seed;
double delta;
bool restart;

// averages
double acc,att;
int blocks_number;
int blocks_length;


//functions
void Input(void);
void Move(void);
void Measure();
void Reset(int);
void Accumulate(void);
void Averages(int);
void ConfFinal(void);
void ConfXYZ(int);
double Force(int, int);
double Pbc(double);
double Error(double, double, int);
/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/
