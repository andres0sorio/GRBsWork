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
  
  var = new TObjString("Sin2Q13-2-0");
  v_Variations->Add( var ); 
  var = new TObjString("Sin2Q13-2-180");
  v_Variations->Add( var );

  var = new TObjString("Sin2Q13-2.2-0");
  v_Variations->Add( var ); 
  var = new TObjString("Sin2Q13-2.2-180");
  v_Variations->Add( var );

  int * linewidth = new int[6];
  int * linestyle = new int[6];
  int * linecolor = new int[6];
  
  linewidth[0] = 1;
  linewidth[1] = 2;
  linewidth[2] = 1;
  linewidth[3] = 2;
  linewidth[4] = 1;
  linewidth[5] = 2;

  linecolor[0] = 2;
  linecolor[1] = 1;
  linecolor[2] = 2;
  linecolor[3] = 1;
  linecolor[4] = 2;
  linecolor[5] = 1;

  linestyle[0] = 1;
  linestyle[1] = 1;
  linestyle[2] = 2;
  linestyle[3] = 2;
  linestyle[4] = 3;
  linestyle[5] = 3;

  TList * v_Labels = new TList();
  TObjString *label;
  
  label = new TObjString( "#alpha = 1.8" );
  v_Labels->Add( label ); 
  label = new TObjString( "#alpha = 2.0" );
  v_Labels->Add( label ); 
  label = new TObjString( "#alpha = 2.2" );
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
    
  TLegend * leg = new TLegend(0.18,0.71,0.44,0.87);
  leg->SetBorderSize(0);
  leg->SetTextFont(22);
  leg->SetTextSize(0.064);
  leg->SetLineColor(1);
  leg->SetLineStyle(1);
  leg->SetLineWidth(1);
  leg->SetFillColor(0);
  leg->SetFillStyle(1001);

  int labelpos = 0;
  
  for( int k = 0; k < max; ++k ) 
  {

    TGraph * gg = (TGraph*)v_Graphs->At(k);

    gg->SetMarkerStyle(25);
    gg->SetFillColor(10);

    gg->SetLineColor(linecolor[k]);
    gg->SetLineWidth(linewidth[k]);
    gg->SetLineStyle(linestyle[k]);

    gg->SetMaximum(3.1);
    gg->SetMinimum(1.8);

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

    if ( ((k+1) % 2) == 0 )
    {
      TString alpha = ((TObjString*)v_Labels->At(labelpos))->GetString();      
      leg->AddEntry( gg, alpha.Data(),"l");
      labelpos+=1;
    }
    
    c1->cd();

    if( k == 0 )
      gg->Draw("AC");
    else
      gg->Draw("C");

    
  
  }
  
  leg->Draw();

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

