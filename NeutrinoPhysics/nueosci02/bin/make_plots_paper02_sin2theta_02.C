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
  
  double xx, yy;
  
  TGraph * graphT4 = new TGraph(); 

  int i = 0;
  
  while ( in.good() ) 
  {
    
    in >> xx >> yy; //we read 2 columns of data
    
    graphT4->SetPoint( i , xx, yy);
    
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
  tdrStyle->SetPadBottomMargin(0.14);
  tdrStyle->SetLegendBorderSize(0);
  tdrStyle->SetTitleYOffset(0.8);
  tdrStyle->SetOptStat(0);
  tdrStyle->SetOptFit(0);
  tdrStyle->SetTitleFontSize(0.08);
  tdrStyle->SetStatStyle(0);
  tdrStyle->SetStatStyle(0);
  tdrStyle->SetFrameLineWidth(2);
  tdrStyle->cd();

  bool plotSetII = true;
  
  TString inputFile_SetI("detection_Models_SetI_RvsSin2Q13.root");

  TString inputFile_SetII("detection_Models_SetII_RvsSin2Q13.root");

  TString inputFile_ModelA_SetI("detection_ModelA_SetI_RvsSin2Q13.root");

  TString inputFile_ModelA_SetII("detection_ModelA_SetII_RvsSin2Q13.root");
  
  
  if( 0 ) { 
    makePlots("ModelA", "EarthB", "Vacuum", "SetI", inputFile_SetI.Data() );
  }

  //... AO: Replacement due to bump

  makePlots("ModelA", "EarthB", "Vacuum", "SetI", inputFile_ModelA_SetI.Data() );

  makePlots("ModelB", "EarthB", "Vacuum", "SetI", inputFile_SetI.Data() );
  
  makePlots("ModelC", "EarthB", "Vacuum", "SetI", inputFile_SetI.Data() );
  
  if( plotSetII ) 
  {
    
    if( 0 ) {
      makePlots("ModelA", "EarthB", "Vacuum", "SetII", inputFile_SetII.Data() );
    }

    //... AO: Replacement due to bump

    makePlots("ModelA", "EarthB", "Vacuum", "SetII", inputFile_ModelA_SetII.Data() );

    makePlots("ModelB", "EarthB", "Vacuum", "SetII", inputFile_SetII.Data() );
    
    makePlots("ModelC", "EarthB", "Vacuum", "SetII", inputFile_SetII.Data() );

  }
  
  makePlots("StdPicture", "EarthB", "Vacuum", "SetI", "detection_StdPicture_SetI_RvsSin2Q13.root");

  
}

void makePlots( const char * model,
                const char * target, 
                const char * src, 
                const char * config,
                const char * infile) 
{

  double MAXY = 2.4;
  
  double x_Q13_SetI = sin(8.8*TMath::Pi()/180.0)*sin(8.8*TMath::Pi()/180.0);
  
  double x_Q13_SetII = sin(12.0*TMath::Pi()/180.0)*sin(12.0*TMath::Pi()/180.0);

  //Input path
  TString inpath("./root_files/RvsQ13/");
  
  TString inputfile = inpath + TString(infile);

  //Output path
  TString path("./paper02-plots/ratio/");
    
  TList * v_Variations = new TList();
  TObjString *var;

  var = new TObjString("Sin2Q13-1.8-dCP0");
  v_Variations->Add( var ); 
  var = new TObjString("Sin2Q13-1.8-dCP180");
  v_Variations->Add( var );

  if ( TString(model) != TString("StdPicture") ) {
    
    var = new TObjString("Sin2Q13-2.0-dCP0");
    v_Variations->Add( var ); 
    var = new TObjString("Sin2Q13-2.0-dCP180");
    v_Variations->Add( var );

  } else {

    var = new TObjString("Sin2Q13-2-dCP0");
    v_Variations->Add( var ); 
    var = new TObjString("Sin2Q13-2-dCP180");
    v_Variations->Add( var );  
  
  }
  
  var = new TObjString("Sin2Q13-2.2-dCP0");
  v_Variations->Add( var ); 
  var = new TObjString("Sin2Q13-2.2-dCP180");
  v_Variations->Add( var );
  
  int * linewidth = new int[6];
  int * linestyle = new int[6];
  int * linecolor = new int[6];
  
  linewidth[0] = 2;
  linewidth[1] = 3;
  linewidth[2] = 2;
  linewidth[3] = 3;
  linewidth[4] = 2;
  linewidth[5] = 3;

  linecolor[0] = 1;
  linecolor[1] = 2;
  linecolor[2] = 1;
  linecolor[3] = 2;
  linecolor[4] = 1;
  linecolor[5] = 2;

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
  
  TFile * f1 = new TFile( inputfile.Data() );
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
  
  TString cname = TString("Ratio") + TString("_") + TString(model) +  TString("_") + TString(config);
  
  TCanvas * c1 = new TCanvas( cname.Data(), "track/shower ratio", 206,141,722,575); 

  c1->SetBorderSize(2);
    
  TLegend * leg = new TLegend(0.18,0.64,0.44,0.87);
  
  leg->SetBorderSize(0);
  leg->SetTextFont(22);
  leg->SetTextSize(0.062);
  leg->SetLineColor(1);
  leg->SetLineStyle(1);
  leg->SetLineWidth(1);
  leg->SetFillColor(0);
  leg->SetFillStyle(0);

  int labelpos = 0;
  
  for( int k = 0; k < max; ++k ) 
  {

    TGraph * gg = (TGraph*)v_Graphs->At(k);

    gg->SetMarkerStyle(25);
    gg->SetFillColor(10);

    gg->SetLineColor(linecolor[k]);
    gg->SetLineWidth(linewidth[k]);
    gg->SetLineStyle(linestyle[k]);

    gg->SetMaximum(MAXY);
    gg->SetMinimum(1.6);

    gg->GetXaxis()->SetLimits( 0.0, 0.055 );

    gg->GetXaxis()->SetTitle("sin^{2}#theta_{13}");
    gg->GetXaxis()->CenterTitle(true);
    gg->GetXaxis()->SetLabelFont(42);
    gg->GetXaxis()->SetLabelOffset(0.006);
    gg->GetXaxis()->SetLabelSize(0.06);
    gg->GetXaxis()->SetTitleSize(0.06);
    gg->GetXaxis()->SetTickLength(0.05);
    gg->GetXaxis()->SetTitleOffset(1.07);
    gg->GetXaxis()->SetTitleFont(42);
    gg->GetXaxis()->SetNdivisions(509);
    gg->GetYaxis()->SetTitle("R");
    gg->GetYaxis()->CenterTitle(true);
    gg->GetYaxis()->SetNdivisions(509);
    gg->GetYaxis()->SetLabelFont(42);
    gg->GetYaxis()->SetLabelOffset(0.007);
    gg->GetYaxis()->SetLabelSize(0.06);
    gg->GetYaxis()->SetTitleSize(0.06);
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

    TString ThisModel;
    TString ThisConfig;
    
    if( TString(model) == TString("StdPicture") )
      ThisModel = TString("No matter effect");
    else 
    {
      ThisModel = TString(model);
      ThisModel.Insert(5," ");
    }

    ThisConfig = TString(config);
    ThisConfig.Insert(3," ");
      
    TLatex *   tex = new TLatex(0.033, (MAXY-(MAXY*0.035)), ThisModel.Data() );
    tex->SetLineWidth(2);
    tex->Draw();

    if ( TString(model) != TString("StdPicture") ) {
      tex = new TLatex(0.033, (MAXY-(MAXY*0.055)), ThisConfig.Data() );
      tex->SetLineWidth(2);
      tex->Draw();
    }
      
  
  }
  
  leg->Draw();

  
  
  double y_min = 1.60;
  double y_max = MAXY;
  
  TLine *line = new TLine(x_Q13_SetI, y_min,x_Q13_SetI, y_max);
  //line->Draw();
  line = new TLine(x_Q13_SetII, y_min,x_Q13_SetII, y_max);
  //line->Draw();

  c1->cd();
  
  std::stringstream saveAs;
  
  saveAs.str("");
  saveAs << path << model << "/pdf/" << "RvsSin2Q13_" << model << "_" << target << "_" << config << ".pdf";
  c1->SaveAs( saveAs.str().c_str() );
  
  saveAs.str("");
  saveAs << path << model << "/png/" << "RvsSin2Q13_" << model << "_" << target << "_" << config << ".png";
  c1->SaveAs( saveAs.str().c_str() );

  saveAs.str("");
  saveAs << path << model << "/eps/" << "RvsSin2Q13_" << model<< "_" <<  target << "_" << config << ".eps";
  c1->SaveAs( saveAs.str().c_str() );
  
  
}

