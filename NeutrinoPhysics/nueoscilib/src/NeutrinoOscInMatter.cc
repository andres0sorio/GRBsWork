// $Id: $
// Include files 



// local
#include "NeutrinoOscInMatter.h"

//-----------------------------------------------------------------------------
// Implementation file for class : NeutrinoOscInMatter
//
// 2011-01-09 : Andres Osorio
//-----------------------------------------------------------------------------

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
NeutrinoOscInMatter::NeutrinoOscInMatter(  ) : IProbabilityMatrix() {

  has_mixing_matrix = false;
  use_default_pars  = true;

  m_Qm   = new matrix< long double >(3,3);
  initializeMatrix( m_Qm );
  
  initializeAngles();
  
}

NeutrinoOscInMatter::NeutrinoOscInMatter(  MixingParameters * mixpars ) : IProbabilityMatrix() {

  has_mixing_matrix = false;
  use_default_pars  = false;

  SetParameters( mixpars );
  
  m_Qm   = new matrix< long double >(3,3);
  initializeMatrix( m_Qm );
  
  initializeAngles();

}

//=============================================================================
// Destructor
//=============================================================================
NeutrinoOscInMatter::~NeutrinoOscInMatter() {} 

//=============================================================================

void NeutrinoOscInMatter::initializeAngles() 
{
  
  for(int i=0; i < 3; ++i) 
    for( int j=0; j < 3; ++j)
      (*m_Q)(i,j) = 0.0;
  
  if ( use_default_pars ) 
  {
    
    std::cout << "initializeAngles> Warning: using default values." << std::endl;
    
    setAngle(1, 2, 33.0);  //theta_12
    setAngle(1, 3,  0.0);  //theta_13
    setAngle(2, 3, 45.0);  //theta_23
    m_Dm23sq = 1.0;
    m_L_m = 1.0;
    m_Enu = 1.0;
    m_Ve = 1.0;
    
  } else {
    
    setAngle(1, 2, m_input->GetPar1() );  //theta_12
    setAngle(1, 3, m_input->GetPar2() );  //theta_13
    setAngle(2, 3, m_input->GetPar3() );  //theta_23
    
    std::cout << "initializeAngles> mixing angles set" << std::endl;
    
    m_Dm23sq = m_input->GetPar4(); // Dm23 squared
    m_L_m    = m_input->GetPar5(); // L (m)
    m_Enu    = m_input->GetPar6(); // Enu
    m_Ve     = m_input->GetPar7(); // Ve
    
    m_L = m_L_m * (InvEvfactor);
    
    std::cout << "initializeAngles> extra parameters set" << std::endl;
    
  }

  // mixing angles in matter
  // theta_1,3,m != 0

  std::cout << "initializeAngles> using default pars" <<  use_default_pars << std::endl;
  std::cout << "initializeAngles> Dm23sq: " << m_Dm23sq << std::endl;
  std::cout << "initializeAngles> L: " << m_L << std::endl;  
  
  (*m_Qm)(1-1,3-1) = 0.5 * asin( sin2Q13m() );

}

long double NeutrinoOscInMatter::sinTh_m( int i, int j )
{
  
  return sin( (*m_Qm)(i-1,j-1) );
  
}

long double NeutrinoOscInMatter::cosTh_m( int i, int j )
{
  
  return cos( (*m_Qm)(i-1,j-1) );
  
}

long double NeutrinoOscInMatter::sin2Q13m() 
{
  
  long double result = ( sin( 2.0 * (*m_Q)(1-1,3-1) ) ) /
    sqrt( ( cos( 2.0 * (*m_Q)(1-1,3-1) ) - (2.0 * m_Enu * m_Ve / m_Dm23sq )) * 
          ( cos( 2.0 * (*m_Q)(1-1,3-1) ) - (2.0 * m_Enu * m_Ve / m_Dm23sq )) +
          ( sin( 2.0 * (*m_Q)(1-1,3-1) )) * ( sin( 2.0 * (*m_Q)(1-1,3-1) )) );
  
  return result;
  
}

long double NeutrinoOscInMatter::deltaMu21()
{

  long double result = ( 0.5 * m_Dm23sq) * ( ( sin( 2.0 * (*m_Q)(1-1,3-1)) / sin( 2.0 * (*m_Qm)(1-1,3-1)) ) - 1.0)
    - ( m_Enu * m_Ve );
  
  return result;
  
}

long double NeutrinoOscInMatter::deltaMu32()
{
  
  long double result = ( 0.5 * m_Dm23sq) * ( ( sin( 2.0 * (*m_Q)(1-1,3-1)) / sin( 2.0 * (*m_Qm)(1-1,3-1)) ) + 1.0)
    + ( m_Enu * m_Ve );
  
  return result;

}

long double NeutrinoOscInMatter::deltaMu31()
{
  
  long double result = m_Dm23sq * ( ( sin( 2.0 * (*m_Q)(1-1,3-1)) / sin( 2.0 * (*m_Qm)(1-1,3-1)) ) );
  
  return result;
  
}

long double NeutrinoOscInMatter::Sij(int i, int j)
{

  //
  if ( i==2 && j==1 )
    return pow( sin( deltaMu21() * m_L / ( 4.0 * m_Enu ) ) , 2.0L );
  
  //  
  if ( i==3 && j==2 )
    return pow( sin( deltaMu32() * m_L / ( 4.0 * m_Enu ) ) , 2.0L );

  //
  if ( i==3 && j==1 )
    return pow( sin( deltaMu31() * m_L / ( 4.0 * m_Enu ) ) , 2.0L );

  std::cout << "Warning: option not considered!" << std::endl;
  
  return -1.0;
  
}

void NeutrinoOscInMatter::calcProbabilities() 
{
  
  std::cout << "calcProbabilities> starts" << std::endl;

  //(*m_Prob_AtoB) (0,0) = Pee ok
  (*m_Prob_AtoB) (0,0) = 1.0 
    - 4.0*sinTh_m(1,3)*sinTh_m(1,3)*cosTh_m(1,3)*cosTh_m(1,3)*Sij(3,1);
  
  //(*m_Prob_AtoB) (0,1) = Pem ok
  (*m_Prob_AtoB) (0,1) = 4.0 * pow( sinTh_m(1,3), 2.0L) 
    * pow( cosTh_m(1,3), 2.0L) * pow( sinTh(2,3), 2.0L) * Sij(3,1);

  //(*m_Prob_AtoB) (0,2) = Pet ok
  (*m_Prob_AtoB) (0,2) = 4.0 * pow( sinTh_m(1,3), 2.0L) 
    * pow( cosTh_m(1,3), 2.0L) * pow( cosTh(2,3), 2.0L) * Sij(3,1);
  
  //(*m_Prob_AtoB) (1,0) = Pme ok
  (*m_Prob_AtoB) (1,0) = (*m_Prob_AtoB) (0,1);
  
  //(*m_Prob_AtoB) (1,1) = Pmm
  (*m_Prob_AtoB) (1,1) = 1.0 
    - 4.0 * pow( sinTh_m(1,3), 2.0L) * pow(cosTh_m(1,3), 2.0L)
    * pow( sinTh(2,3), 4.0L) * Sij(3,1)
    - 4.0 * pow( sinTh_m(1,3), 2.0L) * pow(sinTh(2,3), 2.0L)
    * pow( cosTh(2,3), 2.0L) * Sij(2,1)
    - 4.0 * pow( cosTh_m(1,3), 2.0L) * pow(sinTh(2,3), 2.0L)
    * pow( cosTh(2,3), 2.0L) * Sij(3,2);
  
  //(*m_Prob_AtoB) (1,2) = Pmt
  (*m_Prob_AtoB) (1,2) = - 4.0 * pow( sinTh_m(1,3), 2.0L) * pow(cosTh_m(1,3),2.0L)
    * pow( sinTh(2,3), 2.0L) * pow( cosTh(2,3), 2.0L) * Sij(3,1)
    + 4.0 * pow( sinTh_m(1,3), 2.0L) * pow(sinTh(2,3),2.0L)
    * pow( cosTh(2,3), 2.0L) * Sij(2,1)
    + 4.0 * pow( cosTh_m(1,3), 2.0L) * pow(sinTh(2,3),2.0L)
    * pow( cosTh(2,3), 2.0L) * Sij(3,2);

  //(*m_Prob_AtoB) (2,0) = Pte
  (*m_Prob_AtoB) (2,0) = (*m_Prob_AtoB) (0,2);
  
  //(*m_Prob_AtoB) (2,1) = Ptm
  (*m_Prob_AtoB) (2,1) = (*m_Prob_AtoB) (1,2);
  
  //(*m_Prob_AtoB) (2,2) = Ptt
  (*m_Prob_AtoB) (2,2) = 1.0
    - 4.0 * pow( sinTh_m(1,3), 2.0L) * pow(cosTh_m(1,3),2.0L)
    * pow( cosTh(2,3), 4.0L) * Sij(3,1)
    - 4.0 * pow( sinTh_m(1,3), 2.0L) * pow(sinTh(2,3),2.0L)
    * pow( cosTh(2,3), 2.0L) * Sij(2,1)
    - 4.0 * pow( cosTh_m(1,3), 2.0L) * pow(sinTh(2,3),2.0L)
    * pow( cosTh(2,3), 2.0L) * Sij(3,2);


  std::cout << "calcProbabilities> " << (*m_Prob_AtoB) << std::endl;
   
}

void NeutrinoOscInMatter::updateMixingMatrix()
{

  std::cout << "updateMixingMatrix> starts" << std::endl;
  
  setAngle(1, 2, m_input->GetPar1() );  //theta_12
  setAngle(1, 3, m_input->GetPar2() );  //theta_13
  setAngle(2, 3, m_input->GetPar3() );  //theta_23
  
  m_Dm23sq = m_input->GetPar4(); // Dm23 squared
  m_L_m    = m_input->GetPar5(); // L (m)
  m_Enu    = m_input->GetPar6(); // Enu
  m_Ve     = m_input->GetPar7(); // Ve
  
  m_L = m_L_m * (InvEvfactor);
  
  (*m_Qm)(1-1,3-1) = 0.5 * asin( sin2Q13m() );
  
  std::cout << "updateMixingMatrix> DmSq: "         << m_Dm23sq << std::endl;
  std::cout << "updateMixingMatrix> Ve: "           << m_Ve << std::endl;
  std::cout << "updateMixingMatrix> m_Qm: "         << (*m_Qm) << std::endl;
  std::cout << "updateMixingMatrix> sinTh_m(1,3): " << sinTh_m(1,3) << std::endl;
  std::cout << "updateMixingMatrix> cosTh_m(1,3): " << cosTh_m(1,3) << std::endl;
  std::cout << "updateMixingMatrix> Sij(3,1): "     << Sij(3,1) << std::endl;
  std::cout << "updateMixingMatrix> Sij(2,1): "     << Sij(2,1) << std::endl;
  std::cout << "updateMixingMatrix> Sij(3,2): "     << Sij(3,2) << std::endl;
  


}
