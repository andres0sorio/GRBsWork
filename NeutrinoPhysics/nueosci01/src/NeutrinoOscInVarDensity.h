// $Id: $
#ifndef NEUTRINOOSCINVARDENSITY_H 
#define NEUTRINOOSCINVARDENSITY_H 1

// Include files
#include <RootCommon.h>
#include "IProbabilityMatrix.h"
#include "uBlasExt.h"
#include "TF1.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TStyle.h"

#include <boost/math/special_functions/fpclassify.hpp>

/** @class NeutrinoOscInVarDensity NeutrinoOscInVarDensity.h
 *  
 * This class implements the neutrino oscillation in varying density matter
 * Ref: Neutrino Oscillation with three flavours in matter of varying density - Ohlsson, Snellman
 *      Three Flavor neutrino oscillation in matter, Ohlsson - Snellman
 *
 *  @author Andres Osorio
 *  @date   2011-07-28
 */

using namespace boost::numeric::ublas;

double densityModA(double *x, double *par);

class NeutrinoOscInVarDensity : public IProbabilityMatrix {
public: 
  
  /// Standard constructor
  NeutrinoOscInVarDensity( ) {}; 

  NeutrinoOscInVarDensity( MixingParameters * );
  
  virtual ~NeutrinoOscInVarDensity( ); ///< Destructor

  void   calcProbabilities();
  
  void   initializeAngles();
  void   updateEab();
  void   updateCoefficients();
  void   updateLambdas();
  void   updateMixingMatrix();

  void   Eval_TnuT(  long double );
  void   Eval_TnuT(  long double , long double );
  void   Eval_UFlavour( );
  void   Eval_UFlavour( long double );
  void   Eval_UFlavour( long double , long double );
  
  void   Validate( );
  void   ValidateInVarDensity( );
  void   ValidateSolarProfile( );
  void   TestProcedure( );
  
  void   setPotential( TF1 * f1) { f_Ve = f1; } ;
  
  bool m_debug;
  bool m_validation;

  long double m_Ev;
  long double m_DeltamSq; // Delta m21^2
  long double m_DeltaMSq; // Delta m32^2
  
  long double m_c0;
  long double m_c1;
  long double m_sign;
  long double m_c0_0;

  std::complex<  long double > m_phi;
  
  matrix<  long double > * v_Lambda; // Eigenvalues lambda 
  
  matrix<  long double > * m_Eab; // Energy differences
  
  matrix<  long double > * m_Tab; // T == Hm - (tr Hm)I3/
  
  matrix<  long double > * m_Ur; // Re( U_CKM )
  
  matrix<  long double > * m_invUr; // Re( U_CKM )^-1
  
  matrix<  long double > * m_UTU; // (T~)
  
  matrix<  long double > * m_UTUSq; // (T~)^2
  
  matrix< std::complex< long double> > * m_Uf; // 
  
  matrix< std::complex< long double> > * m_Ufd; // 
  
  TF1 * f_Ve;
  
  TStyle * tdrStyle;

protected:
  
private:
  

  
};
#endif // NEUTRINOOSCINVARDENSITY_H
