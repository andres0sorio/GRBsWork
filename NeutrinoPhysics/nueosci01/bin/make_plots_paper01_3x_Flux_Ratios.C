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
  label = new TObjString( "#phi ( E_{#nu} )" );
  v_Labels->Add( label ); 
  label = new TObjString( "#phi ( E_{#nu} ) " );
  v_Labels->Add( label ); 
  label = new TObjString( "#phi ( E_{#nu} ) " );
  v_Labels->Add( label ); 

  TList * v_Title = new TList();
  label = new TObjString( "(0) Vacuum" );
  v_Title->Add( label ); 
  label = new TObjString( "(a) Model A" );
  v_Title->Add( label ); 
  label = new TObjString( "(b) Model B" );
  v_Title->Add( label ); 
  label = new TObjString( "(c) Model C" );
  v_Title->Add( label );
  
  TList * PeeTree = new TList();
  TList * PhiGraphs = new TList();
  TList * RatioGraphs = new TList();
  TList * VacuumGraphs = new TList();

  TLegend * leg = new TLegend(0.14,0.69,0.24,0.85);

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

  TCanvas * c1 = new TCanvas(modelA, "Oscillation probabilities", 184, 60, 861, 670);
  c1->Divide(1,4); 
  c1->Draw();

  for( int k=0; k < nGraphs; ++k) 
  {
    
    if ( idx >= 4 ) idx = 1;
    
    TGraph * g1 = (TGraph*)PhiGraphs->At(k);
    
    g1->SetLineWidth(2); 
    
    if ( idx == 1 ) 
    {
      
      std::cout << idxc << " " << idx << std::endl;
      
      c1->cd( idxc );
            
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogx();
      //gPad->SetLogy();

      

      g1->SetMarkerStyle(1);
      g1->SetFillColor(10);
      g1->SetMaximum(1.04);
      g1->SetMinimum(-0.05);
      
      g1->GetYaxis()->SetNdivisions(504);
      TString yaxis = ((TObjString*)v_Labels->At( idxc-1))->GetString();
      g1->GetYaxis()->SetTitle( yaxis.Data() );
      g1->GetXaxis()->SetTitle("E_{#nu} (eV)");
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

      g1->SetLineStyle( lineStyle[idx-1] );
      
      g1->SetLineColor( 1 );

      if( use_color ) g1->SetLineColor( color[idx-1] );
      
      g1->Draw("AL");

      TString title = ((TObjString*)v_Title->At(idxc-1))->GetString();
      topTitle(title.Data());
      
      //
      if ( idxc == 1 ) 
      {
        
        char HistoName[20];
        sprintf(HistoName,"H1_%d", k);
        TGraph * h1 = g1->Clone(HistoName);
        VacuumGraphs->Add( h1 );
        std::cout << " graph " << k << " == in vacuum" << h1 << " " << std::string(HistoName) << std::endl;
        
      }
        
    }
              
    else if ( idx == 2 ) {

      std::cout << " - " << idxc << " " << idx << std::endl;
      
      g1->SetLineColor( 1 );
      
      if( use_color ) g1->SetLineColor( color[idx-1] );
      
      g1->SetLineStyle( lineStyle[idx-1] );
      
      g1->Draw("L");
      
      if ( idxc == 1 ) 
      {
        
        char HistoName[20];
        sprintf(HistoName,"H1_%d", k);
        TGraph * h1 = g1->Clone(HistoName);
        VacuumGraphs->Add( h1 );
        std::cout << " graph " << k << " == in vacuum" << h1 << " " << std::string(HistoName) << std::endl;

      }
      
    } else if ( idx == 3 ) {
      
      std::cout << " -- "<< idxc << " " << idx << std::endl;
      
      g1->SetLineColor( 1 );
      
      if( use_color ) g1->SetLineColor( color[idx-1]);
      
      g1->SetLineStyle( lineStyle[idx-1] );
      
      g1->Draw("L");
      
      if ( idxc == 1 ) 
      {
        
        char HistoName[20];
        sprintf(HistoName,"H1_%d", k);
        TGraph * h1 = g1->Clone(HistoName);
        VacuumGraphs->Add( h1 );
        std::cout << " graph " << k << " == in vacuum" << h1 << " " << std::string(HistoName) << std::endl;

      }
      
      idxc += 1;
      
    } else { }
   
    TLine *line = new TLine(7.9e+10,0.333,2.25e+14,0.333);
    line->SetLineColor(3);
    line->SetLineStyle(2);
    line->SetLineWidth(2);
    line->Draw("same");

 
    idx += 1;
        
  }
  
  c1->cd();

  //..................................................................................................

  idx = 1;

  double xp1 = 0.0;
  double xp2 = 0.0;
    
  for( int k = 3; k < nGraphs; ++k) 
  {
    
    if ( idx >= 4 ) idx = 1;
    
    TGraph * g1 = (TGraph*)PhiGraphs->At(k);
    
    int maxpts = g1->GetN();
    
    xx = 0.0;
    double y1    = 0.0;
    double y2    = 0.0;
    double ratio = 1.0;
    
    TGraph * r1 = new TGraph();
    int np = 0;
    
    if( idx == 1 ) 
    {
      
      TGraph * denom = (TGraph*)VacuumGraphs->At(0);
      np = 0;
      for( int j = 0; j < maxpts; ++j) 
      {
        
        g1->GetPoint(j, xp1, y1);
        denom->GetPoint(j, xp2, y2);
        if ( y2 <= 1.0e-3 ) continue;
        ratio = y1/y2;

        if (ratio > 5 ) std::cout << " nu_e> x1= " << xp1 << " x2= " << xp2 << " num= " 
                                  << y1 << " den= " << y2 << " num/den= " << ratio << std::endl;

        r1->SetPoint( np, xp1, ratio );
        ++np;
      }
      
      RatioGraphs->Add(r1);
      std::cout << " ratio added " << std::endl;
      
    } else if ( idx == 2 ) 
    {

      TGraph * denom = (TGraph*)VacuumGraphs->At(1);
      np = 0;
      for( int j = 0; j < maxpts; ++j) 
      {

        g1->GetPoint(j, xp1, y1);
        denom->GetPoint(j, xp2, y2);
        if ( y2 <= 1.0e-3 ) continue;
        ratio = y1/y2;
        if (ratio > 5 ) std::cout << " nu_e> x1= " << xp1 << " x2= " << xp2 << " num= " 
                                  << y1 << " den= " << y2 << " num/den= " << ratio << std::endl;
        r1->SetPoint( np, xp1, ratio );
        ++np;
      }
      
      RatioGraphs->Add(r1);
      std::cout << " ratio added " << std::endl;

    } else if ( idx == 3 ) 
    {

      TGraph * denom = (TGraph*)VacuumGraphs->At(2);
      np = 0;
      for( int j = 0; j < maxpts; ++j) 
      {
        
        g1->GetPoint(j, xp1, y1);
        denom->GetPoint(j, xp2, y2);
        if ( y2 <= 1.0e-3 ) continue;
        ratio = y1/y2;
        if (ratio > 5 ) std::cout << " nu_e> x1= " << xp1 << " x2= " << xp2 << " num= " 
                                  << y1 << " den= " << y2 << " num/den= " << ratio << std::endl;
        r1->SetPoint( np, xp1, ratio );
        ++np;
      }
      
      RatioGraphs->Add(r1);
      std::cout << " ratio added " << std::endl;

    } else {}
                
    idx += 1;
    
  }

  c1->cd(1);
  TGraph * g1 = (TGraph*)PhiGraphs->At(0);
  g1->Draw("L");
  c1->cd(2);
  g1 = (TGraph*)PhiGraphs->At(3);
  g1->Draw("L");
  c1->cd(3);
  g1 = (TGraph*)PhiGraphs->At(6);
  g1->Draw("L");
  c1->cd(4);
  g1 = (TGraph*)PhiGraphs->At(9);
  g1->Draw("L");
     
  c1->cd();


  std::stringstream saveAs;
    
  saveAs.str("");
  saveAs << path << modelA << "/pdf/" << "nueosc_flux_" << config << "_Models_ABC_Vac" << ".pdf";
  c1->SaveAs( saveAs.str().c_str() );
  
  saveAs.str("");
  saveAs << path << modelA << "/png/" << "nueosc_flux_" << config << "_Models_ABC_Vac" << ".png";
  c1->SaveAs( saveAs.str().c_str() );

  saveAs.str("");
  saveAs << path << modelA << "/eps/" << "nueosc_flux_" << config << "_Models_ABC_Vac" << ".eps";
  c1->SaveAs( saveAs.str().c_str() );
  

  //..................................................................................................

  Float_t small = 1e-5; //Referee request

  TCanvas * c2 = new TCanvas("Ratios", "Oscillation probabilities - Ratios", 184, 60, 861, 670);
  c2->Divide(1,3,small,small);
  c2->Draw();
  c2->cd();
  
  //
  idx = 1;
  idxc = 1;
  nGraphs = RatioGraphs->GetSize();

  for( int k=0; k < nGraphs; ++k) 
  {
    
    if ( idx >= 4 ) idx = 1;
     
    TGraph * g1 = (TGraph*)RatioGraphs->At(k);
    
    g1->SetLineWidth(lineWidth); 
    
    std::cout << " g1 " << g1 << std::endl;
      
    if ( idx == 1 ) 
    {
      
      std::cout << idxc << " " << idx << std::endl;
      
      c2->cd( idxc );
      
      //Referee request
    
      if ( idxc == 1 )
        gPad->SetBottomMargin(small);
      else if ( idxc == 3)
        gPad->SetTopMargin(small);
      else { }
      
      if ( idxc == 2 ) 
      {
        gPad->SetBottomMargin(small);
        gPad->SetTopMargin(small);
      }
      
      // ......................

      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogx();
      //gPad->SetLogy();
      
        
      g1->SetMarkerStyle(1);
      g1->SetFillColor(10);
      
      g1->SetMaximum(21.00);
      g1->SetMinimum(-1.05);
      
      g1->GetYaxis()->SetNdivisions(504);
      TString yaxis = ((TObjString*)v_Labels->At( idxc-1))->GetString();
      g1->GetYaxis()->SetTitle( "#phi/#phi_{vac}" );

      g1->GetXaxis()->SetTitle("E_{#nu} (eV)          ");
      g1->GetXaxis()->CenterTitle(true); 
      g1->GetXaxis()->SetLabelOffset(0.007);
      g1->GetXaxis()->SetLabelSize(0.10);
      g1->GetXaxis()->SetTitleSize(0.11);
      g1->GetXaxis()->SetTitleOffset(0.5);
      g1->GetXaxis()->SetLabelFont(22);
      g1->GetXaxis()->SetTitleFont(22);
      g1->GetXaxis()->SetTickLength(0.05);

      g1->GetYaxis()->CenterTitle(true); 
      g1->GetYaxis()->SetLabelOffset(0.007);
      g1->GetYaxis()->SetLabelSize(0.10);
      g1->GetYaxis()->SetTitleSize(0.16);
      g1->GetYaxis()->SetTitleOffset(0.25);
      g1->GetYaxis()->SetTitleFont(22);
      g1->GetYaxis()->SetLabelFont(22);
      g1->GetYaxis()->SetTickLength(0.05);

      g1->SetLineStyle( lineStyle[idx-1] );
      
      g1->SetLineColor( 1 );

      if( use_color ) g1->SetLineColor( color[idx-1] );
      
      
      g1->Draw("AL");

      TString title = ((TObjString*)v_Title->At(idxc))->GetString();
      topTitle(title.Data());

    } 
    
    else if ( idx == 2 ) {

      std::cout << " - " << idxc << " " << idx << std::endl;

      g1->SetLineColor( 1 );

      if( use_color ) g1->SetLineColor( color[idx-1] );
      
      g1->SetLineStyle( lineStyle[idx-1] );

      g1->Draw("L");
      
    }
 
    else if ( idx == 3 ) {
      
      std::cout << " -- "<< idxc << " " << idx << std::endl;

      g1->SetLineColor( 1 );
      
      if( use_color ) g1->SetLineColor( color[idx-1]);
      
      g1->SetLineStyle( lineStyle[idx-1] );

      g1->Draw("L");
      
      idxc += 1;
      
    } else { }
    
    idx += 1;
    
  }

  
  c2->cd(1);
  TGraph * g1 = (TGraph*)RatioGraphs->At(0);
  g1->Draw("L");
  c2->cd(2);
  g1 = (TGraph*)RatioGraphs->At(3);
  g1->Draw("L");
  c2->cd(3);
  g1 = (TGraph*)RatioGraphs->At(6);
  g1->Draw("L");
   
  c2->cd();
  
  //

  saveAs.str("");
  saveAs << path << modelA << "/pdf/" << "nueosc_flux_" << config << "_4x_Ratios_noLog" << ".pdf";
  c2->SaveAs( saveAs.str().c_str() );
  
  saveAs.str("");
  saveAs << path << modelA << "/png/" << "nueosc_flux_" << config << "_4x_Ratios_noLog" << ".png";
  c2->SaveAs( saveAs.str().c_str() );

  saveAs.str("");
  saveAs << path << modelA << "/eps/" << "nueosc_flux_" << config << "_4x_Ratios_noLog" << ".eps";
  c2->SaveAs( saveAs.str().c_str() );
  
}


