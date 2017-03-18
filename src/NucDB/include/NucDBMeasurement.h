#ifndef NucDBMeasurement_HH
#define NucDBMeasurement_HH

#include "TNamed.h"
#include "TList.h"
#include <iostream>
#include <functional>
#include "NucDBDataPoint.h"
#include "TGraphErrors.h"
#include "TString.h"
#include "TObjString.h"
#include "TAxis.h"
#include "TBrowser.h"
//#include "NucDBPaper.h"
#include <functional>
#include <vector>
#include <algorithm>
#include "NucDBReference.h"
#include "TMultiGraph.h"
#include "TLegend.h"
#include "TH1.h"
#include "NucDBUtil.h"


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
class NucDBMeasurement : public TNamed, public TAttLine, public TAttFill, public TAttMarker {

   public:

   private:

      NucDB::Type                  fType;     
      std::vector<NucDB::Process>  fProcesses; // Can full under more than one process 
      // e.g. fProcesses = {DVCS, Exclusive};
      
      // Sets are logical groupings of data.
      std::vector<std::string>  fSets;
      int                       fNSets = 0; // Current number of sets

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

      int  GetNSets() const { return fSets.size(); }
      void AddSet(const char * desc = "") {
         fSets.push_back( std::string(desc) );
         fNSets++;
      }

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

      /** Remove a datapoint from the list of points. 
       */
      void    RemoveDataPoint(NucDBDataPoint *point);

      /** Adds a list data point to the list of datapoints with the option to
       *  clear exisiting datapoints.
       */
      void    AddDataPoints(TList * listOfPoints, bool clear=false );
      void    AddDataPoints(std::vector<NucDBDataPoint*> listOfPoints, bool clear=false );

      /** Remove a list of datapoints from the list of points. 
       */
      void    RemoveDataPoints(TList * listOfPoints);

      /** Returns a list of datapoints falling in the bin.  */
      TList *  FilterWithBin(NucDBBinnedVariable const *bin);

      /** Returns a list of datapoints equal to variable.  */
      TList *  FilterWith(NucDBDiscreteVariable const *v);

      /** Returns a list of datapoints equal to variable.  */
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
      TList *  MergeDataPoints(const std::vector<int>  & points, bool modify=false);

      /** Merge datapoints that fall in the bin.
       *  Returns the datapoint of all the points merged. 
       *  If modify is true, the merged data points are removed from the measurment
       *  and the new data point is added.
       */
      NucDBDataPoint * MergeDataPoints(
            NucDBBinnedVariable * var, 
            NucDBBinnedVariable * var2 = nullptr, 
            bool                  modify = false);

      /** Similar to MergeDataPoints(unsigned int n, const char * var, bool modify). However, 
       *  only merges bins if the mean of two adjacent bins are within dx of each other.
       */
      TList *  MergeNeighboringDataPoints(
            unsigned int  n      = 2,
            const char *  var    = "x",
            double        dx     = 0.0001,
            bool          modify = false);

      /** Same as above but with the addtional requirement that 
       *  the second variable is also in the range.
       */
      TList *  MergeNeighboringDataPoints(
            unsigned int   n      ,
            const char *   varname,
            double         dx     ,
            const char *   varname2,
            double         dx2    ,
            bool           modify = false);


      TList * MergeDataPoints(
            unsigned int n, 
            const std::vector<NucDBBinnedVariable>& bin_sizes, 
            bool modify);

         /** Sort the datapoints by the supplied variable.
          *  TODO: Fix to be a stable sort such that the order is preserved.
          */ 
      void     SortBy(const char * n = "x", const char * n2 = "");
      void     SortBy(const std::vector<std::string> & names);

      /** Multiply the datapoint by the value of the variable supplied.  */
      void     Multiply(const char * v );

      /** Use the binned variable supplied to select data for a a new measurement.
       *  The bins are compared using NucDBBinnedVariable::operator==(). See FilterWithBin() for more details.
       *  TODO fix the name of this method.
       */ 
      NucDBMeasurement * CreateMeasurementFilteredWithBin(const NucDBBinnedVariable * bin) ;

      /** Use the binned variable supplied to select data for a a new measurement.
       *  The bins are compared using NucDBBinnedVariable::operator==(). See FilterWithBin() for more details.
       *  TODO fix the name of this method.
       */ 
      NucDBMeasurement * NewMeasurementWithFilter(const NucDBBinnedVariable * bin) ;


      /** Uses the vector of values to filter the data, creating a new measurment for each value.
       *  This can be used with GetUniqueBinnedVariableValues(), as is done in the method wich only takes a variable name.
       */
      TList *  CreateMeasurementsWithUniqueBins(const std::vector<double> & vect,const char * var = "x");
      TList *  CreateMeasurementsWithUniqueBins(const char * var = "x");

      /** Simply create a new measurement for each dataset.
       */
      std::vector<NucDBMeasurement*> CreateDataSetMeasurements() const;
      NucDBMeasurement*              GetDataSet(int i_set) const;
      std::vector<NucDBDataPoint*>   GetDataSetPoints(int set) const;


      /** Same as FilterWithBin(bin) but it modifies the measurement.  */
      TList *  ApplyFilterWithBin(NucDBBinnedVariable *bin);
      TList *  ApplyFilterWith(NucDBDiscreteVariable *v);
      TList *  ApplyFilterWith(NucDBVariable *v);

      /** Transform the data set by calling the supplied function for each data point.
       */
      void  TransformDataPoints(std::function<void(NucDBDataPoint*)> func);

      TList * GetDataPoints() {return(&fDataPoints);}

      NucDBDataPoint * GetDataPoint(int i){return( (NucDBDataPoint*)fDataPoints.At(i) );}

      const TList& GetDataRef() const { return(fDataPoints);}

      /** Print information about the experiment. 
       *  Options are:
       *     - "v" prints variables
       *     - "ref" prints the references
       *     - "comm" prints comments
       *     - "data" calls print for all the datapoints
       */
      void    Print(     Option_t * opt = "") const ; // *MENU*
      void    PrintData( Option_t * opt = "") const ; // *MENU*

      void    PrintTable(std::ostream& stream) const ;

      void    ListVariables() ; // *MENU*
      const char * GetExperimentName() const {return(fExperimentName.Data());}
      void    SetExperimentName(const char * s){fExperimentName = s;}
      Int_t   GetNDataPoints() const {return(fNumberOfDataPoints);}
      void    SetColor(Int_t col){SetMarkerColor(col);SetFillColor(col);SetLineColor(col);}
      Int_t   GetColor() const {return(GetMarkerColor());}

      const TList *  GetRefs() const { return &fReferences ; }
      void           AddRef(NucDBReference * r) { fReferences.Add(r); }

      /** Build a graph with errors */
      TGraphErrors * BuildGraph(const char * varName = "x", bool syst_err = false); // *MENU*
      TGraphErrors * BuildSystematicErrorBand(const char * var = "x", double offset = 0.0); // *MENU*

      TGraphErrors * BuildOrderedGraph(const char * varName = "", bool syst_err = false); // *MENU*

      /** This version builds an error band but multiplies by the fn argument.
       *  The function is function of the varName variable.
       *
       *  For example, this can be  used to take the error band of g1(x) to an
       *  error gand in x^2*g1(x).
       */
      TGraphErrors * BuildSystematicErrorBand(const char * varName, std::function<double(double)> f  , double offset = 0.0);

      TGraph * BuildKinematicGraph(const char * var1Name = "x", const char * var2Name = "Qsquared"); // *MENU*

      TMultiGraph * BuildGraphUnique(const char * var = "x", const char * uniqueVar = "Qsquared", TLegend * leg = nullptr ); // *MENU*

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

      /** Fills the vector with unique values of the binned variable. 
       *  TODO: Better name.
       */ 
      Int_t GetUniqueBinnedVariableValues(const char * name, std::vector<double> & vect) const;

      /** Fills the vector with unique values of the binned variable. 
       *  TODO: Better name.
       */ 
      Int_t GetUniqueBinnedVariableValues(const char * name, std::vector<double> & vect, std::vector<int> & counts) const;

      /** Prints a simple breakdown of the data interms of unique values of var.
       *  The number of rows printed is limited to nmax.
       */
      void  PrintBreakDown(const char * var = "theta", int nmax = 20) const ;

      void SetType(NucDB::Type t)                      { fType = t; }
      void AddProcess(NucDB::Process p)                { fProcesses.push_back(p); }
      void SetProcesses(std::vector<NucDB::Process> p) { fProcesses = p ; }
      void ClearProcesses()                            { fProcesses.clear(); }

      NucDB::Type                 GetType() const { return fType; }
      std::vector<NucDB::Process> GetProcesses() const { return fProcesses; }
      bool IsType(NucDB::Type t)       const { return( t==fType ? true : false ) ; }
      bool IsProcess(NucDB::Process p) const {
         using namespace std;
         return(find(begin(fProcesses), end(fProcesses), p) != end(fProcesses) ? true : false ) ;
      }


   protected:

      //Int_t     CalculateVariable(NucDBDependentVariable * var);
      
   public:


      const TList *              GetGraphs() const { return(&fGraphs);}

      //TList  * GetPapers(){ return(&fPapers); }
      //void     AddPapers(NucDBPaper * p) { fPapers.Add(p); }



      ClassDef(NucDBMeasurement,3)
};



#endif

