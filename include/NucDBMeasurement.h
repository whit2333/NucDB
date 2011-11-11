#ifndef NucDBMeasurement_HH
#define NucDBMeasurement_HH
#include "TNamed.h"
#include "TList.h"
#include <iostream>
#include "NucDBDataPoint.h"
#include "TGraphErrors.h"
#include "TString.h"
#include "TAxis.h"

/** A measured quantitiy
 * 
 *  Many of these may fall under a single experiment, for example "F1p" and "F2p"
 *   may belong to SLAC143, while the measured ratio "F2C/F2D" would be apart of 
 *   the experiment EMC. Not "F2C" and "F2D" individually since it was the ratio
 *   that was measured.
 * 
 *  Name is used in the database queries and title is used for display purposes
 *  For example, name="F2p" and title="F_{2}^{p}".
 */
class NucDBMeasurement : public TNamed {
public:
   NucDBMeasurement(const char * name ="unknownexp",const char * title="unknown")
     :TNamed(name,title) {
      fNumberOfDataPoints=0;
      fDataPoints.Clear();
      fGraph=0;
      fColor=1;
      fExperiment="";
   }
   ~NucDBMeasurement(){ }

   Int_t fColor;

   void ClearDataPoints(){
      fDataPoints.Clear();
      fNumberOfDataPoints=0;
   }

   /** Add a data point to the list of datapoints */
   void AddDataPoint(NucDBDataPoint *point) {
      if(point) {
         fDataPoints.Add(point);
         fNumberOfDataPoints++;
      } else {
         printf(" NULL NucDBDataPoint pointer \n");
      } 
   }

   /** Adds a list data point to the list of datapoints with the option to
    *  clear exisiting datapoints
    */
   void AddDataPoints(TList * listOfPoints, bool clear=false ) {
      if(listOfPoints) {
         if(clear) ClearDataPoints();
         for(int i = 0; i < listOfPoints->GetEntries();i++) 
            AddDataPoint((NucDBDataPoint*)listOfPoints->At(i));
      } else {
         printf(" NULL TList pointer \n");
      } 
   }

   /** Returns a list of datapoints falling in the bin */
   TList *  FilterWithBin(NucDBBinnedVariable const *bin) {
      TList * list = new TList();
      list->Clear();
      if(!bin) {
         printf(" NULL NucDBBinnedVariable pointer. Returning list with no entries. \n");
         return list;
      }
      for(int i = 0; i < fDataPoints.GetEntries();i++) {
         NucDBDataPoint * point = (NucDBDataPoint*)fDataPoints.At(i);
         NucDBBinnedVariable * var = point->GetBinVariable(bin->GetName());
         if(var){
            if ( (*var) == (*bin) ) list->Add(point);
         }
      }
      return list;
   }

   Int_t fNumberOfDataPoints;
   TList & GetDataPoints() {return(fDataPoints);}

   TString fExperiment;

protected : 
   TList fDataPoints;

public: 
   void Print(){
      std::cout << "   --------------------------\n";
      std::cout << "   " << GetName() << "\n";
      std::cout << "   --------------------------\n";
      std::cout << "  title = " << GetTitle() << "\n";
      std::cout << "  fNumberOfDataPoints = " << fNumberOfDataPoints <<  "\n";
   }

   /** Build a graph with errors */
   TGraphErrors * BuildGraph(const char * varName = "x") {
      if(fGraph) delete fGraph;
      fGraph=0;
      fGraph= new TGraphErrors(fNumberOfDataPoints);
      for(int i = 0; i < fNumberOfDataPoints;i++) {
         NucDBDataPoint *point = (NucDBDataPoint *) fDataPoints.At(i);
         NucDBBinnedVariable * var = point->GetBinVariable(varName);
         if(i==0) fGraph->GetXaxis()->SetTitle(varName);
         fGraph->SetPoint(i,var->fMean,point->fValue);
         fGraph->SetPointError(i,0.0,point->fTotalError.GetError());
      }
      fGraph->SetTitle(GetTitle());
      fGraph->SetMarkerColor(fColor);
      fGraph->SetLineColor(fColor);
      fGraph->SetMarkerStyle(20);
      fGraph->SetLineStyle(1);
      fGraph->SetLineWidth(2);
      
      return(fGraph);
   }

   TGraphErrors * fGraph; //->

ClassDef(NucDBMeasurement,1)
};



#endif

