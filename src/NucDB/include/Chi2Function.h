#ifndef nucdb_Chi2Function_HH
#define nucdb_Chi2Function_HH

#include "Fit/Fitter.h"
#include "Math/IFunction.h"
#include "Math/FitMethodFunction.h"
#include "NucDBDataPoint.h"
#include <functional>

class FitResult;

/** FCN implementation for new ROOT::Fit library.
 *  Note the arguments are always the parameters since we must compute the the chi2.
 */
namespace NucDB {

  class Chi2Function : public ROOT::Math::FitMethodFunction {
    
    private:
      std::function<double(const double *, int)> fNucDBFCN;
      std::function<double(const double *, const double*)> fModelFunction;

    public:

      Chi2Function(int dim=2, int npoint = 10);
      Chi2Function(int dim  , int npoint     , std::function<double(const double *, int )> );
      virtual ~Chi2Function();

      virtual Chi2Function*  Clone() const;
      virtual double         DoEval(const double *p) const ;

      void    SetFunction(std::function<double(const double *, const double * )> f ){fModelFunction=f;}

      std::function<double(const double *, const double * )>  GetFunction(){return fModelFunction;}

      /**      Method returning the data i-th contribution to the fit objective function
       *       For example the residual for the least square functions or the pdf element for the
       *       likelihood functions.
       *       Estimating eventually also the gradient of the data element if the passed pointer 
       */
      virtual double DataElement(const double *x, unsigned int i, double *g) const;

      std::vector<double> GetConfidenceIntervals(const ROOT::Fit::FitResult& result, double cl,
          std::function<double(const double *, const double * )> f, std::vector<NucDBDataPoint*> data_points);
      
      std::vector<double> GetConfidenceIntervals(const ROOT::Fit::FitResult& result, double cl,
          std::function<double(const double *, const double * )> f, std::vector<double> xval, std::vector<double> x2val);
      

      ClassDef(Chi2Function,1)
  };

}

#endif
