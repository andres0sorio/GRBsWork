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

  tdrStyle = new TStyle("tdrStyle","Style for P-TDR");

  tdrStyle->SetCanvasBorderMode(0);
  tdrStyle->SetCanvasColor(kWhite);
  tdrStyle->SetTitleBorderSize(0);
  tdrStyle->SetTitleFillColor(kWhite);
  tdrStyle->SetTitleFontSize(0.04);
  tdrStyle->SetGridColor(kGray);
  tdrStyle->SetGridStyle(3);
  tdrStyle->SetGridWidth(1);

  tdrStyle->SetAxisColor(1, "XYZ");
  tdrStyle->SetStripDecimals(kTRUE);
  tdrStyle->SetTickLength(0.03, "XYZ");
  tdrStyle->SetNdivisions(510, "XYZ");
  tdrStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
  tdrStyle->SetPadTickY(1);
  
  tdrStyle->SetLabelColor(1, "XYZ");
  tdrStyle->SetLabelFont(42, "XYZ");
  tdrStyle->SetLabelSize(0.03, "XYZ");
  tdrStyle->SetLabelOffset(0.003, "XYZ");

  tdrStyle->SetTitleColor(1, "XYZ");
  tdrStyle->SetTitleFont(42, "XYZ");
  tdrStyle->SetTitleSize(0.03, "XYZ");
  tdrStyle->SetTitleOffset(1.50, "XYZ"); // Another way to set the Offset
  
  tdrStyle->cd();
        
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

  // There is a problem with the neutrino probabilities
  
  long double Ve  = ( Ve1 + Ve2 ) / 2.0L; //Take the average potential between the two points

  /// std::cout << x2  << " v1 " << Ve1 << " v2 " << Ve2 << " average " << Ve << std::endl;
  
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
  
  double LMAX = (12742.0E3) * InvEvfactor;
    
  TGraph * gEv1 = new TGraph();
  TGraph * gEv2 = new TGraph();
  
  //................ Linear Profile No 1

  TF1 * linearProf = new TF1("Ve","[0] + [1]*x", 0.0, LMAX);
  linearProf->SetParameter(0, 0.0);
  linearProf->SetParameter(1, 5.875E-27 );
  this->setPotential( linearProf );
  
  TCanvas * c1 = new TCanvas("c1","plot 1",100,100,500,500);
  c1->SetFillColor(10);
  linearProf->SetLineColor(2);
  linearProf->SetLineWidth(1);
  c1->cd();
  linearProf->Draw();
  c1->Print("results/linear-profile.eps");
  delete c1;

  setAngle(1, 2, 45.0);  //theta_12
  setAngle(1, 3,  5.0);  //theta_13
  setAngle(2, 3, 45.0);  //theta_23
  
  m_DeltamSq = 0.0L;
  m_DeltaMSq = 3.2E-3L;
  
  this->updateMixingMatrix();
    
  long double Ex = 1.0E9L;
  long double Emax = 1.0E11L;
  
  long double dx = 2.5E13;
  
  int k = 0;
  int counter = 0;

  while ( Ex < Emax ) {
    
    m_Ev = Ex;
    this->updateEab();
    
    matrix <std::complex< long double> >  * tmp;
    tmp = new matrix<std::complex< long double> >(3,3);
    
    double long x1 = 0.0;
    double long x2 = 0.5E13;

    int i = 0;

    while ( x2 < LMAX ) {
      
      this->Eval_UFlavour( x2, x1 );
      
      if( i == 0) {
	(*tmp) = (*m_Uf); 
      } else
	(*tmp) = prod( (*m_Uf), (*tmp) );
      
      x1  = x2;
      x2 += dx;
      
      i += 1;
      counter  += 1;

    }
    
    (*m_Uf) = (*tmp);
    (*m_Ufd) = conj ( (*m_Uf) );

    this->calcProbabilities();
    
    double d1 = (*m_Prob_AtoB)(1,0);

    gEv1->SetPoint( k, Ex, d1);
    k += 1; 
    
    Ex += 0.01E9L;

    delete tmp;
    
  }

  TCanvas * cPap2 = new TCanvas("cPap2-67","Figures in paper 2",100,100,1000,300);
  tdrStyle->cd();
  cPap2->SetFillColor(10);
  cPap2->Divide(3,1);

  cPap2->cd(1);
  gPad->SetLogx();
  gEv1->GetXaxis()->SetTitle("Ev (eV)");
  gEv1->GetYaxis()->SetTitle("Pe#mu");
  gEv1->GetXaxis()->CenterTitle(true);
  gEv1->GetYaxis()->CenterTitle(true);

  gEv1->Draw("AL");
  
  //................ Linear Profile No 2
  
  linearProf->SetParameter(0, 0.0);
  linearProf->SetParameter(1, 1.1750E-27 );
  this->setPotential( linearProf );

  m_DeltamSq = 0.0L;
  m_DeltaMSq = 3.2E-3L;
  
  this->updateMixingMatrix();
    
  Ex = 1.0E9L;
  Emax = 1.0E11L;
  
  dx = 2.5E13;
  
  k = 0;
  counter = 0;

  while ( Ex < Emax ) {
    
    m_Ev = Ex;
    this->updateEab();
    
    matrix <std::complex< long double> >  * tmp;
    tmp = new matrix<std::complex< long double> >(3,3);
    
    double long x1 = 0.0;
    double long x2 = 0.5E13;

    int i = 0;

    while ( x2 < LMAX ) {
      
      this->Eval_UFlavour( x2, x1 );
      
      if( i == 0) {
	(*tmp) = (*m_Uf); 
      } else
	(*tmp) = prod( (*m_Uf), (*tmp) );
      
      x1  = x2;
      x2 += dx;
      
      i += 1;
      counter  += 1;

    }
    
    (*m_Uf) = (*tmp);
    (*m_Ufd) = conj ( (*m_Uf) );

    this->calcProbabilities();
    
    double d1 = (*m_Prob_AtoB)(1,0);

    gEv2->SetPoint( k, Ex, d1);
    k += 1; 
    
    Ex += 0.01E9L;

    delete tmp;
    
  }

  cPap2->cd(2);
  gPad->SetLogx();
  gEv2->GetXaxis()->SetTitle("Ev (eV)");
  gEv2->GetXaxis()->CenterTitle(true);
  gEv2->GetYaxis()->SetTitle("Pe#mu");
  gEv2->GetYaxis()->CenterTitle(true);
  gEv2->Draw("AL");
  
  cPap2->Print("results/figures-6-7-paper_2.eps");

  //................ Step Function Profile No 1
  
  TGraph * gEv3 = new TGraph();
  TGraph * gEv4 = new TGraph();
  
  LMAX = (2885.0E3 + 6972.0E3) * InvEvfactor;
    
  TF1 * stepFunction = new TF1("stepFunction",stepProf, 0.0, LMAX, 2);
  TF1 * stepFunction2 = new TF1("stepFunction2",stepProf, 0.0, LMAX, 2);
  stepFunction->SetParameter(0, 1.7E-13);
  stepFunction->SetParameter(1, 2.1E-13);
  this->setPotential( stepFunction );
  
  TCanvas * cPap2A = new TCanvas("cPap2-23","Figures in paper 2",100,100,1000,300);
  tdrStyle->cd();
  cPap2A->SetFillColor(10);
  cPap2A->Divide(3,1);

  setAngle(1, 2, 45.0);  //theta_12
  setAngle(1, 3,  5.0);  //theta_13
  setAngle(2, 3, 45.0);  //theta_23
  
  m_DeltamSq = 0.0L;
  m_DeltaMSq = 3.2E-3L;
  
  this->updateMixingMatrix();
    
  Ex = 1.0E9L;
  Emax = 0.2E11L;
  
  dx = 1.0E13;
  
  k = 0;
  counter = 0;
  
  while ( Ex < Emax ) {
    
    m_Ev = Ex;
    this->updateEab();
    
    matrix <std::complex< long double> >  * tmp;
    tmp = new matrix<std::complex< long double> >(3,3);
    
    double long x1 = 0.0;
    double long x2 = 0.5E13;
    
    int i = 0;
    
    while ( x2 < LMAX ) {
      
      this->Eval_UFlavour( x2, x1 );
      
      if( i == 0) {
	(*tmp) = (*m_Uf); 
      } else
	(*tmp) = prod( (*m_Uf), (*tmp) );
      
      x1  = x2;
      x2 += dx;
      
      i += 1;
      counter  += 1;
      
    }
    
    (*m_Uf) = (*tmp);
    (*m_Ufd) = conj ( (*m_Uf) );

    this->calcProbabilities();
    
    double d1 = (*m_Prob_AtoB)(1,0);
    
    gEv3->SetPoint( k, Ex, d1);
    k += 1; 
    
    Ex += 0.001E9L;
    
    delete tmp;
    
  }
 
  cPap2A->cd(1);
  gPad->SetLogy();
  stepFunction->SetLineColor(2);
  stepFunction->SetLineWidth(1);
  stepFunction->Draw();
  stepFunction->GetXaxis()->SetTitle("r (1/eV)");
  stepFunction->GetYaxis()->SetTitle("A(r) (eV)");
  stepFunction->SetMinimum(1E-14);
  stepFunction->SetMaximum(1E-12);
  stepFunction->Draw();
  
  cPap2A->cd(2);
  gPad->SetLogx();
  gEv3->GetXaxis()->SetTitle("Ev (eV)");
  gEv3->GetXaxis()->CenterTitle(true);
  gEv3->GetYaxis()->SetTitle("Pe#mu");
  gEv3->GetYaxis()->CenterTitle(true);
  gEv3->Draw("AL");

  //................ Step Function Profile No 2
  
  setAngle(1, 2, 45.0);  //theta_12
  setAngle(1, 3,  5.0);  //theta_13
  setAngle(2, 3, 45.0);  //theta_23
  
  m_DeltamSq = 0.0L;
  m_DeltaMSq = 3.2E-3L;
  
  this->updateMixingMatrix();

  stepFunction2->SetParameter(0, 3.8E-14);
  stepFunction2->SetParameter(1, 7.6E-14);
  this->setPotential( stepFunction2 );
  
  Ex = 1.0E9L;
  Emax = 1.0E11L;
  
  dx = 0.1E13;
  
  k = 0;
  counter = 0;
  
  while ( Ex < Emax ) {
    
    m_Ev = Ex;
    this->updateEab();
    
    matrix <std::complex< long double> >  * tmp;
    tmp = new matrix<std::complex< long double> >(3,3);
    
    double long x1 = 0.0;
    double long x2 = 0.5E13;
    
    int i = 0;
    
    while ( x2 < LMAX ) {
      
      this->Eval_UFlavour( x2, x1 );
      
      if( i == 0) {
	(*tmp) = (*m_Uf); 
      } else
	(*tmp) = prod( (*m_Uf), (*tmp) );
      
      x1  = x2;
      x2 += dx;
      
      i += 1;
      counter  += 1;
      
    }
    
    (*m_Uf) = (*tmp);
    (*m_Ufd) = conj ( (*m_Uf) );

    this->calcProbabilities();
    
    double d1 = (*m_Prob_AtoB)(1,0);
    
    gEv4->SetPoint( k, Ex, d1);
    k += 1; 
    
    Ex += 0.001E9L;
    
    delete tmp;
    
  }

  cPap2A->cd(1);
  stepFunction2->SetLineColor(1);
  stepFunction2->SetLineWidth(1);
  stepFunction2->Draw("LSAME");
  stepFunction2->GetXaxis()->SetTitle("r (1/eV)");
  stepFunction2->GetYaxis()->SetTitle("A(r) (eV)");
  stepFunction2->Draw("LSAME");
    
  cPap2A->cd(3);
  gPad->SetLogx();
  gEv4->GetXaxis()->SetTitle("Ev (eV)");
  gEv4->GetXaxis()->CenterTitle(true);
  gEv4->GetYaxis()->SetTitle("Pe#mu");
  gEv4->GetYaxis()->CenterTitle(true);
  gEv4->Draw("AL");

  cPap2A->Print("results/figures-2-3-paper_2.eps");

  //................ Mantle-Core-Mantle Profile
  
  TGraph * gEv5 = new TGraph();
  
  LMAX = (2885.0E3 + 6972.0E3 + 2885.0E3) * InvEvfactor;
  
  TF1 * mantleCore = new TF1("mantleCore", mcProf, 0.0, LMAX, 2);
  mantleCore->SetParameter(0, 1.7E-13);
  mantleCore->SetParameter(1, 4.4E-13);
  this->setPotential( mantleCore );
  
  TCanvas * cPap2C = new TCanvas("cPap2-1","Figures in paper 2",100,100,1000,300);
  tdrStyle->cd();
  cPap2C->SetFillColor(10);
  cPap2C->Divide(3,1);
  
  setAngle(1, 2, 45.0);  //theta_12
  setAngle(1, 3,  5.0);  //theta_13
  setAngle(2, 3, 45.0);  //theta_23
  
  m_DeltamSq = 0.0L;
  m_DeltaMSq = 3.2E-3L;
  
  this->updateMixingMatrix();
    
  Ex = 1.0E9L;
  Emax = 0.1E11L;
  
  dx = 0.1E13;
  
  k = 0;
  counter = 0;
  
  while ( Ex < Emax ) {
    
    m_Ev = Ex;
    this->updateEab();
    
    matrix <std::complex< long double> >  * tmp;
    tmp = new matrix<std::complex< long double> >(3,3);
    
    double long x1 = 0.0;
    double long x2 = 0.5E13;
    
    int i = 0;
    
    while ( x2 < LMAX ) {
      
      this->Eval_UFlavour( x2, x1 );
      
      if( i == 0) {
	(*tmp) = (*m_Uf); 
      } else
	(*tmp) = prod( (*m_Uf), (*tmp) );
      
      x1  = x2;
      x2 += dx;
      
      i += 1;
      counter  += 1;
      
    }
    
    (*m_Uf) = (*tmp);
    (*m_Ufd) = conj ( (*m_Uf) );

    this->calcProbabilities();
    
    double d1 = (*m_Prob_AtoB)(1,0);
    
    gEv5->SetPoint( k, Ex, d1);
    k += 1; 
    
    Ex += 0.01E9L;
    
    delete tmp;
    
  }
  
  cPap2C->cd(1);
  mantleCore->SetLineColor(2);
  mantleCore->SetLineWidth(1);
  mantleCore->Draw();
  mantleCore->GetXaxis()->SetTitle("r (1/eV)");
  mantleCore->GetYaxis()->SetTitle("A(r) (eV)");
  mantleCore->Draw();
  
  cPap2C->cd(2);
  gPad->SetLogx();
  gEv5->GetXaxis()->SetTitle("Ev (eV)");
  gEv5->GetXaxis()->CenterTitle(true);
  gEv5->GetYaxis()->SetTitle("Pe#mu");
  gEv5->GetYaxis()->CenterTitle(true);
  gEv5->Draw("AL");
  gEv5->GetXaxis()->SetLimits(0.9E9, 1.0E10);
  gEv5->Draw("AL");
  cPap2C->Update();


  cPap2C->Print("results/figures-1-paper_2.eps");

}

void NeutrinoOscInVarDensity::ValidateSolarProfile() {
  
  double LMAX = (6.60E7) * InvEvfactor;
  double r0   = (6.60E7) * InvEvfactor;
  double A0   = 0.75E-11;
  
  TGraph * gEv1 = new TGraph();
  
  //................ Solar Profile No 1
  
  TF1 * solarProf = new TF1("SolarRho","[0]*TMath::Exp(- x / [1])", 0.0, LMAX);
  solarProf->SetParameter(0, A0 );
  solarProf->SetParameter(1, r0 );
  this->setPotential( solarProf );
  
  setAngle(1, 2, 45.0);  //theta_12
  setAngle(1, 3,  5.0);  //theta_13
  setAngle(2, 3, 45.0);  //theta_23
  
  m_DeltamSq = 0.0L;
  m_DeltaMSq = 3.2E-3L;
  
  this->updateMixingMatrix();
    
  long double Ex = 1.0E8L;
  long double Emax = 1.0E9L;
  long double dx = 0.05E13L;
  
  int k = 0;
  int counter = 0;

  while ( Ex < Emax ) {
    
    m_Ev = Ex;
    this->updateEab();
    
    matrix <std::complex< long double> >  * tmp;
    tmp = new matrix<std::complex< long double> >(3,3);
    
    double long x1 = 0.0;
    double long x2 = 0.5E13;

    int i = 0;

    while ( x2 < LMAX ) {
      
      this->Eval_UFlavour( x2, x1 );
      
      if( i == 0) {
        (*tmp) = (*m_Uf); 
      } else
        (*tmp) = prod( (*m_Uf), (*tmp) );
      
      x1  = x2;
      x2 += dx;
      
      i += 1;
      counter  += 1;

    }
    
    (*m_Uf) = (*tmp);
    (*m_Ufd) = conj ( (*m_Uf) );

    this->calcProbabilities();
    
    double d1 = (*m_Prob_AtoB)(1,0);

    gEv1->SetPoint( k, Ex, d1);
    k += 1; 
  
    if ( Ex < 6.0E8 )
      Ex += 0.002E9L;
    else
      Ex += 0.005E9L;

    delete tmp;
    
  }

  TCanvas * cPap2 = new TCanvas("cPap2-Solar","Figures in paper 2: Solar profile",100,100,1000,300);
  tdrStyle->cd();
  cPap2->SetFillColor(10);
  cPap2->Divide(3,1);
  
  cPap2->cd(1);
  solarProf->SetLineColor(2);
  solarProf->SetLineWidth(1);
  solarProf->Draw();
  solarProf->GetXaxis()->SetTitle("r (1/eV)");
  solarProf->GetYaxis()->SetTitle("A(r) (eV)");
  solarProf->Draw();

  cPap2->cd(2);
  gPad->SetLogx();
  gEv1->SetMaximum(1.0);
  gEv1->Draw("AL");
  gEv1->GetXaxis()->SetTitle("Ev (eV)");
  gEv1->GetXaxis()->CenterTitle(true);
  gEv1->GetYaxis()->SetTitle("Pe#mu");
  gEv1->GetYaxis()->CenterTitle(true);
  gEv1->GetXaxis()->SetLimits(0.09E9, 1.0E9);
  gEv1->Draw("AL");
  
  cPap2->Print("results/solar-profile-paper_2.eps");

}

void NeutrinoOscInVarDensity::TestProcedure()
{
  
  //.....................................................................................
  
  m_validation = true;
  
  //.....................................................................................
  double Ax = 1.E-21;
  double Amax = 1.E-14;

  TF1 * ctProf = new TF1("A", "x", Ax, Amax);
  this->setPotential( ctProf );

  setAngle(1, 2, 45.0);  //theta_12
  setAngle(1, 3,  5.0);  //theta_13
  setAngle(2, 3, 45.0);  //theta_23
  
  m_DeltamSq = 0.0;
  m_DeltaMSq = 3.0E-3;
    
  long double LE = (3.0E10) * InvEvfactor;
    
  this->updateMixingMatrix();
  this->updateEab();
  this->Eval_TnuT(Ax);
  this->updateCoefficients();
  this->updateLambdas();
  
  double DX = LE;
  this->Eval_UFlavour( DX );
  this->calcProbabilities();

  //.....................................................................................
  
  if (m_debug) std::cout << "phi: "    << real(m_phi)    
                         << " "        << imag(m_phi) << std::endl
                         << "c0: "     << m_c0           << std::endl
                         << "c1: "     << m_c1           << std::endl
                         << "Lambda: " << (*v_Lambda)    << std::endl
                         << "Eab: "    << (*m_Eab)       << std::endl
                         << "Tab: "    << (*m_Tab)       << std::endl
                         << "U: "      << (*m_Ur)        << std::endl
                         << "inv U: "  << (*m_invUr)     << std::endl
                         << "UTU: "    << (*m_UTU)       << std::endl
                         << "UTUSq: "  << (*m_UTUSq)     << std::endl
                         << "Uf: "     << (*m_Uf)        << std::endl
                         << "Ufd: "    << (*m_Ufd)       << std::endl
                         << "Pab: "    << (*m_Prob_AtoB) << std::endl;

  if (m_debug) std::cout << "Neutrino energy fixed at: " << m_Ev << std::endl;
  
  //.....................................................................................
  
  TGraph * g7 = new TGraph();
  TGraph * g8 = new TGraph();
  TGraph * g9 = new TGraph();
  TGraph * g10 = new TGraph();

  //.....................................................................................

  int k=0;

  while ( Ax < Amax ) {

    this->Eval_TnuT( Ax );
    this->updateCoefficients();
    this->updateLambdas();
    
    this->Eval_UFlavour( LE );
    this->calcProbabilities();
    
    double d1 = (*m_Prob_AtoB)(0,0);
    g7->SetPoint( k, Ax, d1);
    
    double d2 = (*m_Prob_AtoB)(0,1);
    g8->SetPoint( k, Ax, d2);

    double d3 = (*m_Prob_AtoB)(0,2);
    g9->SetPoint( k, Ax, d3);
    
    double d4 = (*m_Prob_AtoB)(1,2);
    g10->SetPoint( k, Ax, d4);
    
    k += 1; 

    if ( Ax <= 1.E-18 )
      Ax += 0.1E-21;
    else if ( Ax > 1.E-18 && Ax < 1.E-15)
      Ax += 0.1E-17;
    else
      Ax += 0.05E-14;

  }
  
  tdrStyle->cd();
  
  TCanvas * c4 = new TCanvas("c4","plot 4",100,100,1400,600);
  c4->SetFillColor(10);
  c4->Divide(3,2);

  c4->cd(1);
  gPad->SetLogx();
  g7->Draw("AL");
  g7->GetXaxis()->CenterTitle(true);
  g7->GetYaxis()->CenterTitle(true);
  g7->GetXaxis()->SetTitle("A (eV)");
  g7->GetYaxis()->SetTitle("Pee");
  g7->SetMaximum(1.05);
  g7->SetMinimum(0.90);
  g7->SetLineStyle(1);
  g7->SetLineWidth(1);
  g7->SetLineColor(2);
  g7->Draw("AL");
    
  c4->cd(2);
  gPad->SetLogx();
  g8->Draw("AL");
  g8->GetXaxis()->CenterTitle(true);
  g8->GetYaxis()->CenterTitle(true);
  g8->GetXaxis()->SetTitle("A (eV)");
  g8->GetYaxis()->SetTitle("Pem");
  g8->SetMaximum(1.05);
  g8->SetMinimum(0.00);
  g8->SetLineStyle(1);
  g8->SetLineWidth(1);
  g8->SetLineColor(2);
  g8->Draw("AL");
  
  c4->cd(3);
  gPad->SetLogx();
  g9->Draw("AL");
  g9->GetXaxis()->CenterTitle(true);
  g9->GetYaxis()->CenterTitle(true);
  g9->GetXaxis()->SetTitle("A (eV)");
  g9->GetYaxis()->SetTitle("Pet");
  g9->SetMaximum(1.05);
  g9->SetMinimum(0.0);
  g9->SetLineStyle(1);
  g9->SetLineWidth(1);
  g9->SetLineColor(2);
  g9->Draw("AL");
  
  c4->cd(4);
  gPad->SetLogx();
  g10->Draw("AL");
  g10->GetXaxis()->CenterTitle(true);
  g10->GetYaxis()->CenterTitle(true);
  g10->GetXaxis()->SetTitle("A (eV)");
  g10->GetYaxis()->SetTitle("Pmt");
  g10->SetMaximum(1.05);
  g10->SetMinimum(0.0);
  g10->SetLineStyle(1);
  g10->SetLineWidth(1);
  g10->SetLineColor(2);
  g10->Draw("AL");
  
  
  c4->Print("results/test-profile-A.eps");
  
}
