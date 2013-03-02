// $Id: 
// Include files 
#include <NeutrinoOscInVarDensity.h>
#include <iostream>
#include <sstream>
#include <boost/program_options.hpp>
namespace po = boost::program_options;

///////////////////////////////////////////////////////////////

int main(int iargv, char **argv) {

  int valOpt;
  
  try {
    
    po::options_description desc("Allowed options");
    
    desc.add_options()
      ("help"     , "produce help message")
      ("opt"      , po::value<int>(), "validation option")
      ;
    
    po::variables_map vm;
    po::store(po::parse_command_line(iargv, argv, desc), vm);
    po::notify(vm);
    
    if (vm.count("help")) 
      {
        std::cout << desc << "\n";
        return 0;
      }
    
    if (vm.count("opt")) 
      valOpt = vm["opt"].as<int>();
    else {
      std::cout << "please select wich validation set to run \n";
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

  NeutrinoOscInVarDensity * neuOsc = new NeutrinoOscInVarDensity( mixpars );

  if( valOpt == 1 ) 
    neuOsc->Validate();
  else if ( valOpt == 2 ) 
    neuOsc->ValidateInVarDensity();
  else if ( valOpt == 3 ) 
    neuOsc->ValidateSolarProfile();
  else if ( valOpt == 4 ) 
    neuOsc->ValidateProfileA();
  else if ( valOpt == 5 ) 
    neuOsc->TestProcedure();
  else {
    std::cout << "Not a valid option" << std::endl;
  }
  
  delete neuOsc;

  return 0;
  
}
