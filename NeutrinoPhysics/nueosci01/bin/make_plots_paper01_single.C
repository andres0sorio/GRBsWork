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

  makePlots("ModelA","Pee","output.root");
  makePlots("ModelA","aPee","output.root");
  
}

void makePlots( const char * model, const char * prob, const char * infile) 
{
  
  //Output path
  TString path("./results");
  
  TString dataPxx = TString( model ) + TString("_") + TString(prob) +  TString("/data");
  
  TList * v_Labels = new TList();
  TObjString *label;
  label = new TObjString( prob );
  v_Labels->Add( label ); 
    
  TFile * f1 = new TFile(infile);
  
  f1->cd();
  
  TTree * PxxTreeNu = (TTree*)gDirectory->Get( dataPxx.Data() );
    
  //Branches
  double xx = 0.0;
  double yy = 0.0;
  
  TString cname = TString(model) + TString("_") + TString(prob);
  
  TCanvas * c1 = new TCanvas( cname.Data(), "Oscillation probabilities", 184,60,861,263); 
    
  TGraph * ProbNu[3];
  ProbNu[0] = new TGraph();
    
  TLegend * leg = new TLegend(0.14,0.69,0.24,0.85);
    
  PxxTreeNu->SetBranchAddress("Ex",&xx);
  PxxTreeNu->SetBranchAddress("Pb",&yy);
  
  Long64_t nentries = PxxTreeNu->GetEntries();
  
  for (Long64_t i=0;i<nentries;i++) {
    PxxTreeNu->GetEntry(i);
    ProbNu[0]->SetPoint( i, xx, yy);
  }

  ProbNu[0]->SetMarkerStyle(1);
  ProbNu[0]->SetFillColor(10);
  ProbNu[0]->SetMaximum(1.3);
  TString yaxis = ((TObjString*)v_Labels->At(0))->GetString();
  ProbNu[0]->GetYaxis()->SetTitle( yaxis.Data() );
  ProbNu[0]->GetXaxis()->SetTitle("E [eV]");
  ProbNu[0]->GetYaxis()->CenterTitle(true); 
  ProbNu[0]->GetXaxis()->CenterTitle(true); 
  
  leg->AddEntry( ProbNu[0], "#nu");
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
  topTitle(model);
  leg->DrawClone();
  
  c1->cd();
  
  std::stringstream saveAs;
  
  saveAs.str("");
  saveAs << path << "/nueosc_" << model << "_" << prob << "_f1" << ".pdf";
  c1->SaveAs( saveAs.str().c_str() );
  
  saveAs.str("");
  saveAs << path << "/nueosc_" << model << "_" << prob << "_f1" << ".png";
  c1->SaveAs( saveAs.str().c_str() );
  
  
}

