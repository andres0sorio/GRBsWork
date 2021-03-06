// $Id: $
// Include files 

// local
#include "Integrals.h"
#include "Math/GSLIntegrator.h"
#include "Math/GaussLegendreIntegrator.h"

//-----------------------------------------------------------------------------
// Implementation file for class : Integrals
//
// 2010-09-13 : Andres Osorio
//-----------------------------------------------------------------------------

//=============================================================================
// Standard constructor, destructor and Setters of Base class Integrals
//=============================================================================

// GSL Integration parameters

const int    Integrals::SubIntervals = 1000;
const double Integrals::AbsError     = 1.0e-4;
const double Integrals::RelError     = 1.0e-3;
const double Integrals::NMFactor     = 1.0e10;

/*
  ... GSL Integration methods
  
  kGAUSS: simple Gauss integration method with fixed rule
  kLEGENDRE: Gauss-Legendre integration
  kNONADAPTIVE : to be used for smooth functions
  kADAPTIVE : to be used for general functions without singularities.
  kADAPTIVESINGULAR: default adaptive integration type which can be used in the case of the presence of singularities.

  ... Gauss-KronRod integration rule for ADAPTIVE integration type
  
  Enumerator:
  kGAUSS15 
  kGAUSS21 
  kGAUSS31 
  kGAUSS41 
  kGAUSS51 
  kGAUSS61 

*/

const ROOT::Math::Integration::Type Integrals::IntMethod = ROOT::Math::IntegrationOneDim::kADAPTIVE;
//const ROOT::Math::Integration::Type Integrals::IntMethod = ROOT::Math::IntegrationOneDim::kADAPTIVESINGULAR;

//const ROOT::Math::Integration::GKRule Integrals::KronRule = ROOT::Math::Integration::kGAUSS21;
const ROOT::Math::Integration::GKRule Integrals::KronRule = ROOT::Math::Integration::kGAUSS61;

Integrals::Integrals() { 
  m_debug = false;
  m_destroy_interp = false;
}

Integrals::~Integrals() {
  
  if ( m_destroy_interp ) {

    if (m_debug) std::cout << "destroying interpolator" << std::endl;
    
    if ( nu_xsec_interp ) delete nu_xsec_interp;
    if ( antinu_xsec_interp ) delete antinu_xsec_interp;
    if( m_pshadow )
      if (m_debug) std::cout << "destroying pshadow" << std::endl;
    delete m_pshadow;
  
  }
  
} 

void Integrals::SetParameters( Parameters * input ) 
{
  m_input = input;
}

void Integrals::SetData( const std::string & xsec1, const std::string & xsec2)
{
  
  nu_xsec_interp     = new Interpolation( xsec1.c_str() );
  antinu_xsec_interp = new Interpolation( xsec2.c_str() );
  m_pshadow          = new PShadow( xsec1, xsec2 );
  
}

void Integrals::SetData( const std::string & xsec1, const std::string & xsec2, const std::string & pshad)
{
  
  nu_xsec_interp     = new Interpolation( xsec1.c_str() );
  antinu_xsec_interp = new Interpolation( xsec2.c_str() );
  m_pshadow          = new PShadow( pshad );
  
}

//=============================================================================
// auxiliary functions:

double Rmu ( double * x )
{
  
  double E1 = x[0];
  double E2 = x[1];
  double kk = 2.6e5;
  
  double f_rmu = log ( 2.0 + ( 0.0042*E1 ) ) - log ( 2.0 + ( 0.0042*E2 ) );
  
  return kk * f_rmu;
  
}

double dFnub( double * x, double * par) 
{
  
  double alfa  = par[0];
  double Nbeta = par[1]; //this is the neutrino flux
  double f_dfnu = Nbeta * pow ( x[0], -1.0 * alfa );
  
  return f_dfnu;
  
}

double fBLimOne( double * x ) 
{
  
  double E1 = x[0]; // E_tau
  double E2 = x[1]; // E_mu
  double ff = (5.0)/(3.0*E1) 
    - (3.0 * pow( E2, 2.0 ) ) / ( pow ( E1, 3.0 ) ) 
    + (4.0 * pow( E2, 3.0 ) ) / ( 3.0 * pow ( E1, 4.0 ) );
  
  return ff;

}

double dsigmady( double * vars ) 
{

  //... AO april 2014 - fit made by Juan Carlos

  if( 1 ) {
     
    double y  = vars[0];
    double x  = log10( vars[1] ); // this takes into account the energy dependent factors( E in GeV )
    
    double a1 = -0.0163*pow(x,2.0) + 0.3877*x - 1.1905;
    double a2 = -0.0222*pow(x,2.0) + 0.4222*x - 0.9833;
    double b1 =  0.0168*pow(x,2.0) - 0.3683*x + 2.0038;
    double b2 =  0.0139*pow(x,2.0) - 0.2739*x + 1.4233;
    
    double Ycut = exp((log(b1) - log(b2))/(a1 - a2));
    
    double Norm = b1*pow(Ycut,(1.0 - a1))/(1.0 -a1);
    
    Norm += b2*(1.0 - pow(Ycut,(1.0 - a2)))/(1.0 - a2);
    
    b1 = b1/Norm;
    b2 = b2/Norm;
    
    double dsdy = 0.0;
    
    if ( y < Ycut )
      dsdy = b1*pow(y,-a1);
    else
      dsdy = b2*pow(y,-a2);
    
    return dsdy;
    
  }
  
  //... AO april 2014 - function used in Karla's thesis
  if( 0 ) 
  {
    
    double y = vars[0];
    double a = -0.932537;
    double b = 0.4231082;
    double corr = 1.0 / 0.99284; // so integral = 1.0 x sigmaCC
    return ( exp( a*y + b ) ) * corr;
    
  }
  

  //... AO preliminary version - not in use
  // return ( 5.47 * exp (-y) );
  
}

//=============================================================================
// Clone method implementation: just a formality

ROOT::Math::IBaseFunctionOneDim* m_Numu_integral_dx::Clone() const{
  
  m_Numu_integral_dx * f1 = new m_Numu_integral_dx();
  return f1;
  
}

ROOT::Math::IBaseFunctionOneDim* m_Numu_integral_dxdy::Clone() const{
  
  m_Numu_integral_dxdy * f1 = new m_Numu_integral_dxdy();
  return f1;
  
}


ROOT::Math::IBaseFunctionOneDim* m_antiNumu_integral_dx::Clone() const{
  
  m_antiNumu_integral_dx * f1 = new m_antiNumu_integral_dx();
  return f1;
  
}

ROOT::Math::IBaseFunctionOneDim* m_antiNumu_integral_dxdy::Clone() const{
  
  m_antiNumu_integral_dxdy * f1 = new m_antiNumu_integral_dxdy();
  return f1;
  
}

ROOT::Math::IBaseFunctionOneDim* m_Nutau_integral_dx::Clone() const{
  
  m_Nutau_integral_dx * f1 = new m_Nutau_integral_dx();
  return f1;
  
}

ROOT::Math::IBaseFunctionOneDim* m_Nutau_integral_dxdy::Clone() const{
  
  m_Nutau_integral_dxdy * f1 = new m_Nutau_integral_dxdy();
  return f1;
  
}

ROOT::Math::IBaseFunctionOneDim* m_Nutau_integral_dxdydz::Clone() const{
  
  m_Nutau_integral_dxdydz * f1 = new m_Nutau_integral_dxdydz();
  return f1;
  
}

// AO nov 2014

ROOT::Math::IBaseFunctionOneDim* m_antiNutau_integral_dx::Clone() const{
  
  m_antiNutau_integral_dx * f1 = new m_antiNutau_integral_dx();
  return f1;
  
}

ROOT::Math::IBaseFunctionOneDim* m_antiNutau_integral_dxdy::Clone() const{
  
  m_antiNutau_integral_dxdy * f1 = new m_antiNutau_integral_dxdy();
  return f1;
  
}

ROOT::Math::IBaseFunctionOneDim* m_antiNutau_integral_dxdydz::Clone() const{
  
  m_antiNutau_integral_dxdydz * f1 = new m_antiNutau_integral_dxdydz();
  return f1;
  
}


//=============================================================================

// Shower integrals

ROOT::Math::IBaseFunctionOneDim* m_NC_showers_integral_dx::Clone() const{
  
  m_NC_showers_integral_dx* f1 = new m_NC_showers_integral_dx();
  return f1;
  
}

ROOT::Math::IBaseFunctionOneDim* m_antiNC_showers_integral_dx::Clone() const{
  
  m_antiNC_showers_integral_dx* f1 = new m_antiNC_showers_integral_dx();
  return f1;
  
}

//.............................................................................

// 7 A and B

ROOT::Math::IBaseFunctionOneDim* m_CCnue_showers_integral_dx::Clone() const{
  
  m_CCnue_showers_integral_dx* f1 = new m_CCnue_showers_integral_dx();
  return f1;
  
}

ROOT::Math::IBaseFunctionOneDim* m_CCantinue_showers_integral_dx::Clone() const{
  
  m_CCantinue_showers_integral_dx* f1 = new m_CCantinue_showers_integral_dx();
  return f1;
  
}

//.............................................................................

// 8 A and B

ROOT::Math::IBaseFunctionOneDim* m_CCnutau_showers_integral_dx::Clone() const{
  
  m_CCnutau_showers_integral_dx* f1 = new m_CCnutau_showers_integral_dx();
  return f1;
  
}

ROOT::Math::IBaseFunctionOneDim* m_CCantinutau_showers_integral_dx::Clone() const{
  
  m_CCantinutau_showers_integral_dx* f1 = new m_CCantinutau_showers_integral_dx();
  return f1;
  
}

//.............................................................................

// Convolution integrals

ROOT::Math::IBaseFunctionOneDim* convolveXsec::Clone() const{
  
  convolveXsec * f1 = new convolveXsec();
  return f1;
  
};

ROOT::Math::IBaseFunctionOneDim* convolveXsecbar::Clone() const{
  
  convolveXsecbar * f1 = new convolveXsecbar();
  return f1;
  
}

//=============================================================================
// N.1 - A

double m_Numu_integral_dx::DoEval(double enu) const{
  
  m_Numu_integral_dxdy * ff = new m_Numu_integral_dxdy( *(Integrals*)this );
  
  ff->SetVar1 ( enu );
  
  float m_mu_Th = m_input->GetPar1();
  
  float m_y_low = 0.0;
  float m_y_upp = ( enu - m_mu_Th) / enu;  
  
  ROOT::Math::GSLIntegrator * nminteg =  new ROOT::Math::GSLIntegrator( IntMethod,
                                                                        KronRule,
                                                                        AbsError, RelError, SubIntervals );
  
  nminteg->SetFunction( *(ROOT::Math::IGenFunction*)ff );
  
  double result = nminteg->Integral(m_y_low , m_y_upp);
  
  delete ff;
  delete nminteg;
  
  return result;
  
}

// N.1 - B
double m_antiNumu_integral_dx::DoEval(double enu) const{
  
  m_antiNumu_integral_dxdy * ff = new m_antiNumu_integral_dxdy( *(Integrals*)this );
  
  ff->SetVar1 ( enu );
  
  float m_mu_Th = m_input->GetPar1();
  
  float m_y_low = 0.0;
  float m_y_upp = ( enu - m_mu_Th) / enu;  
  
  ROOT::Math::GSLIntegrator * nminteg =  new ROOT::Math::GSLIntegrator( IntMethod,
                                                                        KronRule,
                                                                        AbsError, RelError, SubIntervals );
  
  nminteg->SetFunction( *(ROOT::Math::IGenFunction*)ff );
  
  double result = nminteg->Integral(m_y_low , m_y_upp);
  
  delete ff;
  delete nminteg;
  
  return result;
  
}

//=============================================================================
// N.2 - PShadow in
double m_Numu_integral_dxdy::DoEval(double y) const {
  
  double x1[2];
  double x2[1];
  double x3[2];
  
  double par[2];
  
  // x == Enu
  
  double Emu = m_x * ( 1.0 - y );
  double sigma_CC_log10 = nu_xsec_interp->evaluateCC( log10(m_x) );
  double sigma_CC = pow( 10.0, sigma_CC_log10);

  double k1 = m_input->GetKonst1();
  
  x1[0] = Emu;
  x1[1] = m_input->GetPar1(); // Emu_Th
  
  x2[0] = m_x;

  x3[0] = y;
  x3[1] = m_x; // energy in GeV
      
  par[0] = m_input->GetPar3(); // alfa
  par[1] = m_input->GetPar("N_mu", m_x ); // N_beta = phi_mu
  
  // AO dec 2013
  // P-shadow evaluation - with interpoled data
  if ( m_input->GetPar14() > 0 ) 
    m_pshadow->Eval2( m_x );
  else
    m_pshadow->Eval();
  
  double pshadow = m_pshadow->Nu_PShadow();
  
  double ff = k1 * Rmu ( x1 ) * dFnub( x2, par) * sigma_CC * dsigmady( x3 ) * pshadow;
  
  return ff;
  
}

//=============================================================================
// N.2 - B
double m_antiNumu_integral_dxdy::DoEval(double y) const {
  
  double x1[2];
  double x2[1];
  double x3[2];
  
  double par[2];
  
  // x == Enu
  
  double Emu = m_x * ( 1.0 - y );
  double sigma_CC_log10 = antinu_xsec_interp->evaluateCC( log10( m_x) );
  double sigma_CC = pow( 10.0, sigma_CC_log10);

  double k1  = m_input->GetKonst1();
  
  x1[0] = Emu;
  x1[1] = m_input->GetPar1(); // Emu_Th
  
  x2[0] = m_x;

  x3[0] = y;
  x3[1] = m_x; // energy in GeV
      
  if ( m_input->GetPar14() > 0 ) 
    m_pshadow->Eval2( m_x );
  else
    m_pshadow->Eval();
  
  double pshadow = m_pshadow->ANu_PShadow();
  
  pshadow = m_pshadow->ANu_PShadow();
  
  par[0] = m_input->GetPar3(); // alfa
  par[1] = m_input->GetPar("N_amu", m_x); // N_beta = phi_amu
  
  double gg = k1 * Rmu ( x1 ) * dFnub( x2, par) * sigma_CC * dsigmady( x3 ) * pshadow;
  
  return gg;
  
}


//=============================================================================
// N.3
double m_Nutau_integral_dx::DoEval(double enut) const{

  m_Nutau_integral_dxdy * ff = new m_Nutau_integral_dxdy( *(Integrals*)this );
  
  ff->SetVar1 ( enut ); // ... x == Enu_tau
  
  float m_mu_Th = m_input->GetPar1();
  
  float m_y_low = 0.0;
  float m_y_upp = ( enut - m_mu_Th ) / enut;
  
  ROOT::Math::GSLIntegrator * nminteg =  new ROOT::Math::GSLIntegrator( IntMethod,
                                                                        KronRule,
                                                                        AbsError, RelError, SubIntervals );
  
  nminteg->SetFunction( *(ROOT::Math::IGenFunction*)ff );
  
  double result = nminteg->Integral(m_y_low , m_y_upp);
  
  delete ff;
  delete nminteg;
  
  return result;
  
}

//=============================================================================
// N.4
double m_Nutau_integral_dxdy::DoEval(double y) const{
  
  m_Nutau_integral_dxdydz * ff = new m_Nutau_integral_dxdydz( *(Integrals*)this );
  
  ff->SetVar1 ( m_x );
  ff->SetVar2 ( y );
  
  //std::cout << "given: y" << y << std::endl;
  
  float mu_Th = m_input->GetPar1();
  
  float M_tau = 1.7840; // GeV/c^2
  float E_tau = m_x * ( 1.0 - y );
  float Gamma = M_tau / E_tau;
  float Beta  = sqrt ( 1.0 - (Gamma*Gamma) );
  
  float z_low = mu_Th;
  float z_upp = 0.5 * E_tau * ( 1.0 + Beta);
  
  ROOT::Math::GSLIntegrator * nminteg =  new ROOT::Math::GSLIntegrator( IntMethod,
                                                                        KronRule,
                                                                        AbsError, RelError, SubIntervals );
  
  nminteg->SetFunction( *(ROOT::Math::IGenFunction*)ff );
  
  double result = nminteg->Integral(z_low , z_upp);
  
  delete ff;
  delete nminteg;
  
  return result;
  
}

//=============================================================================
// N.5 - PShadow in
double m_Nutau_integral_dxdydz::DoEval(double z) const{
  
  double xx[2];
  double yy[2];
  double x2[1];
  double x3[2];
  
  double par[2];
  
  x3[0] = m_y;
  x3[1] = m_x;

  float E_tau = m_x * ( 1.0 - m_y );
  
  double sigma_CC_log10 = nu_xsec_interp->evaluateCC( log10( m_x ) );
  double sigma_CC = pow( 10.0, sigma_CC_log10);

  double k1 = m_input->GetKonst1();
  
  xx[0] = z;
  xx[1] = m_input->GetPar1(); // Emu_Th
  
  yy[0] = E_tau;
  yy[1] = z;
  
  x2[0] = m_x;
  
  par[0] = m_input->GetPar3(); // alfa
  par[1] = m_input->GetPar("N_mu", m_x); // N_beta = phi_mu

  // AO dec 2013
  // P-shadow evaluation - with interpoled data
  if ( m_input->GetPar14() > 0 ) 
    m_pshadow->Eval2( m_x );
  else
    m_pshadow->Eval();
  
  double pshadow = m_pshadow->Nu_PShadow();
  
  double ff = k1 * Rmu ( xx ) * dFnub( x2, par) * fBLimOne( yy ) * sigma_CC * dsigmady( x3 ) * pshadow;
  
  return ff;
  
}

//=============================================================================
// N.3 - B

double m_antiNutau_integral_dx::DoEval(double enut) const{
  
  m_antiNutau_integral_dxdy * ff = new m_antiNutau_integral_dxdy( *(Integrals*)this );
  
  ff->SetVar1 ( enut ); // ... x == Enu_tau
  
  float m_mu_Th = m_input->GetPar1();
  
  float m_y_low = 0.0;
  float m_y_upp = ( enut - m_mu_Th ) / enut;
  
  ROOT::Math::GSLIntegrator * nminteg =  new ROOT::Math::GSLIntegrator( IntMethod,
                                                                        KronRule,
                                                                        AbsError, RelError, SubIntervals );
    
  nminteg->SetFunction( *(ROOT::Math::IGenFunction*)ff );
  
  double result = nminteg->Integral(m_y_low , m_y_upp);
  
  delete ff;
  delete nminteg;
  
  return result;
  
}

//=============================================================================
// N.4 - B
double m_antiNutau_integral_dxdy::DoEval(double y) const{
  
  m_antiNutau_integral_dxdydz * ff = new m_antiNutau_integral_dxdydz( *(Integrals*)this );
  
  ff->SetVar1 ( m_x );
  ff->SetVar2 ( y );
  
  float mu_Th = m_input->GetPar1();
  
  float M_tau = 1.7840; // GeV/c^2
  float E_tau = m_x * ( 1.0 - y );
  float Gamma = M_tau / E_tau;
  float Beta  = sqrt ( 1.0 - (Gamma*Gamma) );
  
  float z_low = mu_Th;
  float z_upp = 0.5 * E_tau * ( 1.0 + Beta);
  
  ROOT::Math::GSLIntegrator * nminteg =  new ROOT::Math::GSLIntegrator( IntMethod,
                                                                        KronRule,
                                                                        AbsError, RelError, SubIntervals );
  
  nminteg->SetFunction( *(ROOT::Math::IGenFunction*)ff );
  
  double result = nminteg->Integral(z_low , z_upp);
  
  delete ff;
  delete nminteg;
  
  return result;
  
}

//=============================================================================
// N.5 - B
double m_antiNutau_integral_dxdydz::DoEval(double z) const{
  
  double xx[2];
  double yy[2];
  double x2[1];
  double x3[2];
  
  double par[2];
  
  x3[0] = m_y;
  x3[1] = m_x;
  
  float E_tau = m_x * ( 1.0 - m_y );
  
  double sigma_CC_log10 = antinu_xsec_interp->evaluateCC( log10( m_x ) );
  double sigma_CC = pow( 10.0, sigma_CC_log10);
  
  double k1 = m_input->GetKonst1();
  
  xx[0] = z;
  xx[1] = m_input->GetPar1(); // Emu_Th
  
  yy[0] = E_tau;
  yy[1] = z;
  
  x2[0] = m_x;
  
  if ( m_input->GetPar14() > 0 ) 
    m_pshadow->Eval2( m_x );
  else
    m_pshadow->Eval();
  
  double pshadow = m_pshadow->ANu_PShadow();
  
  par[0] = m_input->GetPar3(); // alfa
  par[1] = m_input->GetPar("N_amu", m_x); // N_beta = phi_mu
  
  double gg = k1 * Rmu ( xx ) * dFnub( x2, par) * fBLimOne( yy ) * sigma_CC * dsigmady( x3 ) * pshadow;
  
  return gg;
  
}

//=============================================================================
// N.6 - PShadow in
double m_NC_showers_integral_dx::DoEval(double x) const{
  
  double xx[2];
  double par[2];
  
  double sigma_NC_log10 = nu_xsec_interp->evaluateNC( log10( x ) );
  double sigma_NC = pow( 10.0, sigma_NC_log10);

  double k1 = m_input->GetKonst1();
  
  xx[0]  = x;
  
  par[0] = m_input->GetPar3(); // alfa
  par[1] = m_input->GetPar( m_flavour[m_fv].c_str() , x); // N_beta = phi_(e,mu,tau)
  
  // AO dec 2013
  // P-shadow evaluation - with interpoled data
  if ( m_input->GetPar14() > 0 ) 
    m_pshadow->Eval2( x );
  else
    m_pshadow->Eval();
 
  double pshadow = m_pshadow->Nu_PShadow();
  
  double ff = k1 * dFnub( xx, par) * sigma_NC * pshadow ;
  
  return ( ff );
  
}

// N.6 - B
double m_antiNC_showers_integral_dx::DoEval(double x) const{
  
  double xx[2];
  double par[2];
  
  double k1 = m_input->GetKonst1();
  
  xx[0]  = x;
  
  if ( m_input->GetPar14() > 0 ) 
    m_pshadow->Eval2( x );
  else
    m_pshadow->Eval();
  
  double sigma_NC_log10 = antinu_xsec_interp->evaluateNC( log10(x) );
  double sigma_NC = pow( 10.0, sigma_NC_log10);
  
  double pshadow = m_pshadow->ANu_PShadow();
  
  par[0] = m_input->GetPar3(); // alfa
  par[1] = m_input->GetPar( m_flavour[m_fv].c_str() , x); // N_beta = phi_(ae,amu,atau)
  
  double gg = k1 * dFnub( xx, par) * sigma_NC * pshadow ;
  
  return ( gg );
  
}

//=============================================================================
// N.7 - PShadow in
double m_CCnue_showers_integral_dx::DoEval(double x) const{

  double xx[2];
  double par[2];
  
  double sigma_CC_log10 = nu_xsec_interp->evaluateCC( log10( x ) );
  double sigma_CC = pow( 10.0, sigma_CC_log10);

  double k1 = m_input->GetKonst1();
  
  xx[0]  = x;
  
  par[0] = m_input->GetPar3(); // alfa
  par[1] = m_input->GetPar("N_e", x); // N_beta = phi_e
 
  // AO dec 2013
  // P-shadow evaluation - with interpoled data
  if ( m_input->GetPar14() > 0 ) 
    m_pshadow->Eval2( x );
  else
    m_pshadow->Eval();

  double pshadow = m_pshadow->Nu_PShadow();
  
  double ff = k1 * dFnub( xx, par) * sigma_CC * pshadow;
  
  return ( ff );
  
}

// N.7 - B - PShadow in

double m_CCantinue_showers_integral_dx::DoEval(double x) const{
  
  double xx[2];
  double par[2];
  
  double k1 = m_input->GetKonst1();
  
  xx[0]  = x;
  
  // AO dec 2013
  // P-shadow evaluation - with interpoled data
  if ( m_input->GetPar14() > 0 ) 
    m_pshadow->Eval2( x );
  else
    m_pshadow->Eval();
  
  double sigma_CC_log10 = antinu_xsec_interp->evaluateCC( log10(x) );
  double sigma_CC = pow( 10.0, sigma_CC_log10);

  double pshadow = m_pshadow->ANu_PShadow();

  par[0] = m_input->GetPar3(); // alfa
  par[1] = m_input->GetPar("N_ae", x); // N_beta = phi_ae
  
  double gg = k1 * dFnub( xx, par) * sigma_CC * pshadow;
  
  return ( gg );
  
}

//=============================================================================
// N.8 - PShadow in
double m_CCnutau_showers_integral_dx::DoEval(double x) const{

  double xx[2];
  double par[2];
  
  double sigma_CC_log10 = nu_xsec_interp->evaluateCC( log10( x ) );
  double sigma_CC = pow( 10.0, sigma_CC_log10);

  double k1 = m_input->GetKonst1();
  
  xx[0]  = x;
  
  par[0] = m_input->GetPar3(); // alfa
  par[1] = m_input->GetPar("N_tau", x ); // N_beta = phi_tau
  
  // AO dec 2013
  // P-shadow evaluation - with interpoled data
  if ( m_input->GetPar14() > 0 ) 
    m_pshadow->Eval2( x );
  else
    m_pshadow->Eval();

  double pshadow = m_pshadow->Nu_PShadow();
  
  double ff = k1 * dFnub( xx, par) * sigma_CC * pshadow;
  
  return ( ff );
  
}

// N.8 - B 
double m_CCantinutau_showers_integral_dx::DoEval(double x) const{
  
  double xx[2];
  double par[2];
  
  double k1 = m_input->GetKonst1();
  
  xx[0]  = x;
  
  // AO dec 2013
  // P-shadow evaluation - with interpoled data
  if ( m_input->GetPar14() > 0 ) 
    m_pshadow->Eval2( x );
  else
    m_pshadow->Eval();
  
  double pshadow = m_pshadow->Nu_PShadow();
  
  double sigma_CC_log10 = antinu_xsec_interp->evaluateCC( log10( x ) );
  double sigma_CC = pow( 10.0, sigma_CC_log10);
  
  pshadow = m_pshadow->ANu_PShadow();

  par[0] = m_input->GetPar3(); // alfa
  par[1] = m_input->GetPar("N_atau", x); // N_beta = phi_atau

  double gg = k1 * dFnub( xx, par) * sigma_CC * pshadow;
  
  return ( gg );
  
}

//=============================================================================
//AO dec 2013

double convolveXsec::DoEval(double x) const{

  double sigma_CC = EvalBox( m_x0 - x );
  double result = 1.0 * sigma_CC;
  return result;
    
};

double convolveXsec::EvalBox(double x) const
{
  
  if( x < 10.0 || x > 1.0E12 ) return 0.0; //Range in GeV
  else {
    
    double sigma_CC_log10 = nu_xsec_interp->evaluateCC( log10( x ));
    double sigma_CC = pow( 10.0, sigma_CC_log10);

    return sigma_CC;
    
  }
  
}

//=============================================================================
//AO dec 2013

double convolveXsecbar::DoEval(double x) const{

  double sigma_CC = EvalBox( m_x0 - x ); // in GeV
  double result = 1.0 * sigma_CC;
  return result;
    
}

double convolveXsecbar::EvalBox(double x) const
{
  
  if( x < 10.0 || x > 1.0E12) return 0.0; // range is in GeV
  else {
    
    double sigma_CC_log10 = antinu_xsec_interp->evaluateCC( log10( x ) );
    double sigma_CC = pow( 10.0, sigma_CC_log10);
    
    return sigma_CC;
    
  }
  
}
