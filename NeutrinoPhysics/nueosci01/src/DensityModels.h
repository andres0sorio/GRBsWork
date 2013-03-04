// $Id: $
#ifndef DENSITYMODELS_H 
#define DENSITYMODELS_H 1

// Include files
#include <RootCommon.h>
#include <cmath>



/** @class DensityModels DensityModels.h
 *  
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
  
  double m_pars[10];
  static const double Mp;
  static const double GF;
  static const double InveV;
  static const double InveV2;
  static const double TeV;
  
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
  rhoModelA( ) : DensityModels() {}; 
  
  virtual ~rhoModelA( ){}; ///< Destructor
  
  virtual double operator() (double *x, double *p) {
    
    return p[0] * pow( ((p[1]/x[0]) - 1.0), 3.0);
    
  };
  
  
};

class rhoModelB : public DensityModels {
public: 
  
  /// Standard constructor
  rhoModelB( ) : DensityModels() {}; 
  
  virtual ~rhoModelB( ){}; ///< Destructor
  
  virtual double operator() (double *x, double *p) { 
    
    double rmin = pow( 10.0, 10.8 ) * p[3]; //p[3] ->to convert to natural units if needed
    double rmax = 1.0E12 * p[3]; //p[3] ->to convert to natural units if needed
    
    
    if ( x[0] >= rmin && x[0] <= rmax ) 
      return p[0] * pow( (p[1]/x[0]), 2.42857 );
    else 
      return p[0] * pow( (p[1]/p[2]), 2.42857 ) * pow( (x[0] - p[1]), 5.0 ) / pow( (p[2]-p[1]), 5.0);
    
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
      return p[0] * 20.0 * pow( ((p[1]/x[0])-1.0), 2.1 );
    else 
      return p[0] * 1.0  * pow( ((p[1]/x[0])-1.0), 2.5 );
    
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
