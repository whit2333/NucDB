#ifndef NucDBReference_HH
#define NucDBReference_HH 1

#include <iostream>
#include <string>
#include "TString.h"
#include "TObjString.h"
#include "TList.h"
#include "TNamed.h"
#include "TBrowser.h"

TString wordWrap( TString s, size_t width = 55 );

/** Reference for a data set.
 *   
 */
class NucDBReference : public TNamed {

   public:
      enum RefType { kUnknown, kWebsite, kPublished, kPreprint, kPreliminary, kPrivateComm};

   protected:
      TString   fURL;
      TString   fDOI;
      TString   fDescription; 
      RefType   fRefType;
      TList     fPapers;
      TList     fComments;

   public:
      NucDBReference(const char * n = "", const char * t = "");
      virtual ~NucDBReference();

      /** Necessary for Browsing */
      Bool_t  IsFolder() const { return kTRUE; }
      void    Browse(TBrowser* b) {
         b->Add(&fPapers,"Papers");
      }
      void Print(Option_t * opt = "") const ; // *MENU*

      const char * GetURL() const { return fURL.Data() ; }
      const char * GetDOI() const { return fDOI.Data() ; }
      const char * GetDescription() const { return fDescription.Data() ; }
      RefType      GetRefType() const { return fRefType; }  
      TList      * GetPapers() { return &fPapers ; }

      void         SetURL(const char * s) { fURL = s; }
      void         SetDOI(const char * s) { fDOI = s; }
      void         SetDescription(const char * s) { fDescription = s; }
      void         SetRefType(RefType t) { fRefType = t; }

      void PrintComments() const {
         TString prefix = " o ";
         for(int i = 0; i< fComments.GetEntries();i++){
            auto * comment = (TObjString*)fComments.At(i);
            TString wrapped = prefix;
            wrapped += comment->String();
            wrapped = wordWrap(wrapped,50);
            std::cout << wrapped << std::endl;
         }
      }
      void AddComment(const char * c){
         auto * comment = new TObjString(c);
         fComments.Add(comment);
      }
      TList * GetComments(){ return &fComments; }

ClassDef(NucDBReference,1)
};

#endif

