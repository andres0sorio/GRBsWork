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

  std::cout << " total bins " << k << std::endl;
  
}
