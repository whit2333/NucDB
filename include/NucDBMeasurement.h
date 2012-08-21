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
   Int_t    fColor;

protected:
   TList    fDataPoints;
   TList    fBinnedVariables;
   TList    fDependentVariables;
   TList    fGraphs;
   TString  fExperimentName;
   Int_t    fNumberOfDataPoints;

public:
   NucDBMeasurement(const char * name ="unknown-meas",const char * title="unknown meas");
   ~NucDBMeasurement();

   /** Necessary for Browsing */
   Bool_t  IsFolder() const { return kTRUE; }
   void    Browse(TBrowser* b) {
      b->Add(&fDataPoints,"Data Points");
      b->Add(&fGraphs,"Graphs");
      if(fGraph)b->Add(fGraph,"vs x");
   }

   void    ClearDataPoints();

   /** Add a data point to the list of datapoints */
   void    AddDataPoint(NucDBDataPoint *point);

   /** Adds a list data point to the list of datapoints with the option to
    *  clear exisiting datapoints
    */
   void    AddDataPoints(TList * listOfPoints, bool clear=false );

   /** Returns a list of datapoints falling in the bin.
    */
   TList * FilterWithBin(NucDBBinnedVariable const *bin);
   TList * GetDataPoints() {return(&fDataPoints);}

   void    Print() const ; // *MENU*
   void    PrintData() const ; // *MENU*
   
   TString GetExperimentName() {return(fExperimentName);}
   void    SetExperimentName(TString s){fExperimentName = s;}
   Int_t   GetNDataPoints(){ return(fNumberOfDataPoints);}
   void    SetColor(Int_t col){fColor = col;}
   Int_t   GetColor(){return(fColor);}

   /** Build a graph with errors */
   TGraphErrors * BuildGraph(const char * varName = "x"); // *MENU*
   TGraphErrors * fGraph; //->

   TList *              GetBinnedVariables() { return(&fBinnedVariables);}
   void                 AddBinnedVariable(NucDBBinnedVariable * var){fBinnedVariables.Add(var);}
   NucDBBinnedVariable* GetBinnedVariable(const char * name) {
      for(int i = 0;i<fBinnedVariables.GetEntries();i++) {
          if( !strcmp( ((NucDBBinnedVariable*)fBinnedVariables.At(i))->GetName(),name) ) 
             return((NucDBBinnedVariable*)fBinnedVariables.At(i));
      }
      return(0);
   }

   TList *              GetDependentVariables() { return(&fDependentVariables);}
   void                 AddDependentVariables(NucDBBinnedVariable * var){fDependentVariables.Add(var);}
   NucDBBinnedVariable* GetDependentVariable(const char * name) {
      for(int i = 0;i<fDependentVariables.GetEntries();i++) {
          if( !strcmp( ((NucDBBinnedVariable*)fDependentVariables.At(i))->GetName(),name) ) 
             return((NucDBBinnedVariable*)fDependentVariables.At(i));
      }
      return(0);
   }


ClassDef(NucDBMeasurement,1)
};



#endif

