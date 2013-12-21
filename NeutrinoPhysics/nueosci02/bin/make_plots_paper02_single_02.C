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

  makePlots("EarthB", "Vacuum", "alpha", "detection.root");
   
}

void makePlots( const char * target, 
                const char * src, 
                const char * var, 
                const char * infile) 
{
  
  //Output path
  TString path("./paper02-plots/ratio/");
  
  TList * v_Models = new TList();

  TObjString *label;

  label = new TObjString( "StdPicture" );
  v_Models->Add( label ); 

  label = new TObjString( "ModelA" );
  v_Models->Add( label ); 

  label = new TObjString( "ModelB" );
  v_Models->Add( label ); 

  label = new TObjString( "ModelC" );
  v_Models->Add( label ); 
  
    
      
  TFile * f1 = new TFile(infile);
  
  f1->cd();

  TList * v_Trees = new TList();
  TList * v_Graphs = new TList();

  int max = v_Models->GetEntries();
  
  for( int k = 0; k < max; ++k ) {
    
    TString model = ((TObjString*)v_Models->At(k))->GetString();
    
    TString dataPxx = TString( "Ratio_" ) + TString( model ) + TString("_") 
      + TString( target ) + TString("_") + TString( src ) + TString("_") + TString(var) +  TString("/data");
    
    std::cout << " " << dataPxx << std::endl;
    
    TTree * PxxTreeNu = (TTree*)gDirectory->Get( dataPxx.Data() );
    
    v_Trees->Add( PxxTreeNu );

    TGraph * ProbNu = new TGraph();
    
    v_Graphs->Add( ProbNu );
    
  }
  
    
  TString cname = TString("Ratio") + TString("_") + TString(var);
  
  TCanvas * c1 = new TCanvas( cname.Data(), "track/shower ratio", 206,141,722,575); 
  
  TLegend * leg = new TLegend(0.39,0.71,0.66,0.87);

  //... Branches
  double xx = 0.0;
  double yy = 0.0;
  
  for( int k = 0; k < max; ++k ) { 
    
    TTree * PxxTreeNu = (TTree*)v_Trees->At(k);
    
    TGraph * ProbNu = (TGraph*)v_Graphs->At(k);
    
    PxxTreeNu->SetBranchAddress("Xx",&xx);
    PxxTreeNu->SetBranchAddress("Ratio",&yy);
  
    Long64_t nentries = PxxTreeNu->GetEntries();
  
    for (Long64_t i=0;i<nentries;i++) {
      PxxTreeNu->GetEntry(i);
      ProbNu->SetPoint( i, xx, yy);
    }

  }
  
  ///
  
  for( int k = 0; k < max; ++k ) { 

    TGraph * ProbNu = (TGraph*)v_Graphs->At(k);
    
    ProbNu->SetMarkerStyle(25);
    ProbNu->SetFillColor(10);
    ProbNu->SetMarkerColor(1);
    ProbNu->SetLineColor(1);
    ProbNu->SetFillColor(10);
    ProbNu->SetMaximum(3.0);
    ProbNu->SetMinimum(1.0);
    
    ProbNu->GetXaxis()->SetLimits( 2.0, 3.1);
    
    ProbNu->GetXaxis()->SetTitle("#alpha");
    ProbNu->GetXaxis()->CenterTitle(true);
    ProbNu->GetXaxis()->SetLabelFont(42);
    ProbNu->GetXaxis()->SetLabelOffset(0.006);
    ProbNu->GetXaxis()->SetLabelSize(0.06);
    ProbNu->GetXaxis()->SetTitleSize(0.06);
    ProbNu->GetXaxis()->SetTickLength(0.05);
    ProbNu->GetXaxis()->SetTitleOffset(0.89);
    ProbNu->GetXaxis()->SetTitleFont(42);
    ProbNu->GetYaxis()->SetTitle("R");
    ProbNu->GetYaxis()->CenterTitle(true);
    ProbNu->GetYaxis()->SetNdivisions(509);
    ProbNu->GetYaxis()->SetLabelFont(42);
    ProbNu->GetYaxis()->SetLabelOffset(0.007);
    ProbNu->GetYaxis()->SetLabelSize(0.06);
    ProbNu->GetYaxis()->SetTitleSize(0.06);
    ProbNu->GetYaxis()->SetTitleOffset(0.93);
    ProbNu->GetYaxis()->SetTitleFont(42);
    
    //leg->AddEntry( ProbNu, "#nu");
    //leg->SetBorderSize(0);
    //leg->SetTextSize(0.1);
    //leg->SetLineColor(1);
    //leg->SetLineStyle(1);
    //leg->SetLineWidth(1);
    //leg->SetFillColor(0);
    //leg->SetFillStyle(1001);
    
    c1->cd();
    
    //gPad->SetGridx();
    //gPad->SetGridy();

    if ( k == 0 ) {
      
      ProbNu->Draw("AP");
      topTitle(var);

    } else 
      ProbNu->Draw("P");
        
  }
  
  
  //leg->DrawClone();
  
  c1->cd();
  
  
  
  std::stringstream saveAs;
  
  saveAs.str("");
  saveAs << path << model << "/pdf/" << "ratio_" << target << "_" << var << "_XX" << ".pdf";
  c1->SaveAs( saveAs.str().c_str() );
  
  saveAs.str("");
  saveAs << path << model << "/png/" << "ratio_" << target << "_" << var << "_XX" << ".png";
  c1->SaveAs( saveAs.str().c_str() );

  saveAs.str("");
  saveAs << path << model << "/eps/" << "ratio_" << target << "_" << var << "_XX" << ".eps";
  c1->SaveAs( saveAs.str().c_str() );
  
  
}

