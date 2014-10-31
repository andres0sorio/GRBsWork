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
  
  m_NCShower = 0.0;
  m_CCNuShower = 0.0;
  m_CCNutauShower = 0.0;

  m_sfactor = Integrals::NMFactor;
    
}

ShowerEvents::ShowerEvents(const char * nuxsec, const char * antinuxsec, const char * pshad, Parameters * input) {
  
  nu_xsec_data = std::string(nuxsec);
  antinu_xsec_data = std::string(antinuxsec);
  pshadow_data = std::string(pshad);
  
  m_input = input;

  m_phi_nu[0] = m_input->GetPar("N_e");
  m_phi_nu[1] = m_input->GetPar("N_mu");
  m_phi_nu[2] = m_input->GetPar("N_tau");
    
  m_phi_anu[0] = m_input->GetPar("N_ae");
  m_phi_anu[1] = m_input->GetPar("N_amu");
  m_phi_anu[2] = m_input->GetPar("N_atau");
  
  m_NCShower = 0.0;
  m_CCNuShower = 0.0;
  m_CCNutauShower = 0.0;

  m_sfactor = Integrals::NMFactor;
  
}

float ShowerEvents::Evaluate() {

  float v1 = EvaluateNCContribution();
  
  m_NCShower = v1;

  std::cout << "ShowerEvents> EvaluateNCContribution> done \n";
  
  float v2 = EvaluateCCNueContribution();

  m_CCNuShower = v2;

  std::cout << "ShowerEvents> EvaluateCCNueContribution> done \n";

  float v3 = EvaluateCCNutauContribution();

  m_CCNutauShower = v3;

  std::cout << "ShowerEvents> EvaluateCCNutauContribution> done \n";
  
  return (v1 + v2 + v3);

}

float ShowerEvents::EvaluateNCContribution() 
{

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
    ff->SetFlavour ( i );
    gg->SetFlavour ( i );
  
    m_input->SetKonst1( kk * m_sfactor );
    
    ff->SetData(nu_xsec_data, antinu_xsec_data, pshadow_data);
    ff->SetParameters( m_input );

    gg->SetData(nu_xsec_data, antinu_xsec_data, pshadow_data);
    gg->SetParameters( m_input );
    
    /*
      ROOT::Math::GSLIntegrator * nminteg = new ROOT::Math::GSLIntegrator( Integrals::AbsError,
      Integrals::RelError,
      Integrals::SubIntervals);
    */
    
    ROOT::Math::GSLIntegrator * nminteg =  new ROOT::Math::GSLIntegrator( Integrals::IntMethod,
                                                                          Integrals::KronRule,
                                                                          Integrals::AbsError, 
                                                                          Integrals::RelError, 
                                                                          Integrals::SubIntervals );
    
    nminteg->SetFunction( *(ROOT::Math::IGenFunction*)ff );
    
    float m_sh_Th = m_input->GetPar10();
    float m_nu_Cut = m_input->GetPar2();
    
    double resultA = nminteg->Integral(m_sh_Th, m_nu_Cut);

    //std::cout << "ShowerEvents::EvaluateNCContribution> ff " << nminteg->Error( )/m_sfactor << std::endl;
    
    ff->DestroyInterpolator();

    delete ff;

    nminteg->SetFunction( *(ROOT::Math::IGenFunction*)gg );
    
    double resultB = nminteg->Integral(m_sh_Th, m_nu_Cut);

    //std::cout << "ShowerEvents::EvaluateNCContribution> gg " << nminteg->Error( )/m_sfactor << std::endl;

    gg->DestroyInterpolator();
    
    delete gg;
    
    delete nminteg;
      
    sum +=  ( resultA + resultB );
    
  }
  
  return (sum/m_sfactor);
  
}


float ShowerEvents::EvaluateCCNueContribution() 
{
  
  double rho  = m_input->GetPar5();
  double Area = m_input->GetPar6();
  double Na   = m_input->GetPar7();
  double Ldet = m_input->GetPar9();
  double kk   = rho * Area * Ldet * Na;
  
  m_CCnue_showers_integral_dx * ff = new m_CCnue_showers_integral_dx();

  m_CCantinue_showers_integral_dx * gg = new m_CCantinue_showers_integral_dx();
  
  m_input->SetKonst1( kk * m_sfactor );
  
  ff->SetData(nu_xsec_data, antinu_xsec_data, pshadow_data);
  ff->SetParameters( m_input );
  
  gg->SetData(nu_xsec_data, antinu_xsec_data, pshadow_data);
  gg->SetParameters( m_input );
  
  /*
    ROOT::Math::GSLIntegrator * nminteg = new ROOT::Math::GSLIntegrator( Integrals::AbsError,
    Integrals::RelError,
    Integrals::SubIntervals);
  */

  ROOT::Math::GSLIntegrator * nminteg =  new ROOT::Math::GSLIntegrator( Integrals::IntMethod,
                                                                        Integrals::KronRule,
                                                                        Integrals::AbsError, 
                                                                        Integrals::RelError, 
                                                                        Integrals::SubIntervals );
  
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
  
  double rho  = m_input->GetPar5();
  double Area = m_input->GetPar6();
  double Na   = m_input->GetPar7();
  double Ldet = m_input->GetPar9();
  double Br   = m_input->GetPar8();
  double kk   = (1.0 - Br) * rho * Area * Ldet * Na;
  
  m_CCnutau_showers_integral_dx * ff = new m_CCnutau_showers_integral_dx();

  m_CCantinutau_showers_integral_dx * gg = new m_CCantinutau_showers_integral_dx();
  
  m_input->SetKonst1( kk * m_sfactor );
  
  ff->SetData(nu_xsec_data, antinu_xsec_data, pshadow_data);
  ff->SetParameters( m_input );

  gg->SetData(nu_xsec_data, antinu_xsec_data, pshadow_data);
  gg->SetParameters( m_input );
  
  /*
    ROOT::Math::GSLIntegrator * nminteg = new ROOT::Math::GSLIntegrator( Integrals::AbsError,
    Integrals::RelError,
    Integrals::SubIntervals);
  */
  
  ROOT::Math::GSLIntegrator * nminteg =  new ROOT::Math::GSLIntegrator( Integrals::IntMethod,
                                                                        Integrals::KronRule,
                                                                        Integrals::AbsError, 
                                                                        Integrals::RelError, 
                                                                        Integrals::SubIntervals );

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
    ff->SetFlavour ( i );

    m_input->SetKonst1( kk * m_sfactor );
    
    ff->SetData(nu_xsec_data, antinu_xsec_data, pshadow_data);
    ff->SetParameters( m_input );
    
    double result = ff->DoEval( Enu );
    
    ff->DestroyInterpolator();
    
    delete ff;
    
    sum +=  result;
    
  }
  
  return (sum/m_sfactor);
  
}


float ShowerEvents::EvaluateCCNueContribution( double Enu ) 
{
  
  double rho  = m_input->GetPar5();
  double Area = m_input->GetPar6();
  double Na   = m_input->GetPar7();
  double Ldet = m_input->GetPar9();
  double kk   = rho * Area * Ldet * Na;
  
  m_CCnue_showers_integral_dx * ff = new m_CCnue_showers_integral_dx();
  
  m_input->SetKonst1( kk * m_sfactor );
  
  ff->SetData(nu_xsec_data, antinu_xsec_data, pshadow_data);
  ff->SetParameters( m_input );
  
  double result = ff->DoEval( Enu );
 
  ff->DestroyInterpolator();
  
  delete ff;

  return (result/m_sfactor);
  
}

float ShowerEvents::EvaluateCCNutauContribution( double Enu ) 
{
  
  double rho  = m_input->GetPar5();
  double Area = m_input->GetPar6();
  double Na   = m_input->GetPar7();
  double Ldet = m_input->GetPar9();
  double Br   = m_input->GetPar8();
  double kk   = (1.0 - Br) * rho * Area * Ldet * Na;
  
  m_CCnutau_showers_integral_dx * ff = new m_CCnutau_showers_integral_dx();
  
  m_input->SetKonst1( kk * m_sfactor );
  
  ff->SetData(nu_xsec_data, antinu_xsec_data, pshadow_data);
  ff->SetParameters( m_input );
  
  double result = ff->DoEval( Enu );
  
  ff->DestroyInterpolator();
  
  delete ff;

  return  (result/m_sfactor);
  
}
