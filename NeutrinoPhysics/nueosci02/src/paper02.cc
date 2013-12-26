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
  std::string steps;
  
  try {
    
    po::options_description desc("Allowed options");
    
    desc.add_options()
      ("help"     , "produce help message")
      ("input"    , po::value<std::string>(), "input file")
      ("config"   , po::value<std::string>(), "configuration file (.xml)")
      ("steps"    , po::value<std::string>(), "excecute steps ( 1,2,3,... )")
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

    if (vm.count("steps")) {
      steps = vm["steps"].as<std::string>();
      std::cout << "steps to run over " << steps << std::endl;
    } 
    else {
      std::cout << "using the default step 1  \n";
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

  MixingParameterList mixparlist;
  
  std::cout << " ---MixingParameterList: read ---" << '\n';
  
  if (mixparlist.ParseFile("matrix_config.xml") == 0)
    std::cout << mixparlist;
  
  MixingParameters *mixpars =  mixparlist.GetParameters(0);
  
  //............................................................................................
  
  //Step Selection
  
  std::map<std::string,bool> execSteps;
  execSteps["1"] = false;
  execSteps["2"] = false;
  execSteps["3"] = false;
  execSteps["4"] = false;
  
  if( steps.size() != 0 )
  {
    std::vector<std::string> stepstr;
    boost::split(stepstr, steps, boost::is_any_of(","));
    std::vector<std::string>::iterator itr;
    for( itr = stepstr.begin(); itr != stepstr.end(); ++itr) {
      execSteps[ *itr ] = true;
      std::cout << (*itr) << " turns true" << std::endl;
    }
    
  }

  //............................................................................................

  //Setup dataset to run over
  std::vector<std::string> dataset;
  std::vector<std::string>::iterator itr;

  std::ifstream * m_in = new std::ifstream( input.c_str(), ifstream::in);
  
  if(!m_in->is_open()) {
    std::cout << "Data> cannot open file" << std::endl;
  } else { 
    std::cout << "Data> file is now open" << std::endl;
  }

  std::string strfile;
  
  while( m_in->good() ) {
    
    if ( m_in->eof() ) break;
    
    (*m_in) >> strfile;
    
    dataset.push_back( strfile );
    
    if ( m_in->fail()) break;

  }
  
  //............................................................................................

  // Variation 1 ( IceCube! -> R calculation as a function of alpha )
  // 
  // -- (Start from the output of paper01 which contains the fluxes at the detector for
  //     a specific model )
  //
  
  NeutrinosDetectionPaper * nudet;
    
  nudet = new NeutrinosDetectionPaper( pars );
  
  //Work out first the standard picture
  
  if( execSteps["1"] ) {
    
    nudet->MakeVariationStdPicture("EarthB","Vacuum", 2.0, 3.1, 0.05); // 
    
  }
  
  // (EarthB,Vacuum) == Model X ---> Vaccuum ---> Earth ---> Detector
  
  //... Loop over the different datasets
  
  if( execSteps["2"] ) {

    TFile * infile;
    std::string model;
  

    for( itr = dataset.begin(); itr != dataset.end(); ++itr) 
    {
      
      std::cout << (*itr) << std::endl;
      
      infile = new TFile( (*itr).c_str(), "READ");
      
      unsigned pos2 = (*itr).rfind("/");
      unsigned pos1 = (*itr).rfind("/", pos2-1);
      
      model = (*itr).substr(pos1+1, (pos2-pos1-1) );
      
      //std::cout << pos1 << " " << pos2 << " " << (*itr).substr(pos1+1, (pos2-pos1-1) ) << std::endl;
      
      nudet->SetFluxHistograms(infile, model.c_str(), "EarthB","Vacuum" );
      
      nudet->MakeVariation02(model.c_str(), "EarthB","Vacuum", 2.0, 3.1, 0.05); //
      
      nudet->ResetFluxHistograms();
      
      infile->Close();
   
      //break;
      
      
    }
    
  }
  
  if( execSteps["3"] ) {
    
    nudet->MakeVariation03("StdPicture", "EarthB","Vacuum", 0.0, 1000.0, 1.0); //
  
  }

  if( execSteps["4"] ) {
    
    nudet->SetMixingParameters (  mixpars );
    
    nudet->MakeVariation04("StdPicture", "EarthB", "Vacuum", 0.0, 15.0, 1.0, 1.8, 0.0); //

    nudet->MakeVariation04("StdPicture", "EarthB", "Vacuum", 0.0, 15.0, 1.0, 1.8, 180.0); //
    
  }
  
  //
  
  delete nudet;
  
  //............................................................................................
  
  
  //............................................................................................

  return 0;
  
}
  

