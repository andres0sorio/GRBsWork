// $Id: $
// Include files 



// local
#include "ModelParameters.h"

//-----------------------------------------------------------------------------
// Implementation file for class : ModelParameters
//
// 2013-03-01 : Andres Osorio
//-----------------------------------------------------------------------------

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
int ModelParameterList::ParseFile(TString filename) {
  
  TDOMParser *domParser = new TDOMParser();
  
  Int_t parsecode = domParser->ParseFile(filename);
  
  if (parsecode < 0) {
    std::cerr << domParser->GetParseCodeMessage(parsecode) << std::endl;
    return -1;
  }
  
  TXMLNode * node = domParser->GetXMLDocument()->GetRootNode();
  
  ParseParameterList(node);
  
  return 0;
  
}

void ModelParameterList::ParseParameterList(TXMLNode *node) {
    
  for (; node; node = node->GetNextNode()) {
    
    if (node->GetNodeType() == TXMLNode::kXMLElementNode) { // Element Node
      if (strcmp(node->GetNodeName(), "ModelParameters") == 0) {
        
        Int_t id=0;
        std::string model_name;
        
        if (node->HasAttributes()) {
          TList *attrList = node->GetAttributes();
          TXMLAttr *attr = 0;
          TIter next(attrList);
          
          while ((attr=(TXMLAttr*)next())) {
            if (strcmp(attr->GetName(), "Id") == 0) {
              model_name = std::string(attr->GetValue());
              std::cout << " * " << model_name << std::endl;
              break;
            }
          }
        }
        listOfParameters->Add(ParseParameters(node->GetChildren(), id, model_name ));
      }
    }
    ParseParameterList(node->GetChildren());
  }
}

ModelParameters* ModelParameterList::ParseParameters(TXMLNode *node, Int_t Id, std::string Name) {
  
  ModelParameters * p1 = new  ModelParameters(Id, Name);
  
  float x = 0.0;
  
  for ( ; node; node = node->GetNextNode()) {
    
    if (node->GetNodeType() == TXMLNode::kXMLElementNode) { // Element Node
      
      if (strcmp(node->GetNodeName(), "Par") == 0) {
        x = atof(node->GetText());
        
        TXMLAttr * att = (TXMLAttr*)node->GetAttributes()->At(0); //This the id attribute
        p1->SetPar( atof( att->GetValue() ), x );
        
        att = (TXMLAttr*)node->GetAttributes()->At(1); //This the name attribute
        p1->SetPar( att->GetValue() , x );
        
      }
    }
  }
  
  return p1;
  
}
