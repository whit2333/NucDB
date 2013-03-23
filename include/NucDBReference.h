#ifndef NucDBReference_HH
#define NucDBReference_HH 1

#include "NucDBPaper.h"
#include "TString.h"
#include "TObject.h"

/** Reference for a data set.
 *   
 */
class NucDBReference : public TNamed {
   public:
      enum ERefType { kUnknown, kWeb, kPublished, kPreprint, kPreliminary, kPrivateComm};

   protected:
      TString   fURL;
      TString   fDOI;
      TString   fDescription; 
      ERefType  fRefType;

   public:
      NucDBReference(const char * n = "", const char * t = ""): TNamed(n,t) {
         fRefType     = kUnknown;
         fURL         = "";
         fDescription = "";
         fDOI         = "";
      }
      ~NucDBReference(){}

      const char * GetURL() const { return fURL.Data() ; }
      const char * GetDOI() const { return fDOI.Data() ; }
      ERefType     GetRefType() const { return fRefType; }  

      void         SetURL(const char * s) { fURL = s; }
      void         SetDOI(const char * s) { fDOI = s; }
      void         SetRefType(ERefType t) { fRefType = t; }

ClassDef(NucDBReference,1)
};

#endif
