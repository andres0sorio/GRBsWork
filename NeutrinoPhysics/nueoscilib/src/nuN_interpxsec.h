#ifndef NUN_XSEC_H 
#define NUN_XSEC_H 1

// Include files

#include "RootCommon.h"
#include "GSLHelpers.h"
#include <Math/Interpolator.h>

#include <iterator>

class Interpolation {

 public:
  
  Interpolation();
  Interpolation(const char *);
  virtual ~Interpolation();
  
  double evaluateCC( double );
  double evaluateNC( double );

  void setLogY();
 
  std::vector<double > m_xx;
  std::vector<double > m_cc;
  std::vector<double > m_nc;
  
  void showPoints();

 private:
  
  std::ifstream * m_in;
  
  ROOT::Math::Interpolator * m_interpolatorCC;
  ROOT::Math::Interpolator * m_interpolatorNC;

  bool m_logX;
  bool m_logY;
  double e_nu;
  float cc_xsec;
  float nc_xsec ;
  float tot_xsec;

  unsigned int m_ndata;

};

#endif // NUN_XSEC_H
