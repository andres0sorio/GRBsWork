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

  makePlots("ModelA","output-ModelA-vacuum-All.root");
  
}

void makePlots( const char * model, const char * infile) 
{
  
  //Output path
  TString path("./paper01-plots/flux/");
  
  TString dataPee = TString( model ) + TString("_Vacuum_Pee/data");
  
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
  
  //Branches
  double xx = 0.0;
  double yy = 0.0;
  double phi_e = 0.0;
  double phi_m = 0.0;
  double phi_t = 0.0;
  
  TCanvas * c1[3];
  c1[0] = new TCanvas("c1", "Normalized flux", 182, 134, 825, 263);
  c1[1] = new TCanvas("c2", "Normalized flux", 182, 134, 825, 263);
  c1[2] = new TCanvas("c3", "Normalized flux", 182, 134, 825, 263);

  TGraph * ProbNu[3];
  ProbNu[0] = new TGraph();
  ProbNu[1] = new TGraph();
  ProbNu[2] = new TGraph();
  
  TLegend * leg = new TLegend(0.14,0.69,0.24,0.85);
  
  PeeTreeNu->SetBranchAddress("Ex",&xx);
  PeeTreeNu->SetBranchAddress("Phi_e",&phi_e);
  PeeTreeNu->SetBranchAddress("Phi_m",&phi_m);
  PeeTreeNu->SetBranchAddress("Phi_t",&phi_t);
  
  Long64_t nentries = PeeTreeNu->GetEntries();
  
  std::cout << " data: " << nentries << std::endl;
  
  for (Long64_t i=0;i<nentries;i++) {
    PeeTreeNu->GetEntry(i);
    ProbNu[0]->SetPoint( i, xx, phi_e);
    ProbNu[1]->SetPoint( i, xx, phi_m);
    ProbNu[2]->SetPoint( i, xx, phi_t);
  }
  
  for( int k=0; k < 3; ++k) 
  {
    ProbNu[k]->SetMarkerStyle(1);
    ProbNu[k]->SetFillColor(10);
    ProbNu[k]->SetMaximum(0.5);
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
  
  std::stringstream saveAs;

  for( int k=0; k < 3; ++k ) 
  {
    
    c1[k]->cd();
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogx();
    ProbNu[k]->Draw("APL");
    topTitle(model);
    leg->DrawClone();

    TLine *line = new TLine(8.4e+10,0.33,1.50e+14,0.33);
    line->SetLineColor(2);
    line->SetLineStyle(2);
    line->SetLineWidth(2);
    line->Draw();
    c1[k]->Modified();
    c1[k]->cd();
    
    saveAs.str("");
    saveAs << path << model << "/pdf/" << "nueosc_flux_" << model << "_ind_" << k << ".pdf";
    c1[k]->SaveAs( saveAs.str().c_str() );
    
    saveAs.str("");
    saveAs << path << model << "/png/" << "nueosc_flux_" << model << "_ind_" << k << ".png";
    c1[k]->SaveAs( saveAs.str().c_str() );

    saveAs.str("");
    saveAs << path << model << "/eps/" << "nueosc_flux_" << model << "_ind_" << k << ".eps";
    c1[k]->SaveAs( saveAs.str().c_str() );
    
  }
    
  
    
  
    
}

