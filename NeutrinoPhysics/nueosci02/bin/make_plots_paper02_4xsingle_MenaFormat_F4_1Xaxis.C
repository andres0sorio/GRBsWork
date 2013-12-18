// $Id: $
// Include files 

//-----------------------------------------------------------------------------
// Implementation file for class : make_plots_paper02
// 
// 2013-03-02 : Andres Osorio
//-----------------------------------------------------------------------------

void topTitle(const char *title)
{
  TLatex latex;
  latex.SetNDC();
  latex.SetTextSize(0.10);
  latex.SetTextAlign(31); // align right
  latex.DrawLatex(0.88, 0.66, title);
 
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

  //Figure 4 - these plots go into paper
  //Star -> Vacuum -> Earth ( Profile B )

  makePlots("EarthB", "SetI",
            "./root_files/ModelA/output_ModelA_SetI.root", 
            "./root_files/ModelB/output_ModelB_SetI.root", 
            "./root_files/ZeroPt/output_ZeroPt_SetI.root");

  //makePlots("EarthB", "SetII",
  //          "./root_files/ModelA/output_ModelA_SetII.root", 
  //          "./root_files/ModelB/output_ModelB_SetII.root", 
  //          "./root_files/ZeroPt/output_ZeroPt_SetII.root");
  
}

void makePlots( const char * modelA, const char * config,
                const char * infileA ,  const char * infileB,  const char * infileC)
{
  
  bool use_color = true;
  
  int color[3] = {1, 4, 2};

  int lineStyle[3] = {1,2,3};
  
  int lineWidth = 2;
  
  //Output path
  TString path("./paper02-plots/flux/");

  TFile * f1 = new TFile(infileA);
  TFile * f2 = new TFile(infileB);
  TFile * f3 = new TFile(infileC);

  TList * v_Data = new TList();
  TObjString *data;

  //Vaccum
  data = new TObjString( TString( modelA )  + TString("_") + TString( "Vacuum" ) + TString("_Pee/data") );
  v_Data->Add( data );
  //A
  data = new TObjString( TString( modelA )  + TString("_") + TString( "Vacuum" ) + TString("_Pee/data") );
  v_Data->Add( data );
  //B
  data = new TObjString( TString( modelA )  + TString("_") + TString( "Vacuum" ) + TString("_Pee/data") );
  v_Data->Add( data );
  //anti A
  data = new TObjString( TString( modelA )  + TString("_") + TString( "Vacuum" ) + TString("_aPee/data") );
  v_Data->Add( data );
  
  TList * v_Labels = new TList();
  TObjString *label;
  label = new TObjString( "#phi ( E_{#nu} )" );
  v_Labels->Add( label ); 
  label = new TObjString( "#phi ( E_{#nu} )" );
  v_Labels->Add( label ); 
  label = new TObjString( "#phi ( E_{#nu} )" );
  v_Labels->Add( label ); 
  label = new TObjString( "#phi ( E_{#bar{#nu}} )" );
  v_Labels->Add( label ); 

  TList * v_Title = new TList();
  label = new TObjString( "(a) Vacuum (inside star)" );
  v_Title->Add( label ); 
  label = new TObjString( "(b) Model A" );
  v_Title->Add( label ); 
  label = new TObjString( "(c) Model B" );
  v_Title->Add( label ); 
  label = new TObjString( "(d) Model A" );
  v_Title->Add( label );
  
  TList * PeeTree = new TList();
  TList * PhiGraphs = new TList();

  TLegend * leg = new TLegend(0.14,0.69,0.24,0.85);

  TString treeName = ((TObjString*)v_Data->At(0))->GetString();
  f3->cd();
  PeeTree->Add( (TTree*)gDirectory->Get( treeName.Data() ) );
  std::cout << treeName << " " << (TTree*)gDirectory->Get( treeName.Data() ) << std::endl;
  
  treeName = ((TObjString*)v_Data->At(1))->GetString();
  f1->cd();
  PeeTree->Add( (TTree*)gDirectory->Get( treeName.Data() ) );
  std::cout << treeName << " " << (TTree*)gDirectory->Get( treeName.Data() ) << std::endl;
  
  treeName = ((TObjString*)v_Data->At(2))->GetString();
  f2->cd();
  PeeTree->Add( (TTree*)gDirectory->Get( treeName.Data() ) );
  std::cout << treeName << " " << (TTree*)gDirectory->Get( treeName.Data() ) << std::endl;
  
  treeName = ((TObjString*)v_Data->At(3))->GetString();
  f1->cd();
  PeeTree->Add( (TTree*)gDirectory->Get( treeName.Data() ) );
  std::cout << treeName << " " << (TTree*)gDirectory->Get( treeName.Data() ) << std::endl;
  
  for( int k = 0; k < 4; ++k ) 
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
  
  int idx = 1;
  int idxc = 1;
  
  int nGraphs = PhiGraphs->GetSize();
  
  std::cout << " " << nGraphs << std::endl;

  Float_t small = 1e-5; //Referee request
  
  //TCanvas * c1 = new TCanvas(modelA, "Oscillation probabilities", 184, 60, 861, 670);

  TCanvas * c1 = new TCanvas(modelA, "Oscillation probabilities", 1483,27,893,992);
  
  c1->Divide(1,4,small,small); //Referee request

  c1->Draw();
  
  for( int k=0; k < nGraphs; ++k) 
  {
    
    if ( idx >= 4 ) idx = 1;
     
    TGraph * g1 = (TGraph*)PhiGraphs->At(k);
    
    std::cout << " g1 " << g1 << std::endl;
  
    g1->SetLineWidth(lineWidth); //update April 04 - for final version
    
    if ( idx == 1 ) 
    {
      
      std::cout << idxc << " " << idx << std::endl;
      
      c1->cd( idxc );

      //Referee request
    
      if ( idxc == 1 )
        gPad->SetBottomMargin(small);
      else if ( idxc == 4)
        gPad->SetTopMargin(small);
      else { }
      
      if ( idxc == 2 || idxc == 3 ) 
      {
        gPad->SetBottomMargin(small);
        gPad->SetTopMargin(small);
      }
      
      // ......................

      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogx();
      
      g1->SetMarkerStyle(1);
      g1->SetFillColor(10);

      g1->SetMinimum(0.25);
      g1->SetMaximum(0.45);

      g1->GetYaxis()->SetNdivisions(503);
      TString yaxis = ((TObjString*)v_Labels->At( idxc-1))->GetString();
      g1->GetYaxis()->SetTitle( yaxis.Data() );
      g1->GetXaxis()->SetTitle("E_{#nu} (eV)");
      
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

      TString title = ((TObjString*)v_Title->At(idxc-1))->GetString();
      topTitle(title.Data());
      
    } 
    
    else if ( idx == 2 ) {

      std::cout << " - " << idxc << " " << idx << std::endl;

      g1->SetLineStyle( lineStyle[idx-1] );
      g1->SetLineColor( 1 );

      if( use_color ) g1->SetLineColor( color[idx-1] );

      g1->SetLineStyle(2);
      g1->Draw("L");
      
    }
 
    else if ( idx == 3 ) {
      
      std::cout << " -- "<< idxc << " " << idx << std::endl;

      g1->SetLineStyle( lineStyle[idx-1] );
      g1->SetLineColor( 1 );
      
      if( use_color ) g1->SetLineColor( color[idx-1] );

      g1->SetLineStyle(3);
      g1->Draw("L");
      idxc += 1;
      
    } else { }
    
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
  saveAs << path << modelA << "/pdf/" << "nueosc_flux_" << config << "_4x_MenaFormat_F4" << ".pdf";
  c1->SaveAs( saveAs.str().c_str() );
  
  saveAs.str("");
  saveAs << path << modelA << "/png/" << "nueosc_flux_" << config << "_4x_MenaFormat_F4" << ".png";
  c1->SaveAs( saveAs.str().c_str() );

  saveAs.str("");
  saveAs << path << modelA << "/eps/" << "nueosc_flux_" << config << "_4x_MenaFormat_F4" << ".eps";
  c1->SaveAs( saveAs.str().c_str() );
  
}


