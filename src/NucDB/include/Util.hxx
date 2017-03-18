// NucDBUtil hxx

//namespace NucDB {

  template<int Ndim>
    std::vector<double> GetConfidenceIntervals(const TMatrixDSym& cov, double Chi2, double Ndf, double cl, std::function<double(const double *, const double * )> f, const std::vector<double>& pars, const std::vector<NucDBDataPoint*>& data_points, const std::array<std::string,Ndim>& vars)
    {
      std::vector<std::array<double, Ndim>> coords;
      std::array<double, Ndim>              xpoint;
      int                                   npoints = data_points.size();//NPoints();
      // loop on the points
      for (unsigned int ipoint = 0; ipoint < npoints; ++ipoint) {
        for(int ivar = 0; ivar < Ndim; ivar++) {
          xpoint[ivar] = data_points.at(ipoint)->GetBinVariable(vars.at(ivar).c_str())->GetMean();
        }
        coords.push_back(xpoint);
      }
      return GetConfidenceIntervals(cov,Chi2,Ndf,cl,f,pars,coords);
    }
  //______________________________________________________________________________

  template<int Ndim>
  std::vector<double> GetConfidenceIntervals(const TMatrixDSym& cov, double Chi2, double Ndf, double cl,
      std::function<double(const double *, const double * )> f,
      const std::vector<double>& pars, const std::vector<std::array<double,Ndim>>& coords  )
    {
      int    npar       = pars.size();
      int    npoints    = coords.size();//NPoints();
      double corrFactor = 1.0;
      bool   norm       = true;
      if (Chi2 <= 0 || Ndf == 0) { norm = false;
}
      if (norm) {
        corrFactor = TMath::StudentQuantile(0.5 + cl/2, Ndf) * std::sqrt( Chi2/Ndf );
      } else {
        // value to go up in chi2 (1: 1 sigma error(CL=0.683) , 4: 2 sigma errors
        corrFactor = ROOT::Math::chisquared_quantile(cl, 1);
      }
      std::array<double, Ndim> xpoint;
      std::vector<double> grad(npar);
      std::vector<double> vsum(npar);
      std::vector<double> res;
      // loop on the points
      for (unsigned int ipoint = 0; ipoint < npoints; ++ipoint) {
        xpoint[0] = coords.at(ipoint).at(0);
        xpoint[1] = coords.at(ipoint).at(1);

        // calculate gradient of fitted function w.r.t the parameters
        // check first if fFitFunction provides parameter gradient or not
        // does not provide gradient
        // t.b.d : skip calculation for fixed parameters
        ROOT::Math::RichardsonDerivator d;
        for (unsigned int ipar = 0; ipar < npar; ++ipar) {
          ROOT::Math::OneDimParamFunctionAdapter<std::function<double(const double *, const double * )> > fadapter(
              f,&xpoint.front(), &pars.front(), ipar );
          d.SetFunction(fadapter);
          grad[ipar] = d(pars.at(ipar) ); // evaluate df/dp
        }

        // multiply covariance matrix with gradient
        vsum.assign(npar,0.0);
        for (unsigned int ipar = 0; ipar < npar; ++ipar) {
          for (unsigned int jpar = 0; jpar < npar; ++jpar) {
            vsum[ipar] += cov(ipar,jpar) * grad[jpar];
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

  template<int Ndim>
    TH1* GetConfidenceIntervals(const TMatrixDSym& cov, double Chi2, double Ndf, double cl, 
        std::function<double(const double *, const double * )> f, 
        const std::vector<double>& pars, const TH1 * h, unsigned int i_var, std::array<double,Ndim> fixed_coords)
    {
      Bool_t addStatus = TH1::AddDirectoryStatus();
      TH1::AddDirectory( kFALSE );
      auto * res = dynamic_cast<TH1*>(h->Clone("confidence_interval"));
      TH1::AddDirectory(addStatus);

      Int_t   xmax = h->GetNbinsX();
      Int_t   ymax = h->GetNbinsY();
      Int_t   zmax = h->GetNbinsZ();

      std::vector<std::array<double,Ndim>> args;
      std::vector<double> x_1;
      std::vector<double> x_2;

      for(Int_t i=1; i<= xmax; i++){
        for(Int_t j=1; j<= ymax; j++){
          for(Int_t k=1; k<= zmax; k++){
            Int_t bin = h->GetBin(i, j, k);
            double x_val = h->GetXaxis()->GetBinCenter(i);
            fixed_coords[i_var] = x_val;
            args.push_back(fixed_coords);
            res->SetBinContent( bin, f(&fixed_coords[0],&pars[0]) );
          }
        }
      }
      std::vector<double>  ci_res = GetConfidenceIntervals<Ndim>(cov, Chi2, Ndf, cl, f, pars, args);

      int ibin = 1;
      for(auto CI : ci_res) {
        res->SetBinError( ibin, CI );
        ibin++;
      }
      return res;
  }
  //______________________________________________________________________________
    




