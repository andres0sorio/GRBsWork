#ifndef INTEGRALS_H 
#define INTEGRALS_H 1


// Include files
#include "RootCommon.h"
#include "GSLHelpers.h"
#include <Math/IFunction.h>
#include <nuN_interpxsec.h>
#include <DOMParameters.h>
#include <PShadow.h>

/** @class Integrals Integrals.h
 *  
 *
 *  @author Andres Osorio
 *  @date   2010-09-13
 */

double Rmu ( double * x );

double fBLimOne( double * x );

double dFnub( double * x, double * par);

double dsigmady( double * x );

//=============================================================================

class Integrals {
public: 
  
  /// Standard constructor of base class Integrals
  Integrals( ); 
  
  Integrals( Integrals & base ) {
    m_destroy_interp = false;
    m_input = base.m_input;
    nu_xsec_interp = base.nu_xsec_interp;
    antinu_xsec_interp = base.antinu_xsec_interp;
    m_pshadow = base.m_pshadow;
    m_debug = base.m_debug;
  }; 

  virtual ~Integrals( ); ///< Destructor
  
  void SetData( const std::string &, const std::string &);

  void SetData( const std::string &, const std::string &,  const std::string &);
  
  void SetParameters( Parameters * );

  void DestroyInterpolator() { m_destroy_interp = true; } ;
  
  void PrintParameters() 
  {
    std::cout << (*m_input) ;
  }
  
  Parameters    * m_input;
  
  Interpolation * nu_xsec_interp;
  
  Interpolation * antinu_xsec_interp;

  PShadow       * m_pshadow;

  static const double AbsError;
  static const double RelError;
  static const int SubIntervals;
  static const double NMFactor;

 protected:
  
 private:
    
  bool m_debug;
  bool m_destroy_interp;
  
};

//=============================================================================

class m_Numu_integral_dx : public ROOT::Math::IBaseFunctionOneDim, public Integrals {
  
public:
  
  m_Numu_integral_dx(): Integrals() {};
  
  m_Numu_integral_dx( Integrals & base ) : Integrals( base ) {};
  
  virtual ~m_Numu_integral_dx() {};
  
  ROOT::Math::IBaseFunctionOneDim* Clone() const;
  
  double DoEval(double x) const;

private:
    
};

class m_Numu_integral_dxdy : public ROOT::Math::IBaseFunctionOneDim, public Integrals {
  
public:
  
  m_Numu_integral_dxdy() : Integrals() {};
  
  m_Numu_integral_dxdy( Integrals & base ) : Integrals( base ) {};
  
  virtual ~m_Numu_integral_dxdy() {};
  
  ROOT::Math::IBaseFunctionOneDim* Clone() const;
  
  void SetVar1 ( double x1 ) { m_x = x1; };
  
private:
  
  double m_x;
  
  double DoEval(double y) const;
  
};

//=============================================================================

class m_Nutau_integral_dx : public ROOT::Math::IBaseFunctionOneDim, public Integrals {
  
public:
  
  m_Nutau_integral_dx(): Integrals() {};
  
  m_Nutau_integral_dx( Integrals & base ) : Integrals( base ) {};
  
  virtual ~m_Nutau_integral_dx() {};
  
  ROOT::Math::IBaseFunctionOneDim* Clone() const;
  
  double DoEval(double x) const;

private:
    
};

class m_Nutau_integral_dxdy : public ROOT::Math::IBaseFunctionOneDim, public Integrals {
  
public:
  
  m_Nutau_integral_dxdy() : Integrals() {};
  
  m_Nutau_integral_dxdy( Integrals & base ) : Integrals( base ) {};
  
  virtual ~m_Nutau_integral_dxdy() {};
  
  ROOT::Math::IBaseFunctionOneDim* Clone() const;
  
  void SetVar1 ( double x1 ) { m_x = x1; };
  
private:
  
  double m_x;
  
  double DoEval(double y) const;
  
};

class m_Nutau_integral_dxdydz : public ROOT::Math::IBaseFunctionOneDim, public Integrals {
  
public:
  
  m_Nutau_integral_dxdydz() : Integrals() {};
  
  m_Nutau_integral_dxdydz( Integrals & base ) : Integrals( base ) {};
  
  virtual ~m_Nutau_integral_dxdydz() {};
  
  ROOT::Math::IBaseFunctionOneDim* Clone() const;
  
  void SetVar1 ( double x1 ) { m_x = x1; };
  void SetVar2 ( double x2 ) { m_y = x2; };
    
private:
  
  double m_x;
  double m_y;
  
  double DoEval(double z) const;
  
};

//=============================================================================

class m_NC_showers_integral_dx : public ROOT::Math::IBaseFunctionOneDim, public Integrals {
  
public:
  
  m_NC_showers_integral_dx() : Integrals() { 
    
    m_flavour.push_back(std::string("N_e"));
    m_flavour.push_back(std::string("N_mu"));
    m_flavour.push_back(std::string("N_tau"));

  };
  
  m_NC_showers_integral_dx( Integrals & base ) : Integrals( base ) {};
  
  virtual ~m_NC_showers_integral_dx() {};
  
  ROOT::Math::IBaseFunctionOneDim* Clone() const;

  double DoEval(double z) const;
  
  void SetFlavour( int fv ) { m_fv = fv; };
  
private:

  int m_fv;
    
  std::vector<std::string> m_flavour;
    

};

class m_antiNC_showers_integral_dx : public ROOT::Math::IBaseFunctionOneDim, public Integrals {
  
public:
  
  m_antiNC_showers_integral_dx() : Integrals() {
    m_flavour.push_back(std::string("N_ae"));
    m_flavour.push_back(std::string("N_amu"));
    m_flavour.push_back(std::string("N_atau"));
  };
  
  m_antiNC_showers_integral_dx( Integrals & base ) : Integrals( base ) {};
  
  virtual ~m_antiNC_showers_integral_dx() {};
  
  ROOT::Math::IBaseFunctionOneDim* Clone() const;

  double DoEval(double z) const;

  void SetFlavour( int fv ) { m_fv = fv; };

private:
  
  int m_fv;

  std::vector<std::string> m_flavour;
  
};

//=============================================================================

class m_CCnue_showers_integral_dx : public ROOT::Math::IBaseFunctionOneDim, public Integrals {
  
public:
  
  m_CCnue_showers_integral_dx() : Integrals() {};
  
  m_CCnue_showers_integral_dx( Integrals & base ) : Integrals( base ) {};
  
  virtual ~m_CCnue_showers_integral_dx() {};
  
  ROOT::Math::IBaseFunctionOneDim* Clone() const;
  
  double DoEval(double z) const;

private:
  
  
  
};

class m_CCantinue_showers_integral_dx : public ROOT::Math::IBaseFunctionOneDim, public Integrals {
  
public:
  
  m_CCantinue_showers_integral_dx() : Integrals() {};
  
  m_CCantinue_showers_integral_dx( Integrals & base ) : Integrals( base ) {};
  
  virtual ~m_CCantinue_showers_integral_dx() {};
  
  ROOT::Math::IBaseFunctionOneDim* Clone() const;
  
  double DoEval(double z) const;

private:
  
  
  
};

//=============================================================================

class m_CCnutau_showers_integral_dx : public ROOT::Math::IBaseFunctionOneDim, public Integrals {
  
public:
  
  m_CCnutau_showers_integral_dx() : Integrals() {};
  
  m_CCnutau_showers_integral_dx( Integrals & base ) : Integrals( base ) {};
  
  virtual ~m_CCnutau_showers_integral_dx() {};
  
  ROOT::Math::IBaseFunctionOneDim* Clone() const;
  
  double DoEval(double z) const;
  
private:
  
  
  
};

class m_CCantinutau_showers_integral_dx : public ROOT::Math::IBaseFunctionOneDim, public Integrals {
  
public:
  
  m_CCantinutau_showers_integral_dx() : Integrals() {};
  
  m_CCantinutau_showers_integral_dx( Integrals & base ) : Integrals( base ) {};
  
  virtual ~m_CCantinutau_showers_integral_dx() {};
  
  ROOT::Math::IBaseFunctionOneDim* Clone() const;
  
  double DoEval(double z) const;
  
private:
  
  
  
};


//=============================================================================

#endif // INTEGRALS_H
