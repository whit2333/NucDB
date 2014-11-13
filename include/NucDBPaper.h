#ifndef NucDBPapers_HH
#define NucDBPapers_HH
#include "TNamed.h"
#include "TList.h"
#include <iostream>
#include "NucDBCalculation.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TBrowser.h"

/** Representation of a published or unpublished Paper.
 * 
 *  
 */
class NucDBPaper : public TNamed {

   protected:
      TString   fJournal;
      TString   fDOI;
      TString   fAuthor;
      TString   fTitle;
      TString   fShortRef;
      TString   fBibtex;
      TString   fInSpire;
      TList     fCalculations;
      TString   fReference;
      TString   fComments;

   public:
      NucDBPaper(const char * name ="apaper",const char * title="unknown");
      virtual ~NucDBPaper();

      Bool_t IsFolder() const { return kTRUE; }
      void Browse(TBrowser* b) {
         b->Add(&fCalculations, "Calculations");
      }

      NucDBCalculation * GetCalculation(const char * name);
      const char * GetReference(){ return( fReference.Data() ); }


      void SetInSpire(const char * v)  { fInSpire = v;  }
      void SetAuthor(const char * v)   { fAuthor = v;   }
      void SetShortRef(const char * v) { fShortRef = v; }
      void SetBibtex(const char * v)   { fBibtex = v;   }
      void SetJournal(const char * v)  { fJournal = v;  }
      void SetDOI(const char * v)      { fDOI = v;      }
      void SetComments(const char * v) { fComments = v;      }

      const char * GetInSpire() const { return fInSpire.Data();  }
      const char * GetAuthor()  const { return fAuthor.Data();   }
      const char * GetShortRef()const { return fShortRef.Data(); }
      const char * GetBibtex()  const { return fBibtex.Data();   }
      const char * GetJournal() const { return fJournal.Data();  }
      const char * GetDOI()     const { return fDOI.Data();      }
      const char * GetComments()     const { return fComments.Data();      }


   public:
      void AddCalculation(NucDBCalculation* c){
         if(c){
            c->fPaper = GetName();
            c->fReference = GetReference();
            fCalculations.Add(c);
         }
      }
      void Print(Option_t * opt = "") const {
         std::cout << " Paper : " << GetName()  << std::endl;
         std::cout << "         " << GetTitle() << std::endl;
         for(int i =0; i<fCalculations.GetEntries();i++)
            ((NucDBCalculation*)fCalculations.At(i))->Print(opt);
      }


      //    void PlotCl(const char * var = "x"){
      //       TCanvas * c1 = new TCanvas( Form("%splots",GetName()),Form("%s plots",GetTitle() ) );
      //       c1->Divide(2,fCalculations.GetEntries()/2);
      //       for(int i =0; i<fCalculations.GetEntries();i++) {
      //           c1->cd(i+1);
      //           ((NucDBPaper*)fCalculations.At(i))->BuildGraph(var);
      //           ((NucDBPaper*)fCalculations.At(i))->fGraph->Draw("ap");
      //       }
      //    }

      ClassDef(NucDBPaper,2)
};




#endif
