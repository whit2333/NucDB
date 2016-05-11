#ifndef NucDBUtil_HH
#define NucDBUtil_HH 1

#include <vector>

class TList;
class TLegend;
class TMultiGraph;
class NucDBDataPoint;
class NucDBBinnedVariable;
class NucDBExperiment;
class NucDBMeasurement;


namespace NucDB {

   enum class Type;

   void ApplyFilterOnList(NucDBBinnedVariable * var, TList * list);

   TList * FilterMeasurements( TList * list, NucDBBinnedVariable * var );

   TMultiGraph * CreateMultiGraph(TList * list, const char * var);

   void FillLegend(TLegend * leg, TList * list, TMultiGraph * mg );

   /** From the list of measurement for the experiment name.
    *  Returns the first one found. Returns zero if not found.
    */
   NucDBMeasurement * GetExperiment(const char * exp_name, TList * meas_list);

   NucDBMeasurement * FindExperiment(const char * name, TList * meas_list);

   NucDBMeasurement * RemoveExperiment(const char * name, TList * meas_list);

   std::vector<NucDBMeasurement*> RemoveExperiment(const char * name, std::vector<NucDBMeasurement*> meas_list);

   NucDBMeasurement * Merge(TList * meas_list, const char * name = "" );

   /** Merge all the datapoints in the list into a single data point.
    */
   NucDBDataPoint *  MergeDataPoints(TList * points);

   /** Filter the list of datapoints with the bin.
    */
   TList * FilterDataPoints(TList * list, NucDBBinnedVariable * var);

   bool CompareDataPoint(const NucDBDataPoint *lhs, const NucDBDataPoint *rhs) ;
   TList * StableSort(const TList & list) ; 

   void Print(const std::vector<NucDBMeasurement*>& measurements);

   std::vector<NucDBMeasurement*> SelectType(NucDB::Type type, const std::vector<NucDBMeasurement*>& list );

}


#endif
