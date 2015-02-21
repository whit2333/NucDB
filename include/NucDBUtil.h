#ifndef NucDBUtil_HH
#define NucDBUtil_HH 1

class TList;
class TLegend;
class TMultiGraph;
class NucDBDataPoint;
class NucDBBinnedVariable;
class NucDBMeasurement;

namespace NucDB {

   void ApplyFilterOnList(NucDBBinnedVariable * var, TList * list);


   TMultiGraph * CreateMultiGraph(TList * list, const char * var);

   void FillLegend(TLegend * leg, TList * list, TMultiGraph * mg );

   /** From the list of measurement for the experiment name.
    *  Returns the first one found.
    */
   NucDBMeasurement * GetExperiment(const char * exp_name, TList * meas_list);

   /** Merge all the datapoints in the list into a single data point.
    */
   NucDBDataPoint *  MergeDataPoints(TList * points);

   /** Filter the list of datapoints with the bin.
    */
   TList * FilterDataPoints(TList * list, NucDBBinnedVariable * var);

   bool CompareDataPoint(const NucDBDataPoint *lhs, const NucDBDataPoint *rhs) ;
   void StableSort(TList * list) ; 

}


#endif
