// $Id: $
// Include files 



// local
#include "PShadow.h"
#include "Math/GSLIntegrator.h"
#include "Math/IntegrationTypes.h"
#include "TAxis.h"

const double PI = 3.1415926;

//-----------------------------------------------------------------------------
// Implementation file for class : PShadow
//
// 2011-02-27 : Andres Osorio
//-----------------------------------------------------------------------------

double GetEarthDensity( double x )
{

  // rho [g/cm^3]: Ref Physics Reports 458 (2008) 173-246
  
  double value = 0.0;
  double xx = pow(x,2);
  double xxx = pow(x,3);
  
  if( x < 0.192 )
    value = 13.0885 - 8.8381*xx; /// AO dec 2013: corrected should be 8.8381*x^2
  else if( x > 0.192 && x < 0.546 )
    value = 12.5815 - 1.2638 * x - 3.6426 * xx - 5.5281 * xxx;
  else if( x > 0.546 && x < 0.895 ) 
    value = 7.9565 - 6.4761 * x + 5.5283 * xx - 3.0807 * xxx;
  else if( x > 0.895 && x < 0.906) 
    value = 5.3197 - 1.4836 * x;
  else if( x > 0.906 && x < 0.937) 
    value = 11.2494 - 8.0298 * x;
  else if( x > 0.937 && x < 0.965) 
    value = 7.1089 - 3.8045 * x;
  else if( x > 0.965 && x < 0.996) 
    value = 2.691 + 0.6924 * x;
  else if( x > 0.996 && x < 0.998) 
    value = 2.9;
  else if( x > 0.998 && x < 0.999) 
    value = 2.6;
  else if ( x > 0.999 && x <= 1.0) 
    value = 1.02;
  else value = -1.1111;
  
  return value;
  
}

//=============================================================================
// Clone method implementation: just a formality

ROOT::Math::IBaseFunctionOneDim* m_rho_dt::Clone() const{
  
  m_rho_dt * f1 = new m_rho_dt();
  return f1;
  
}
//=============================================================================

void m_rho_dt::SetParameters( double v_, double y0_, double r0_, double phi_) 
{
  
  m_v = v_;
  m_y0 = y0_;
  m_phi = (3.0*PI/2.0) - phi_;
  m_r0 = r0_;
   
}

double m_rho_dt::DoEval(double time) const {
  
  double rho = 1.0;
  
  rho = sqrt( (m_v*cos(m_phi)*time)*(m_v*cos(m_phi)*time) 
	      + (m_v*sin(m_phi)*time + m_y0)*(m_v*sin(m_phi)*time + m_y0) );
  
  double gg  = 1.0;
  
  gg = m_v * GetEarthDensity( rho / m_r0 );
  
  return gg;
  
}

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================

PShadow::PShadow() {
  
  m_debug = false;
  
  nu_xsec_interp = new Interpolation( "XSec_neut.dat" );
  antinu_xsec_interp = new Interpolation ( "XSec_anti.dat");

  // From Ref[] Physics Reports 458 (2008) 173-246
  m_d  = 2.0E5;
  m_r0 = 6378.0E5;
  m_C  = 3.0E10;
  m_NA = 6.022E23;
  m_y0 = m_r0 - m_d;
  
  m_pshadow_Nu = 1.0;
  m_pshadow_aNu = 1.0;

  m_full_calculation = true;
    
}

PShadow::PShadow( const std::string & xsec1, const std::string & xsec2 ) {
  
  m_debug = false;
  
  nu_xsec_interp = new Interpolation( xsec1.c_str() );
  antinu_xsec_interp = new Interpolation ( xsec2.c_str() );
  
  // From Ref[] Physics Reports 458 (2008) 173-246
  m_d  = 2.0E5;
  m_r0 = 6378.0E5; 
  m_C  = 3.0E10;
  m_NA = 6.0221415E23;
  m_y0 = m_r0 - m_d;
  
  m_pshadow_Nu = 1.0;
  m_pshadow_aNu = 1.0;

  m_full_calculation = true;
  
}

//Interpolate the PShadow term from data points

PShadow::PShadow( const std::string & infile ) {
  
  m_debug = false;
  
  m_in = new std::ifstream(infile.c_str(), ifstream::in);
  
  if(!m_in->is_open()) {
    //std::cout << "Data> cannot open file" << std::endl;
  } else { 
    //std::cout << "Data> file is now open" << std::endl;
  }
  
  m_xx.clear();
  m_nu.clear();
  m_anu.clear();
  
  double e_nu      = -.1;
  double nu_point  = -.1;
  double anu_point = -.1;
  
  while( m_in->good() ) {
    
    if ( m_in->eof() ) break;
    
    (*m_in) >> e_nu >> nu_point >> anu_point;
    
    m_xx.push_back( e_nu );
    m_nu.push_back( nu_point );
    m_anu.push_back( anu_point );
    
    if ( m_in->fail()) break;
    
  }
  
  m_xx.pop_back();
  m_nu.pop_back();
  m_anu.pop_back();
  
  m_interpolatorNu  = new  ROOT::Math::Interpolator( m_xx, m_nu, ROOT::Math::Interpolation::kCSPLINE);
  m_interpolatorANu = new  ROOT::Math::Interpolator( m_xx, m_anu, ROOT::Math::Interpolation::kCSPLINE);
  
  m_pshadow_Nu = 1.0;
  m_pshadow_aNu = 1.0;

  m_in->close();
  
  delete m_in;

  m_full_calculation = false;

  // From Ref[] Physics Reports 458 (2008) 173-246
  m_d  = 2.0E5;
  m_r0 = 6378.0E5; 
  m_C  = 3.0E10;
  m_NA = 6.0221415E23;
  m_y0 = m_r0 - m_d;
 
}


//=============================================================================
// Destructor
//=============================================================================
PShadow::~PShadow() {
  
  if( m_full_calculation ) 
  {
    
    if ( nu_xsec_interp ) delete nu_xsec_interp;
    if ( antinu_xsec_interp ) delete antinu_xsec_interp;
    
  } else {
    
    delete m_interpolatorNu;
    delete m_interpolatorANu;

  }
  
} 

//=============================================================================

double PShadow::timeAtT1( double angle )
{
  
  double z0  = (3.0*PI/2.0) - angle;
  double C2  = m_C*m_C;
  double d2  = m_d*m_d;
  double r02 = m_r0 * m_r0;
  
  double Delta = -( d2 * C2)*cos(z0)*cos(z0) + 2.0*m_d*m_r0*C2*cos(z0)*cos(z0) 
    + r02*C2*sin(z0)*sin(z0);
  
  return (1.0 / C2)*( m_d*m_C*sin(z0) - m_r0*m_C*sin(z0) - sqrt(Delta) );

}

double PShadow::Tower( double angle )
{
  
  m_rho_dt * ff = new m_rho_dt();
  
  ROOT::Math::GSLIntegrator * nminteg = 
    new ROOT::Math::GSLIntegrator( ROOT::Math::IntegrationOneDim::kADAPTIVE, 
                                   ROOT::Math::Integration::kGAUSS21,
                                   1.E-4, 1.E-6, 1000 );
  
  ff->SetParameters( m_C, m_y0, m_r0, angle );
  
  nminteg->SetFunction( *(ROOT::Math::IGenFunction*)ff );
  
  double m_t1 = timeAtT1( angle );
  double m_t0 = 0.0;
  
  double result = nminteg->Integral(m_t1, m_t0);
  
  delete ff;
  delete nminteg;
  
  return result;
  
}

void PShadow::Eval( double enu )
{
  
  //Using the default angle PI
  Eval( PI , enu );
  
}

void PShadow::Eval2( double enu )
{
  
  //AO dec 2013 - use interpolated data
  
  m_pshadow_Nu = m_interpolatorNu->Eval( enu );
  
  m_pshadow_aNu = m_interpolatorANu->Eval( enu );
  

}



void PShadow::Eval( double angle, double enu )
{
  
  double X = Tower( angle );
  
  //first for Neutrinos
  double sigma_CC_log10 = nu_xsec_interp->evaluateCC( log10(enu) );
  double sigma_CC = pow( 10.0, sigma_CC_log10);
  double sigma_NC_log10 = nu_xsec_interp->evaluateNC( log10(enu) );
  double sigma_NC = pow( 10.0, sigma_NC_log10);
  
  m_pshadow_Nu = exp ( - X * m_NA * 1 * ( sigma_CC + sigma_NC ) );
  
  //second for Anti-neutrinos
  sigma_CC_log10 = antinu_xsec_interp->evaluateCC( log10(enu) );
  sigma_CC = pow( 10.0, sigma_CC_log10);
  sigma_NC_log10 = antinu_xsec_interp->evaluateNC( log10(enu) );
  sigma_NC = pow( 10.0, sigma_NC_log10);
  
  m_pshadow_aNu = exp ( - X * m_NA * 1 * ( sigma_CC + sigma_NC ) );
  
  
}

void PShadow::Eval2( double max, double enu) 
{

  //PShadow as a Step function
  
  if ( enu < max ) {
    m_pshadow_Nu = 1.0;
    m_pshadow_aNu = 1.0;
  } else {
    m_pshadow_Nu = 0.0;
    m_pshadow_aNu = 0.0;
  }
  
}

void PShadow::Validate() 
{

  std::cout << "--------------------------------------------------" << '\n';
  std::cout << "PShadow::Validate" << std::endl;

  double max_enu = 12;
  double enu     = 1.0;
  double dx      = 0.5;
  int i = 0;

  std::cout << "Angle" << '\t' << '\t' << "Tower::X()" << '\n';
  double angle = PI;
  double min_angle = 0.0;
  double dangle = 0.6;
  
  while ( angle > min_angle ) {
    std::cout << angle << '\t' << '\t' << Tower( angle ) << std::endl;
    angle -= dangle;
  }

  g1 = new TGraph();
  g2 = new TGraph();
    
  while ( enu < max_enu ) 
    {
      
    Eval( pow( 10.0, enu) );
    
    double x1 = Nu_PShadow();
    double x2 = ANu_PShadow();
    
    g1->SetPoint( i, enu, x1);
    g2->SetPoint( i, enu, x2);
    
    i+=1;
    enu += dx;
    
  }
  
  tdrStyle = new TStyle("tdrStyle","Style for P-TDR");

  tdrStyle->SetCanvasBorderMode(0);
  tdrStyle->SetCanvasColor(kWhite);
  tdrStyle->SetTitleBorderSize(0);
  tdrStyle->SetTitleFillColor(kWhite);
  tdrStyle->SetTitleFontSize(0.04);
  tdrStyle->SetGridColor(kGray);
  tdrStyle->SetGridStyle(3);
  tdrStyle->SetGridWidth(1);
  tdrStyle->SetLabelColor(1, "XYZ");
  tdrStyle->SetLabelFont(42, "XYZ");
  tdrStyle->SetLabelOffset(0.007, "XYZ");
  tdrStyle->SetLabelSize(0.04, "XYZ");

  tdrStyle->cd();

  c1 = new TCanvas();
  c2 = new TCanvas();
  
  g1->SetLineColor(2);
  g2->SetLineColor(4);

  g1->GetXaxis()->SetTitle("log_{10} E_{#nu/#bar#nu} [GeV]");
  g1->GetYaxis()->SetTitle("Pshadow");

  c1->cd();
  g1->Draw("ACP");
  g2->Draw("CP");
  
  //////////////////////
  angle = PI;
  min_angle = 1.50;
  bool isFirst = true;
  
  while ( angle > min_angle ) {

    i = 0;
    enu = 1.0;
    
    g3 = new TGraph();
    g4 = new TGraph();

    while ( enu < max_enu ) 
      {
	
	Eval( angle, pow( 10.0, enu) );
	
	double x1 = Nu_PShadow();
	double x2 = ANu_PShadow();
	
	g3->SetPoint( i, enu, x1);
	g4->SetPoint( i, enu, x2);
	
	i+=1;
	enu += dx;
	
      }
    
    g3->SetLineColor(2);
    g4->SetLineColor(4);
  
    c2->cd();

    if( isFirst ) { g3->Draw("ACP"); isFirst = false; 
      g3->GetXaxis()->SetTitle("log_{10} E_{#nu/#bar#nu} [GeV]");
      g3->GetYaxis()->SetTitle("Pshadow");}
    else g3->Draw("CP");
    
    g4->Draw("CP");
    
    angle -= dangle;
    
  }
  
  c1->Print("results/pshadow-plots-1.eps");
  c2->Print("results/pshadow-plots-2.eps");

  delete c1;
    
  std::cout << "--------------------------------------------------" << '\n';

}
