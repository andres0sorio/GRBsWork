// $Id: $
#ifndef MIXINGPARAMETERS_H 
#define MIXINGPARAMETERS_H 1

#include <RootCommon.h>
#include <TDOMParser.h>
#include <TXMLAttr.h>
#include <TXMLNode.h>
#include <TList.h>
#include <map>

/** @class MixingParameters MixingParameters.h
 *  @class ParameterList ParameterList.h
 *
 *  @author Andres Osorio
 *  @date   2010-09-12
 */

class MixingParameters : public TObject {
  
 public:
  
  MixingParameters() {};
  MixingParameters(int id) {
    Id = id; 
    par1  = -1.0; 
    par2  = -1.0; 
    par3  = -1.0;
    par4  = -1.0;
    par5  = -1.0;
    par6  = -1.0;
    par7  = -1.0;
    par8  = -1.0;
    
    
  };
  
  virtual ~MixingParameters() {};
  
  float GetPar1() const { return par1; }
  float GetPar2() const { return par2; }
  float GetPar3() const { return par3; }
  float GetPar4() const { return par4; }
  float GetPar5() const { return par5; }
  float GetPar6() const { return par6; }
  float GetPar7() const { return par7; }
  float GetPar8() const { return par8; }

  void SetPar1(float x) { par1 =x;}
  void SetPar2(float x) { par2 =x;}
  void SetPar3(float x) { par3 =x;}
  void SetPar4(float x) { par4 =x;}
  void SetPar5(float x) { par5 =x;}
  void SetPar6(float x) { par6 =x;}
  void SetPar7(float x) { par7 =x;}
  void SetPar8(float x) { par8 =x;}

  
  void SetPar(const char * name, float x) {
    m_locator = m_params.find( std::string( name ) );
    if ( m_locator == m_params.end() )
      m_params[std::string(name)] = x;
  };
  
  float GetPar( const char * name ) { 
    m_locator = m_params.find( std::string( name ) );
    if ( m_locator != m_params.end() )
      return (*m_locator).second;
    return -1.1234; 
  };
  
  friend std::ostream & operator << (std::ostream& out, const MixingParameters& p) {
    out << "Par1 " << p.par1 << std::endl;
    out << "Par2 " << p.par2 << std::endl;
    out << "Par3 " << p.par3 << std::endl;
    out << "Par4 " << p.par4 << std::endl;
    out << "Par5 " << p.par5 << std::endl;
    out << "Par6 " << p.par6 << std::endl;
    out << "Par7 " << p.par7 << std::endl;
    out << "Par8 " << p.par8 << std::endl;
    return out;
  }
  
private:
  int Id;
  
  float par1;
  float par2;
  float par3;
  float par4;
  float par5;
  float par6;
  float par7;
  float par8;

  std::map<std::string,float> m_params;
  
  std::map<std::string,std::string> m_params_name;

  std::map<std::string,float>::iterator m_locator;
    
};

class MixingParameterList {
  
 public:
  
  MixingParameterList() {
    listOfParameters = new TList();
    m_iter = new TIter(listOfParameters);
  };

  //..................................................................................

  int ParseFile( TString );
  
  void ParseParameterList( TXMLNode *);
  
  MixingParameters* ParseParameters( TXMLNode *, Int_t Id);

  //..................................................................................
  
  friend std::ostream& operator << (std::ostream& out, const MixingParameterList & pl) {
    TIter next(pl.listOfParameters);
    MixingParameters *p;
    while ((p =(MixingParameters*)next())){
      out << *p << std::endl;
    }
    return out;
  }
  
  void PrintParameters() {
    TIter next(listOfParameters);
    MixingParameters *p;
    while ((p =(MixingParameters*)next())) {
      std::cout << *p << std::endl;
    }
  }

  MixingParameters* GetParameters( int idx ) 
  {
    return ( MixingParameters*)listOfParameters->At( idx );
  }

  Int_t GetNParameters() 
  {
    return numberOfParameters;
  }
  
  MixingParameters * Next() 
  {
    return (MixingParameters*)(*m_iter)();
  }
  
private:
  
  Int_t   numberOfParameters;
  TList * listOfParameters;
  TIter * m_iter;
  
};

#endif
