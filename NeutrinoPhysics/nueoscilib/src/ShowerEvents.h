#ifndef SHOWEREVENTS_H 
#define SHOWEREVENTS_H 1

// Include files

#include "RootCommon.h"
#include "Math/GSLIntegrator.h"
#include <Integrals.h>

/** @class ShowerEvents ShowerEvents.h
 *  
 *
 *  @author Andres Osorio
 *  @date   2010-09-15
 */

class ShowerEvents {

public: 
  
  ShowerEvents() {};
  
  ShowerEvents(const char *, const char *, Parameters *);

  ShowerEvents(const char *, const char *, const char *, Parameters *);

  virtual ~ShowerEvents() {};
  
  //............................................

  float Evaluate();
  
  float EvaluateNCContribution();
  
  float EvaluateCCNueContribution();
  
  float EvaluateCCNutauContribution();

  //............................................

  float Evaluate(double );
  
  float EvaluateNCContribution(double );
  
  float EvaluateCCNueContribution(double );
  
  float EvaluateCCNutauContribution(double );
  
  //............................................

  double m_NCShower;

  double m_CCNuShower;

  double m_CCNutauShower;
  
protected:

private:

  Parameters * m_input;
  
  std::string nu_xsec_data;
  std::string antinu_xsec_data;
  std::string pshadow_data;
  
  float m_phi_nu[3];
  float m_phi_anu[3];
    
};
#endif // SHOWEREVENTS_H
