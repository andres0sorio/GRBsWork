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

  //............................................................................................
  
  MixingParameterList mixparlist;
  
  if (mixparlist.ParseFile("matrix_config.xml") == 0)
    std::cout << mixparlist;
  std::cout << "MixingParameterList-------------------------------------------------" << '\n';
  
  MixingParameters *mixpars =  mixparlist.GetParameters(0);
  
  //............................................................................................

  ModelParameterList modparlist;
  
  if (modparlist.ParseFile("model_config_validation.xml") == 0) 
    std::cout << modparlist;
  std::cout << "ModelParameterList-------------------------------------------------" << '\n';
  
  //............................................................................................

  std::string models("solar,linear");
  
  NeutrinosInMediumPaper * neuOsc = new NeutrinosInMediumPaper( mixpars );
  
  std::vector<std::string> strs;
  
  boost::split(strs, models, boost::is_any_of(","));

  std::vector<std::string>::iterator itr;
  
  for( itr = strs.begin(); itr != strs.end(); ++itr ) 
  {
    
    std::cout << (*itr) << std::endl;
    
    ModelParameters *modpars =  modparlist.GetParameters((*itr).c_str());
    
    neuOsc->GenerateDatapoints( (*itr).c_str(), "Pem", modpars );
    
  }

  delete neuOsc;
  
  strs.clear();



  return 0;
  
}
