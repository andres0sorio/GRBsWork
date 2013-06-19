// $Id: $
#ifndef GRAPHICS_H 
#define GRAPHICS_H 1

// Include files

#include "RootCommon.h"
#include "TGraph.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TLegend.h"
#include "TColor.h"

/** @class Graphics Graphics.h
 *  
 *
 *  @author Andres Osorio
 *  @date   2010-09-19
 */

class Graphics {
public: 
  /// Standard constructor
  Graphics( );
  
  virtual ~Graphics( ); ///< Destructor

  Int_t txtci;
  
  void SetOptions();
  
  std::map<std::string, TH1D *>    h_1dHistos;
  
  std::map<std::string, TCanvas *> h_Canvas;
  
  std::map<std::string, TGraph  *> h_Graphs;

protected:
  
private:

};
#endif // GRAPHICS_H
