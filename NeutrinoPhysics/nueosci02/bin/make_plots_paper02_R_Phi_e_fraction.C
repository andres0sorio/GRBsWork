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

TGraph * readTypeFour( const char * fname) 
{

  ifstream in;
  in.open(fname); //
  
  double en, flux;
  
  TGraph * graphT4 = new TGraph(); 

  int i = 0;
  
  while ( in.good() ) 
  {
    
    in >> en >> flux; //we read 2 columns of data
    
    graphT4->SetPoint( i , en, flux);
    
    ++i;
    
  }

  std::cout << "Total points read: " << i << std::endl; 
    
  in.close();
  
  return graphT4;
  
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

  ///
  //... R (ratio muon+tau / showers) vs fraction of phi_e
  
  makePlots("StdPicture", "EarthB", "Vacuum", "phi_nue", "root_files/StdPicture/detection-Rphiefrac.root");
  
}

void makePlots( const char * model,
                const char * target, 
                const char * src, 
                const char * var, 
                const char * infile) 
{
  
  //Output path
  TString path("./paper02-plots/ratio/");
  
  TString dataPxx = TString( "Ratio_" ) 
    + TString( model )  + TString("_")
    + TString( target ) + TString("_") 
    + TString( src )    + TString("_") 
    + TString( var)     +  TString("/data");
   
  TList * v_Labels = new TList();
  TObjString *label;
  label = new TObjString( model );
  v_Labels->Add( label ); 
      
  TFile * f1 = new TFile(infile);
  
  f1->cd();
  
  TTree * PxxTreeNu = (TTree*)gDirectory->Get( dataPxx.Data() );
    
  //Branches
  double xx = 0.0;
  double yy = 0.0;
  
  double y1 = 0.0;
  double y2 = 0.0;
    
  TString cname = TString("Ratio") + TString("_") + TString(var);
  
  TCanvas * c1 = new TCanvas( cname.Data(), "track/shower ratio", 206,141,722,575); 
    
  TGraph * ProbNu[3];
  ProbNu[0] = new TGraph();
  ProbNu[1] = new TGraph();
  
  TLegend * leg = new TLegend(0.39,0.71,0.66,0.87);

  leg->SetBorderSize(0);
  leg->SetTextSize(0.05);
  leg->SetLineColor(1);
  leg->SetLineStyle(1);
  leg->SetLineWidth(1);
  leg->SetFillColor(0);
  leg->SetFillStyle(1001);

  PxxTreeNu->SetBranchAddress("Xx",&xx);
  PxxTreeNu->SetBranchAddress("Ratio",&yy);
  PxxTreeNu->SetBranchAddress("MuTks",&y1);
  PxxTreeNu->SetBranchAddress("HadShw",&y2);
  
  Long64_t nentries = PxxTreeNu->GetEntries();
  
  for (Long64_t i=0;i<nentries;i++) {
    PxxTreeNu->GetEntry(i);
    ProbNu[0]->SetPoint( i, xx, yy);
    ProbNu[1]->SetPoint( i, xx, (y1/y2) );
  }

  ProbNu[0]->SetMarkerStyle(25);
  ProbNu[0]->SetFillColor(10);
  ProbNu[0]->SetMarkerColor(1);
  ProbNu[0]->SetLineColor(1);
  ProbNu[0]->SetFillColor(10);
  ProbNu[0]->SetMaximum(8.0);
  ProbNu[0]->SetMinimum(0.0);

  ProbNu[0]->GetXaxis()->SetLimits( 0.0, 1.0 );

  ProbNu[0]->GetXaxis()->SetTitle("#phi_{#nu_e} fraction");
  ProbNu[0]->GetXaxis()->CenterTitle(true);
  ProbNu[0]->GetXaxis()->SetLabelFont(42);
  ProbNu[0]->GetXaxis()->SetLabelOffset(0.006);
  ProbNu[0]->GetXaxis()->SetLabelSize(0.05);
  ProbNu[0]->GetXaxis()->SetTitleSize(0.05);
  ProbNu[0]->GetXaxis()->SetTickLength(0.05);
  ProbNu[0]->GetXaxis()->SetTitleOffset(1.10);
  ProbNu[0]->GetXaxis()->SetTitleFont(42);
  ProbNu[0]->GetYaxis()->SetTitle("R");
  ProbNu[0]->GetYaxis()->CenterTitle(true);
  ProbNu[0]->GetYaxis()->SetNdivisions(509);
  ProbNu[0]->GetYaxis()->SetLabelFont(42);
  ProbNu[0]->GetYaxis()->SetLabelOffset(0.007);
  ProbNu[0]->GetYaxis()->SetLabelSize(0.05);
  ProbNu[0]->GetYaxis()->SetTitleSize(0.05);
  ProbNu[0]->GetYaxis()->SetTitleOffset(0.93);
  ProbNu[0]->GetYaxis()->SetTitleFont(42);

  ProbNu[0]->SetLineWidth(2);

  leg->AddEntry( ProbNu[0], "#mu + #tau tracks","L");
  leg->AddEntry( ProbNu[1], "#mu tracks","L");

  c1->cd();

  ProbNu[0]->Draw("AC");
  ProbNu[1]->Draw("C");
  
  //

  if( 1 ) {
    
    TGraph * graphT4 = readTypeFour("../data/beacom-fig-3-100GeVmuTh-R-phie_frac.csv");
    
    graphT4->SetMarkerColor(2);
    graphT4->SetLineColor(2);
    graphT4->SetLineWidth(2);
    
    graphT4->Draw("L");
    
    leg->AddEntry(graphT4, "Beacom [*]","L");
    
  }
  
  leg->Draw();
    
  c1->cd();
  
  std::stringstream saveAs;
  
  saveAs.str("");
  saveAs << path << model << "/pdf/" << "ratio_" << target << "_" << var << "_fraction" << ".pdf";
  c1->SaveAs( saveAs.str().c_str() );
  
  saveAs.str("");
  saveAs << path << model << "/png/" << "ratio_" << target << "_" << var << "_fraction" << ".png";
  c1->SaveAs( saveAs.str().c_str() );

  saveAs.str("");
  saveAs << path << model << "/eps/" << "ratio_" << target << "_" << var << "_fraction" << ".eps";
  c1->SaveAs( saveAs.str().c_str() );
  
  
}

