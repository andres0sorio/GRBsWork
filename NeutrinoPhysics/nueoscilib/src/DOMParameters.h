#include <RootCommon.h>
#include <TDOMParser.h>
#include <TXMLAttr.h>
#include <TXMLNode.h>
#include <TList.h>
#include <TH1F.h>
#include <map>

/** @class Parameters Parameters.h
 *  @class ParameterList ParameterList.h
 *
 *  @author Andres Osorio
 *  @date   2010-09-12
 */

class Parameters : public TObject {
  
 public:
  
  Parameters() {};
  Parameters(int id) {
    
    Id = id; 
    
    par1  = -1.0; 
    par2  = -1.0; 
    par3  = -1.0;
    par4  = -1.0;
    par5  = -1.0;
    par6  = -1.0;
    par7  = -1.0;
    par8  = -1.0;
    par9  = -1.0;
    par10 = -1.0;
    par11 = -1.0;
    par12 = -1.0;
    par13 = -1.0;
    par14 = -1.0;
  
    konst1 = 1.0;

    m_use_varying_Nbeta = false;
            
  };
  
  virtual ~Parameters() {};
  
  double GetPar1() const { return par1; }
  double GetPar2() const { return par2; }
  double GetPar3() const { return par3; }
  double GetPar4() const { return par4; }
  double GetPar5() const { return par5; }
  double GetPar6() const { return par6; }
  double GetPar7() const { return par7; }
  double GetPar8() const { return par8; }
  double GetPar9() const { return par9; }
  double GetPar10() const { return par10; }
  double GetPar11() const { return par11; }
  double GetPar12() const { return par12; }
  double GetPar13() const { return par13; }
  double GetPar14() const { return par14; }
  
  double GetKonst1() const { return konst1; }

  void SetPar1(double x) { par1 =x;}
  void SetPar2(double x) { par2 =x;}
  void SetPar3(double x) { par3 =x;}
  void SetPar4(double x) { par4 =x;}
  void SetPar5(double x) { par5 =x;}
  void SetPar6(double x) { par6 =x;}
  void SetPar7(double x) { par7 =x;}
  void SetPar8(double x) { par8 =x;}
  void SetPar9(double x) { par9 =x;}
  void SetPar10(double x) { par10 =x;}
  void SetPar11(double x) { par11 =x;}
  void SetPar12(double x) { par12 =x;}
  void SetPar13(double x) { par13 =x;}
  void SetPar14(double x) { par14 =x;}

  void SetKonst1(double x) { konst1 =x;}

  void SetPar(const char * name, double x) {
    //m_locator = m_params.find( std::string( name ) );
    //if ( m_locator == m_params.end() )
    m_params[std::string(name)] = x;
  };
  
  double GetPar( const char * name ) { 
    m_locator = m_params.find( std::string( name ) );
    if ( m_locator != m_params.end() )
      return (*m_locator).second;
    return -1.1234; 
  };

  double GetPar( const char * name , double xx) { 

    if (m_use_varying_Nbeta) 
    {
      m_histo_locator = m_histo_params.find( std::string( name ) );
      if ( m_histo_locator != m_histo_params.end() ) 
      {
        //
        int bin_pos = m_histo_locator->second->FindBin( xx*1.0E9 );
        return m_histo_locator->second->GetBinContent( bin_pos );
        //
      } else
        return -1.1234;
    } else {
      
      m_locator = m_params.find( std::string( name ) );
      if ( m_locator != m_params.end() )
        return (*m_locator).second;
      return -1.1234; 
    }
        
  };
  
  // Use a specific parametrization
  
  void UseVaryingNbeta( bool option ) {
    m_use_varying_Nbeta = option;
  };
  
  
  friend std::ostream & operator << (std::ostream& out, const Parameters& p) {
    
    out << "Par1  => " << p.par1 << std::endl;
    out << "Par2  => " << p.par2 << std::endl;
    out << "Par3  => " << p.par3 << std::endl;
    out << "Par4  => " << p.par4 << std::endl;
    out << "Par5  => " << p.par5 << std::endl;
    out << "Par6  => " << p.par6 << std::endl;
    out << "Par7  => " << p.par7 << std::endl;
    out << "Par8  => " << p.par8 << std::endl;
    out << "Par9  => " << p.par9 << std::endl;
    out << "Par10  => " << p.par10 << std::endl;
    out << "Par11  => " << p.par11 << std::endl;
    out << "Par12  => " << p.par12 << std::endl;
    out << "Par13  => " << p.par13 << std::endl;
    out << "Par14  => " << p.par14 << std::endl;
    
    out << "Konst1 " << p.konst1 << std::endl;

    std::map<std::string,double>::const_iterator itr;
    std::map<std::string,TH1F*>::const_iterator itr2;
    
    for (itr = p.m_params.begin(); itr != p.m_params.end(); ++itr)
      std::cout << itr->first << " => " << itr->second << '\n';
    
    for (itr2 = p.m_histo_params.begin(); itr2 != p.m_histo_params.end(); ++itr2)
      std::cout << itr2->first << " => " << itr2->second << '\n';



        
    return out;
  };
  

  ///
  // AO dec 2013
  
  std::map<std::string,TH1F*> m_histo_params; //Histogram parametrization
  
  ///
  
private:
  int Id;
  
  double par1;
  double par2;
  double par3;
  double par4;
  double par5;
  double par6;
  double par7;
  double par8;
  double par9;
  double par10;
  double par11;
  double par12;
  double par13;
  double par14;

  double konst1;
  
  std::map<std::string,double> m_params;
  
  std::map<std::string,std::string> m_params_name;

  std::map<std::string,double>::iterator m_locator;

  std::map<std::string,TH1F*>::iterator m_histo_locator;

  bool m_use_varying_Nbeta;
    
    
};

class ParameterList {
  
 public:
  
  ParameterList() {
    listOfParameters = new TList();
    m_iter = new TIter(listOfParameters);
  };

  //..................................................................................

  int ParseFile( TString );
  
  void ParseParameterList( TXMLNode *);
  
  Parameters* ParseParameters( TXMLNode *, Int_t Id);

  //..................................................................................
  
  friend std::ostream& operator << (std::ostream& out, const ParameterList & pl) {
    TIter next(pl.listOfParameters);
    Parameters *p;
    while ((p =(Parameters*)next())){
      out << *p << std::endl;
    }
    return out;
  }
  
  void PrintParameters() {
    TIter next(listOfParameters);
    Parameters *p;
    while ((p =(Parameters*)next())) {
      std::cout << *p << std::endl;
    }
  }

  Parameters* GetParameters( int idx ) 
  {
    return ( Parameters*)listOfParameters->At( idx );
  }

  Int_t GetNParameters() 
  {
    return numberOfParameters;
  }
  
  Parameters * Next() 
  {
    return (Parameters*)(*m_iter)();
  }
  
private:
  
  Int_t   numberOfParameters;
  TList * listOfParameters;
  TIter * m_iter;
  
};
