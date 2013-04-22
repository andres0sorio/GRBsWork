// $Id: 
// Include files 
#include <NeutrinosDetectionPaper.h>
#include <iostream>
#include <sstream>
#include <boost/program_options.hpp>
#include <boost/algorithm/string.hpp>

namespace po = boost::program_options;

///////////////////////////////////////////////////////////////

int main(int iargv, char **argv) {

  std::string input;
  std::string config;
  std::string erange;
  std::string angles;
  std::string dmass;
  
  try {
    
    po::options_description desc("Allowed options");
    
    desc.add_options()
      ("help"     , "produce help message")
      ("input"    , po::value<std::string>(), "input file")
      ("config"   , po::value<std::string>()  , "configuration file (.xml)")
      ;
    
    po::variables_map vm;
    po::store(po::parse_command_line(iargv, argv, desc), vm);
    po::notify(vm);
    
    if (vm.count("help")) 
    {
      std::cout << desc << "\n";
      return 0;
    }
        
    if (vm.count("input")) 
      input = vm["input"].as<std::string>();
    else {
      std::cout << "please select which file to run on \n";
      return 1; }
    
    if (vm.count("config")) 
      config = vm["config"].as<std::string>();
    else {
      config = std::string("config.xml");
      std::cout << "using the default configuration file config.xml \n";
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
  
  ParameterList parlist;
  
  if (parlist.ParseFile( config.c_str() ) == 0) 
    std::cout << "ParameterList> opened file: " << config << '\n';
  else
    return 1;
  
  Parameters *pars = parlist.Next();
  
  //............................................................................................

  //Step Selection
  
  std::map<std::string,bool> execSteps;
  execSteps["1"] = true;

  //............................................................................................

  // Step 1 ( IceCube! -> R calculation )
  // 
  // -- (Start from the output of paper01 which contains the fluxes at the surface of earth for
  //     a specific model )
  //

  TFile * infile;

  NeutrinosDetectionPaper * nudet;
  
  infile = new TFile( input.c_str(), "READ");
  
  nudet = new NeutrinosDetectionPaper( pars , infile );
  
  nudet->MakeVariation01("Vacuum","ModelA");
  
  delete nudet;
  
  //............................................................................................
  
  
  //............................................................................................

  return 0;
  
}
  

