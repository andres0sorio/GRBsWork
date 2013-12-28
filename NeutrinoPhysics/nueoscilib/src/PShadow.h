// $Id: $
#ifndef PSHADOW_H 
#define PSHADOW_H 1

// Include files
#include <RootCommon.h>
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

  //AO dec 2013
  PShadow( const std::string & ); 
  
  PShadow( const std::string & , const std::string & ); 
  
  virtual ~PShadow( ); ///< Destructor
  
  double Tower( double );

  //... Eval - PShadow always == 1.0
  void Eval() 
  {
    m_pshadow_Nu = 1.0;
    m_pshadow_aNu = 1.0;
  };
  
  //... Evaluate as function of the neutrino energy enu @ angle = Pi
  void Eval(double);

  void Eval2(double);
  
  //... Evaluate  as function of neutrino angle and energy 
  void Eval(double, double); 

  //... Evaluate  as a step function 
  void Eval2(double, double);

  double timeAtT1(double);
  
  void Validate();
  
  double Nu_PShadow() { if( m_pshadow_Nu < 0 ) return 0.0; else return m_pshadow_Nu; };
  double ANu_PShadow() { if( m_pshadow_aNu < 0 ) return 0.0; else return m_pshadow_aNu; };
  
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

  //AO dec 2013

  std::ifstream * m_in;
  std::vector<double> m_xx;
  std::vector<double> m_nu;
  std::vector<double> m_anu;
  
  ROOT::Math::Interpolator * m_interpolatorNu;
  ROOT::Math::Interpolator * m_interpolatorANu;

  bool m_full_calculation;
  
};
#endif // PSHADOW_H
