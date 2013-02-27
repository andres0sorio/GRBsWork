// $Id: $
#ifndef NEUTRINOOSCINMATTER_H 
#define NEUTRINOOSCINMATTER_H 1

// Include files
#include "IProbabilityMatrix.h"

/** @class NeutrinoOscInMatter NeutrinoOscInMatter.h
 *
 *  Class for Modelling of neutrino oscillation in matter with Effective Potential
 *  Ref: 
 *
 *
 *  @author Andres Osorio
 *  @date   2011-01-09
 */

using namespace boost::numeric::ublas;

class NeutrinoOscInMatter : public IProbabilityMatrix {
public: 

  /// Standard constructor
  NeutrinoOscInMatter( ); 

  NeutrinoOscInMatter( MixingParameters * ); 

  virtual ~NeutrinoOscInMatter( ); ///< Destructor

  void   calcProbabilities();

  void   initializeAngles();

  void   updateMixingMatrix();
  
protected:

private:

  double m_Dm23sq;
  double m_L;
  double m_Enu;
  double m_Ve;
  double m_L_m;
  
  matrix< long double > * m_Qm; // mixing angles matrix in matter

  long double sin2Q13m();
  
  long double deltaMu21();
  long double deltaMu32();
  long double deltaMu31();
  
  long double cosTh_m(int, int);
  long double sinTh_m(int, int);
  
  long double Sij( int, int );
  
  
};
#endif // NEUTRINOOSCINMATTER_H
