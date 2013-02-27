// $Id: $
// Include files 



// local
#include "Graphics.h"

//-----------------------------------------------------------------------------
// Implementation file for class : Graphics
//
// 2010-09-19 : Andres Osorio
//-----------------------------------------------------------------------------

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
Graphics::Graphics(  ) {

  g1 = new TGraph();
  g2 = new TGraph();
  g3 = new TGraph();
  g4 = new TGraph();
  g5 = new TGraph();
  g6 = new TGraph();
  
  g7 = new TGraph();
  g8 = new TGraph();
  g9 = new TGraph();
  g10 = new TGraph();
  g11 = new TGraph();
  g12 = new TGraph();
    
  
  tleg_1 = new TLegend(0.51, 0.80, 0.85, 0.89, NULL, "brNDC");

  tleg_2 = new TLegend(0.51, 0.80, 0.85, 0.89, NULL, "brNDC");

  txtci = TColor::GetColor("#333333");
    
}

//=============================================================================
// Destructor
//=============================================================================

Graphics::~Graphics() {} 

//=============================================================================
void Graphics::SetOptions() {

  TH1F * h1 = g1->GetHistogram();
  h1->SetXTitle("#nu_e fraction");
  g1->GetXaxis()->SetLabelSize(0.03);
  g1->GetXaxis()->SetTitleFont(22);
  
  h1->SetYTitle("R=Tracks/Showers");
  g1->GetYaxis()->SetLabelSize(0.03);
  g1->GetYaxis()->SetTitleOffset(1.09);
  g1->GetYaxis()->SetTitleFont(22);

  h1->SetTitle("");
  
  h1 = g2->GetHistogram();
  h1->SetXTitle("#nu_e fraction");
  g2->GetXaxis()->SetLabelSize(0.03);
  g2->GetXaxis()->SetTitleFont(22);
   
  h1->SetYTitle("R=Tracks/Showers");
  g2->GetYaxis()->SetLabelSize(0.03);
  g2->GetYaxis()->SetTitleOffset(1.09);
  g2->GetYaxis()->SetTitleFont(22);

  h1->SetTitle("");
  
  g1->SetLineColor(1);
  g1->SetLineStyle(1);
  g1->SetLineWidth(1);
  
  g2->SetLineColor(2);
  g2->SetLineStyle(2);
  g2->SetLineWidth(1);
  
  /////////////////////////////////////////////////////////

  h1 = g3->GetHistogram();
  h1->SetXTitle("Spectral index (#alpha)");
  g3->GetXaxis()->SetLabelSize(0.03);
  g3->GetXaxis()->SetTitleFont(22);
  
  h1->SetYTitle("R=Tracks/Showers");
  g3->GetYaxis()->SetLabelSize(0.03);
  g3->GetYaxis()->SetTitleOffset(1.09);
  g3->GetYaxis()->SetTitleFont(22);
  h1->SetTitle("");

  h1 = g4->GetHistogram();
  h1->SetXTitle("Spectral index (#alpha)");
  g4->GetXaxis()->SetLabelSize(0.03);
  g4->GetXaxis()->SetTitleFont(22);
  
  h1->SetYTitle("R=Tracks/Showers");
  g4->GetYaxis()->SetLabelSize(0.03);
  g4->GetYaxis()->SetTitleOffset(1.09);
  g4->GetYaxis()->SetTitleFont(22);
  h1->SetTitle("");

  g3->SetLineColor(1);
  g3->SetLineStyle(1);
  g3->SetLineWidth(1);
  
  g4->SetLineColor(2);
  g4->SetLineStyle(2);
  g4->SetLineWidth(1);

  /////////////////////////////////////////////////////////

  h1 = g5->GetHistogram();
  h1->SetXTitle("Sin^{2}(#theta_{13})");
  g5->GetXaxis()->SetLabelSize(0.03);
  g5->GetXaxis()->SetTitleFont(22);
  
  h1->SetYTitle("R=Tracks/Showers");
  g5->GetYaxis()->SetLabelSize(0.03);
  g5->GetYaxis()->SetTitleOffset(1.09);
  g5->GetYaxis()->SetTitleFont(22);
  h1->SetTitle("");

  h1 = g6->GetHistogram();
  h1->SetXTitle("Sin^{2}(#theta_{13})");
  g6->GetXaxis()->SetLabelSize(0.03);
  g6->GetXaxis()->SetTitleFont(22);
  
  h1->SetYTitle("R=Tracks/Showers");
  g6->GetYaxis()->SetLabelSize(0.03);
  g6->GetYaxis()->SetTitleOffset(1.09);
  g6->GetYaxis()->SetTitleFont(22);
  h1->SetTitle("");

  g5->SetLineColor(1);
  g5->SetLineStyle(1);
  g5->SetLineWidth(1);
  g5->SetFillColor(10);
  
  g6->SetLineColor(2);
  g6->SetLineStyle(1);
  g6->SetLineWidth(1);
  g6->SetFillColor(10);

  tleg_1->SetBorderSize(0);
  tleg_1->SetTextFont(22);
  tleg_1->SetTextSize(0.04);
  tleg_1->SetLineColor(1);
  tleg_1->SetLineStyle(1);
  tleg_1->SetLineWidth(1);
  tleg_1->SetFillColor(0);
  tleg_1->SetFillStyle(1001);
  tleg_1->SetTextColor(txtci);
  
  tleg_1->AddEntry(g5,"#delta=0.0");
  tleg_1->AddEntry(g6,"#delta=#pi");
  
  
}
