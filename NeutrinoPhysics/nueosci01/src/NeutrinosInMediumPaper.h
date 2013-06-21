// $Id: $
#ifndef NEUTRINOSINMEDIUMPAPER_H 
#define NEUTRINOSINMEDIUMPAPER_H 1

// Include files
#include <utility>
#include <Graphics.h>
#include <TTree.h>
#include <TFile.h>
#include <NeutrinoOscInVacuum.h>
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
  
  NeutrinoOscInVacuum     * m_Physics_Vacuum;
    
  void PropagateVacuum(const char *, const char * );
  
  void Propagate(const char *, const char *, const char *, ModelParameters *);
  
  void GenerateDatapoints(const char *, const char * , ModelParameters *);
  
  void StudyResonances( const char *, const char *, ModelParameters *, bool);
  
protected:
  
private:
  
  bool Init( const char *,  const char *, const char * );
  
  bool m_debug;
  
  std::map<std::string, std::pair<int,int> > m_ProbIndex;
  
  TFile * m_file;
  
  TTree * m_tree;
  
  TTree * m_input_tree;
  
  std::map<std::string, DensityModels*> m_Models;
  
  //Output branches
  
  double m_Ex;
  double m_Pb;
  double m_Qf;
  double m_Phi_e;
  double m_Phi_m;
  double m_Phi_t;
  
  //Input branches
  
  double m_Ex_in;
  double m_Pb_in;
  double m_Phi_e_in;
  double m_Phi_m_in;
  double m_Phi_t_in;
  
  TBranch * b_Ex_in;
  TBranch * b_Pb_in;
  TBranch * b_Phi_e_in;
  TBranch * b_Phi_m_in;
  TBranch * b_Phi_t_in;
    

};
#endif // NEUTRINOSINMEDIUMPAPER_H
