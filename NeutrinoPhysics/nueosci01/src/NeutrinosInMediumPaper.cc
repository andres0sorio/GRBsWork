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

  m_file = new TFile("output.root","RECREATE");
  m_file->cd();
  
  m_debug = true;
  
}

NeutrinosInMediumPaper::NeutrinosInMediumPaper( MixingParameters * mixpars , TFile * prevStep ) {

  m_Physics = new NeutrinoOscInVarDensity( mixpars );
  m_Physics->use_default_pars = false;
  
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

  m_file = prevStep;
  
  m_file->cd();
  
  m_debug = true;
  
}

//=============================================================================
// Destructor
//=============================================================================
NeutrinosInMediumPaper::~NeutrinosInMediumPaper() {

  delete m_Physics;

  delete m_Models["ModelA"];
  delete m_Models["ModelB"];
  delete m_Models["ModelC"];

  m_file->Close();

  
} 

//=============================================================================
void NeutrinosInMediumPaper::Test()
{

  //... Make the bloody plots ;-)

  h_canvas["PeePadA"] = new TCanvas("PeePadA", "Oscillation probabilities", 184, 60, 861, 670);

  h_canvas["PeePadA"]->SetLogx();
  
  h_paper01Graphs["PeeA"] = new TGraph();
  
  double K0   = (4.0E-6) * 4.2951E18;
  double LMIN = (8.0E8)  * IProbabilityMatrix::InvEvfactor;
  double LMAX = (3.0E10) * IProbabilityMatrix::InvEvfactor;
  
  if (m_debug) std::cout << "Constants: " << '\n'
                         << "K0 " << K0 << '\n'
                         << "LMIN " << LMIN << '\n'
                         << "LMAX " << LMAX << std::endl;

  m_Physics->initializeAngles();

  m_Physics->updateMixingMatrix();
  
  //................ Profile A
  
  TF1 * profA = new TF1("profA", densityModA, LMIN, LMAX, 2);
  profA->SetParameter(0, K0 );
  profA->SetParameter(1, LMAX );
  
  m_Physics->setPotential( profA );
  
  long double Ex = 1.0E11L;
  long double Emax = 1.0E14L;
  long double dx = 0.5E14L; // This is the distance step
  
  int k = 0;
  
  long double LRes1 = 1.1E9 * IProbabilityMatrix::InvEvfactor; //starting point along the radius of the star
  
  while ( Ex <= Emax ) {
    
    m_Physics->m_Ev = Ex;
    m_Physics->updateEab();
    
    matrix <std::complex< long double> >  * tmp;
    tmp = new matrix<std::complex< long double> >(3,3);
    
    double long x1 = 0.0;
    double long x2 = LRes1;
    
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
    
    double d1 = (*m_Physics->m_Prob_AtoB)(0,0);

    if ( ! (boost::math::isnan)(d1) )
      h_paper01Graphs["PeeA"]->SetPoint( k, Ex, d1);
    
    k += 1; 
    
    if ( Ex < 1.0E12 )
      Ex += 1.0E10L; // step in energy
    else
      Ex += 1.0E12L; //step in energy
    
    delete tmp;
    
  }
  
  std::cout << "max pts: " << k << std::endl;
  
  h_canvas["PeePadA"]->cd();
  h_paper01Graphs["PeeA"]->Draw("APL");
  h_canvas["PeePadA"]->Print("results/profile-A-probabilities.png");
  
  std::cout << " all done " << std::endl;  
  
  delete h_canvas["PeePadA"];
    
}


void NeutrinosInMediumPaper::GenerateDatapoints(const char * model, 
                                                const char * probability , 
                                                ModelParameters * modelpars )
{

  double m_Ex  = 0.0;
  double m_Pb  = 0.0;
  double m_Phi = 0.0;

  bool  eval_flux = false;
  
  m_file->mkdir(TString(model) + TString("_") + TString(probability))->cd();

  m_tree = new TTree("data","Data points");
  m_tree->Branch("Ex", &m_Ex, "Ex/d");
  m_tree->Branch("Pb", &m_Pb, "Pb/d");

  if ( m_ProbIndex[probability].first == m_ProbIndex[probability].second ) {
    eval_flux = true;
    m_tree->Branch("Phi", &m_Phi, "Phi/d");
  }
  
  DensityModels * density_Mod = m_Models[model];
  
  double Gf = DensityModels::GF * DensityModels::InveV2; // [1/eV^2]
  double Ar = (1.0/sqrt(2.0)) * Gf * (1.0/DensityModels::Mp); // This has a wrong factor of 2 (Sarira)
  
  double K0   = (4.0E-6) * 4.2951E18 * Ar;
  double LMAX = (3.0E10) * IProbabilityMatrix::InvEvfactor;
  double LMIN = (8.0E8)  * IProbabilityMatrix::InvEvfactor;

  if (m_debug) std::cout << "Constants: " << '\n'
                         << "K0 " << K0 << '\n'
                         << "LMIN " << LMIN << '\n'
                         << "LMAX " << LMAX << std::endl;

  long double Ex   = (long double) modelpars->GetPar("Emin");
  long double Emax = (long double) modelpars->GetPar("Emax");
  long double dx   = (long double) modelpars->GetPar("Dx");  //this is the distance step
  long double dE   = (long double) modelpars->GetPar("De");  //this is the energy step
  
  int maxpars = (int)modelpars->GetPar(0);

  TF1 * profA = new TF1("profA", density_Mod, LMIN, LMAX, maxpars);
   
  for( int i=1; i <= maxpars; ++i) {
    profA->SetParameter( ( i-1 ), (modelpars->GetPar(i))  );
  }
  
  m_Physics->initializeAngles();
  
  m_Physics->updateMixingMatrix();
  
  m_Physics->setPotential( profA );
  
  int k = 0;
  
  //long double LRes1 = 0.1E9 * IProbabilityMatrix::InvEvfactor; //starting point along the radius of the star
  
  std::cout << "GenerateDatapoints> looping over energy> " << std::endl;
  
  while ( Ex <= Emax ) {
    
    m_Physics->m_Ev = Ex;
    m_Physics->updateEab();
    
    matrix <std::complex< long double> >  * tmp;
    tmp = new matrix<std::complex< long double> >(3,3);
    
    double long x1 = 0.0;
    double long x2 = LMIN;
    
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

    //get the Transition probability A->B
    double d1 = (*m_Physics->m_Prob_AtoB)( m_ProbIndex[probability].first , m_ProbIndex[probability].second );
    
    if ( ! (boost::math::isnan)(d1) ) {
      m_Ex = Ex;
      m_Pb = d1;
      if ( eval_flux ) 
        m_Phi = m_Physics->Propagate( m_ProbIndex[probability].first, 1.0, 2.0, 0.0 ); 
      m_tree->Fill();
    }
    
    k += 1; 
    
    if ( Ex < 1.0E12 )
      Ex += dE; // step in energy
    else if ( Ex >=1.0E12 && Ex < 1.0E13 )
      Ex += (dE*10.0); //step in energy
    else
      Ex += (dE*100.0); //step in energy

    delete tmp;
    
  }
  
  std::cout << "GenerateDatapoints> max pts: " << k << std::endl;
  
  m_tree->Write();

  m_file->cd("../");
  
  std::cout << "GenerateDatapoints> all done " << std::endl;  
  
}


void NeutrinosInMediumPaper::PropagateVacuum( const char * model, const char * probability )
{

  //Initialize input tree
  
  //Init ( model, probability );

  // setup new output tree
  
  m_file->mkdir(TString(model) + TString("_") + TString(probability) + TString("_Vacuum") )->cd();

  double m_Ex = 0.0;
  double m_Pb = 0.0;

  m_tree = new TTree("data","Data points");
  m_tree->Branch("Ex", &m_Ex, "Ex/d");
  m_tree->Branch("Pb", &m_Pb, "Pb/d");
  
  //Loop over input, propagate and save into new tree
  
  
  //

}

void NeutrinosInMediumPaper::Init( const char * model, const char * probability )
{
  
  TString path = TString(model) + TString("_") + TString(probability) + TString("/data");
  
  m_in_Ex = 0;
  m_in_Pb = 0;
  
  m_file->cd();
  
  m_input_tree = (TTree*)gDirectory->Get(path);
  
  // Set branch addresses and branch pointers
  if (!m_input_tree)  { 
    std::cout << " Init> could not find Tree " << std::endl;
    return;
  }
  
  m_input_tree->SetBranchAddress("Ex", &m_in_Ex, &b_in_Ex);
  m_input_tree->SetBranchAddress("Pb", &m_in_Pb, &b_in_Pb);
  
}

