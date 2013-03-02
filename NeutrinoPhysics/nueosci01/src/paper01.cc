// $Id: 
// Include files 
#include <NeutrinosInMediumPaper.h>
#include <iostream>
#include <sstream>
#include <boost/program_options.hpp>
namespace po = boost::program_options;

///////////////////////////////////////////////////////////////

int main(int iargv, char **argv) {

  std::string model;
  std::string prob;
    
  try {
    
    po::options_description desc("Allowed options");
    
    desc.add_options()
      ("help"     , "produce help message")
      ("model"    , po::value<std::string>(), "model selection")
      ("prob"     , po::value<std::string>(), "selected probability")
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
  
  std::cout << (*modpars) << std::endl;
    
  //............................................................................................
  
  NeutrinosInMediumPaper * neuOsc = new NeutrinosInMediumPaper( mixpars );
  
  //neuOsc->Test();
    
  neuOsc->GenerateDatapoints( model.c_str(), prob.c_str(), modpars );
  
  delete neuOsc;
  
  return 0;
  
}
