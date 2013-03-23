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
      enum ERefType { kUnknown, kWebsite, kPublished, kPreprint, kPreliminary, kPrivateComm};

   protected:
      TString   fURL;
      TString   fDOI;
      TString   fDescription; 
      ERefType  fRefType;
      TList     fPapers;

   public:
      NucDBReference(const char * n = "", const char * t = "");
      ~NucDBReference();
      /** Necessary for Browsing */
      Bool_t  IsFolder() const { return kTRUE; }
      void    Browse(TBrowser* b) {
         b->Add(&fPapers,"Papers");
      }
      void Print() const ; // *MENU*

      const char * GetURL() const { return fURL.Data() ; }
      const char * GetDOI() const { return fDOI.Data() ; }
      const char * GetDescription() const { return fDescription.Data() ; }
      ERefType     GetRefType() const { return fRefType; }  
      TList      * GetPapers() { return &fPapers ; }

      void         SetURL(const char * s) { fURL = s; }
      void         SetDOI(const char * s) { fDOI = s; }
      void         SetDescription(const char * s) { fDescription = s; }
      void         SetRefType(ERefType t) { fRefType = t; }

ClassDef(NucDBReference,1)
};

#endif
