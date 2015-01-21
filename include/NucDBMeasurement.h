#ifndef NucDBMeasurement_HH
#define NucDBMeasurement_HH

#include "TNamed.h"
#include "TList.h"
#include <iostream>
#include "NucDBDataPoint.h"
#include "TGraphErrors.h"
#include "TString.h"
#include "TObjString.h"
#include "TAxis.h"
#include "TBrowser.h"
//#include "NucDBPaper.h"
#include <vector>
#include <algorithm>
#include "NucDBReference.h"

/** A measured quantitiy.
 * 
 *  Many of these may fall under a single experiment, for example "F1p" and "F2p"
 *   may belong to SLAC143, while the measured ratio "F2C/F2D" would be apart of 
 *   the experiment EMC. Not "F2C" and "F2D" individually since it was the ratio
 *   that was measured.
 * 
 *  Name is used in the database queries and title is used for display purposes
 *  For example, name="F2p" and title="F_{2}^{p}".
 *
 * \todo develop (or adopt) a naming convention  
 *
 */
class NucDBMeasurement : public TNamed {
   private:
      Int_t    fColor;

   protected:
      TList    fDataPoints;           // data points
      //TList    fDiscreteVariables;    // discrete variables
      //TList    fBinnedVariables;    // binned variables
      //TList    fDependentVariables; // dependent (calculated) binned variables
      TList    fFilterBins;           //
      TList    fGraphs;               // graphs generated 
      TString  fExperimentName;       // experiment name
      Int_t    fNumberOfDataPoints;   //
      TList    fReferences;           // List of references
      TList    fComments;             // List of TObjString comments

   public:
      NucDBMeasurement(const char * name ="unknown-meas",const char * title="unknown meas");
      virtual ~NucDBMeasurement();

      NucDBMeasurement(const NucDBMeasurement& v);

      void PrintComments() const {
         TString prefix = " o ";
         for(int i = 0; i< fComments.GetEntries();i++){
            TObjString * comment = (TObjString*)fComments.At(i);
            TString wrapped = prefix;
            wrapped += comment->String();
            wrapped = wordWrap(wrapped,50);
            std::cout << wrapped << std::endl;
         }
      }
      void AddComment(const char * c){
         TObjString * comment = new TObjString(c);
         fComments.Add(comment);
      }
      TList * GetComments(){ return &fComments; }

      Bool_t  IsFolder() const { return kTRUE; }
      void    Browse(TBrowser* b);

      void    Clear(Option_t * opt = ""){
         ClearDataPoints();
         fReferences.Clear();
         fComments.Clear();
         fFilterBins.Clear();
         //fBinnedVariables.Clear();
         //fDependentVariables.Clear();
         fGraphs.Clear(); 
      }

      void    ClearDataPoints();

      /** Add a data point to the list of datapoints */
      void    AddDataPoint(NucDBDataPoint *point);

      /** Adds a list data point to the list of datapoints with the option to
       *  clear exisiting datapoints.
       */
      void    AddDataPoints(TList * listOfPoints, bool clear=false );

      /** Returns a list of datapoints falling in the bin.  */
      TList *  FilterWithBin(NucDBBinnedVariable const *bin);
      TList *  FilterWith(NucDBDiscreteVariable const *v);
      TList *  FilterWith(NucDBVariable const *v);


      /** Returns list of data points which have be merged. 
       *  The list is created on the heap and it is up to the user to free the memory.
       *  The number of datapoints merged for each new data point is provided in the argument.
       *  This is similar to rebinning.
       *  The second argument is the name of the variable by which the data points are first sorted
       *  before merging. This prevents far away bins from being merged.
       *  It is worth noting here that the FilterWithBin is very handy to apply before hand
       *  so that only a certain range of the data is left to merge.
       *  For example, you might only want a certin range of Q2 bins so you would filter the measurment
       *  first then merge the bins in x. 
       *  If modify is called, the measurement's data points are replaced with the merged datapoints.
       */ 
      TList *  MergeDataPoints(unsigned int n = 2, const char * var = "x", bool modify=false);
      void     SortBy(const char * n = "x");
      void     Multiply(const char * v );

      // The function names here kind of suck...
      NucDBMeasurement * CreateMeasurementFilteredWithBin(const NucDBBinnedVariable * bin) ;
      NucDBMeasurement * NewMeasurementWithFilter(const NucDBBinnedVariable * bin) ;

      /** Same as FilterWithBin(bin) but it modifies the measurement */
      TList *  ApplyFilterWithBin(NucDBBinnedVariable *bin);
      TList *  ApplyFilterWith(NucDBDiscreteVariable *v);
      TList *  ApplyFilterWith(NucDBVariable *v);


      TList * GetDataPoints() {return(&fDataPoints);}

      const TList& GetDataRef() const { return(fDataPoints);}

      void    Print(Option_t * opt ="") const ; // *MENU*
      void    PrintData(Option_t * opt = "") const ; // *MENU*
      void    ListVariables() ; // *MENU*
      const char * GetExperimentName() const {return(fExperimentName.Data());}
      void    SetExperimentName(const char * s){fExperimentName = s;}
      Int_t   GetNDataPoints() const {return(fNumberOfDataPoints);}
      void    SetColor(Int_t col){fColor = col;}
      Int_t   GetColor() const {return(fColor);}

      const TList *  GetRefs() const { return &fReferences ; }
      void           AddRef(NucDBReference * r) { fReferences.Add(r); }

      /** Build a graph with errors */
      TGraphErrors * BuildGraph(const char * varName = "x"); // *MENU*
      TGraphErrors * BuildKinematicGraph(const char * var1Name = "x", const char * var2Name = "Qsquared"); // *MENU*
      TGraphErrors * fGraph; //->

      // Uses only the first datapoint
      //const TList *        GetBinnedVariables() const { return(&fBinnedVariables);}
      //void                 AddBinnedVariable(NucDBBinnedVariable * var){fBinnedVariables.Add(var);}
      NucDBBinnedVariable* GetBinnedVariable(const char * name);

      // Uses only the first datapoint
      //const TList *          GetDiscreteVariables() const { return(&fDiscreteVariables);}
      //void                   AddDiscreteVariable(NucDBDiscreteVariable * var){fDiscreteVariables.Add(var);}
      NucDBDiscreteVariable* GetDiscreteVariable(const char * name);

      //const TList *        GetDependentVariables() const { return(&fDependentVariables);}
      Int_t                 AddDependentVariable(NucDBDependentVariable * var);
      NucDBDependentVariable* GetDependentVariable(const char * name);

      Double_t  GetBinnedVariableMean(const char * name);
      Double_t  GetBinnedVariableVariance(const char * name);
      Double_t  GetBinnedVariableMax(const char * name);
      Double_t  GetBinnedVariableMin(const char * name);

      /** fills the vector with unique values of the binned variable. */ 
      Int_t GetUniqueBinnedVariableValues(const char * name, std::vector<double> & vect) const;
      Int_t GetUniqueBinnedVariableValues(const char * name, std::vector<double> & vect, std::vector<int> & counts) const;
      void  PrintBreakDown(const char * var = "theta", int nmax = 20) const ;


   protected:

      //Int_t     CalculateVariable(NucDBDependentVariable * var);
      
   public:


      const TList *              GetGraphs() const { return(&fGraphs);}

      //TList  * GetPapers(){ return(&fPapers); }
      //void     AddPapers(NucDBPaper * p) { fPapers.Add(p); }

      ClassDef(NucDBMeasurement,1)
};



#endif

