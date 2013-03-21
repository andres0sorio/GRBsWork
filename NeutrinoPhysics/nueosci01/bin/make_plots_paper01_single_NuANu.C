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
  latex.DrawLatex(0.90,0.92, title);
  latex.SetTextAlign(11); // align left
  latex.DrawLatex(0.18,0.92,"Preliminary");
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

  makePlots("ModelA","0", "Pem","output.root");
   
}

void makePlots( const char * model, const char * src, const char * prob, const char * infile) 
{
  
  //Output path
  TString path("./paper01-plots/probs/");
  
  TString dataPxx = TString( model ) + TString("_") + TString( src ) + TString("_") + TString(prob) +  TString("/data");
  TString dataAPxx = TString( model ) + TString("_") + TString( src ) + TString("_a") + TString(prob) +  TString("/data");
  
  TList * v_Labels = new TList();
  TObjString *label;
  label = new TObjString( prob );
  v_Labels->Add( label ); 
  label = new TObjString( prob );
  v_Labels->Add( label ); 
    
  TFile * f1 = new TFile(infile);
  
  f1->cd();
  
  TTree * PxxTreeNu = (TTree*)gDirectory->Get( dataPxx.Data() );
  TTree * PxxTreeANu = (TTree*)gDirectory->Get( dataAPxx.Data() );
  
  //Branches
  double xx = 0.0;
  double yy = 0.0;
  
  TString cname = TString(model) + TString("_") + TString(prob);
  
  TCanvas * c1 = new TCanvas( cname.Data(), "Oscillation probabilities", 184,60,861,263); 
    
  TGraph * ProbNu[3];
  ProbNu[0] = new TGraph();
  ProbNu[1] = new TGraph();
  
  TLegend * leg = new TLegend(0.14,0.69,0.24,0.85);
  
  PxxTreeNu->SetBranchAddress("Ex",&xx);
  PxxTreeNu->SetBranchAddress("Pb",&yy);
  
  Long64_t nentries = PxxTreeNu->GetEntries();
  
  for (Long64_t i=0;i<nentries;i++) {
    PxxTreeNu->GetEntry(i);
    ProbNu[0]->SetPoint( i, xx, yy);
  }

  PxxTreeANu->SetBranchAddress("Ex",&xx);
  PxxTreeANu->SetBranchAddress("Pb",&yy);
  
  Long64_t nentries = PxxTreeANu->GetEntries();
  
  for (Long64_t i=0;i<nentries;i++) {
    PxxTreeANu->GetEntry(i);
    ProbNu[1]->SetPoint( i, xx, yy);
  }

  ProbNu[0]->SetMarkerStyle(1);
  ProbNu[0]->SetFillColor(10);
  ProbNu[0]->SetMaximum(1.0);
  ProbNu[0]->SetMinimum(0.0);

  TString yaxis = ((TObjString*)v_Labels->At(0))->GetString();
  ProbNu[0]->GetYaxis()->SetTitle( yaxis.Data() );
  ProbNu[0]->GetXaxis()->SetTitle("E [eV]");
  ProbNu[0]->GetYaxis()->CenterTitle(true); 
  ProbNu[0]->GetXaxis()->CenterTitle(true); 
  ProbNu[0]->GetXaxis()->SetLabelOffset(0.007);
  ProbNu[0]->GetXaxis()->SetLabelSize(0.08);
  ProbNu[0]->GetXaxis()->SetTitleSize(0.07);
  ProbNu[0]->GetXaxis()->SetTitleOffset(0.9);
  ProbNu[0]->GetXaxis()->SetLabelFont(42);
  ProbNu[0]->GetYaxis()->SetLabelOffset(0.007);
  ProbNu[0]->GetYaxis()->SetLabelSize(0.08);
  ProbNu[0]->GetYaxis()->SetLabelFont(42);
  ProbNu[0]->GetYaxis()->SetTitleSize(0.09);
  ProbNu[0]->GetYaxis()->SetTitleOffset(0.45);
  ProbNu[0]->GetYaxis()->SetTitleFont(42);
  ProbNu[0]->GetYaxis()->SetNdivisions(509);
  
  ProbNu[1]->SetMarkerStyle(1);
  ProbNu[1]->SetMarkerColor(2);
  ProbNu[1]->SetLineColor(2);
  ProbNu[1]->SetFillColor(10);
  ProbNu[1]->SetMaximum(0.5);
  TString yaxis = ((TObjString*)v_Labels->At(0))->GetString();
  ProbNu[1]->GetYaxis()->SetTitle( yaxis.Data() );
  ProbNu[1]->GetXaxis()->SetTitle("E [eV]");
  ProbNu[1]->GetYaxis()->CenterTitle(true); 
  ProbNu[1]->GetXaxis()->CenterTitle(true); 
  ProbNu[1]->GetXaxis()->SetLabelOffset(0.007);
  ProbNu[1]->GetXaxis()->SetLabelSize(0.08);
  ProbNu[1]->GetXaxis()->SetTitleSize(0.07);
  ProbNu[1]->GetXaxis()->SetTitleOffset(0.9);
  ProbNu[1]->GetXaxis()->SetLabelFont(42);
  ProbNu[1]->GetYaxis()->SetLabelOffset(0.007);
  ProbNu[1]->GetYaxis()->SetLabelSize(0.08);
  ProbNu[1]->GetYaxis()->SetLabelFont(42);
  ProbNu[1]->GetYaxis()->SetTitleSize(0.09);
  ProbNu[1]->GetYaxis()->SetTitleOffset(0.45);
  ProbNu[1]->GetYaxis()->SetTitleFont(42);

  leg->AddEntry( ProbNu[0], "#nu");
  leg->AddEntry( ProbNu[1], "#bar{#nu}");

  leg->SetBorderSize(0);
  leg->SetTextSize(0.1);
  leg->SetLineColor(1);
  leg->SetLineStyle(1);
  leg->SetLineWidth(1);
  leg->SetFillColor(0);
  leg->SetFillStyle(1001);

  c1->cd();
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogx();
  ProbNu[0]->Draw("APL");
  ProbNu[1]->Draw("P");
  topTitle(model);
  leg->DrawClone();
  
  c1->cd();
  
  std::stringstream saveAs;
  
  saveAs.str("");
  saveAs << path << model << "/pdf/" << "nueosc_" << model << "_" << prob << "_nuanu" << ".pdf";
  c1->SaveAs( saveAs.str().c_str() );
  
  saveAs.str("");
  saveAs << path << model << "/png/" << "nueosc_" << model << "_" << prob << "_nuanu" << ".png";
  c1->SaveAs( saveAs.str().c_str() );

  saveAs.str("");
  saveAs << path << model << "/eps/" << "nueosc_" << model << "_" << prob << "_nuanu" << ".eps";
  c1->SaveAs( saveAs.str().c_str() );
  
  
}

