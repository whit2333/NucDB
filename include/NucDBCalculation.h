#ifndef NucDBCalculation_HH
#define NucDBCalculation_HH 1
#include "TNamed.h"
#include "TList.h"
#include <iostream>
#include "NucDBMeasurement.h"
//#include "NucDBDataPoint.h"
#include "TGraphErrors.h"
#include "TCanvas.h"

/** Base class for a calculation object.
 * 
 * A calculation then includes all measured quantities as measurements.
 * The name should be the name used to query the database and the title
 * is used for display purposes.
 */
class NucDBCalculation : public TNamed {
   public:
      NucDBCalculation(const char * name ="unknowncalc",const char * title="unknown") : TNamed(name,title) {
         fMeasurements.Clear();
      }
      ~NucDBCalculation() { }

      NucDBMeasurement * GetMeasurement(const char * name) {
         NucDBMeasurement * meas = 0;
         for(int i = 0;i<fMeasurements.GetEntries();i++) {
            if( !strcmp(((NucDBMeasurement *)fMeasurements.At(i))->GetName(),name) ) {
               meas = (NucDBMeasurement *)fMeasurements.At(i) ;
               break;
            }
         }
         if(!meas) std::cout << " Calculation named " << name << " NOT FOUND!\n";
         return(meas);
      }

   protected: 
      TList fMeasurements;
      TList fFunctions;

   public:
      TString fPaper;
      TString fReference;   

   public:
      void AddMeasurement(NucDBMeasurement* meas){
         if(meas) meas->SetExperimentName(GetName());
         if(meas) fMeasurements.Add(meas);
      }

      void Print(Option_t * opt = "") const {
         std::cout << "===================================\n";
         std::cout << "   " << GetName() << "\n";
         std::cout << "===================================\n";
         std::cout << "  title = " << GetTitle() << "\n";
         std::cout << "+++++ Calculations +++++\n";
         fMeasurements.Print(opt);
      }

      //void PlotMeasurements(const char * var = "x") {
      //   TCanvas * c1 = new TCanvas( Form("%splots",GetName()),Form("%s plots",GetTitle() ) );
      //   c1->Divide(2,fMeasurements.GetEntries()/2);
      //   for(int i =0; i<fMeasurements.GetEntries();i++) {
      //      c1->cd(i+1);
      //      ((NucDBMeasurement*)fMeasurements.At(i))->BuildGraph(var);
      //      ((NucDBMeasurement*)fMeasurements.At(i))->fGraph->Draw("ap");
      //   }
      //}

      ClassDef(NucDBCalculation,1)
};



#endif
