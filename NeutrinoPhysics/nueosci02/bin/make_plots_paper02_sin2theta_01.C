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
  tdrStyle->SetPadLeftMargin(0.14);
  tdrStyle->SetPadRightMargin(0.08);
  tdrStyle->SetPadTopMargin(0.10);
  tdrStyle->SetPadBottomMargin(0.13);
  tdrStyle->SetLegendBorderSize(0);
  tdrStyle->SetTitleYOffset(0.8);
  tdrStyle->SetOptStat(0);
  tdrStyle->SetOptFit(0);
  tdrStyle->SetTitleFontSize(0.08);
  tdrStyle->SetStatStyle(0);
  tdrStyle->cd();

  makePlots("StdPicture", "EarthB", "Vacuum", "detection.root");
  
}

void makePlots( const char * model,
                const char * target, 
                const char * src, 
                const char * infile) 
{
  
  //Output path
  TString path("./paper02-plots/ratio/");
  
  
  TList * v_Variations = new TList();
  TObjString *var;

  var = new TObjString("Sin2Q13-1.8-0");
  v_Variations->Add( var ); 
  var = new TObjString("Sin2Q13-1.8-180");
  v_Variations->Add( var ); 
  
  TList * v_Labels = new TList();
  TObjString *label;
  
  label = new TObjString( model );
  v_Labels->Add( label ); 

  TFile * f1 = new TFile(infile);
  f1->cd();

  TList * v_Graphs = new TList();

  int max = v_Variations->GetEntries();
  
  for( int k = 0; k < max; ++k ) 
  {
    
    TString current = ((TObjString*)v_Variations->At(k))->GetString();

    TString dataPxx = TString( "Ratio_" ) 
      + TString( model )  + TString("_")
      + TString( target ) + TString("_") 
      + TString( src )    + TString("_") 
      + TString( current.Data() )
      + TString("/data");
   
    std::cout << dataPxx << std::endl;
    
 
    TTree * PxxTreeNu = (TTree*)gDirectory->Get( dataPxx.Data() );
    
    //Branches
    double xx = 0.0;
    double yy = 0.0;
    
    PxxTreeNu->SetBranchAddress("Xx",&xx);
    PxxTreeNu->SetBranchAddress("Ratio",&yy);
    
    Long64_t nentries = PxxTreeNu->GetEntries();
    
    TGraph * g1 = new TGraph();
    
    for (Long64_t i=0;i<nentries;i++) {
      PxxTreeNu->GetEntry(i);
      g1->SetPoint( i, xx, yy);
    }
    
    v_Graphs->Add( g1 );
      
  }
  
  TString cname = TString("Ratio") + TString("_") + TString(var);
  
  TCanvas * c1 = new TCanvas( cname.Data(), "track/shower ratio", 206,141,722,575); 
    
  TLegend * leg = new TLegend(0.39,0.71,0.66,0.87);
  
  
  for( int k = 0; k < max; ++k ) 
  {

    TGraph * gg = (TGraph*)v_Graphs->At(k);

    gg->SetMarkerStyle(25);
    gg->SetFillColor(10);
    gg->SetMarkerColor(1);
    gg->SetLineColor(1);
    gg->SetFillColor(10);
    gg->SetMaximum(2.5);
    gg->SetMinimum(1.5);

    gg->GetXaxis()->SetLimits( 0.0, 0.055 );

    gg->GetXaxis()->SetTitle("sin^{2}#theta_{13}");
    gg->GetXaxis()->CenterTitle(true);
    gg->GetXaxis()->SetLabelFont(42);
    gg->GetXaxis()->SetLabelOffset(0.006);
    gg->GetXaxis()->SetLabelSize(0.05);
    gg->GetXaxis()->SetTitleSize(0.05);
    gg->GetXaxis()->SetTickLength(0.05);
    gg->GetXaxis()->SetTitleOffset(1.10);
    gg->GetXaxis()->SetTitleFont(42);
    gg->GetXaxis()->SetNdivisions(509);
    gg->GetYaxis()->SetTitle("R");
    gg->GetYaxis()->CenterTitle(true);
    gg->GetYaxis()->SetNdivisions(509);
    gg->GetYaxis()->SetLabelFont(42);
    gg->GetYaxis()->SetLabelOffset(0.007);
    gg->GetYaxis()->SetLabelSize(0.05);
    gg->GetYaxis()->SetTitleSize(0.05);
    gg->GetYaxis()->SetTitleOffset(0.93);
    gg->GetYaxis()->SetTitleFont(42);

    gg->SetLineWidth(2);

    //leg->AddEntry( gg, "#nu");
    //leg->SetBorderSize(0);
    //leg->SetTextSize(0.1);
    //leg->SetLineColor(1);
    //leg->SetLineStyle(1);
    //leg->SetLineWidth(1);
    //leg->SetFillColor(0);
    //leg->SetFillStyle(1001);

    c1->cd();

    if( k == 0 )
      gg->Draw("AC");
    else
      gg->Draw("C");

    //topTitle(var);
  
    //leg->DrawClone();
  
  }
  


  c1->cd();
  
  std::stringstream saveAs;
  
  saveAs.str("");
  saveAs << path << model << "/pdf/" << "ratio_" << target << "_Sin2Q13" << ".pdf";
  c1->SaveAs( saveAs.str().c_str() );
  
  saveAs.str("");
  saveAs << path << model << "/png/" << "ratio_" << target << "_Sin2Q13" << ".png";
  c1->SaveAs( saveAs.str().c_str() );

  saveAs.str("");
  saveAs << path << model << "/eps/" << "ratio_" << target << "_Sin2Q13" << ".eps";
  c1->SaveAs( saveAs.str().c_str() );
  
  
}

