// $Id: $
// Include files 



// local
#include "IProbabilityMatrix.h"

//-----------------------------------------------------------------------------
// Implementation file for class : IProbabilityMatrix
//
// 2011-01-06 : Andres Osorio
//-----------------------------------------------------------------------------

const long double IProbabilityMatrix::InvEvfactor = 5.076E6L; // = (1/eV) = (1.97 10^-7)m : natural units [wikipedia]

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
IProbabilityMatrix::IProbabilityMatrix(  ) {

  debug = false;
  
  //intialize parameters and set matrix elements
  setPhase( 0.0 );
  
  //Angles:
  m_Q   = new matrix< long double >(3,3);
  //initializeAngles();
  
  m_Prob_AtoB = new matrix< long double >(3,3);
  m_U  = new matrix< std::complex<long double> >(3,3);
  m_UC = new matrix< std::complex<long double> >(3,3);
  
  initializeMatrix( m_U );
  initializeMatrix( m_UC );
  initializeMatrix( m_Prob_AtoB );

  std::cout << "IProbabilityMatrix> All set and initialized" << std::endl;
  
}
//=============================================================================
// Destructor
//=============================================================================
IProbabilityMatrix::~IProbabilityMatrix() {

  if( m_U ) delete m_U;
  
  if( m_UC ) delete m_UC;
  
  if( m_Prob_AtoB ) delete m_Prob_AtoB;

  std::cout << "destroying base clase components" << std::endl;
    
} 

//=============================================================================

void IProbabilityMatrix::setAngle(int i, int j, long double angle) 
{

  (*m_Q)(i-1,j-1) = (angle * M_PI)/180.0;
  (*m_Q)(j-1,i-1) = (angle * M_PI)/180.0;
  
}

void IProbabilityMatrix::setPhase( long double angle ) 
{

  m_dCP = (angle * M_PI)/180.0;
  
}

long double IProbabilityMatrix::cosTh( int i, int j )
{
  
  return cos( (*m_Q)(i-1,j-1) );
  
}

long double IProbabilityMatrix::sinTh( int i, int j )
{
  
  return sin( (*m_Q)(i-1,j-1) );

}

void IProbabilityMatrix::initializeMatrix( matrix< long double > * Mat )
{
  
  for(int i=0; i < 3; ++i) 
    for( int j=0; j < 3; ++j)
      (*Mat)(i,j) = 0.0;
  
}

void IProbabilityMatrix::initializeMatrix( matrix< std::complex<long double> > * Mat )
{
  
  for(int i=0; i < 3; ++i) 
    for( int j=0; j < 3; ++j)
      (*Mat)(i,j) = std::complex<long double>(0.0, 0.0);
  
}

void IProbabilityMatrix::Propagate(double phi_e0, double phi_mu0, double phi_tau0)
{

  if ( has_mixing_matrix ) updateMixingMatrix();
  
  calcProbabilities();
    
  double phi_b0[3];

  phi_b0[0] = phi_e0;
  phi_b0[1] = phi_mu0;
  phi_b0[2] = phi_tau0;
    
  m_phi_e_f = 0.0;

  matrix<double> T_Prob_AtoB( trans(*m_Prob_AtoB) );

  for(int b=0; b<3; ++b) 
    m_phi_e_f += T_Prob_AtoB (b,0) * phi_b0[b];
  
  m_phi_mu_f = 0.0;
  
  for(int b=0; b<3; ++b) 
    m_phi_mu_f += T_Prob_AtoB (b,1) * phi_b0[b];
  

  m_phi_tau_f = 0.0;
  
  for(int b=0; b<3; ++b) 
    m_phi_tau_f += T_Prob_AtoB (b,2) * phi_b0[b];
  
  std::cout << m_phi_e_f << " "
            << m_phi_mu_f << " " 
            << m_phi_tau_f << std::endl;
    

}

double IProbabilityMatrix::Propagate(int ridx, double phi_e0, double phi_mu0, double phi_tau0)
{

  double * phi_b0_in = new double[3];
  
  double phi_b0_out = 0.0;
   
  double norm_factor = phi_e0 + phi_mu0 + phi_tau0;
  
  phi_b0_in[0]  = phi_e0;
  phi_b0_in[1]  = phi_mu0;
  phi_b0_in[2]  = phi_tau0;
  
  matrix<double> T_Prob_AtoB( trans(*m_Prob_AtoB) );
  
  for(int b=0; b<3; ++b) 
    phi_b0_out += T_Prob_AtoB (b,ridx) * phi_b0_in[b];
  
  ///std::cout << "IProbabilityMatrix::Propagate> " << ( phi_b0_out / norm_factor ) << std::endl;
  
  delete [] phi_b0_in;
  
  return ( phi_b0_out / norm_factor );
  
}

