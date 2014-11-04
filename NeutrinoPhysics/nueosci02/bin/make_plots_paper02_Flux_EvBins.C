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
  tdrStyle->SetPadBottomMargin(0.15);
  tdrStyle->SetLegendBorderSize(0);
  tdrStyle->SetTitleYOffset(0.8);
  tdrStyle->SetOptStat(0);
  tdrStyle->SetOptFit(0);
  tdrStyle->SetTitleFontSize(0.08);
  tdrStyle->SetStatStyle(0);
  tdrStyle->SetFrameLineWidth(2);
  tdrStyle->cd();

  bool plotSetII = false;
  
  TString inputFile_SetI("detection-All-Models-SetI-RvsAlfa.root");

  TString inputFile_SetII("detection-All-Models-SetII-RvsAlfa.root");
  
  TList * v_Variation = new TList();
  TObjString *label;
  
  label = new TObjString( "Var1_dCP0" );
  v_Variation->Add( label );
  
  label = new TObjString( "Var2_dCP0" );
  v_Variation->Add( label );
  
  label = new TObjString( "Var3_dCP0" );
  v_Variation->Add( label );
  
  makePlots(v_Variation, "ModelA", "EarthB", "Vacuum", inputFile_SetI.Data() );
  //makePlots(v_Variation, "ModelB", "EarthB", "Vacuum", inputFile_SetI.Data() );
  //makePlots(v_Variation, "ModelC", "EarthB", "Vacuum", inputFile_SetI.Data() );
  
  v_Variation->Clear();
  
  label = new TObjString( "Var1_dCP180" );
  v_Variation->Add( label );
  
  label = new TObjString( "Var2_dCP180" );
  v_Variation->Add( label );
  
  label = new TObjString( "Var3_dCP180" );
  v_Variation->Add( label );
  
  //makePlots(v_Variation, "ModelA", "EarthB", "Vacuum", inputFile_SetI.Data() );
  //makePlots(v_Variation, "ModelB", "EarthB", "Vacuum", inputFile_SetI.Data() );
  //makePlots(v_Variation, "ModelC", "EarthB", "Vacuum", inputFile_SetI.Data() );
  
  v_Variation->Clear();
  
  if( plotSetII ) 
  {
    
    label = new TObjString( "Var1_dCP0" );
    v_Variation->Add( label );
    
    label = new TObjString( "Var2_dCP0" );
    v_Variation->Add( label );
    
    label = new TObjString( "Var3_dCP0" );
    v_Variation->Add( label );
    
    makePlots(v_Variation, "ModelA", "EarthB", "Vacuum", inputFile_SetII.Data() );
    makePlots(v_Variation, "ModelB", "EarthB", "Vacuum", inputFile_SetII.Data() );
    makePlots(v_Variation, "ModelC", "EarthB", "Vacuum", inputFile_SetII.Data() );
    
    v_Variation->Clear();
    
    label = new TObjString( "Var1_dCP180" );
    v_Variation->Add( label );
    
    label = new TObjString( "Var2_dCP180" );
    v_Variation->Add( label );
    
    label = new TObjString( "Var3_dCP180" );
    v_Variation->Add( label );
    
    makePlots(v_Variation, "ModelA", "EarthB", "Vacuum", inputFile_SetII.Data() );
    makePlots(v_Variation, "ModelB", "EarthB", "Vacuum", inputFile_SetII.Data() );
    makePlots(v_Variation, "ModelC", "EarthB", "Vacuum", inputFile_SetII.Data() );
    
    v_Variation->Clear();
    
  }
  
}

void makePlots( TList      * variations,
                const char * model,
                const char * target,
                const char * src,
                const char * infile ) 
{
  
  //Input path
  TString inpath("./root_files/RvsAlfa/");
  TString inputfile = inpath + TString(infile);
  
  //Output path
  TString path("./paper02-plots/flux/");
  
  TList * v_Labels = new TList();
  TList * v_Canvas = new TList();
  TList * v_Graphs = new TList();
  TList * v_Histos = new TList();
  
  TList * v_Canvas_Combined = new TList();
  
  TObjString *histo;

  histo = new TObjString( "phi_e" );
  v_Histos->Add( histo );
  
  histo = new TObjString( "phi_ae" );
  v_Histos->Add( histo );
  
  histo = new TObjString( "phi_mu" );
  v_Histos->Add( histo );
  
  histo = new TObjString( "phi_amu" );
  v_Histos->Add( histo );
  
  histo = new TObjString( "phi_tau" );
  v_Histos->Add( histo );
  
  histo = new TObjString( "phi_atau" );
  v_Histos->Add( histo );
  
  TFile * f1 = new TFile(inputfile);
  
  f1->cd();
  
  int max = variations->GetEntries();
  int maxhisto = v_Histos->GetEntries();

  for( int k = 0; k < max; ++k ) {
    
    TString option = ((TObjString*)variations->At(k))->GetString();
    
    TString dataPxx = TString( "Flux_Histograms_" ) + TString( model ) + TString("_") 
      + TString( target ) + TString("_") + TString( src ) + TString("_") + TString(option) +  TString("/");
    
    std::cout << " " << dataPxx << std::endl;
    
    f1->cd( dataPxx.Data() );
    
    for( int p = 0; p < maxhisto; ++p ) 
    {
      
      TString hname = ((TObjString*)v_Histos->At(p))->GetString();
      TH1F * h1 = (TH1F*)gDirectory->Get( hname.Data() );
      std::cout << h1 << std::endl;
      v_Graphs->Add( h1 );

    }
    
    f1->cd("../");
    
    TString cname = TString("Fluxes") + TString("_") + TString(option);
    
    TString ctitle = TString("Flux chart") + TString(" ") + TString(option);
    
    TCanvas * c1 = new TCanvas( cname.Data(), ctitle.Data(), 206,141,722,575); 
    c1->Divide(2,3);
    
    v_Canvas->Add( c1 );
  
    //... AO - Added nov 2014
    
    cname = TString("Fluxes") + TString("_phi_") + TString(option);
    
    ctitle = TString("Flux for neutrinos") + TString(" ") + TString(option);
    
    c1 = new TCanvas( cname.Data(), ctitle.Data(), 206,141,722,575); 
    
    v_Canvas_Combined->Add( c1 );
    
    cname = TString("Fluxes") + TString("_antiphi_") + TString(option);
    
    ctitle = TString("Flux for anti-neutrinos ") + TString(" ") + TString(option);
    
    c1 = new TCanvas( cname.Data(), ctitle.Data(), 206,141,722,575);
        
    v_Canvas_Combined->Add( c1 );
    
    //...

  }

  std::cout << "v_Canvas_Combined> size " << v_Canvas_Combined->GetEntries() << std::endl;
  
  ///
  int cpos = 0;
  int ccpos = 0;
  int varpos = 0;
  int idx  = 1;
  
  max = v_Graphs->GetEntries();
  
  for( int k = 0; k < max; ++k ) { 

    if ( idx > 6 ) 
    {
      cpos+=1;
      idx = 1;
      
    }
    
    TCanvas * c1 = (TCanvas*)v_Canvas->At(cpos);
    c1->cd( idx );
    gPad->SetLogx();
    
    TH1F * gg = (TH1F*)v_Graphs->At(k);
    
    gg->SetMaximum( 0.40 );
    gg->SetMinimum( 0.28 );

    gg->GetXaxis()->SetTitle("E_{v} [eV]");
    gg->GetXaxis()->CenterTitle(true);
    gg->GetXaxis()->SetLabelFont(42);
    gg->GetXaxis()->SetLabelOffset(0.006);
    gg->GetXaxis()->SetLabelSize(0.06);
    gg->GetXaxis()->SetTitleSize(0.06);
    gg->GetXaxis()->SetTickLength(0.05);
    gg->GetXaxis()->SetTitleOffset(1.22);
    gg->GetXaxis()->SetTitleFont(42);
    gg->GetYaxis()->SetTitle("#phi");
    gg->GetYaxis()->CenterTitle(true);
    gg->GetYaxis()->SetNdivisions(509);
    gg->GetYaxis()->SetLabelFont(42);
    gg->GetYaxis()->SetLabelOffset(0.007);
    gg->GetYaxis()->SetLabelSize(0.06);
    gg->GetYaxis()->SetTitleSize(0.06);
    gg->GetYaxis()->SetTitleOffset(1.22);
    gg->GetYaxis()->SetTitleFont(42);
    
    gg->SetLineWidth(3);
    
    if ( (k % 2) == 0 ) 
    {
      gg->SetLineColor( 1 );

    }
    else 
    {
      gg->SetLineColor( 2 );
    }
              
    gg->Draw();
   
    if( idx == 6 ) 
    {
    
      std::stringstream saveAs;
  
      saveAs.str("");
      saveAs << path << model << "/pdf/" << c1->GetName() << "_" << model << ".pdf";
      c1->SaveAs( saveAs.str().c_str() );
      
      saveAs.str("");
      saveAs << path << model << "/png/" << c1->GetName() << "_" << model << ".png";
      c1->SaveAs( saveAs.str().c_str() );
      
      saveAs.str("");
      saveAs << path << model << "/eps/" << c1->GetName() << "_" << model << ".eps";
      c1->SaveAs( saveAs.str().c_str() );

      saveAs.str("");
  
    }
 
    idx+=1;

    //.....................................................................................................
    
    //... AO - Added nov 2014
    
    if( (k%6) == 0 && k > 0 ) varpos += 1;
    
    ccpos = (2*varpos) + (k%2);
    
    std::cout << "k: " << k << " " << (k%2) << " " << (k%6) << " " << varpos << " " << ccpos << std::endl;
    
    c1 = (TCanvas*)v_Canvas_Combined->At(ccpos);
    c1->SetLogx();
    c1->cd();
    
    if( (k%6) == 0 || (k%6) == 1 ) 
      gg->Draw();
    else
      gg->Draw("SAME");
    
    TLine *line = new TLine(1.0e+11,0.33,1.0e+16,0.33);
    line->Draw();

    
    //
      
  }
  






  
}



