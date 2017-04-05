#ifndef NucDBUtil_HH
#define NucDBUtil_HH 1

#include "TList.h"
#include "TMatrixD.h"
#include "TMatrixDSym.h"
#include "TH1.h"
#include "NucDBDataPoint.h"
#include "TMath.h"
#include "Math/IFunction.h"

#include "Math/IParamFunction.h"
#include "Math/OneDimFunctionAdapter.h"

#include "Math/ProbFuncMathCore.h"
#include "Math/QuantFuncMathCore.h"

#include "Math/RichardsonDerivator.h"
#include "Math/Error.h"

#include <vector>
#include <functional>
#include <iostream>
#include <string>

#include "TLegend.h"
#include "TMultiGraph.h"

#include "NucDBDataPoint.h"
#include "NucDBBinnedVariable.h"
class NucDBExperiment;
class NucDBMeasurement;


namespace NucDB {

  enum class Type {
    CrossSection, 
    CrossSectionDifference, 
    Asymmetry, 
    Ratio, 
    FormFactor,
    StructureFunction,
    PDF,
    TMD,
    GPD,
    ComptonFormFactor,
    MatrixElement,
    Amplitude,
    Other
  };
  std::ostream& operator<< (std::ostream & os, Type t);

  enum class Process {
    DIS, 
    Elastic, 
    DVCS, 
    DVMP, 
    SIDIS, 
    DrellYan,
    Inclusive,
    Exclusive,
    Electroproduction,
    Photoproduction,
    Other
  };
  std::ostream& operator<< (std::ostream & os, Process p);

  int  SetColors( std::vector<NucDBMeasurement*> /*meas*/, std::vector<int> /*colors*/);
  int  SetColors( std::vector<NucDBMeasurement*> /*meas*/, int /*color*/);
  int  SetLineColor(std::vector<NucDBMeasurement*> meas, int color) ;

  std::vector<int> GoodColors(int ncol=10,int offset=0);

  void ApplyFilterOnList(NucDBBinnedVariable * var, TList * list);

  TList *                        FilterMeasurements( TList * list, NucDBBinnedVariable * var );
  std::vector<NucDBMeasurement*> FilterMeasurements( std::vector<NucDBMeasurement*> /*list*/, NucDBBinnedVariable * var );

  std::vector<double> Values( std::vector<NucDBDataPoint*> points );
  std::vector<double> Errors( std::vector<NucDBDataPoint*> points );
  std::vector<double> BinValues(  std::vector<NucDBDataPoint*> points, const char * var = "x");
  std::vector<double> BinSizes(   std::vector<NucDBDataPoint*> points, const char * var = "x");

  TMultiGraph * CreateMultiGraph(TList * list, const char * var);
  TMultiGraph * CreateMultiGraph(std::vector<NucDBMeasurement*> /*vec*/, const char * var);
  TMultiGraph * CreateMultiKineGraph(std::vector<NucDBMeasurement*> /*vec*/, const char * var, const char * var2);

  void FillLegend(TLegend * leg, TList * list, TMultiGraph * mg );
  void FillLegend(TLegend * leg, std::vector<NucDBMeasurement*> /*vec*/, TMultiGraph * mg );

  /** From the list of measurement for the experiment name.
   *  Returns the first one found. Returns zero if not found.
   */
  NucDBMeasurement * GetExperiment(const char * exp_name, TList * meas_list);
  NucDBMeasurement * GetExperiment(const char * exp_name, std::vector<NucDBMeasurement*> meas_list);

  NucDBMeasurement * FindExperiment(const char * name, TList * meas_list);

  NucDBMeasurement * RemoveExperiment(const char * name, TList * meas_list);
  std::vector<NucDBMeasurement*> RemoveExperiment(const char * name, std::vector<NucDBMeasurement*> meas_list);

  NucDBMeasurement * Merge(TList * meas_list, const char * name = "" );
  NucDBMeasurement * Merge(const std::vector<NucDBMeasurement*>& /*list*/, const char * name = "" );

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

  template<class T>
    std::vector<T> ToVector(TList * l){
      std::vector<T> result;
      int n = l->GetEntries();
      l->SetOwner(false);
      for(int i=0; i<n; i++){
        auto v = (T)l->At(i);
        result.push_back(v);
      }
      return result;
    }

  template<class T>
    TList* ToList(const std::vector<T>& v )
    {
      auto * l = new TList();
      for(auto x: v){
        l->Add(x);
      }
      return l;
    }

  TH1* GetConfidenceIntervals(const TMatrixDSym& cov, double Chi2, double Ndf, double cl, 
      std::function<double(const double*, const double*)> f, const int Ndim,
      const std::vector<double>& pars, const TH1 * h, int i_var, std::vector<double> fixed_coords);

  std::vector<double> GetConfidenceIntervals(const TMatrixDSym& cov, double Chi2, double Ndf, double cl,
      std::function<double(const double *, const double * )> f, const int Ndim,
      const std::vector<double>& pars, const std::vector<std::vector<double>>& coords  );

  //______________________________________________________________________________

  //#include "Util.hxx"
}


#endif

