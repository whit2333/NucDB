#include "NucDBReference.h"

TString wordWrap( TString s, size_t width ) {
   std::string str = s.Data();
   size_t curWidth = width;
   while( curWidth < str.length() ) {
      std::string::size_type spacePos = str.rfind( ' ', curWidth );
      if( spacePos == std::string::npos )
         spacePos = str.find( ' ', curWidth );
      if( spacePos != std::string::npos ) {
         str[ spacePos ] = '\n';
         curWidth = spacePos + width + 1;
      }
   }
   TString s_wrap = str.c_str();
   return s_wrap;
}

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
   Bool_t printComments = opt ? (!strcmp(opt, "comm") ? kTRUE : kFALSE) : kFALSE;
   std::cout << "----------------------------------------------------------" << std::endl;
   std::cout << "    Reference : " << GetName() << ", " << GetTitle() << std::endl;
   std::cout << "          DOI : " << fDOI.Data() << std::endl;
   std::cout << "          URL : " << fURL.Data() << std::endl;
   std::cout << "  Description : " << fDescription.Data() << std::endl;
   std::cout << "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl;
   if(printComments) PrintComments();
   fPapers.Print(opt);
}
//________________________________________________________________________________

