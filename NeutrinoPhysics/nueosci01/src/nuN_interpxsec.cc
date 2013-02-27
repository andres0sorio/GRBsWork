#include "nuN_interpxsec.h"

const double Gf = 1.16632e-15; //G_fermi
const double Mn = 0.939573; //M nucleon
const double Mw = 81.800;
const double Mz = 91.600;

Interpolation::Interpolation() {

}

Interpolation::Interpolation( const char * f_name ) {

  m_in = new std::ifstream(f_name, ifstream::in);
  
  if(!m_in->is_open()) {
    //std::cout << "Data> cannot open file" << std::endl;
  } else { 
    //std::cout << "Data> file is now open" << std::endl;
  }

  m_xx.clear();
  m_cc.clear();
  m_nc.clear();

  m_logX = false;
  m_logY = false;
  
  e_nu      = -.1;
  cc_xsec   = -.1;
  nc_xsec   = -.1;
  tot_xsec  = -.1;

  while( m_in->good() ) {
    
    if ( m_in->eof() ) break;
  
    (*m_in) >> e_nu >> cc_xsec >> nc_xsec >> tot_xsec;

    m_xx.push_back( log10(e_nu) );
    m_cc.push_back( log10(cc_xsec) );
    m_nc.push_back( log10(nc_xsec) );
    
    //m_cc.push_back( cc_xsec );
    //m_nc.push_back( nc_xsec );
    //std::cout << log10(e_nu) << '\t' << log10(cc_xsec) << '\n';
    
    if ( m_in->fail()) break;
    
  }
  
  m_xx.pop_back();
  m_cc.pop_back();
  m_nc.pop_back();
  
  m_ndata = m_xx.size();

  m_interpolatorCC = new  ROOT::Math::Interpolator( m_xx, m_cc, ROOT::Math::Interpolation::kCSPLINE);
  m_interpolatorNC = new  ROOT::Math::Interpolator( m_xx, m_nc, ROOT::Math::Interpolation::kCSPLINE);
  

}

Interpolation::~Interpolation() {

  delete m_interpolatorCC;
  
  delete m_interpolatorNC;

  m_in->close();
  if (m_in )
    delete m_in;
  
}

void Interpolation::setLogY() {
  
  unsigned int xdata = m_xx.size();
  
  for (unsigned int k =0; k < xdata; ++k ){
    
  }
  
}

double Interpolation::evaluateCC( double x ) {

  double ff = m_interpolatorCC->Eval( x );
  return ff;

}


double Interpolation::evaluateNC( double x ) {
  
  double ff = m_interpolatorNC->Eval( x );
  return ff;
  
}

void Interpolation::showPoints() {
  
  unsigned int xdata = m_xx.size();
  
  for (unsigned int k =0; k < xdata; ++k ){
    
    std::cout << m_xx[k] << '\t' 
	      << m_cc[k] << '\n';
    
  }
  
  
}
