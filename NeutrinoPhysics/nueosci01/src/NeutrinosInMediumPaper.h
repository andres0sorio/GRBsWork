// $Id: $
#ifndef NEUTRINOSINMEDIUMPAPER_H 
#define NEUTRINOSINMEDIUMPAPER_H 1

// Include files
#include <Graphics.h>
#include <NeutrinoOscInVarDensity.h>

/** @class NeutrinosInMediumPaper NeutrinosInMediumPaper.h
 *  
 *
 *  @author Andres Osorio
 *  @date   2013-02-27
 */

class NeutrinosInMediumPaper : public Graphics {
public: 
  /// Standard constructor
  NeutrinosInMediumPaper( ) { }; 

  NeutrinosInMediumPaper( MixingParameters * ); 
  
  virtual ~NeutrinosInMediumPaper( ); ///< Destructor
  
  NeutrinoOscInVarDensity * m_modelA;
  
  void MakePlots();
  
protected:

private:

  bool m_debug;
  

};
#endif // NEUTRINOSINMEDIUMPAPER_H
