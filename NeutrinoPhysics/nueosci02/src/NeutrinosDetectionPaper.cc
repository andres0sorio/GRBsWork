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
NeutrinosDetectionPaper::NeutrinosDetectionPaper( Parameters * pars , TFile * prevStep  ) {

  m_config = pars;
  
  m_file = prevStep;
   
  m_output_file = new TFile("detection.root","RECREATE");
  
  m_debug = true;
  
}
//=============================================================================
// Destructor
//=============================================================================
NeutrinosDetectionPaper::~NeutrinosDetectionPaper() {
  
  m_file->Close();
  
  m_output_file->Close();

  std::cout << "NeutrinosDetectionPaper> cleanly destroyed" << std::endl;
  
} 

//=============================================================================

void NeutrinosDetectionPaper::MakeVariation01(const char * in_model, const char * src_model) 
{
  
  std::vector<std::string> input;
  std::vector<std::string>::iterator inputItr;
  
  input.push_back( std::string("Pee") );
  input.push_back( std::string("aPee") );
  
  for( inputItr = input.begin(); inputItr != input.end(); ++inputItr) {
    
    //Initialize input tree
    bool valid = Init ( in_model, src_model, (*inputItr).c_str() );
    if( !valid ) continue;
    
    m_output_file->cd();
    
    m_output_file->mkdir( TString("Ratio") 
                          + TString("_") 
                          + TString(in_model) 
                          + TString("_") 
                          + TString(src_model)
                          + TString("_")
                          + TString( (*inputItr).c_str() ) )->cd();
    
    m_tree = new TTree("data","Data points");
    
    m_tree->Branch("Ex",     &m_Ex,      "Ex/d");
    m_tree->Branch("MuTks",  &m_MuTks,   "MuTks/d");
    m_tree->Branch("TauTks", &m_TauTks,  "TauTks/d");
    m_tree->Branch("HadShw", &m_HadShw,  "HadShw/d");
    m_tree->Branch("Ratio",  &m_Ratio,   "Ratio/d");
          
    //Loop over input, propagate and save into new tree
    ////
    
    Long64_t nentries = m_input_tree->GetEntries();
    
    std::cout << "NeutrinosDetectionPaper> n points: " << nentries << std::endl;
    
    for (Long64_t i=0;i<nentries;i++) {
      
      m_input_tree->GetEntry(i);
      
      m_Ex = m_Ex_in;
      
      //double ExGev = m_Ex / 1.0e9;
      
      double sumphi = m_Phi_e_in + m_Phi_m_in + m_Phi_t_in;
            
      std::cout << m_Phi_e_in << '\t' << m_Phi_m_in << '\t' << m_Phi_t_in << '\t' << sumphi << std::endl;
      
      m_config->SetPar11( m_Phi_e_in );
      m_config->SetPar12( m_Phi_m_in );
      m_config->SetPar13( m_Phi_t_in );
            
      MuTrackEvents * mu1 = new MuTrackEvents("XSec_neut.dat", "XSec_anti.dat", m_config );
    
      //double TkSum = mu1->Evaluate( ExGev ); // evaluate returns N_mu+N_tau
      //m_config->SetPar1(ExGev);
      //m_config->SetPar2((ExGev*0.30) + ExGev);

      double TkSum = mu1->Evaluate( );

      m_MuTks  = mu1->m_NuMuTracks;
      m_TauTks = mu1->m_NuTauTracks;
      
      std::cout << " mu done " << std::endl;
      
      ShowerEvents * sh1 =  new ShowerEvents("XSec_neut.dat", "XSec_anti.dat", m_config );
      
      //double m_HadShw = sh1->Evaluate( ExGev );
      //m_config->SetPar10( ExGev );
      //m_config->SetPar2( (ExGev*0.30) + ExGev);
      
      m_HadShw = sh1->Evaluate( );

      m_Ratio  = TkSum / m_HadShw;


      std::cout << "NeutrinosDetectionPaper> "
                << m_Ex     << '\t'
                << m_MuTks  << '\t' 
                << m_TauTks << '\t'
                << m_HadShw << '\t'
                << m_Ratio  << std::endl;
  
    
      m_tree->Fill();

      delete mu1;
      delete sh1;

      //break;
        
    }
    
    
    m_tree->Write();
    
    m_output_file->cd("../");
   
    
  }
  
  
  
  
}

bool NeutrinosDetectionPaper::Init( const char * in_model , const char * src_step, const char * option)
{
  
  /////

  TString path = TString(in_model) + TString("_") + TString(src_step) + TString("_") + TString(option) + TString("/data"); 
  
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
  
  std::cout << "Init> done" << path << std::endl;

  return true;
 
}
