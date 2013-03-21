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
  std::string angles;
  std::string dmass;
  std::string modfile;
  std::string steps;
  std::string earth;
  
  try {
    
    po::options_description desc("Allowed options");
    
    desc.add_options()
      ("help"     , "produce help message")
      ("model"    , po::value<std::string>(), "model selection ( ModelA, ModelB, ModelC )")
      ("prob"     , po::value<std::string>(), "selected probability ( Pee,Pem,Pet,Pmm,Ptt,... )")
      ("erange"   , po::value<std::string>(), "energy range ( = emin,emax )")
      ("angles"   , po::value<std::string>(), "use the following mixing angles ( = theta_1(12),theta_2(13),theta3(23) )")
      ("dmass2"   , po::value<std::string>(), "use the following delta masses square ( = dm2(Dm23sq),dM2(Dm21sq) )")
      ("modfile"  , po::value<std::string>(), "use the specified model paramater file ( mymodelfile.xml )")
      ("steps"    , po::value<std::string>(), "excecute steps ( 1,2,3,... )")
      ("earth"    , po::value<std::string>(), "select earth model (EarthA, EarthB)")

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

    if (vm.count("angles")) {
      angles = vm["angles"].as<std::string>();
      std::cout << "mixing will be set to " <<  angles << std::endl;
    } 
    else {
      std::cout << "using the mixing angles as in the configuration file \n";
    }

    if (vm.count("dmass2")) {
      dmass = vm["dmass2"].as<std::string>();
      std::cout << "dm2 and dM2 will be set to " <<  dmass << std::endl;
    } 
    else {
      std::cout << "using dm2 and dM2 read from the configuration file \n";
    }
    
    if (vm.count("modfile")) {
      modfile = vm["modfile"].as<std::string>();
      std::cout << "model configuration file will be set to " <<  modfile << std::endl;
    } 
    else {
      std::cout << "using the default model configuration file \n";
    }
    
    if (vm.count("steps")) {
      steps = vm["steps"].as<std::string>();
      std::cout << "steps to run over " << steps << std::endl;
    } 
    else {
      std::cout << "using the default step 1  \n";
    }
    
    if (vm.count("earth")) {
      earth = vm["earth"].as<std::string>();
      std::cout << "select earth model " << earth << std::endl;
    } 
    else {
      earth = std::string("EarthB");
      std::cout << "using the default EarthB model \n";
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
  
  std::cout << " ---MixingParameterList: read ---" << '\n';
  
  if (mixparlist.ParseFile("matrix_config.xml") == 0)
    std::cout << mixparlist;
  
  MixingParameters *mixpars =  mixparlist.GetParameters(0);
  
  if( angles.size() != 0 )
  {
    std::vector<std::string> theta;
    boost::split(theta, angles, boost::is_any_of(","));
    if( theta.size() != 3 ) {
      std::cout << " you need to provide all three angles in the correct order. Type --help" << std::endl;
      return 1;
    }
    else
    {
      mixpars->SetPar1( atof( theta[0].c_str() ) );
      mixpars->SetPar2( atof( theta[1].c_str() ) );
      mixpars->SetPar3( atof( theta[2].c_str() ) );
    }
    std::cout << (*mixpars) << std::endl;
  }
  
  if( dmass.size() != 0 )
  {
    std::vector<std::string> dmasses;
    boost::split(dmasses, dmass, boost::is_any_of(","));
    if( dmasses.size() != 2 ) {
      std::cout << " you need to provide both mass differences. Type --help" << std::endl;
      return 1;
    }
    else 
    {
      mixpars->SetPar4( atof( dmasses[0].c_str() ) );
      mixpars->SetPar8( atof( dmasses[1].c_str() ) );
    }
    std::cout << (*mixpars) << std::endl;
  }
  
  std::cout << " ---MixingParameterList: read ---" << '\n';

  //............................................................................................

  ModelParameterList modparlist;

  std::cout << " ---ModelParameterList---" << '\n';
  
  if( modfile.size() != 0 )
  {
    if (modparlist.ParseFile( modfile.c_str() ) == 0) 
      std::cout << modparlist;
  }
  else 
  {
    if (modparlist.ParseFile("model_config.xml") == 0) 
      std::cout << modparlist;
  }
  
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

  std::cout << " ---ModelParameterList: read ---" << '\n';
  
  //............................................................................................

  //Step Selection
  
  std::map<std::string,bool> execSteps;
  execSteps["1"] = true;
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

  // Step 1 ( IceCube! -> R calculation )
  // 
  // -- (Start from the output of paper01 which contains the fluxes at the surface of earth for
  //     a specific model )
  //
  // TFile * infile == output.root
  //
  //............................................................................................
  
  

  
  //............................................................................................

  return 0;
  
}
  

