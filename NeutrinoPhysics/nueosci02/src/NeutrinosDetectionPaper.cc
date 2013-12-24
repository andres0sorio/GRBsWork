// $Id: $
// Include files 



// local
#include "NeutrinosDetectionPaper.h"

//-----------------------------------------------------------------------------
// Implementation file for class : NeutrinosDetectionPaper
//
// 2013-03-29 : Andres Osorio
//-----------------------------------------------------------------------------

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
NeutrinosDetectionPaper::NeutrinosDetectionPaper( Parameters * pars ) : Graphics() {
  
  m_config = pars;
  
  m_output_file = new TFile("detection.root","RECREATE");
  
  m_debug = true;

  ///Set energy bins


  m_e_min  = 1.0E11;
  m_e_max  = 1.0E17;
  
  m_energy_bin[1]  = 0.5E12;
  m_energy_bin[2]  = 1.0E12;
  m_energy_bin[3]  = 0.5E13;
  m_energy_bin[4]  = 1.0E13;
  m_energy_bin[5]  = 0.5E14;
  m_energy_bin[6]  = 1.0E14;
  m_energy_bin[7]  = 0.5E15;
  m_energy_bin[8]  = 1.0E15;
  m_energy_bin[9]  = 0.5E16;
  m_energy_bin[10] = 1.0E16;
  m_energy_bin[11] = 0.5E17;
  m_energy_bin[12] = m_e_max;

  m_e_bins = m_energy_bin.size();
  
}
//=============================================================================
// Destructor
//=============================================================================
NeutrinosDetectionPaper::~NeutrinosDetectionPaper() {
  
  m_output_file->Close();
  
  std::cout << "NeutrinosDetectionPaper> cleanly destroyed" << std::endl;
  
} 

//=============================================================================

void NeutrinosDetectionPaper::MakeVariation01(TFile * infile, const char * target, const char * source)
{
 
  m_file = infile;
  
  // source ----> (progragation) -----> target
 
  std::vector<std::string> input;
  std::vector<std::string>::iterator inputItr;
  
  input.push_back( std::string("Pee") );
  input.push_back( std::string("aPee") );

  InitOutput("X", target, source, (*inputItr).c_str() );

  m_tree->Write();
  
  m_output_file->cd("../");
        
}

void NeutrinosDetectionPaper::MakeVariationStdPicture(const char * target, 
                                                      const char * source, 
                                                      double Xmin, double Xmax, double Dx)
{
  
  //Make the variation for the Standard Picture 1:1:1
  
  InitOutput("StdPicture", target, source, "alpha");
  
  // Loop over the parameter
  
  m_Xx = Xmin;
  
  // Set N_betas 
  
  m_config->SetPar("N_e", 1.0 );
  m_config->SetPar("N_mu", 1.0 );
  m_config->SetPar("N_tau", 1.0 );
  
  m_config->SetPar("N_ae", 1.0 );
  m_config->SetPar("N_amu", 1.0 );
  m_config->SetPar("N_atau", 1.0 );

  while ( 1 ) {
    
    if ( m_Xx >= Xmax ) break;
    
    m_config->SetPar3( m_Xx ); // par3 == alpha
    
    MuTrackEvents * mu1 = new MuTrackEvents("../data/XSec_neut.dat", "../data/XSec_anti.dat", 
                                            "../data/pshadow-at-180.dat", m_config );
    
    double TkSum = mu1->Evaluate( );
    
    std::cout << "MakeVariationStdPicture> TkSum done " << std::endl;
    
    m_MuTks  = mu1->m_NuMuTracks;
    m_TauTks = mu1->m_NuTauTracks;
    
    ShowerEvents * sh1 =  new ShowerEvents("../data/XSec_neut.dat", "../data/XSec_anti.dat", 
                                           "../data/pshadow-at-180.dat", m_config );
    
    m_HadShw = sh1->Evaluate( );
    
    std::cout << "MakeVariationStdPicture> Sh done " << std::endl;

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

    m_Xx = m_Xx + Dx;
    
    //break;
    
  }
  
  m_tree->Write();
  
  m_output_file->cd("../");
  
}

void NeutrinosDetectionPaper::MakeVariation02(const char * model,
                                              const char * target, 
                                              const char * source, 
                                              double Xmin, 
                                              double Xmax, 
                                              double Dx)
{
  
  InitOutput(model, target, source, "alpha");
  
  // Here is the loop for the parameter variation
  
  m_Xx = Xmin;

  m_config->UseVaryingNbeta( true );

  std::cout << (*m_config) << std::endl;
  
  while ( 1 ) {
    
    if ( m_Xx >= Xmax ) break;
    
    m_config->SetPar3( m_Xx ); // Par3 == alpha
    
    MuTrackEvents * mu1 = new MuTrackEvents("../data/XSec_neut.dat", "../data/XSec_anti.dat", 
                                            "../data/pshadow-at-180.dat", m_config );
    
    double TkSum = mu1->Evaluate( );
    
    m_MuTks  = mu1->m_NuMuTracks;
    m_TauTks = mu1->m_NuTauTracks;
    
    ShowerEvents * sh1 =  new ShowerEvents("../data/XSec_neut.dat", "../data/XSec_anti.dat", 
                                           "../data/pshadow-at-180.dat", m_config );
    
    m_HadShw = sh1->Evaluate( );
    
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
  
  InitOutput(model, target, source, "phi_nue");
  
  // Here is the loop for the parameter variation
  
  float phi_nue = 0.0;
  float phi_ne_fr = 0.0;
  float phi_nmu_fr = 0.0;
  float phi_ntau_fr = 0.0;

  m_config->SetPar3( 2.0 );

  phi_nue = Xmin;
  
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
  
    std::cout << *m_config << std::endl;
  
    MuTrackEvents * mu1 = new MuTrackEvents("../data/XSec_neut.dat", "../data/XSec_anti.dat", 
                                            "../data/pshadow-at-180.dat", m_config );
    
    double TkSum = mu1->Evaluate( );
    
    m_MuTks  = mu1->m_NuMuTracks;
    m_TauTks = mu1->m_NuTauTracks;
    
    ShowerEvents * sh1 =  new ShowerEvents("../data/XSec_neut.dat", "../data/XSec_anti.dat", 
                                           "../data/pshadow-at-180.dat", m_config );
    
    m_HadShw = sh1->Evaluate( );
    
    std::cout << " shower done " << std::endl;

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
                                              double alpha,
                                              double phase )
{
  
  NeutrinoOscInVacuum * m_Physics_Vacuum =  new NeutrinoOscInVacuum( m_mixpars );
  
  m_Physics_Vacuum->use_default_pars = false; // read parameters from file

  std::stringstream Var;

  Var << "Sin2Q13-" << alpha << "-" << phase;
    
  InitOutput(model, target, source, Var.str().c_str() );
  
  double Xx = Xmin;

  m_Physics_Vacuum->initializeAngles();

  m_Physics_Vacuum->setPhase( phase );
  
  m_config->SetPar3( alpha );

  while(1) 
  {
    
    if( Xx >= Xmax ) break;
    
    double sin2theta = pow ( sin( Xx * M_PI / 180.0), 2.0 );
    
    m_Physics_Vacuum->setAngle(1, 3, Xx);

    m_Physics_Vacuum->updateMixingMatrix();

    m_Physics_Vacuum->calcProbabilities();
    
    m_Physics_Vacuum->Propagate( 1.0, 2.0, 0.0 );
    
    m_config->SetPar("N_e", (m_Physics_Vacuum->m_phi_e_f) / 2.0 );
    m_config->SetPar("N_mu", (m_Physics_Vacuum->m_phi_mu_f) / 2.0 );
    m_config->SetPar("N_tau", (m_Physics_Vacuum->m_phi_tau_f) / 2.0 );
    
    m_config->SetPar("N_ae", (m_Physics_Vacuum->m_phi_e_f) / 2.0 );
    m_config->SetPar("N_amu", (m_Physics_Vacuum->m_phi_mu_f) / 2.0 );
    m_config->SetPar("N_atau", (m_Physics_Vacuum->m_phi_tau_f) / 2.0 );
    
    MuTrackEvents * mu1 = new MuTrackEvents("../data/XSec_neut.dat", "../data/XSec_anti.dat", 
                                            "../data/pshadow-at-180.dat", m_config );
    
    double TkSum = mu1->Evaluate( );
    
    m_MuTks  = mu1->m_NuMuTracks;
    m_TauTks = mu1->m_NuTauTracks;
    
    ShowerEvents  * sh1 = new ShowerEvents("../data/XSec_neut.dat", "../data/XSec_anti.dat",
                                           "../data/pshadow-at-180.dat", m_config );
    
    m_HadShw = sh1->Evaluate( );
    
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
  
  m_output_file->mkdir( TString("Ratio") 
                        + TString("_")
                        + TString(model)
                        + TString("_")
                        + TString(target) 
                        + TString("_") 
                        + TString(source)
                        + TString("_")
                        + TString(option))->cd();
  
  m_tree = new TTree("data","Data points");
                        
  m_tree->Branch("Xx",     &m_Xx,      "Xx/d");
  m_tree->Branch("MuTks",  &m_MuTks,   "MuTks/d");
  m_tree->Branch("TauTks", &m_TauTks,  "TauTks/d");
  m_tree->Branch("HadShw", &m_HadShw,  "HadShw/d");
  m_tree->Branch("Ratio",  &m_Ratio,   "Ratio/d");

  std::cout << "InitOutput> done" << std::endl;
  
  return true;
  
}

//..............................................................................................

void NeutrinosDetectionPaper::SetFluxHistograms(TFile * infile, 
                                                const char * model, 
                                                const char * target,
                                                const char * source)
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
                        + TString(source) )->cd();
  
  /// Book now 3+3 histograms

  m_flux_histos["phi_e"]     = new TH1F("phi_e"    ,"flux as a function of energy", m_e_bins, m_e_min, m_e_max );
  m_flux_histos["phi_mu"]    = new TH1F("phi_mu"   ,"flux as a function of energy", m_e_bins, m_e_min, m_e_max );
  m_flux_histos["phi_tau"]   = new TH1F("phi_tau"  ,"flux as a function of energy", m_e_bins, m_e_min, m_e_max );

  m_flux_histos["phi_ae"]    = new TH1F("phi_ae"   ,"flux as a function of energy", m_e_bins, m_e_min, m_e_max );
  m_flux_histos["phi_amu"]   = new TH1F("phi_amu"  ,"flux as a function of energy", m_e_bins, m_e_min, m_e_max );
  m_flux_histos["phi_atau"]  = new TH1F("phi_atau" ,"flux as a function of energy", m_e_bins, m_e_min, m_e_max );

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
      
      std::cout << bin_pos << " " << sum["phi_e"] << '\t' << sum["phi_m"] << '\t' << sum["phi_t"] << std::endl; 

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

    if ( i == (nentries-1) ) 
    {
      // Get the averages
      sum["phi_e"] = sum["phi_e"] / kval;
      sum["phi_m"] = sum["phi_m"] / kval;
      sum["phi_t"] = sum["phi_t"] / kval;
      
      //Store the info in the histograms
      
      std::cout << bin_pos << " " << sum["phi_e"] << '\t' << sum["phi_m"] << '\t' << sum["phi_t"] << std::endl; 
      
      m_flux_histos["phi_e"]->SetBinContent(bin_pos, sum["phi_e"]);
      m_flux_histos["phi_mu"]->SetBinContent(bin_pos, sum["phi_m"]);
      m_flux_histos["phi_tau"]->SetBinContent(bin_pos, sum["phi_t"]);
      
    }
    
    
  }


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
      
      std::cout << bin_pos << " " << sum["phi_e"] << '\t' << sum["phi_m"] << '\t' << sum["phi_t"] << std::endl; 

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

    if ( i == (nentries-1) ) 
    {
      // Get the averages
      sum["phi_e"] = sum["phi_e"] / kval;
      sum["phi_m"] = sum["phi_m"] / kval;
      sum["phi_t"] = sum["phi_t"] / kval;
      
      //Store the info in the histograms
      
      std::cout << bin_pos << " " << sum["phi_e"] << '\t' << sum["phi_m"] << '\t' << sum["phi_t"] << std::endl; 
      
      m_flux_histos["phi_ae"]->SetBinContent(bin_pos, sum["phi_e"]);
      m_flux_histos["phi_amu"]->SetBinContent(bin_pos, sum["phi_m"]);
      m_flux_histos["phi_atau"]->SetBinContent(bin_pos, sum["phi_t"]);
      
    }
    
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

