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

  virtual ~ShowerEvents() {};
  
  float Evaluate();
  
  float EvaluateNCContribution();
  
  float EvaluateCCNueContribution();
  
  float EvaluateCCNutauContribution();
  
protected:

private:

  Parameters * m_input;
  
  std::string nu_xsec_data;
  std::string antinu_xsec_data;

  float m_phi_nu[3];
  
};
#endif // SHOWEREVENTS_H
