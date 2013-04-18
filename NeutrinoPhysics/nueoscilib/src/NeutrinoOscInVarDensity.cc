// $Id: $
// Include files 



// local
#include "NeutrinoOscInVarDensity.h"
#include "DensityModels.h"

//-----------------------------------------------------------------------------
// Implementation file for class : NeutrinoOscInVarDensity
//
// 2011-07-28 : Andres Osorio
//-----------------------------------------------------------------------------

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
NeutrinoOscInVarDensity::NeutrinoOscInVarDensity( MixingParameters * mixpars ) : IProbabilityMatrix() {

  m_debug           = true;
  m_validation      = false;
  use_default_pars  = true;
  has_mixing_matrix = true;
  
  SetParameters( mixpars );
  
  v_Lambda = new matrix<  long double >(1,3);
  m_Eab    = new matrix<  long double >(3,3);
  m_Tab    = new matrix<  long double >(3,3);
  m_Ur     = new matrix<  long double >(3,3);
  m_invUr  = new matrix<  long double >(3,3);
  m_UTU    = new matrix<  long double >(3,3);
  m_UTUSq  = new matrix<  long double >(3,3);
  m_Uf     = new matrix< std::complex<  long double > >(3,3);
  m_Ufd    = new matrix< std::complex<  long double > >(3,3);
  
  m_phi    = std::complex<  long double >(1.0, 1.0);
  
  initializeAngles();
        
}

//=============================================================================
// Destructor
//=============================================================================
NeutrinoOscInVarDensity::~NeutrinoOscInVarDensity() {

  //delete all Matrices

} 

//=============================================================================
void NeutrinoOscInVarDensity::initializeAngles() 
{
  
  for(int i=0; i < 3; ++i) 
    for( int j=0; j < 3; ++j)
      (*m_Q)(i,j) = 0.0;
  
  if ( use_default_pars ) 
  {
    setAngle(1, 2, 33.0);  //theta_12
    setAngle(1, 3,  0.0);  //theta_13
    setAngle(2, 3, 45.0);  //theta_23
    
    m_Ev = 10.0E9L;
    m_DeltamSq = 1.0E-4L;
    m_DeltaMSq = 1.0E-3L;

  } else {

    std::cout << "initializeAngles> reading parameters from XML" << std::endl;
    
    m_Ev = 10.0E9L;

    setAngle(1, 2, m_input->GetPar1() );  //theta_12
    setAngle(1, 3, m_input->GetPar2() );  //theta_13
    setAngle(2, 3, m_input->GetPar3() );  //theta_23
    
    m_DeltaMSq = m_input->GetPar4(); //Dm2_32 -> typically set to 3e-3
    m_DeltamSq = m_input->GetPar8(); //Dm2_21 -> typically set to 8e-5
    
  }
  
}

void NeutrinoOscInVarDensity::updateEab() 
{
  
  initializeMatrix ( m_Eab );

  (*m_Eab)(1,0) = m_DeltamSq / ( 2.0L * m_Ev); //E_21
  (*m_Eab)(2,1) = m_DeltaMSq / ( 2.0L * m_Ev); //E_32
  (*m_Eab)(0,1) = -1.0L * (*m_Eab)(1,0);
  (*m_Eab)(1,2) = -1.0L * (*m_Eab)(2,1);
  
  (*m_Eab)(2,0) = -1.0L * ( (*m_Eab)(0,1) + (*m_Eab)(1,2) );
  
  (*m_Eab)(0,2) = -1.0L * (*m_Eab)(2,0);
  
}

void NeutrinoOscInVarDensity::updateCoefficients()
{
  
  m_c0 = -1.0L * determinant( (*m_Tab) ) ; 

  m_c1 = ( (*m_Tab)(0,0) * (*m_Tab)(1,1) )
    -    ( (*m_Tab)(0,1) * (*m_Tab)(1,0) ) 
    +    ( (*m_Tab)(0,0) * (*m_Tab)(2,2) )
    -    ( (*m_Tab)(0,2) * (*m_Tab)(2,0) )
    +    ( (*m_Tab)(1,1) * (*m_Tab)(2,2) )
    -    ( (*m_Tab)(1,2) * (*m_Tab)(2,1) );
    
}

void NeutrinoOscInVarDensity::updateLambdas()
{

  long double oneover3 = 1.0L/3.0L;
  long double threeover2 = 3.0L/2.0L;
  
  long double Lambda_1 = 2.0L * sqrt(- oneover3 * m_c1) * 
    cos( oneover3 * acos( threeover2 * (m_c0/m_c1) * sqrt(-3.0L / m_c1 ) ) );

  long double Lambda_2 = 2.0L * sqrt(- oneover3 * m_c1) * 
    cos( oneover3 * acos( threeover2 * (m_c0/m_c1) * sqrt(-3.0L / m_c1 ) ) - (2.0L * M_PI / 3.0L) );
  
  long double Lambda_3 = 2.0L * sqrt(- oneover3 * m_c1) * 
    cos( oneover3 * acos( threeover2 * (m_c0/m_c1) * sqrt(-3.0L / m_c1 ) ) - (4.0L * M_PI / 3.0L) );
  
  (*v_Lambda)(0,0) = Lambda_1;
  (*v_Lambda)(0,1) = Lambda_2;
  (*v_Lambda)(0,2) = Lambda_3;
  
}

void NeutrinoOscInVarDensity::updateMixingMatrix( )
{
  
  // this is the CKM Matrix 
  (*m_Ur) (0,0) = real( std::complex<long double>( cosTh(1,2)*cosTh(1,3), 0.0) );
  
  (*m_Ur) (0,1) = real( std::complex<long double>( cosTh(1,3)*sinTh(1,2), 0.0) );
  
  (*m_Ur) (0,2) = real( std::complex<long double>( sinTh(1,3)*cos(m_dCP), - sinTh(1,3)*sin(m_dCP) ) );
  
  (*m_Ur) (1,0) = real( std::complex<long double>( -cosTh(2,3)*sinTh(1,2) - cosTh(1,2)*sinTh(1,3)*sinTh(2,3)*cos(m_dCP), 
						   -cosTh(1,2)*sinTh(1,3)*sinTh(2,3)*sin(m_dCP) ) );
  
  (*m_Ur) (1,1) = real( std::complex<long double>( cosTh(1,2)*cosTh(2,3) - sinTh(1,2)*sinTh(1,3)*sinTh(2,3)*cos(m_dCP), 
						   -sinTh(1,2)*sinTh(1,3)*sinTh(2,3)*sin(m_dCP) ) );
  
  (*m_Ur) (1,2) = real( std::complex<long double>( cosTh(1,3)*sinTh(2,3), 0.0) );
  
  (*m_Ur) (2,0) = real( std::complex<long double>( sinTh(1,2)*sinTh(2,3) - cosTh(1,2)*cosTh(2,3)*sinTh(1,3)*cos(m_dCP), 
						   -cosTh(1,2)*cosTh(2,3)*sinTh(1,3)*sin(m_dCP) ) );
  
  (*m_Ur) (2,1) = real( std::complex<long double>( -cosTh(1,2)*sinTh(2,3) - cosTh(2,3)*sinTh(1,2)*sinTh(1,3)*cos(m_dCP), 
						   -cosTh(2,3)*sinTh(1,2)*sinTh(1,3)*sin(m_dCP) ) );
  
  (*m_Ur) (2,2) = real( std::complex<long double>( cosTh(1,3)*cosTh(2,3), 0.0) );
  
  // U-1 == inverse of Matrix U
  invertMat ( (*m_Ur), (*m_invUr) );
  
}

void NeutrinoOscInVarDensity::Eval_TnuT(  long double x ) 
{
  
  initializeMatrix ( m_Tab );
  
  long double Ve = f_Ve->Eval( x );
  
  long double oneover3 = (1.0L/3.0L);
  
  (*m_Tab) (0,0) = Ve * ( (*m_Ur)(0,0) * (*m_Ur)(0,0) ) - ( oneover3 * Ve )
    + oneover3 * ((*m_Eab)(0,1) + (*m_Eab)(0,2));
  
  (*m_Tab) (1,1) = Ve * ( (*m_Ur)(0,1) * (*m_Ur)(0,1) ) - ( oneover3 * Ve )
    + oneover3 * ((*m_Eab)(1,0) + (*m_Eab)(1,2));
  
  (*m_Tab) (2,2) = Ve * ( (*m_Ur)(0,2) * (*m_Ur)(0,2) ) - ( oneover3 * Ve )
    + oneover3 * ((*m_Eab)(2,0) + (*m_Eab)(2,1));
  
  (*m_Tab) (0,1) = Ve * (*m_Ur)(0,0) * (*m_Ur)(0,1);
  (*m_Tab) (0,2) = Ve * (*m_Ur)(0,0) * (*m_Ur)(0,2);
  (*m_Tab) (1,2) = Ve * (*m_Ur)(0,1) * (*m_Ur)(0,2);
  
  (*m_Tab) (1,0) = (*m_Tab) (0,1);
  (*m_Tab) (2,0) = (*m_Tab) (0,2);
  (*m_Tab) (2,1) = (*m_Tab) (1,2);
  
  matrix< long double > tmp_UT =  prod( (*m_Ur), (*m_Tab) );
  
  (*m_UTU) = prod( tmp_UT, (*m_invUr) );

  (*m_UTUSq) = prod( (*m_UTU), (*m_UTU) );

}

void NeutrinoOscInVarDensity::Eval_TnuT(  long double x1, long double x2  ) 
{
  
  initializeMatrix ( m_Tab );
  
  long double Ve1 = f_Ve->Eval( x1 );
  long double Ve2 = f_Ve->Eval( x2 );

  long double Ve  = ( Ve1 + Ve2 ) / 2.0L; 
  //Take the average potential between the two points
  
  long double oneover3 = (1.0L/3.0L);
  
  (*m_Tab) (0,0) = Ve * ( (*m_Ur)(0,0) * (*m_Ur)(0,0) ) - ( oneover3 * Ve )
    + oneover3 * ((*m_Eab)(0,1) + (*m_Eab)(0,2));
  
  (*m_Tab) (1,1) = Ve * ( (*m_Ur)(0,1) * (*m_Ur)(0,1) ) - ( oneover3 * Ve )
    + oneover3 * ((*m_Eab)(1,0) + (*m_Eab)(1,2));
  
  (*m_Tab) (2,2) = Ve * ( (*m_Ur)(0,2) * (*m_Ur)(0,2) ) - ( oneover3 * Ve )
    + oneover3 * ((*m_Eab)(2,0) + (*m_Eab)(2,1));
  
  (*m_Tab) (0,1) = Ve * (*m_Ur)(0,0) * (*m_Ur)(0,1);
  (*m_Tab) (0,2) = Ve * (*m_Ur)(0,0) * (*m_Ur)(0,2);
  (*m_Tab) (1,2) = Ve * (*m_Ur)(0,1) * (*m_Ur)(0,2);
  
  (*m_Tab) (1,0) = (*m_Tab) (0,1);
  (*m_Tab) (2,0) = (*m_Tab) (0,2);
  (*m_Tab) (2,1) = (*m_Tab) (1,2);
  
  matrix< long double > tmp_UT =  prod( (*m_Ur), (*m_Tab) );
  
  (*m_UTU) = prod( tmp_UT, (*m_invUr) );

  (*m_UTUSq) = prod( (*m_UTU), (*m_UTU) );

}

void NeutrinoOscInVarDensity::Eval_UFlavour(  long double x  ) {

  initializeMatrix ( m_Uf  );
  initializeMatrix ( m_Ufd );
  
  for( int a = 1; a <= 3; ++a) {
    
    long double lambdaSq = (*v_Lambda)(0,a-1) * (*v_Lambda)(0,a-1);
    
    (*m_Uf)(0,0) += std::complex<  long double > ( cos( (*v_Lambda)(0,a-1) * x ), -1.0L * sin( (*v_Lambda)(0,a-1) * x ) ) *
      (1.0L / ( (3.0L * lambdaSq) + m_c1 )) * ( ( lambdaSq + m_c1 ) + (*v_Lambda)(0,a-1)*(*m_UTU)(0,0) + (*m_UTUSq)(0,0) );
    
    (*m_Uf)(1,1) += std::complex<  long double > ( cos( (*v_Lambda)(0,a-1) * x ), -1.0L * sin( (*v_Lambda)(0,a-1) * x ) ) * 
      (1.0L / ( (3.0L * lambdaSq) + m_c1 )) * ( ( lambdaSq + m_c1 ) + (*v_Lambda)(0,a-1)*(*m_UTU)(1,1) + (*m_UTUSq)(1,1) );
    
    (*m_Uf)(2,2) += std::complex<  long double > ( cos( (*v_Lambda)(0,a-1) * x ), -1.0L * sin( (*v_Lambda)(0,a-1) * x ) ) * 
      (1.0L / ( (3.0L * lambdaSq) + m_c1 )) * ( ( lambdaSq + m_c1 ) + (*v_Lambda)(0,a-1)*(*m_UTU)(2,2) + (*m_UTUSq)(2,2) );
    
    (*m_Uf)(0,1) += std::complex<  long double > ( cos( (*v_Lambda)(0,a-1) * x ), -1.0L * sin( (*v_Lambda)(0,a-1) * x ) ) * 
      (1.0L / ( (3.0L * lambdaSq) + m_c1 )) *  ( (*v_Lambda)(0,a-1)*(*m_UTU)(0,1) + (*m_UTUSq)(0,1) );
    
    (*m_Uf)(0,2) += std::complex<  long double > ( cos( (*v_Lambda)(0,a-1) * x ), -1.0L * sin( (*v_Lambda)(0,a-1) * x ) ) * 
      (1.0L / ( (3.0L * lambdaSq) + m_c1 )) *  ( (*v_Lambda)(0,a-1)*(*m_UTU)(0,2) + (*m_UTUSq)(0,2) );
    
    (*m_Uf)(1,0) += std::complex<  long double > ( cos( (*v_Lambda)(0,a-1) * x ), -1.0L * sin( (*v_Lambda)(0,a-1) * x ) ) * 
      (1.0L / ( (3.0L * lambdaSq) + m_c1 )) *  ( (*v_Lambda)(0,a-1)*(*m_UTU)(1,0) + (*m_UTUSq)(1,0) );
    
    (*m_Uf)(2,0) += std::complex<  long double > ( cos( (*v_Lambda)(0,a-1) * x ), -1.0L * sin( (*v_Lambda)(0,a-1) * x ) ) * 
      (1.0L / ( (3.0L * lambdaSq) + m_c1 )) *  ( (*v_Lambda)(0,a-1)*(*m_UTU)(2,0) + (*m_UTUSq)(2,0) );
    
    (*m_Uf)(1,2) += std::complex<  long double > ( cos( (*v_Lambda)(0,a-1) * x ), -1.0L * sin( (*v_Lambda)(0,a-1) * x ) ) * 
      (1.0L / ( (3.0L * lambdaSq) + m_c1 )) *  ( (*v_Lambda)(0,a-1)*(*m_UTU)(1,2) + (*m_UTUSq)(1,2) );
    
    (*m_Uf)(2,1) += std::complex<  long double > ( cos( (*v_Lambda)(0,a-1) * x ), -1.0L * sin( (*v_Lambda)(0,a-1) * x ) ) * 
      (1.0L / ( (3.0L * lambdaSq) + m_c1 )) *  ( (*v_Lambda)(0,a-1)*(*m_UTU)(2,1) + (*m_UTUSq)(2,1) );
    
  }
  
  (*m_Ufd) = conj ( (*m_Uf) );
  
}

void NeutrinoOscInVarDensity::Eval_UFlavour(  long double x1, long double x0) {

  long double dx = x1 - x0;

  //march 17: remove this method - better use average AO
  //long double xH = x1 - (dx/2.0L);
  //this->Eval_TnuT( xH );

  this->Eval_TnuT( x1, x0 );
  this->updateCoefficients();
  this->updateLambdas();
  this->Eval_UFlavour( dx );
  
}

void NeutrinoOscInVarDensity::calcProbabilities() 
{
  
  (*m_Prob_AtoB)(0,0) = real( (*m_Ufd)(0,0) * (*m_Uf)(0,0));
  (*m_Prob_AtoB)(1,1) = real( (*m_Ufd)(1,1) * (*m_Uf)(1,1));
  (*m_Prob_AtoB)(2,2) = real( (*m_Ufd)(2,2) * (*m_Uf)(2,2));
  
  (*m_Prob_AtoB)(0,1) = real( (*m_Ufd)(0,1) * (*m_Uf)(0,1));
  (*m_Prob_AtoB)(0,2) = real( (*m_Ufd)(0,2) * (*m_Uf)(0,2));
  
  (*m_Prob_AtoB)(1,0) = (*m_Prob_AtoB)(0,1);
  (*m_Prob_AtoB)(2,0) = (*m_Prob_AtoB)(0,2);
  
  (*m_Prob_AtoB)(1,2) = real( (*m_Ufd)(1,2) * (*m_Uf)(1,2));
  (*m_Prob_AtoB)(2,1) = (*m_Prob_AtoB)(1,2);
  
  /*
    matrix <std::complex< long double> > tmp = prod( (*m_Ufd), (*m_Uf) );
    (*m_Prob_AtoB) = real( tmp );
  */
  
}

void NeutrinoOscInVarDensity::Validate() 
{
  
  //.....................................................................................
  
  m_validation = true;

  std::cout << "NeutrinoOscInVarDensity::Validate> Important: this method is no longer in use. " << std::endl;
  std::cout << "NeutrinoOscInVarDensity::Validate> Changes apply from Apr 02 2013. " << std::endl;
  std::cout << "NeutrinoOscInVarDensity::Validate> It will be removed. " << std::endl;
  
  //.....................................................................................

}

void NeutrinoOscInVarDensity::ValidateInVarDensity() {
  
  m_validation = true;
  
  std::cout << "NeutrinoOscInVarDensity::ValidateInVarDensity> Important: this method is no longer in use. " << std::endl;
  std::cout << "NeutrinoOscInVarDensity::ValidateInVarDensity> Changes apply from Apr 05 2013. " << std::endl;
  std::cout << "NeutrinoOscInVarDensity::ValidateInVarDensity> It will be removed. " << std::endl;
  
}

void NeutrinoOscInVarDensity::ValidateSolarProfile() {
  
  m_validation = true;
  
  std::cout << "NeutrinoOscInVarDensity::ValidateSolarProfile> Important: this method is no longer in use. " << std::endl;
  std::cout << "NeutrinoOscInVarDensity::ValidateSolarProfile> Changes apply from Apr 05 2013. " << std::endl;
  std::cout << "NeutrinoOscInVarDensity::ValidateSolarProfile> It will be removed. " << std::endl;
  
  
}

void NeutrinoOscInVarDensity::TestProcedure()
{

  std::cout << "NeutrinoOscInVarDensity::TestProcedure> Important: this method is no longer in use. " << std::endl;
  std::cout << "NeutrinoOscInVarDensity::TestProcedure> Changes apply from Apr 05 2013. " << std::endl;
  std::cout << "NeutrinoOscInVarDensity::TestProcedure> It will be removed. " << std::endl;
  
}