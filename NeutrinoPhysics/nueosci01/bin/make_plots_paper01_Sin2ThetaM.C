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
  tdrStyle->SetPadLeftMargin(0.15);
  tdrStyle->SetPadRightMargin(0.10);
  tdrStyle->SetPadTopMargin(0.10);
  tdrStyle->SetLegendBorderSize(0);
  tdrStyle->SetTitleYOffset(0.8);
  tdrStyle->SetOptStat(0);
  tdrStyle->SetOptFit(0);
  tdrStyle->SetTitleFontSize(0.08);
  tdrStyle->SetStatStyle(0);
  tdrStyle->cd();

  
  //... 

  makePlots("LinearFig2","0", "Fig2","root_files/Lambdas/output-Fig2.root");

  makePlots("LinearFig4","0", "Fig4","root_files/Lambdas/output-Fig4.root");

  
}

void makePlots( const char * model, const char * src, const char * config, const char * infile) 
{
  
  //Output path
  TString path("./paper01-plots/probs/");
  
  TString dataPxx = TString( model ) + TString("_") + TString( src ) + TString("_") + TString(config) +  TString("/data");
  
  TList * v_Labels = new TList();
  TObjString *label;
  label = new TObjString( "sin^{2} 2#theta_{1}^{M}" );
  v_Labels->Add( label ); 
  label = new TObjString( "sin^{2} 2#theta_{2}^{M}" );
  v_Labels->Add( label ); 
  label = new TObjString( "sin^{2} 2#theta_{3}^{M}" );
  v_Labels->Add( label ); 
    
  TFile * f1 = new TFile(infile);
  
  f1->cd();
  
  TTree * InputTree = (TTree*)gDirectory->Get( dataPxx.Data() );
    
  //Branches
  double Ex  = 0.0;
  double Ax  = 0.0;
  double theta_1M = 0.0;
  double theta_2M = 0.0;
  double theta_3M = 0.0;
  
  TString cname = TString(model) + TString("_") + TString(config);
  TCanvas * c1 = new TCanvas( cname.Data(), "Resonance studies", 101,127,676,562);
 
  
  TLegend * leg = new TLegend(0.59,0.70,0.80,0.86);
  leg->SetBorderSize(1);
  leg->SetMargin(0.30);
  leg->SetTextFont(42);
  leg->SetTextSize(0.04);
  leg->SetLineColor(1);
  leg->SetLineStyle(1);
  leg->SetLineWidth(1);
  leg->SetFillColor(0);
  leg->SetFillStyle(1001);
    
  TGraph * LambdaDiff[3];
  LambdaDiff[0] = new TGraph();
  LambdaDiff[1] = new TGraph();
  LambdaDiff[2] = new TGraph();
  
  int ndataset = 0;
  TList * allgraphs = new TList(); //this is a ROOT container. It will store all of your Graphs
  
  allgraphs->Add( LambdaDiff[0] ); 
  ndataset++;
  
  allgraphs->Add( LambdaDiff[1] ); 
  ndataset++;
  
  allgraphs->Add( LambdaDiff[2] ); 
  ndataset++;
  
  InputTree->SetBranchAddress("Ex", &Ex);
  InputTree->SetBranchAddress("Ax", &Ax);
  InputTree->SetBranchAddress("theta_1M",&theta_1M);
  InputTree->SetBranchAddress("theta_2M",&theta_2M);
  InputTree->SetBranchAddress("theta_3M",&theta_3M);
  
  Long64_t nentries = InputTree->GetEntries();
  
  double factor =  1.0e14;
  double xmin   =  1.0e-14 * factor;
  double xmax   =  1.0e-13 * factor;
  double ymin   =  0.5e-14 * factor;
  double ymax   = 12.0e-14 * factor;
  
  TString xLabel;
  TString yLabel;
  
  if ( std::string(config).compare("Fig2") == 0) 
  {

    factor =  1.0;
    xmin   =  1.0e-16;
    xmax   =  1.0e-10;
    ymin   =  0.0;
    ymax   =  1.0;

    c1->SetLogx();

    xLabel = TString("A (eV)");
    yLabel = TString("sin^{2}2#theta^{M}_{i}");
    
  } else if ( std::string(config).compare("Fig4") == 0) 
  {

    factor =  1.0;
    xmin   =  1.0e-22;
    xmax   =  1.0e-13;
    ymin   =  0.0;
    ymax   =  1.0;

    xLabel = TString("A (eV)");
    yLabel = TString("sin^{2}2#theta^{M}_{i}");

    c1->SetLogx();

  } else {
    
    factor =  1.0;
    xmin   =  1.0e-24 * factor;
    xmax   =  1.0e-13 * factor;
    ymin   =  0.0;
    ymax   =  1.0;
        
    xLabel = TString("A (eV)");
    yLabel = TString("sin^{2}2#theta^{M}_{i}");
    
    c1->SetLogx();

  }

   
  for (Long64_t i=0;i<nentries;i++) {
    InputTree->GetEntry(i);

    double sintheta_1 = pow( sin(2.0*theta_1M), 2.0);
    double sintheta_2 = pow( sin(2.0*theta_2M), 2.0);
    double sintheta_3 = pow( sin(2.0*theta_3M), 2.0);
    
    LambdaDiff[0]->SetPoint( i, Ax*factor, sintheta_1);
    LambdaDiff[1]->SetPoint( i, Ax*factor, sintheta_2);
    LambdaDiff[2]->SetPoint( i, Ax*factor, sintheta_3);

  }

  //Datasets options (Markers: style, color, size) : You can also do it by hand using the interactive Editor
  int style[5];
  int color[5];
  int line[5];
  float size[5];
  
  
  //For dataset No1
  style[0] = 7;
  color[0] = 1;
  size[0]  = 0.7;
  line[0]  = 1;
  
  //For dataset No2
  style[1] = 7;
  color[1] = 1;
  size[1]  = 0.8;
  line[1]  = 3;
  
  //For dataset No3
  style[2] = 7;
  color[2] = 1;
  size[2]  = 0.8;
  line[2]  = 2;
    
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
      ((TGraph*)allgraphs->At(k))->SetLineStyle( line[k] );

      ((TGraph*)allgraphs->At(k))->SetFillColor( 10 );
      ((TGraph*)allgraphs->At(k))->GetXaxis()->SetTitle(xLabel.Data());
      ((TGraph*)allgraphs->At(k))->GetYaxis()->SetTitle(yLabel.Data());

      ((TGraph*)allgraphs->At(k))->GetYaxis()->CenterTitle(true); 
      ((TGraph*)allgraphs->At(k))->GetXaxis()->CenterTitle(true); 
      ((TGraph*)allgraphs->At(k))->GetXaxis()->SetLabelOffset(0.007);
      ((TGraph*)allgraphs->At(k))->GetXaxis()->SetLabelSize(0.04);
      ((TGraph*)allgraphs->At(k))->GetXaxis()->SetTitleSize(0.05);
      ((TGraph*)allgraphs->At(k))->GetXaxis()->SetTitleOffset(1.20);
      ((TGraph*)allgraphs->At(k))->GetXaxis()->SetLabelFont(42);
    
      ((TGraph*)allgraphs->At(k))->GetYaxis()->SetLabelOffset(0.007);
      ((TGraph*)allgraphs->At(k))->GetYaxis()->SetLabelSize(0.04);
      ((TGraph*)allgraphs->At(k))->GetYaxis()->SetTitleSize(0.05);
      ((TGraph*)allgraphs->At(k))->GetYaxis()->SetTitleOffset(1.45);
      ((TGraph*)allgraphs->At(k))->GetYaxis()->SetTitleFont(42);
      ((TGraph*)allgraphs->At(k))->GetYaxis()->SetLabelFont(42);

      ((TGraph*)allgraphs->At(k))->Draw("AL"); // Draw option AP A=draw axis P=draw a marker for the data
      
      TString name = ((TObjString*)v_Labels->At(k))->GetString();
      leg->AddEntry( (TGraph*)allgraphs->At(k), name.Data(),"L");
          
    }
    
    else {
      ((TGraph*)allgraphs->At(k))->SetMarkerStyle( style[k] );
      ((TGraph*)allgraphs->At(k))->SetMarkerSize( size[k] );
      ((TGraph*)allgraphs->At(k))->SetMarkerColor( color[k] );
      ((TGraph*)allgraphs->At(k))->SetLineColor( color[k] );
      ((TGraph*)allgraphs->At(k))->SetFillColor( 10 );
      ((TGraph*)allgraphs->At(k))->SetLineStyle( line[k] );

      ((TGraph*)allgraphs->At(k))->Draw("L"); // since we have already plotted the axis on the first graph we only need option P
      
      TString name = ((TObjString*)v_Labels->At(k))->GetString();
      leg->AddEntry( (TGraph*)allgraphs->At(k), name.Data(),"L");
      
    }
    
    
  }
  
  leg->Draw();
  
  c1->cd();
  
  std::stringstream saveAs;
  
  saveAs.str("");
  saveAs << path << "Lambdas" << "/pdf/" << "sin2thetaM_" << model << "_" << config << "_A" << ".pdf";
  c1->SaveAs( saveAs.str().c_str() );
  
  saveAs.str("");
  saveAs << path << "Lambdas" << "/png/" << "sin2thetaM_" << model << "_" << config << "_A" << ".png";
  c1->SaveAs( saveAs.str().c_str() );

  saveAs.str("");
  saveAs << path << "Lambdas" << "/eps/" << "sin2thetaM_" << model << "_" << config << "_A" << ".eps";
  c1->SaveAs( saveAs.str().c_str() );
  
  
}

