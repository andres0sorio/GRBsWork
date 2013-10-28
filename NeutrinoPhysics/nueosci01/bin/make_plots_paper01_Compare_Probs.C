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

  makePlots("ZeroPt","ModelA", "SetI", "0", "Pee", "output.root");
   
}

void makePlots(const char * profile,
               const char * model, 
               const char * config,
               const char * src,
               const char * prob, 
               const char * infile)
{
  
  //Output path
  TString path("./paper01-plots/probs/");
  
  TString dataPxx = TString( profile )  + TString("_") + TString( src ) + TString("_") + TString(prob) + ("/data");

  TList * v_Trees = new TList();
  TList * v_Graphs = new TList();
  TList * v_Probs = new TList();

  TObjString *probStr;
  probStr =  new TObjString( dataPxx );
  v_Probs->Add( probStr );
  
  TFile * f1 = new TFile(infile);
  
  f1->cd();

  int max = v_Probs->GetEntries();
  
  std::cout << "max: " << max << std::endl;
    
  for( int k = 0; k < max; ++k ) 
  {
    
    //1.Get Tree
    TString name = ((TObjString*)v_Probs->At(k))->GetString();
    TTree * t1 = (TTree*)f1->Get( name.Data() );
    
    //2.Generate the Graph
    //Branches
    double xx = 0.0;
    double y1 = 0.0;
    double y2 = 0.0;

    t1->SetBranchAddress("Ex" , &xx);
    t1->SetBranchAddress("Pb" , &y1);
    t1->SetBranchAddress("PbV", &y2);
    
    Long64_t nentries = t1->GetEntries();
    
    TGraph * g1 = new TGraph();
    TGraph * g2 = new TGraph();
    
    for (Long64_t i=0;i<nentries;i++) {
      t1->GetEntry(i);
      g1->SetPoint( i, xx, y1);
      g2->SetPoint( i, xx, y2);
    }
    
    v_Graphs->Add( g1 );
    v_Graphs->Add( g2 );
    
  }
  
  TCanvas * c1 = new TCanvas(model, "Oscillation probabilities", 184, 60, 861, 670);
  c1->Divide(1,4); 
  c1->Draw();

  int idx = 1;
  int idxc = 1;
  int nGraphs = v_Graphs->GetSize();
  
  for( int k=0; k < nGraphs; ++k) 
  {
    
    if ( idx >= 3 ) idx = 1;
    
    TGraph * g1 = (TGraph*)v_Graphs->At(k);
    
    g1->SetLineWidth(2); 

    std::cout << idxc << " " << idx << std::endl;

    if ( idx == 1 ) 
    {
      
      c1->cd( idx );
      
      gPad->SetGridx();
      gPad->SetGridy();
      gPad->SetLogx();
      //gPad->SetLogy();
      
      g1->SetMarkerStyle(1);
      g1->SetFillColor(10);
      g1->SetMaximum(1.04);
      g1->SetMinimum(-0.05);
      
      g1->GetYaxis()->SetNdivisions(504);
      //TString yaxis = ((TObjString*)v_Labels->At( idxc-1))->GetString();
      //g1->GetYaxis()->SetTitle( yaxis.Data() );
      g1->GetXaxis()->SetTitle("E_{#nu} (eV)               ");
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
              
    else if ( idx == 2 ) {
      
      g1->SetLineColor( 2 );
      g1->SetLineStyle( 2 );
      
      g1->Draw("L");
      
      
    } else { }
    
    ++idx;
    
    
  }
  
  
  c1->cd();
  
  
  
  
  
  
  std::stringstream saveAs;
  
  saveAs.str("");
  saveAs << path << src << "/pdf/" << "prob_compare_" << src << "_" << prob << "_M1M2" << ".pdf";
  //c1->SaveAs( saveAs.str().c_str() );
  
  saveAs.str("");
  saveAs << path << src << "/png/" << "prob_compare_" << src << "_" << prob << "_M1M2" << ".png";
  //c1->SaveAs( saveAs.str().c_str() );
  
  saveAs.str("");
  saveAs << path << src << "/eps/" << "prob_compare_" << src << "_" << prob << "_M1M2" << ".eps";
  //c1->SaveAs( saveAs.str().c_str() );
  
  
}

