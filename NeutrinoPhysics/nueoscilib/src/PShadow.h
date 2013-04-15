// $Id: $
#ifndef PSHADOW_H 
#define PSHADOW_H 1

// Include files
#include <RootCommon.h>
#include "GSLHelpers.h"
#include <Math/IFunction.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TStyle.h>
#include <nuN_interpxsec.h>

/** @class PShadow PShadow.h
 *  
 * \brief Opacity of Earth to very high energy neutrinos, P shadow effectimplementation based on the following article:
 * "High-energy neutrinos in the context of multimessenger astrophysics" Julia K. Becker,
 *  Physics Reports 458 (2008) 173–246
 *
 *  @author Andres Osorio
 *  @date   2011-02-27
 */

//................................................................................

double GetEarthDensity( double x );

//................................................................................

class m_rho_dt : public ROOT::Math::IBaseFunctionOneDim {
  
public:
  
  m_rho_dt() {};
    
  virtual ~m_rho_dt() {};
  
  ROOT::Math::IBaseFunctionOneDim* Clone() const;

  void SetParameters( double, double, double, double);
    
private:
  
  double DoEval(double x) const;

  double m_v;
  double m_y0;
  double m_phi;
  double m_r0;
      
};

//................................................................................

class PShadow {
public: 
  /// Standard constructor
  PShadow( ); 

  PShadow( const std::string & , const std::string & ); 
  
  virtual ~PShadow( ); ///< Destructor
  
  double Tower( double );

  //... Eval - PShadow always == 1.0
  void Eval() 
  {
    m_pshadow_Nu = 1.0;
    m_pshadow_aNu = 1.0;
  };
  
  //... Eval as function of the neutrino energy enu @ angle = Pi
  void Eval(double);
  
  //... Eval as a step function
  void Eval(int, double);
  
  //... Eval as function of neutrino angle and energy 
  void Eval(double, double); 
  
  double timeAtT1(double);
  
  void Validate();
  
  double Nu_PShadow() {return m_pshadow_Nu; };
  double ANu_PShadow() { return m_pshadow_aNu; };
  
 protected:
  
 private:
  
  double m_d;
  double m_r0;
  double m_C;
  double m_NA;
  double m_y0;
  
  double m_pshadow_Nu;
  double m_pshadow_aNu;
  
  TCanvas *c1;
  TCanvas *c2;
  TStyle  *tdrStyle;
  
  TGraph * g1;
  TGraph * g2;
  TGraph * g3;
  TGraph * g4;
  
  bool m_debug;
  
  Interpolation * nu_xsec_interp;
  Interpolation * antinu_xsec_interp;
  
};
#endif // PSHADOW_H
