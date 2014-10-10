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
  std::string neuosc;
  std::string steps;
  std::string dmass;
  std::string angles;
  std::string dCP;

  std::vector<std::string> avsteps;
  
  avsteps.push_back("1. R vs the spectral index alpha for the standard picture" );
  avsteps.push_back("2. R vs the spectral index alpha for different star models - needs step 1" );
  avsteps.push_back("3. R vs phi_e fraction - standard picture" );
  avsteps.push_back("4. R as a function of sin2(theta_13) - for different dCP values - standard picture" );
  avsteps.push_back("5. R vs Ev energy - as in Olga Mena reference" );

  //... Added Oct/2014 - AO
  
  avsteps.push_back("6. R vs phi_e fraction - standard picture - Form diff. star models" );
  
  avsteps.push_back("7. R vs a function of sin2(theta_13) - Form diff. star models" );
  
  bool no_dataset = false;
  
  try {
    
    po::options_description desc("Allowed options");
    
    desc.add_options()
      ("help"     , "produce help message")
      ("input"    , po::value<std::string>(), "input file (.root) containing fluxes through diff. media")
      ("config"   , po::value<std::string>(), "model configuration file (.xml)")
      ("neuosc"   , po::value<std::string>(), "neutrino oscillation data & input constants (.xml)")
      ("steps"    , po::value<std::string>(), "excecution steps ( 1,2,3,... )")
      ("dCP"      , po::value<std::string>(), "dCP phase ( 0.0, 180.0 , ... )")
      ("dmass2"   , po::value<std::string>(), "use the following delta masses square ( = dm2(Dm23sq),dM2(Dm21sq) )")
      ("angles"   , po::value<std::string>(), "use the following mixing angles ( = theta_1(12),theta_2(13),theta3(23) )")
      ;
    
    po::variables_map vm;
    po::store(po::parse_command_line(iargv, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) 
    {
      std::cout << desc;
      std::cout << "Available steps (studies):" << std::endl;
      std::vector<std::string>::iterator txtItr;
      for( txtItr = avsteps.begin(); txtItr != avsteps.end(); ++txtItr)
        std::cout << (*txtItr) << std::endl;
      return 0;
    }
        
    if (vm.count("input")) 
      input = vm["input"].as<std::string>();
    else {
      std::cout << "No input file selected ... I hope this is Ok. \n";
      no_dataset = true;
    }
    
    if (vm.count("config")) 
      config = vm["config"].as<std::string>();
    else {
      config = std::string("config.xml");
      std::cout << "using the default configuration file config.xml \n";
    }

    if (vm.count("neuosc")) 
      neuosc = vm["neuosc"].as<std::string>();
    else {
      neuosc = std::string("matrix_config.xml");
      std::cout << "using the default configuration file matrix_config.xml \n";
    }

    if (vm.count("steps")) {
      steps = vm["steps"].as<std::string>();
      std::cout << "steps to run over " << steps << std::endl;
    } 
    else {
      std::cout << "using the default step 1  \n";
    }

    if (vm.count("dCP")) {
      dCP = vm["dCP"].as<std::string>();
      std::cout << "dCP value fixed to " << dCP << std::endl;
    } 
    else {
      dCP.append("0");
      std::cout << "using the default value of dCP (as read in xml config) \n";
    }

    if (vm.count("dmass2")) {
      dmass = vm["dmass2"].as<std::string>();
      std::cout << "dm2 and dM2 will be set to " <<  dmass << std::endl;
    } 
    else {
      std::cout << "using dm2 and dM2 read from the configuration file \n";
    }

    if (vm.count("angles")) {
      angles = vm["angles"].as<std::string>();
      std::cout << "mixing will be set to " <<  angles << std::endl;
    } 
    else {
      std::cout << "using the mixing angles as in the configuration file \n";
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
  
  ParameterList parlist; // This configures the integration 
  
  if (parlist.ParseFile( config.c_str() ) == 0) 
    std::cout << "ParameterList> opened file: " << config << '\n';
  else
    return 1;
  
  Parameters *integ_pars = parlist.Next();

  //............................................................................................

  MixingParameterList mixparlist; // This configures mixing angles / masses parameters
  
  if (mixparlist.ParseFile( neuosc.c_str() ) == 0) 
    std::cout << "MixingParameterList> opened file: " << neuosc << '\n';
  else
    return 1;
    
  MixingParameters *mixpars =  mixparlist.GetParameters(0);

  //............................................................................................
  
  std::string dCPtxt ("dCP");
  std::string Option ("Var0");
  
  std::map<std::string,std::string> v_Vars;
  
  v_Vars["0.0014"] = std::string("Var1");
  v_Vars["0.006"]  = std::string("Var2");
  v_Vars["0.0032"] = std::string("Var3");
  
  if( dCP.size() != 0 )
  {
    mixpars->SetPar9( atof( dCP.c_str() ) ); // dCP
    dCPtxt.append(dCP);
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
      Option = v_Vars[dmasses[0]];
    }
    std::cout << (*mixpars) << std::endl;
  }

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
  
  //............................................................................................
  
  //Step Selection
  
  std::map<std::string,bool> execSteps;
  execSteps["0"] = false;
  execSteps["1"] = false;
  execSteps["2"] = false;
  execSteps["3"] = false;
  execSteps["4"] = false;
  execSteps["5"] = false;

  //... Added Oct/2014 - AO

  execSteps["6"] = false;
  execSteps["7"] = false;
  
  if( steps.size() != 0 )
  {
    std::vector<std::string> stepstr;
    boost::split(stepstr, steps, boost::is_any_of(","));
    std::vector<std::string>::iterator itr;
    for( itr = stepstr.begin(); itr != stepstr.end(); ++itr) {
      execSteps[ *itr ] = true;
      std::cout << (*itr) << " turns true" << std::endl;
    }
    
  } else 
    execSteps["1"] = true; 
  
  //............................................................................................
  
  //Setup dataset to run over
  
  std::vector<std::string> dataset;
  std::vector<std::string>::iterator itr;

  if( !no_dataset ) 
  {
    
    std::ifstream * m_in = new std::ifstream( input.c_str(), ifstream::in);
    
    if(!m_in->is_open()) {
      std::cout << "Data> cannot open file" << std::endl;
      return 0;
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
      
  }
  
  //............................................................................................
  
  // Variation 1 ( IceCube! -> R calculation as a function of alpha )
  // 
  // (Start from the output of paper01 which contains the fluxes at the detector for
  //     a specific model )
  //
  
  NeutrinosDetectionPaper * nudet;
    
  nudet = new NeutrinosDetectionPaper( integ_pars );
  
  //Work out first the standard picture
  
  if( execSteps["1"] ) {
    
    //This is R vs the spectral index alpha for the standard picture
    
    Option.append("_");
    Option.append(dCPtxt);

    std::cout << "paper02> MakeVariationStdPicture with option " << Option << std::endl;
        
    nudet->SetMixingParameters ( mixpars );

    nudet->MakeVariationStdPicture("EarthB","Vacuum", Option.c_str(), 2.0, 3.1, 0.05); //
    
  }
  
  // (EarthB,Vacuum) == Model X ---> Vaccuum ---> Earth ---> Detector
  
  //... Loop over the different datasets
  
  if( execSteps["2"] ) {

    TFile * infile;
    std::string model;
    std::string var;

    for( itr = dataset.begin(); itr != dataset.end(); ++itr) 
    {
     
      //... This is R vs the spectral index alpha for different models
 
      std::cout << (*itr) << std::endl;
      
      infile = new TFile( (*itr).c_str(), "READ");
      
      unsigned pos2 = (*itr).rfind("/");
      unsigned pos1 = (*itr).rfind("/", pos2-1);
      
      model = (*itr).substr(pos1+1, (pos2-pos1-1) );
      
      pos2  = (*itr).rfind(".");
      pos1  = (*itr).rfind("Var", pos2-1);

      var   = (*itr).substr(pos1, (pos2-pos1) );
      
      std::cout << "paper02> MakeVariation02 with option " << var << " Model " << model << std::endl;
      
      nudet->SetFluxHistograms(infile, model.c_str(), "EarthB", "Vacuum", var.c_str() );
      
      nudet->MakeVariation02(model.c_str(), "EarthB","Vacuum", var.c_str(), 2.0, 3.1, 0.05); //
      
      nudet->ResetFluxHistograms();
      
      infile->Close();
         
    }
    
  }
  
  if( execSteps["3"] ) {
    
    //This is R vs phi_e fraction - Standard picture

    nudet->MakeVariation03("StdPicture", "EarthB", "Vacuum", 0.0, 1000.0, 1.0); //
  
  }

  if( execSteps["4"] ) {

    // This is R as a function of sin2(theta_13) - for different dCP values - Standard picture
    
    nudet->SetMixingParameters (  mixpars );
    
    nudet->MakeVariation04("StdPicture", "EarthB", "Vacuum", 0.0, 15.0, 1.0, 1.8 ); //
    
    nudet->MakeVariation04("StdPicture", "EarthB", "Vacuum", 0.0, 15.0, 1.0, 2.0 ); //
    
    nudet->MakeVariation04("StdPicture", "EarthB", "Vacuum", 0.0, 15.0, 1.0, 2.2 ); //
    
  }
  
  if( execSteps["5"] ) {
    
    TFile * infile;
    std::string model;
    std::string var;
    
    double *dCP = new double[10];
    dCP[0] = 0.0;
    dCP[1] = 180.0;
    
    int k = 0;
    
    for( itr = dataset.begin(); itr != dataset.end(); ++itr) 
    {
     
      //This is R vs Ev bins - Olga Mena []
 
      std::cout << (*itr) << std::endl;
      
      infile = new TFile( (*itr).c_str(), "READ");
      
      unsigned pos2 = (*itr).rfind("/");
      unsigned pos1 = (*itr).rfind("/", pos2-1);
      
      model = (*itr).substr(pos1+1, (pos2-pos1-1) );

      pos2  = (*itr).rfind(".");
      pos1  = (*itr).rfind("-", pos2-1);

      var   = (*itr).substr(pos1+1, (pos2-pos1-1) );

      nudet->SetFluxHistograms(infile, model.c_str(), "EarthB", "Vacuum", var.c_str() );
      
      nudet->MakeVariation01(model.c_str(), "EarthB", "Vacuum", var.c_str(), dCP[k] ); //

      nudet->ResetFluxHistograms();
      
      infile->Close();

      ++k;
      
    }
    
  }

  //... Added Oct/2014 - AO

  if( execSteps["6"] ) {
    
    //This is R vs phi_e fraction - for different star models
    //Work in progress

  }

  //... Added Oct/2014 - AO

  if( execSteps["7"] ) {
    
    //This is R vs sin(theta13) - for different star models
    //Work in progress
    
  }
  
  //
  
  delete nudet;
  
  //............................................................................................
  
  
  //............................................................................................
  
  return 0;
  
}


