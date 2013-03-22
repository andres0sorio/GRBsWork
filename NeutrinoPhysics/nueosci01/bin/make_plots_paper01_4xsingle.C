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
  tdrStyle->SetPadLeftMargin(0.10);
  tdrStyle->SetPadRightMargin(0.08);
  tdrStyle->SetPadTopMargin(0.10);
  tdrStyle->SetLegendBorderSize(0);
  tdrStyle->SetTitleYOffset(0.8);
  tdrStyle->SetOptStat(0);
  tdrStyle->SetOptFit(0);
  tdrStyle->SetTitleFontSize(0.08);
  tdrStyle->SetStatStyle(0);
  tdrStyle->cd();

  //makePlots("ModelA","0","./root_files/Mena1/output_ModelA.root");

  //makePlots("EarthB","0","output_0_.root");

  //makePlots("ModelA","0","./root_files/Consistency/output_ModelA_OLDPAR_SetA_Avg.root");

  //makePlots("ModelA","0","./root_files/Consistency/output_ModelA_OLDPAR_SetB_Avg.root");
  
  //makePlots("ModelA","0","output_ModelA_SetI.root");

  makePlots("ModelA","0","output_ModelA_SetII.root");

  
}

void makePlots( const char * model, const char * src, const char * infile )
{
  
  TFile * f1 = new TFile(infile);
  
  //Output path
  TString path("./paper01-plots/probs/");
  
  TString dataPee  = TString( model ) + TString("_") + TString( src ) + TString("_Pee/data");
  TString dataPem  = TString( model ) + TString("_") + TString( src ) + TString("_Pem/data");
  TString dataPet  = TString( model ) + TString("_") + TString( src ) + TString("_Pet/data");
  TString dataPmt  = TString( model ) + TString("_") + TString( src ) + TString("_Pmt/data");
  TString dataAPee = TString( model ) + TString("_") + TString( src ) + TString("_aPee/data");
  TString dataAPem = TString( model ) + TString("_") + TString( src ) + TString("_aPem/data");
  TString dataAPet = TString( model ) + TString("_") + TString( src ) + TString("_aPet/data");
  TString dataAPmt = TString( model ) + TString("_") + TString( src ) + TString("_aPmt/data");
  
  TList * v_Probs = new TList();
  TObjString *prob;
  prob =  new TObjString( dataPee );
  v_Probs->Add( prob );
  prob =  new TObjString( dataPem );
  v_Probs->Add( prob );
  prob =  new TObjString( dataPet );
  v_Probs->Add( prob );
  prob =  new TObjString( dataPmt );
  v_Probs->Add( prob );
  prob =  new TObjString( dataAPee );
  v_Probs->Add( prob );
  prob =  new TObjString( dataAPem );
  v_Probs->Add( prob );
  prob =  new TObjString( dataAPet );
  v_Probs->Add( prob );
  prob =  new TObjString( dataAPmt );
  v_Probs->Add( prob );
  
  TList * v_Labels = new TList();
  TObjString *label;
  label = new TObjString( "Pee" );
  v_Labels->Add( label ); 
  label = new TObjString( "Pe#mu" );
  v_Labels->Add( label ); 
  label = new TObjString( "Pe#tau" );
  v_Labels->Add( label ); 
  label = new TObjString( "P#mu#tau" );
  v_Labels->Add( label ); 
  
  TList * v_Trees = new TList();
  TList * v_Graphs = new TList();
  
  int max = v_Probs->GetEntries();
  
  for( int k = 0; k < max; ++k ) 
  {
    
    //1.Get Tree
    TString name = ((TObjString*)v_Probs->At(k))->GetString();
    TTree * t1 = (TTree*)f1->Get( name.Data() );
    
    //2.Generate the Graph
     //Branches
    double xx = 0.0;
    double yy = 0.0;
    t1->SetBranchAddress("Ex",&xx);
    t1->SetBranchAddress("Pb",&yy);
    
    Long64_t nentries = t1->GetEntries();
    
    TGraph * g1 = new TGraph();
    
    for (Long64_t i=0;i<nentries;i++) {
      t1->GetEntry(i);
      g1->SetPoint( i, xx, yy);
    }
    
    v_Graphs->Add( g1 );
    
  }
  

  TCanvas * c1 = new TCanvas(model, "Oscillation probabilities", 184, 60, 861, 670);
  c1->Divide(1,4);
    
  TLegend * leg = new TLegend(0.14,0.69,0.24,0.85);
  
  leg->SetBorderSize(0);
  leg->SetTextSize(0.1);
  leg->SetLineColor(1);
  leg->SetLineStyle(1);
  leg->SetLineWidth(1);
  leg->SetFillColor(0);
  leg->SetFillStyle(1001);

  for( int k=0; k < (max/2); ++k) 
  {
    
    TGraph * g1 = (TGraph*)v_Graphs->At(k);
    c1->cd(k+1);
    g1->SetMarkerStyle(1);
    g1->SetFillColor(10);
    
    g1->SetMaximum(1.0);
    g1->SetMinimum(0.0);
    
    TString yaxis = ((TObjString*)v_Labels->At(k))->GetString();
    g1->GetYaxis()->SetTitle( yaxis.Data() );
    g1->GetXaxis()->SetTitle("E [eV]");
    g1->GetYaxis()->CenterTitle(true); 
    g1->GetXaxis()->CenterTitle(true); 
    g1->GetXaxis()->SetLabelOffset(0.007);
    g1->GetXaxis()->SetLabelSize(0.08);
    g1->GetXaxis()->SetTitleSize(0.07);
    g1->GetXaxis()->SetTitleOffset(0.9);
    g1->GetXaxis()->SetLabelFont(42);
    g1->GetYaxis()->SetLabelOffset(0.007);
    g1->GetYaxis()->SetLabelSize(0.08);
    g1->GetYaxis()->SetLabelFont(42);
    g1->GetYaxis()->SetTitleSize(0.09);
    g1->GetYaxis()->SetTitleOffset(0.45);
    g1->GetYaxis()->SetTitleFont(42);
    g1->GetYaxis()->SetNdivisions(505);

    if ( std::string(model).compare("EarthB") == 0 ) 
      g1->GetXaxis()->SetLimits(0.98e9, 1.0e10);

    gPad->SetGridx();
    gPad->SetGridy();
    gPad->SetLogx();
    g1->Draw("APL");
    topTitle(model);

    TGraph * g2 = (TGraph*)v_Graphs->At(k+4);
    
    g2->SetMarkerStyle(1);
    g2->SetMarkerColor(2);
    g2->SetLineColor(2);
    g2->SetFillColor(10);
    g2->SetMaximum(1.0);
    
    if ( std::string(model).compare("EarthB") == 0 ) 
      g2->Draw("PL");
    else
      g2->Draw("P");

    if( k < 1) 
    {
      leg->AddEntry( g1, "#nu");
      leg->AddEntry( g2, "#bar{#nu}");
    }
    
    leg->DrawClone();
    
  }
  
  c1->cd();
  
  std::stringstream saveAs;
  
  saveAs.str("");
  saveAs << path << model << "/pdf/" << "nueosc_probs_" << model << "_4x_SetII" << ".pdf";
  c1->SaveAs( saveAs.str().c_str() );
  
  saveAs.str("");
  saveAs << path << model << "/png/" << "nueosc_probs_" << model << "_4x_SetII" << ".png";
  c1->SaveAs( saveAs.str().c_str() );

  saveAs.str("");
  saveAs << path << model << "/eps/" << "nueosc_probs_" << model << "_4x_SetII" << ".eps";
  c1->SaveAs( saveAs.str().c_str() );
  
    
}

