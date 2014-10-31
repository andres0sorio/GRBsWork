// Include files 

// local
#include "MuTrackEvents.h"

//-----------------------------------------------------------------------------
// Implementation file for class : MuTrackEvents
//
// 2010-09-13 : Andres Osorio
//-----------------------------------------------------------------------------

MuTrackEvents::MuTrackEvents(const char * nuxsec, const char * antinuxsec, Parameters * input) {
  
  nu_xsec_data = std::string(nuxsec);
  antinu_xsec_data = std::string(antinuxsec);
  
  m_input = input;

  m_phi_nu[0] = m_input->GetPar11(); //phi_e
  m_phi_nu[1] = m_input->GetPar12(); //phi_mu
  m_phi_nu[2] = m_input->GetPar13(); //phi_tau
  
  //std::cout << "MuTrackEvents> " << m_phi_nu[0] << " " << m_phi_nu[1] << " " << m_phi_nu[2] << '\n';

  m_NuMuTracks = 0.0;
  
  m_NuTauTracks = 0.0;
  
  m_sfactor = Integrals::NMFactor;
    
}

MuTrackEvents::MuTrackEvents(const char * nuxsec, const char * antinuxsec, const char * pshad, Parameters * input) {
  
  nu_xsec_data = std::string(nuxsec);
  antinu_xsec_data = std::string(antinuxsec);
  pshadow_data = std::string(pshad);
  
  m_input = input;

  m_phi_nu[0] = m_input->GetPar11(); //phi_e
  m_phi_nu[1] = m_input->GetPar12(); //phi_mu
  m_phi_nu[2] = m_input->GetPar13(); //phi_tau
  
  //std::cout << "MuTrackEvents> " << m_phi_nu[0] << " " << m_phi_nu[1] << " " << m_phi_nu[2] << '\n';

  m_NuMuTracks = 0.0;
  
  m_NuTauTracks = 0.0;

  m_sfactor = Integrals::NMFactor;
    
}

float MuTrackEvents::Evaluate() {

  //... get Nu_mu / Anti-Nu_mu contribution

  float v1 = EvaluateNuMuContribution();
  
  std::cout << "MuTrackEvents> EvaluateNuMuContribution> done \n";
  
  m_NuMuTracks = v1;
    
  //... get Nu_tau / Anti-Nu_tau contribution

  float v2 = EvaluateNuTauContribution();
  
  std::cout << "MuTrackEvents> EvaluateNuTauContribution> done \n";

  m_NuTauTracks = v2;

  return (v1 + v2);

}

float MuTrackEvents::EvaluateNuMuContribution() {
  
  double rho  = m_input->GetPar5();
  double Area = m_input->GetPar6();
  double Na   = m_input->GetPar7();
  double kk   = rho * Area * Na;

  //... This is formula 5 from Ref[1]
  
  m_Numu_integral_dx * ff = new m_Numu_integral_dx();
  
  m_input->SetKonst1( kk * m_sfactor );
  
  ff->SetData(nu_xsec_data, antinu_xsec_data, pshadow_data);
  ff->SetParameters( m_input );

  /*
    ROOT::Math::GSLIntegrator * nminteg =  new ROOT::Math::GSLIntegrator( Integrals::AbsError,
    Integrals::RelError,
    Integrals::SubIntervals);
  */
  
  ROOT::Math::GSLIntegrator * nminteg =  new ROOT::Math::GSLIntegrator( Integrals::IntMethod,
                                                                        Integrals::KronRule,
                                                                        Integrals::AbsError, 
                                                                        Integrals::RelError, 
                                                                        Integrals::SubIntervals );
  
  nminteg->SetFunction( *(ROOT::Math::IGenFunction*)ff );
  
  float m_mu_Th = m_input->GetPar1();
  float m_nu_Cut = m_input->GetPar2();
  
  double result = nminteg->Integral(m_mu_Th, m_nu_Cut);
  
  ff->DestroyInterpolator();
  
  delete ff;
  delete nminteg;
  
  return (result/m_sfactor);

}

float MuTrackEvents::EvaluateNuTauContribution() {

  double rho  = m_input->GetPar5();
  double Area = m_input->GetPar6();
  double Na   = m_input->GetPar7();
  double Br   = m_input->GetPar8();
  double kk   = Br * rho * Area * Na;
  
  //... This is formula 7 from Ref[1]
  
  m_Nutau_integral_dx * ff = new m_Nutau_integral_dx();
  
  m_input->SetKonst1( kk * m_sfactor );
  
  ff->SetData(nu_xsec_data, antinu_xsec_data, pshadow_data);
  ff->SetParameters( m_input );
  
  /*
    ROOT::Math::GSLIntegrator * nminteg =  new ROOT::Math::GSLIntegrator( Integrals::AbsError,
    Integrals::RelError,
    Integrals::SubIntervals);
  */
  
  ROOT::Math::GSLIntegrator * nminteg =  new ROOT::Math::GSLIntegrator( Integrals::IntMethod,
                                                                        Integrals::KronRule,
                                                                        Integrals::AbsError, 
                                                                        Integrals::RelError, 
                                                                        Integrals::SubIntervals );
  
  nminteg->SetFunction( *(ROOT::Math::IGenFunction*)ff );
  
  float m_mu_Th = m_input->GetPar1();
  float m_nu_Cut = m_input->GetPar2();
  
  double result = nminteg->Integral(m_mu_Th, m_nu_Cut);
  
  ff->DestroyInterpolator();
  
  delete ff;
  delete nminteg;
  
  return (result/m_sfactor);
  
}

//...

float MuTrackEvents::Evaluate( double Enu ) {

  //... get Nu_mu / Anti-Nu_mu contribution

  float v1 = EvaluateNuMuContribution( Enu );

  m_NuMuTracks = v1;
    
  //... get Nu_tau / Anti-Nu_tau contribution

  float v2 = EvaluateNuTauContribution( Enu );
  
  m_NuTauTracks = v2;

  return (v1 + v2);

}

float MuTrackEvents::EvaluateNuMuContribution( double Enu ) {
  
  double rho  = m_input->GetPar5();
  double Area = m_input->GetPar6();
  double Na   = m_input->GetPar7();
  double kk   = rho * Area * Na;

  //... This is formula 5 from Ref[1]
  
  m_Numu_integral_dx * ff = new m_Numu_integral_dx();
  
  m_input->SetKonst1( kk * m_sfactor );
  
  ff->SetData(nu_xsec_data, antinu_xsec_data, pshadow_data);
  ff->SetParameters( m_input );
  
  double result = ff->DoEval( Enu );
  
  ff->DestroyInterpolator();

  delete ff;

  return (result/m_sfactor);

}

float MuTrackEvents::EvaluateNuTauContribution( double Enu ) {

  double rho  = m_input->GetPar5();
  double Area = m_input->GetPar6();
  double Na   = m_input->GetPar7();
  double Br   = m_input->GetPar8();
  double kk   = Br * rho * Area * Na;
  
  //... This is formula 7 from Ref[1]
  
  m_Nutau_integral_dx * ff = new m_Nutau_integral_dx();
  
  m_input->SetKonst1( kk * m_sfactor );
  
  ff->SetData(nu_xsec_data, antinu_xsec_data, pshadow_data);
  ff->SetParameters( m_input );

  double result = ff->DoEval( Enu );
  
  ff->DestroyInterpolator();
  
  delete ff;

  return (result/m_sfactor);
  
}


