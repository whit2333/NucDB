#include "Chi2Function.h"
#include "Fit/FitResult.h"
#include "TMath.h"
#include "Math/QuantFuncMathCore.h"

#include "Math/IParamFunction.h"
#include "Math/OneDimFunctionAdapter.h"

#include "Math/ProbFuncMathCore.h"
#include "Math/QuantFuncMathCore.h"

#include "TMath.h"
#include "Math/RichardsonDerivator.h"
#include "Math/Error.h"

namespace NucDB {

  Chi2Function::Chi2Function(int dim, int npoint) : ROOT::Math::FitMethodFunction(dim, npoint) 
  { }
//______________________________________________________________________________

  Chi2Function::Chi2Function(int dim, int npoint, std::function<double(const double *,int)> f)
    : ROOT::Math::FitMethodFunction(dim, npoint), fNucDBFCN(f)
  {  }
//______________________________________________________________________________

  Chi2Function::~Chi2Function()
  { }
//______________________________________________________________________________

  Chi2Function*  Chi2Function::Clone() const
  { 
    Chi2Function * copy = new Chi2Function(NDim(), NPoints(), fNucDBFCN );
    //(*copy) = (*this);
    return copy;
  }
  //______________________________________________________________________________

  double Chi2Function::DoEval(const double *p) const 
  {
    double chi2 = 0.0;
    for(int ip = 0; ip< NPoints();ip++){
      chi2 += fNucDBFCN(p,ip); 
    }
    return chi2;
  }
//______________________________________________________________________________


  /**      Method returning the data i-th contribution to the fit objective function
   *       For example the residual for the least square functions or the pdf element for the
   *       likelihood functions.
   *       Estimating eventually also the gradient of the data element if the passed pointer 
   */
  double Chi2Function::DataElement(const double *x, unsigned int i, double *g) const
  {
      return fNucDBFCN(x,i); 
  }
  //______________________________________________________________________________
  //______________________________________________________________________________


  std::vector<double> Chi2Function::GetConfidenceIntervals(const ROOT::Fit::FitResult& result, double cl,
     std::function<double(const double *, const double * )> f, std::vector<NucDBDataPoint*> data_points)
  {
    SetFunction(f);
    // use student quantile in case of normalized errors
    double Chi2 = result.Chi2();
    int    Ndf  = result.Ndf();
    double corrFactor = 1.0;
    bool norm = true;
    if (Chi2 <= 0 || Ndf == 0) norm = false;
    if (norm)
      corrFactor = TMath::StudentQuantile(0.5 + cl/2, Ndf) * std::sqrt( Chi2/Ndf );
    else
      // value to go up in chi2 (1: 1 sigma error(CL=0.683) , 4: 2 sigma errors
      corrFactor = ROOT::Math::chisquared_quantile(cl, 1);

    unsigned int ndim = 2;//NDim();
    unsigned int npar = result.NPar();

    std::vector<double> xpoint(ndim);
    std::vector<double> grad(npar);
    std::vector<double> vsum(npar);

    std::vector<double> res;

    auto params = result.Parameters();
    int n = data_points.size();//NPoints();
    // loop on the points
    std::cout << n    << " points \n";
    std::cout << npar << " npar \n";
    std::cout << ndim << " ndim \n";
    for (unsigned int ipoint = 0; ipoint < n; ++ipoint) {

      //for (unsigned int kdim = 0; kdim < ndim; ++kdim) {
      //  unsigned int i = ipoint * stride1 + kdim * stride2;
      //  assert(i < ndim*n);
      data_points[ipoint]->Print();
      xpoint[0] = data_points[ipoint]->GetBinVariable("W")->GetMean();
      xpoint[1] = data_points[ipoint]->GetBinVariable("Qsquared")->GetMean();
      //}

      // calculate gradient of fitted function w.r.t the parameters
      // check first if fFitFunction provides parameter gradient or not
      // does not provide gradient
      // t.b.d : skip calculation for fixed parameters
      ROOT::Math::RichardsonDerivator d;
      for (unsigned int ipar = 0; ipar < npar; ++ipar) {
        ROOT::Math::OneDimParamFunctionAdapter<std::function<double(const double *, const double * )> > fadapter(
            fModelFunction,&xpoint.front(), &params.front(), ipar );
        d.SetFunction(fadapter);
        grad[ipar] = d(params[ipar] ); // evaluate df/dp
      }

      // multiply covariance matrix with gradient
      vsum.assign(npar,0.0);
      for (unsigned int ipar = 0; ipar < npar; ++ipar) {
        for (unsigned int jpar = 0; jpar < npar; ++jpar) {
          vsum[ipar] += result.CovMatrix(ipar,jpar) * grad[jpar];
        }
      }
      // multiply gradient by vsum
      double r2 = 0;
      for (unsigned int ipar = 0; ipar < npar; ++ipar) {
        r2 += grad[ipar] * vsum[ipar];
      }
      double r = std::sqrt(r2);
      //ci[ipoint] = r * corrFactor;
      res.push_back(r * corrFactor);
    }
    return res;
  }


//______________________________________________________________________________

  std::vector<double> Chi2Function::GetConfidenceIntervals(const ROOT::Fit::FitResult& result, double cl,
      std::function<double(const double *, const double * )> f, std::vector<double> x1val, std::vector<double> x2val)
  {
    SetFunction(f);
    // use student quantile in case of normalized errors
    double Chi2 = result.Chi2();
    int    Ndf  = result.Ndf();
    double corrFactor = 1.0;
    bool norm = true;
    if (Chi2 <= 0 || Ndf == 0) norm = false;
    if (norm)
      corrFactor = TMath::StudentQuantile(0.5 + cl/2, Ndf) * std::sqrt( Chi2/Ndf );
    else
      // value to go up in chi2 (1: 1 sigma error(CL=0.683) , 4: 2 sigma errors
      corrFactor = ROOT::Math::chisquared_quantile(cl, 1);

    unsigned int ndim = 2;//NDim();
    unsigned int npar = result.NPar();

    std::vector<double> xpoint(ndim);
    std::vector<double> grad(npar);
    std::vector<double> vsum(npar);

    std::vector<double> res;

    auto params = result.Parameters();
    int n = x1val.size();//NPoints();
    // loop on the points
    std::cout << n    << " points \n";
    std::cout << npar << " npar \n";
    std::cout << ndim << " ndim \n";
    for (unsigned int ipoint = 0; ipoint < n; ++ipoint) {

      //for (unsigned int kdim = 0; kdim < ndim; ++kdim) {
      //  unsigned int i = ipoint * stride1 + kdim * stride2;
      //  assert(i < ndim*n);
      //data_points[ipoint]->Print();
      xpoint[0] = x1val[ipoint];//data_points[ipoint]->GetBinVariable("W")->GetMean();
      xpoint[1] = x2val[ipoint];//data_points[ipoint]->GetBinVariable("Qsquared")->GetMean();
      //}

      // calculate gradient of fitted function w.r.t the parameters
      // check first if fFitFunction provides parameter gradient or not
      // does not provide gradient
      // t.b.d : skip calculation for fixed parameters
      ROOT::Math::RichardsonDerivator d;
      for (unsigned int ipar = 0; ipar < npar; ++ipar) {
        ROOT::Math::OneDimParamFunctionAdapter<std::function<double(const double *, const double * )> > fadapter(
            fModelFunction,&xpoint.front(), &params.front(), ipar );
        d.SetFunction(fadapter);
        grad[ipar] = d(params[ipar] ); // evaluate df/dp
      }

      // multiply covariance matrix with gradient
      vsum.assign(npar,0.0);
      for (unsigned int ipar = 0; ipar < npar; ++ipar) {
        for (unsigned int jpar = 0; jpar < npar; ++jpar) {
          vsum[ipar] += result.CovMatrix(ipar,jpar) * grad[jpar];
        }
      }
      // multiply gradient by vsum
      double r2 = 0;
      for (unsigned int ipar = 0; ipar < npar; ++ipar) {
        r2 += grad[ipar] * vsum[ipar];
      }
      double r = std::sqrt(r2);
      //ci[ipoint] = r * corrFactor;
      res.push_back(r * corrFactor);
    }
    return res;
  }
  //______________________________________________________________________________

}

