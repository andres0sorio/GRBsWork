// $Id: 
// Include files 
#include <NeutrinoOscInVarDensity.h>
#include <iostream>
#include <sstream>
#include <boost/program_options.hpp>
namespace po = boost::program_options;

///////////////////////////////////////////////////////////////

int main(int iargv, char **argv) {

  //............................................................................................
  
  MixingParameterList mixparlist;
  
  if (mixparlist.ParseFile("matrix_config.xml") == 0)
    std::cout << mixparlist;
  std::cout << "MixingParameterList-------------------------------------------------" << '\n';
  
  MixingParameters *mixpars =  mixparlist.GetParameters(0);
  
  //............................................................................................
  
  NeutrinoOscInVarDensity * neuOsc = new NeutrinoOscInVarDensity( mixpars );
  
  neuOsc->ValidateProfileA();
  
  neuOsc->TestProfileA();
  
  delete neuOsc;
  
  return 0;
  
}
