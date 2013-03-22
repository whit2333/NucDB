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
#include "NucDBPaper.h"

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
   TList    fPapers;

public:
   NucDBMeasurement(const char * name ="unknown-meas",const char * title="unknown meas");
   virtual ~NucDBMeasurement();
   NucDBMeasurement(const NucDBMeasurement& v){
      SetNameTitle(v.GetName(),v.GetTitle());
      fColor = v.GetColor();
      SetExperimentName(v.GetExperimentName());
      fNumberOfDataPoints = v.fNumberOfDataPoints;

      fDataPoints.AddAll(v.GetDataPoints());
      fBinnedVariables.AddAll(v.GetBinnedVariables());
      fDependentVariables.AddAll(v.GetDependentVariables());
      fGraphs.AddAll(v.GetGraphs());
   }

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
   const TList * GetDataPoints() const {return(&fDataPoints);}

   void    Print() const ; // *MENU*
   void    PrintData() const ; // *MENU*
   void    ListVariables() ; // *MENU*
   TString GetExperimentName() const {return(fExperimentName);}
   void    SetExperimentName(TString s){fExperimentName = s;}
   Int_t   GetNDataPoints(){ return(fNumberOfDataPoints);}
   void    SetColor(Int_t col){fColor = col;}
   Int_t   GetColor() const {return(fColor);}

   /** Build a graph with errors */
   TGraphErrors * BuildGraph(const char * varName = "x"); // *MENU*
   TGraphErrors * BuildKinematicGraph(const char * var1Name = "x", const char * var2Name = "Qsquared"); // *MENU*
   TGraphErrors * fGraph; //->

   const TList *        GetBinnedVariables() const { return(&fBinnedVariables);}
   void                 AddBinnedVariable(NucDBBinnedVariable * var){fBinnedVariables.Add(var);}
   NucDBBinnedVariable* GetBinnedVariable(const char * name) {
      for(int i = 0;i<fBinnedVariables.GetEntries();i++) {
          if( !strcmp( ((NucDBBinnedVariable*)fBinnedVariables.At(i))->GetName(),name) ) 
             return((NucDBBinnedVariable*)fBinnedVariables.At(i));
      }
      return(0);
   }

   /** Returns the mean value of the variable. */
   Double_t GetBinnedVariableMean(const char * name) {
      Double_t tot = 0;
      Int_t N = 0;
      const TList * datapoints = GetDataPoints();
      NucDBDataPoint * p = 0;
      NucDBBinnedVariable * v = 0;
      for(int i = 0;i<datapoints->GetEntries();i++) {
         p = (NucDBDataPoint*)datapoints->At(i);
         if(p) {
            v = (NucDBBinnedVariable*)p->GetBinVariable(name);
            if(v){
               tot += v->GetMean();
               N++;
            }
         }
      }
      if(N>0) return( tot/double(N) );
      return(0);
   }

   /** Returns the mean value of the variable. */
   Double_t GetBinnedVariableVariance(const char * name) {
      Double_t mean = GetBinnedVariableMean(name);
      Double_t tot = 0;
      Int_t N = 0;
      const TList * datapoints = GetDataPoints();
      NucDBDataPoint * p = 0;
      NucDBBinnedVariable * v = 0;
      for(int i = 0;i<datapoints->GetEntries();i++) {
         p = (NucDBDataPoint*)datapoints->At(i);
         if(p) {
            v = (NucDBBinnedVariable*)p->GetBinVariable(name);
            if(v){
               Double_t vmean = v->GetMean();
               tot += (vmean*vmean);
               N++;
            }
         }
      }
      if(N>0) return( tot/double(N) - mean*mean );
      return(0);
   }

   const TList *        GetDependentVariables() const { return(&fDependentVariables);}
   void                 AddDependentVariables(NucDBBinnedVariable * var){fDependentVariables.Add(var);}
   NucDBBinnedVariable* GetDependentVariable(const char * name) {
      for(int i = 0;i<fDependentVariables.GetEntries();i++) {
          if( !strcmp( ((NucDBBinnedVariable*)fDependentVariables.At(i))->GetName(),name) ) 
             return((NucDBBinnedVariable*)fDependentVariables.At(i));
      }
      return(0);
   }

   const TList *              GetGraphs() const { return(&fGraphs);}

   TList  * GetPapers(){ return(&fPapers); }
   void     AddPapers(NucDBPaper * p) { fPapers.Add(p); }

ClassDef(NucDBMeasurement,1)
};



#endif

