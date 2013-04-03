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

  // Star -> Vacuum -> Surface of Earth

  makePlots("Vacuum",
            "SetupI",
            "./root_files/ModelA/output_ModelA_SetI.root", 
            "./root_files/ModelB/output_ModelB_SetI.root");
  
  makePlots("Vacuum",
            "SetupII",
            "./root_files/ModelA/output_ModelA_SetII.root", 
            "./root_files/ModelB/output_ModelB_SetII.root");

}

void makePlots( const char * modelA, 
                const char * source,
                const char * infileA ,  const char * infileB)
{
  
  //Output path
  TString path("./paper01-plots/flux/");

  TFile * f1 = new TFile(infileA);
  TFile * f2 = new TFile(infileB);

  TList * v_Data = new TList();
  TObjString *data;

  //A
  data = new TObjString( TString( modelA )  + TString("_") + TString( "ModelA" ) + TString("_Pee/data") );
  v_Data->Add( data );
  //B
  data = new TObjString( TString( modelA )  + TString("_") + TString( "ModelB" ) + TString("_Pee/data") );
  v_Data->Add( data );
  //anti A
  data = new TObjString( TString( modelA )  + TString("_") + TString( "ModelA" ) + TString("_aPee/data") );
  v_Data->Add( data );
  
  TList * v_Labels = new TList();
  TObjString *label;
  label = new TObjString( "#phi( E_{#nu} )" );
  v_Labels->Add( label ); 
  label = new TObjString( "#phi( E_{#nu} )" );
  v_Labels->Add( label ); 
  label = new TObjString( "#phi( E_{#bar{#nu}} )" );
  v_Labels->Add( label ); 

  TList * v_Title = new TList();
  label = new TObjString( "Model A" );
  v_Title->Add( label ); 
  label = new TObjString( "Model B" );
  v_Title->Add( label ); 
  label = new TObjString( "Model A" );
  v_Title->Add( label ); 
  
  TList * PeeTree = new TList();
  TList * PhiGraphs = new TList();

  TLegend * leg = new TLegend(0.14,0.69,0.24,0.85);

  TString treeName = ((TObjString*)v_Data->At(0))->GetString();
  f1->cd();
  PeeTree->Add( (TTree*)gDirectory->Get( treeName.Data() ) );
  std::cout << treeName << " " << (TTree*)gDirectory->Get( treeName.Data() ) << std::endl;
  
  treeName = ((TObjString*)v_Data->At(1))->GetString();
  f2->cd();
  PeeTree->Add( (TTree*)gDirectory->Get( treeName.Data() ) );
  std::cout << treeName << " " << (TTree*)gDirectory->Get( treeName.Data() ) << std::endl;
  
  treeName = ((TObjString*)v_Data->At(2))->GetString();
  f1->cd();
  PeeTree->Add( (TTree*)gDirectory->Get( treeName.Data() ) );
  std::cout << treeName << " " << (TTree*)gDirectory->Get( treeName.Data() ) << std::endl;
  
  for( int k = 0; k < 3; ++k ) 
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

  TCanvas * c1 = new TCanvas(modelA, "Oscillation probabilities", 184, 60, 861, 470);
  c1->Divide(1,3);
  c1->SetTopMargin(0.18);
  c1->SetBottomMargin(0.18);
  c1->Draw();
  
  for( int k=0; k < nGraphs; ++k) 
  {
    
    if ( idx >= 4 ) idx = 1;
     
    TGraph * g1 = (TGraph*)PhiGraphs->At(k);
    
    std::cout << " g1 " << g1 << std::endl;
  
    g1->SetLineWidth(1);
    
    if ( idx == 1 ) 
    {
      
      std::cout << idxc << " " << idx << std::endl;
      
      c1->cd( idxc );
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogx();
      //gPad->SetFillColor(2);
      
      g1->SetMarkerStyle(1);
      g1->SetFillColor(10);
      g1->SetMinimum(0.2);
      g1->SetMaximum(0.5);
      g1->GetYaxis()->SetNdivisions(504);
      TString yaxis = ((TObjString*)v_Labels->At( idxc-1))->GetString();
      g1->GetYaxis()->SetTitle( yaxis.Data() );
      g1->GetXaxis()->SetTitle("E [eV]");
      g1->GetYaxis()->CenterTitle(true); 
      g1->GetXaxis()->CenterTitle(true); 
      g1->GetXaxis()->SetLabelOffset(0.007);
      g1->GetXaxis()->SetLabelSize(0.13);
      g1->GetXaxis()->SetTitleSize(0.07);
      g1->GetXaxis()->SetTitleOffset(0.9);
      g1->GetXaxis()->SetLabelFont(42);
      g1->GetYaxis()->SetLabelOffset(0.007);
      g1->GetYaxis()->SetLabelSize(0.13);
      g1->GetYaxis()->SetLabelFont(42);
      g1->GetYaxis()->SetTitleSize(0.09);
      g1->GetYaxis()->SetTitleOffset(0.45);
      g1->GetYaxis()->SetTitleFont(42);
    
      g1->Draw("AL");

      TString title = ((TObjString*)v_Title->At(idxc-1))->GetString();
      topTitle(title.Data());
      
    } 
    
    else if ( idx == 2 ) {

      std::cout << " - " << idxc << " " << idx << std::endl;

      g1->SetLineColor(4);
      g1->SetLineStyle(2);
      g1->Draw("L");
      
    }
 
    else if ( idx == 3 ) {
      
      std::cout << " -- "<< idxc << " " << idx << std::endl;
      
      g1->SetLineColor(2);
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
    
  c1->cd();
  
  //topTitle("Fig 4");
  
  std::stringstream saveAs;
    
  saveAs.str("");
  saveAs << path << modelA << "/pdf/" << "nueosc_flux_" << source << "_3x_MenaFormat_F4" << ".pdf";
  c1->SaveAs( saveAs.str().c_str() );
  
  saveAs.str("");
  saveAs << path << modelA << "/png/" << "nueosc_flux_" << source << "_3x_MenaFormat_F4" << ".png";
  c1->SaveAs( saveAs.str().c_str() );

  saveAs.str("");
  saveAs << path << modelA << "/eps/" << "nueosc_flux_" << source << "_3x_MenaFormat_F4" << ".eps";
  c1->SaveAs( saveAs.str().c_str() );
  
}


