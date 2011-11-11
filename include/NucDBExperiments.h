#ifndef NucDBExperiments_HH
#define NucDBExperiments_HH
#include "TNamed.h"
#include "TList.h"
#include <iostream>
#include "NucDBMeasurement.h"
#include "NucDBDataPoint.h"
#include "TGraphErrors.h"
#include "TCanvas.h"

/** Base class for an experiment
 * 
 * An experiment includes all measured quantities.
 * The name should be the name used to query the database and the title
 * is used for display purposes.
 */
class NucDBExperiment : public TNamed {
public:
   NucDBExperiment(const char * name ="unknownexp",const char * title="unknown"):TNamed(name,title) {
      fMeasurements.Clear();
   }
   ~NucDBExperiment(){}

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


   
protected: 
   TList fMeasurements;
public:
   void AddMeasurement(NucDBMeasurement* meas){
      if(meas) meas->fExperiment = GetName();
      if(meas) fMeasurements.Add(meas);
   }


   void Print(){
      std::cout << "===================================\n";
      std::cout << "   " << GetName() << "\n";
      std::cout << "===================================\n";
      std::cout << "  title = " << GetTitle() << "\n";
      std::cout << "+++++ Measurements +++++\n";
      for(int i =0; i<fMeasurements.GetEntries();i++)
          ((NucDBMeasurement*)fMeasurements.At(i))->Print();
   }


   void PlotMeasurements(){
      TCanvas * c1 = new TCanvas( Form("%splots",GetName()),Form("%s plots",GetTitle() ) );
      c1->Divide(2,2);
      for(int i =0; i<fMeasurements.GetEntries();i++) {
          c1->cd(i+1);
          ((NucDBMeasurement*)fMeasurements.At(i))->BuildGraph();
          ((NucDBMeasurement*)fMeasurements.At(i))->fGraph->Draw("ap");
      }
   }

ClassDef(NucDBExperiment,1)
};




#endif