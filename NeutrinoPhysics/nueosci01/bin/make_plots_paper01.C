// $Id: $
// Include files 

//-----------------------------------------------------------------------------
// Implementation file for class : make_plots_paper01
// 
// 2013-03-02 : Andres Osorio
//-----------------------------------------------------------------------------

void topTitle()
{
  TLatex latex;
  latex.SetNDC();
  latex.SetTextSize(0.04);
  latex.SetTextAlign(31); // align right
  latex.DrawLatex(0.90,0.96,"Model A");
  latex.SetTextAlign(11); // align left
  latex.DrawLatex(0.18,0.96,"Preliminary");
}

void makePlots() 
{

  //Output path
  TString path("./slhc-plots/resolution/Taus/noPUC/isoStudies");
  
  gROOT->SetStyle("Plain");
  gROOT->SetBatch(false);
  // --- Use the CMS TDR style
  gROOT->ProcessLine(".L tdrStyle.C");
  setTDRStyle();
  tdrStyle->SetErrorX(0.5);
  tdrStyle->SetPadLeftMargin(0.18);
  tdrStyle->SetPadRightMargin(0.08);
  tdrStyle->SetLegendBorderSize(0);
  tdrStyle->SetTitleYOffset(1.3);
  tdrStyle->SetOptStat(0);
  tdrStyle->SetOptFit(0);
  tdrStyle->SetTitleFontSize(0.05);
  tdrStyle->SetStatStyle(0);

  TFile * f1 = new TFile("output.root" );

  f1->cd();

  TTree * PeeTree = (TTree*)gDirectory->Get("ModelA_Pee/data");
  TTree * PemTree = (TTree*)gDirectory->Get("ModelA_Pee/data");
  TTree * PetTree = (TTree*)gDirectory->Get("ModelA_Pee/data");
  
  //Branches
  double xx = 0.0;
  double yy = 0.0;

  PeeTree->SetBranchAddress("Ex",&xx);
  PeeTree->SetBranchAddress("Pb",&yy);
  
  TCanvas * c1 = new TCanvas("ModelA", "Oscillation probabilities", 184, 60, 861, 670);
  c1->Divide(1,3);
  
  TGraph * Prob[3];
  Prob[0] = new TGraph();
  Prob[1] = new TGraph();
  Prob[2] = new TGraph();
  
  TLegend * leg = new TLegend(0.21,0.72,0.44,0.86);
    
  Long64_t nentries = PeeTree->GetEntries();
  
  for (Long64_t i=0;i<nentries;i++) {
    PeeTree->GetEntry(i);
    Prob[0]->SetPoint( i, xx, yy);
  }
  
  nentries = PemTree->GetEntries();
  
  for (Long64_t i=0;i<nentries;i++) {
    PemTree->GetEntry(i);
    Prob[1]->SetPoint( i, xx, yy);
  }
  
  nentries = PetTree->GetEntries();
  
  for (Long64_t i=0;i<nentries;i++) {
    PetTree->GetEntry(i);
    Prob[2]->SetPoint( i, xx, yy);
  }
  
  for( int k=0; k < 3; ++k) 
  {
    Prob[k]->SetMarkerStyle(1);
    Prob[k]->SetFillColor(10);
  }
  
  c1->cd(1);
  gPad->SetLogx();
  Prob[0]->Draw("APL");
  topTitle();
  
  c1->cd(2);
  gPad->SetLogx();
  Prob[1]->Draw("APL");
  
  leg->AddEntry( Prob[1], "#nu");
  leg->SetBorderSize(0);
  leg->SetTextSize(0.1);
  leg->SetLineColor(1);
  leg->SetLineStyle(1);
  leg->SetLineWidth(1);
  leg->SetFillColor(0);
  leg->SetFillStyle(1001);
  
  leg->Draw();
  
  c1->cd(3);
  gPad->SetLogx();
  Prob[2]->Draw("APL");

  c1->cd();
  
    
}

