//-----------------------------------------------------------------------------
// Energy binning - IceCube energy resolution of 30% DEx = 0.30Ex
///

{
  
  double xmin = 1.0E11;
  double xmax = 1.0E17;
  
  double DEx  = 0.30;
  
  double Xx   = xmin;
  
  int k = 0;

  while( Xx < xmax ) 
  {
    
    double DeltaBin = DEx * Xx;
    
    double low_bin = Xx;
    double upp_bin = Xx + DeltaBin;
    
    std::cout << Xx << " " << low_bin << " " << upp_bin << std::endl;
    
    Xx = upp_bin;

    k += 1;
        
  }

  //////////////////////////////////////////////
  // Set the binning
  
  float * m_vbins = new float[100];
  
  double m_e_min = 1.0E11;
  double m_e_max = 1.0E17;
  double DEx     = 0.30;
  double Xx      = m_e_min;
  
  int k = 1;
  
  m_vbins[0] = Xx; // This is the first lower edge bin value
  
  while( Xx < m_e_max ) 
  {
    
    double DeltaBin = DEx * Xx;
    double low_bin = Xx;
    double upp_bin = Xx + DeltaBin;
    
    Xx = upp_bin;
    m_vbins[k] = Xx;
    k += 1;
    
  }
  
  int m_e_bins = ( k - 1); //Array has to have nbins+1 size
  
  //////////////////////////////////////////////
  
  TH1F * h1 = new TH1F("h1","R vs energy bing", m_e_bins, m_vbins);
  
  int BC = 1;
  
  for(Long64_t i=0; i < m_e_bins; i++) {
    
    h1->SetBinContent(i+1, BC );
    
    BC += 1;
    
  }
  
  TCanvas * c1 = new TCanvas();
  c1->SetLogx();
  
  c1->cd();
  
  h1->Draw();
  
  // Histogram info

  std::cout << "Histogram info: " << h1->GetNbinsX() << std::endl;
  
  for(Long64_t i=1; i <= m_e_bins; ++i) 
  {
    
    
    std::cout << "bin: " << i << '\t' 
              << h1->GetBinLowEdge(i) << '\t'
              << h1->GetBinWidth(i) << '\t' 
              << ( h1->GetBinLowEdge(i)+h1->GetBinWidth(i) ) << '\n';
    
    
  }
  
  
}
