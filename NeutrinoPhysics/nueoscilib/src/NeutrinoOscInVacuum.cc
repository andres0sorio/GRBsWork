// $Id: $
// Include files 



// local
#include "NeutrinoOscInVacuum.h"

//-----------------------------------------------------------------------------
// Implementation file for class : NeutrinoOscInVacuum
//
//
//
// 2010-10-28 : Andres Osorio
//-----------------------------------------------------------------------------

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
NeutrinoOscInVacuum::NeutrinoOscInVacuum( ) : IProbabilityMatrix( ) {

  use_default_pars = true;

  has_mixing_matrix = true;

  initializeAngles();
    
}

NeutrinoOscInVacuum::NeutrinoOscInVacuum( MixingParameters * mixpars ) : IProbabilityMatrix( ) {

  use_default_pars = false;
  
  has_mixing_matrix = true;
  
  SetParameters( mixpars );
  
  initializeAngles();
    
}

//=============================================================================
// Destructor
//=============================================================================

NeutrinoOscInVacuum::~NeutrinoOscInVacuum() {
} 

//=============================================================================

void NeutrinoOscInVacuum::initializeAngles() 
{
  
  for(int i=0; i < 3; ++i) 
    for( int j=0; j < 3; ++j)
      (*m_Q)(i,j) = 0.0;
  
  if ( use_default_pars ) 
  {
    setAngle(1, 2, 33.0);  //theta_12
    setAngle(1, 3,  0.0);  //theta_13
    setAngle(2, 3, 45.0);  //theta_23
        
    setPhase( 0.0 );
    
  } else {

    std::cout << "initializeAngles> reading parameters from XML" << std::endl;
    
    setAngle(1, 2, m_input->GetPar1() );  //theta_12
    setAngle(1, 3, m_input->GetPar2() );  //theta_13
    setAngle(2, 3, m_input->GetPar3() );  //theta_23

    setPhase( m_input->GetPar9() ); //AO ok fixed dec 2013
    
    //std::cout << "NeutrinoOscInVacuum> ** dCP=" << m_dCP << std::endl; 

  }
    
 
}

void NeutrinoOscInVacuum::updateMixingMatrix( )
{
  
  (*m_U) (0,0) = std::complex<double>( cosTh(1,2)*cosTh(1,3), 0.0);

  (*m_U) (0,1) = std::complex<double>( cosTh(1,3)*sinTh(1,2), 0.0);
  
  (*m_U) (0,2) = std::complex<double>( sinTh(1,3)*cos(m_dCP),
                                       - sinTh(1,3)*sin(m_dCP) );
  
  (*m_U) (1,0) = std::complex<double>( -cosTh(2,3)*sinTh(1,2) - cosTh(1,2)*sinTh(1,3)*sinTh(2,3)*cos(m_dCP), 
                                       -cosTh(1,2)*sinTh(1,3)*sinTh(2,3)*sin(m_dCP) );

  (*m_U) (1,1) = std::complex<double>( cosTh(1,2)*cosTh(2,3) - sinTh(1,2)*sinTh(1,3)*sinTh(2,3)*cos(m_dCP), 
                                       -sinTh(1,2)*sinTh(1,3)*sinTh(2,3)*sin(m_dCP) );
  
  (*m_U) (1,2) = std::complex<double>( cosTh(1,3)*sinTh(2,3), 0.0);
  
  (*m_U) (2,0) = std::complex<double>( sinTh(1,2)*sinTh(2,3) - cosTh(1,2)*cosTh(2,3)*sinTh(1,3)*cos(m_dCP), 
                                       -cosTh(1,2)*cosTh(2,3)*sinTh(1,3)*sin(m_dCP) );
  
  (*m_U) (2,1) = std::complex<double>( -cosTh(1,2)*sinTh(2,3) - cosTh(2,3)*sinTh(1,2)*sinTh(1,3)*cos(m_dCP), 
                                       -cosTh(2,3)*sinTh(1,2)*sinTh(1,3)*sin(m_dCP) );
  
  (*m_U) (2,2) = std::complex<double>( cosTh(1,3)*cosTh(2,3), 0.0);

  //conj == conjugate of Matrix U
  (*m_UC) = conj( (*m_U) );
    
}

void NeutrinoOscInVacuum::calcProbabilities() 
{
  
  double sum=0.0;
  
  for(int a=0; a<3; ++a)
    for(int b=0; b<3; ++b) 
    {
      sum = 0.0;
      for(int i=0; i<3; ++i) 
        sum += ((*m_U)(a,i) * (*m_UC)(a,i)).real() * ((*m_U)(b,i) * (*m_UC)(b,i)).real();
            
      (*m_Prob_AtoB) (a,b) = sum;
      
    }
  
  
}
