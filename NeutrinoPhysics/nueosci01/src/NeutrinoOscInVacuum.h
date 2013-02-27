// $Id: $
#ifndef NEUTRINOOSCILLATION_H 
#define NEUTRINOOSCILLATION_H 1

// Include files
#include "IProbabilityMatrix.h"

/** @class NeutrinoOscInVacuum NeutrinoOscInVacuum.h
 *
 *  Class for Modelling of neutrino oscillation between source and earth
 *  Ref:
 *  
 *
 *  @author Andres Osorio
 *  @date   2010-10-28
 */

using namespace boost::numeric::ublas;

class NeutrinoOscInVacuum : public IProbabilityMatrix {
public: 
  
  /// Standard constructor
  NeutrinoOscInVacuum( ); 

  NeutrinoOscInVacuum( MixingParameters * ); 
  
  virtual ~NeutrinoOscInVacuum( ); ///< Destructor
  
  void   updateMixingMatrix();
  
  void   calcProbabilities();
  
  void   initializeAngles();

protected:

private:

    
};
#endif // NEUTRINOOSCILLATION_H
