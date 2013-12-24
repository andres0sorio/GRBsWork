#ifndef MUTRACKEVENTS_H
#define MUTRACKEVENTS_H 1

// Include files

#include "RootCommon.h"
#include "Math/GSLIntegrator.h"
#include <Integrals.h>

/** @class MuTrackEvents MuTrackEvents.h 
 *  
 *
 *  @author Andres Osorio
 *  @date   2010-09-12
 */

class MuTrackEvents {
  
public:
  
  MuTrackEvents() {};
  
  MuTrackEvents(const char *, const char *, Parameters *);

  MuTrackEvents(const char *, const char *, const char *, Parameters *);

  virtual ~MuTrackEvents() {};
  
  //............................................
  
  float Evaluate();
  
  float EvaluateNuMuContribution();
  
  float EvaluateNuTauContribution();

  //............................................

  float Evaluate(double);
  
  float EvaluateNuMuContribution(double);
  
  float EvaluateNuTauContribution(double);

  //............................................

  float m_NuMuTracks;
  
  float m_NuTauTracks;
  
private:
  
  Parameters * m_input;
  
  std::string nu_xsec_data;
  std::string antinu_xsec_data;
  std::string pshadow_data;
  
  float m_phi_nu[3];
  
};

#endif // MUTRACKEVENTS
