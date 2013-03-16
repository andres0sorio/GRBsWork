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

  makePlots("Vacuum","ZeroPt", "ModelA", "Pee", "output-ModelA-Vacuum-validation.root");
  
}

void makePlots( const char * model1, const char * model2, const char * src, const char * flavour, const char * infile) 
{
  
  tdrStyle->cd();
  
  //Output path
  TString path("./paper01-plots/flux/");
  
  TString dataPee1 = TString( model1 ) + TString("_") + TString( src ) + TString("_") + TString(flavour) + TString("/data");
  TString dataPee2 = TString( model2 ) + TString("_") + TString( src ) + TString("_") + TString(flavour) + TString("/data");
  
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
  
  TTree * PeeTreeNu1 = (TTree*)gDirectory->Get( dataPee1.Data() );
  TTree * PeeTreeNu2 = (TTree*)gDirectory->Get( dataPee2.Data() );
  
  //Branches
  double xx = 0.0;
  double yy = 0.0;
  double phi_e = 0.0;
  double phi_m = 0.0;
  double phi_t = 0.0;
  
  TGraph * ProbNu1[3];
  ProbNu1[0] = new TGraph();
  ProbNu1[1] = new TGraph();
  ProbNu1[2] = new TGraph();
  
  PeeTreeNu1->SetBranchAddress("Ex",&xx);
  PeeTreeNu1->SetBranchAddress("Phi_e",&phi_e);
  PeeTreeNu1->SetBranchAddress("Phi_m",&phi_m);
  PeeTreeNu1->SetBranchAddress("Phi_t",&phi_t);
  
  Long64_t nentries = PeeTreeNu1->GetEntries();
  
  std::cout << " data: " << nentries << std::endl;
  
  for (Long64_t i=0;i<nentries;i++) {
    PeeTreeNu1->GetEntry(i);
    ProbNu1[0]->SetPoint( i, xx, phi_e);
    ProbNu1[1]->SetPoint( i, xx, phi_m);
    ProbNu1[2]->SetPoint( i, xx, phi_t);
  }
  
  TGraph * ProbNu2[3];
  ProbNu2[0] = new TGraph();
  ProbNu2[1] = new TGraph();
  ProbNu2[2] = new TGraph();
  
  PeeTreeNu2->SetBranchAddress("Ex",&xx);
  PeeTreeNu2->SetBranchAddress("Phi_e",&phi_e);
  PeeTreeNu2->SetBranchAddress("Phi_m",&phi_m);
  PeeTreeNu2->SetBranchAddress("Phi_t",&phi_t);
  
  nentries = PeeTreeNu2->GetEntries();
  
  std::cout << " data: " << nentries << std::endl;
  
  for (Long64_t i=0;i<nentries;i++) {
    PeeTreeNu2->GetEntry(i);
    ProbNu2[0]->SetPoint( i, xx, phi_e);
    ProbNu2[1]->SetPoint( i, xx, phi_m);
    ProbNu2[2]->SetPoint( i, xx, phi_t);
  }

  TCanvas * c1[3];
  c1[0] = new TCanvas("flux1", "Normalized flux", 182, 134, 825, 263);
  c1[1] = new TCanvas("flux2", "Normalized flux", 182, 134, 825, 263);
  c1[2] = new TCanvas("flux3", "Normalized flux", 182, 134, 825, 263);

  for(int k=0; k < 3; ++k) 
  {
    
    ProbNu1[k]->SetMarkerStyle(1);
    ProbNu1[k]->SetMarkerColor(1);
    ProbNu2[k]->SetMarkerStyle(1);
    ProbNu2[k]->SetMarkerColor(2);
    ProbNu2[k]->SetLineColor(2);
    
    c1[k]->Divide(1,2);
    c1[k]->cd(1);
    gPad->SetLogx();
    ProbNu1[k]->Draw("APL");
    c1[k]->cd(2);
    gPad->SetLogx();
    ProbNu2[k]->Draw("APL");

  }
 
  TList * v_Labels = new TList();
  TObjString *label;
  label = new TObjString( "#phi_{e}" );
  v_Labels->Add( label ); 
  label = new TObjString( "#phi_{#mu}" );
  v_Labels->Add( label ); 
  label = new TObjString( "#phi_{#tau}" );
  v_Labels->Add( label ); 

  for( int j = 0; j < 3; ++j ) 
  {
    
    //char gname[10];
      
    TH1F * h1 =  ProbNu1[j]->GetHistogram()->Clone("g1");
    TH1F * h2 =  h1->Clone("g2");
    
    for(int k=1; k <= nentries; ++k) 
    {
      double xx = 0.0;
      double yy = 0.0;
    
      ProbNu1[j]->GetPoint(k-1, xx, yy);
      h1->SetBinContent(k,yy);
      ProbNu2[j]->GetPoint(k-1, xx, yy);
      h2->SetBinContent(k,yy);

    }
  
    TString yaxis = ((TObjString*)v_Labels->At(j))->GetString();
    TCanvas * compare = plotHistsAndRatio( h2 , h1, "h1","E [eV]", yaxis.Data());
    
    topTitle(src);
    
    std::stringstream saveAs;

    saveAs.str("");
    saveAs << path  << model1 << "/pdf/" << "nueosc_flux_compare_" << model2 << "_phi_" << j << ".pdf";
    compare->SaveAs( saveAs.str().c_str() );
    
    saveAs.str("");
    saveAs << path  << model1 << "/png/" << "nueosc_flux_compare_" << model2 << "_phi_" << j << ".png";
    compare->SaveAs( saveAs.str().c_str() );
  
    saveAs.str("");
    saveAs << path  << model1 << "/eps/" << "nueosc_flux_compare_" << model2 << "_phi_" << j << ".eps";
    compare->SaveAs( saveAs.str().c_str() );
    
  }
  
  
    
}

