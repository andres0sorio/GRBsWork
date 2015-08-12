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
  tdrStyle->SetPadBottomMargin(0.10);
  tdrStyle->SetLegendBorderSize(0);
  tdrStyle->SetTitleYOffset(0.8);
  tdrStyle->SetOptStat(0);
  tdrStyle->SetOptFit(0);
  tdrStyle->SetTitleFontSize(0.08);
  tdrStyle->SetStatStyle(0);
  tdrStyle->SetFrameLineWidth(1);
  tdrStyle->cd();

  //... These plots go into paper - Figs 7 & 8
  //Star -> Vacuum -> Earth ( Profile B )
  
  TString inputFile_ModelA_SetI("ModelA/output_ModelA_SetI_1e17_dCP0.root");
  TString inputFile_ModelB_SetI("ModelB/output_ModelB_SetI_1e17_dCP0.root");
  TString inputFile_ZeroPt_SetI("ZeroPt/output_ZeroPt_SetI_1e17_dCP0.root");
  
  TString inputFile_ModelA_SetII("ModelA/output_ModelA_SetII_1e17_dCP0.root");
  TString inputFile_ModelB_SetII("ModelB/output_ModelB_SetII_1e17_dCP0.root");
  TString inputFile_ZeroPt_SetII("ZeroPt/output_ZeroPt_SetII_1e17_dCP0.root");
  
  makePlots("EarthB", "Vacuum", "SetI",
            inputFile_ModelA_SetI.Data(),
            inputFile_ModelB_SetI.Data(),
            inputFile_ZeroPt_SetI.Data());
  
  makePlots("EarthB", "Vacuum", "SetII",
            inputFile_ModelA_SetII.Data(),
            inputFile_ModelB_SetII.Data(),
            inputFile_ZeroPt_SetII.Data());
  
  if( 0 ) 
  {
    
    
    //... These plots are for debugging & understanding
    //Star -> Vacuum -> (Earth surface)
    
    makePlots("Vacuum", "ModelX", "SetI",
              inputFile_ModelA_SetI.Data(),
              inputFile_ModelB_SetI.Data(),
              inputFile_ZeroPt_SetI.Data());
    
    makePlots("Vacuum", "ModelX", "SetII",
              inputFile_ModelA_SetII.Data(),
              inputFile_ModelB_SetII.Data(),
              inputFile_ZeroPt_SetII.Data());
    
    makePlots("ModelX", "0", "SetI",
              inputFile_ModelA_SetI.Data(),
              inputFile_ModelB_SetI.Data(),
              inputFile_ZeroPt_SetI.Data());
    
    makePlots("ModelX", "0", "SetII",
              inputFile_ModelA_SetII.Data(),
              inputFile_ModelB_SetII.Data(),
              inputFile_ZeroPt_SetII.Data());
    
  }
  
  //... Debuging lambdas - inverted positions 1->3 3->1 - Nov 05/2014
  //... in NeutrinosOscVarDensity.cc
  //... Data generated using jobs 484,491
  //... Nov 27: those files where delete - nothing changed

  if( 0 ) 
  {
    
    TString inputFile_ModelA_SetI_DBG("ModelA/output_ModelA_SetI_1e17_dCP0_debug.root");
    TString inputFile_ModelB_SetI_DBG("ModelB/output_ModelB_SetI_1e17_dCP0_debug.root");
    TString inputFile_ZeroPt_SetI_DBG("ZeroPt/output_ZeroPt_SetI_1e17_dCP0_debug.root");
  
    makePlots("EarthB", "Vacuum", "SetI",
              inputFile_ModelA_SetI_DBG.Data(),
              inputFile_ModelB_SetI_DBG.Data(),
              inputFile_ZeroPt_SetI_DBG.Data());
  
  }
  
  
}

void makePlots( const char * target, 
                const char * src, 
                const char * config,
                const char * infileA ,  const char * infileB,  const char * infileC)
{

  //... Limits on X and Y

  double XMIN = 0.9E11;
  double XMAX = 1.1E15;
  
  double YMIN = 0.25;
  double YMAX = 0.45;
  
  //..................................................................................................

  bool use_color = true;
  
  int linecolor[6] = { 1, 4, 2, 1, 1, 1};
  int linestyle[6] = { 1, 2, 3, 1, 1, 1};
  int linewidth[6] = { 2, 2, 2, 1, 1, 1};
  
  //..................................................................................................

  //Input path
  TString inpath("./root_files/");
  
  //Output path
  TString path("./paper02-plots/flux/");
  
  TFile * f1 = new TFile(inpath + TString(infileA));
  TFile * f2 = new TFile(inpath + TString(infileB));
  TFile * f3 = new TFile(inpath + TString(infileC));

  TList * v_Data = new TList();

  TObjString *data;
  
  TString TRG[4];
  TString SRC[4];
  
  if ( TString(target) == TString("Vacuum") )
  {
    
    int pos = 6;
    SRC[0] = TString(infileC)(0,pos);
    SRC[1] = TString(infileA)(0,pos);
    SRC[2] = TString(infileB)(0,pos);
    SRC[3] = TString(infileA)(0,pos);
    
    for(int ii=0; ii<4; ++ii) 
      TRG[ii] = TString(target);
    
  } else if ( TString(target) == TString("ModelX") ) 
  {
    
    for(int ii=0; ii<4; ++ii)
      SRC[ii] = TString(src);

    int pos = 6;
    TRG[0] = TString(infileC)(0,pos);
    TRG[1] = TString(infileA)(0,pos);
    TRG[2] = TString(infileB)(0,pos);
    TRG[3] = TString(infileA)(0,pos);

    YMIN = 0.00;
    YMAX = 0.90;
    
  }
  else 
  {
    for(int ii=0; ii<4; ++ii) {
      TRG[ii] = TString(target);
      SRC[ii] = TString(src);
    }
    
  }
  
  
  //Vaccum
  data = new TObjString( TRG[0]  + TString("_") + SRC[0] + TString("_Pee/data") );
  v_Data->Add( data );
  //A
  data = new TObjString( TRG[1]  + TString("_") + SRC[1] + TString("_Pee/data") );
  v_Data->Add( data );
  //B
  data = new TObjString( TRG[2]  + TString("_") + SRC[2] + TString("_Pee/data") );
  v_Data->Add( data );
  //anti A
  data = new TObjString( TRG[3]  + TString("_") + SRC[3] + TString("_aPee/data") );
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
  
  TCanvas * c1 = new TCanvas(target, "Fluxes for different models and falvours", 1483,27,893,992);
  
  c1->Divide(1,5,small,small); //Referee request

  c1->cd(1);
  gPad->SetPad(0, 0.7600, 0.9999999, 1.0000);

  c1->cd(2);
  gPad->SetPad(0, 0.5200, 0.9999999, 0.7600);
  
  c1->cd(3);
  gPad->SetPad(0, 0.2800, 0.9999999, 0.5200);

  c1->cd(4);
  gPad->SetPad(0, 0.0400, 0.9999999, 0.2800);
  
  c1->cd(5);
  gPad->SetPad(0, 0.0000, 0.9999999, 0.0400);

  c1->Draw();
  

  for( int k=0; k < nGraphs; ++k) 
  {
    
    if ( idx >= 4 ) idx = 1;
     
    TGraph * g1 = (TGraph*)PhiGraphs->At(k);
    
    std::cout << " g1 " << g1 << std::endl;
  
    TAxis * xaxis = g1->GetXaxis();
    
    xaxis->SetLimits( XMIN , XMAX); //force the range on the X-Axis
    
    g1->SetLineWidth(linewidth[0]); //update April 04 - for final version
    
    if ( idx == 1 ) 
    {
      
      std::cout << idxc << " " << idx << std::endl;
      
      c1->cd( idxc );

      //Referee request
    
      if ( idxc == 1 )
        gPad->SetBottomMargin(small);
      else if ( idxc == 4) 
      {
        gPad->SetTopMargin(small);
        gPad->SetBottomMargin(0.1176);
      }
                
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

      g1->SetMinimum(YMIN);
      g1->SetMaximum(YMAX);

      g1->GetYaxis()->SetNdivisions(503);
      TString yaxis = ((TObjString*)v_Labels->At( idxc-1))->GetString();
      g1->GetYaxis()->SetTitle( yaxis.Data() );
      //g1->GetXaxis()->SetTitle("E_{#nu} (eV)       ");
      
      g1->GetXaxis()->CenterTitle(true); 
      g1->GetXaxis()->SetLabelOffset(0.005);
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

      g1->SetLineStyle( linestyle[idx-1] );
      g1->SetLineColor( 1 );
      
      
      if( use_color ) g1->SetLineColor( linecolor[idx-1] );
      
      g1->Draw("AL");

      TString title = ((TObjString*)v_Title->At(idxc-1))->GetString();
      topTitle(title.Data());
      
    } 
    
    else if ( idx == 2 ) {

      std::cout << " - " << idxc << " " << idx << std::endl;

      g1->SetLineStyle( linestyle[idx-1] );
      g1->SetLineColor( 1 );

      if( use_color ) g1->SetLineColor( linecolor[idx-1] );

      g1->SetLineStyle(2);
      g1->Draw("L");
      
    }
 
    else if ( idx == 3 ) {
      
      std::cout << " -- "<< idxc << " " << idx << std::endl;

      g1->SetLineStyle( linestyle[idx-1] );
      g1->SetLineColor( 1 );
      
      if( use_color ) g1->SetLineColor( linecolor[idx-1] );

      g1->SetLineStyle(3);
      g1->Draw("L");
      idxc += 1;
      
    } else { }
    
    idx += 1;
    
  }

  c1->cd(5);
  
  TLatex * XTitle = new TLatex(0.46,0.21,"E_{#nu} (eV)");
  XTitle->SetTextFont(22);
  XTitle->SetTextSize(0.68);
  XTitle->Draw();
  
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
  saveAs << path << target << "/pdf/" << "nueosc_flux_" << target << "_" << config << "_4x_MenaFormat_F4" << ".pdf";
  c1->SaveAs( saveAs.str().c_str() );
  
  saveAs.str("");
  saveAs << path << target << "/png/" << "nueosc_flux_" << target << "_" << config << "_4x_MenaFormat_F4" << ".png";
  c1->SaveAs( saveAs.str().c_str() );

  saveAs.str("");
  saveAs << path << target << "/eps/" << "nueosc_flux_" << target << "_" << config << "_4x_MenaFormat_F4" << ".eps";
  c1->SaveAs( saveAs.str().c_str() );
  
}


