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
  
  TList * v_Variation = new TList();
  TObjString *label;

  TList * v_Labels = new TList();
  TString legend = TString("#Delta m^{2}_{32}=1.4 10^{-3} eV^{2}");
  
  label = new TObjString( legend.Data() );
  v_Labels->Add( label ); 

  legend = TString("#Delta m^{2}_{32}=6.0 10^{-3} eV^{2}");
  
  label = new TObjString( legend.Data() );
  v_Labels->Add( label ); 

  legend = TString("#Delta m^{2}_{32}=3.2 10^{-3} eV^{2}");

  label = new TObjString( legend.Data() );
  v_Labels->Add( label ); 

  //... Variations - match the name of the directory in the root file

  label = new TObjString( "Var1_dCP0" );
  v_Variation->Add( label );

  label = new TObjString( "Var2_dCP0" );
  v_Variation->Add( label );

  label = new TObjString( "Var3_dCP0" );
  v_Variation->Add( label );

  makePlots(v_Variation, v_Labels, 
            "ModelA", "EarthB", "Vacuum", "Set I", "SetI_dCP0", "detection-RvsAlfa-All-Models.root");
  
  v_Variation->Clear();

  /*

  label = new TObjString( "Var1_dCP180" );
  v_Variation->Add( label );

  label = new TObjString( "Var2_dCP180" );
  v_Variation->Add( label );

  label = new TObjString( "Var3_dCP180" );
  v_Variation->Add( label );


  //label = new TObjString( "SetI-1E17-Var1-dCP1" );
  //v_Variation->Add( label );
  
  //label = new TObjString( "SetI-1E17-Var2-dCP1" );
  //v_Variation->Add( label );
  
  //label = new TObjString( "SetI-1E17-Var3-dCP1" );
  //v_Variation->Add( label );
  
  makePlots(v_Variation, "ModelA", "EarthB", "Vacuum", "Set I", "alpha_SetI_dCP180", "detection-RvsAlfa-All-Models.root");
  
  v_Variation->Clear();

  */

  if( 0 ) 
  {
    
    
    label = new TObjString( "dCP0" );
    v_Variation->Add( label );
    
    label = new TObjString( "SetII-1E17-Var1-dCP0" );
    v_Variation->Add( label );
    
    label = new TObjString( "SetII-1E17-Var2-dCP0" );
    v_Variation->Add( label );
    
    label = new TObjString( "SetII-1E17-Var3-dCP0" );
    v_Variation->Add( label );
    
    //makePlots(v_Variation, "EarthB", "Vacuum", "Set II", "alpha_SetII_dCP0", "detection-setII.root");
    
    v_Variation->Clear();
    
    label = new TObjString( "dCP1" );
    v_Variation->Add( label );
    
    label = new TObjString( "SetII-1E17-Var1-dCP1" );
    v_Variation->Add( label );
    
    label = new TObjString( "SetII-1E17-Var2-dCP1" );
    v_Variation->Add( label );
    
    label = new TObjString( "SetII-1E17-Var3-dCP1" );
    v_Variation->Add( label );
    
    //makePlots(v_Variation, "EarthB", "Vacuum", "Set II", "alpha_SetII_dCP1", "detection-setII.root");
    
    v_Variation->Clear();
  
  }
  
   
}


void makePlots( TList      * Variations,
                TList      * Labels,
                const char * model,
                const char * target,
                const char * src,
                const char * config,
                const char * param, 
                const char * infile ) 
{
  
  //Input path
  TString inpath("./root_files/RvsAlfa/");

  TString inputfile = inpath + TString(infile);
  
  //Output path
  TString path("./paper02-plots/ratio/");
  
  TList * v_Trees  = new TList();
  TList * v_Graphs = new TList();
  
  //Input file
  
  TFile * f1 = new TFile( inputfile.Data() );
  
  f1->cd();
  
  int max = Variations->GetEntries();
  
  for( int k = 0; k < max; ++k ) {
    
    TString variation = ((TObjString*)Variations->At(k))->GetString();
    
    TString dataPxx = TString( "Ratio_" ) + 
      TString( model )     + TString("_") + 
      TString( target )    + TString("_") + 
      TString( src )       + TString("_") + 
      TString( variation ) + TString("/data");
    
    std::cout << " " << dataPxx << std::endl;
    
    TTree * PxxTreeNu = (TTree*)gDirectory->Get( dataPxx.Data() );
    
    v_Trees->Add( PxxTreeNu );
    
    TGraph * ProbNu = new TGraph();
    
    v_Graphs->Add( ProbNu );

    break;
    
    
  }

  TString cname = TString("Ratio") + TString("_") + TString(model) + TString("_") + TString(param);

  TString ctitle = TString("track/shower ratio") + TString(" ") + TString(param);

  TCanvas * c1 = new TCanvas( cname.Data(), ctitle.Data(), 206,141,722,575); 
  
  TLegend * leg = new TLegend(0.17,0.60,0.56,0.85);
  
  leg->SetBorderSize(0);
  leg->SetTextSize(0.04);
  leg->SetLineColor(1);
  leg->SetLineStyle(1);
  leg->SetLineWidth(1);
  leg->SetFillColor(0);
  leg->SetFillStyle(1001);
  
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

      std::cout << " xx: " << xx << " yy " << yy << std::endl;
      
    }

    break;
    
  }

  int linecolor[5] = { 2, 2, 2, 2, 1};
  int linestyle[5] = { 1, 2, 3, 4, 1};
  int linewidth[5] = { 2, 2, 2, 2, 2};
  
  ///
  
  for( int k = 0; k < max; ++k ) { 

    TGraph * gg = (TGraph*)v_Graphs->At(k);
    
    gg->SetMarkerStyle(25);
    gg->SetMarkerColor(1);
    gg->SetFillColor(10);

    gg->SetMaximum(2.2);
    gg->SetMinimum(1.4);
    
    gg->GetXaxis()->SetLimits( 2.0, 3.1 );
    
    gg->GetXaxis()->SetTitle("#alpha");
    gg->GetXaxis()->CenterTitle(true);
    gg->GetXaxis()->SetLabelFont(42);
    gg->GetXaxis()->SetLabelOffset(0.006);
    gg->GetXaxis()->SetLabelSize(0.06);
    gg->GetXaxis()->SetTitleSize(0.06);
    gg->GetXaxis()->SetTickLength(0.05);
    gg->GetXaxis()->SetTitleOffset(0.89);
    gg->GetXaxis()->SetTitleFont(42);
    gg->GetYaxis()->SetTitle("R");
    gg->GetYaxis()->CenterTitle(true);
    gg->GetYaxis()->SetNdivisions(509);
    gg->GetYaxis()->SetLabelFont(42);
    gg->GetYaxis()->SetLabelOffset(0.007);
    gg->GetYaxis()->SetLabelSize(0.06);
    gg->GetYaxis()->SetTitleSize(0.06);
    gg->GetYaxis()->SetTitleOffset(0.93);
    gg->GetYaxis()->SetTitleFont(42);
    
    gg->SetLineColor( linecolor[k] );
    gg->SetLineStyle( linestyle[k] );
    gg->SetLineWidth( linewidth[k] );
    
    TString alpha = ((TObjString*)Labels->At(k))->GetString();      
    leg->AddEntry( gg, alpha.Data(),"l");
    
    c1->cd();
    
    if ( k == 0 ) {
      
      gg->Draw("AL");
      
    } else 
      gg->Draw("L");

    break;
        
  }
  
  leg->Draw();
  
  c1->cd();
  
  std::stringstream saveAs;
  
  saveAs.str("");
  saveAs << path << model << "/pdf/" << "ratio_" << target << "_" << param << ".pdf";
  c1->SaveAs( saveAs.str().c_str() );
  
  saveAs.str("");
  saveAs << path << model << "/png/" << "ratio_" << target << "_" << param << ".png";
  c1->SaveAs( saveAs.str().c_str() );
  
  saveAs.str("");
  saveAs << path << model << "/eps/" << "ratio_" << target << "_" << param << ".eps";
  c1->SaveAs( saveAs.str().c_str() );
  
  
}



