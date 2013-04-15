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

///Constant parameters
const int    Integrals::SubIntervals = 1000;
const double Integrals::AbsError = 1.e-4;
const double Integrals::RelError = 1.e-3;

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

double dsigmady( double * x ) 
{

  double y = x[0];
  
  
  double a = -0.932537;
  double b = 0.4231082;
  double corr = 1.0 / 0.99284; // so integral = 1.0 x sigmaCC
  return ( exp( a*y + b ) ) * corr;
  
  
  //return ( 5.47 * exp (-y) );
  
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

ROOT::Math::IBaseFunctionOneDim* m_NC_showers_integral_dx::Clone() const{
  
  m_NC_showers_integral_dx* f1 = new m_NC_showers_integral_dx();
  return f1;
  
}

ROOT::Math::IBaseFunctionOneDim* m_CCnue_showers_integral_dx::Clone() const{
  
  m_CCnue_showers_integral_dx* f1 = new m_CCnue_showers_integral_dx();
  return f1;
  
}

ROOT::Math::IBaseFunctionOneDim* m_CCnutau_showers_integral_dx::Clone() const{
  
  m_CCnutau_showers_integral_dx* f1 = new m_CCnutau_showers_integral_dx();
  return f1;
  
}

//=============================================================================
// N.1
double m_Numu_integral_dx::DoEval(double enu) const{
  
  m_Numu_integral_dxdy * ff = new m_Numu_integral_dxdy( *(Integrals*)this );
  
  ff->SetVar1 ( enu );
  
  float m_mu_Th = m_input->GetPar1();
  
  float m_y_low = 0.0;
  float m_y_upp = ( enu - m_mu_Th) / enu;  
  
  ROOT::Math::GSLIntegrator * nminteg =   new ROOT::Math::GSLIntegrator( Integrals::AbsError,
                                                                         Integrals::RelError,
                                                                         Integrals::SubIntervals);
  
  /*
    new ROOT::Math::GSLIntegrator( ROOT::Math::IntegrationOneDim::kADAPTIVE,
    ROOT::Math::Integration::kGAUSS31,
    AbsError, RelError, SubIntervals );
  */

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
  double x3[1];
  
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
    
  par[0] = m_input->GetPar3(); // alfa
  par[1] = m_input->GetPar4(); // N_beta = phi_mu
  
  //P-shadow as a step function
  //m_pshadow->Eval( 1e5, m_x );
  
  //P-shadow always == 1
  m_pshadow->Eval();

  //P-shadow normal evaluation
  //m_pshadow->Eval( m_x );
 
  double pshadow = m_pshadow->Nu_PShadow();
  
  double ff = k1 * 0.5 * Rmu ( x1 ) * dFnub( x2, par) * sigma_CC * dsigmady( x3 ) * pshadow;
  
  sigma_CC_log10 = antinu_xsec_interp->evaluateCC( log10( m_x) );
  sigma_CC = pow( 10.0, sigma_CC_log10);
  
  pshadow = m_pshadow->ANu_PShadow();
    
  double gg = k1 * 0.5 * Rmu ( x1 ) * dFnub( x2, par) * sigma_CC * dsigmady( x3 ) * pshadow;
  
  return ( ff + gg );
  
}

//=============================================================================
// N.3
double m_Nutau_integral_dx::DoEval(double enut) const{

  //std::cout << "given: enut" << enut << std::endl;
  
  m_Nutau_integral_dxdy * ff = new m_Nutau_integral_dxdy( *(Integrals*)this );
  
  ff->SetVar1 ( enut ); // ... x == Enu_tau
  
  float m_mu_Th = m_input->GetPar1();
  
  float m_y_low = 0.0;
  float m_y_upp = ( enut - m_mu_Th ) / enut;
  
  ROOT::Math::GSLIntegrator * nminteg = new ROOT::Math::GSLIntegrator( Integrals::AbsError,
                                                                       Integrals::RelError,
                                                                       Integrals::SubIntervals);
  
  /*
    new ROOT::Math::GSLIntegrator( ROOT::Math::IntegrationOneDim::kADAPTIVE,
    ROOT::Math::Integration::kGAUSS31,
    AbsError, RelError, SubIntervals );
  */

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
    
  ROOT::Math::GSLIntegrator * nminteg = new ROOT::Math::GSLIntegrator( Integrals::AbsError,
                                                                       Integrals::RelError,
                                                                       Integrals::SubIntervals);
  
  /*
    new ROOT::Math::GSLIntegrator( ROOT::Math::IntegrationOneDim::kADAPTIVE,
    ROOT::Math::Integration::kGAUSS31,
    AbsError, RelError, SubIntervals );
  */

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
  double x3[1];
  
  double par[2];
  
  x3[0] = m_y;
  
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
  par[1] = m_input->GetPar4(); // N_beta
  
  //P-shadow normal evaluation
  //m_pshadow->Eval(m_x);

  //P-shadow always == 1
  m_pshadow->Eval();
  
  double pshadow = m_pshadow->Nu_PShadow();
    
  double ff = k1 * 0.5 * Rmu ( xx ) * dFnub( x2, par) * fBLimOne( yy ) * sigma_CC * dsigmady( x3 ) * pshadow;
  
  sigma_CC_log10 = antinu_xsec_interp->evaluateCC( log10( m_x ) );
  sigma_CC = pow( 10.0, sigma_CC_log10);
  
  pshadow = m_pshadow->ANu_PShadow();
  
  double gg = k1 * 0.5 * Rmu ( xx ) * dFnub( x2, par) * fBLimOne( yy ) * sigma_CC * dsigmady( x3 ) * pshadow;
  
  return ( ff + gg );
  
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
  par[1] = m_input->GetPar4(); // N_beta
  
  //P-shadow normal evaluation
  //m_pshadow->Eval( x );

  //P-shadow always == 1
  m_pshadow->Eval();

  double pshadow = m_pshadow->Nu_PShadow();
  
  double ff = k1 * 0.5 * dFnub( xx, par) * sigma_NC * pshadow ;
  
  sigma_NC_log10 = antinu_xsec_interp->evaluateNC( log10(x) );
  sigma_NC = pow( 10.0, sigma_NC_log10);

  pshadow = m_pshadow->ANu_PShadow();
  
  double gg = k1 * 0.5 * dFnub( xx, par) * sigma_NC * pshadow ;
  
  return ( ff + gg );
  
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
  par[1] = m_input->GetPar4(); // N_beta

  //P-shadow normal evaluation
  //m_pshadow->Eval( x );

  //P-shadow always == 1
  m_pshadow->Eval();

  double pshadow = m_pshadow->Nu_PShadow();
  
  double ff = k1 * 0.5 * dFnub( xx, par) * sigma_CC * pshadow;
  
  sigma_CC_log10 = antinu_xsec_interp->evaluateCC( log10(x) );
  sigma_CC = pow( 10.0, sigma_CC_log10);

  pshadow = m_pshadow->ANu_PShadow();

  double gg = k1 * 0.5 * dFnub( xx, par) * sigma_CC * pshadow;
  
  return ( ff + gg );
  
}

//=============================================================================
// N.8 - PShadow in
double m_CCnutau_showers_integral_dx::DoEval(double x) const{

  double xx[2];
  double par[2];
  
  double sigma_CC_log10 = nu_xsec_interp->evaluateCC( log10( x ) );
  double sigma_CC = pow( 10.0, sigma_CC_log10);

  double k1 = m_input->GetKonst1();
  
  //double sigma_CC = sigma_CC_log10;

  xx[0]  = x;
  
  par[0] = m_input->GetPar3(); // alfa
  par[1] = m_input->GetPar4(); // N_beta

  //P-shadow normal evaluation
  //m_pshadow->Eval( x );

  //P-shadow always == 1
  m_pshadow->Eval();

  double pshadow = m_pshadow->Nu_PShadow();

  double ff = k1 * 0.5 * dFnub( xx, par) * sigma_CC * pshadow;

  sigma_CC_log10 = antinu_xsec_interp->evaluateCC( log10( x ) );
  sigma_CC = pow( 10.0, sigma_CC_log10);
  
  //sigma_CC = sigma_CC_log10;

  pshadow = m_pshadow->ANu_PShadow();
  
  double gg = k1 * 0.5 * dFnub( xx, par) * sigma_CC * pshadow;
  
  return ( ff + gg );
  
}

//=============================================================================
