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

  makePlots("ZeroPt", "ModelA","output.root");
  
}

void makePlots( const char * model, const char * src, const char *infile) 
{
  
  //Output path
  TString path("./paper01-plots/flux/");
  
  TString dataPee = TString( model ) + TString("_") + TString( src ) + TString("_Pee/data");
  TString dataAPee = TString( model ) + TString("_") + TString( src ) + TString("_aPee/data");
  
  TList * v_Labels = new TList();
  TObjString *label;
  label = new TObjString( "#phi_{e}" );
  v_Labels->Add( label ); 
  label = new TObjString( "#phi_{#mu}" );
  v_Labels->Add( label ); 
  label = new TObjString( "#phi_{#tau}" );
  v_Labels->Add( label ); 
  
  TFile * f1 = new TFile(infile);
  
  f1->cd();
  
  TTree * PeeTreeNu = (TTree*)gDirectory->Get( dataPee.Data() );
  TTree * PeeTreeANu = (TTree*)gDirectory->Get( dataAPee.Data() );
  
  //Branches
  double xx = 0.0;
  double y1 = 0.0;
  double y2 = 0.0;
  double y3 = 0.0;
  
  TCanvas * c1 = new TCanvas(model, "Oscillation probabilities", 184, 60, 861, 670);
  c1->Divide(1,3);
  
  TGraph * ProbNu[3];
  ProbNu[0] = new TGraph();
  ProbNu[1] = new TGraph();
  ProbNu[2] = new TGraph();

  TGraph * ProbANu[3];
  ProbANu[0] = new TGraph();
  ProbANu[1] = new TGraph();
  ProbANu[2] = new TGraph();
  
  TLegend * leg = new TLegend(0.14,0.69,0.24,0.85);
  
  PeeTreeNu->SetBranchAddress("Ex",&xx);
  PeeTreeNu->SetBranchAddress("Phi_e",&y1);
  PeeTreeNu->SetBranchAddress("Phi_m",&y2);
  PeeTreeNu->SetBranchAddress("Phi_t",&y3);
  
  Long64_t nentries = PeeTreeNu->GetEntries();
  
  for (Long64_t i=0;i<nentries;i++) {
    PeeTreeNu->GetEntry(i);
    ProbNu[0]->SetPoint( i, xx, y1);
    ProbNu[1]->SetPoint( i, xx, y2);
    ProbNu[2]->SetPoint( i, xx, y3);
  }
  
  ///Pee
  
  PeeTreeANu->SetBranchAddress("Ex",&xx);
  PeeTreeANu->SetBranchAddress("Phi_e",&y1);
  PeeTreeANu->SetBranchAddress("Phi_m",&y2);
  PeeTreeANu->SetBranchAddress("Phi_t",&y3);

  nentries = PeeTreeANu->GetEntries();
  
  for (Long64_t i=0;i<nentries;i++) {
    PeeTreeANu->GetEntry(i);
    ProbANu[0]->SetPoint( i, xx, y1);
    ProbANu[1]->SetPoint( i, xx, y2);
    ProbANu[2]->SetPoint( i, xx, y3);
  }
  
  for( int k=0; k < 3; ++k) 
  {
    ProbNu[k]->SetMarkerStyle(1);
    ProbNu[k]->SetFillColor(10);
    ProbNu[k]->SetMinimum(0.25);
    ProbNu[k]->SetMaximum(0.50);
    TString yaxis = ((TObjString*)v_Labels->At(k))->GetString();
    ProbNu[k]->GetYaxis()->SetTitle( yaxis.Data() );
    ProbNu[k]->GetXaxis()->SetTitle("E [eV]");
    ProbNu[k]->GetYaxis()->CenterTitle(true); 
    ProbNu[k]->GetXaxis()->CenterTitle(true); 
    ProbNu[k]->GetXaxis()->SetLabelOffset(0.007);
    ProbNu[k]->GetXaxis()->SetLabelSize(0.08);
    ProbNu[k]->GetXaxis()->SetTitleSize(0.08);
    ProbNu[k]->GetXaxis()->SetTitleOffset(0.9);
    ProbNu[k]->GetXaxis()->SetLabelFont(42);
    ProbNu[k]->GetYaxis()->SetLabelOffset(0.007);
    ProbNu[k]->GetYaxis()->SetLabelSize(0.08);
    ProbNu[k]->GetYaxis()->SetLabelFont(42);
    ProbNu[k]->GetYaxis()->SetTitleSize(0.09);
    ProbNu[k]->GetYaxis()->SetTitleOffset(0.45);
    ProbNu[k]->GetYaxis()->SetTitleFont(42);

  }

  for( int k=0; k < 3; ++k) 
  {
    ProbANu[k]->SetLineColor(4);
    ProbANu[k]->SetMarkerColor(4);
    ProbANu[k]->SetMarkerStyle(1);
    ProbANu[k]->SetFillColor(10);
    ProbANu[k]->SetMaximum(0.5);
    TString yaxis = ((TObjString*)v_Labels->At(k))->GetString();
    ProbANu[k]->GetYaxis()->SetTitle( yaxis.Data() );
    ProbANu[k]->GetXaxis()->SetTitle("E [eV]");
    ProbANu[k]->GetYaxis()->CenterTitle(true); 
    ProbANu[k]->GetXaxis()->CenterTitle(true); 
    ProbANu[k]->GetXaxis()->SetLabelOffset(0.007);
    ProbANu[k]->GetXaxis()->SetLabelSize(0.08);
    ProbANu[k]->GetXaxis()->SetTitleSize(0.08);
    ProbANu[k]->GetXaxis()->SetTitleOffset(0.9);
    ProbANu[k]->GetXaxis()->SetLabelFont(42);
    ProbANu[k]->GetYaxis()->SetLabelOffset(0.007);
    ProbANu[k]->GetYaxis()->SetLabelSize(0.08);
    ProbANu[k]->GetYaxis()->SetLabelFont(42);
    ProbANu[k]->GetYaxis()->SetTitleSize(0.09);
    ProbANu[k]->GetYaxis()->SetTitleOffset(0.45);
    ProbANu[k]->GetYaxis()->SetTitleFont(42);
    
  }
    
  leg->AddEntry( ProbNu[0], "#nu");
  leg->AddEntry( ProbANu[0], "anti-#nu");
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
  ProbANu[0]->Draw("PL");
  topTitle(model);
  leg->DrawClone();
  
  TLine *line = new TLine(8.4e+10,0.33,1.50e+14,0.33);
  line->SetLineColor(2);
  line->SetLineStyle(2);
  line->SetLineWidth(2);
  line->Draw();
  c1->Modified();
  
  c1->cd(2);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogx();
  ProbNu[1]->Draw("APL");
  ProbANu[1]->Draw("PL");
  leg->DrawClone();
  
  line = new TLine(8.4e+10,0.33,1.50e+14,0.33);
  line->SetLineColor(2);
  line->SetLineStyle(2);
  line->SetLineWidth(2);
  line->Draw();
  c1->Modified();

  c1->cd(3);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogx();
  ProbNu[2]->Draw("APL");
  ProbANu[2]->Draw("PL");
  leg->DrawClone();
  
  line = new TLine(8.4e+10,0.33,1.50e+14,0.33);
  line->SetLineColor(2);
  line->SetLineStyle(2);
  line->SetLineWidth(2);
  line->Draw();
  c1->Modified();
  
  c1->cd();

  std::stringstream saveAs;
  
  saveAs.str("");
  saveAs << path << model << "/pdf" << "/nueosc_fluxes_vac_" << model << "_f1" << ".pdf";
  c1->SaveAs( saveAs.str().c_str() );
  
  saveAs.str("");
  saveAs << path << model << "/png" << "/nueosc_fluxes_vac_" << model << "_f1" << ".png";
  c1->SaveAs( saveAs.str().c_str() );
  
  saveAs.str("");
  saveAs << path << model << "/eps" << "/nueosc_fluxes_vac_" << model << "_f1" << ".eps";
  c1->SaveAs( saveAs.str().c_str() );
  
      
}

