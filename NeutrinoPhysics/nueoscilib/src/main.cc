// $Id: 
// Include files 

#include <MuTrackEvents.h>
#include <ShowerEvents.h>
#include <Graphics.h>
#include <sstream>
#include "sys/stat.h"

#include <boost/program_options.hpp>
namespace po = boost::program_options;

int main(int iargv, char **argv) {
  
  std::string configFile;
  
  try {
    
    po::options_description desc("Allowed options");
    
    desc.add_options()
      ("help"     , "produce help message")
      ("config"   , po::value<std::string>()  , "Configuration file (.xml)")
      ;
    
    po::variables_map vm;
    po::store(po::parse_command_line(iargv, argv, desc), vm);
    po::notify(vm);
    
    if (vm.count("help")) 
    {
      std::cout << desc << "\n";
      return 0;
    }
    
    if (vm.count("config")) 
      configFile = vm["config"].as<std::string>();
    else {
      std::cout << "please provide integration method \n";
      return 1; }
  }
  catch(std::exception& e) 
  {
    std::cerr << "error: " << e.what() << "\n";
    return 1;
  }
  catch(...)  {
    std::cerr << "Exception of unknown type!\n";
    return 1;
  }
  
  std::cout << "-------------------------------------------------------" << '\n';
  
  ParameterList parlist;
  
  if (parlist.ParseFile( configFile.c_str() ) == 0) 
    std::cout << "ParameterList> opened file: " << configFile << '\n';
  else
    return 1;
  
  std::cout << "-------------------------------------------------------" << '\n';
  
  int out = mkdir("results", 0777 );
  if ( out == 0 ) 
    std::cout << "results directory created" << std::endl;
  
  bool debug = false;
  int parset = 1;
  
  Parameters *pars;
  
  while ( (pars = parlist.Next()) ) {
    
    Graphics * graphs = new Graphics();
    
    float phi_neu = 0.0;
    float dphi = 10.0;
    float phi_ne_fr = 0.0;
    float phi_nmu_fr = 0.0;
    float phi_ntau_fr = 0.0;
    
    int k = 0;
    std::stringstream ss_png;
    std::stringstream ss_eps;
    
    ss_png << "./paper01-plots/ratio/ratio.vs.phi_new.pars_" << parset << ".png";
    ss_eps << "./paper01-plots/ratio/ratio.vs.phi_new.pars_" << parset << ".eps";

    while (1) 
    {
      
      if ( phi_neu > 200 ) break;
      if ( phi_neu > 20 ) dphi = 100.0;
      
      float sum = phi_neu + 2.0;
      
      phi_ne_fr = (phi_neu / sum);
      phi_nmu_fr = (1.0 - phi_ne_fr) / 2.0;
      phi_ntau_fr = phi_nmu_fr;
      
      pars->SetPar11( phi_ne_fr );
      pars->SetPar12( phi_nmu_fr );
      pars->SetPar13( phi_ntau_fr );
      
      MuTrackEvents * mu1 = new MuTrackEvents("XSec_neut.dat", "XSec_anti.dat", pars );
      double N_mu_racks = mu1->Evaluate();
      
      double numu_trks  = mu1->m_NuMuTracks;
      
      ShowerEvents * sh1 =  new ShowerEvents("XSec_neut.dat", "XSec_anti.dat", pars );
      double N_shower_racks = sh1->Evaluate();
      
      double ratio = N_mu_racks / N_shower_racks;
      
      double ratio_notau = numu_trks / N_shower_racks;
      
      graphs->g1->SetPoint(k, (phi_neu / sum), ratio);
      graphs->g2->SetPoint(k, (phi_neu / sum), ratio_notau);
      
      if ( ! debug ) 
      {
        std::cout << "-------------------------------------------------------" << '\n';
        std::cout << mu1->EvaluateNuMuContribution() << '\n';
        std::cout << mu1->EvaluateNuTauContribution() << '\n';
        std::cout << sh1->EvaluateNCContribution() << '\n';
        std::cout << sh1->EvaluateCCNueContribution() << '\n';
        std::cout << sh1->EvaluateCCNutauContribution() << '\n';
      }
      
      std::cout << "-------------------------------------------------------" << '\n';
      std::cout << "Ratio Ntracks/Nshower like events: " <<  ratio << std::endl;
      std::cout << phi_ne_fr << " " << phi_nmu_fr << " " << phi_ntau_fr << std::endl;
      std::cout << "-------------------------------------------------------" << '\n';
      
      k = k + 1;
      phi_neu = phi_neu + dphi;
      
      delete sh1;
      delete mu1;
      
    }
    
    TCanvas *c1 = new TCanvas("c1","Graphics canvas",526,216,650,500);
    c1->SetFillColor(10);
    c1->SetTickx(1);
    c1->SetTicky(1);
    c1->cd();
    
    graphs->SetOptions();
    graphs->g1->Draw("ACP");
    graphs->g2->Draw("CP");
    graphs->tleg_1->Draw();
    
    c1->Print(ss_png.str().c_str());
    c1->Print(ss_eps.str().c_str());

    std::cout << "Parameter set: " << parset << " done!" << std::endl;
    
    parset = parset + 1;
    
    delete c1;
    
  }
  
  Graphics * graphs = new Graphics();
  
  double alpha = 2.0;
  
  int k = 0;
  std::stringstream ss_png;
  std::stringstream ss_eps;
  
  ss_png << "./paper01-plots/ratio/ratio.vs.alpha.pars_" << parset << ".png";
  ss_eps << "./paper01-plots/ratio/ratio.vs.alpha.pars_" << parset << ".eps";
  
  pars = parlist.GetParameters(0);
  
  pars->SetPar11( 0.33 );
  pars->SetPar12( 0.33 );
  pars->SetPar13( 0.33 );
  
  MuTrackEvents * mu1 = new MuTrackEvents("XSec_neut.dat", "XSec_anti.dat", pars );
  double N_mu_racks = mu1->Evaluate();
  
  ShowerEvents * sh1 =  new ShowerEvents("XSec_neut.dat", "XSec_anti.dat", pars );
  double N_shower_racks = sh1->Evaluate();
  
  double ratio = N_mu_racks / N_shower_racks;
  
  std::cout << "-------------------------------------------------------" << '\n';
  std::cout << "Ratio Ntracks/Nshower like events: " <<  ratio << std::endl;
  std::cout << "-------------------------------------------------------" << '\n';
  
  pars->SetPar11( 1.00 );
  pars->SetPar12( 1.00 );
  pars->SetPar13( 1.00 );
  
  while (1) 
  {
    
    if ( alpha >= 3.1 ) break;
    
    pars->SetPar3( alpha );
    
    MuTrackEvents * mu1 = new MuTrackEvents("XSec_neut.dat", "XSec_anti.dat", parlist.GetParameters(0) );
    double N_mu_racks = mu1->Evaluate();
    
    double numu_trks  = mu1->m_NuMuTracks;
    
    ShowerEvents * sh1 =  new ShowerEvents("XSec_neut.dat", "XSec_anti.dat", parlist.GetParameters(0) );
    double N_shower_racks = sh1->Evaluate();
    
    double ratio = N_mu_racks / N_shower_racks;
    
    double ratio_notau = numu_trks / N_shower_racks;
    
    graphs->g3->SetPoint(k, alpha, ratio);
    graphs->g4->SetPoint(k, alpha, ratio_notau);
    
    k = k + 1;
    alpha = alpha + 0.05;
    
    delete sh1;
    delete mu1;
    
  }
  
  TCanvas *c1 = new TCanvas("c1","Graphics canvas",526,216,650,500);
  c1->SetFillColor(10);
  c1->SetTickx(1);
  c1->SetTicky(1);
  c1->cd();
  
  graphs->SetOptions();
  graphs->g3->SetMinimum(1.4);
  graphs->g3->SetMarkerStyle(22);
  graphs->g3->SetMarkerColor(2);
  graphs->g3->Draw("ACP");
  graphs->tleg_2->Draw();
  
  c1->Print(ss_png.str().c_str());
  c1->Print(ss_eps.str().c_str());
  
  delete c1;
  
  //.........................................................................................

  return 0;
  
}
