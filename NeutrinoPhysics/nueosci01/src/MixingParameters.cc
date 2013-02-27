#include "MixingParameters.h"

int MixingParameterList::ParseFile(TString filename) {
  
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

void MixingParameterList::ParseParameterList(TXMLNode *node) {
    
  for (; node; node = node->GetNextNode()) {
    
    if (node->GetNodeType() == TXMLNode::kXMLElementNode) { // Element Node
      if (strcmp(node->GetNodeName(), "MixingParameters") == 0) {
        Int_t id=0;
        if (node->HasAttributes()) {
          TList *attrList = node->GetAttributes();
          TXMLAttr *attr = 0;
          TIter next(attrList);
          
          while ((attr=(TXMLAttr*)next())) {
            if (strcmp(attr->GetName(), "Id") == 0) {
              id = atoi(attr->GetValue());
              break;
            }
          }
          
        }
        listOfParameters->Add(ParseParameters(node->GetChildren(), id));
      }
    }
    ParseParameterList(node->GetChildren());
  }
}

MixingParameters* MixingParameterList::ParseParameters(TXMLNode *node, Int_t Id) {
  
  MixingParameters * p1 = new  MixingParameters(Id);

  float x=0.0;
  
  for ( ; node; node = node->GetNextNode()) {
    
    if (node->GetNodeType() == TXMLNode::kXMLElementNode) { // Element Node
      
      if (strcmp(node->GetNodeName(), "Par1") == 0) {
        x = atof(node->GetText());
        p1->SetPar1(x);
      }
      
      if (strcmp(node->GetNodeName(), "Par2") == 0) {
        x = atof(node->GetText());
        p1->SetPar2(x);
      }
      
      if (strcmp(node->GetNodeName(), "Par3") == 0) {
        x = atof(node->GetText());
        p1->SetPar3(x);
      }
      
      if (strcmp(node->GetNodeName(), "Par4") == 0) {
        x = atof(node->GetText());
        p1->SetPar4(x);
      }
      
      if (strcmp(node->GetNodeName(), "Par5") == 0) {
        x = atof(node->GetText());
        p1->SetPar5(x);
      }
      
      if (strcmp(node->GetNodeName(), "Par6") == 0) {
        x = atof(node->GetText());
        p1->SetPar6(x);
      }

      if (strcmp(node->GetNodeName(), "Par7") == 0) {
        x = atof(node->GetText());
        p1->SetPar7(x);
      }
    }
  }
  
  return p1;
  
}
