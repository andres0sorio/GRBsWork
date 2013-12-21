// Include files 

// local
#include "ShowerEvents.h"

//-----------------------------------------------------------------------------
// Implementation file for class : ShowerEvents
//
// 2010-09-15 : Andres Osorio
//-----------------------------------------------------------------------------

ShowerEvents::ShowerEvents(const char * nuxsec, const char * antinuxsec, Parameters * input) {
  
  nu_xsec_data = std::string(nuxsec);
  antinu_xsec_data = std::string(antinuxsec);
  m_input = input;

  m_phi_nu[0] = m_input->GetPar("N_e");
  m_phi_nu[1] = m_input->GetPar("N_mu");
  m_phi_nu[2] = m_input->GetPar("N_tau");
    
  m_phi_anu[0] = m_input->GetPar("N_ae");
  m_phi_anu[1] = m_input->GetPar("N_amu");
  m_phi_anu[2] = m_input->GetPar("N_atau");
  
  std::cout << "ShowerEvents> Fluxes " << m_phi_nu[0] << " " << m_phi_nu[1] << " " << m_phi_nu[2] << '\n';
  std::cout << "ShowerEvents> Fluxes " << m_phi_anu[0] << " " << m_phi_anu[1] << " " << m_phi_anu[2] << '\n';
  
  m_NCShower = 0.0;
  
  m_CCNuShower = 0.0;
  
  m_CCNutauShower = 0.0;
  
}

float ShowerEvents::Evaluate() {

  float v1 = EvaluateNCContribution();
  
  m_NCShower = v1;
  
  float v2 = EvaluateCCNueContribution();

  m_CCNuShower = v2;

  float v3 = EvaluateCCNutauContribution();

  m_CCNutauShower = v3;
  
  return (v1 + v2 + v3);

}

float ShowerEvents::EvaluateNCContribution() 
{

  double m_sfactor = 1.e5;
  float sum   = 0.0;
  
  double rho  = m_input->GetPar5();
  double Area = m_input->GetPar6();
  double Na   = m_input->GetPar7();
  double Ldet = m_input->GetPar9();
  double kk   = rho * Area * Ldet * Na;
  
  //
  for( int i=0; i < 3; ++i ) 
  {
      
    m_NC_showers_integral_dx * ff = new m_NC_showers_integral_dx();

    m_antiNC_showers_integral_dx * gg = new m_antiNC_showers_integral_dx();
    
    // AO dec 2013
    m_input->SetPar( "N_beta", m_phi_nu[i] ); // N_beta = phi_(e,mu,tau)
    m_input->SetPar( "N_abeta", m_phi_anu[i] ); // N_beta = phi_(ae,amu,atau)
    
    m_input->SetKonst1( kk * m_sfactor );
    
    ff->SetData(nu_xsec_data, antinu_xsec_data);
    ff->SetParameters( m_input );

    gg->SetData(nu_xsec_data, antinu_xsec_data);
    gg->SetParameters( m_input );
    
    ROOT::Math::GSLIntegrator * nminteg = new ROOT::Math::GSLIntegrator( Integrals::AbsError,
                                                                         Integrals::RelError,
                                                                         Integrals::SubIntervals);
    
    /*
      new ROOT::Math::GSLIntegrator( ROOT::Math::IntegrationOneDim::kADAPTIVE,
      ROOT::Math::Integration::kGAUSS31,
      Integrals::AbsError, 
      Integrals::RelError, 
      Integrals::SubIntervals );
    */
    
    nminteg->SetFunction( *(ROOT::Math::IGenFunction*)ff );
    
    float m_sh_Th = m_input->GetPar10();
    float m_nu_Cut = m_input->GetPar2();
    
    double resultA = nminteg->Integral(m_sh_Th, m_nu_Cut);
      
    nminteg->SetFunction( *(ROOT::Math::IGenFunction*)gg );

    double resultB = nminteg->Integral(m_sh_Th, m_nu_Cut);
    
    ff->DestroyInterpolator();

    gg->DestroyInterpolator();
      
    delete ff;
    delete gg;
    delete nminteg;
      
    sum +=  ( resultA + resultB );
    
  }
  
  return (sum/m_sfactor);
  
}


float ShowerEvents::EvaluateCCNueContribution() 
{
  
  double m_sfactor = 1.e5;
  
  double rho  = m_input->GetPar5();
  double Area = m_input->GetPar6();
  double Na   = m_input->GetPar7();
  double Ldet = m_input->GetPar9();
  double kk   = rho * Area * Ldet * Na;
  
  m_CCnue_showers_integral_dx * ff = new m_CCnue_showers_integral_dx();

  m_CCantinue_showers_integral_dx * gg = new m_CCantinue_showers_integral_dx();
  
  m_input->SetKonst1( kk * m_sfactor );
  
  ff->SetData(nu_xsec_data, antinu_xsec_data);
  ff->SetParameters( m_input );
  
  gg->SetData(nu_xsec_data, antinu_xsec_data);
  gg->SetParameters( m_input );
  
  ROOT::Math::GSLIntegrator * nminteg = new ROOT::Math::GSLIntegrator( Integrals::AbsError,
                                                                       Integrals::RelError,
                                                                       Integrals::SubIntervals);
  
  /*
    new ROOT::Math::GSLIntegrator( ROOT::Math::IntegrationOneDim::kADAPTIVE,
    ROOT::Math::Integration::kGAUSS31,
    Integrals::AbsError, 
    Integrals::RelError, 
    Integrals::SubIntervals );
  */
  
  nminteg->SetFunction( *(ROOT::Math::IGenFunction*)ff );
  
  float m_sh_Th  = m_input->GetPar10();
  float m_nu_Cut = m_input->GetPar2();
  
  double resultA = nminteg->Integral(m_sh_Th, m_nu_Cut);
  
  ff->DestroyInterpolator();

  nminteg->SetFunction( *(ROOT::Math::IGenFunction*)gg );
  
  double resultB = nminteg->Integral(m_sh_Th, m_nu_Cut);
  
  gg->DestroyInterpolator();
  
  delete ff;
  delete gg;
  
  delete nminteg;
  
  return (( resultA + resultB ) / m_sfactor);
  
}

float ShowerEvents::EvaluateCCNutauContribution() 
{
  
  double m_sfactor = 1.e5;

  double rho  = m_input->GetPar5();
  double Area = m_input->GetPar6();
  double Na   = m_input->GetPar7();
  double Ldet = m_input->GetPar9();
  double Br   = m_input->GetPar8();
  double kk   = (1.0 - Br) * rho * Area * Ldet * Na;
  
  m_CCnutau_showers_integral_dx * ff = new m_CCnutau_showers_integral_dx();

  m_CCantinutau_showers_integral_dx * gg = new m_CCantinutau_showers_integral_dx();
  
  m_input->SetKonst1( kk * m_sfactor );
  
  ff->SetData(nu_xsec_data, antinu_xsec_data);
  ff->SetParameters( m_input );

  gg->SetData(nu_xsec_data, antinu_xsec_data);
  gg->SetParameters( m_input );
  
  ROOT::Math::GSLIntegrator * nminteg = new ROOT::Math::GSLIntegrator( Integrals::AbsError,
                                                                       Integrals::RelError,
                                                                       Integrals::SubIntervals);
  
  /*
    new ROOT::Math::GSLIntegrator( ROOT::Math::IntegrationOneDim::kADAPTIVE,
    ROOT::Math::Integration::kGAUSS31,
    Integrals::AbsError, 
    Integrals::RelError, 
    Integrals::SubIntervals );
  */
  
  nminteg->SetFunction( *(ROOT::Math::IGenFunction*)ff );
  
  float m_sh_Th = m_input->GetPar10();
  float m_nu_Cut = m_input->GetPar2();
  
  double resultA = nminteg->Integral(m_sh_Th, m_nu_Cut);
  
  ff->DestroyInterpolator();
  
  nminteg->SetFunction( *(ROOT::Math::IGenFunction*)gg );
  
  double resultB = nminteg->Integral(m_sh_Th, m_nu_Cut);
  
  gg->DestroyInterpolator();
  
  delete ff;
  delete gg;
  
  delete nminteg;

  return  ( ( resultA + resultB ) / m_sfactor);
  
}

//...

float ShowerEvents::Evaluate( double Enu ) {

  float v1 = EvaluateNCContribution( Enu );
  
  float v2 = EvaluateCCNueContribution( Enu );

  float v3 = EvaluateCCNutauContribution( Enu );

  return (v1 + v2 + v3);
  
}

float ShowerEvents::EvaluateNCContribution( double Enu ) 
{

  double m_sfactor = 1.e5;
  float sum   = 0.0;
  
  double rho  = m_input->GetPar5();
  double Area = m_input->GetPar6();
  double Na   = m_input->GetPar7();
  double Ldet = m_input->GetPar9();
  double kk   = rho * Area * Ldet * Na;
  
  //
  for( int i=0; i < 3; ++i ) 
  {
    
    m_NC_showers_integral_dx * ff = new m_NC_showers_integral_dx();
    
    // AO dec 2013
    m_input->SetPar( "N_beta", m_phi_nu[i] ); // N_beta = phi_(e,mu,tau)
    m_input->SetPar( "N_abeta", m_phi_anu[i] ); // N_beta = phi_(ae,amu,atau)

    m_input->SetKonst1( kk * m_sfactor );
    
    ff->SetData(nu_xsec_data, antinu_xsec_data);
    ff->SetParameters( m_input );
    
    double result = ff->DoEval( Enu );
    
    ff->DestroyInterpolator();
    
    delete ff;
    
    sum +=  result;
    
  }
  
  //m_sfactor = 1.0;

  return (sum/m_sfactor);
  
}


float ShowerEvents::EvaluateCCNueContribution( double Enu ) 
{
  
  double m_sfactor = 1.e5;
  
  double rho  = m_input->GetPar5();
  double Area = m_input->GetPar6();
  double Na   = m_input->GetPar7();
  double Ldet = m_input->GetPar9();
  double kk   = rho * Area * Ldet * Na;
  
  m_CCnue_showers_integral_dx * ff = new m_CCnue_showers_integral_dx();
  
  m_input->SetKonst1( kk * m_sfactor );
  
  ff->SetData(nu_xsec_data, antinu_xsec_data);
  ff->SetParameters( m_input );
  
  double result = ff->DoEval( Enu );
 
  ff->DestroyInterpolator();
  
  delete ff;

  //m_sfactor = 1.0;

  return (result/m_sfactor);
  
}

float ShowerEvents::EvaluateCCNutauContribution( double Enu ) 
{
  
  double m_sfactor = 1.e5;

  double rho  = m_input->GetPar5();
  double Area = m_input->GetPar6();
  double Na   = m_input->GetPar7();
  double Ldet = m_input->GetPar9();
  double Br   = m_input->GetPar8();
  double kk   = (1.0 - Br) * rho * Area * Ldet * Na;
  
  m_CCnutau_showers_integral_dx * ff = new m_CCnutau_showers_integral_dx();
  
  m_input->SetKonst1( kk * m_sfactor );
  
  ff->SetData(nu_xsec_data, antinu_xsec_data);
  ff->SetParameters( m_input );
  
  double result = ff->DoEval( Enu );
  
  ff->DestroyInterpolator();
  
  delete ff;

  //m_sfactor = 1.0;
  
  return  (result/m_sfactor);
  
}
