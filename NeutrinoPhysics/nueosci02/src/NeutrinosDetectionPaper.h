// $Id: $
#ifndef NEUTRINOSDETECTIONPAPER_H 
#define NEUTRINOSDETECTIONPAPER_H 1

// Include files
#include <utility>
#include <Graphics.h>
#include <MuTrackEvents.h>
#include <ShowerEvents.h>
#include <DensityModels.h>
#include <ModelParameters.h>
#include <TTree.h>
#include <TFile.h>
#include <TH1F.h>
#include <NeutrinoOscInVacuum.h>
#include <NeutrinoOscInVarDensity.h>

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
  
  void EvaluateRvsEnergyMena( const char *, const char *, const char *, const char * );

  void EvaluateRvsEnergy( const char *, const char *, const char *, const char * ); //Introduced Oct-2014
  
  void MakeVariation02( const char *, const char *, const char *, const char *, double, double, double);

  void MakeVariation03( const char *, const char *, const char *, double, double, double);

  void MakeVariation04( const char *, const char *, const char *, double, double, double, double);

  void EvaluateR( const char *, const char *, const char *, const char *, double );
  
  void MakeVariationStdPicture( const char *, const char *, const char *, double, double, double);

  void MakeVariationStdPictureEarth( const char *, const char *, const char *, double, double, double);
  
  void SetFluxHistograms( TFile *, const char *, const char *, const char *, const char *);
  
  void ResetFluxHistograms( );
  
  void SetMixingParameters( MixingParameters * mixing) { m_mixpars = mixing; };
  
  void SetModelParameters( ModelParameters * modelpars) { m_modelpars = modelpars; };
  
  void PropagateThroughEarth( const char *, const char *, const char *, const char *, double, double, double );
  
  bool InitOutput( const char *, const char *,  const char *, const char * );

  void WriteOutput();
  
protected:
  
private:
  
  bool Init( const char *,  const char *, const char * );

  void SetFluxAverages( TFile *, const char *, const char *, const char *);
    
  bool m_debug;
  
  Parameters * m_config;
  MixingParameters * m_mixpars;
  ModelParameters * m_modelpars;
  
  TFile * m_file;
  TFile * m_output_file;
  TTree * m_tree;
  TTree * m_input_tree;
  
  TDirectory * m_outdir;
  
  //Output branches

  double m_Xx;
  double m_MuTks;
  double m_TauTks;
  double m_HadShw;
  double m_HadShwE;
  double m_HadShwT;
  double m_HadShwNC;
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
  
  float * m_vbins;
  int m_e_bins;
  float m_e_max;
  float m_e_min;
  
  std::string m_data_path;

  std::string m_data_pshadow;
  std::string m_data_xsec_neut;
  std::string m_data_xsec_anti;
  
  std::map<int,double> m_energy_bin;
  
  std::map<std::string,TH1F*> m_flux_histos;
  
};
#endif // NEUTRINOSDETECTIONPAPER_H
