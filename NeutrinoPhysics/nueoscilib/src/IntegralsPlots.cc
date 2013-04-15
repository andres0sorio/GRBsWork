// $Id: $
// Include files 



// local
#include "IntegralsPlots.h"
#include "TAxis.h"
#include "Math/GSLIntegrator.h"
#include "Math/GaussLegendreIntegrator.h"

//-----------------------------------------------------------------------------
// Implementation file for class : IntegralsPlots
//
// 2011-06-30 : Andres Osorio
//-----------------------------------------------------------------------------

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
IntegralsPlots::IntegralsPlots( const char * nuxsec, const char * antinuxsec, Parameters * input) {
  
  m_input = input;

  nu_xsec_data = std::string(nuxsec);
  antinu_xsec_data = std::string(antinuxsec);
  
  m_phi_nu[0] = m_input->GetPar11(); //phi_e
  m_phi_nu[1] = m_input->GetPar12(); //phi_mu
  m_phi_nu[2] = m_input->GetPar13(); //phi_tau

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
  
  canvasOne = new TCanvas("canvasOne","Integral One study",526,216,650,800);
  canvasOne->SetFillColor(10);
  canvasOne->Divide(1,2);

  canvasTwo = new TCanvas("canvasTwo","Integral Two study",526,216,650,800);
  canvasTwo->SetFillColor(10);
  canvasTwo->Divide(1,2);

  canvasThree = new TCanvas("canvasThree","Integral Three study",526,216,650,800);
  canvasThree->SetFillColor(10);
  canvasThree->Divide(1,2);
  
  canvasFour = new TCanvas("canvasFour","Integral Four study",526,216,650,800);
  canvasFour->SetFillColor(10);
  canvasFour->Divide(1,2);
  

  canvasFive = new TCanvas("canvasFive","Integral Five study",526,216,650,800);
  canvasFive->SetFillColor(10);
  canvasFive->Divide(1,2);

  methodOne = new TCanvas("methodOne","Integral Evaluation",526,216,650,800);
  methodOne->SetFillColor(10);
  methodOne->Divide(1,2);

  methodTwo = new TCanvas("methodTwo","Integral Evaluation",526,216,650,800);
  methodTwo->SetFillColor(10);
  methodTwo->Divide(1,2);

  m_sfactor = 1.0e5;
  
  m_x0 = 100.0;

}
//=============================================================================
// Destructor
//=============================================================================
IntegralsPlots::~IntegralsPlots() {} 

//=============================================================================
void IntegralsPlots::ConfigMethodOne( int subint, double abserr, double relerr ) 
{
  
  SubIntervals = subint;
  AbsError     = abserr;
  RelError     = relerr;
  
}

void IntegralsPlots::ConfigMethodTwo( int subint, double epsilon ) 
{
  
  SubIntervals = subint;
  Epsilon      = epsilon;
  
}

void IntegralsPlots::IntegralOne() 
{
  
  m_Numu_integral_dx * f1 = new m_Numu_integral_dx();
  
  m_input->SetPar4( m_phi_nu[1] ); // N_beta = phi_mu
  
  f1->SetData(nu_xsec_data, antinu_xsec_data);
  f1->SetParameters( m_input );

  float x0 = m_input->GetPar1();
  float xmax = m_input->GetPar2();
  float dx = 10.e4;
  float x = x0;
  
  TGraph * g1 = new TGraph();
  g1->SetTitle("Int_dEnu");
  int k = 0;
  
  while(  x < xmax) 
  {
    double y = (*f1)(x);
    g1->SetPoint( k, log10(x), y );
    x += dx;
    k += 1;
  }
  
  f1->DestroyInterpolator();
  delete f1;

  canvasOne->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  g1->SetMaximum(1.0E-38);
  g1->GetXaxis()->SetTitle("log_{10} Enu");
  g1->Draw("ACP");
  
  //..................................................................
  
  m_Numu_integral_dxdy * f2 = new m_Numu_integral_dxdy();
  
  f2->SetData(nu_xsec_data, antinu_xsec_data);
  f2->SetParameters( m_input );
  
  dx = 0.1;
  
  float enu = m_input->GetPar1();
  bool isFirst = true;
  
  while ( enu < m_input->GetPar2() ) 
  {
    
    f2->SetVar1 ( enu );
    xmax = ( enu - x0) / enu;

    if(xmax <= 0.0) { 
      enu *= 5.0;
      continue;
    }
    
    
    TGraph * g2 = new TGraph();
    g2->SetTitle("Int_dy");
    
    k = 0;
    x = 0.0;
    
    while(  x < xmax) 
    {
      double y = (*f2)(x);
      g2->SetPoint( k, x, y );
      x += dx;
      k += 1;
    }

    canvasOne->cd(2);
    gPad->SetLogy();
    gPad->SetGridx();
    gPad->SetGridy();
    
    if ( isFirst ) {
      g2->SetMaximum(1.e-35);
      g2->SetMinimum(1.e-45);
      g2->SetLineColor(2);
      g2->GetXaxis()->SetTitle("y [0,1]");
      g2->Draw("ACP");
    }
    else g2->Draw("CP");
    
    isFirst = false;
    
    enu *= 10.0;
    
  }
  
  //..................................................................
  
  canvasOne->Print("results/integralOne.ps");
    
}

void IntegralsPlots::IntegralTwo() 
{

  m_Nutau_integral_dx * f1 = new m_Nutau_integral_dx();
  
  m_input->SetPar4( m_phi_nu[2] );
  
  f1->SetData(nu_xsec_data, antinu_xsec_data);
  f1->SetParameters( m_input );
  
  float x0 = m_input->GetPar1();
  float xmax = m_input->GetPar2();
  float dx = 5.e4;
  float x = x0;
  
  TGraph * g1 = new TGraph();
  g1->SetTitle("Int_dEnu");
  int k = 0;
  
  while(  x < xmax) 
  {
    double y = (*f1)(x);
    g1->SetPoint( k, log10(x), y );
    x += dx;
    k += 1;
  }
  
  f1->DestroyInterpolator();
  delete f1;
  
  canvasTwo->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  //g1->SetMaximum(0.4E-39);
  g1->GetXaxis()->SetTitle("log_{10} Enu");
  g1->Draw("ACP");
  
  //..................................................................
  
  canvasTwo->Print("results/integralTwo.ps");
  
}

void IntegralsPlots::IntegralThree() 
{
  
  double rho  = m_input->GetPar5();
  double Area = m_input->GetPar6();
  double Na   = m_input->GetPar7();
  double Ldet = m_input->GetPar9();
  double kk   = rho * Area * Ldet * Na;

  float x0 = m_input->GetPar1();
  float xmax = m_input->GetPar2();
  float dx = 5.e4;
  float x = x0;
  
  TGraph * g1 = new TGraph();
  g1->SetTitle("Int_dEnu");

  TGraph * g2 = new TGraph();
  g2->SetTitle("K * Int_dEnu");

  int k = 0;
    
  while(  x < xmax) 
  {
    float sum = 0;
    for( int i=0; i < 3; ++i ) {
      
      m_NC_showers_integral_dx * f1 = new m_NC_showers_integral_dx();
      
      m_input->SetPar4( m_phi_nu[i] );
      
      f1->SetData(nu_xsec_data, antinu_xsec_data);
      f1->SetParameters( m_input );
      
      double y = (*f1)(x);
      
      f1->DestroyInterpolator();
      
      delete f1;
      
      sum +=  y;
    }
    
    g1->SetPoint( k, log10(x), sum );
    g2->SetPoint( k, log10(x), kk*sum );
    
    x += dx;
    k += 1;
  }
  
  canvasThree->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  g1->GetXaxis()->SetTitle("log_{10} Enu");
  g1->Draw("ACP");

  canvasThree->cd(2);
  gPad->SetGridx();
  gPad->SetGridy();
  g2->GetXaxis()->SetTitle("log_{10} Enu");
  g2->Draw("ACP");
  
  //..................................................................

  canvasThree->Print("results/integralThree.ps");
  
}

void IntegralsPlots::IntegralFour() 
{

  double rho  = m_input->GetPar5();
  double Area = m_input->GetPar6();
  double Na   = m_input->GetPar7();
  double Ldet = m_input->GetPar9();
  
  double kk   = rho * Area * Ldet * Na;
  
  m_CCnue_showers_integral_dx * f1 = new m_CCnue_showers_integral_dx();
    
  m_input->SetPar4( m_phi_nu[0] );
  
  f1->SetData(nu_xsec_data, antinu_xsec_data);
  f1->SetParameters( m_input );
  
  float x0 = m_input->GetPar1();
  float xmax = m_input->GetPar2();
  float dx = 5.e4;
  float x = x0;
  
  TGraph * g1 = new TGraph();
  g1->SetTitle("Int_dEnu");

  TGraph * g2 = new TGraph();
  g2->SetTitle("K * Int_dEnu");

  int k = 0;
  
  while(  x < xmax) 
  {
    double y = (*f1)(x);
    g1->SetPoint( k, log10(x), y );
    g2->SetPoint( k, log10(x), kk * y );
    x += dx;
    k += 1;
  }
  
  f1->DestroyInterpolator();
  delete f1;
  
  canvasFour->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  g1->GetXaxis()->SetTitle("log_{10} Enu");
  g1->Draw("ACP");

  canvasFour->cd(2);
  gPad->SetGridx();
  gPad->SetGridy();
  g2->GetXaxis()->SetTitle("log_{10} Enu");
  g2->Draw("ACP");
  
  //..................................................................
  
  canvasFour->Print("results/integralFour.ps");
  
}

void IntegralsPlots::IntegralFive() 
{

  double rho  = m_input->GetPar5();
  double Area = m_input->GetPar6();
  double Na   = m_input->GetPar7();
  double Ldet = m_input->GetPar9();
  double Br   = m_input->GetPar8();

  double kk   = (1.0 - Br) * rho * Area * Ldet * Na;
  
  m_CCnutau_showers_integral_dx * f1 = new m_CCnutau_showers_integral_dx();
    
  m_input->SetPar4( m_phi_nu[2] );
  
  f1->SetData(nu_xsec_data, antinu_xsec_data);
  f1->SetParameters( m_input );
  
  float x0 = m_input->GetPar1();
  float xmax = m_input->GetPar2();
  float dx = 5.e4;
  float x = x0;
  
  TGraph * g1 = new TGraph();
  g1->SetTitle("Int_dEnu");

  TGraph * g2 = new TGraph();
  g2->SetTitle("K * Int_dEnu");
  
  int k = 0;
  
  while(  x < xmax) 
  {
    double y = (*f1)(x);
    g1->SetPoint( k, log10(x), y );
    g2->SetPoint( k, log10(x), kk * y );
    x += dx;
    k += 1;
  }
  
  f1->DestroyInterpolator();
  delete f1;
  
  canvasFive->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  g1->GetXaxis()->SetTitle("log_{10} Enu");
  g1->Draw("ACP");

  canvasFive->cd(2);
  gPad->SetGridx();
  gPad->SetGridy();
  g2->GetXaxis()->SetTitle("log_{10} Enu");
  g2->Draw("ACP");
  
  //..................................................................
  
  canvasFive->Print("results/integralFive.ps");
  
}

void IntegralsPlots::EvalIntegralMethodOne() 
{
  
  float x0   = m_input->GetPar1();
  float xmax = m_input->GetPar2();
  float dx   = 10;
  float x    = x0*dx;
  
  TGraph * g1 = new TGraph();
  g1->SetTitle("Eval ( Int dEnu )");

  int k = 0;
  
  while( x < xmax ) 
  {
    
    double y = EvalIntegralMethodOne( x );
    g1->SetPoint( k, log10(x), y );
    x *= dx;
    k += 1;
    
  }
  
  methodOne->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  g1->GetXaxis()->SetTitle("log_{10} Enu");
  g1->Draw("ACP");
  
  //..................................................................
  
  methodOne->Print("results/integralMethodOne.ps");

}

double IntegralsPlots::EvalIntegralMethodOne( double x ) 
{
  
  float x0    = m_input->GetPar1();
  double rho  = m_input->GetPar5();
  double Area = m_input->GetPar6();
  double Na   = m_input->GetPar7();
  double kk   = rho * Area * Na * m_sfactor;
  
  m_input->SetKonst1( kk );
  
  m_Numu_integral_dx  * f1 = new m_Numu_integral_dx();
  
  m_input->SetPar4( m_phi_nu[1] ); // N_beta = phi_mu
  f1->SetData(nu_xsec_data, antinu_xsec_data);
  f1->SetParameters( m_input );
  
  //Do the integral with method1
  ROOT::Math::GSLIntegrator * nminteg = 
    new ROOT::Math::GSLIntegrator( ROOT::Math::IntegrationOneDim::kADAPTIVESINGULAR, 
                                   ROOT::Math::Integration::kGAUSS61,
                                   AbsError, RelError, SubIntervals );
  
  nminteg->SetFunction( *(ROOT::Math::IGenFunction*)f1 );
    
  double y = nminteg->Integral( x0, x);

  std::cout << log10(x) << " " << (y/m_sfactor) << std::endl;
    
  f1->DestroyInterpolator();
  delete f1;
  
  return (y/m_sfactor);
  
}

void IntegralsPlots::EvalIntegralMethodTwo() 
{
  
  std::cout << "IntegralsPlots::EvalIntegralMethodTwo>" << std::endl;
  
  float x0   = m_input->GetPar1();
  float xmax = m_input->GetPar2();
  float dx   = 10;
  float x    = x0*dx;
  
  TGraph * g1 = new TGraph();
  g1->SetTitle("Eval ( Int dEnu )");
  
  int k = 0;
  
  while( x < xmax ) 
  {
    
    double y = EvalIntegralMethodTwo( x );
    g1->SetPoint( k, log10(x), y );
    x *= dx;
    k += 1;
    
  }
  
  methodTwo->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  g1->GetXaxis()->SetTitle("log_{10} Enu");
  g1->Draw("ACP");
  
  //..................................................................
  
  methodTwo->Print("results/integralMethodTwo.ps");
  
}

double IntegralsPlots::EvalIntegralMethodTwo( double x ) 
{
  
  float x0    = m_input->GetPar1();
  double rho  = m_input->GetPar5();
  double Area = m_input->GetPar6();
  double Na   = m_input->GetPar7();
  double kk   = rho * Area * Na * m_sfactor;
  
  m_input->SetKonst1( kk );
  
  m_Numu_integral_dx * f1 = new m_Numu_integral_dx();
    
  m_input->SetPar4( m_phi_nu[1] ); // N_beta = phi_mu
  f1->SetData(nu_xsec_data, antinu_xsec_data);
  f1->SetParameters( m_input );
  
  //Do the integral with method1
  ROOT::Math::GaussLegendreIntegrator::GaussLegendreIntegrator *nminteg =
    new ROOT::Math::GaussLegendreIntegrator::GaussLegendreIntegrator( SubIntervals,
                                                                      Epsilon );
  nminteg->SetFunction( *(ROOT::Math::IGenFunction*)f1 );
  
  double y = nminteg->Integral( x0, x );
  
  std::cout << log10(x) << " " << (y/m_sfactor) << std::endl;
  
  f1->DestroyInterpolator();
  delete f1;
  
  return (y/m_sfactor);
  
}

//..........................................................................................................
//
//..........................................................................................................

double IntegralsPlots::EvalIntegralMethodOne( ROOT::Math::IGenFunction * f1, double x ) 
{
  
  //Do the integral with method1
  ROOT::Math::GSLIntegrator * nminteg = 
    new ROOT::Math::GSLIntegrator( ROOT::Math::IntegrationOneDim::kADAPTIVESINGULAR, 
                                   ROOT::Math::Integration::kGAUSS61,
                                   AbsError, RelError, SubIntervals );
  
  nminteg->SetFunction( (*f1) );
  
  double y = nminteg->Integral( m_x0, x);
  
  std::cout << log10(x) << " " << (y/m_sfactor) << std::endl;
  
  return (y/m_sfactor);
  
}

double IntegralsPlots::EvalIntegralMethodTwo( ROOT::Math::IGenFunction * f1, double x ) 
{
  
  //Do the integral with method1
  ROOT::Math::GaussLegendreIntegrator::GaussLegendreIntegrator *nminteg =
    new ROOT::Math::GaussLegendreIntegrator::GaussLegendreIntegrator( SubIntervals,
                                                                      Epsilon );
  
  nminteg->SetFunction( (*f1) );
  
  double y = nminteg->Integral( m_x0, x );
  
  std::cout << log10(x) << " " << (y/m_sfactor) << std::endl;
  
  return (y/m_sfactor);
  
}

ROOT::Math::IGenFunction* IntegralsPlots::GetIntegral( int Id ) {


  ROOT::Math::IGenFunction* integral = NULL;
  
  double rho     = m_input->GetPar5();
  double Area    = m_input->GetPar6();
  double Na      = m_input->GetPar7();
  double Br      = m_input->GetPar8();
  double Ldet    = m_input->GetPar9();
  
  double kk1     = rho * Area * Na * m_sfactor;
  double kk2     = Br * rho * Area * Na * m_sfactor;
  double kk3     = rho * Area * Ldet * Na * m_sfactor;
  double kk4     = (1.0 - Br) * rho * Area * Ldet * Na * m_sfactor;
  
  std::cout << (kk1/m_sfactor) << " " 
	    << (kk2/m_sfactor) << " " 
	    << (kk3/m_sfactor) << " " 
	    << (kk4/m_sfactor) << std::endl;

  switch( Id )
    
    {
      
    case 1: { 
      m_Numu_integral_dx * f1 = new m_Numu_integral_dx();
      m_input->SetKonst1( kk1 );
      m_input->SetPar4( m_phi_nu[1] ); 
      f1->SetData(nu_xsec_data, antinu_xsec_data);
      f1->SetParameters( m_input );
      m_x0 = m_input->GetPar1();
      integral = (ROOT::Math::IGenFunction *) f1;
      break;
    }
      
    case 2: { 
      m_Nutau_integral_dx * f1 = new m_Nutau_integral_dx(); 
      m_input->SetKonst1( kk2 );
      m_input->SetPar4( m_phi_nu[2] ); 
      f1->SetData(nu_xsec_data, antinu_xsec_data);
      f1->SetParameters( m_input );
      m_x0 = m_input->GetPar1();
      integral = (ROOT::Math::IGenFunction *) f1;
      break;
    }
      
    case 3: {
      m_NC_showers_integral_dx * f1 = new m_NC_showers_integral_dx();
      m_input->SetKonst1( kk3 );
      m_input->SetPar4( m_phi_nu[0] );
      f1->SetData(nu_xsec_data, antinu_xsec_data);
      f1->SetParameters( m_input );
      integral = (ROOT::Math::IGenFunction *) f1;
      m_x0 = m_input->GetPar10();
      break;
    }
      
    case 4: {
      m_CCnue_showers_integral_dx * f1 = new m_CCnue_showers_integral_dx();
      m_input->SetKonst1( kk3 );
      m_input->SetPar4( m_phi_nu[0] );
      f1->SetData(nu_xsec_data, antinu_xsec_data);
      f1->SetParameters( m_input );
      m_x0 = m_input->GetPar10();
      integral = (ROOT::Math::IGenFunction *) f1;
      break;
    }
      
    case 5: {
      m_CCnutau_showers_integral_dx * f1 = new m_CCnutau_showers_integral_dx();
      m_input->SetKonst1( kk4 );
      m_input->SetPar4( m_phi_nu[2] );
      f1->SetData(nu_xsec_data, antinu_xsec_data);
      f1->SetParameters( m_input );
      m_x0 = m_input->GetPar10();
      integral = (ROOT::Math::IGenFunction *) f1;
      break;
    }
      
    default: std::cout << "Warning: No integral selected" << std::endl;
      
    }
  
  std::cout << "GetIntegral> seleted Id " << Id << " " << std::endl;
  
  return integral;
  
}

//..........................................................................................................
