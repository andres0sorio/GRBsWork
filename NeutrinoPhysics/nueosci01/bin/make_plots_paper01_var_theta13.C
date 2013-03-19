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
  latex.SetTextSize(0.03);
  latex.SetTextAlign(31); // align right
  latex.DrawLatex(0.90,0.92, title);
  latex.SetTextAlign(11); // align left
  latex.DrawLatex(0.24,0.92,"Preliminary");
}

void plotId( const char *name )
{
  TLatex *   tex = new TLatex(1.51e+11,0.67, name);
  tex->SetTextSize(0.286);
  tex->SetLineWidth(1);
  tex->Draw(); 
}


void set_plot_style()
{
    const Int_t NRGBs = 5;
    const Int_t NCont = 255;

    Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
    Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
    Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
    Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
    TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
    gStyle->SetNumberContours(NCont);

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

  makePlots("ModelA","0", "Pee","file_list_theta13.txt");
  
}

void makePlots( const char * model, const char * src, const char * prob, const char * infile) 
{
  
  // X - range 
  double xmin = 1.0e11;
  double xmax = 1.0e14;
  
  // Y - range 
  double ymin = 8.0;
  double ymax = 13.00;

  //Output path
  TString path("./paper01-plots/probs/");
  
  TString dataPxx = TString( model ) + TString("_") + TString( src ) + TString("_") + TString(prob) +  TString("/data");
  
  TString cname = TString(model) + TString("_") + TString(prob);
  
  TCanvas * c1 = new TCanvas( cname.Data(), "Oscillation probabilities", 185,87,672,578); 
  c1->SetRightMargin(0.1257485);
  c1->SetBottomMargin(0.1309091);
  c1->SetFrameFillStyle(0);
  c1->SetFrameBorderMode(0);
  c1->SetFrameFillStyle(0);
  c1->SetFrameBorderMode(0);
  c1->Draw();
  
  int maxPads = 5;
  
  TString cnameB = TString(model) + TString("_") + TString(prob) + TString("_") + TString("B");
  
  TCanvas * c2 = new TCanvas( cnameB.Data(), "Oscillation probabilities", 690,130,426,579);
  c2->SetRightMargin(0.1257485);
  c2->SetBottomMargin(0.1309091);
  c2->SetFrameFillStyle(0);
  c2->SetFrameBorderMode(0);
  c2->SetFrameFillStyle(0);
  c2->SetFrameBorderMode(0);
  c2->Divide(1,maxPads);
  c2->Draw();

  //Branches
  double xx = 0.0;
  double yy = 0.0;
  
  TGraph2D * radiusVar= new TGraph2D();
  
  //.......
  
  TList * v_Labels = new TList();
  TObjString *label;
  
  ifstream in;
  in.open(infile); //
  
  int i = 0;
  
  while ( in.good() ) 
  {
    std::string input_file;
    in >> input_file;
    std::cout << "adding: " << input_file << std::endl;
    if ( input_file.size() != 0 && input_file.size() > 5) 
    {
      label = new TObjString( input_file.c_str() );
      v_Labels->Add( label ); 
      ++i;
    }
    
  }
  
  std::cout << "Total files added: " << i << " " << v_Labels->GetSize() <<std::endl; 
  
  in.close();
  
  //.......
  
  double * theta = new double[30];
  double value = ymin;
  double dtheta = (ymax-ymin)/(double)(i-1);
  for(int j=0; j< i; ++j) 
  {
    theta[j] = value;
    value += dtheta;
  }
  
  int idx = 0;

  TList * v_Profiles = new TList();
  
  TProfile * pf1;
  
  for( int k = 0; k < v_Labels->GetSize(); ++k ) {

    char pf1name[20];
    sprintf(pf1name,"pf_%d", k);

    pf1 = new TProfile(pf1name,"Profile histogram",500,1e11,1e14,0.0,1.0);

    ////
    
    TString name = ((TObjString*)v_Labels->At(k))->GetString();

    TFile * f1 = TFile::Open( name.Data(), "READ");
    
    f1->cd();
        
    TTree * PxxTreeNu = (TTree*)gDirectory->Get( dataPxx.Data() );
    
    PxxTreeNu->SetBranchAddress("Ex",&xx);
    PxxTreeNu->SetBranchAddress("Pb",&yy);
  
    Long64_t nentries = PxxTreeNu->GetEntries();
    
    for (Long64_t ni=0;ni<nentries;ni++) {
    
      PxxTreeNu->GetEntry(ni);
      pf1->Fill( xx, yy, 1.0);
      
    }
    
    v_Profiles->Add( pf1 );
    
    //f1->Close();
    std::cout << " done with file" << std::endl;
        
  }

  //
  
  idx = 0;
  
  int nprofiles = v_Profiles->GetEntries();
  
  for( int k = 0; k < nprofiles; ++k) 
  {
    
    TProfile *pf1 = (TProfile*)v_Profiles->At(k);
    
    int nbins = pf1->GetNbinsX();
    
    for( int j=1 ; j <= nbins; ++j ) 
    {
      radiusVar->SetPoint( idx , j , k, pf1->GetBinContent(j) );
      idx += 1;
    }
    
  }
  
  
  //....

  TAxis *axis1 = radiusVar->GetXaxis();
  axis1->SetLimits(xmin, xmax);

  TAxis *axis2 = radiusVar->GetYaxis();
  axis2->SetLimits(ymin, ymax);
  
  //set_plot_style();
  
  gStyle->SetPalette(51,0);
  gStyle->SetNumberContours(255);
  
  std::cout << " drawing ... contours " << std::endl;
  
  c1->cd();
  gPad->SetLogx();

  radiusVar->SetMaximum(1.0);
  radiusVar->SetMinimum(0.0);
  
  radiusVar->GetYaxis()->SetNdivisions(509);
  radiusVar->GetYaxis()->SetTitle( "#theta_{13}" );
  radiusVar->GetYaxis()->SetLabelSize(0.05);
  radiusVar->GetYaxis()->SetLabelFont(42);
  radiusVar->GetYaxis()->SetTitleSize(0.05);
  radiusVar->GetYaxis()->SetTitleOffset(0.45);
  radiusVar->GetYaxis()->SetTitleFont(42);
  radiusVar->GetYaxis()->SetLabelOffset(0.01);
  radiusVar->GetYaxis()->CenterTitle(true); 
  
  radiusVar->GetXaxis()->SetTitle("E [eV]");
  radiusVar->GetXaxis()->CenterTitle(true); 
  radiusVar->GetXaxis()->SetLabelOffset(0.01);
  radiusVar->GetXaxis()->SetLabelSize(0.05);
  radiusVar->GetXaxis()->SetTitleSize(0.05);
  radiusVar->GetXaxis()->SetTitleOffset(0.9);
  radiusVar->GetXaxis()->SetLabelFont(42);

  radiusVar->Draw("COLZ");
  topTitle(model);
  c1->cd();
  
  std::cout << " drawing ... profiles " << std::endl;
  
  c2->cd();
  idx = 1;
  
  for( int k = 0; k < nprofiles; ++k) 
  {
    
    TProfile *pf1 = (TProfile*)v_Profiles->At(k);
    
    c2->cd(idx);
    gPad->SetLogx();
    pf1->SetFillColor(5);
    pf1->SetMinimum(0.0);
    pf1->SetMaximum(1.0);
    
    pf1->GetYaxis()->SetNdivisions(504);
    pf1->GetYaxis()->SetTitle( "Pxx" );
    pf1->GetYaxis()->SetLabelSize(0.11);
    pf1->GetYaxis()->SetLabelFont(42);
    pf1->GetYaxis()->SetTitleSize(0.09);
    pf1->GetYaxis()->SetTitleOffset(0.45);
    pf1->GetYaxis()->SetTitleFont(42);
    pf1->GetYaxis()->SetLabelOffset(0.007);
    pf1->GetYaxis()->CenterTitle(true); 
    
    pf1->GetXaxis()->SetTitle("E [eV]");
    pf1->GetXaxis()->CenterTitle(true); 
    pf1->GetXaxis()->SetLabelOffset(0.007);
    pf1->GetXaxis()->SetLabelSize(0.11);
    pf1->GetXaxis()->SetTitleSize(0.07);
    pf1->GetXaxis()->SetTitleOffset(0.9);
    pf1->GetXaxis()->SetLabelFont(42);
    
    if ( (k) % 4 == 0 ) {
      std::cout << " pf: " << idx << " k " << k  << std::endl;
      pf1->Draw("hist");
      idx+=1;
      
    }
    char aname[50];
    sprintf(aname,"#theta = %0.1f", theta[k] );
    plotId( aname );

    if ( idx > maxPads ) break;

  }
  
  
  std::cout << " drawing ... done" << std::endl;
  
  std::stringstream saveAs;
  
  saveAs.str("");
  saveAs << path << model << "/pdf/" << "nueosc_" << model << "_" << prob << "_theta13_var" << ".pdf";
  c1->SaveAs( saveAs.str().c_str() );
  
  saveAs.str("");
  saveAs << path << model << "/png/" << "nueosc_" << model << "_" << prob << "_theta13_var" << ".png";
  c1->SaveAs( saveAs.str().c_str() );

  saveAs.str("");
  saveAs << path << model << "/eps/" << "nueosc_" << model << "_" << prob << "_theta13_var" << ".eps";
  c1->SaveAs( saveAs.str().c_str() );

  saveAs.str("");
  saveAs << path << model << "/pdf/" << "nueosc_" << model << "_" << prob << "_Profs_theta13_var" << ".pdf";
  c2->SaveAs( saveAs.str().c_str() );
  
  saveAs.str("");
  saveAs << path << model << "/png/" << "nueosc_" << model << "_" << prob << "_Profs_theta13_var" << ".png";
  c2->SaveAs( saveAs.str().c_str() );
  
  saveAs.str("");
  saveAs << path << model << "/eps/" << "nueosc_" << model << "_" << prob << "_Profs_theta13_var" << ".eps";
  c2->SaveAs( saveAs.str().c_str() );

  delete[] theta;
  
}

