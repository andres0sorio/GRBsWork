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
  
  TGraph * g1;
  TGraph * g2;
  TGraph * g3;
  TGraph * g4;
  TGraph * g5;
  TGraph * g6;
  
  TGraph * g7;
  TGraph * g8;
  TGraph * g9;
  TGraph * g10;
  TGraph * g11;
  TGraph * g12;
  
  TLegend * tleg_1;
  
  TLegend * tleg_2;
  
  Int_t txtci;
  
  void SetOptions();
  
  std::map<std::string, TH1D *> h_paper01;
  
  std::map<std::string, TCanvas *> h_canvas;
  
  std::map<std::string, TGraph  *> h_paper01Graphs;

    
protected:
  
private:

};
#endif // GRAPHICS_H
