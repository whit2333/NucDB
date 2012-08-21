#ifndef NucDBPapers_HH
#define NucDBPapers_HH
#include "TNamed.h"
#include "TList.h"
#include <iostream>
#include "NucDBCalculation.h"
#include "NucDBDataPoint.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TBrowser.h"

/** Representation of a published or unpublished Paper.
 * 
 *  
 */
class NucDBPaper : public TNamed {
public:
   NucDBPaper(const char * name ="apaper",const char * title="unknown"):TNamed(name,title) {
      fCalculations.Clear();
   }
   ~NucDBPaper(){}
   /** Necessary for Browsing */
   Bool_t IsFolder() const {
      return kTRUE;
   }
   /** Needed to make object browsable. */
   void Browse(TBrowser* b) {
       b->Add(&fCalculations, "Calculations");
   }

   NucDBCalculation * GetCalculation(const char * name) {
      NucDBCalculation * meas = 0;
      for(int i = 0;i<fCalculations.GetEntries();i++) {
         if( !strcmp(((NucDBCalculation *)fCalculations.At(i))->GetName(),name) ) {
            meas = (NucDBCalculation *)fCalculations.At(i) ;
            break;
         }
      }
      if(!meas) std::cout << " Calculation named " << name << " NOT FOUND!\n";
      return(meas);
   }

   const char * GetReference(){ return( fReference.Data() ); }

protected: 
   TList   fCalculations;
   TString fReference;
   TString fAuthor;
   TString fijef;

public:
   void AddCalculation(NucDBCalculation* c){
      if(c){
	 c->fPaper = GetName();
         c->fReference = GetReference();
         fCalculations.Add(c);
      }
   }

   void Print(){
      std::cout << "===================================\n";
      std::cout << "   " << GetName() << "\n";
      std::cout << "===================================\n";
      std::cout << "  title = " << GetTitle() << "\n";
      std::cout << "+++++ Calculations +++++\n";
      for(int i =0; i<fCalculations.GetEntries();i++)
          ((NucDBCalculation*)fCalculations.At(i))->Print();
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

ClassDef(NucDBPaper,1)
};




#endif
