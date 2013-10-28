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
  latex.SetTextSize(0.11);
  latex.SetTextAlign(31); // align right
  latex.DrawLatex(0.88, 0.70, title);

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
  tdrStyle->SetLegendBorderSize(0);
  tdrStyle->SetTitleYOffset(0.8);
  tdrStyle->SetOptStat(0);
  tdrStyle->SetOptFit(0);
  tdrStyle->SetTitleFontSize(0.08);
  tdrStyle->SetStatStyle(0);
  tdrStyle->cd();

  //Figure 3
  //Star - These plots go into the paper01


  //. SET I
  
  makePlots("ModelA",
            "ModelB",
            "ModelC", 
            "0",
            "SetI",
            "./root_files/Ratios/output_ModelA_SetI.root",
            "./root_files/Ratios/output_ModelB_SetI.root",
            "./root_files/Ratios/output_ModelC_SetI.root",
            "./root_files/Ratios/output_ZeroPt_SetI.root");
  
  /*
    
    //. SET I - Fine DX
    
    makePlots("ModelA",
    "ModelB",
    "ModelC", 
    "0",
    "SetI",
    "./root_files/ModelA/output_ModelA_SetI_FineDX.root", 
    "./root_files/ModelB/output_ModelB_SetI_FineDX.root",
    "./root_files/ModelC/output_ModelC_SetI_FineDX.root",
    "./root_files/ZeroPt/output_ZeroPt_SetI_FineDX.root");
      
  */
  
  
  /* 
     
     //. SET II 
     
     makePlots("ModelA",
     "ModelB", 
     "ModelC", 
     "0", 
     "SetII",
     "./root_files/ModelA/output_ModelA_SetII.root", 
     "./root_files/ModelB/output_ModelB_SetII.root",
     "./root_files/ModelC/output_ModelC_SetII.root",
     "./root_files/ZeroPt/output_ZeroPt_SetII.root");
     
  */
  
  
}

void makePlots( const char * modelA, 
                const char * modelB,
                const char * modelC, 
                const char * src, 
                const char * config,
                const char * infileA,  
                const char * infileB,  
                const char * infileC,
                const char * infileD )
{
  
  bool use_color = true;
  
  int color[3] = {1, 4, 2}; //black, blue, red
 
  int lineStyle[3] = { 1 , 2 , 3};
      
  int lineWidth = 2;
  
  //Output path
  TString path("./paper01-plots/ratio/");

  TFile * f1 = new TFile(infileA);
  TFile * f2 = new TFile(infileB);
  TFile * f3 = new TFile(infileC);
  TFile * f4 = new TFile(infileD); // This is the Vacuum model inside Earth

  TList * v_Data = new TList();
  TObjString *data;

  //Vaccum
  data = new TObjString( TString( "ZeroPt" )  + TString("_") + TString( src ) + TString("_Pee/data") );
  v_Data->Add( data );
  //A
  data = new TObjString( TString( modelA )    + TString("_") + TString( src ) + TString("_Pee/data") );
  v_Data->Add( data );
  //B
  data = new TObjString( TString( modelB )    + TString("_") + TString( src ) + TString("_Pee/data") );
  v_Data->Add( data );
  //anti A
  data = new TObjString( TString( modelC )    + TString("_") + TString( src ) + TString("_Pee/data") );
  v_Data->Add( data );
  
  TList * v_Labels = new TList();
  TObjString *label;
  label = new TObjString( "#phi ( E_{#nu} )" );
  v_Labels->Add( label ); 
  label = new TObjString( "#phi_{e} ( E_{#nu} )" );
  v_Labels->Add( label ); 
  label = new TObjString( "#phi_{#mu} ( E_{#nu} ) " );
  v_Labels->Add( label ); 
  label = new TObjString( "#phi_{#tau} ( E_{#nu} ) " );
  v_Labels->Add( label ); 

  TList * v_Title = new TList();
  label = new TObjString( "(0) Vacuum" );
  v_Title->Add( label ); 
  label = new TObjString( "Model A" );
  v_Title->Add( label ); 
  label = new TObjString( "Model B" );
  v_Title->Add( label ); 
  label = new TObjString( "Model C" );
  v_Title->Add( label );
  
  TList * PeeTree = new TList();
  TList * PhiGraphs = new TList();
  TList * RatioGraphs = new TList();
  TList * VacuumGraphs = new TList();

  TString treeName = ((TObjString*)v_Data->At(0))->GetString();
  f4->cd();
  PeeTree->Add( (TTree*)gDirectory->Get( treeName.Data() ) );
  std::cout << treeName << " " << (TTree*)gDirectory->Get( treeName.Data() ) << std::endl;
  
  treeName = ((TObjString*)v_Data->At(1))->GetString();
  f1->cd(); //ModelA 
  PeeTree->Add( (TTree*)gDirectory->Get( treeName.Data() ) );
  std::cout << treeName << " " << (TTree*)gDirectory->Get( treeName.Data() ) << std::endl;
  
  treeName = ((TObjString*)v_Data->At(2))->GetString();
  f2->cd(); //ModelB
  PeeTree->Add( (TTree*)gDirectory->Get( treeName.Data() ) );
  std::cout << treeName << " " << (TTree*)gDirectory->Get( treeName.Data() ) << std::endl;
  
  treeName = ((TObjString*)v_Data->At(3))->GetString();
  f3->cd(); //ModelC 
  PeeTree->Add( (TTree*)gDirectory->Get( treeName.Data() ) );
  std::cout << treeName << " " << (TTree*)gDirectory->Get( treeName.Data() ) << std::endl;
  
  for( int k = 0; k < 4; ++k ) 
  {
    //Branches
    double xx = 0.0;
    double phi_e = 0.0;
    double phi_m = 0.0;
    double phi_t = 0.0;
    double Qf = 0.0;
    
    TTree * currentTree = (TTree*)PeeTree->At(k);
    
    currentTree->SetBranchAddress("Ex",&xx);
    currentTree->SetBranchAddress("Phi_e",&phi_e);
    currentTree->SetBranchAddress("Phi_m",&phi_m);
    currentTree->SetBranchAddress("Phi_t",&phi_t);
    currentTree->SetBranchAddress("Qf",&Qf);
    
    Long64_t nentries = currentTree->GetEntries();
    
    TGraph * g1 = new TGraph();
    TGraph * g2 = new TGraph();
    TGraph * g3 = new TGraph();
    
    int np = 0;
    
    for (Long64_t i=0;i<nentries;i++) {
      currentTree->GetEntry(i);
      
      if ( Qf >= 0 ) 
      {
        
        g1->SetPoint( np, xx, phi_e);
        g2->SetPoint( np, xx, phi_m);
        g3->SetPoint( np, xx, phi_t);
        np++;
        
      }
      

    }
    
    PhiGraphs->Add( g1 );
    PhiGraphs->Add( g2 );
    PhiGraphs->Add( g3 );
  
    std::cout << " * " << g1 << " " << g2 << " " << g3 << std::endl;
    
  }
  
  int idx = 1;
  int idxc = 1;
  int nGraphs = PhiGraphs->GetSize();
  
  //..................................................................................................

  Float_t small = 1e-5; //Referee request

  TCanvas * c1[3];
  
  c1[0] = new TCanvas(modelA, "Oscillation fluxes", 184, 60, 861, 670);
  c1[0]->Divide(1,3,small,small);
  c1[0]->Draw();
  
  c1[1] = new TCanvas(modelB, "Oscillation fluxes", 184, 60, 861, 670);
  c1[1]->Divide(1,3,small,small);
  c1[1]->Draw();
  
  c1[2] = new TCanvas(modelC, "Oscillation fluxes", 184, 60, 861, 670);
  c1[2]->Divide(1,3,small,small);
  c1[2]->Draw();

  TLegend * leg[3];

  for(int ll = 0; ll < 3; ++ll) 
  {
    
    leg[ll] = new TLegend(0.14,0.69,0.28,0.85);
   
    leg[ll]->SetBorderSize(1);
    leg[ll]->SetMargin(0.50);
    leg[ll]->SetTextFont(42);
    leg[ll]->SetTextSize(0.072);
    leg[ll]->SetLineColor(1);
    leg[ll]->SetLineStyle(1);
    leg[ll]->SetLineWidth(1);
    leg[ll]->SetFillColor(0);
    leg[ll]->SetFillStyle(1001);
    
  }
  
  
  bool plot_vacuum = true;
  
  for( int k=0; k < nGraphs; ++k) 
  {
    
    if ( k < 3 ) plot_vacuum = true;
    else  
      plot_vacuum = false;
        
    if ( idx >= 4 ) idx = 1; 
    
    TGraph * g1 = (TGraph*)PhiGraphs->At(k);
    
    g1->SetLineWidth(2); 
    
    //Plot first fluxes in Vaccum
    
    if ( plot_vacuum ) {
      
      for( idxc = 0; idxc < 3 ; ++idxc ) 
      {
        
        std::cout << idxc << " " << idx << std::endl;
        
        c1[idxc]->cd( idx );

        if( idx == 1 ) 
          leg[idxc]->AddEntry( g1, "Vaccum","L");

        if ( idx == 1 )
          gPad->SetBottomMargin(small);
        else if ( idx == 3)
          gPad->SetTopMargin(small);
        else { }
        
        if ( idx == 2 ) 
        {
          gPad->SetBottomMargin(small);
          gPad->SetTopMargin(small);
        }

        gPad->SetGridx();
        gPad->SetGridy();
        gPad->SetLogx();
        //gPad->SetLogy();
        
        g1->SetMarkerStyle(1);
        g1->SetFillColor(10);
        g1->SetMaximum(1.04);
        g1->SetMinimum(-0.05);
        
        g1->GetYaxis()->SetNdivisions(504);
        
        TString yaxis = ((TObjString*)v_Labels->At( idx ))->GetString();

        g1->GetYaxis()->SetTitle( yaxis.Data() );
        
        g1->GetXaxis()->SetTitle("E_{#nu} (eV)          ");
        g1->GetYaxis()->CenterTitle(true); 
        g1->GetXaxis()->CenterTitle(true); 
        g1->GetXaxis()->SetLabelOffset(0.007);
        g1->GetXaxis()->SetLabelSize(0.10);
        g1->GetXaxis()->SetTitleSize(0.11);
        g1->GetXaxis()->SetTitleOffset(0.5);
        g1->GetXaxis()->SetLabelFont(22);
        g1->GetXaxis()->SetTitleFont(22);
        g1->GetYaxis()->SetLabelOffset(0.007);
        g1->GetYaxis()->SetLabelSize(0.10);
        g1->GetYaxis()->SetTitleSize(0.16);
        g1->GetYaxis()->SetTitleOffset(0.25);
        g1->GetYaxis()->SetTitleFont(22);
        g1->GetYaxis()->SetLabelFont(22);
        
        g1->SetLineStyle( 1 );
        g1->SetLineColor( 1 );
        
        g1->Draw("AL");
        
      }
      
      ++idx;
      idxc = 0;
      
    } else {
          
      std::cout << "now each model " << idxc << " " << idx << std::endl;
        
      c1[idxc]->cd( idx );

      g1->SetLineStyle( 2 );
      g1->SetLineColor( 2 );

      g1->Draw("L");
      
      if( idx == 1 ) {
        leg[idxc]->AddEntry( g1, "Matter","L");
        TString title = ((TObjString*)v_Title->At(idxc+1))->GetString();
        topTitle(title.Data());
        leg[idxc]->Draw();
        
      }
      
      ++idx; 
          
      if ( idx >= 4 ) idxc += 1;

    }
        
        
  }
  

  for( int k = 0; k < 3; ++k) 
  {
    
    c1[k]->cd();
    
    std::stringstream saveAs;
    
    saveAs.str("");
    saveAs << path << modelA << "/pdf/" << "nueosc_flux_" << config << "_Models_ABC_Vac_" << k << ".pdf";
    c1[k]->SaveAs( saveAs.str().c_str() );
    
    saveAs.str("");
    saveAs << path << modelA << "/png/" << "nueosc_flux_" << config << "_Models_ABC_Vac_" << k << ".png";
    c1[k]->SaveAs( saveAs.str().c_str() );
    
    saveAs.str("");
    saveAs << path << modelA << "/eps/" << "nueosc_flux_" << config << "_Models_ABC_Vac_" << k << ".eps";
    c1[k]->SaveAs( saveAs.str().c_str() );
  
  }
  
  
}


