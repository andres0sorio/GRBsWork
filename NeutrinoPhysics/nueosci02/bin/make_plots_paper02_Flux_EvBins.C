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
  tdrStyle->SetPadLeftMargin(0.16);
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

  bool plotSetII = true;
  
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
  
  makePlots(v_Variation, "ModelA", "EarthB", "Vacuum", "SetI", inputFile_SetI.Data() );
  makePlots(v_Variation, "ModelB", "EarthB", "Vacuum", "SetI", inputFile_SetI.Data() );
  makePlots(v_Variation, "ModelC", "EarthB", "Vacuum", "SetI", inputFile_SetI.Data() );
  
  v_Variation->Clear();
  
  label = new TObjString( "Var1_dCP180" );
  v_Variation->Add( label );
  
  label = new TObjString( "Var2_dCP180" );
  v_Variation->Add( label );
  
  label = new TObjString( "Var3_dCP180" );
  v_Variation->Add( label );
  
  makePlots(v_Variation, "ModelA", "EarthB", "Vacuum", "SetI", inputFile_SetI.Data() );
  makePlots(v_Variation, "ModelB", "EarthB", "Vacuum", "SetI", inputFile_SetI.Data() );
  makePlots(v_Variation, "ModelC", "EarthB", "Vacuum", "SetI", inputFile_SetI.Data() );
  
  v_Variation->Clear();
  
  if( plotSetII ) 
  {
    
    label = new TObjString( "Var1_dCP0" );
    v_Variation->Add( label );
    
    label = new TObjString( "Var2_dCP0" );
    v_Variation->Add( label );
    
    label = new TObjString( "Var3_dCP0" );
    v_Variation->Add( label );
    
    makePlots(v_Variation, "ModelA", "EarthB", "Vacuum", "SetII", inputFile_SetII.Data() );
    makePlots(v_Variation, "ModelB", "EarthB", "Vacuum", "SetII", inputFile_SetII.Data() );
    makePlots(v_Variation, "ModelC", "EarthB", "Vacuum", "SetII", inputFile_SetII.Data() );
    
    v_Variation->Clear();
    
    label = new TObjString( "Var1_dCP180" );
    v_Variation->Add( label );
    
    label = new TObjString( "Var2_dCP180" );
    v_Variation->Add( label );
    
    label = new TObjString( "Var3_dCP180" );
    v_Variation->Add( label );
    
    makePlots(v_Variation, "ModelA", "EarthB", "Vacuum", "SetII", inputFile_SetII.Data() );
    makePlots(v_Variation, "ModelB", "EarthB", "Vacuum", "SetII", inputFile_SetII.Data() );
    makePlots(v_Variation, "ModelC", "EarthB", "Vacuum", "SetII", inputFile_SetII.Data() );
    
    v_Variation->Clear();
    
  }
  
}

void makePlots( TList      * variations,
                const char * model,
                const char * target,
                const char * src,
                const char * config,
                const char * infile ) 
{

  bool normalize = false;
    
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
    
    cname = TString("Combined") + TString("_phi_") + TString(option);
    
    ctitle = TString("Flux for neutrinos") + TString(" ") + TString(option);
    
    c1 = new TCanvas( cname.Data(), ctitle.Data(), 206,141,722,575); 
    
    v_Canvas_Combined->Add( c1 );
    
    cname = TString("Combined") + TString("_antiphi_") + TString(option);
    
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

  int linecolor[6] = { 1, 1, 4, 4, 2, 2};
  int linestyle[6] = { 1, 1, 2, 2, 3, 3};
  int linewidth[6] = { 3, 3, 3, 3, 3, 3};

  TLegend * leg[2];
  
  leg[0] = new TLegend(0.66,0.62,0.89,0.85);
  
  leg[0]->SetBorderSize(0);
  leg[0]->SetTextSize(0.04);
  leg[0]->SetLineColor(1);
  leg[0]->SetLineStyle(1);
  leg[0]->SetLineWidth(1);
  leg[0]->SetFillColor(0);
  leg[0]->SetFillStyle(0);

  leg[1] = new TLegend(0.66,0.62,0.89,0.85);
    
  leg[1]->SetBorderSize(0);
  leg[1]->SetTextSize(0.04);
  leg[1]->SetLineColor(1);
  leg[1]->SetLineStyle(1);
  leg[1]->SetLineWidth(1);
  leg[1]->SetFillColor(0);
  leg[1]->SetFillStyle(0);

  max = v_Graphs->GetEntries();
  
  TString yTitle[2];
  yTitle[0] = TString("#phi ( E_{#nu} )");
  yTitle[1] = TString("#phi ( E_{#bar{#nu}})");

  TList * v_Flavour = new TList();
  TObjString *flavour;

  flavour = new TObjString( "e" );
  v_Flavour->Add( flavour );
  
  flavour = new TObjString( "e" );
  v_Flavour->Add( flavour );
  
  flavour = new TObjString( "#mu" );
  v_Flavour->Add( flavour );
  
  flavour = new TObjString( "#mu" );
  v_Flavour->Add( flavour );
  
  flavour = new TObjString( "#tau" );
  v_Flavour->Add( flavour );
  
  flavour = new TObjString( "#tau" );
  v_Flavour->Add( flavour );

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

    gg->GetXaxis()->SetTitle("E_{#nu} (eV)");
    gg->GetXaxis()->CenterTitle(true);
    gg->GetXaxis()->SetLabelFont(42);
    gg->GetXaxis()->SetLabelOffset(0.006);
    gg->GetXaxis()->SetLabelSize(0.06);
    gg->GetXaxis()->SetTitleSize(0.06);
    gg->GetXaxis()->SetTickLength(0.05);
    gg->GetXaxis()->SetTitleOffset(1.22);
    gg->GetXaxis()->SetTitleFont(42);
    gg->GetYaxis()->SetTitle(yTitle[(k%2)]);
    gg->GetYaxis()->CenterTitle(true);
    gg->GetYaxis()->SetNdivisions(509);
    gg->GetYaxis()->SetLabelFont(42);
    gg->GetYaxis()->SetLabelOffset(0.007);
    gg->GetYaxis()->SetLabelSize(0.06);
    gg->GetYaxis()->SetTitleSize(0.06);
    gg->GetYaxis()->SetTitleOffset(1.22);
    gg->GetYaxis()->SetTitleFont(42);

    // Configure each graph
    
    int kk = (k%6);
    
    gg->SetLineColor( linecolor[kk] );
    gg->SetLineWidth( linewidth[kk] );
    gg->SetLineStyle( linestyle[kk] );
                  
    gg->Draw();

    //... Save the graphs
   
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
    
    if( k < 6 ) 
    {
      TString alpha = ((TObjString*)v_Flavour->At(k))->GetString();      
      leg[(k%2)]->AddEntry( gg,alpha.Data(),"l");
    }
        
    c1 = (TCanvas*)v_Canvas_Combined->At(ccpos);
    c1->SetLogx();
    c1->cd();
    
    double norm = gg->Integral("width");
    std::cout << "Normalization factor " << norm << std::endl;
    
    if ( normalize )
      gg->Scale( 1.0 / norm );
    
    if( (k%6) == 0 || (k%6) == 1 ) 
      gg->Draw();
    else
      gg->Draw("SAME");
    
    
            
    if( (k%6) == 4 || (k%6) == 5 ) 
    {

      //TLine *line = new TLine(1.0e+11,0.3333,1.0e+16,0.3333);
      //line->Draw();
      
      leg[(k%2)]->DrawClone();

      
      std::stringstream saveAs;
  
      saveAs.str("");
      saveAs << path << model << "/pdf/" << c1->GetName() << "_" << config << "_" << model << ".pdf";
      c1->SaveAs( saveAs.str().c_str() );
      
      saveAs.str("");
      saveAs << path << model << "/png/" << c1->GetName() << "_" << config << "_" << model << ".png";
      c1->SaveAs( saveAs.str().c_str() );
      
      saveAs.str("");
      saveAs << path << model << "/eps/" << c1->GetName() << "_" << config << "_" << model << ".eps";
      c1->SaveAs( saveAs.str().c_str() );

      saveAs.str("");


      
    }
    

    
    //
      
  }
  






  
}



