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
   NucDBExperiment(const char * name ="unknownexp",const char * title="unknown"):TNamed(name,title) {
      fMeasurements.Clear();
   }
   ~NucDBExperiment(){}

   /** Necessary for Browsing */
   Bool_t IsFolder() const {
      return kTRUE;
   }
   /** Needed to make object browsable. */
   void Browse(TBrowser* b) {
      b->Add(&fMeasurements, "Measurements");
      b->Add(&fPapers, "Papers");
   }

   NucDBMeasurement * GetMeasurement(const char * name) {
      NucDBMeasurement * meas = 0;
      for(int i = 0;i<fMeasurements.GetEntries();i++) {
         if( !strcmp(((NucDBMeasurement *)fMeasurements.At(i))->GetName(),name) ) {
            meas = (NucDBMeasurement *)fMeasurements.At(i) ;
            break;
         }
      }
      if(!meas) std::cout << " Measurement named " << name << " NOT FOUND!\n";
      return(meas);
   }

   TList * GetMeasurements(){ return(&fMeasurements); }

public:
   void AddMeasurement(NucDBMeasurement* meas){
      if(meas) meas->fExperiment = GetName();
      if(meas) fMeasurements.Add(meas);
   }

   void Print(); // *MENU*
   void PlotMeasurements(const char * var = "x"); // *MENU*

ClassDef(NucDBExperiment,1)
};




#endif
