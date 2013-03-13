// $Id: $
// Include files 



// local
#include "DensityModels.h"
#include "IProbabilityMatrix.h"

//-----------------------------------------------------------------------------
// Implementation file for class : DensityModels
//
// 2011-09-25 : Andres Osorio
//-----------------------------------------------------------------------------

const double DensityModels::GF = 1.1663E-5; // Fermi constant [GeV-2]
const double DensityModels::Mp = 938.0E6; // [eV];
const double DensityModels::InveV2 = 1.0E-18; // [1/eV^2]

double densityModA(double *x, double *par) 
{
  
  rhoModelA * rhoA = new rhoModelA (); // [eV^4]
  double Gf = DensityModels::GF * DensityModels::InveV2; // [1/eV^2]
  double Ar = (2.0/sqrt(2.0)) * Gf * (1.0/DensityModels::Mp) * (*rhoA)( x, par ); // Factor of two corrected 10 march 2013
  delete rhoA;
  
  return Ar; // [eV]
  
}

double stepProf(double *x, double *par)
{
  
  double xx = x[0]; // distance
  double L1 = 2885.E3 * IProbabilityMatrix::InvEvfactor;
  
  //parameters just as needed
  double A1 = par[0];
  double A2 = par[1];

  if ( xx <= L1 ) return A1;
  else return A2;
  
  return 0.0;
  
}

double mcProf(double *x, double *par)
{
  
  double xx = x[0]; // distance
  double L1 = 2885E3 * IProbabilityMatrix::InvEvfactor;
  double L2 = L1 + ( 6972E3 * IProbabilityMatrix::InvEvfactor);
  
  //parameters just as needed
  double A1 = par[0];
  double A2 = par[1];

  if ( xx <= L1 ) return A1;
  else if ( xx > L1 && xx < L2 ) return A2;
  else return A1;
  
  return 0.0;
  
}
