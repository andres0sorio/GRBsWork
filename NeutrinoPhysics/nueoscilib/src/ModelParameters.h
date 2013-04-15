// $Id: $
#ifndef MODELPARAMETERS_H 
#define MODELPARAMETERS_H 1

// Include files

#include <RootCommon.h>
#include <TDOMParser.h>
#include <TXMLAttr.h>
#include <TXMLNode.h>
#include <TList.h>
#include <map>

/** @class ModelParameters ModelParameters.h
 *  
 * \brief XML parser to read from file the star model (A,B,C) configuration
 *  
 *  @author Andres Osorio
 *  @date   2013-03-01
 */

class ModelParameters : public TObject {
  
 public:
  
  ModelParameters() {};
  ModelParameters( int id, std::string name) {
    Id = id;
    Name = name;
  };
  
  virtual ~ModelParameters() {};
  
  float GetPar( int idx ) {
    m_locator = m_params.find( idx );
    if ( m_locator == m_params.end() )
      return -1234.0;
    return m_params.find(idx)->second;
  };
  
  void SetPar(int idx, float x) {
    m_params[ idx ] = x;
  };

  float GetPar( const char * name ) {
    m_locator_name = m_params_name.find( name );
    if ( m_locator_name == m_params_name.end() )
      return -1234.0;
    return m_params_name.find(name)->second;
  };
  
  void SetPar(const char * name, float x) {
    m_params_name[ name ] = x;
  };

  
  friend std::ostream & operator << (std::ostream& out, const ModelParameters& p) {
    
    std::map<int,float>::const_iterator itr;
    
    for( itr = p.m_params.begin(); itr != p.m_params.end(); ++itr )
      out << "Par: " << (*itr).first << " " << (*itr).second << std::endl;
   
    std::map<std::string,float>::const_iterator str;

    for( str  = p.m_params_name.begin(); str != p.m_params_name.end(); ++str )
      out << " -> " << (*str).first << " " << (*str).second << std::endl;
   
    return out;
    
  }
  
  int Id;
  std::string Name;
  
private:
  
  std::map<int,float> m_params;
  
  std::map<int,float>::const_iterator m_locator;

  std::map<std::string,float> m_params_name;
  
  std::map<std::string,float>::const_iterator m_locator_name;
  
  
};

//..................................................................................

class ModelParameterList {
  
 public:
  
  ModelParameterList() {
    listOfParameters = new TList();
    m_iter = new TIter(listOfParameters);
  };
  
  //..................................................................................
  
  int ParseFile( TString );
  
  void ParseParameterList( TXMLNode *);
  
  ModelParameters* ParseParameters( TXMLNode *, int, std::string );
  
  //..................................................................................
  
  friend std::ostream& operator << (std::ostream& out, const ModelParameterList & pl) {
    TIter next(pl.listOfParameters);
    ModelParameters *p;
    while ((p =(ModelParameters*)next())){
      out << *p << std::endl;
    }
    return out;
  }
  
  void PrintParameters() {
    TIter next(listOfParameters);
    ModelParameters *p;
    while ((p =(ModelParameters*)next())) {
      std::cout << *p << std::endl;
    }
  }

  ModelParameters* GetParameters( const char * name ) 
  {
    TIter next(listOfParameters);
    ModelParameters *p;
    while ((p =(ModelParameters*)next())) {
      if ( std::string(name).compare( p->Name ) == 0 )
        return p;
      
    }
    
    return 0;
    
  }
  

  ModelParameters* GetParameters( int idx ) 
  {
    return ( ModelParameters*)listOfParameters->At( idx );
  }
  
  Int_t GetNParameters() 
  {
    return numberOfParameters;
  }
  
  ModelParameters * Next() 
  {
    return (ModelParameters*)(*m_iter)();
  }
  
private:
  
  Int_t   numberOfParameters;
  TList * listOfParameters;
  TIter * m_iter;
  
};

#endif // MODELPARAMETERS_H
