#include "NucDBUtil.h"

#include "TList.h"
#include "TLegend.h"
#include "TMultiGraph.h"
#include "NucDBMeasurement.h"
#include "NucDBDataPoint.h"
#include "NucDBBinnedVariable.h"
#include "NucDBManager.h"

namespace NucDB {

  std::ostream& operator<< (std::ostream & os, Type t)
  {
    switch (t)
    {
      case Type::CrossSection          : return os << "CrossSection";
      case Type::CrossSectionDifference: return os << "CrossSectionDifference";
      case Type::Asymmetry             : return os << "Asymmetry";
      case Type::Ratio                 : return os << "Ratio";
      case Type::FormFactor            : return os << "FormFactor";
      case Type::StructureFunction     : return os << "StructureFunction";
      case Type::PDF                   : return os << "PDF";
      case Type::TMD                   : return os << "TMD";
      case Type::GPD                   : return os << "GPD";
      case Type::ComptonFormFactor     : return os << "ComptonFormFactor";
      case Type::MatrixElement         : return os << "MatrixElement";
      case Type::Amplitude             : return os << "Amplitude";
      case Type::Other                 : return os << "Other";
    };
    return os << static_cast<std::uint16_t>(t);
  }

  std::ostream& operator<< (std::ostream & os, Process p)
  {
    switch(p)
    {
      case Process::DIS               : return os << "DIS";
      case Process::Elastic           : return os << "Elastic";
      case Process::DVCS              : return os << "DVCS";
      case Process::DVMP              : return os << "DVMP";
      case Process::SIDIS             : return os << "SIDIS";
      case Process::DrellYan          : return os << "DrellYan";
      case Process::Inclusive         : return os << "Inclusive";
      case Process::Exclusive         : return os << "Exclusive";
      case Process::Electroproduction : return os << "Electroproduction";
      case Process::Photoproduction   : return os << "Photoproduction";
      case Process::Other             : return os << "Other";
    };
    return os << static_cast<std::uint16_t>(p);
  }
  //______________________________________________________________________________

  int  SetColors(std::vector<NucDBMeasurement*> meas, std::vector<int> colors) 
  {
    NucDBManager * dbman = NucDBManager::GetManager();
    if( meas.size() > colors.size() ) {
      std::cout << "Warning NucDB::SetColors(meas,colors): colors less than number of measurements\n";
    }
    for(int i = 0; i< meas.size();i++) {
      int col = 0;
      if(i<colors.size()) {
        col = colors.at(i);
      } else {
        col = dbman->NextColor();
      }
      meas[i]->SetColor(col);
    }
    return meas.size();
  }
  //______________________________________________________________________________

  int  SetColors(std::vector<NucDBMeasurement*> meas, int color) 
  {
    for(auto m : meas ) {
      m->SetColor(color);
    }
    return meas.size();
  }
  //______________________________________________________________________________

  int  SetLineColor(std::vector<NucDBMeasurement*> meas, int color) 
  {
    for(auto m : meas ) {
      m->SetLineColor(color);
    }
    return meas.size();
  }
  //______________________________________________________________________________

   std::vector<int> GoodColors(int ncol, int offset)
   {
     std::vector<int> res = {1,2,4,6,8,9};
     int ncols = res.size();
     if(ncols>=ncol){
       res.resize(ncol);
       return res;
     }
     int counter = 0;
     int j = 0;
     int stride = 11;
     for(int i = ncols; i<ncol+offset; i++){

       int a_col = 20+stride*counter + j;

       if(a_col>100) { a_col=1;
}
       res.push_back(a_col);
       counter++;
       if(counter == stride) {
         counter=0;
         j++;
       }
     }
     res.erase(res.begin(),res.begin()+offset);
     return res;
   }


   void ApplyFilterOnList(NucDBBinnedVariable * var, TList * list) {
      if(!var) {
         std::cout << "Error null bin variable" << std::endl;
         return;
      }
      if(!list) {
         std::cout << "Error null list" << std::endl;
         return;
      }
      for(unsigned int i = 0; i<list->GetEntries();i++) {
         auto * mes = dynamic_cast<NucDBMeasurement*>(list->At(i));
         mes->ApplyFilterWithBin(var);
      }
   }

   TList * FilterMeasurements(TList * list, NucDBBinnedVariable * var) {
      if(!var) {
         std::cout << "Error null bin variable" << std::endl;
         return nullptr;
      }
      if(!list) {
         std::cout << "Error null list" << std::endl;
         return nullptr;
      }
      auto * filtered_list = new TList();
      for(unsigned int i = 0; i<list->GetEntries();i++) {
         auto * mes    = dynamic_cast<NucDBMeasurement*>(list->At(i));
         NucDBMeasurement * meas2  = mes->NewMeasurementWithFilter(var);
         if( meas2->GetNDataPoints() > 0 ) {
            filtered_list->Add(meas2);
         }
      }
      return filtered_list;
   }
   //______________________________________________________________________________

   std::vector<NucDBMeasurement*> FilterMeasurements( std::vector<NucDBMeasurement*> list, NucDBBinnedVariable * var )
   {
      std::vector<NucDBMeasurement*> filtered_list;
      if(!var) {
         std::cout << "Error null bin variable" << std::endl;
         return filtered_list;
      }
      for(auto mes  : list) {
         NucDBMeasurement * meas2  = mes->NewMeasurementWithFilter(var);
         if( meas2->GetNDataPoints() > 0 ) {
            filtered_list.push_back(meas2);
         }
      }
      return filtered_list;
   }
//______________________________________________________________________________

   std::vector<double> Values(std::vector<NucDBDataPoint*> points )
   {
     std::vector<double> res;
     for(auto p  : points) {
       res.push_back(p->GetValue());
     }
     return res;
   }
   //______________________________________________________________________________
   
   std::vector<double> Errors(std::vector<NucDBDataPoint*> points )
   {
     std::vector<double> res;
     for(auto p  : points) {
       res.push_back(p->GetError());
     }
     return res;
   }
   //______________________________________________________________________________


   std::vector<double> BinValues(std::vector<NucDBDataPoint*> points , const char * var)
   {
     std::vector<double> res;
     for(auto p  : points) {
       res.push_back(p->GetBinVariable(var)->GetMean());
     }
     return res;
   }
   //______________________________________________________________________________
   
   std::vector<double> BinSizes(std::vector<NucDBDataPoint*> points , const char * var)
   {
     std::vector<double> res;
     for(auto p  : points) {
       res.push_back(p->GetBinVariable(var)->GetBinWidth());
     }
     return res;
   }
   //______________________________________________________________________________

   TMultiGraph * CreateMultiGraph(TList * list, const char * var) {
      // creates a multi graph from a list of measurments
      if(!list) {
         std::cout << "Error null list" << std::endl;
         return nullptr;
      }
      NucDBManager * dbman = NucDBManager::GetManager();
      auto * mg = new TMultiGraph();
      for(unsigned int i = 0; i<list->GetEntries();i++) {
         auto * mes = dynamic_cast<NucDBMeasurement*>(list->At(i));
         TGraph * gr = mes->BuildGraph(var);
         if(gr) {
            Int_t color = mes->GetColor();//dbman->NextColor();
            Int_t mark  = mes->GetMarkerStyle();//dbman->NextMarker();
            gr->SetMarkerColor(color);
            gr->SetLineColor(  mes->GetLineColor());
            gr->SetMarkerStyle(mark);
            mg->Add(gr,"p");
         }
      }
      return mg;
   }
//______________________________________________________________________________

   TMultiGraph * CreateMultiGraph(std::vector<NucDBMeasurement*> vec, const char * var){
      // creates a multi graph from a list of measurments
      NucDBManager * dbman = NucDBManager::GetManager();
      auto  * mg = new TMultiGraph();
      for(auto mes : vec) {
         TGraphErrors * gr = mes->BuildGraph(var);
         if(gr) {
            Int_t color = mes->GetColor();//dbman->NextColor();
            Int_t mark  = mes->GetMarkerStyle();//dbman->NextMarker();
            gr->SetMarkerColor(color);
            gr->SetLineColor(  mes->GetLineColor());
            gr->SetMarkerStyle(mark);
            mg->Add(gr,"p");
         }
      }
      return mg;
   }
//______________________________________________________________________________
   
   TMultiGraph * CreateMultiKineGraph(std::vector<NucDBMeasurement*> vec, const char * var, const char * var2)
   {
      // creates a multi graph from a list of measurments
      NucDBManager * dbman = NucDBManager::GetManager();
      auto  * mg = new TMultiGraph();
      for(auto mes : vec) {
         TGraph * gr = mes->BuildKinematicGraph(var,var2);
         if(gr) {
            Int_t color = mes->GetColor();//dbman->NextColor();
            Int_t mark  = mes->GetMarkerStyle();//dbman->NextMarker();
            gr->SetMarkerColor(color);
            gr->SetLineColor(  mes->GetLineColor());
            gr->SetMarkerStyle(mark);
            mg->Add(gr,"p");
         }
      }
      return mg;
   }
//______________________________________________________________________________

   void FillLegend(TLegend * leg, TList * list, TMultiGraph * mg ) {
      // Fills the legend with  the list of measurements associated with the multigraph
      if(!list) {
         std::cout << "Error null list" << std::endl;
         return ;
      }
      if(!leg) {
         std::cout << "Error null leg" << std::endl;
         return ;
      }

      for(unsigned int i = 0; i<list->GetEntries();i++) {
         auto * mes = dynamic_cast<NucDBMeasurement*>(list->At(i));
         TGraph * gr = nullptr;
         if(mg) {
            gr = dynamic_cast<TGraph*>(mg->GetListOfGraphs()->At(i));
         }
         if(!gr) {
            // just build the default graph for the style TODO: fixe this
            gr = mes->BuildGraph();
         }
         leg->AddEntry(gr,mes->GetExperimentName(),"lp");
      }
   }
//______________________________________________________________________________

   void FillLegend(TLegend * leg, std::vector<NucDBMeasurement*> vec, TMultiGraph * mg ) {
      // Fills the legend with  the list of measurements associated with the multigraph
      if(!leg) {
         std::cout << "Error null leg" << std::endl;
         return ;
      }
      if(!mg) {
        std::cout << "Error null multigraph" << std::endl;
        return ;
      }

      int i = 0;
      for(auto mes : vec) {
         auto * gr = dynamic_cast<TGraph*>(mg->GetListOfGraphs()->At(i));
         if(!gr) {
            // just build the default graph for the style TODO: fixe this
           std::cout << " warning shouldn't be here in NucDB::FillLegend" <<std::endl;
            gr = mes->BuildGraph();
         }
         leg->AddEntry(gr,mes->GetExperimentName(),"lp");
         i++;
      }
   }
   //______________________________________________________________________________

   /** From the list of measurement for the experiment name.
    *  Returns the first one found.
    */
   NucDBMeasurement * GetExperiment(const char * exp_name, TList * meas_list){
      NucDBMeasurement * aMeas = nullptr;
      if(!meas_list) {
         std::cout << "Measurement list is Null" << std::endl;
         return nullptr;
      }
      for(int i = 0; i<meas_list->GetEntries();i++) {
         aMeas = dynamic_cast<NucDBMeasurement*>(meas_list->At(i));
         if( !strcmp(exp_name,aMeas->GetExperimentName()) ) {
            return aMeas;
         }
      }
      return nullptr;
   }
//______________________________________________________________________________

   /** From the list of measurement for the experiment name.
    *  Returns the first one found.
    */
   NucDBMeasurement * GetExperiment(const char * exp_name, std::vector<NucDBMeasurement*> meas_list){
      NucDBMeasurement * aMeas = nullptr;
      for(auto m: meas_list) {
         if( !strcmp(exp_name,m->GetExperimentName()) ) {
            return m;
         }
      }
      return nullptr;
   }
//______________________________________________________________________________

   std::vector<NucDBMeasurement*> RemoveExperiment(const char * name, std::vector<NucDBMeasurement*> meas_list)
   {
      std::vector<NucDBMeasurement*> ret_list;
      std::string n1 = name;
      //NucDBMeasurement * ret_meas = nullptr;
      
      int i = 0;
      for(auto m : meas_list ) {

         std::string n2 = m->GetExperimentName();
         //std::size_t found = n2.find(name);
         if( n1 == n2 ) {
            //std::cout << " found " << found << std::endl;
            //ret_meas = m;
            //meas_list.erase(meas_list.begin() + i);
         } else {
            ret_list.push_back(m);
         }
         i++;
      }
      return ret_list;
   }
   //______________________________________________________________________________
   
   NucDBMeasurement * RemoveExperiment(const char * name, TList * meas_list)
   {
      NucDBMeasurement * aMeas = nullptr;
      if(!meas_list) {
         std::cout << "Measurement list is Null" << std::endl;
         return nullptr;
      }
      for(int i = 0; i<meas_list->GetEntries();i++) {
         aMeas = dynamic_cast<NucDBMeasurement*>(meas_list->At(i));
         std::string n2 = aMeas->GetExperimentName();
         std::size_t found = n2.find(name);
         if (found!=std::string::npos) {
            meas_list->Remove(aMeas);
            return aMeas;
         }
      }
      return nullptr;
   }
   //______________________________________________________________________________

   NucDBMeasurement * FindExperiment(const char * name, TList * meas_list){
      // searches for name in the experinment name measurement
      // returns the first match
      NucDBMeasurement * aMeas = nullptr;
      if(!meas_list) {
         std::cout << "Measurement list is Null" << std::endl;
         return nullptr;
      }
      for(int i = 0; i<meas_list->GetEntries();i++) {
         aMeas = dynamic_cast<NucDBMeasurement*>(meas_list->At(i));
         std::string n2 = aMeas->GetExperimentName();
         std::size_t found = n2.find(name);
         if (found!=std::string::npos) {
            return aMeas;
         }
      }
      return nullptr;
   }
//______________________________________________________________________________

   NucDBMeasurement * Merge(TList * meas_list, const char * name )
   {
      if(!meas_list) {
         std::cout << "cannot merge: measurement list is Null" << std::endl;
         return nullptr;
      }
      if(meas_list->GetEntries() <= 0) {
         std::cout << "cannot merge: measurement list is empty" << std::endl;
         return nullptr;
      }

      auto * merged_meas = new NucDBMeasurement(name,name);
      for(int i = 0; i<meas_list->GetEntries();i++) {
         auto * aMeas = dynamic_cast<NucDBMeasurement*>(meas_list->At(i));
         merged_meas->AddDataPoints(dynamic_cast<TList*>(aMeas->GetDataPoints()->Clone()));
      }
      return merged_meas;
   }
//______________________________________________________________________________

   NucDBMeasurement * Merge(const std::vector<NucDBMeasurement*>& list , const char * name )
   {
      auto * merged_meas = new NucDBMeasurement(name,name);
      for(auto m : list) {
         merged_meas->AddDataPoints(dynamic_cast<TList*>(m->GetDataPoints()->Clone()));
      }
      return merged_meas;
   }
//______________________________________________________________________________

   NucDBDataPoint *  MergeDataPoints(TList * points){
      if(!points) {
         std::cout << "Null points list" << std::endl;
         return nullptr;
      }
      if(points->GetEntries() <= 0 ) {
         std::cout << "Empty points list" << std::endl;
         return nullptr;
      }

      NucDBDataPoint * mergedPoint = nullptr;
      for(unsigned int i = 0; i< points->GetEntries(); i++) {
         auto * point = dynamic_cast<NucDBDataPoint*>(points->At(i));
         if(i == 0){
            mergedPoint = new NucDBDataPoint(*point);
         } else {
            (*mergedPoint) += (*point);
         }
      }
      return mergedPoint;
   }

//_____________________________________________________________________________
   TList * FilterDataPoints(TList * list, NucDBBinnedVariable * var){
      auto * plist = new TList();
      if(!var) {
         std::cout << "NULL NucDBVariable pointer" << std::endl;
         return list;
      }
      for(int i = 0; i < list->GetEntries();i++) {
         auto * point = dynamic_cast<NucDBDataPoint*>(list->At(i));
         if(!point) {
            std::cout << "list does not contain data points." <<std::endl;
            continue;
         }
         NucDBBinnedVariable * var2    = point->GetBinVariable(var->GetName());
         if(var2){
            if ( (*var) == (*var2) ) { plist->Add(point);
}
         }
      }
      return plist;
   }
   //_____________________________________________________________________________
   
   bool CompareDataPoint(const NucDBDataPoint *lhs, const NucDBDataPoint *rhs)
   {
      int n = lhs->GetNSortVariables();
      for(int i = n-1; i>=0; i--) {
         NucDBBinnedVariable * var3  = lhs->GetSortPriority(i);
         NucDBBinnedVariable * var4  = rhs->GetSortPriority(i);
         if(!var3 || !var4) {
            Error("Compare","Could not find both bin variables. Check sorting variable name.");
            return false;
         }
         double b0 = var3->GetMean();
         double b1 = var4->GetMean();
         if(b0!=b1) {
            //std::cout << var3->GetName() << b0 << " < " << b1 << std::endl;
            return( b0<b1 );
         }

         //if( var3->Contains(b1) ) {
         //   //std::cout << var3->GetName() << " contains " << var4->GetName() << std::endl;
         //}
         //if( !(var3->Contains(b1)) && (var3->IsBelow(b1))  )  true;
         //std::cout << " not true"; 
         //if( !(var4->Contains(b0)) && (var4->IsBelow(b0))  )  false;
         //std::cout << " not false"; 
      }
      //lhs->Print();
      //if( (*lhs) < (*rhs) ) {
      //   std::cout << " LHS is < " << std::endl;
      //} else {
      //   std::cout << " LHS is > " << std::endl;
      //}
      //rhs->Print();

      return( (*lhs) < (*rhs) );
   }
//_____________________________________________________________________________
   TList *  StableSort( const TList & list ) { 
      std::vector<NucDBDataPoint*> templist;
      Int_t n1 = list.GetEntries();
      for(int i = 0; i < list.GetEntries();i++) {
         auto * point = dynamic_cast<NucDBDataPoint*>(list.At(i));
         templist.push_back(point);
      }
      //std::sort (templist.begin(), templist.end(), CompareDataPoint );
      std::stable_sort (templist.begin(), templist.end(), CompareDataPoint );
      Int_t n2 =  templist.size();
      auto * newlist  = new TList();
      for(int i = 0; i< templist.size() ; i++ )  { 
         //templist[i]->Print();
         newlist->Add( new NucDBDataPoint( *(templist[i]))  );
      }
      if( n1 != n2 ) {
         std::cout << " ERROR sorting counts not equal! " << std::endl;
      }
      return newlist;
   }
//______________________________________________________________________________


   void Print(const std::vector<NucDBMeasurement*>& measurements)
   {
      for(auto m : measurements) {
         m->Print();
      }
   }
//______________________________________________________________________________


   std::vector<NucDBMeasurement*> SelectType(NucDB::Type type,const std::vector<NucDBMeasurement*>& list)
   {
      std::vector<NucDBMeasurement*> results;
      for(auto m : list){
         if( m->IsType(type) ) {
            results.push_back(m);
         }
      }
      return results;
   }
   //______________________________________________________________________________

   TH1* GetConfidenceIntervals(const TMatrixDSym& cov, double Chi2, double Ndf, double cl, 
       std::function<double(const double *, const double * )> f, const int Ndim,
       const std::vector<double>& pars, const TH1 * h, int i_var, std::vector<double> fixed_coords)
   {
     Bool_t addStatus = TH1::AddDirectoryStatus();
     TH1::AddDirectory( kFALSE );
     auto * res = dynamic_cast<TH1*>(h->Clone("confidence_interval"));
     TH1::AddDirectory(addStatus);

     Int_t   xmax = h->GetNbinsX();
     Int_t   ymax = h->GetNbinsY();
     Int_t   zmax = h->GetNbinsZ();

     int ndim = fixed_coords.size();
     std::vector<std::vector<double>> args;
     std::vector<double> x_1;
     std::vector<double> x_2;

     for(Int_t i=1; i<= xmax; i++){
       for(Int_t j=1; j<= ymax; j++){
          for(Int_t k=1; k<= zmax; k++){
            Int_t bin = h->GetBin(i, j, k);
            double x = h->GetXaxis()->GetBinCenter(i);
            fixed_coords[i_var] = x;
            args.push_back(fixed_coords);
            res->SetBinContent( bin, f(fixed_coords.data(),&pars[0]) );
          }
        }
      }
      std::vector<double>  ci_res = GetConfidenceIntervals(cov, Chi2, Ndf, cl, f, Ndim, pars, args);

      int ibin = 1;
      for(auto CI : ci_res) {
        res->SetBinError( ibin, CI );
        ibin++;
      }
      return res;
    }
   //______________________________________________________________________________

   std::vector<double> GetConfidenceIntervals(const TMatrixDSym& cov, double Chi2, double Ndf, double cl,
      std::function<double(const double *, const double * )> f, const int Ndim,
      const std::vector<double>& pars, const std::vector<std::vector<double>>& coords  )
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
      std::vector<double> xpoint(Ndim);
      std::vector<double> grad(npar);
      std::vector<double> vsum(npar);
      std::vector<double> res;
      // loop on the points
      for (unsigned int ipoint = 0; ipoint < npoints; ++ipoint) {
        //xpoint[0] = coords.at(ipoint).at(0);
        //xpoint[1] = coords.at(ipoint).at(1);
        for(int ivar = 0; ivar < Ndim; ivar++) {
          xpoint[ivar] = coords.at(ipoint).at(ivar);
        }

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

}

