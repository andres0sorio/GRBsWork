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
  latex.SetTextSize(0.08);
  latex.SetTextAlign(31); // align right
  latex.DrawLatex(0.90,0.92, title);
  //latex.SetTextAlign(11); // align left
  //latex.DrawLatex(0.18,0.92,"Preliminary");
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

  //Model A - Set I
  makePlots("ModelA","0","./root_files/ModelA/output_ModelA_SetI.root","SetI");
  
  //Model A - Set II
  //makePlots("ModelA","0","./root_files/ModelA/output_ModelA_SetII.root", "SetII");

  //Model B - Set I
  //makePlots("ModelB","0","./root_files/ModelB/output_ModelB_SetI.root","SetI");
  
  //Model B - Set II
  //makePlots("ModelB","0","./root_files/ModelB/output_ModelB_SetII.root", "SetII");
  
  //Model C - Set I
  //makePlots("ModelC","0","./root_files/ModelC/output_ModelC_SetI.root","SetI");
  
  //Model C - Set II
  //makePlots("ModelC","0","./root_files/ModelC/output_ModelC_SetII.root", "SetII");

  //Model A - Mena
  //makePlots("ModelA","0","./root_files/Mena/output_EarthB_ModelA.root","Mena");
  
  //Model B - Mena
  //makePlots("ModelB","0","./root_files/Mena/output_EarthB_ModelB.root","Mena");
  
  //Model C - Mena
  //makePlots("ModelC","0","./root_files/Mena/output_EarthB_ModelC.root","Mena");
  
  //ZeroPt - Mena
  //makePlots("ZeroPt","0","./root_files/Mena/output_EarthB_ZeroPt.root","Mena");
  
  //Earth B - Set I
  //makePlots("EarthB","0","./root_files/EarthB/output_EarthB_SetI_Fine.root","SetI");
  
  //Earth B - Set II
  //makePlots("EarthB","0","./root_files/EarthB/output_EarthB_SetII_Fine.root","SetII");
  
  //Earth B - Ohlsson
  //makePlots("EarthB","0","./root_files/EarthB/output_EarthB_Ohlsson.root","Ohl");
  
  //Earth B - Ohlsson - with correction
  //makePlots("EarthB","0","./root_files/EarthB/output_EarthB_Ohlsson-x2.root","OhlX2");
 
  //Fine detail studies 1.e11 to 1.e13
  //makePlots("ModelA","0","./root_files/Mena/output_ModelA_Fine1e13.root","Fine1e13");
  
  //makePlots("ModelB","0","./root_files/Mena/output_ModelB_Fine1e13.root","Fine1e13");
  
   
}

void makePlots( const char * model, const char * src, const char * infile , const char * option)
{
  
  TFile * f1 = new TFile(infile);
  
  //Output path
  TString path("./paper01-plots/probs/");
  
  TString dataPem  = TString( model ) + TString("_") + TString( src ) + TString("_Pem/data");
  TString dataPet  = TString( model ) + TString("_") + TString( src ) + TString("_Pet/data");
  TString dataAPem = TString( model ) + TString("_") + TString( src ) + TString("_aPem/data");
  TString dataAPet = TString( model ) + TString("_") + TString( src ) + TString("_aPet/data");
  
  TList * v_Probs = new TList();
  TObjString *prob;
  prob =  new TObjString( dataPem );
  v_Probs->Add( prob );
  prob =  new TObjString( dataPet );
  v_Probs->Add( prob );
  prob =  new TObjString( dataAPem );
  v_Probs->Add( prob );
  prob =  new TObjString( dataAPet );
  v_Probs->Add( prob );
  
  TList * v_Labels = new TList();
  TObjString *label;
  label = new TObjString( "Pe#mu" );
  v_Labels->Add( label ); 
  label = new TObjString( "Pe#tau" );
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
  

  TCanvas * c1 = new TCanvas(model, "Oscillation probabilities", 184, 60, 861, 305);
  c1->Divide(1,2);
    
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

    TGraph * g2 = (TGraph*)v_Graphs->At(k+2);
    
    g2->SetMarkerStyle(1);
    g2->SetMarkerColor(2);
    g2->SetLineColor(2);
    g2->SetFillColor(10);
    g2->SetMaximum(1.0);
    
    if ( std::string(model).compare("EarthB") == 0 ) 
      g2->Draw("PL");
    else if ( std::string(model).compare("ZeroPt") == 0 ) 
      g2->Draw("P");
    else
      g2->Draw("PL");
        
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
  saveAs << path << model << "/pdf/" << "nueosc_probs_" << model << "_2x_" << option << ".pdf";
  c1->SaveAs( saveAs.str().c_str() );
  
  saveAs.str("");
  saveAs << path << model << "/png/" << "nueosc_probs_" << model << "_2x_" << option << ".png";
  c1->SaveAs( saveAs.str().c_str() );
  
  saveAs.str("");
  saveAs << path << model << "/eps/" << "nueosc_probs_" << model << "_2x_" << option << ".eps";
  c1->SaveAs( saveAs.str().c_str() );
  
    
}

