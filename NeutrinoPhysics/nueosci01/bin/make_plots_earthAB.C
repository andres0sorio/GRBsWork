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

  makePlots("./root_files/Earth/output-earthA-x2.root",
            "./root_files/Earth/output-earthB-x2.root",
            "X2");
    
}

void makePlots( const char * inputA, const char * inputB, const char * option) 
{
  
  //Output path
  TString path("./paper01-plots/probs/");
  
  TString dataPxxA = TString( "EarthA" ) + TString("_") + TString( "0" ) + TString("_") + TString("Pme") +  TString("/data");
  TString dataPxxB = TString( "EarthB" ) + TString("_") + TString( "0" ) + TString("_") + TString("Pme") +  TString("/data");
  
  TList * v_Labels = new TList();
  TObjString *label;
  label = new TObjString( "Earth: Step function" );
  v_Labels->Add( label ); 
  label = new TObjString( "Earth: Realistic" );
  v_Labels->Add( label ); 
  
  TFile * f1 = new TFile(inputA);
  TFile * f2 = new TFile(inputB);
  
  f1->cd();
  TTree * PxxATreeNu = (TTree*)gDirectory->Get( dataPxxA.Data() );
    
  f2->cd();
  TTree * PxxBTreeNu = (TTree*)gDirectory->Get( dataPxxB.Data() );
  
  //Branches
  double xx = 0.0;
  double yy = 0.0;
  
  TString cname = TString("Earth") + TString("_") + TString("Pee");
  
  TGraph * ProbNu[3];
  ProbNu[0] = new TGraph();
  ProbNu[1] = new TGraph();
  
  PxxATreeNu->SetBranchAddress("Ex",&xx);
  PxxATreeNu->SetBranchAddress("Pb",&yy);
  
  Long64_t nentries = PxxATreeNu->GetEntries();
  
  for (Long64_t i=0;i<nentries;i++) {
    PxxATreeNu->GetEntry(i);
    ProbNu[0]->SetPoint( i, xx, yy);
    if( i < 5 ) 
      std::cout << i << " " << xx << " " << yy << std::endl;
  }
  
  PxxBTreeNu->SetBranchAddress("Ex",&xx);
  PxxBTreeNu->SetBranchAddress("Pb",&yy);
  
  nentries = PxxBTreeNu->GetEntries();
  
  for (Long64_t i=0;i<nentries;i++) {
    PxxBTreeNu->GetEntry(i);
    ProbNu[1]->SetPoint( i, xx, yy);
  }
  
  int ndataset = 0;
  TList * allgraphs = new TList(); //this is a ROOT container. It will store all of your Graphs
  
  allgraphs->Add( ProbNu[0] ); 
  ndataset++;

  allgraphs->Add( ProbNu[1] ); 
  ndataset++;
  
  //Datasets options (Markers: style, color, size) : You can also do it by hand using the interactive Editor
  int style[5];
  int color[5];
  float size[5];
  
  //For dataset No1
  style[0] = 21;
  color[0] = 2;
  size[0]  = 0.7;
  
  //For dataset No2
  style[1] = 20;
  color[1] = 4;
  size[1]  = 0.8;
  
  //For dataset No3
  style[2] = 22;
  color[2] = 38;
  size[2]  = 0.8;

  TCanvas * c1 = new TCanvas( cname.Data(), "Oscillation probabilities", 184,60,861,263); 
  c1->cd();
  gPad->SetGridx();
  gPad->SetGridy();
  gPad->SetLogx();

  float ymin = 0.0;
  float ymax = 0.5;
  float xmin = 0.9e9;
  float xmax = 1.0e10;

  TLegend * leg = new TLegend(0.14,0.58,0.35,0.85);
    
  leg->SetBorderSize(0);
  leg->SetTextSize(0.1);
  leg->SetLineColor(1);
  leg->SetLineStyle(1);
  leg->SetLineWidth(1);
  leg->SetFillColor(0);
  leg->SetFillStyle(1001);
  leg->SetTextFont(62);
  leg->SetTextSize(0.085);

  ndataset = allgraphs->GetSize(); //Get the ndatasets from the size of the List

  // Loop now over the List using the index k
  
  for(int k=0; k < ndataset; ++k) {
    
    if( k == 0 ) {
      //this is our first graph and it is special (to define axis min,max)
      ((TGraph*)allgraphs->At(k))->SetMinimum(ymin);
      ((TGraph*)allgraphs->At(k))->SetMaximum(ymax);
      ((TGraph*)allgraphs->At(k))->Draw("AP");
      ((TGraph*)allgraphs->At(k))->GetXaxis()->SetLimits(xmin, xmax);
      //set the color options
      ((TGraph*)allgraphs->At(k))->SetMarkerStyle( style[k] );
      ((TGraph*)allgraphs->At(k))->SetMarkerSize( size[k] );
      ((TGraph*)allgraphs->At(k))->SetMarkerColor( color[k] );
      ((TGraph*)allgraphs->At(k))->SetLineColor( color[k] );
      ((TGraph*)allgraphs->At(k))->SetFillColor( 10 );
      ((TGraph*)allgraphs->At(k))->GetYaxis()->SetNdivisions(505);
      ((TGraph*)allgraphs->At(k))->GetXaxis()->SetTitle("E [eV]");
      ((TGraph*)allgraphs->At(k))->GetYaxis()->SetTitle("P_{#mue}");

      ((TGraph*)allgraphs->At(k))->GetYaxis()->CenterTitle(true); 
      ((TGraph*)allgraphs->At(k))->GetXaxis()->CenterTitle(true); 
      ((TGraph*)allgraphs->At(k))->GetXaxis()->SetLabelOffset(0.007);
      ((TGraph*)allgraphs->At(k))->GetXaxis()->SetLabelSize(0.08);
      ((TGraph*)allgraphs->At(k))->GetXaxis()->SetTitleSize(0.07);
      ((TGraph*)allgraphs->At(k))->GetXaxis()->SetTitleOffset(0.9);
      ((TGraph*)allgraphs->At(k))->GetXaxis()->SetLabelFont(42);
      ((TGraph*)allgraphs->At(k))->GetYaxis()->SetLabelOffset(0.007);
      ((TGraph*)allgraphs->At(k))->GetYaxis()->SetLabelSize(0.08);
      ((TGraph*)allgraphs->At(k))->GetYaxis()->SetLabelFont(42);
      ((TGraph*)allgraphs->At(k))->GetYaxis()->SetTitleSize(0.09);
      ((TGraph*)allgraphs->At(k))->GetYaxis()->SetTitleOffset(0.45);
      ((TGraph*)allgraphs->At(k))->GetYaxis()->SetTitleFont(42);
      
      ((TGraph*)allgraphs->At(k))->Draw("APL"); // Draw option AP A=draw axis P=draw a marker for the data
      
      TString name = ((TObjString*)v_Labels->At(k))->GetString();
      leg->AddEntry( (TGraph*)allgraphs->At(k), name.Data());
      
      
    }
    else {
      ((TGraph*)allgraphs->At(k))->SetMarkerStyle( style[k] );
      ((TGraph*)allgraphs->At(k))->SetMarkerSize( size[k] );
      ((TGraph*)allgraphs->At(k))->SetMarkerColor( color[k] );
      ((TGraph*)allgraphs->At(k))->SetLineColor( color[k] );
      ((TGraph*)allgraphs->At(k))->SetFillColor( 10 );

      ((TGraph*)allgraphs->At(k))->Draw("PL"); // since we have already plotted the axis on the first graph we only need option P
      
      TString name = ((TObjString*)v_Labels->At(k))->GetString();
      leg->AddEntry( (TGraph*)allgraphs->At(k), name.Data());
    
    }
    
    
  }
  
  topTitle("Earth (corrected)");
  
  /// All done!
  leg->Draw();
  std::stringstream saveAs;
    
  saveAs.str("");
  saveAs << path << "EarthA" << "/pdf/" << "nueosc_earth_AB_" << option << ".pdf";
  c1->SaveAs( saveAs.str().c_str() );
  
  saveAs.str("");
  saveAs << path << "EarthA" << "/png/" << "nueosc_earth_AB_" << option << ".png";
  c1->SaveAs( saveAs.str().c_str() );
  
  saveAs.str("");
  saveAs << path << "EarthA" << "/eps/" << "nueosc_earth_AB_" << option << ".eps";
  c1->SaveAs( saveAs.str().c_str() );
    
  
  
}

