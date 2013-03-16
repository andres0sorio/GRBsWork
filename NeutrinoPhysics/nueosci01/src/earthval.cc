// $Id: 
// Include files 
#include <NeutrinosInMediumPaper.h>
#include <iostream>
#include <sstream>
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>

namespace po = boost::program_options;

///////////////////////////////////////////////////////////////

int main(int iargv, char **argv) {

  std::string model;
  std::string prob;
  std::string erange;
      
  try {
    
    po::options_description desc("Allowed options");
    
    desc.add_options()
      ("help"     , "produce help message")
      ("model"    , po::value<std::string>(), "model selection ( ModelA, ModelB, ModelC )")
      ("prob"     , po::value<std::string>(), "selected probability ( Pee,Pem,Pet,Pmm,Ptt,... )")
      ("erange"   , po::value<std::string>(), "energy range ( emin,emax )")
      ;
    
    po::variables_map vm;
    po::store(po::parse_command_line(iargv, argv, desc), vm);
    po::notify(vm);
    
    if (vm.count("help")) 
    {
      std::cout << desc << "\n";
      return 0;
    }
    
    if (vm.count("model")) 
      model = vm["model"].as<std::string>();
    else {
      std::cout << "please select wich model to run on \n";
      return 1; }
    
    if (vm.count("prob")) 
      prob = vm["prob"].as<std::string>();
    else {
      std::cout << "please select wich probability to run on \n";
      return 1; }

    if (vm.count("erange")) {
      erange = vm["erange"].as<std::string>();
      std::cout << "energy range will be set to " <<  erange << std::endl;
    } 
    else {
      std::cout << "energy range reads from the configuration file \n";
    }
    
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
  

  //............................................................................................
  
  MixingParameterList mixparlist;
  
  if (mixparlist.ParseFile("matrix_config.xml") == 0)
    std::cout << mixparlist;
  std::cout << "MixingParameterList-------------------------------------------------" << '\n';
  
  MixingParameters *mixpars =  mixparlist.GetParameters(0);
  
  //............................................................................................

  ModelParameterList modparlist;
  
  if (modparlist.ParseFile("model_config.xml") == 0) 
    std::cout << modparlist;
  std::cout << "ModelParameterList-------------------------------------------------" << '\n';
  
  ModelParameters *modpars =  modparlist.GetParameters(model.c_str());

  
    
  if( erange.size() != 0 )
  {
    std::vector<std::string> rangestr;
    boost::split(rangestr, erange, boost::is_any_of(","));
    if( rangestr.size() != 2 ) {
      std::cout << " you provided a wrong range. Type --help" << std::endl;
      return 1;
    }
    else 
    {
      modpars->SetPar("Emin", atof( rangestr[0].c_str() ) );
      modpars->SetPar("Emax", atof( rangestr[1].c_str() ) );
    }
    std::cout << (*modpars) << std::endl;
  }
      
  //............................................................................................

  //Set parameters as in the Olshonn paper
  
  mixpars->SetPar1(45.0);
  mixpars->SetPar2(5.0);
  mixpars->SetPar3(45.0);
  mixpars->SetPar8(0.0);
  
  NeutrinosInMediumPaper * neuOsc = new NeutrinosInMediumPaper( mixpars );
  
  std::vector<std::string> strs;
  
  boost::split(strs, prob, boost::is_any_of(","));

  std::vector<std::string>::iterator itr;
  
  for( itr = strs.begin(); itr != strs.end(); ++itr ) 
  {
   
    std::cout << (*itr) << std::endl;
    neuOsc->GenerateDatapoints( model.c_str(), (*itr).c_str(), modpars );
    
  }

  delete neuOsc;
  
  strs.clear();
  
  return 0;
  
}
