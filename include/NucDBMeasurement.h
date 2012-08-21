#ifndef NucDBMeasurement_HH
#define NucDBMeasurement_HH
#include "TNamed.h"
#include "TList.h"
#include <iostream>
#include "NucDBDataPoint.h"
#include "TGraphErrors.h"
#include "TString.h"
#include "TAxis.h"
#include "TBrowser.h"

/** A measured quantitiy.
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
private:

protected:
   Int_t    fColor;
   TList    fDataPoints;
   TList    fGraphs;
   TString  fExperimentName;
   Int_t    fNumberOfDataPoints;

public:
   NucDBMeasurement(const char * name ="unknown-meas",const char * title="unknown meas");
   ~NucDBMeasurement();

   /** Necessary for Browsing */
   Bool_t IsFolder() const { return kTRUE; }
   void Browse(TBrowser* b) {
      b->Add(&fDataPoints,"Data Points");
      b->Add(&fGraphs,"Graphs");
   }

   void ClearDataPoints();

   /** Add a data point to the list of datapoints */
   void AddDataPoint(NucDBDataPoint *point);

   /** Adds a list data point to the list of datapoints with the option to
    *  clear exisiting datapoints
    */
   void AddDataPoints(TList * listOfPoints, bool clear=false );

   /** Returns a list of datapoints falling in the bin.
    */
   TList *  FilterWithBin(NucDBBinnedVariable const *bin);

   TList * GetDataPoints() {return(&fDataPoints);}

   void Print() const ; // *MENU*
   void PrintData() const ; // *MENU*
   
   TString GetExperimentName() {return(fExperimentName);}
   void    SetExperimentName(TString s){fExperimentName = s;}

   /** Build a graph with errors */
   TGraphErrors * BuildGraph(const char * varName = "x"); //*MENU*
   TGraphErrors * fGraph; //->

ClassDef(NucDBMeasurement,1)
};



#endif

