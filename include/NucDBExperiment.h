#ifndef NucDBExperiments_HH
#define NucDBExperiments_HH
#include "TNamed.h"
#include "TList.h"
#include <iostream>
#include "NucDBMeasurement.h"
#include "NucDBDataPoint.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TBrowser.h"

/** Base class for an experiment object.
 * 
 * An experiment then includes all measured quantities as measurements.
 * The name should be the name used to query the database and the title
 * is used for display purposes.
 */
class NucDBExperiment : public TNamed {
protected: 
   TList fMeasurements;
   TList fPapers;

public:
   NucDBExperiment(const char * name ="unknownexp",const char * title="unknown");
   virtual ~NucDBExperiment();

   /** Necessary for Browsing */
   Bool_t IsFolder() const { return kTRUE; }
   void Browse(TBrowser* b) {
      b->Add(&fMeasurements, "Measurements");
      b->Add(&fPapers, "Papers");
   }

   NucDBMeasurement * GetMeasurement(const char * name);
   TList *            GetMeasurements(){ return(&fMeasurements); }

   /** Returns a new list containing all the refs from each measurement. 
    */
   TList * GetRefs(){
      TList * refs = new TList();
      NucDBMeasurement * m = 0;
      for(int i = 0;i<fMeasurements.GetEntries();i++){
         m = (NucDBMeasurement*)fMeasurements.At(i);
         if(m->GetRefs()->GetEntries()>0)refs->AddAll(m->GetRefs());
      }
      return(refs);
   }


   void AddMeasurement(NucDBMeasurement* meas);
   void Print(); // *MENU*
   void PlotMeasurements(const char * var = "x"); // *MENU*

ClassDef(NucDBExperiment,1)
};




#endif
