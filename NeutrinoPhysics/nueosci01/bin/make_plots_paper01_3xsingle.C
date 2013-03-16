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

  makePlots("ModelA","0","output-ModelA-vacuum-All.root");
  
}

void makePlots( const char * model, const char * src, const char * infile )
{
  
  //Output path
  TString path("./paper01-plots/probs/");
  
  TString dataPee  = TString( model ) + TString("_") + TString( src ) + TString("_Pee/data");
  TString dataPem  = TString( model ) + TString("_") + TString( src ) + TString("_Pem/data");
  TString dataPet  = TString( model ) + TString("_") + TString( src ) + TString("_Pet/data");
  TString dataAPee = TString( model ) + TString("_") + TString( src ) + TString("_aPee/data");
  TString dataAPem = TString( model ) + TString("_") + TString( src ) + TString("_aPem/data");
  TString dataAPet = TString( model ) + TString("_") + TString( src ) + TString("_aPet/data");
    
  TList * v_Labels = new TList();
  TObjString *label;
  label = new TObjString( "Pee" );
  v_Labels->Add( label ); 
  label = new TObjString( "Pe#mu" );
  v_Labels->Add( label ); 
  label = new TObjString( "Pe#tau" );
  v_Labels->Add( label ); 
  
  TFile * f1 = new TFile(infile);
  
  f1->cd();
  
  TTree * PeeTreeNu = (TTree*)gDirectory->Get( dataPee.Data() );
  TTree * PemTreeNu = (TTree*)gDirectory->Get( dataPem.Data() );
  TTree * PetTreeNu = (TTree*)gDirectory->Get( dataPet.Data() );
  
  TTree * PeeTreeANu = (TTree*)gDirectory->Get( dataAPee.Data() );
  TTree * PemTreeANu = (TTree*)gDirectory->Get( dataAPem.Data() );
  TTree * PetTreeANu = (TTree*)gDirectory->Get( dataAPem.Data() );
  
  //Branches
  double xx = 0.0;
  double yy = 0.0;
  
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
  PeeTreeNu->SetBranchAddress("Pb",&yy);
  
  Long64_t nentries = PeeTreeNu->GetEntries();
  
  for (Long64_t i=0;i<nentries;i++) {
    PeeTreeNu->GetEntry(i);
    ProbNu[0]->SetPoint( i, xx, yy);
  }

  PeeTreeANu->SetBranchAddress("Ex",&xx);
  PeeTreeANu->SetBranchAddress("Pb",&yy);
  
  nentries = PeeTreeANu->GetEntries();
  
  for (Long64_t i=0;i<nentries;i++) {
    PeeTreeANu->GetEntry(i);
    ProbANu[0]->SetPoint( i, xx, yy);
  }

  ///Pem
  
  PemTreeNu->SetBranchAddress("Ex",&xx);
  PemTreeNu->SetBranchAddress("Pb",&yy);

  nentries = PemTreeNu->GetEntries();
  
  for (Long64_t i=0;i<nentries;i++) {
    PemTreeNu->GetEntry(i);
    ProbNu[1]->SetPoint( i, xx, yy);
  }
  
  PemTreeANu->SetBranchAddress("Ex",&xx);
  PemTreeANu->SetBranchAddress("Pb",&yy);
  
  nentries = PemTreeANu->GetEntries();
  
  for (Long64_t i=0;i<nentries;i++) {
    PemTreeANu->GetEntry(i);
    ProbANu[1]->SetPoint( i, xx, yy);
  }

  ///Pet

  PetTreeNu->SetBranchAddress("Ex",&xx);
  PetTreeNu->SetBranchAddress("Pb",&yy);

  nentries = PetTreeNu->GetEntries();
  
  for (Long64_t i=0;i<nentries;i++) {
    PetTreeNu->GetEntry(i);
    ProbNu[2]->SetPoint( i, xx, yy);
  }
  
  PetTreeANu->SetBranchAddress("Ex",&xx);
  PetTreeANu->SetBranchAddress("Pb",&yy);
  
  nentries = PetTreeANu->GetEntries();
  
  for (Long64_t i=0;i<nentries;i++) {
    PetTreeANu->GetEntry(i);
    ProbANu[2]->SetPoint( i, xx, yy);
  }

  for( int k=0; k < 3; ++k) 
  {
    ProbNu[k]->SetMarkerStyle(1);
    ProbNu[k]->SetFillColor(10);
    ProbNu[k]->SetMaximum(1.0);
    TString yaxis = ((TObjString*)v_Labels->At(k))->GetString();
    ProbNu[k]->GetYaxis()->SetTitle( yaxis.Data() );
    ProbNu[k]->GetXaxis()->SetTitle("E [eV]");
    ProbNu[k]->GetYaxis()->CenterTitle(true); 
    ProbNu[k]->GetXaxis()->CenterTitle(true); 
    ProbNu[k]->GetXaxis()->SetLabelOffset(0.007);
    ProbNu[k]->GetXaxis()->SetLabelSize(0.08);
    ProbNu[k]->GetXaxis()->SetTitleSize(0.07);
    ProbNu[k]->GetXaxis()->SetTitleOffset(0.9);
    ProbNu[k]->GetXaxis()->SetLabelFont(42);
    ProbNu[k]->GetYaxis()->SetLabelOffset(0.007);
    ProbNu[k]->GetYaxis()->SetLabelSize(0.08);
    ProbNu[k]->GetYaxis()->SetLabelFont(42);
    ProbNu[k]->GetYaxis()->SetTitleSize(0.09);
    ProbNu[k]->GetYaxis()->SetTitleOffset(0.45);
    ProbNu[k]->GetYaxis()->SetTitleFont(42);
    
    ProbANu[k]->SetMarkerStyle(1);
    ProbANu[k]->SetMarkerColor(2);
    ProbANu[k]->SetLineColor(2);
    ProbANu[k]->SetFillColor(10);
    ProbANu[k]->SetMaximum(1.0);
    

  }
  
  leg->AddEntry( ProbNu[0], "#nu");
  leg->AddEntry( ProbANu[0], "#bar{#nu}");
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
  
  c1->cd(2);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogx();
  ProbNu[1]->Draw("APL");
  ProbANu[1]->Draw("P");
  leg->DrawClone();
    
  c1->cd(3);
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogx();
  ProbNu[2]->Draw("APL");
  ProbANu[2]->Draw("P");
  leg->DrawClone();

  c1->cd();

  std::stringstream saveAs;
    
  saveAs.str("");
  saveAs << path << model << "/pdf/" << "nueosc_probs_" << model << "_f2" << ".pdf";
  c1->SaveAs( saveAs.str().c_str() );
  
  saveAs.str("");
  saveAs << path << model << "/png/" << "nueosc_probs_" << model << "_f2" << ".png";
  c1->SaveAs( saveAs.str().c_str() );

  saveAs.str("");
  saveAs << path << model << "/eps/" << "nueosc_probs_" << model << "_f2" << ".eps";
  c1->SaveAs( saveAs.str().c_str() );
    
    
}

