// $Id: 
// Include files 

#include <IntegralsPlots.h>
#include <sstream>

///////////////////////////////////////////////////////////////

int main(int iargv, const char **argv) {

  std::cout << "-------------------------------------------------------" << '\n';
  
  ParameterList parlist;
  
  if (parlist.ParseFile("config.xml") == 0)
    std::cout << "Opened config.xml" << std::endl;
  
  std::cout << "-------------------------------------------------------" << '\n';
  
  IntegralsPlots * p1 = new IntegralsPlots("XSec_neut.dat", "XSec_anti.dat", parlist.GetParameters(0) );
  
  p1->IntegralOne();
  p1->IntegralTwo();
  p1->IntegralThree();
  p1->IntegralFour();
  p1->IntegralFive();
  
  delete p1;
  
  PShadow * p2 = new PShadow();
  p2->Validate();
  delete p2;
  
  return 0;
    
}
