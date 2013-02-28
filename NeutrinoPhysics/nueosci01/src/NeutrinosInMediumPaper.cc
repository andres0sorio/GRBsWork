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


  m_modelA = new NeutrinoOscInVarDensity( mixpars );
  
  //
  
  //
  
  
  m_debug = true;
  
}
//=============================================================================
// Destructor
//=============================================================================
NeutrinosInMediumPaper::~NeutrinosInMediumPaper() {

  delete m_modelA;
  
} 

//=============================================================================
void NeutrinosInMediumPaper::MakePlots() 
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
  
  //................ Profile No A
  
b  TF1 * profA = new TF1("profA", densityModA, LMIN, LMAX, 2);
  profA->SetParameter(0, K0 );
  profA->SetParameter(1, LMAX );
  
  m_modelA->setPotential( profA );
  
  m_modelA->setAngle(1, 2, 33.83);  //theta_12
  m_modelA->setAngle(1, 3, 6.0);    //theta_13
  m_modelA->setAngle(2, 3, 45.0);   //theta_23
  
  m_modelA->m_DeltamSq = 8.0E-5L;
  m_modelA->m_DeltaMSq = 1.4E-3L;
  
  m_modelA->updateMixingMatrix();
  
  long double Ex = 1.0E11L;
  long double Emax = 1.0E14L;
  long double dx = 0.5E13L;
  
  int k = 0;
  int counter = 0;
  
  long double LRes1 = 1.1E9 * IProbabilityMatrix::InvEvfactor;
  long double LRes2 = 1.3E9 * IProbabilityMatrix::InvEvfactor;
  
  while ( Ex <= Emax ) {
    
    m_modelA->m_Ev = Ex;
    m_modelA->updateEab();
    
    matrix <std::complex< long double> >  * tmp;
    tmp = new matrix<std::complex< long double> >(3,3);
    
    double long x1 = 0.0;
    double long x2 = LRes1;
    
    int i = 0;
    
    while ( x2 <= LMAX ) {
      
      m_modelA->Eval_UFlavour( x2, x1 );
      
      if( i == 0) {
        (*tmp) = (*m_modelA->m_Uf); 
      } else
        (*tmp) = prod( (*m_modelA->m_Uf), (*tmp) );
      
      x1  = x2;
      x2 += dx;
      
      //std::cout << i << " " << x2 << std::endl;
      
      i += 1;
      counter  += 1;
      
    }
    
    (*m_modelA->m_Uf) = (*tmp);
    (*m_modelA->m_Ufd) = conj ( (*m_modelA->m_Uf) );
    
    m_modelA->calcProbabilities();
    
    double d1 = (*m_modelA->m_Prob_AtoB)(0,0);

    if ( ! (boost::math::isnan)(d1) )
      h_paper01Graphs["PeeA"]->SetPoint( k, Ex, d1);
    

    //d1 = (*m_Prob_AtoB)(0,1);
    //if ( ! (boost::math::isnan)(d1) )
    //  gEv[1]->SetPoint( k, Ex, d1);
    
    //d1 = (*m_Prob_AtoB)(0,2);
    //if ( ! (boost::math::isnan)(d1) )
    //  gEv[2]->SetPoint( k, Ex, d1);
    
    //d1 = (*m_Prob_AtoB)(1,2);
    //if ( ! (boost::math::isnan)(d1) )
    //  gEv[3]->SetPoint( k, Ex, d1);

    //std::cout << k << " " << Ex << " " << d1 << std::endl;
    
    k += 1; 
    
    if ( Ex < 1.0E12 )
      Ex += 0.5E10L;
    else
      Ex += 1.0E12L;
    
    delete tmp;
    
  }

  std::cout << "max pts: " << k << std::endl;

  h_canvas["PeePadA"]->cd();
  h_paper01Graphs["PeeA"]->Draw("APL");
  h_canvas["PeePadA"]->Print("results/profile-A-probabilities.C");

  //std::vector<std::string> labels;
  //labels.push_back( std::string("Pee") );
  //labels.push_back( std::string("Pe#mu") );
  //labels.push_back( std::string("Pe#tau") );
  //labels.push_back( std::string("P#mu#tau") );

  //valA->cd();
  //valA->Print("results/profile-A-validation.C");
 
  std::cout << " all done " << std::endl;  
  


}
