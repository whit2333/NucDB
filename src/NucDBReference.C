#include "NucDBReference.h"

ClassImp(NucDBReference)

NucDBReference::NucDBReference(const char * n , const char * t ): TNamed(n,t) {
   fRefType     = kUnknown;
   fURL         = "";
   fDescription = "";
   fDOI         = "";
   fPapers.SetName("Papers");
}
//________________________________________________________________________________
NucDBReference::~NucDBReference(){

}
//________________________________________________________________________________

void NucDBReference::Print(Option_t * opt) const {
   std::cout << "----------------------------------------------------------" << std::endl;
   std::cout << "    Reference : " << GetName() << ", " << GetTitle() << std::endl;
   std::cout << "          DOI : " << fDOI.Data() << std::endl;
   std::cout << "          URL : " << fURL.Data() << std::endl;
   std::cout << "  Description : " << fDescription.Data() << std::endl;
   std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
   fPapers.Print(opt);
}
//________________________________________________________________________________

