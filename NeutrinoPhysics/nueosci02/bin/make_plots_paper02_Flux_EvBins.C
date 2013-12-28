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
  
  label = new TObjString( "SetI-1E17-Var1-dCP0" );
  v_Variation->Add( label );
  
  label = new TObjString( "SetI-1E17-Var2-dCP0" );
  v_Variation->Add( label );
  
  label = new TObjString( "SetI-1E17-Var3-dCP0" );
  v_Variation->Add( label );
  
  makePlots(v_Variation, "ModelA", "EarthB", "Vacuum", "detection-setI.root");
  
  v_Variation->Clear();
  
  label = new TObjString( "SetI-1E17-Var1-dCP1" );
  v_Variation->Add( label );
  
  label = new TObjString( "SetI-1E17-Var2-dCP1" );
  v_Variation->Add( label );
  
  label = new TObjString( "SetI-1E17-Var3-dCP1" );
  v_Variation->Add( label );
  
  makePlots(v_Variation, "ModelA", "EarthB", "Vacuum", "detection-setI.root");
  
  v_Variation->Clear();

  label = new TObjString( "SetII-1E17-Var1-dCP0" );
  v_Variation->Add( label );
  
  label = new TObjString( "SetII-1E17-Var2-dCP0" );
  v_Variation->Add( label );
  
  label = new TObjString( "SetII-1E17-Var3-dCP0" );
  v_Variation->Add( label );
  
  makePlots(v_Variation, "ModelA", "EarthB", "Vacuum", "detection-setII.root");

  v_Variation->Clear();

  label = new TObjString( "SetII-1E17-Var1-dCP1" );
  v_Variation->Add( label );
  
  label = new TObjString( "SetII-1E17-Var2-dCP1" );
  v_Variation->Add( label );
  
  label = new TObjString( "SetII-1E17-Var3-dCP1" );
  v_Variation->Add( label );
  
  makePlots(v_Variation, "ModelA", "EarthB", "Vacuum", "detection-setII.root");

  v_Variation->Clear();



  


   
}

void makePlots( TList      * variations,
                const char * model,
                const char * target,
                const char * src,
                const char * infile ) 
{
  
  //Output path
  TString path("./paper02-plots/flux/");
  
  TList * v_Labels = new TList();
  TList * v_Canvas = new TList();
  TList * v_Graphs = new TList();
  TList * v_Histos = new TList();
  
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
  
  TFile * f1 = new TFile(infile);
  
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
      
  }
  
  ///
  int cpos = 0;
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

    if ( k % 2 == 0 ) 
      gg->SetLineColor( 1 );
    else 
      gg->SetLineColor( 2 );
          
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
      
  }
  
}



