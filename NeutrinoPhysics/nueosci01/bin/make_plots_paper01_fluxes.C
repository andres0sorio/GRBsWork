// $Id: $
// Include files 

//-----------------------------------------------------------------------------
// Implementation file for class : make_plots_paper01
// 
// 2013-03-02 : Andres Osorio
//-----------------------------------------------------------------------------

void topTitle(const char *title)
{
  TLatex latex;
  latex.SetNDC();
  latex.SetTextSize(0.06);
  latex.SetTextAlign(31); // align right
  latex.DrawLatex(0.90,0.96, title);
  latex.SetTextAlign(11); // align left
  latex.DrawLatex(0.18,0.96,"Preliminary");
}

void makePlots() 
{
  gROOT->SetStyle("Plain");
  gROOT->SetBatch(false);
  // --- Use the CMS TDR style
  gROOT->ProcessLine(".L tdrStyle.C");
  setTDRStyle();
  tdrStyle->SetErrorX(0.5);
  tdrStyle->SetPadLeftMargin(0.10);
  tdrStyle->SetPadRightMargin(0.08);
  tdrStyle->SetPadTopMargin(0.10);
  tdrStyle->SetLegendBorderSize(0);
  tdrStyle->SetTitleYOffset(0.8);
  tdrStyle->SetOptStat(0);
  tdrStyle->SetOptFit(0);
  tdrStyle->SetTitleFontSize(0.08);
  tdrStyle->SetStatStyle(0);
  tdrStyle->cd();

  makePlots("ModelA","Pee");
  
}

void makePlots( const char * model, const char * flavour) 
{
  
  //Output path
  TString path("./results");
  
  TString dataPee = TString( model ) + TString("_") + TString(flavour) + TString("/data");
  
  TList * v_Labels = new TList();
  TObjString *label;
  label = new TObjString( "#phi" );
  v_Labels->Add( label ); 
  
  TFile * f1 = new TFile("output.root");
  
  f1->cd();
  
  TTree * PeeTreeNu = (TTree*)gDirectory->Get( dataPee.Data() );
  
  //Branches
  double xx = 0.0;
  double yy = 0.0;
  double ph = 0.0;
  
  TCanvas * c1 = new TCanvas(model, "Normalized flux", 182, 134, 825, 263);
  
  TGraph * ProbNu[3];
  ProbNu[0] = new TGraph();
  ProbNu[1] = new TGraph();
  ProbNu[2] = new TGraph();
  
  TLegend * leg = new TLegend(0.14,0.69,0.24,0.85);
  
  PeeTreeNu->SetBranchAddress("Ex",&xx);
  PeeTreeNu->SetBranchAddress("Pb",&yy);
  PeeTreeNu->SetBranchAddress("Phi",&ph);
  
  Long64_t nentries = PeeTreeNu->GetEntries();
  
  for (Long64_t i=0;i<nentries;i++) {
    PeeTreeNu->GetEntry(i);
    ProbNu[0]->SetPoint( i, xx, ph);
  }
  
  for( int k=0; k < 1; ++k) 
  {
    ProbNu[k]->SetMarkerStyle(1);
    ProbNu[k]->SetFillColor(10);
    ProbNu[k]->SetMaximum(1.3);
    TString yaxis = ((TObjString*)v_Labels->At(k))->GetString();
    ProbNu[k]->GetYaxis()->SetTitle( yaxis.Data() );
    ProbNu[k]->GetXaxis()->SetTitle("E [eV]");
    ProbNu[k]->GetYaxis()->CenterTitle(true); 
    ProbNu[k]->GetXaxis()->CenterTitle(true); 
    ProbNu[k]->GetXaxis()->SetLabelOffset(0.007);
    ProbNu[k]->GetXaxis()->SetLabelSize(0.07);
    ProbNu[k]->GetXaxis()->SetTitleSize(0.06);
    ProbNu[k]->GetXaxis()->SetTitleOffset(0.9);
    ProbNu[k]->GetYaxis()->SetLabelOffset(0.007);
    ProbNu[k]->GetYaxis()->SetLabelSize(0.07);
    ProbNu[k]->GetYaxis()->SetTitleSize(0.06);
    ProbNu[k]->GetYaxis()->SetTitleOffset(0.8);
    ProbNu[k]->GetYaxis()->SetTitleFont(42);
    
  }
  
  leg->AddEntry( ProbNu[0], "#nu");
  leg->SetBorderSize(0);
  leg->SetTextSize(0.1);
  leg->SetLineColor(1);
  leg->SetLineStyle(1);
  leg->SetLineWidth(1);
  leg->SetFillColor(0);
  leg->SetFillStyle(1001);
  

  c1->cd(1);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogx();
  ProbNu[0]->Draw("APL");
  topTitle(model);
  leg->DrawClone();
  
  c1->cd();

  std::stringstream saveAs;
    
  saveAs.str("");
  saveAs << path << "/nueosc_flux_" << model << "_" << flavour << "_f1" << ".pdf";
  c1->SaveAs( saveAs.str().c_str() );
    
  saveAs.str("");
  saveAs << path << "/nueosc_flux_" << model << "_" << flavour << "_f1" << ".png";
  c1->SaveAs( saveAs.str().c_str() );
  
  saveAs.str("");
  saveAs << path << "/nueosc_flux_" << model << "_" << flavour << "_f1" << ".eps";
  c1->SaveAs( saveAs.str().c_str() );
  
    
}

