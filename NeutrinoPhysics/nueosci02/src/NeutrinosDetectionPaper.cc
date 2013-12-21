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
    
    MuTrackEvents * mu1 = new MuTrackEvents("../data/XSec_neut.dat", "../data/XSec_anti.dat", m_config );
    
    double TkSum = mu1->Evaluate( );
    
    m_MuTks  = mu1->m_NuMuTracks;
    m_TauTks = mu1->m_NuTauTracks;
    
    std::cout << " mu done " << std::endl;
    
    ShowerEvents * sh1 =  new ShowerEvents("../data/XSec_neut.dat", "../data/XSec_anti.dat", m_config );
    
    m_HadShw = sh1->Evaluate( );
    
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

void NeutrinosDetectionPaper::MakeVariation02(TFile * infile,
                                              const char * model,
                                              const char * target, 
                                              const char * source, 
                                              double Xmin, double Xmax, double Dx)
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
  
  InitOutput(model, target, source, "alpha");
  
  // Here is the loop for the parameter variation
  
  m_Xx = Xmin;
  
  // Set the N_betas
  
  m_config->SetPar("N_e", Nbeta[0] );
  m_config->SetPar("N_mu", Nbeta[1] );
  m_config->SetPar("N_tau", Nbeta[2] );
  
  m_config->SetPar("N_ae", Nbeta[3] );
  m_config->SetPar("N_amu", Nbeta[4] );
  m_config->SetPar("N_atau", Nbeta[5] );
  
  std::cout << *m_config << std::endl;
  

  while ( 1 ) {
    
    if ( m_Xx >= Xmax ) break;
    
    m_config->SetPar3( m_Xx ); // Par3 == alpha
    
    MuTrackEvents * mu1 = new MuTrackEvents("../data/XSec_neut.dat", "../data/XSec_anti.dat", m_config );
    
    double TkSum = mu1->Evaluate( );
    
    m_MuTks  = mu1->m_NuMuTracks;
    m_TauTks = mu1->m_NuTauTracks;
    
    std::cout << " mu done " << std::endl;
    
    ShowerEvents * sh1 =  new ShowerEvents("../data/XSec_neut.dat", "../data/XSec_anti.dat", m_config );
    
    m_HadShw = sh1->Evaluate( );
    
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
  
  this->Reset(); // Clear content of the auxiliary histograms
  
  Nbeta.clear();
  
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
  
    MuTrackEvents * mu1 = new MuTrackEvents("../data/XSec_neut.dat", "../data/XSec_anti.dat", m_config );
    
    double TkSum = mu1->Evaluate( );
    
    m_MuTks  = mu1->m_NuMuTracks;
    m_TauTks = mu1->m_NuTauTracks;
    
    std::cout << " mu done " << std::endl;
    
    ShowerEvents * sh1 =  new ShowerEvents("../data/XSec_neut.dat", "../data/XSec_anti.dat", m_config );
    
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
