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
  std::string antinu;
  std::string dCP;
  
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
      ("antinu"   , po::value<std::string>(), "select if you want to study with antinu ( Ax = -1.0*Ax : step4)")
      ("dCP"      , po::value<std::string>(), "dCP phase in degrees ( 0.0, 180.0 , ... )")
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
    
    if (vm.count("dCP")) {
      dCP = vm["dCP"].as<std::string>();
      std::cout << "dCP value fixed to " << dCP << std::endl;
    } 
    else {
      std::cout << "using the default value of dCP (as read in xml config) \n";
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

    if (vm.count("antinu")) {
      antinu = vm["antinu"].as<std::string>();
      std::cout << "Will multiply potencial by -1.0 == antinu " << antinu << std::endl;
    } 
    else {
      antinu = std::string("0");
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
      mixpars->SetPar4( atof( dmasses[0].c_str() ) ); // DM2 = DM(32)
      mixpars->SetPar8( atof( dmasses[1].c_str() ) ); // Dm2 = DM(12)
    }
  }
  
  if( dCP.size() != 0 )
  {
    mixpars->SetPar9( atof( dCP.c_str() ) ); // dCP
  }

  std::cout << " ---MixingParameterList: after manual modification ---" << '\n';
  
  std::cout << (*mixpars) << std::endl;
  
  std::cout << " ---MixingParameterList: done ---" << '\n';
  
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

  std::cout << " ---ModelParameterList: done ---" << '\n';
  
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
    
    if( execSteps["4"] ) execSteps["1"] = false; 
  
  }

  //............................................................................................

  NeutrinosInMediumPaper * neuOsc;

  //Step 1 (Star)

  if( execSteps["1"] ) {

    neuOsc = new NeutrinosInMediumPaper( mixpars );
  
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
  
  }

  //............................................................................................

  //Step 2 (Vacuum)

  TFile * infile;

  if( execSteps["2"] ) {

    infile = new TFile("output.root","UPDATE");
  
    neuOsc = new NeutrinosInMediumPaper( mixpars , infile );
  
    neuOsc->PropagateVacuum( model.c_str(), "0" ); // the average method for propagation
  
    delete neuOsc;

  }

  //............................................................................................

  //Step 3 (Earth)
  
  if( execSteps["3"] ) {
    
    infile = new TFile("output.root","UPDATE");
    
    modpars =  modparlist.GetParameters( earth.c_str() );
    
    neuOsc = new NeutrinosInMediumPaper( mixpars , infile );
    
    neuOsc->Propagate( earth.c_str(), "Vacuum" , model.c_str() , modpars); // Propagation through Earth
    
    delete neuOsc;
    
  }

  //............................................................................................
  
  //Step 4 (Resonance studies)
  
  if( execSteps["4"] ) {
    
    neuOsc = new NeutrinosInMediumPaper( mixpars );
    
    bool use_antinu =  (bool) atoi( antinu.c_str() );
    
    neuOsc->StudyResonances( model.c_str(), prob.c_str(), modpars, use_antinu);
	  
    delete neuOsc;
    
  }
  
  //............................................................................................

  return 0;
  
}
  

