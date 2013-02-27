// $Id: $
#ifndef INTEGRALSPLOTS_H 
#define INTEGRALSPLOTS_H 1

// Include files
#include <Integrals.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TStyle.h>

/** @class IntegralsPlots IntegralsPlots.h
 *  
 *
 *  @author Andres Osorio
 *  @date   2011-06-30
 */
class IntegralsPlots {
public: 
  /// Standard constructor
  IntegralsPlots( const char *, const char *, Parameters * ); 
  
  virtual ~IntegralsPlots( ); ///< Destructor
  
  void IntegralOne();
  void IntegralTwo();
  void IntegralThree();
  void IntegralFour();
  void IntegralFive();

  void ConfigMethodOne( int, double, double );
  void ConfigMethodTwo( int, double );
  
  void   EvalIntegralMethodOne( );
  double EvalIntegralMethodOne( double );
  double EvalIntegralMethodOne( ROOT::Math::IGenFunction *, double );
  
  void   EvalIntegralMethodTwo( );
  double EvalIntegralMethodTwo( double );
  double EvalIntegralMethodTwo( ROOT::Math::IGenFunction *, double );
  
  ROOT::Math::IGenFunction * GetIntegral ( int );

protected:
  
private:
  
  Parameters * m_input;
  
  std::string nu_xsec_data;
  std::string antinu_xsec_data;
  
  float m_phi_nu[3];

  TStyle * tdrStyle;
  
  TCanvas * canvasOne;
  TCanvas * canvasTwo;
  TCanvas * canvasThree;
  TCanvas * canvasFour;
  TCanvas * canvasFive;
  TCanvas * methodOne;
  TCanvas * methodTwo;

  int SubIntervals;
  double Epsilon;
  double AbsError;
  double RelError;

  double m_x0;
  double m_sfactor;
    
};
#endif // INTEGRALSPLOTS_H
