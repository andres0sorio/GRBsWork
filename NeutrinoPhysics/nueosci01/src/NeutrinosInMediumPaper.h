// $Id: $
#ifndef NEUTRINOSINMEDIUMPAPER_H 
#define NEUTRINOSINMEDIUMPAPER_H 1

// Include files
#include <utility>
#include <Graphics.h>
#include <TTree.h>
#include <TFile.h>
#include <NeutrinoOscInVarDensity.h>
#include <ModelParameters.h>
#include <DensityModels.h>

/** @class NeutrinosInMediumPaper NeutrinosInMediumPaper.h
 *  
 *
 *  @author Andres Osorio
 *  @date   2013-02-27
 */

class NeutrinosInMediumPaper : public Graphics {
public: 
  /// Standard constructor
  NeutrinosInMediumPaper( ) { }; 

  NeutrinosInMediumPaper( MixingParameters * ); 

  NeutrinosInMediumPaper( MixingParameters * , TFile * ); 

  virtual ~NeutrinosInMediumPaper( ); ///< Destructor
  
  NeutrinoOscInVarDensity * m_Physics;
  
  void Test();
  
  void GenerateDatapoints(const char *, const char * , ModelParameters *);
  
  void PropagateVacuum(const char *, const char * );
  
protected:
  
private:

  void Init( const char *, const char * );

  bool m_debug;

  std::map<std::string, std::pair<int,int> > m_ProbIndex;
  
  TFile * m_file;
  
  TTree * m_tree;

  TTree * m_input_tree;
  
  std::map<std::string, DensityModels*> m_Models;

  float m_in_Ex;
  
  float m_in_Pb;
  
  TBranch * b_in_Ex;
  
  TBranch * b_in_Pb;
  

};
#endif // NEUTRINOSINMEDIUMPAPER_H
