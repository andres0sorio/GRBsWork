// $Id: $
#ifndef NEUTRINOSDETECTIONPAPER_H 
#define NEUTRINOSDETECTIONPAPER_H 1

// Include files
#include <utility>
#include <Graphics.h>
#include <MuTrackEvents.h>
#include <ShowerEvents.h>
#include <DensityModels.h>
#include <TTree.h>
#include <TFile.h>

/** @class NeutrinosDetectionPaper NeutrinosDetectionPaper.h
 *  
 *
 *  @author Andres Osorio
 *  @date   2013-03-29
 */

class NeutrinosDetectionPaper : public Graphics  {
public: 
  /// Standard constructor
  NeutrinosDetectionPaper( ) : Graphics() { }; 
  
  NeutrinosDetectionPaper( Parameters *); 
  
  virtual ~NeutrinosDetectionPaper( ); ///< Destructor
  
  void MakeVariation01(TFile * , const char *, const char * );
  
  void MakeVariation02(TFile * , const char *, const char *, const char *, double, double, double);

  void MakeVariation03(const char *, const char *, const char *, double, double, double);

  void MakeVariationStdPicture( const char *, const char *, double, double, double);
  
protected:
  
private:
  
  bool Init( const char *,  const char *, const char * );

  bool InitOutput( const char *, const char *,  const char *, const char * );
  
  bool m_debug;
  
  Parameters * m_config;
    
  TFile * m_file;
  TFile * m_output_file;
  
  TTree * m_tree;
  TTree * m_input_tree;
  
  //Output branches

  double m_Xx;
  double m_MuTks;
  double m_TauTks;
  double m_HadShw;
  double m_Ratio;
  
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
#endif // NEUTRINOSDETECTIONPAPER_H
