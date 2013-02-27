// $Id: GSLHelpers.h,v 1.2 2007/02/18 23:53:01 aosorio Exp $
#ifndef GSLHELPERS_H 
#define GSLHELPERS_H 1

// Include files
#include <Math/Integrator.h>
#include <Math/IntegrationTypes.h>
#include "Math/SpecFuncMathCore.h"
#include "Math/SpecFuncMathMore.h"
#include "Math/WrappedFunction.h"
#include "Math/RootFinder.h"
#include "Math/RootFinderAlgorithms.h"

///////////////////////////////////////////

// ... Integration parameters

class IntParams {

 public:
  
  double varx;
  double Q2;

  IntParams() { varx = 1.0; Q2 = 1.0; };
  ~IntParams() {};

};

#endif // GSLHELPERS_Hh
