// $Id: $
// Include files 



// local
#include "NeutrinosDetectionPaper.h"
#include <stdlib.h>

//-----------------------------------------------------------------------------
// Implementation file for class : NeutrinosDetectionPaper
//
// 2013-03-29 : Andres Osorio
//-----------------------------------------------------------------------------

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
NeutrinosDetectionPaper::NeutrinosDetectionPaper( Parameters * pars ) : Graphics() {
  
  m_debug = true;

  m_config = pars;
  m_output_file = new TFile("detection.root","RECREATE");

  char* pPath;
  pPath = getenv ("PAPER_DATA");

  if ( pPath != NULL) {
    m_data_path = std::string( pPath );
    std::cout << "NeutrinosDetectionPaper> Paper data path set to: " << m_data_path << std::endl;
  }
  else {
    std::cout << "NeutrinosDetectionPaper> PAPER_DATA envirnoment variable not found! " << std::endl;
    exit(1);
  }

  m_data_pshadow   = m_data_path;
  m_data_xsec_neut = m_data_path;
  m_data_xsec_anti = m_data_path;
  
  m_data_pshadow.append("/pshadow-at-180.dat");
  m_data_xsec_neut.append("/XSec_neut.dat");
  m_data_xsec_anti.append("/XSec_anti.dat" );
  
  ///Set energy bins

  m_e_min  = m_config->GetPar1() * 1.0E9;
  m_e_max  = m_config->GetPar2() * 1.0E9;
  
  //
  // Energy binning - IceCube energy resolution of 30%: DEx = 0.30Ex - Set by parameter 4 "DeltaE" in config file
  //
  
  double DEx  = m_config->GetPar4();

  std::cout << "NeutrinosDetectionPaper> Energy resolution at detector set to: " << DEx << std::endl;
  
  double Xx   = m_e_min;
  
  int k = 1;
  
  m_vbins = new float[500];
  
  m_vbins[0] = Xx; // This is the first lower edge bin value
  m_energy_bin[0] = Xx;
  
  while( Xx < m_e_max ) 
  {
    
    double DeltaBin = DEx * Xx;
    double low_bin = Xx;
    double upp_bin = Xx + DeltaBin;
    
    std::cout << "NeutrinosDetectionPaper> Energy binning: " << low_bin << " " << upp_bin << std::endl;
    
    Xx = upp_bin;

    m_vbins[k] = Xx;
    m_energy_bin[k] = Xx;
    
    k += 1;
        
  }
  
  m_e_bins = ( m_energy_bin.size() - 1); //Array has to have nbins+1 size
  
  for( int i = 0; i <= m_e_bins; ++ i )
    std::cout << "NeutrinosDetectionPaper> Bins lower edges data: " << m_vbins[i] << " " << m_energy_bin[i] << std::endl;
  
  std::cout << "NeutrinosDetectionPaper> Total bins set " << m_e_bins << " - " << (m_energy_bin.size()-1) << std::endl;
  
  
}
//=============================================================================
// Destructor
//=============================================================================
NeutrinosDetectionPaper::~NeutrinosDetectionPaper() {
  
  m_output_file->Close();

  delete[] m_vbins;
    
  std::cout << "NeutrinosDetectionPaper> cleanly destroyed" << std::endl;
  
} 

//=============================================================================

void NeutrinosDetectionPaper::MakeVariation01(const char * model, const char * target, const char * source, const char * var
                                              , double dCP)
{
 
  // source ----> (progragation) -----> target
  //
  // This method reproduces Olga Mena's reference Figure 5 - the shower-to-muon ratio
  //

  TString variation = TString("RvsEv") + TString("_") + TString(var);
  
  InitOutput(model, target, source, variation.Data() );
  
  //.........................................................................................

  // Run the convolution
    
  std::map<std::string, TH1F*> xsecConv_histos;
  std::map<std::string, TH1F*> xsecbarConv_histos;
  
  std::map<std::string,std::string> xsecConvolve;
  std::map<std::string,std::string> xsecbarConvolve;
  
  xsecConvolve["phi_e"]       = std::string("phi_e_conv");
  xsecConvolve["phi_mu"]      = std::string("phi_mu_conv");
  xsecConvolve["phi_tau"]     = std::string("phi_tau_conv");
  
  xsecbarConvolve["phi_ae"]   = std::string("phi_ae_conv");
  xsecbarConvolve["phi_amu"]  = std::string("phi_amu_conv");
  xsecbarConvolve["phi_atau"] = std::string("phi_atau_conv");
  
  std::map<std::string,std::string>::iterator itr;
  
  int max_bins = m_energy_bin.size();
  
  std::cout << " max_bins " << max_bins << std::endl;
  
  for( itr = xsecConvolve.begin(); itr != xsecConvolve.end(); ++itr) 
  {
    
    convolveXsec * ff = new convolveXsec();
    
    ff->SetData(m_data_xsec_neut.c_str(), m_data_xsec_anti.c_str(), m_data_pshadow.c_str());
    ff->SetParameters( m_config );
    
    TH1F * h1C = (TH1F*)m_flux_histos[ itr->first ]->Clone( itr->second.c_str() );
    
    xsecConv_histos[itr->second] = h1C;
            
    for( int k = 1; k < max_bins; ++k) {
      
      double x0 = m_flux_histos[itr->first]->GetBinCenter(k)/1.0e9; // In GeV
      
      ff->SetPoint( x0 );
      
      double xmin = m_flux_histos[itr->first]->GetBinLowEdge(k)/1.0E9; // In GeV
      double xmax =  x0;
      
      ROOT::Math::GSLIntegrator * nminteg =  new ROOT::Math::GSLIntegrator( ROOT::Math::IntegrationOneDim::kADAPTIVESINGULAR,
                                                                            ROOT::Math::Integration::kGAUSS61,
                                                                            Integrals::AbsError, 
                                                                            Integrals::RelError, 
                                                                            Integrals::SubIntervals );
      
      nminteg->SetFunction( *(ROOT::Math::IGenFunction*)ff );
      
      double result = nminteg->Integral( xmin, xmax );
      
      double phi = m_flux_histos[itr->first]->GetBinContent(k);
      
      h1C->SetBinContent( k, phi*result );
      
      //std::cout << " convolve: x0 " << x0 << " " << phi*result << std::endl;
      
      delete nminteg;
      
    }
    
    ff->DestroyInterpolator();
    
    delete ff;
    
  }
    
  //
  
  for( itr = xsecbarConvolve.begin(); itr != xsecbarConvolve.end(); ++itr) 
  {
    
    convolveXsecbar * ff = new convolveXsecbar();
    
    ff->SetData(m_data_xsec_neut.c_str(), m_data_xsec_anti.c_str(), m_data_pshadow.c_str());
    ff->SetParameters( m_config );
    
    TH1F * h1C = (TH1F*)m_flux_histos[ itr->first ]->Clone( itr->second.c_str() );
    
    xsecbarConv_histos[itr->second] = h1C;
    
    for( int k = 1; k < max_bins; ++k) {
      
      double x0 = m_flux_histos[itr->first]->GetBinCenter(k)/1.0e9; 
      
      ff->SetPoint( x0 );
      
      double xmin = m_flux_histos[itr->first]->GetBinLowEdge(k)/1.0E9;
      double xmax =  x0;
      
      ROOT::Math::GSLIntegrator * nminteg =  new ROOT::Math::GSLIntegrator( ROOT::Math::IntegrationOneDim::kADAPTIVESINGULAR,
                                                                            ROOT::Math::Integration::kGAUSS61,
                                                                            Integrals::AbsError, 
                                                                            Integrals::RelError, 
                                                                            Integrals::SubIntervals );
      
      nminteg->SetFunction( *(ROOT::Math::IGenFunction*)ff );
      
      double result = nminteg->Integral( xmin, xmax );

      double phi = m_flux_histos[itr->first]->GetBinContent(k);
      
      h1C->SetBinContent( k, phi * result );
      
      //std::cout << " convolve-bar: x0 " << x0 << " " << phi*result << std::endl;
      
      delete nminteg;
      
    }
    
    ff->DestroyInterpolator();
    
    delete ff;
    
  }
  
  //.........................................................................................
  TH1F * Tks;
  TH1F * Shw = (TH1F*)xsecConv_histos["phi_e_conv"]->Clone("Showers");
  
  Shw->Add( xsecbarConv_histos["phi_ae_conv"] );

  if( dCP == 0 ) 
  {
    Shw->Add( xsecConv_histos["phi_tau_conv"] );
    Shw->Add( xsecbarConv_histos["phi_atau_conv"] );
    Tks = (TH1F*)xsecConv_histos["phi_mu_conv"]->Clone("Tracks");
    Tks->Add ( xsecbarConv_histos["phi_amu_conv"] );
  } 
  else if ( dCP == 180.0 ) 
  {
    Shw->Add( xsecConv_histos["phi_mu_conv"] );
    Shw->Add( xsecbarConv_histos["phi_amu_conv"] );
    Tks = (TH1F*)xsecConv_histos["phi_tau_conv"]->Clone("Tracks");
    Tks->Add ( xsecbarConv_histos["phi_atau_conv"] );
  } 
  else { } 
    
  Shw->Divide( Tks );
  
  std::cout << " max_bins " << max_bins << std::endl;
  
  for( int k = 1; k < max_bins; ++k) {
    
    m_Xx       = Shw->GetBinLowEdge(k);
    m_MuTks    = Tks->GetBinContent(k);
    m_TauTks   = 1.0;
    m_HadShw   = 1.0;
    m_HadShwE  = 1.0;
    m_HadShwT  = 1.0;
    m_HadShwNC = 1.0;
    
    m_Ratio  =  Shw->GetBinContent(k);
    
    std::cout << "NeutrinosDetectionPaper> "
              << "Ev "      << m_Xx       << '\t'
              << "muTrk "   << m_MuTks    << '\t' 
              << "tauTrk "  << m_TauTks   << '\t'
              << "hadShow " << m_HadShw   << '\t'
              << "R "       << m_Ratio    << '\n';
        
    m_tree->Fill();
    
    
  }
  
  m_tree->Write();
  
  //// clean up memory
  
  xsecConvolve.clear();
  xsecbarConvolve.clear();
  
  std::map<std::string, TH1F*>::iterator histoItr;
  
  for( histoItr = xsecConv_histos.begin(); histoItr != xsecConv_histos.end(); ++histoItr)
    delete histoItr->second;
  
  for( histoItr = xsecbarConv_histos.begin(); histoItr != xsecbarConv_histos.end(); ++histoItr)
    delete histoItr->second;
  
  xsecConv_histos.clear();
  xsecbarConv_histos.clear();
  
  ////
  
  m_output_file->cd("../");
    
}

void NeutrinosDetectionPaper::MakeVariationStdPicture(const char * target, 
                                                      const char * source,
                                                      const char * option,
                                                      double Xmin, 
                                                      double Xmax, 
                                                      double Dx)
{
 
  //.......................................................................................................................
  // Make the variation of R vs Alpha for the Standard Picture 1:1:1
  
  NeutrinoOscInVacuum * m_Physics_Vacuum =  new NeutrinoOscInVacuum( m_mixpars );
  
  m_Physics_Vacuum->use_default_pars = false; // -> read parameters from xml file
  m_Physics_Vacuum->initializeAngles();
  m_Physics_Vacuum->updateMixingMatrix();
  m_Physics_Vacuum->calcProbabilities();
  m_Physics_Vacuum->Propagate( 1.0, 2.0, 0.0 );
  
  std::cout << "MakeVariationStdPicture> alpha: " << m_config->GetPar3() << " dCP: " << m_Physics_Vacuum->m_dCP << std::endl;
  
  //.......................................................................................................................
  //
  // Propagation through Earth -- (Oscillations in var density is energy dependent).
  // 06/Oct/2014 - AO
  // Added propragation through Earth
  //
  
  ModelParameterList modparlist;
  
  std::cout << " ---ModelParameterList---" << '\n';
  
  if (modparlist.ParseFile( "model_config_Earth.xml" ) == 0)
    std::cout << modparlist;
  else {
    std::cout << "ModelParameterList> Problem opening model config.";
    return;
  }
  
  ModelParameters *modpars =  modparlist.GetParameters(target);
  
  //.......................................................................................................................

  TString Source = TString("Vacuum") + TString("_") + TString(option);

  this->SetModelParameters( modpars );
  this->PropagateThroughEarth( "EarthB", "Vacuum" , option, "Pee" ,  1.0, 1.0, 1.0 );
  this->PropagateThroughEarth( "EarthB", "Vacuum" , option, "aPee",  1.0, 1.0, 1.0 );
  this->SetFluxHistograms( m_output_file, "StdPicture", "EarthB", Source.Data(), "RvsAlpha" );

  //.......................................................................................................................

  InitOutput("StdPicture", target, source, option);
  
  // Loop over the varying parameter

  m_Xx = Xmin;
  
  m_config->UseVaryingNbeta( true );

  while ( 1 ) {
    
    if ( m_Xx >= Xmax ) break;
    
    m_config->SetPar3( m_Xx ); // par3 == alpha
    
    if(m_debug) std::cout << "MakeVariationStdPicture> using Alfa= " << m_config->GetPar3() << std::endl; // Par3 == alpha

    MuTrackEvents * mu1 = new MuTrackEvents(m_data_xsec_neut.c_str(), m_data_xsec_anti.c_str(), 
                                            m_data_pshadow.c_str(), m_config );
    
    double TkSum = mu1->Evaluate( );

    m_MuTks  = mu1->m_NuMuTracks;
    m_TauTks = mu1->m_NuTauTracks;

    ShowerEvents * sh1 =  new ShowerEvents(m_data_xsec_neut.c_str(), m_data_xsec_anti.c_str(), 
                                           m_data_pshadow.c_str(), m_config );
    
    m_HadShw   = sh1->Evaluate( );
    
    m_HadShwE  = sh1->m_CCNuShower;
    
    m_HadShwT  = sh1->m_CCNutauShower;
    
    m_HadShwNC = sh1->m_NCShower;
    
    m_Ratio    = TkSum / m_HadShw;

    std::cout << "NeutrinosDetectionPaper> "
              << m_Xx     << '\t'
              << m_MuTks  << '\t' 
              << m_TauTks << '\t'
              << m_HadShw << '\t'
              << m_Ratio  << std::endl;
    
    m_tree->Fill();
    
    m_Xx = m_Xx + Dx;
    
    delete sh1;
    delete mu1;
    
    
  }
  
  m_tree->Write();
  
  m_output_file->cd("../");
  
  delete m_Physics_Vacuum;
  

}

void NeutrinosDetectionPaper::MakeVariation02(const char * model,
                                              const char * target, 
                                              const char * source,
                                              const char * option,
                                              double Xmin, 
                                              double Xmax, 
                                              double Dx)
{

  //.......................................................................................................................
  // This Code should produce a graph of R vs Alpha for the Models A,B,C
  // You need to pass the generated files with fluxes up to Earth

  InitOutput(model, target, source, option);
  
  // Here is the loop for the parameter variation
  
  m_Xx = Xmin;

  m_config->UseVaryingNbeta( true );

  std::cout << (*m_config) << std::endl;
  
  while ( 1 ) {
    
    if ( m_Xx >= Xmax ) break;
    
    m_config->SetPar3( m_Xx ); // Par3 == alpha
    
    if(m_debug) std::cout << "MakeVariation02> using Alfa= " << m_config->GetPar3() << std::endl; // Par3 == alpha

    MuTrackEvents * mu1 = new MuTrackEvents(m_data_xsec_neut.c_str(), m_data_xsec_anti.c_str(), 
                                            m_data_pshadow.c_str(), m_config );

    double TkSum = mu1->Evaluate( );
    
    m_MuTks  = mu1->m_NuMuTracks;
    m_TauTks = mu1->m_NuTauTracks;
    
    ShowerEvents * sh1 =  new ShowerEvents(m_data_xsec_neut.c_str(), m_data_xsec_anti.c_str(), 
                                           m_data_pshadow.c_str(), m_config );
    
    m_HadShw = sh1->Evaluate( );
    
    m_HadShwE = sh1->m_CCNuShower;
    
    m_HadShwT = sh1->m_CCNutauShower;
    
    m_HadShwNC = sh1->m_NCShower;

    m_Ratio  = TkSum / m_HadShw;
    
    std::cout << "NeutrinosDetectionPaper> "
              << "alpha "   << m_Xx     << '\t'
              << "muTrk "   << m_MuTks  << '\t' 
              << "tauTrk "  << m_TauTks << '\t'
              << "hadShow " << m_HadShw << '\t'
              << "R "       << m_Ratio  << std::endl;
    
    m_tree->Fill();
    
    delete mu1;
    delete sh1;
    
    m_Xx = m_Xx + Dx;
    
  }
  
  m_tree->Write();
  
  m_output_file->cd("../");
  
}

void NeutrinosDetectionPaper::MakeVariation03(const char * model,
                                              const char * target, 
                                              const char * source, 
                                              double Xmin, 
                                              double Xmax, 
                                              double Dx)
{
  
  //
  // This is the method for calculating R as function of the neutrino fraction phi_nue
  // StdPicture - reproduces results as in literature
  //

  InitOutput(model, target, source, "phi_nue");

  float phi_nue = 0.0;
  float phi_ne_fr = 0.0;
  float phi_nmu_fr = 0.0;
  float phi_ntau_fr = 0.0;

  m_config->SetPar3( 2.0 );

  if(m_debug) std::cout << "MakeVariation03> using Alfa= " << m_config->GetPar3() << std::endl; // Par3 == alpha

  phi_nue = Xmin;

  // Here is the loop for the parameter variation
  
  while ( 1 ) {
    
    if ( phi_nue >= Xmax ) break;
    if ( phi_nue > 20 ) Dx = 100.0;

    float sum = phi_nue + 2.0;

    phi_ne_fr = (phi_nue / sum);
    phi_nmu_fr = (1.0 - phi_ne_fr) / 2.0;
    phi_ntau_fr = phi_nmu_fr;
    
    m_config->SetPar("N_e", phi_ne_fr );
    m_config->SetPar("N_mu", phi_nmu_fr );
    m_config->SetPar("N_tau", phi_ntau_fr );
    
    m_config->SetPar("N_ae", phi_ne_fr );
    m_config->SetPar("N_amu", phi_nmu_fr );
    m_config->SetPar("N_atau", phi_ntau_fr );
  
    MuTrackEvents * mu1 = new MuTrackEvents(m_data_xsec_neut.c_str(), m_data_xsec_anti.c_str(), 
                                            m_data_pshadow.c_str(), m_config );
    
    double TkSum = mu1->Evaluate( );
    
    m_MuTks  = mu1->m_NuMuTracks;
    m_TauTks = mu1->m_NuTauTracks;
    
    ShowerEvents * sh1 =  new ShowerEvents(m_data_xsec_neut.c_str(), m_data_xsec_anti.c_str(), 
                                           m_data_pshadow.c_str(), m_config );
    
    m_HadShw = sh1->Evaluate( );
    
    m_HadShwE = sh1->m_CCNuShower;
    
    m_HadShwT = sh1->m_CCNutauShower;
    
    m_HadShwNC = sh1->m_NCShower;

    m_Xx = phi_nue / sum;
        
    m_Ratio  = TkSum / m_HadShw;
    
    std::cout << "NeutrinosDetectionPaper> "
              << m_Xx     << '\t'
              << m_MuTks  << '\t' 
              << m_TauTks << '\t'
              << m_HadShw << '\t'
              << m_Ratio  << std::endl;
    
    m_tree->Fill();
    
    delete mu1;
    delete sh1;
    
    phi_nue = phi_nue + Dx;
    
    //break;
    
  }
  
  m_tree->Write();
  
  m_output_file->cd("../");
  
  this->Reset(); // Clear content of the auxiliary histograms
  
  
}

void NeutrinosDetectionPaper::MakeVariation04(const char * model,
                                              const char * target, 
                                              const char * source, 
                                              double Xmin,
                                              double Xmax, 
                                              double Dx,
                                              double alpha)
{
  
  ///
  /// Variation of R as a function of theta13 - for the Standard Picture
  ///

  NeutrinoOscInVacuum * m_Physics_Vacuum =  new NeutrinoOscInVacuum( m_mixpars );
  
  m_Physics_Vacuum->use_default_pars = false; // read parameters from file
  m_Physics_Vacuum->initializeAngles();
  
  std::stringstream Var;
  double phase = m_mixpars->GetPar9();
  
  Var << "Sin2Q13-" << alpha << "-dCP" << phase;
  
  InitOutput(model, target, source, Var.str().c_str() );
  
  double Xx = Xmin;

  m_config->SetPar3( alpha );

  if(m_debug) std::cout << "MakeVariation04> using Alfa= " << m_config->GetPar3() << std::endl; // Par3 == alpha
  if(m_debug) std::cout << "MakeVariation04> using dCP= " << m_Physics_Vacuum->m_dCP << std::endl;
  
  while(1) 
  {
    
    if( Xx >= Xmax ) break;
    
    double sin2theta = pow ( sin( Xx * M_PI / 180.0), 2.0 );
    
    m_Physics_Vacuum->setAngle(1, 3, Xx);

    m_Physics_Vacuum->updateMixingMatrix();

    m_Physics_Vacuum->calcProbabilities();
    
    m_Physics_Vacuum->Propagate( 1.0, 2.0, 0.0 );
    
    m_config->SetPar("N_e", (m_Physics_Vacuum->m_phi_e_f) );
    m_config->SetPar("N_mu", (m_Physics_Vacuum->m_phi_mu_f) );
    m_config->SetPar("N_tau", (m_Physics_Vacuum->m_phi_tau_f) );

    double N1bar = (m_Physics_Vacuum->m_phi_e_f);
    double N2bar = (m_Physics_Vacuum->m_phi_mu_f);
    double N3bar = (m_Physics_Vacuum->m_phi_tau_f);
    
    m_config->SetPar("N_ae", N1bar );
    m_config->SetPar("N_amu", N2bar );
    m_config->SetPar("N_atau", N3bar );
    
    MuTrackEvents * mu1 = new MuTrackEvents(m_data_xsec_neut.c_str(), m_data_xsec_anti.c_str(), 
                                            m_data_pshadow.c_str(), m_config );
    
    double TkSum = mu1->Evaluate( );
    
    m_MuTks  = mu1->m_NuMuTracks;
    m_TauTks = mu1->m_NuTauTracks;
    
    ShowerEvents  * sh1 = new ShowerEvents(m_data_xsec_neut.c_str(), m_data_xsec_anti.c_str(),
                                           m_data_pshadow.c_str(), m_config );
    
    m_HadShw = sh1->Evaluate( );
    
    m_HadShwE = sh1->m_CCNuShower;
    
    m_HadShwT = sh1->m_CCNutauShower;
    
    m_HadShwNC = sh1->m_NCShower;
   
    m_Ratio  = TkSum / m_HadShw;

    m_Xx = sin2theta;
    
    std::cout << "NeutrinosDetectionPaper> "
              << m_Xx     << '\t'
              << m_MuTks  << '\t' 
              << m_TauTks << '\t'
              << m_HadShw << '\t'
              << m_Ratio  << std::endl;
    
    m_tree->Fill();

    Xx = Xx + Dx;
    
    delete mu1;
    delete sh1;
    
  }
  
  m_tree->Write();
  
  m_output_file->cd("../");
  
  delete m_Physics_Vacuum;

}

void NeutrinosDetectionPaper::EvaluateR(const char * model,
                                        const char * target, 
                                        const char * source, 
                                        const char * option,
                                        double pointX )
{
  
  // October 10 2014 - AO
  // Variation of R as a function of theta13 - for any model
  // No variation done here, just calculate R and store it
  
  m_outdir->cd();
    
  m_Xx = pointX;

  m_config->UseVaryingNbeta( true );

  //
  if(m_debug) std::cout << "EvaluateR> using Alfa= " << m_config->GetPar3() << std::endl; // Par3 == alpha
  
  std::cout << (*m_config) << std::endl;
  
  MuTrackEvents * mu1 = new MuTrackEvents(m_data_xsec_neut.c_str(), m_data_xsec_anti.c_str(), 
                                          m_data_pshadow.c_str(), m_config );
 
  double TkSum = mu1->Evaluate( );
  
  m_MuTks  = mu1->m_NuMuTracks;
  m_TauTks = mu1->m_NuTauTracks;

  ShowerEvents * sh1 =  new ShowerEvents(m_data_xsec_neut.c_str(), m_data_xsec_anti.c_str(), 
                                           m_data_pshadow.c_str(), m_config );

  m_HadShw = sh1->Evaluate( );
  
  m_HadShwE = sh1->m_CCNuShower;
    
  m_HadShwT = sh1->m_CCNutauShower;
  
  m_HadShwNC = sh1->m_NCShower;
  
  m_Ratio  = TkSum / m_HadShw;

  std::cout << "NeutrinosDetectionPaper> "
            << "pointX "   << m_Xx     << '\t'
            << "muTrk "    << m_MuTks  << '\t' 
            << "tauTrk "   << m_TauTks << '\t'
            << "hadShow "  << m_HadShw << '\t'
            << "R "        << m_Ratio  << std::endl;
  
  m_tree->Fill();
  
  delete mu1;
  delete sh1;
 
}

void NeutrinosDetectionPaper::WriteOutput() 
{
  
  m_tree->Write();
  m_output_file->cd("../");

}

//====================================================================================================

bool NeutrinosDetectionPaper::Init( const char * target , const char * src_step, const char * option)
{
  
  /////

  TString path = TString(target) + 
    TString("_") + 
    TString(src_step) + 
    TString("_") + 
    TString(option) + 
    TString("/data"); 
  
  std::cout << "Init> initializing: " << path << std::endl;
  
  m_Ex_in = 0;
  m_Phi_e_in = 0.0;
  m_Phi_m_in = 0.0;
  m_Phi_t_in = 0.0;
  
  m_file->cd();
  
  m_input_tree = (TTree*)gDirectory->Get(path);
  
  if (!m_input_tree)  { 
    std::cout << " Init> could not find Tree " << std::endl;
    return false;
  }
  
  // Set branch addresses and branch pointers
  
  m_input_tree->SetBranchAddress("Ex", &m_Ex_in, &b_Ex_in);
  m_input_tree->SetBranchAddress("Phi_e", &m_Phi_e_in, &b_Phi_e_in);
  m_input_tree->SetBranchAddress("Phi_m", &m_Phi_m_in, &b_Phi_m_in);
  m_input_tree->SetBranchAddress("Phi_t", &m_Phi_t_in, &b_Phi_t_in);
  
  std::cout << "Init> done " << path << std::endl;

  return true;
 
}

//..............................................................................................

bool NeutrinosDetectionPaper::InitOutput(const char * model, 
                                         const char * target, 
                                         const char * source,
                                         const char * option )
{
  
  /////
  m_output_file->cd();
  
  m_outdir = m_output_file->mkdir( TString("Ratio") 
                                   + TString("_")
                                   + TString(model)
                                   + TString("_")
                                   + TString(target) 
                                   + TString("_") 
                                   + TString(source)
                                   + TString("_")
                                   + TString(option));
  
  m_outdir->cd();
  
  m_tree = new TTree("data","Data points");

  m_tree->Branch("Xx",        &m_Xx,         "Xx/d");
  m_tree->Branch("MuTks",     &m_MuTks,      "MuTks/d");
  m_tree->Branch("TauTks",    &m_TauTks,     "TauTks/d");
  m_tree->Branch("HadShw",    &m_HadShw,     "HadShw/d");
  m_tree->Branch("HadShwE",   &m_HadShwE,    "HadShwE/d");
  m_tree->Branch("HadShwT",   &m_HadShwT,    "HadShwT/d");
  m_tree->Branch("HadShwNC",  &m_HadShwNC,   "HadShwNC/d");

  m_tree->Branch("Ratio",     &m_Ratio,      "Ratio/d");

  m_Xx       = -1.0;
  m_MuTks    = -1.0;
  m_HadShw   = -1.0;
  m_HadShwE  = -1.0;
  m_HadShwT  = -1.0;
  m_HadShwNC = -1.0;
  
  std::cout << "InitOutput> done" << std::endl;
  
  return true;
  
}

//..............................................................................................

void NeutrinosDetectionPaper::SetFluxHistograms(TFile * infile, 
                                                const char * model, 
                                                const char * target,
                                                const char * source,
                                                const char * option)
{
  
  m_file = infile;
  
  // source ----> (progragation) -----> target
  
  std::vector<std::string> input;
    
  input.push_back( std::string("Pee") );
  input.push_back( std::string("aPee") );

  /////
  m_output_file->cd();
  
  m_output_file->mkdir( TString("Flux_Histograms") 
                        + TString("_")
                        + TString(model)
                        + TString("_")
                        + TString(target) 
                        + TString("_") 
                        + TString(source) 
                        + TString("_")
                        + TString(option) )->cd();
  
  /// Book now 3+3 histograms (with variable size bins)
  
  m_flux_histos["phi_e"]     = new TH1F("phi_e"    ,"flux as a function of energy", m_e_bins, m_vbins );
  m_flux_histos["phi_mu"]    = new TH1F("phi_mu"   ,"flux as a function of energy", m_e_bins, m_vbins );
  m_flux_histos["phi_tau"]   = new TH1F("phi_tau"  ,"flux as a function of energy", m_e_bins, m_vbins );

  m_flux_histos["phi_ae"]    = new TH1F("phi_ae"   ,"flux as a function of energy", m_e_bins, m_vbins );
  m_flux_histos["phi_amu"]   = new TH1F("phi_amu"  ,"flux as a function of energy", m_e_bins, m_vbins );
  m_flux_histos["phi_atau"]  = new TH1F("phi_atau" ,"flux as a function of energy", m_e_bins, m_vbins );

  int bin_pos = 1;
  double kval = 0.0;
  std::map<std::string,double> sum;
  
  //Initialize input tree
  bool valid = Init ( target, source, input[0].c_str() );
  if( !valid ) return;
  
  Long64_t nentries = m_input_tree->GetEntries();
    
  std::cout << "NeutrinosDetectionPaper> n points: " << nentries << std::endl;
  
  for (Long64_t i=0;i<nentries;i++) {
      
    m_input_tree->GetEntry(i);

    //std::cout << "SetFluxHistograms> " << i << " " << m_Ex_in << " " << bin_pos<< " " << m_energy_bin[bin_pos] << '\n';
    
    if ( m_Ex_in < m_energy_bin[bin_pos] ) 
    {
      
      sum["phi_e"] += m_Phi_e_in;
      sum["phi_m"] += m_Phi_m_in;
      sum["phi_t"] += m_Phi_t_in;

      kval += 1.0;
      
    } else {
      
      // Get the averages
      sum["phi_e"] = sum["phi_e"] / kval;
      sum["phi_m"] = sum["phi_m"] / kval;
      sum["phi_t"] = sum["phi_t"] / kval;
      
      //Store the info in the histograms
      //std::cout << bin_pos << " " << sum["phi_e"] << '\t' << sum["phi_m"] << '\t' << sum["phi_t"] << std::endl; 
      
      m_flux_histos["phi_e"]->SetBinContent(bin_pos, sum["phi_e"]);
      m_flux_histos["phi_mu"]->SetBinContent(bin_pos, sum["phi_m"]);
      m_flux_histos["phi_tau"]->SetBinContent(bin_pos, sum["phi_t"]);
      
      // Prepare for next bin
      
      sum["phi_e"] = m_Phi_e_in;
      sum["phi_m"] = m_Phi_m_in;
      sum["phi_t"] = m_Phi_t_in;
      
      kval = 1.0;
      
      bin_pos += 1;
      
    }
    
    if ( bin_pos > m_e_bins ) break;
  
    /*
      if ( i == (nentries-1) ) {
      // Get the averages
      sum["phi_e"] = sum["phi_e"] / kval;
      sum["phi_m"] = sum["phi_m"] / kval;
      sum["phi_t"] = sum["phi_t"] / kval;
      
      //Store the info in the histograms
      //std::cout << bin_pos << " " << sum["phi_e"] << '\t' << sum["phi_m"] << '\t' << sum["phi_t"] << std::endl; 
      
      m_flux_histos["phi_e"]->SetBinContent(bin_pos, sum["phi_e"]);
      m_flux_histos["phi_mu"]->SetBinContent(bin_pos, sum["phi_m"]);
      m_flux_histos["phi_tau"]->SetBinContent(bin_pos, sum["phi_t"]);
      
      }
    */
    
    
    
  }
  
  //std::cout << "SetFluxHistograms> bin_pos " << bin_pos << std::endl;
  
  bin_pos = 1;
  kval = 0.0;
  
  valid = Init ( target, source, input[1].c_str() );
  if( !valid ) return;
  
  nentries = m_input_tree->GetEntries();
  
  std::cout << "NeutrinosDetectionPaper> n points: " << nentries << std::endl;
  
  for (Long64_t i=0;i<nentries;i++) {
    
    m_input_tree->GetEntry(i);
    
    if ( m_Ex_in < m_energy_bin[bin_pos] ) 
    {
      
      sum["phi_e"] += m_Phi_e_in;
      sum["phi_m"] += m_Phi_m_in;
      sum["phi_t"] += m_Phi_t_in;
      
      kval += 1.0;
      
    } else {
      
      // Get the averages
      sum["phi_e"] = sum["phi_e"] / kval;
      sum["phi_m"] = sum["phi_m"] / kval;
      sum["phi_t"] = sum["phi_t"] / kval;
      
      //Store the info in the histograms
      //std::cout << bin_pos << " " << sum["phi_e"] << '\t' << sum["phi_m"] << '\t' << sum["phi_t"] << std::endl; 
      
      m_flux_histos["phi_ae"]->SetBinContent(bin_pos, sum["phi_e"]);
      m_flux_histos["phi_amu"]->SetBinContent(bin_pos, sum["phi_m"]);
      m_flux_histos["phi_atau"]->SetBinContent(bin_pos, sum["phi_t"]);
      
      // Prepare for next bin
      
      sum["phi_e"] = m_Phi_e_in;
      sum["phi_m"] = m_Phi_m_in;
      sum["phi_t"] = m_Phi_t_in;
      
      kval = 1.0;
      
      bin_pos += 1;
      
    }

    if ( bin_pos > m_e_bins ) break;

    /*
      if ( i == (nentries-1) ) 
      {
      // Get the averages
      sum["phi_e"] = sum["phi_e"] / kval;
      sum["phi_m"] = sum["phi_m"] / kval;
      sum["phi_t"] = sum["phi_t"] / kval;
      
      //Store the info in the histograms
      //std::cout << bin_pos << " " << sum["phi_e"] << '\t' << sum["phi_m"] << '\t' << sum["phi_t"] << std::endl; 
      
      m_flux_histos["phi_ae"]->SetBinContent(bin_pos, sum["phi_e"]);
      m_flux_histos["phi_amu"]->SetBinContent(bin_pos, sum["phi_m"]);
      m_flux_histos["phi_atau"]->SetBinContent(bin_pos, sum["phi_t"]);
      }
    */
    
  }
  
  //... Pass now this information to the Parameters / Configuration settings
  
  m_config->m_histo_params["N_e"] = m_flux_histos["phi_e"];
  m_config->m_histo_params["N_mu"] = m_flux_histos["phi_mu"];
  m_config->m_histo_params["N_tau"] = m_flux_histos["phi_tau"];
  
  m_config->m_histo_params["N_ae"] = m_flux_histos["phi_ae"];
  m_config->m_histo_params["N_amu"] = m_flux_histos["phi_amu"];
  m_config->m_histo_params["N_atau"] = m_flux_histos["phi_atau"];
  
  ///
  
  m_output_file->Write();
  m_output_file->cd("../");
  
  std::cout << "SetFluxHistograms " << m_energy_bin.size() << std::endl;
  

}

void NeutrinosDetectionPaper::ResetFluxHistograms( )
{

  delete m_flux_histos["phi_e"];
  delete m_flux_histos["phi_mu"];
  delete m_flux_histos["phi_tau"];

  delete m_flux_histos["phi_ae"];
  delete m_flux_histos["phi_amu"];
  delete m_flux_histos["phi_atau"];
}

//..............................................................................................

void NeutrinosDetectionPaper::SetFluxAverages(TFile      * infile, 
                                              const char * model, 
                                              const char * target,
                                              const char * source)
{
  
  m_file = infile;
  
  // source ----> (progragation) -----> target
  
  std::vector<std::string> input;
  std::vector<std::string>::iterator inputItr;
  
  input.push_back( std::string("Pee") );
  input.push_back( std::string("aPee") );
  
  std::vector<double> Nbeta;
  
  for( inputItr = input.begin(); inputItr != input.end(); ++inputItr) {
    
    //Initialize input tree
    bool valid = Init ( target, source, (*inputItr).c_str() );
    if( !valid ) continue;
    
    Long64_t nentries = m_input_tree->GetEntries();
    
    std::cout << "NeutrinosDetectionPaper> n points: " << nentries << std::endl;
    
    for (Long64_t i=0;i<nentries;i++) {
      
      m_input_tree->GetEntry(i);
      
      if ( (*inputItr).compare("Pee") == 0 ) 
      {

        h_1dHistos["phi_e"]->Fill(m_Phi_e_in);
        h_1dHistos["phi_mu"]->Fill(m_Phi_m_in);
        h_1dHistos["phi_tau"]->Fill(m_Phi_t_in);
        
      } else {
        
        h_1dHistos["phi_ae"]->Fill(m_Phi_e_in);
        h_1dHistos["phi_amu"]->Fill(m_Phi_m_in);
        h_1dHistos["phi_atau"]->Fill(m_Phi_t_in);
        
      }
      
    }
    
  }
  
  Nbeta.push_back( h_1dHistos["phi_e"]->GetMean() );
  Nbeta.push_back( h_1dHistos["phi_mu"]->GetMean() );
  Nbeta.push_back( h_1dHistos["phi_tau"]->GetMean() );
  
  Nbeta.push_back( h_1dHistos["phi_ae"]->GetMean() );
  Nbeta.push_back( h_1dHistos["phi_amu"]->GetMean() );
  Nbeta.push_back( h_1dHistos["phi_atau"]->GetMean() );
  
  std::cout << "Results for Nu(): " 
            <<  h_1dHistos["phi_e"]->GetMean() << '\t'
            <<  h_1dHistos["phi_mu"]->GetMean() << '\t'
            <<  h_1dHistos["phi_tau"]->GetMean() << '\t'
            << std::endl;
  
  std::cout << "Results for aNu(): " 
            <<  h_1dHistos["phi_ae"]->GetMean() << '\t'
            <<  h_1dHistos["phi_amu"]->GetMean() << '\t'
            <<  h_1dHistos["phi_atau"]->GetMean() << '\t'
            << std::endl;
  
  // Set the N_betas
  
  m_config->SetPar("N_e", Nbeta[0] );
  m_config->SetPar("N_mu", Nbeta[1] );
  m_config->SetPar("N_tau", Nbeta[2] );
  
  m_config->SetPar("N_ae", Nbeta[3] );
  m_config->SetPar("N_amu", Nbeta[4] );
  m_config->SetPar("N_atau", Nbeta[5] );
  
  std::cout << *m_config << std::endl;

  this->Reset(); // Clear content of the auxiliary histograms

  Nbeta.clear();
  
}

//====================================================================================================

void NeutrinosDetectionPaper::PropagateThroughEarth(const char * target, 
                                                    const char * source,
                                                    const char * option,
                                                    const char * probability,
                                                    double f1, 
                                                    double f2, 
                                                    double f3 )
{
  
  if( m_debug ) std::cout << "PropagateThroughEarth> Starts!" << std::endl;
  
  NeutrinoOscInVarDensity * m_Physics = new NeutrinoOscInVarDensity( m_mixpars );
  
  m_Physics->use_default_pars = false;
  m_Physics->initializeAngles();
  
  bool  anti_nu   = false;
  
  m_output_file->mkdir( TString(target)       + TString("_") 
                        + TString(source)     + TString("_")
                        + TString(option)     + TString("_")
                        + TString(probability) )->cd(); //
   
  std::map<std::string, std::pair<int,int> > m_ProbIndex;
  
  m_ProbIndex["Pee"] = std::make_pair( 0, 0);
  
  // Argument "probability" tells if we are working with neutrinos (default) or anti-neutrinos ("a" in front)
  
  std::string Pxx( probability );
  
  unsigned found = Pxx.find("a");
  
  if ( found == 0 ) {
    anti_nu  = true;
    Pxx.erase(0,1);
    std::cout << "PropagateThroughEarth> Will treat for anit-nu: " << Pxx << '\n';
  } 

  double m_Ex;
  double m_Pb;
  double m_PbV;
  double m_Phi_e;
  double m_Phi_m;
  double m_Phi_t;

  m_tree = new TTree("data","Data points");
  
  m_tree->Branch("Ex" , &m_Ex , "Ex/d");
  m_tree->Branch("Pb" , &m_Pb , "Pb/d");
  m_tree->Branch("PbV", &m_PbV, "PbV/d");
  m_tree->Branch("Phi_e", &m_Phi_e, "Phi_e/d");
  m_tree->Branch("Phi_m", &m_Phi_m, "Phi_m/d");
  m_tree->Branch("Phi_t", &m_Phi_t, "Phi_t/d");
    
  DensityModels * density_Mod = (DensityModels*) new rhoEarthB(); 
  
  if ( anti_nu ) 
    density_Mod->treat_as_AntiNu();
  else 
    density_Mod->treat_as_Nu();
  
  if( m_debug ) std::cout << "PropagateThroughEarth> checking sign in front of the out_model: " << density_Mod->m_sign << std::endl;
  
  double LMIN      = m_modelpars->GetPar("LMIN");
  
  double LMAX      = m_modelpars->GetPar("LMAX");

  long double Ex   = (long double) m_modelpars->GetPar("Emin");
  long double Emax = (long double) m_modelpars->GetPar("Emax");
  long double dx   = (long double) m_modelpars->GetPar("Dx");  //this is the distance step
  long double dE   = (long double) m_modelpars->GetPar("De");  //this is the energy step
  
  if (m_debug) std::cout << "PropagateThroughEarth> Constants: "   << '\n'
                         << "LMIN " << LMIN << '\n'
                         << "LMAX " << LMAX << std::endl;
  
  if (m_debug) std::cout << "PropagateThroughEarth> Mixing parameters: " 
                         << "theta1 "   <<  m_Physics->m_input->GetPar1() << '\t'
                         << "theta2 "   <<  m_Physics->m_input->GetPar2() << '\t'
                         << "theta3 "   <<  m_Physics->m_input->GetPar3() << '\t'
                         << "DM2 (32) " <<  m_Physics->m_input->GetPar4() << '\t'
                         << "Dm2 (21) " <<  m_Physics->m_input->GetPar8() << '\n';
  
  int maxpars = (int)m_modelpars->GetPar(0);
  
  TF1 * profA = new TF1("profA", density_Mod, LMIN, LMAX, maxpars);
  
  for( int i=1; i <= maxpars; ++i) {
    profA->SetParameter( ( i-1 ), (m_modelpars->GetPar(i))  );
    std::cout << "PropagateThroughEarth> * par: " << (m_modelpars->GetPar(i)) << std::endl;
  }
  
  m_Physics->updateMixingMatrix();
  
  m_Physics->setPotential( profA );
  
  int k = 0;
  
  while ( Ex <= Emax ) {
    
    m_Physics->m_Ev = Ex;
    
    m_Physics->updateEab();
    
    matrix <std::complex< long double> >  * tmp;
    tmp = new matrix<std::complex< long double> >(3,3);
    
    double long x1 = LMIN;
    double long x2 = x1 + dx;
    
    int i = 0;
    
    while ( x2 <= LMAX ) {
      
      m_Physics->Eval_UFlavour( x2, x1 );
      
      if( i == 0) {
        (*tmp) = (*m_Physics->m_Uf); 
      } else
        (*tmp) = prod( (*m_Physics->m_Uf), (*tmp) );
      
      x1  = x2;
      x2 += dx;
      
      i += 1;
      
    }
    
    (*m_Physics->m_Uf) = (*tmp);
    (*m_Physics->m_Ufd) = conj ( (*m_Physics->m_Uf) );
    
    m_Physics->calcProbabilities();
    m_Physics->calcVacProbabilities( (LMAX-LMIN) );

    //get the Transition probability A->B
    
    double d1 = (*m_Physics->m_Prob_AtoB)( m_ProbIndex[Pxx].first , m_ProbIndex[Pxx].second );
    double d2 = (*m_Physics->m_VacProb_AtoB)( m_ProbIndex[Pxx].first , m_ProbIndex[Pxx].second );
    
    if ( ! (boost::math::isnan)(d1) ) {
      
      m_Ex  = Ex;
      m_Pb  = d1;
      m_PbV = d2;
      
      // Fluxes at entry point: f1, f2, f3
      
      m_Phi_e = m_Physics->Propagate( 0, f1, f2, f3 ); 
      m_Phi_m = m_Physics->Propagate( 1, f1, f2, f3 ); 
      m_Phi_t = m_Physics->Propagate( 2, f1, f2, f3 ); 
      
      m_tree->Fill();
      
    }     
    
    k += 1; 
    
    if ( Ex < 1.0E12 )
      Ex += dE; // step in energy
    else if ( Ex >=1.0E12 && Ex < 1.0E13 )
      Ex += (dE*10.0); //step in energy
    else if ( Ex >=1.0E13 && Ex < 1.0E14 )
      Ex += (dE*100.0); //step in energy
    else if ( Ex >=1.0E14 && Ex < 1.0E15 )
      Ex += (dE*1000.0); //step in energy
    else if ( Ex >=1.0E15 && Ex < 1.0E16 )
      Ex += (dE*10000.0); //step in energy
    else if ( Ex >=1.0E16 && Ex < 1.0E17 )
      Ex += (dE*100000.0); //step in energy
    else
      Ex += (dE*1000000.0);
    
    delete tmp;
    
  }
  
  std::cout << "PropagateThroughEarth> max pts: " << k << std::endl;
  
  m_tree->Write();
  
  m_output_file->cd("../");
  
  delete profA;
  delete density_Mod;
  delete m_Physics;
  
  std::cout << "PropagateThroughEarth> all done " << std::endl;

}
