/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/
#include <stdlib.h>     // srand, rand: to generate random number
#include <iostream>     // cin, cout: Standard Input/Output Streams Library
#include <fstream>      // Stream class to both read and write from/to files.
#include <string>
#include <cmath>        // rint, pow
#include "MolDyn_NVE.h"
using namespace std;


int main(){

  Input();             //Inizialization

  int iconf = 1;  //indice della configurazione
  matrix  sum_prog(blocks_number,vector<double>(n_props));
  matrix  sum2_prog(blocks_number,vector<double>(n_props));
  matrix  error_prog(blocks_number,vector<double>(n_props));
  vector<double>  sum_props(n_props);

  for(int istep=1; istep <= nstep; ++istep){
     Move();           //Move particles with Verlet algorithm
     if(istep%iprint == 0) cout << "Number of time-steps: " << istep << endl;
     if(istep%conf_length== 0){
        Measure(sum_props);                   //Properties measurement
        ConfXYZ(iconf);                       //Write conf in XYZ format //Commented to avoid "filesystem full"!
        if(iconf%blocks_length == 0){
            int block_index=((iconf-1)/blocks_length);
            cout << " Chiudo e medio blocco " << block_index+1 << " di " << blocks_number << endl;
            BlockAverages(block_index, sum_prog, sum2_prog, error_prog, sum_props);
        }
        iconf += 1;
     }
  }
  ConfFinal();         //Write final configuration to restar

  return 0;
}


void Input(void){ //Prepare all stuff for the simulation
  ifstream ReadInput,ReadConf;
  double ep, ek, pr, et, vir;

  cout << "Classic Lennard-Jones fluid        " << endl;
  cout << "Molecular dynamics simulation in NVE ensemble  " << endl << endl;
  cout << "Interatomic potential v(r) = 4 * [(1/r)^12 - (1/r)^6]" << endl << endl;
  cout << "The program uses Lennard-Jones units " << endl;

  seed = 1;    //Set seed for random numbers
  srand(seed); //Initialize random number generator

  ReadInput.open("input.dat"); //Read input

  ReadInput >> temp;

  ReadInput >> npart;
  cout << "Number of particles = " << npart << endl;

  ReadInput >> rho;
  cout << "Density of particles = " << rho << endl;
  vol = (double)npart/rho;
  cout << "Volume of the simulation box = " << vol << endl;
  box = pow(vol,1.0/3.0);
  cout << "Edge of the simulation box = " << box << endl;

  ReadInput >> rcut;
  ReadInput >> delta;
  ReadInput >> nstep;
  ReadInput >> iprint;
  ReadInput >> restart;
  ReadInput >> blocks_number;
  ReadInput >> nconf;

  cout << "The program integrates Newton equations with the Verlet method " << endl;
  cout << "Time step = " << delta << endl;
  cout << "Number of steps = " << nstep << endl;
  cout << "Number of configurations = " << nconf << endl;
  cout << "Number of blocks = " << blocks_number << endl << endl ;
  if (restart) cout << "Let's restart from old.final" << endl;

  ReadInput.close();

  //Prepare array for measurements
  iv = 0; //Potential energy
  ik = 1; //Kinetic energy
  it = 2; //Temperature
  ie = 3; //Total Energy
  ip = 4; //Pressure
  // n_props = 4; //Number of observables: SET IN THE HEADER

  conf_length=nstep/nconf;             // cioè numero di integrazioni tra una misura e l'altra
  blocks_length=nconf/(blocks_number); //lunghezza del blocco IN UNITA' DI CONFIGURAZIONI

  cout << "block's length = " << blocks_length << endl;
  cout << "conf's length = " << conf_length << endl;
  //Read initial configuration
  cout << "Read initial configuration from file config.0 " << endl << endl;
  ReadConf.open("config.0");
  for (int i=0; i<npart; ++i){
     ReadConf >> x[i] >> y[i] >> z[i];
     x[i] = x[i] * box;
     y[i] = y[i] * box;
     z[i] = z[i] * box;
    }
  ReadConf.close();

if (restart){
    //read last old configuration
    cout << "Read last old configuration from file old.0 " << endl << endl;
    ReadConf.open("old.0");
    for (int i=0; i<npart; ++i){
        ReadConf >> xold[i] >> yold[i] >> zold[i];
        xold[i] = xold[i] * box;
        yold[i] = yold[i] * box;
        zold[i] = zold[i] * box;
    }
    ReadConf.close();

    Move();

    double t = 0.0;
    for (int i=0; i<npart; ++i) t += 0.5*(vx[i]*vx[i] + vy[i]*vy[i] + vz[i]*vz[i]);
    stima_temp = (2.0 / 3.0) * t/(double)npart;

    cout << "mesured T = "<< stima_temp << " ;  initial temperature T = " << temp << endl << endl;

    double fs;
    fs=sqrt(temp/stima_temp);


    cout << " fs = " << endl;

    for (int i=0; i<npart; ++i){
        vx[i] *= fs;
        vy[i] *= fs;
        vz[i] *= fs;

        xold[i] = Pbc(x[i] - vx[i] * delta);
        yold[i] = Pbc(y[i] - vy[i] * delta);
        zold[i] = Pbc(z[i] - vz[i] * delta);
    }
  } else {
    //Prepare initial velocities
    cout << "Prepare random velocities with center of mass velocity equal to zero " << endl << endl;
       double sumv[3] = {0.0, 0.0, 0.0};
       for (int i=0; i<npart; ++i){
         vx[i] = rand()/double(RAND_MAX) - 0.5;
         vy[i] = rand()/double(RAND_MAX) - 0.5;
         vz[i] = rand()/double(RAND_MAX) - 0.5;

         sumv[0] += vx[i];
         sumv[1] += vy[i];
         sumv[2] += vz[i];
       }
       for (int idim=0; idim<3; ++idim) sumv[idim] /= (double)npart;
       double sumv2 = 0.0, fs;
       for (int i=0; i<npart; ++i){
         vx[i] = vx[i] - sumv[0];
         vy[i] = vy[i] - sumv[1];
         vz[i] = vz[i] - sumv[2];
         //cout << vx[i] << endl;

         sumv2 += vx[i]*vx[i] + vy[i]*vy[i] + vz[i]*vz[i];
       }
       sumv2 /= (double)npart;
       cout << sumv2 << endl;

       fs = sqrt(3 * temp / sumv2);   // fs = velocity scale factor
       cout << "fs " << fs << endl;

       for (int i=0; i<npart; ++i){
         vx[i] *= fs;
         vy[i] *= fs;
         vz[i] *= fs;

         xold[i] = Pbc(x[i] - vx[i] * delta);
         yold[i] = Pbc(y[i] - vy[i] * delta);
         zold[i] = Pbc(z[i] - vz[i] * delta);
       }
    }
    return;
}

void Move(void){ //Move particles with Verlet algorithm
  double xnew, ynew, znew, fx[m_part], fy[m_part], fz[m_part];

  for(int i=0; i<npart; ++i){ //Force acting on particle i
    fx[i] = Force(i,0);
    fy[i] = Force(i,1);
    fz[i] = Force(i,2);
  }

  for(int i=0; i<npart; ++i){ //Verlet integration scheme

    xnew = Pbc( 2.0 * x[i] - xold[i] + fx[i] * pow(delta,2) );
    ynew = Pbc( 2.0 * y[i] - yold[i] + fy[i] * pow(delta,2) );
    znew = Pbc( 2.0 * z[i] - zold[i] + fz[i] * pow(delta,2) );

    vx[i] = Pbc(xnew - xold[i])/(2.0 * delta);
    vy[i] = Pbc(ynew - yold[i])/(2.0 * delta);
    vz[i] = Pbc(znew - zold[i])/(2.0 * delta);

    xold[i] = x[i];
    yold[i] = y[i];
    zold[i] = z[i];

    x[i] = xnew;
    y[i] = ynew;
    z[i] = znew;
  }
  return;
}

double Force(int ip, int idir){ //Compute forces as -Grad_ip V(r)
  double f=0.0;
  double dvec[3], dr;

  for (int i=0; i<npart; ++i){
    if(i != ip){
      dvec[0] = Pbc( x[ip] - x[i] );  // distance ip-i in pbc
      dvec[1] = Pbc( y[ip] - y[i] );
      dvec[2] = Pbc( z[ip] - z[i] );

      dr = dvec[0]*dvec[0] + dvec[1]*dvec[1] + dvec[2]*dvec[2];
      dr = sqrt(dr);

      if(dr < rcut){
        f += dvec[idir] * (48.0/pow(dr,14) - 24.0/pow(dr,8)); // -Grad_ip V(r)
      }
    }
  }

  return f;
}

void Measure(vector<double> &sum_props){ //Properties measurement
   int bin;
   double v, w, t, vij, wij;
   double dx, dy, dz, dr;
   ofstream Epot, Ekin, Etot, Temp, Pres;

   Epot.open("output_epot.dat",ios::app);
   Ekin.open("output_ekin.dat",ios::app);
   Temp.open("output_temp.dat",ios::app);
   Etot.open("output_etot.dat",ios::app);
   Pres.open("output_pres.dat",ios::app);

   v = 0.0; //reset observables
   w = 0.0;
   t = 0.0;

   //cycle over pairs of particles
   for (int i=0; i<npart-1; ++i){
       for (int j=i+1; j<npart; ++j){

           dx = Pbc( x[i] - x[j] );
           dy = Pbc( y[i] - y[j] );
           dz = Pbc( z[i] - z[j] );

           dr = dx*dx + dy*dy + dz*dz;
           dr = sqrt(dr);

           if(dr < rcut){
                vij = 4.0/pow(dr,12) - 4.0/pow(dr,6);
                wij = 1.0/pow(dr,12) - 0.5/pow(dr,6);

            //Potential energy
                v += vij;
                w += wij;
            }
       }
   }

   //Kinetic energy
   for (int i=0; i<npart; ++i) t += 0.5 * (vx[i]*vx[i] + vy[i]*vy[i] + vz[i]*vz[i]);

   //estimations
    stima_pot   = v/(double)npart; //Potential energy per particle
    stima_kin   = t/(double)npart; //Kinetic energy per particle
    stima_temp  = (2.0 / 3.0) * t/(double)npart; //Temperature
    stima_etot  = (t+v)/(double)npart; //Total energy per particle
    stima_pres = rho*stima_temp + 48.0*wij/3.0/vol;

    sum_props[iv] += stima_pot;
    sum_props[ik] += stima_kin;
    sum_props[it] += stima_temp;
    sum_props[ie] += stima_etot;
    sum_props[ip] += stima_pres;


    Epot << stima_pot  << endl;
    Ekin << stima_kin  << endl;
    Temp << stima_temp << endl;
    Etot << stima_etot << endl;
    Pres << stima_pres << endl;

    Epot.close();
    Ekin.close();
    Temp.close();
    Etot.close();
    Pres.close();

    return;
}


void ConfFinal(void){ //Write final configuration
  ofstream WriteOld;
  ofstream WriteConf;

  cout << "Printing the file old.final & config.final " << endl << endl;
  WriteOld.open("old.final");
  WriteConf.open("config.final");

  for (int i=0; i<npart; ++i){
    WriteOld << xold[i]/box << "   " <<  yold[i]/box << "   " << zold[i]/box << endl;

  }
  for (int i=0; i<npart; ++i){
    WriteConf<< x[i]/box << "   " <<  y[i]/box << "   " << z[i]/box << endl;
  }
  WriteOld.close();
  WriteConf.close();
  return;
}


void ConfXYZ(int nconf){ //Write configuration in .xyz format
  ofstream WriteXYZ;

  WriteXYZ.open("frames/config_" + to_string(nconf) + ".xyz");
  WriteXYZ << npart << endl;
  WriteXYZ << "This is only a comment!" << endl;
  for (int i=0; i<npart; ++i){
    WriteXYZ << "LJ  " << Pbc(x[i]) << "   " <<  Pbc(y[i]) << "   " << Pbc(z[i]) << endl;
  }
  WriteXYZ.close();
}


double Pbc(double r){  //Algorithm for periodic boundary conditions with side L=box
    return r - box * rint(r/box);
}

void BlockAverages(int block_index,  matrix &sum_prog, matrix &sum2_prog, matrix &error_prog, vector<double> &sum_props){
    vector<double> ave(n_props);
    vector<double> ave2(n_props);

    ofstream ave_Epot, ave_Ekin, ave_Temp, ave_Etot, ave_pres;
    for (int iprops=0; iprops<n_props; iprops++){
        cout << sum_props[iprops]/blocks_length<< endl;
         ave[iprops] = sum_props[iprops]/blocks_length;
        ave2[iprops] = ave[iprops]*ave[iprops];
        //cumulative update
        for (int j=block_index; j<blocks_number; j++){
            //cout << j ;
              sum_prog[j][iprops] +=  ave[iprops]/(double)(j+1);
             sum2_prog[j][iprops] += ave2[iprops]/(double)(j+1);
        }
        error_prog[block_index][iprops] += Error(sum_prog[block_index][iprops],sum2_prog[block_index][iprops],block_index);
        sum_props[iprops]=0;
   }
    cout << endl;
     ave_Epot.open("ave_epot.dat",ios::app);
     ave_Ekin.open("ave_ekin.dat",ios::app);
     ave_Temp.open("ave_temp.dat",ios::app);
     ave_Etot.open("ave_etot.dat",ios::app);
     ave_pres.open("ave_pres.dat",ios::app);


     ave_Epot << (block_index+1)*blocks_length <<  " " << sum_prog[block_index][iv] << " " << error_prog[block_index][iv] << endl;
     ave_Ekin << (block_index+1)*blocks_length <<  " " << sum_prog[block_index][ik] << " " << error_prog[block_index][ik] << endl;
     ave_Temp << (block_index+1)*blocks_length <<  " " << sum_prog[block_index][it] << " " << error_prog[block_index][it] << endl;
     ave_Etot << (block_index+1)*blocks_length <<  " " << sum_prog[block_index][ie] << " " << error_prog[block_index][ie] << endl;
     ave_pres << (block_index+1)*blocks_length <<  " " << sum_prog[block_index][ip] << " " << error_prog[block_index][ip] << endl;

     ave_Epot.close();
     ave_Ekin.close();
     ave_Temp.close();
     ave_Etot.close();
     ave_pres.close();

}

double Error(double a, double a2,int n){  // Function for statistical uncertainty estimation
    if (n==0) return 0;
    else return sqrt((a2 - pow(a,2))/n);
}


/****************************************************************
*****************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
*****************************************************************
*****************************************************************/
