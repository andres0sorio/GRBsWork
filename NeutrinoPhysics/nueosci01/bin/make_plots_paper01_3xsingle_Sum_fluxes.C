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
  latex.SetTextSize(0.10);
  latex.SetTextAlign(31); // align right
  latex.DrawLatex(0.90,0.92, title);
 
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
  tdrStyle->SetPadRightMargin(0.10);

  tdrStyle->SetPadTopMargin(0.20);
  tdrStyle->SetPadBottomMargin(0.20);

  tdrStyle->SetLegendBorderSize(0);
  tdrStyle->SetTitleYOffset(0.8);
  tdrStyle->SetOptStat(0);
  tdrStyle->SetOptFit(0);
  tdrStyle->SetTitleFontSize(0.08);
  tdrStyle->SetStatStyle(0);
  tdrStyle->cd();

  // Star
  
  makePlots("ModelA",
            "0",
            "SetupI",
            "./root_files/ModelA/output_ModelA_SetI.root",
            "Model A (at Star)");
  
  makePlots("Vacuum",
            "ModelA",
            "ModelA_SetupI",
            "./root_files/ModelA/output_ModelA_SetI.root",
            "Model A (at Earth)");
  
}

void makePlots( const char * modelA,
                const char * src,
                const char * config,
                const char * infileA,
                const char * option)
{
  
  bool use_color = false;
  
  int lineColor[3] = {1,2,4};
  int lineStyle[3] = {1,2,4};

  //Output path
  TString path("./paper01-plots/flux/");

  TFile * f1 = new TFile(infileA);
    
  TList * v_Data = new TList();
  TObjString *data;

  //A
  data = new TObjString( TString( modelA )    + TString("_") + TString( src ) + TString("_Pee/data") );
  v_Data->Add( data );
  //anti A
  data = new TObjString( TString( modelA )    + TString("_") + TString( src ) + TString("_aPee/data") );
  v_Data->Add( data );
  
  TList * v_Labels = new TList();
  TObjString *label;
  label = new TObjString( "#phi_{#nu_{e}} + #phi_{#bar{#nu}_{e}}" );
  v_Labels->Add( label ); 
  label = new TObjString( "#phi_{#nu_{#mu}} + #phi_{#bar{#nu}_{#mu}}" );
  v_Labels->Add( label ); 
  label = new TObjString( "#phi_{#nu_{#tau}} + #phi_{#bar{#nu}_{#tau}}" );
  v_Labels->Add( label ); 

  TList * v_Title = new TList();
  label = new TObjString( option );
  v_Title->Add( label ); 
  label = new TObjString( "" );
  v_Title->Add( label ); 
  label = new TObjString( "" );
  v_Title->Add( label ); 
  
  TList * PeeTree = new TList();
  TList * PhiGraphs = new TList();

  TLegend * leg = new TLegend(0.14,0.69,0.24,0.85);

  TString treeName = ((TObjString*)v_Data->At(0))->GetString();
  f1->cd();
  PeeTree->Add( (TTree*)gDirectory->Get( treeName.Data() ) );
  std::cout << treeName << " " << (TTree*)gDirectory->Get( treeName.Data() ) << std::endl;
  
  treeName = ((TObjString*)v_Data->At(1))->GetString();
  f1->cd();
  PeeTree->Add( (TTree*)gDirectory->Get( treeName.Data() ) );
  std::cout << treeName << " " << (TTree*)gDirectory->Get( treeName.Data() ) << std::endl;
  
  int maxData = v_Data->GetSize();
  
  for( int k = 0; k < maxData; ++k ) 
  {
    //Branches
    double xx = 0.0;
    double phi_e = 0.0;
    double phi_m = 0.0;
    double phi_t = 0.0;
    
    TTree * currentTree = (TTree*)PeeTree->At(k);
    
    currentTree->SetBranchAddress("Ex",&xx);
    currentTree->SetBranchAddress("Phi_e",&phi_e);
    currentTree->SetBranchAddress("Phi_m",&phi_m);
    currentTree->SetBranchAddress("Phi_t",&phi_t);
    
    Long64_t nentries = currentTree->GetEntries();
    
    TGraph * g1 = new TGraph();
    TGraph * g2 = new TGraph();
    TGraph * g3 = new TGraph();
    
    for (Long64_t i=0;i<nentries;i++) {
      currentTree->GetEntry(i);
      g1->SetPoint( i, xx, phi_e);
      g2->SetPoint( i, xx, phi_m);
      g3->SetPoint( i, xx, phi_t);
      
    }
    
    PhiGraphs->Add( g1 );
    PhiGraphs->Add( g2 );
    PhiGraphs->Add( g3 );
  
    std::cout << " * " << g1 << " " << g2 << " " << g3 << std::endl;
      
  }
  
  ////
  
  for( int k = 0; k < 3; ++k ) 
  {
    
    TGraph * g1 = (TGraph*)PhiGraphs->At(k);
    TGraph * g2 = (TGraph*)PhiGraphs->At(k+3);
    TGraph * g3 = new TGraph();
    
    int max_points = g1->GetN();
    
    double x1 = 0.0;
    double x2 = 0.0;
    
    double y1 = 0.0;
    double y2 = 0.0;
    
    for ( int n=0; n < max_points; ++n) 
    {
      
      g1->GetPoint( n, x1, y1 );
      g2->GetPoint( n, x2, y2 );

      g3->SetPoint( n, x1, (y1+y2) );
      
    }
    
    PhiGraphs->Add( g3 );
    
  }
  
  ///

  int idxc = 1;
  
  int nGraphs = PhiGraphs->GetSize();
  
  std::cout << " " << nGraphs << std::endl;

  TCanvas * c1 = new TCanvas(modelA, "Fluxes", 184, 60, 861, 470);
  c1->Divide(1,3);
  c1->SetTopMargin(0.18);
  c1->SetBottomMargin(0.18);
  c1->Draw();
  
  double ymin = 0.0;
  double ymax = 2.0;
  
  if ( std::string(modelA).compare("Vacuum") == 0 ) {
    ymin = 0.0;
    ymax = 1.0;
  }
  

  for( int k= 6; k < nGraphs; ++k) 
  {
    
    TGraph * g1 = (TGraph*)PhiGraphs->At(k);
    
    std::cout << " g1 " << g1 << std::endl;
    
    g1->SetLineWidth(1);
    
    std::cout << idxc << " " << std::endl;
    
    c1->cd( idxc );
    
    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogx();
    g1->SetMarkerStyle(1);
    g1->SetFillColor(10);
    g1->SetMaximum( ymax );
    g1->SetMinimum( ymin );

    g1->SetLineColor( lineColor[k-6] );
    g1->SetLineStyle( lineStyle[k-6] );

    g1->GetYaxis()->SetNdivisions(504);
    TString yaxis = ((TObjString*)v_Labels->At( idxc-1))->GetString();
    g1->GetYaxis()->SetTitle( yaxis.Data() );
    g1->GetXaxis()->SetTitle("E [eV]");
    g1->GetYaxis()->CenterTitle(true); 
    g1->GetXaxis()->CenterTitle(true); 
    g1->GetXaxis()->SetLabelOffset(0.007);
    g1->GetXaxis()->SetLabelSize(0.09);
    g1->GetXaxis()->SetTitleSize(0.11);
    g1->GetXaxis()->SetTitleOffset(0.5);
    g1->GetXaxis()->SetLabelFont(42);
    g1->GetYaxis()->SetLabelOffset(0.007);
    g1->GetYaxis()->SetLabelSize(0.09);
    g1->GetYaxis()->SetLabelFont(42);
    g1->GetYaxis()->SetTitleSize(0.12);
    g1->GetYaxis()->SetTitleOffset(0.25);
    g1->GetYaxis()->SetTitleFont(42);
    
    g1->Draw("AL");
    
    TString title = ((TObjString*)v_Title->At(idxc-1))->GetString();
    topTitle(title.Data());
    
    idxc+=1;
    
  }
  
  c1->cd();
  
  std::stringstream saveAs;
    
  saveAs.str("");
  saveAs << path << modelA << "/pdf/" << "nueosc_sumof_flux_" << config << "_3x" << ".pdf";
  c1->SaveAs( saveAs.str().c_str() );
  
  saveAs.str("");
  saveAs << path << modelA << "/png/" << "nueosc_sumof_flux_" << config << "_3x" << ".png";
  c1->SaveAs( saveAs.str().c_str() );
  
  saveAs.str("");
  saveAs << path << modelA << "/eps/" << "nueosc_sumof_flux_" << config << "_3x" << ".eps";
  c1->SaveAs( saveAs.str().c_str() );
  
}


