// $Id: $
// Include files 



// local
#include "NeutrinosInMediumPaper.h"

//-----------------------------------------------------------------------------
// Implementation file for class : NeutrinosInMediumPaper
//
// 2013-02-27 : Andres Osorio
//-----------------------------------------------------------------------------

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
NeutrinosInMediumPaper::NeutrinosInMediumPaper( MixingParameters * mixpars ) {

  m_Physics = new NeutrinoOscInVarDensity( mixpars );
  m_Physics->use_default_pars = false;

  m_Physics_Vacuum = 0x0;

  //
  m_ProbIndex["Pee"] = std::make_pair( 0, 0);
  m_ProbIndex["Pem"] = std::make_pair( 0, 1);
  m_ProbIndex["Pet"] = std::make_pair( 0, 2);
  m_ProbIndex["Pme"] = std::make_pair( 1, 0);
  m_ProbIndex["Pte"] = std::make_pair( 2, 0);
  m_ProbIndex["Pmm"] = std::make_pair( 1, 1);
  m_ProbIndex["Ptt"] = std::make_pair( 2, 2);
  m_ProbIndex["Pmt"] = std::make_pair( 2, 1);
  m_ProbIndex["Ptm"] = std::make_pair( 1, 2);

  m_Models["ModelA"] = (DensityModels*) new rhoModelA();
  m_Models["ModelB"] = (DensityModels*) new rhoModelB();
  m_Models["ModelC"] = (DensityModels*) new rhoModelC();
  m_Models["EarthA"] = (DensityModels*) new rhoEarthA();
  m_Models["EarthB"] = (DensityModels*) new rhoEarthB();
  m_Models["ZeroPt"] = (DensityModels*) new zeroPotencial(); // 

  m_file = new TFile("output.root","RECREATE");
  m_file->cd();
  
  m_debug = true;
  
}

NeutrinosInMediumPaper::NeutrinosInMediumPaper( MixingParameters * mixpars , TFile * prevStep ) {

  m_Physics = new NeutrinoOscInVarDensity( mixpars );
  m_Physics->use_default_pars = false;

  m_Physics_Vacuum =  new NeutrinoOscInVacuum( mixpars );
  m_Physics_Vacuum->use_default_pars = false;

  //
  m_ProbIndex["Pee"] = std::make_pair( 0, 0);
  m_ProbIndex["Pem"] = std::make_pair( 0, 1);
  m_ProbIndex["Pet"] = std::make_pair( 0, 2);
  m_ProbIndex["Pme"] = std::make_pair( 1, 0);
  m_ProbIndex["Pte"] = std::make_pair( 2, 0);
  m_ProbIndex["Pmm"] = std::make_pair( 1, 1);
  m_ProbIndex["Ptt"] = std::make_pair( 2, 2);
  m_ProbIndex["Pmt"] = std::make_pair( 2, 1);
  m_ProbIndex["Ptm"] = std::make_pair( 1, 2);

  m_Models["ModelA"] = (DensityModels*) new rhoModelA();
  m_Models["ModelB"] = (DensityModels*) new rhoModelB();
  m_Models["ModelC"] = (DensityModels*) new rhoModelC();
  m_Models["EarthA"] = (DensityModels*) new rhoEarthA();
  m_Models["EarthB"] = (DensityModels*) new rhoEarthB();
  m_Models["ZeroPt"] = (DensityModels*) new zeroPotencial(); // 

  m_file = prevStep;
  
  m_file->cd();
  
  m_debug = true;
  
}

//=============================================================================
// Destructor
//=============================================================================
NeutrinosInMediumPaper::~NeutrinosInMediumPaper() {
  
  m_file->Close();

  if( m_Physics ) delete m_Physics;

  if( m_Physics_Vacuum ) delete m_Physics_Vacuum;

  std::map<std::string,DensityModels*>::iterator itr;
  
  for( itr = m_Models.begin(); itr != m_Models.end(); ++itr) 
    delete (*itr).second;
  
  m_Models.clear();
  
  std::cout << "NeutrinosInMediumPaper> cleanly destroyed" << std::endl;

} 

void NeutrinosInMediumPaper::GenerateDatapoints(const char * out_model, 
                                                const char * probability , 
                                                ModelParameters * modelpars )
{
  
  bool  anti_nu   = false;
  bool  eval_flux = false;

  m_file->mkdir(TString(out_model) + TString("_0_") + TString(probability))->cd(); // 0 = no model
  
  // Argument "probability" tells if we are working with neutrinos (default) or anti-neutrinos ("a" in front)
  
  std::string Pxx( probability );
  
  unsigned found = Pxx.find("a");
  
  if ( found == 0 ) {
    anti_nu  = true;
    Pxx.erase(0,1);
    std::cout << "GenerateDatapoints> Will treat for anit-nu: " << Pxx << '\n';
  } 
  
  m_tree = new TTree("data","Data points");
  m_tree->Branch("Ex", &m_Ex, "Ex/d");
  m_tree->Branch("Pb", &m_Pb, "Pb/d");
  
  if ( (m_ProbIndex[Pxx].first == 0) && (m_ProbIndex[Pxx].second == 0) ) {
    eval_flux = true;
    m_tree->Branch("Phi_e", &m_Phi_e, "Phi_e/d");
    m_tree->Branch("Phi_m", &m_Phi_m, "Phi_m/d");
    m_tree->Branch("Phi_t", &m_Phi_t, "Phi_t/d");
  }
  
  // Add some control histogram
  TString histo1 = TString("Sum_of_Probs_Row1_") + TString(Pxx);
  TString histo2 = TString("Sum_of_Probs_Col1_") + TString(Pxx);
  h_paper01[histo1.Data()] = new TH1D(histo1.Data(),"Psum_Row1",100,0.99999, 1.0001);
  h_paper01[histo2.Data()] = new TH1D(histo2.Data(),"Psum_Col1",100,0.99999, 1.0001);
  
  DensityModels * density_Mod = m_Models[out_model];
  
  if ( anti_nu ) 
    density_Mod->treat_as_AntiNu();
  else 
    density_Mod->treat_as_Nu();
  
  if( m_debug ) std::cout << "GenerateDatapoints> checking sign in front of the out_model: " << density_Mod->m_sign << std::endl;
  
  double LMIN      = modelpars->GetPar("LMIN");
  double LMAX      = modelpars->GetPar("LMAX");
  long double Ex   = (long double) modelpars->GetPar("Emin");
  long double Emax = (long double) modelpars->GetPar("Emax");
  long double dx   = (long double) modelpars->GetPar("Dx");  //this is the distance step
  long double dE   = (long double) modelpars->GetPar("De");  //this is the energy step
  
  if (m_debug) std::cout << "GenerateDatapoints> Constants: "   << '\n'
                         << "LMIN " << LMIN << '\n'
                         << "LMAX " << LMAX << std::endl;
  
  if (m_debug) std::cout << "GenerateDatapoints> Mixing parameters: " 
                         << "theta1 "   <<  m_Physics->m_input->GetPar1() << '\t'
                         << "theta2 "   <<  m_Physics->m_input->GetPar2() << '\t'
                         << "theta3 "   <<  m_Physics->m_input->GetPar3() << '\t'
                         << "DM2 (32) " <<  m_Physics->m_input->GetPar4() << '\t'
                         << "Dm2 (21) " <<  m_Physics->m_input->GetPar8() << '\n';
  
  int maxpars = (int)modelpars->GetPar(0);
  
  TF1 * profA = new TF1("profA", density_Mod, LMIN, LMAX, maxpars);
  
  for( int i=1; i <= maxpars; ++i) {
    profA->SetParameter( ( i-1 ), (modelpars->GetPar(i))  );
    std::cout << "GenerateDatapoints> * par: " << (modelpars->GetPar(i)) << std::endl;
  }
  
  m_Physics->initializeAngles();
  
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
    
      //std::cout << " debugging: " << x1 << " " << x2 << " " << Ex << " " << profA->Eval( x2 ) << std::endl;
      
      x1  = x2;
      x2 += dx;
      
      i += 1;
      
    }
    
    (*m_Physics->m_Uf) = (*tmp);
    (*m_Physics->m_Ufd) = conj ( (*m_Physics->m_Uf) );
    
    m_Physics->calcProbabilities();

    //get the Transition probability A->B
    
    double d1 = (*m_Physics->m_Prob_AtoB)( m_ProbIndex[Pxx].first , m_ProbIndex[Pxx].second );
    
    if ( ! (boost::math::isnan)(d1) ) {
      m_Ex = Ex;
      m_Pb = d1;
      
      if ( eval_flux ) {
        m_Phi_e = m_Physics->Propagate( 0, 1.0, 2.0, 0.0 ); 
        m_Phi_m = m_Physics->Propagate( 1, 1.0, 2.0, 0.0 ); 
        m_Phi_t = m_Physics->Propagate( 2, 1.0, 2.0, 0.0 ); 
      }
      
      m_tree->Fill();
    
      double SumProbs = 0.0;
      SumProbs = (*m_Physics->m_Prob_AtoB)( 0 , 0 ) + (*m_Physics->m_Prob_AtoB)( 0 , 1 ) + (*m_Physics->m_Prob_AtoB)( 0 , 2 );
      h_paper01[histo1.Data()]->Fill( SumProbs );
      SumProbs = (*m_Physics->m_Prob_AtoB)( 0 , 0 ) + (*m_Physics->m_Prob_AtoB)( 1 , 0 ) + (*m_Physics->m_Prob_AtoB)( 2 , 0 );
      h_paper01[histo2.Data()]->Fill( SumProbs );
            
    }     
    
    k += 1; 
    
    if ( Ex < 1.0E12 )
      Ex += dE; // step in energy
    else if ( Ex >=1.0E12 && Ex < 1.0E13 )
      Ex += (dE*10.0); //step in energy
    else
      Ex += (dE*100.0); //step in energy

    delete tmp;
    
    //if ( k > 1 ) break;
      
  }
  
  std::cout << "GenerateDatapoints> max pts: " << k << std::endl;
  
  m_tree->Write();
  
  h_paper01[histo1.Data()]->Write();
  h_paper01[histo2.Data()]->Write();
  
  m_file->cd("../");

  delete profA;
    
  std::cout << "GenerateDatapoints> all done " << std::endl;  
  
}

void NeutrinosInMediumPaper::Propagate(const char * out_model, const char * in_model,  const char * prev_model,
                                       ModelParameters * modelpars )
{
  
  bool anti_nu = false;
  
  double LMIN      = modelpars->GetPar("LMIN");
  double LMAX      = modelpars->GetPar("LMAX");
  long double dx   = (long double) modelpars->GetPar("Dx");  //this is the distance step
  
  DensityModels * density_Mod = m_Models[out_model];
  
  if ( anti_nu ) density_Mod->treat_as_AntiNu();
  
  int maxpars = (int)modelpars->GetPar(0);
  
  std::cout << "Propagate> checking sign in front of the model: " << density_Mod->m_sign <<  " " << maxpars << std::endl;
  
  TF1 * profA = new TF1("profA", density_Mod, LMIN, LMAX, maxpars);
  
  for( int i=1; i <= maxpars; ++i) {
    profA->SetParameter( ( i-1 ), (modelpars->GetPar(i))  );
    std::cout << "Propagate> * par: " << (modelpars->GetPar(i)) << std::endl;
  }
  
  m_Phi_e = 0.0;
  m_Phi_m = 0.0;
  m_Phi_t = 0.0;
  
  std::vector<std::string> input;
  std::vector<std::string>::iterator inputItr;
  
  input.push_back( std::string("Pee") );
  input.push_back( std::string("aPee") );
  
  for( inputItr = input.begin(); inputItr != input.end(); ++inputItr) {
    
    //Initialize input tree
    bool valid = Init ( in_model, prev_model, (*inputItr).c_str() );
    if( !valid ) continue;
    
    // setup new output tree
        
    m_file->mkdir(TString(out_model) + TString("_") + TString(in_model) + TString("_") + TString((*inputItr).c_str()))->cd();

    TString output = TString(out_model) + TString("_") + TString(in_model) + TString("_") + TString((*inputItr).c_str());
    
    std::cout << "Propagation> output_directory: " <<  output << std::endl;
    
    m_tree = new TTree("data","Data points");
    m_tree->Branch("Ex", &m_Ex, "Ex/d");
    m_tree->Branch("Phi_e", &m_Phi_e, "Phi_e/d");
    m_tree->Branch("Phi_m", &m_Phi_m, "Phi_m/d");
    m_tree->Branch("Phi_t", &m_Phi_t, "Phi_t/d");
    
    //Loop over input, propagate and save into new tree
        
    Long64_t nentries = m_input_tree->GetEntries();
    
    std::cout << "Propagation> n points : " << nentries << std::endl;
    
    m_Physics->initializeAngles();
      
    m_Physics->updateMixingMatrix();
    
    m_Physics->setPotential( profA );

    for (Long64_t i=0;i<nentries;i++) {
      
      m_input_tree->GetEntry(i);
      
      //... we do now the propagation
        
      m_Physics->m_Ev = m_Ex_in;
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

       double d1 = (*m_Physics->m_Prob_AtoB)( m_ProbIndex["Pee"].first , m_ProbIndex["Pee"].second );
      
      if ( ! (boost::math::isnan)(d1) ) {
        
        m_Ex = m_Ex_in;
        m_Phi_e = m_Physics->Propagate( 0, m_Phi_e_in, m_Phi_m_in, m_Phi_t_in ); 
        m_Phi_m = m_Physics->Propagate( 1, m_Phi_e_in, m_Phi_m_in, m_Phi_t_in ); 
        m_Phi_t = m_Physics->Propagate( 2, m_Phi_e_in, m_Phi_m_in, m_Phi_t_in ); 
        
        m_tree->Fill();
        
      } 
      
      delete tmp;
      
    }
    
    m_tree->Write();
    
    m_file->cd("../");
    
  }
    
  std::cout << "Propagate> all done " << std::endl;  
  
}

void NeutrinosInMediumPaper::PropagateVacuum( const char * in_model, const char * src_model )
{
  
  //Initialize input trees
  
  std::vector<std::string> input;
  std::vector<std::string>::iterator inputItr;
  
  input.push_back( std::string("Pee") );
  input.push_back( std::string("aPee") );
  
  for( inputItr = input.begin(); inputItr != input.end(); ++inputItr) {
    
    bool valid = Init ( in_model, src_model, (*inputItr).c_str() );
    
    if( !valid ) continue;
    
    // setup new output tree

    //
    m_file->mkdir( TString("Vacuum") + TString("_") + TString(in_model) + TString("_") + TString( (*inputItr).c_str() ) )->cd();
    
    m_tree = new TTree("data","Data points");
    m_tree->Branch("Ex", &m_Ex, "Ex/d");
    m_tree->Branch("Phi_e", &m_Phi_e, "Phi_e/d");
    m_tree->Branch("Phi_m", &m_Phi_m, "Phi_m/d");
    m_tree->Branch("Phi_t", &m_Phi_t, "Phi_t/d");
    
    //Loop over input, propagate and save into new tree
    ////
    
    Long64_t nentries = m_input_tree->GetEntries();
    
    std::cout << "PropagateVacuum> n points: " << nentries << std::endl;
    
    m_Physics_Vacuum->initializeAngles();
    
    m_Physics_Vacuum->updateMixingMatrix();
    
    for (Long64_t i=0;i<nentries;i++) {
      
      m_input_tree->GetEntry(i);
      
      m_Physics_Vacuum->calcProbabilities();
      
      m_Ex = m_Ex_in;
      m_Phi_e = m_Physics_Vacuum->Propagate( 0, m_Phi_e_in, m_Phi_m_in, m_Phi_t_in ); 
      m_Phi_m = m_Physics_Vacuum->Propagate( 1, m_Phi_e_in, m_Phi_m_in, m_Phi_t_in ); 
      m_Phi_t = m_Physics_Vacuum->Propagate( 2, m_Phi_e_in, m_Phi_m_in, m_Phi_t_in ); 
      
      m_tree->Fill();
      
    }
    
    m_tree->Write();
    
    m_file->cd("../");
    
  }
  
  std::cout << "PropagateVacuum> done!" << std::endl;

}

bool NeutrinosInMediumPaper::Init( const char * in_model , const char * src_step, const char * option)
{
  
  /////

  TString path = TString(in_model) + TString("_") + TString(src_step) + TString("_") + TString(option) + TString("/data"); 
  
  std::cout << "Init> initializing: " << path << std::endl;
  
  m_Ex_in = 0;
  m_Pb_in = 0;
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
  m_input_tree->SetBranchAddress("Pb", &m_Pb_in, &b_Pb_in);
  m_input_tree->SetBranchAddress("Phi_e", &m_Phi_e_in, &b_Phi_e_in);
  m_input_tree->SetBranchAddress("Phi_m", &m_Phi_m_in, &b_Phi_m_in);
  m_input_tree->SetBranchAddress("Phi_t", &m_Phi_t_in, &b_Phi_t_in);
  
  std::cout << "Init> done" << path << std::endl;

  return true;
 
}

