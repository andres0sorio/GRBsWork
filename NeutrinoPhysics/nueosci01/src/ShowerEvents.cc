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

  m_phi_nu[0] = m_input->GetPar11();
  m_phi_nu[1] = m_input->GetPar12();
  m_phi_nu[2] = m_input->GetPar13();
    
  //std::cout << m_phi_nu[0] << " " << m_phi_nu[1] << " " << m_phi_nu[2] << '\n';
  
}

float ShowerEvents::Evaluate() {

  float v1 = EvaluateNCContribution();
  
  float v2 = EvaluateCCNueContribution();

  float v3 = EvaluateCCNutauContribution();

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
      
      m_input->SetPar4( m_phi_nu[i] ); // N_beta = phi_beta
      m_input->SetKonst1( kk * m_sfactor );
      
      ff->SetData(nu_xsec_data, antinu_xsec_data);
      ff->SetParameters( m_input );
      
      ROOT::Math::GSLIntegrator * nminteg = 
	new ROOT::Math::GSLIntegrator( ROOT::Math::IntegrationOneDim::kADAPTIVESINGULAR,
				       ROOT::Math::Integration::kGAUSS61,
				       Integrals::AbsError, 
				       Integrals::RelError, 
				       Integrals::SubIntervals );
      
      nminteg->SetFunction( *(ROOT::Math::IGenFunction*)ff );
      
      float m_sh_Th = m_input->GetPar10();
      float m_nu_Cut = m_input->GetPar2();
      
      double result = nminteg->Integral(m_sh_Th, m_nu_Cut);
      
      ff->DestroyInterpolator();
      
      delete ff;
      delete nminteg;
      
      sum +=  result;
      
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
  
  m_input->SetPar4( m_phi_nu[0] ); // N_beta = phi_beta
  m_input->SetKonst1( kk * m_sfactor );
  
  ff->SetData(nu_xsec_data, antinu_xsec_data);
  ff->SetParameters( m_input );
  
  ROOT::Math::GSLIntegrator * nminteg = 
    new ROOT::Math::GSLIntegrator( ROOT::Math::IntegrationOneDim::kADAPTIVESINGULAR,
				   ROOT::Math::Integration::kGAUSS61,
				   Integrals::AbsError, 
				   Integrals::RelError, 
				   Integrals::SubIntervals );
  
  nminteg->SetFunction( *(ROOT::Math::IGenFunction*)ff );
  
  float m_sh_Th = m_input->GetPar10();
  float m_nu_Cut = m_input->GetPar2();
  
  double result = nminteg->Integral(m_sh_Th, m_nu_Cut);
  
  ff->DestroyInterpolator();
  
  delete ff;
  delete nminteg;
  
  return (result/m_sfactor);
  
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
  
  m_input->SetPar4( m_phi_nu[2] ); // N_beta = phi_beta
  m_input->SetKonst1( kk * m_sfactor );
  
  ff->SetData(nu_xsec_data, antinu_xsec_data);
  ff->SetParameters( m_input );
  
  ROOT::Math::GSLIntegrator * nminteg = 
    new ROOT::Math::GSLIntegrator( ROOT::Math::IntegrationOneDim::kADAPTIVESINGULAR,
				   ROOT::Math::Integration::kGAUSS61,
				   Integrals::AbsError, 
				   Integrals::RelError, 
				   Integrals::SubIntervals );
  
  nminteg->SetFunction( *(ROOT::Math::IGenFunction*)ff );
  
  float m_sh_Th = m_input->GetPar10();
  float m_nu_Cut = m_input->GetPar2();
  
  double result = nminteg->Integral(m_sh_Th, m_nu_Cut);
  
  ff->DestroyInterpolator();
  
  delete ff;
  delete nminteg;

  return  (result/m_sfactor);
  
}
