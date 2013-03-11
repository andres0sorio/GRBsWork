// $Id: $
#ifndef DENSITYMODELS_H 
#define DENSITYMODELS_H 1

// Include files
#include <RootCommon.h>
#include <cmath>
#include "IProbabilityMatrix.h"


/** @class DensityModels DensityModels.h
 *  
 *
 *  @author Andres Osorio
 *  @date   2011-09-25
 */

double mcProf(double *x, double *par);
double stepProf(double *x, double *par);
double densityModA(double *x, double *par);

class DensityModels {
public: 
  
  /// Standard constructor
  DensityModels( ){}; 
  
  virtual ~DensityModels( ){}; ///< Destructor

  virtual double operator() (double *x, double *p) = 0;

  void treat_as_AntiNu() { m_sign = -1.0; };
  
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
    
    double rmin = pow( 10.0, 10.8 ) * p[3]; //p[3] ->to convert to natural units if needed
    double rmax = 1.0E12 * p[3]; //p[3] ->to convert to natural units if needed
    
    
    if ( x[0] >= rmin && x[0] <= rmax ) 
      return m_sign * p[0] * pow( (p[1]/x[0]), 2.42857 );
    else 
      return m_sign * p[0] * pow( (p[1]/p[2]), 2.42857 ) * pow( (x[0] - p[1]), 5.0 ) / pow( (p[2]-p[1]), 5.0);
    
  };
  
};

class rhoModelC : public DensityModels {
public: 
  
  /// Standard constructor
  rhoModelC( ) : DensityModels() {}; 
  
  virtual ~rhoModelC( ){}; ///< Destructor
  
  virtual double operator() (double *x, double *p) {

    double rmin = pow( 10.0, 10.8 ) * p[2]; //p[2] ->to convert to natural units if needed
    double rmax = 1.0E11 * p[2]; //p[2] ->to convert to natural units if needed
    
    if ( x[0] >= rmin && x[0] <= rmax ) 
      return m_sign * p[0] * 20.0 * pow( ((p[1]/x[0])-1.0), 2.1 );
    else 
      return m_sign * p[0] * 1.0  * pow( ((p[1]/x[0])-1.0), 2.5 );
    
  };
  
};

class rhoEarthA : public DensityModels {
public: 
  
  /// Standard constructor
  rhoEarthA( ) : DensityModels() { m_sign = 1.0; }; 
  
  virtual ~rhoEarthA( ){}; ///< Destructor
  
  virtual double operator() (double *x, double *p) {
    
    double xx = x[0]; // distance
    double L1 = p[2] * p[4];
    double L2 = L1 + ( p[3] * p[4] );
    
    double result = 0.0;
    
    if ( xx <= L1 ) 
      result = p[0];
    else if ( xx > L1 && xx < L2 ) 
      result = p[1];
    else 
      result = p[0];
    
    return m_sign * result;
    
  };
  
};

class rhoEarthB : public DensityModels {
public: 
  
  /// Standard constructor
  rhoEarthB( ) : DensityModels() { m_sign = 1.0; }; 
  
  virtual ~rhoEarthB( ){}; ///< Destructor
  
  virtual double operator() (double *x, double *p) {

    // rho [g/cm^3]: Ref Physics Reports 458 (2008) 173-246
    
    double value = 0.0;

    double xx = pow(x[0],2.0);
    double xxx = pow(x[0],3.0);
        
    if( x[0] < 0.192 )
      value = 13.0885 - 8.8381*x[0];
    else if( x[0] > 0.192 && x[0] < 0.546 )
      value = 12.5815 - 1.2638 * x[0] - 3.6426 * xx - 5.5281 * xxx;
    else if( x[0] > 0.546 && x[0] < 0.895 ) 
      value = 7.9565 - 6.4761 * x[0] + 5.5283 * xx - 3.0807 * xxx;
    else if( x[0] > 0.895 && x[0] < 0.906) 
      value = 5.3197 - 1.4836 * x[0];
    else if( x[0] > 0.906 && x[0] < 0.937) 
      value = 11.2494 - 8.0298 * x[0];
    else if( x[0] > 0.937 && x[0] < 0.965) 
      value = 7.1089 - 3.8045 * x[0];
    else if( x[0] > 0.965 && x[0] < 0.996) 
      value = 2.691 + 0.6924 * x[0];
    else if( x[0] > 0.996 && x[0] < 0.998) 
      value = 2.9;
    else if( x[0] > 0.998 && x[0] < 0.999) 
      value = 2.6;
    else if ( x[0] > 0.999 && x[0] <= 1.0) 
      value = 1.02;
    else value = -1.1111;
    
    return m_sign * value;
        
  };
  
};

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




#endif // DENSITYMODELS_H
