// $Id: $
// Include files 



// local
#include "Graphics.h"

//-----------------------------------------------------------------------------
// Implementation file for class : Graphics
//
// 2010-09-19 : Andres Osorio
//-----------------------------------------------------------------------------

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
Graphics::Graphics(  ) {

  txtci = TColor::GetColor("#333333");

  h_1dHistos["phi_e"] = new TH1D("phi_e","flux distribution", 100, 0.0, 1.0);
  h_1dHistos["phi_mu"] = new TH1D("phi_mu","flux distribution", 100, 0.0, 1.0);
  h_1dHistos["phi_tau"] = new TH1D("phi_tau","flux distribution", 100, 0.0, 1.0);

  h_1dHistos["phi_ae"] = new TH1D("phi_ae","flux distribution", 100, 0.0, 1.0);
  h_1dHistos["phi_amu"] = new TH1D("phi_amu", "flux distribution", 100, 0.0, 1.0);
  h_1dHistos["phi_atau"] = new TH1D("phi_atau","flux distribution", 100, 0.0, 1.0);

  std::cout << "Graphics> Ready" << std::endl;
  
}

//=============================================================================
// Destructor
//=============================================================================

Graphics::~Graphics() {} 

//=============================================================================
void Graphics::SetOptions() {

}

void Graphics::Reset() {
  
  h_1dHistos["phi_e"]->Reset();
  h_1dHistos["phi_mu"]->Reset();
  h_1dHistos["phi_tau"]->Reset();
  
  h_1dHistos["phi_ae"]->Reset();
  h_1dHistos["phi_amu"]->Reset();
  h_1dHistos["phi_atau"]->Reset();
  
  std::cout << "Graphics> Reset done." << std::endl;

}
