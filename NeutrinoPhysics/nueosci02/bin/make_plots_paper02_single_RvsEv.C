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
  tdrStyle->SetPadLeftMargin(0.14);
  tdrStyle->SetPadRightMargin(0.08);
  tdrStyle->SetPadTopMargin(0.10);
  tdrStyle->SetPadBottomMargin(0.13);
  tdrStyle->SetLegendBorderSize(0);
  tdrStyle->SetTitleYOffset(0.8);
  tdrStyle->SetOptStat(0);
  tdrStyle->SetOptFit(0);
  tdrStyle->SetTitleFontSize(0.08);
  tdrStyle->SetStatStyle(0);
  tdrStyle->cd();

  TList * v_Variations = new TList();

  TObjString *var;
  var = new TObjString("RvsEv_dCP1");
  v_Variations->Add( var );

  var = new TObjString("RvsEv_dCP2");
  v_Variations->Add( var );
  
  makePlots(v_Variations, "ModelA", "EarthB", "Vacuum", "detection.root");
  
  v_Variations->Clear();

}

void makePlots(TList      * variation, 
               const char * model,
               const char * target, 
               const char * src,
               const char * infile) 
{
  
  //Output path
  TString path("./paper02-plots/ratio/");
    
  TList * v_Labels = new TList();
  TList * v_Graphs = new TList();
  
  TObjString *label;
  label = new TObjString( "#delta=0" );
  v_Labels->Add( label ); 
  
  label = new TObjString( "#delta=#pi" );
  v_Labels->Add( label ); 
  
  //

  TFile * f1 = new TFile(infile);
  f1->cd();

  int max = variation->GetEntries();

  //////////////////////////////////////////////
  // Set the binning
  
  float * m_vbins = new float[100];
  
  double m_e_min = 1.0E11;
  double m_e_max = 1.0E17;
  double DEx     = 0.30;
  double Xx      = m_e_min;
  
  int k = 1;
  
  m_vbins[0] = Xx; // This is the first lower edge bin value
  
  while( Xx < m_e_max ) 
  {
    
    double DeltaBin = DEx * Xx;
    double low_bin = Xx;
    double upp_bin = Xx + DeltaBin;
    
    //std::cout << "NeutrinosDetectionPaper> Energy binning: " << low_bin << " " << upp_bin << std::endl;
    
    Xx = upp_bin;
    m_vbins[k] = Xx;
    k += 1;
    
  }
  
  m_e_bins = ( k - 1); //Array has to have nbins+1 size
  
  //////////////////////////////////////////////
  
  for( int k = 0; k < max; ++k ) 
  {
    
    TString current = ((TObjString*)variation->At(k))->GetString();
    
    TString dataPxx = TString( "Ratio_" ) 
      + TString( model )  + TString("_")
      + TString( target ) + TString("_") 
      + TString( src )    + TString("_") 
      + TString( current.Data() )
      + TString("/data");
   
    std::cout << dataPxx << std::endl;
  
    TTree * InputTree = (TTree*)gDirectory->Get( dataPxx.Data() );
    
    //Branches
    double xx = 0.0;
    double yy = 0.0;
    
    double muTrks = 0.0;
    double tauTrks = 0.0;
    double NCHadShw = 0.0;
    double CCeHadShw = 0.0;
    double CCtauHadShw = 0.0;
    
    InputTree->SetBranchAddress("Xx",&xx);
    InputTree->SetBranchAddress("Ratio",&yy);
    
    InputTree->SetBranchAddress("MuTks",    &muTrks);
    InputTree->SetBranchAddress("TauTks",   &tauTrks);
    InputTree->SetBranchAddress("HadShwNC", &NCHadShw);
    InputTree->SetBranchAddress("HadShwE",  &CCeHadShw);
    InputTree->SetBranchAddress("HadShwT",  &CCtauHadShw);
    
    Long64_t nentries = InputTree->GetEntries();

    TString hname = current;
    
    TH1F * g1 = new TH1F(hname.Data(),"R vs energy bing",m_e_bins,m_vbins);
    
    for (Long64_t i=0;i<nentries;i++) {
      
      InputTree->GetEntry(i);

      g1->SetBinContent(i+1, yy);

      double R_Mena = muTrks / ( CCeHadShw + CCtauHadShw + NCHadShw );
     
      //g1->SetBinContent(i+1, R_Mena);
  
    }
    
    v_Graphs->Add( g1 );
    
  }
  
  TString cname = TString("Ratio") + TString("_") + TString(model);
  
  TCanvas * c1 = new TCanvas( cname.Data(), "track/shower ratio", 206,141,722,575); 
    
  TLegend * leg = new TLegend(0.18,0.70,0.45,0.86);
  
  leg->SetBorderSize(0);
  leg->SetTextSize(0.05);
  leg->SetLineColor(1);
  leg->SetLineStyle(1);
  leg->SetLineWidth(1);
  leg->SetFillColor(0);
  leg->SetFillStyle(1001);
  
  for( int k = 0; k < max; ++k ) 
  {

    TH1F * gg = (TH1F*)v_Graphs->At(k);

    gg->SetMarkerStyle(25);
    gg->SetFillColor(10);
    gg->SetMarkerColor(1);
    gg->SetLineColor(1);
    gg->SetFillColor(10);
    gg->SetMaximum(2.4);
    gg->SetMinimum(1.6);

    int pos1 = gg->FindBin( 1.0E11 );
    int pos2 = gg->FindBin( 1.0E16 );
    
    gg->GetXaxis()->SetRange( pos1, pos2 );

    gg->GetXaxis()->SetTitle("E_{#nu} [eV]");
    gg->GetXaxis()->CenterTitle(true);
    gg->GetXaxis()->SetLabelFont(42);
    gg->GetXaxis()->SetLabelOffset(0.006);
    gg->GetXaxis()->SetLabelSize(0.05);
    gg->GetXaxis()->SetTitleSize(0.05);
    gg->GetXaxis()->SetTickLength(0.05);
    gg->GetXaxis()->SetTitleOffset(1.15);
    gg->GetXaxis()->SetTitleFont(42);
    gg->GetXaxis()->SetNdivisions(509);
    gg->GetYaxis()->SetTitle("R");
    gg->GetYaxis()->CenterTitle(true);
    gg->GetYaxis()->SetNdivisions(509);
    gg->GetYaxis()->SetLabelFont(42);
    gg->GetYaxis()->SetLabelOffset(0.007);
    gg->GetYaxis()->SetLabelSize(0.05);
    gg->GetYaxis()->SetTitleSize(0.05);
    gg->GetYaxis()->SetTitleOffset(0.93);
    gg->GetYaxis()->SetTitleFont(42);

    gg->SetLineWidth(2);

    TString current = ((TObjString*)v_Labels->At(k))->GetString();

    leg->AddEntry( gg, current.Data(), "L");
    
    c1->cd();

    if( k == 0 )
      gg->Draw("");
    else 
    {
      gg->SetStats(0);
      gg->SetFillColor(10);
      gg->SetFillStyle(0);
      gg->SetLineColor(2);
      gg->SetLineStyle(2);
      gg->SetLineWidth(2);
      gg->Draw("SAME");
    }
    
    
  
  }
  
  leg->Draw();

  c1->SetLogx();
  
  c1->cd();
  
  std::stringstream saveAs;
  
  saveAs.str("");
  saveAs << path << model << "/pdf/" << "ratio_" << target << "_RvsEv" << ".pdf";
  c1->SaveAs( saveAs.str().c_str() );
  
  saveAs.str("");
  saveAs << path << model << "/png/" << "ratio_" << target << "_RvsEv" << ".png";
  c1->SaveAs( saveAs.str().c_str() );

  saveAs.str("");
  saveAs << path << model << "/eps/" << "ratio_" << target << "_RvsEv" << ".eps";
  c1->SaveAs( saveAs.str().c_str() );
  
  
}

