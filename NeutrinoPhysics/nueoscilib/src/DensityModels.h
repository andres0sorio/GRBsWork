// $Id: $
#ifndef DENSITYMODELS_H 
#define DENSITYMODELS_H 1

// Include files
#include <RootCommon.h>
#include <cmath>

/** @class DensityModels DensityModels.h
 *  
 *  \brief Density models description file
 *  This file contains all the density models --  derives from DensityModels class
 *  
 *  Ref: 
 *
 *  [1] Olga Mena Phys.Rev.D75:063003,2007 : Models rhoA, rhoB, rhoC
 *  [2] Julia Becker Phys. Reports 458 2008 : 
 *  [3] T. Ohlsson, Eur. Phys. J. C 20, 507–515 2001 :
 *
 *  @author Andres Osorio
 *  @date   2011-09-25
 */

class DensityModels {
public: 
  
  /// Standard constructor
  DensityModels( ){}; 
  
  virtual ~DensityModels( ){}; ///< Destructor

  virtual double operator() (double *x, double *p) = 0;

  void treat_as_AntiNu() { m_sign = -1.0; };

  void treat_as_Nu() { m_sign = 1.0; };

  double m_pars[10];
  static const double Mp;
  static const double GF;
  static const double InveV;
  static const double InveV2;
  static const double TeV;
  
  double m_sign; // m_sign: used for selecting nu vs anti-nu from construction

};

class rhoAtResonance : public DensityModels {
public: 
  
  /// Standard constructor
  rhoAtResonance( ): DensityModels(){
    
    m_pars[0] = Mp;
    m_pars[1] = 2.0 * sqrt(2.0) * GF * 7.7E-15;
    m_pars[2] = 4.0 * M_PI;
    
    m_k0 = ( m_pars[0] * m_pars[2] * 1.0E18 * InveV ) / m_pars[1] ;
    
  } ; 
  
  virtual ~rhoAtResonance( ){}; ///< Destructor
  
  virtual double operator() (double *x, double *p) {
    
    m_cot = 1.0 / tan( 2.0 * p[0] * M_PI / 180.0 );
    
    return  m_k0 * m_cot * (1.0 / x[0]);
    
  };
  
  double m_k0;
  double m_cot;
  
};  

class rhoModelA : public DensityModels {
public: 
  
  /// Standard constructor
  rhoModelA( ) : DensityModels() { m_sign = 1.0; };
  
  virtual ~rhoModelA( ){}; ///< Destructor
  
  virtual double operator() (double *x, double *p) {

    //std::cout << " * " << m_sign << std::endl;
    return m_sign * p[0] * pow( ((p[1]/x[0]) - 1.0), 3.0);
    
  };
    
};

class rhoModelB : public DensityModels {
public: 
  
  /// Standard constructor
  rhoModelB( ) : DensityModels() { m_sign = 1.0; }; 
    
  virtual ~rhoModelB( ){}; ///< Destructor
  
  virtual double operator() (double *x, double *p) { 
    
    //p[4] -> use to convert to natural units when needed
    
    double Rstar = p[1] * p[4]; 
    double Rb    = p[2] * p[4];
    double Rmin  = p[3];
    
    double result = 0.0;
    
    if ( x[0] >= Rmin && x[0] < Rb ) 
    
      result = p[0] * pow( ( Rstar / x[0]), 2.42857 );
    
    else if ( x[0] >= Rb )
      
      result = p[0] * pow( (Rstar / Rb), 2.42857 ) * pow( (x[0] - Rstar) / (Rb - Rstar) , 5.0 );
    
    else 
      result = -1.123456;
    
    return m_sign * result;

  };
  
};

class rhoModelC : public DensityModels {
public: 
  
  /// Standard constructor
  rhoModelC( ) : DensityModels() { m_sign = 1.0; }; 
    
  virtual ~rhoModelC( ){}; ///< Destructor
    
  virtual double operator() (double *x, double *p) {

    //p[8] -> use to convert to natural units when needed

    double Rstar  = p[1] * p[8];
    double Rb     = p[2] * p[8];
    double Rmin   = p[3];
    double A1     = p[4];
    double neff1  = p[5];
    double A2     = p[6];
    double neff2  = p[7];

    double result = 0.0;
    
    if ( x[0] >= Rmin && x[0] < Rb ) 
      
      result = p[0] * A1 * pow( (( Rstar/x[0] ) - 1.0), neff1 );
    
    else if ( x[0] >= Rb )
      
      result = p[0] * A2 * pow( (( Rstar/x[0] ) - 1.0), neff2 );
    
    else
      result =  -1.123456;

    return m_sign * result;
    
  };
  
};

class rhoEarthA : public DensityModels {
public: 

  //Ref [3]
  
  /// Standard constructor
  rhoEarthA( ) : DensityModels() { m_sign = 1.0; }; 
  
  virtual ~rhoEarthA( ){}; ///< Destructor
  
  virtual double operator() (double *x, double *p) {
    
    double xx = x[0];
    
    double L1 = p[2] * p[4];
    double L2 = L1 + ( p[3] * p[4] );
    double A1 = p[0];
    double A2 = p[1];
    
    double result = 0.0;
    
    if ( xx <= L1 ) 
      result = A1;
    else if ( xx > L1 && xx <= L2 )
      result = A2;
    else if ( xx >= L2 ) 
      result = A1;
    else result = -1.1111;
    
    return m_sign * result;
    
  };
  
};

class rhoEarthB : public DensityModels {
public: 

  //Ref [2]
  
  /// Standard constructor
  rhoEarthB( ) : DensityModels() { m_sign = 1.0; }; 
  
  virtual ~rhoEarthB( ){}; ///< Destructor
  
  virtual double operator() (double *x, double *p) {

    double value = 0.0;
    double LMAX  = p[0];
    double DX    = LMAX/2.0; 
    double x0    = abs( ( x[0]-DX) ) / DX;
    double xx    = pow( x0 , 2.0 );
    double xxx   = pow( x0 , 3.0 );

    if( x0 < 0.192 )
      value = 13.0885 - 8.8381*x0*x0;
    else if( x0 > 0.192 && x0 < 0.546 )
      value = 12.5815 - 1.2638 * x0 - 3.6426 * xx - 5.5281 * xxx;
    else if( x0 > 0.546 && x0 < 0.895 ) 
      value = 7.9565 - 6.4761 * x0 + 5.5283 * xx - 3.0807 * xxx;
    else if( x0 > 0.895 && x0 < 0.906) 
      value = 5.3197 - 1.4836 * x0;
    else if( x0 > 0.906 && x0 < 0.937) 
      value = 11.2494 - 8.0298 * x0;
    else if( x0 > 0.937 && x0 < 0.965) 
      value = 7.1089 - 3.8045 * x0;
    else if( x0 > 0.965 && x0 < 0.996) 
      value = 2.691 + 0.6924 * x0;
    else if( x0 > 0.996 && x0 < 0.998) 
      value = 2.9;
    else if( x0 > 0.998 && x0 < 0.999) 
      value = 2.6;
    else if ( x0 > 0.999 && x0 <= 1.0) 
      value = 1.02;
    else value = -1.1111;
    
    return m_sign * value * p[1] * p[2];
    
  };
  
};

class zeroPotencial : public DensityModels {
public: 
  
  /// Standard constructor
  zeroPotencial( ) : DensityModels() { m_sign = 1.0; }; 
  
  virtual ~zeroPotencial( ){}; ///< Destructor
  
  virtual double operator() (double *x, double *p) {

    return 0.0L;
        
  };
  
};

class linearPotencial : public DensityModels {
public: 
  
  /// Standard constructor
  linearPotencial( ) : DensityModels() { m_sign = 1.0; }; 
  
  virtual ~linearPotencial( ){}; ///< Destructor
  
  virtual double operator() (double *x, double *p) {

    double p0 = p[0];
    double p1 = p[1];
    double xx = x[0];
    
    return m_sign * ( p0 + p1 * xx );
    
  };
  
};

class stepPotencial : public DensityModels {
public: 
  
  /// Standard constructor
  stepPotencial( ) : DensityModels() { m_sign = 1.0; }; 
  
  virtual ~stepPotencial( ){}; ///< Destructor
  
  virtual double operator() (double *x, double *p) {

    double xx = x[0]; // distance
    double L1 = 2885.E3 * DensityModels::InveV;
      
    double A1 = p[0];
    double A2 = p[1];
    
    if ( xx <= L1 ) return m_sign * A1;
    else return m_sign * A2;
    
    return 0.0;
    
  };
  
};

class solarPotencial : public DensityModels {
public: 
  
  /// Standard constructor
  solarPotencial( ) : DensityModels() { m_sign = 1.0; }; 
  
  virtual ~solarPotencial( ){}; ///< Destructor
  
  virtual double operator() (double *x, double *p) {
    
    double p0 = p[0]; // A0
    double p1 = p[1] * DensityModels::InveV; //r0 1/eV units
    double xx = x[0];
    
    return m_sign* ( p0*TMath::Exp(- xx / p1) );
    
  };
    
};

//....................................................................................

class nvEnergy {
public: 
  
  /// Standard constructor
  nvEnergy( ) {
    m_k0 = ( 1.0 / (4.0 * M_PI * DensityModels::InveV) );
  };
  
  virtual ~nvEnergy( ){}; ///< Destructor
  
  virtual double operator() (double *x, double *p) {
    
    return  m_k0 * sin( 2.0 * p[1] * M_PI / 180.0) * p[0] * p[2] * x[0];
    
  };
  
  double m_k0;
    
};  

class lR2d {
public: 
  
  /// Standard constructor
  lR2d( ) {
    m_k0 = 4.0 * M_PI * DensityModels::InveV * 1E12;
  };
  
  virtual ~lR2d( ){}; ///< Destructor
  
  virtual double operator() (double *x, double *p) {
    return  m_k0 * (x[1]/x[0]) * ( 1.0 / sin( 2.0 * p[0] * M_PI / 180.0 ));
  };
  
  double m_k0;
  
};  

//....................................................................................


#endif // DENSITYMODELS_H
