#include "NucDBMeasurement.h"
#include <assert.h>
#include <iostream>
#include <iomanip>
#include <map>

ClassImp(NucDBMeasurement)
//_____________________________________________________________________________

NucDBMeasurement::NucDBMeasurement(const char * name,const char * title)
     :TNamed(name,title) {
      fNumberOfDataPoints=0;
      fDataPoints.Clear();
      fGraph=0;
      fColor=1;
      fExperimentName="";
   }
//_____________________________________________________________________________

NucDBMeasurement::~NucDBMeasurement(){ 
}
//_____________________________________________________________________________
NucDBMeasurement::NucDBMeasurement(const NucDBMeasurement& v){
   SetNameTitle(v.GetName(),v.GetTitle());
   fColor = v.GetColor();
   SetExperimentName(v.GetExperimentName());
   fNumberOfDataPoints = v.fNumberOfDataPoints;

   TList * datapoints = (TList *)(v.fDataPoints.Clone());
   fDataPoints.AddAll( datapoints );
   fBinnedVariables.AddAll(v.GetBinnedVariables());
   fDependentVariables.AddAll(v.GetDependentVariables());
   fGraphs.AddAll(v.GetGraphs());
}
//_____________________________________________________________________________
void  NucDBMeasurement::Browse(TBrowser* b) {
   b->Add(&fDataPoints     , "Data Points");
   b->Add(&fGraphs         , "Graphs");
   b->Add(&fReferences     , "Refs");
   if(fGraph)b->Add(fGraph , "vs x");
}
//______________________________________________________________________________
void NucDBMeasurement::ClearDataPoints(){
   fDataPoints.Clear();
   fNumberOfDataPoints=0;
}
//_____________________________________________________________________________
void NucDBMeasurement::AddDataPoint(NucDBDataPoint *point) {
   if(point) {
      point->SetNameTitle(Form("p%d",fNumberOfDataPoints),Form("p%d",fNumberOfDataPoints));
      fDataPoints.Add(point);
      fNumberOfDataPoints++;
   } else {
      printf(" NULL NucDBDataPoint pointer \n");
   } 
}
//_____________________________________________________________________________
void NucDBMeasurement::AddDataPoints(TList * listOfPoints, bool clear) {
   if(listOfPoints) {
      if(clear) ClearDataPoints();
      for(int i = 0; i < listOfPoints->GetEntries();i++) 
         AddDataPoint((NucDBDataPoint*)listOfPoints->At(i));
   } else {
      printf(" NULL TList pointer \n");
   } 
}
//_____________________________________________________________________________
TList *  NucDBMeasurement::MergeDataPoints(unsigned int n, const char * var, bool modify){
   SortBy(var);
   TList * list = new TList();
   list->Clear();
   if(n<2) {
      Error("MergeDataPoints","Wrong number of bins to merge. ");
      return list;
   }

   NucDBDataPoint * mergedPoint = 0;
   unsigned int i = 0;
   while(i < fDataPoints.GetEntries()) {
      NucDBDataPoint * point = (NucDBDataPoint*)fDataPoints.At(i);
      if(i%n == 0){
         mergedPoint = new NucDBDataPoint(*point);
         list->Add(mergedPoint);
      } else {
         (*mergedPoint) += (*point);
      }
      i++;
   }
   if(modify) {
      AddDataPoints(list,true);
   }
   return list;
}
//_____________________________________________________________________________
void NucDBMeasurement::SortBy(const char * n){
   // Sort data according the value of the binned variable
   for(int i = 0; i < fDataPoints.GetEntries();i++) {
      NucDBDataPoint * point = (NucDBDataPoint*)fDataPoints.At(i);
      point->SetSortingVariable(n);
   }
   fDataPoints.Sort();
}
//_____________________________________________________________________________
void  NucDBMeasurement::Multiply(const char * v){
   // Multiplies the datapoint  by the value of the supplied variable
   for(int i = 0; i < fDataPoints.GetEntries();i++) {
      NucDBDataPoint * point = (NucDBDataPoint*)fDataPoints.At(i);
      Double_t value = point->GetValue();
      NucDBBinnedVariable * var    = point->GetBinVariable(v);
      if(!var) {
         Warning("Multiply","Could not find variable, %s, for point %d.",v,i);
         continue; 
      }
      if(var){
         point->Multiply(value);
      }
   }
}
//_____________________________________________________________________________
NucDBMeasurement * NucDBMeasurement::CreateMeasurementFilteredWithBin(NucDBBinnedVariable const * bin) {
   NucDBMeasurement * m = new NucDBMeasurement(Form("%s_%s",this->GetName(),bin->GetName()),
         Form("%s %s",this->GetTitle(),bin->GetTitle()) );
   m->AddDataPoints(this->FilterWithBin(bin));
   return m;
}  
//______________________________________________________________________________
NucDBMeasurement * NucDBMeasurement::NewMeasurementWithFilter(NucDBBinnedVariable const * bin) {
   // same as CreateMeasurementFilteredWithBin above
   return( CreateMeasurementFilteredWithBin(bin) ); 
}
//_____________________________________________________________________________
TList *  NucDBMeasurement::FilterWith(NucDBVariable const *v) {
   TList * list = new TList();
   list->Clear();
   if(!v) {
      printf(" NULL NucDBVariable pointer. Returning list with no entries. \n");
      return list;
   }
   for(int i = 0; i < fDataPoints.GetEntries();i++) {
      NucDBDataPoint * point = (NucDBDataPoint*)fDataPoints.At(i);
      NucDBVariable * var    = point->GetVariable(v->GetName());
      if(var){
         if ( (*v) == (*var) ) list->Add(point);
      }
   }
   return list;
}
//_____________________________________________________________________________
TList *  NucDBMeasurement::FilterWith(NucDBDiscreteVariable const *v) {
   TList * list = new TList();
   list->Clear();
   if(!v) {
      printf(" NULL NucDBDiscreteVariable pointer. Returning list with no entries. \n");
      return list;
   }
   for(int i = 0; i < fDataPoints.GetEntries();i++) {
      NucDBDataPoint * point = (NucDBDataPoint*)fDataPoints.At(i);
      NucDBDiscreteVariable * var = point->GetDiscreteVariable(v->GetName());
      if(var){
         if ( (*v) == (*var) ) list->Add(point);
      }
   }
   return list;
}
//_____________________________________________________________________________
TList *  NucDBMeasurement::FilterWithBin(NucDBBinnedVariable const *bin) {
   TList * list = new TList();
   list->Clear();
   if(!bin) {
      printf(" NULL NucDBBinnedVariable pointer. Returning list with no entries. \n");
      return list;
   }
   for(int i = 0; i < fDataPoints.GetEntries();i++) {
      NucDBDataPoint * point = (NucDBDataPoint*)fDataPoints.At(i);
      NucDBBinnedVariable * var = point->GetBinVariable(bin->GetName());
      if(var){
         if ( (*var) == (*bin) ) list->Add(point);
         continue;
      }
      NucDBDependentVariable * dvar = point->GetDependentVariable(bin->GetName());
      if(dvar){
         if ( (*dvar) == (*bin) ) list->Add(point);
      }
   }
   return list;
}
//_____________________________________________________________________________
TList *  NucDBMeasurement::ApplyFilterWithBin(NucDBBinnedVariable const *bin) {
   TList * list = FilterWithBin(bin); 
   AddDataPoints(list,true);
   return list;
}
//_____________________________________________________________________________
TList *  NucDBMeasurement::ApplyFilterWith(NucDBDiscreteVariable const *v) {
   TList * list = FilterWith(v); 
   AddDataPoints(list,true);
   return list;
}
//_____________________________________________________________________________
TList *  NucDBMeasurement::ApplyFilterWith(NucDBVariable const *v) {
   TList * list = FilterWith(v); 
   AddDataPoints(list,true);
   return list;
}
//______________________________________________________________________________
void NucDBMeasurement::Print(Option_t * opt ) const {
   TString opt1         = opt; opt1.ToLower();
   Bool_t printData     = opt1.Contains("data") ? kTRUE  : kFALSE;
   Bool_t printComments = opt1.Contains("comm") ? kTRUE  : kFALSE;
   Bool_t printRefs     = opt1.Contains("ref")  ? kTRUE  : kFALSE;
   Bool_t printvars     = opt1.Contains("v")  ? kTRUE  : kFALSE;
   std::cout << " --------------------------\n";
   std::cout << " " << GetName() << "\n";
   std::cout << "     title       : " << GetTitle() << "\n";
   std::cout << "     Experiment  : " << GetExperimentName() << std::endl;
   std::cout << "     NDataPoints : " << fNumberOfDataPoints <<  "\n";
   if(printvars && fNumberOfDataPoints>0){
      const NucDBDataPoint * p0 = (const NucDBDataPoint*)fDataPoints.At(0);
      const TList& vars = p0->GetBinnedVariablesRef(); 
      NucDBBinnedVariable * var = 0;
      for(int j=0;j<vars.GetEntries();j++) {
         var = (NucDBBinnedVariable*)vars.At(j);
         std::vector<double> vals;
         GetUniqueBinnedVariableValues(var->GetName(),vals);
         std::cout << var->GetName() << " : " ;
         for(int k=0;k<vals.size();k++){
            if(k!=0) std::cout << ", ";
            std::cout << vals[k]; 
         }
         std::cout << std::endl;
      }

   }
   if(printRefs) fReferences.Print(opt);
   if(printComments) PrintComments();
   if(printData) fDataPoints.Print(opt);
}
//_____________________________________________________________________________

void NucDBMeasurement::PrintData(Option_t * opt) const {
   Print();
   for(int i=0; i<fDataPoints.GetEntries(); i++){
      NucDBDataPoint * aPoint = (NucDBDataPoint*)fDataPoints.At(i);
      std::cout << "[" << i << "] " << GetName();
      aPoint->Print();
   }
}
//_____________________________________________________________________________

void  NucDBMeasurement::ListVariables(){
   if(fDataPoints.GetEntries()>0) {
      NucDBDataPoint * aPoint = (NucDBDataPoint*)fDataPoints.At(0);
      aPoint->ListVariables();
   }
}
//_____________________________________________________________________________
NucDBBinnedVariable* NucDBMeasurement::GetBinnedVariable(const char * name) {
   for(int i = 0;i<fBinnedVariables.GetEntries();i++) {
      if( !strcmp( ((NucDBBinnedVariable*)fBinnedVariables.At(i))->GetName(),name) ) 
         return((NucDBBinnedVariable*)fBinnedVariables.At(i));
   }
   return(0);
}
//______________________________________________________________________________
void NucDBMeasurement::AddDependentVariables(NucDBBinnedVariable * var){
   // Adding variables to already measurment with existing data.
   fDependentVariables.Add(var);
}
//______________________________________________________________________________
NucDBBinnedVariable* NucDBMeasurement::GetDependentVariable(const char * name) {
   for(int i = 0;i<fDependentVariables.GetEntries();i++) {
      if( !strcmp( ((NucDBBinnedVariable*)fDependentVariables.At(i))->GetName(),name) ) 
         return((NucDBBinnedVariable*)fDependentVariables.At(i));
   }
   return(0);
}
//______________________________________________________________________________
Int_t NucDBMeasurement::CalculateVariable(NucDBDependentVariable * var){
   if(!var) return -1;

   // Perhaps this should be in the datapoint class?

   const TList * datapoints = GetDataPoints();
   NucDBDataPoint      * p    = 0;
   NucDBBinnedVariable * v   = 0;
   NucDBBinnedVariable * vdat = 0;
   for(int i = 0;i<datapoints->GetEntries();i++) {
      p = (NucDBDataPoint*)datapoints->At(i);
      if(p) {
         for(int j = 0; j<var->GetNDependentVariables(); j++) {
            v    = var->GetVariable(j);
            if(v) {
               vdat = (NucDBBinnedVariable*)p->GetBinVariable(v->GetName());
               if(vdat){
                  var->SetVariable(j,vdat);
               } else {
                  Error("CalculateVariable","Could not datapoint variable with name %s", v->GetName());
               }
            } else {
              Error("CalculateVariable","Could not find concrete DV class variable");
              return -2;
            }
         }
         var->Calculate();
      }
   }
   return 0;
}
//______________________________________________________________________________
NucDBDiscreteVariable* NucDBMeasurement::GetDiscreteVariable(const char * name) {
   for(int i = 0;i<fDiscreteVariables.GetEntries();i++) {
      if( !strcmp( ((NucDBDiscreteVariable*)fDiscreteVariables.At(i))->GetName(),name) ) 
         return((NucDBDiscreteVariable*)fDiscreteVariables.At(i));
   }
   return(0);
}
//______________________________________________________________________________
Double_t NucDBMeasurement::GetBinnedVariableMean(const char * name) {
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
//______________________________________________________________________________
Double_t NucDBMeasurement::GetBinnedVariableVariance(const char * name) {
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
//______________________________________________________________________________
Double_t NucDBMeasurement::GetBinnedVariableMax(const char * name) {

   const TList * datapoints = GetDataPoints();
   NucDBDataPoint * p       = 0;
   NucDBBinnedVariable * v  = 0;
   double max               = 0;

   for(int i = 0;i<datapoints->GetEntries();i++) {
      p = (NucDBDataPoint*)datapoints->At(i);
      if(p) {
         v = (NucDBBinnedVariable*)p->GetBinVariable(name);
         if(v){
            double aval = v->GetBinMaximum();
            if( i == 0 ) {
               max = aval;
            }
            if( max < aval ) {
               max = aval;
            }
         } else {
            Warning("GetBinnedVariableMax","Binned Variable, %s, does not exist.",name );
         }
      }
   }
   return(max);
}
//______________________________________________________________________________
Double_t NucDBMeasurement::GetBinnedVariableMin(const char * name) {

   const TList * datapoints = GetDataPoints();
   NucDBDataPoint * p       = 0;
   NucDBBinnedVariable * v  = 0;
   double min               = 0;

   for(int i = 0;i<datapoints->GetEntries();i++) {
      p = (NucDBDataPoint*)datapoints->At(i);
      if(p) {
         v = (NucDBBinnedVariable*)p->GetBinVariable(name);
         if(v){
            double aval = v->GetBinMinimum();
            if( i == 0 ) {
               min = aval;
            }
            if( min > aval ) {
               min = aval;
            }
         } else {
            Warning("GetBinnedVariableMin","Binned Variable, %s, does not exist.",name );
         }
      }
   }
   return(min);
}
//______________________________________________________________________________
Int_t NucDBMeasurement::GetUniqueBinnedVariableValues(const char * name, std::vector<double>& vect) const {
   // Note that the unique values are appended to the supplied vector
   // Returns the number of points.
   Int_t N                       = 0;
   const TList &datapoints       = GetDataRef();
   const NucDBDataPoint * p      = 0;
   const NucDBBinnedVariable * v = 0;

   std::vector<double> values;

   for(int i = 0;i<datapoints.GetEntries();i++) {
      p = (const NucDBDataPoint*)datapoints.At(i);
      if(p) {
         v = (const NucDBBinnedVariable*)p->GetBinVariable(name);
         if(v){
            Double_t vmean = v->GetMean();
            values.push_back(vmean);
            //vect.push_back(vmean);
            N++;
         }
      }
   }

   // sort beques unique_copy only considers consecutive duplicates only.
   std::sort(values.begin(),values.end());
   //std::sort(vect.begin(),vect.end());
   //Int_t n1 = values.size();
   //Int_t n2 = vect.size();
   //vect.resize(n2+n1,0);
   //std::cout << " out of " << n1 << " values, " << n2 << " are unique\n";
   // copy uniqes to vector provided
   std::unique_copy(values.begin(),values.end(),std::back_inserter(vect));
   //vect.erase(std::unique(vect.begin(),vect.end()),vect.end());
   //n2 = vect.size();
   //std::cout << " out of " << n1 << " values, " << n2 << " are unique\n";

   return N;
   //for(int i = 0; i< vect->size() ; i++) {
   //   std::cout << name << "[" << i << "] = " << vect->at(i) << "\n";
   //}
}
//______________________________________________________________________________
Int_t NucDBMeasurement::GetUniqueBinnedVariableValues(const char * name,std::vector<double> & vect, std::vector<int> & counts) const {
   // Note that the unique values are appended to the supplied vector
   // Returns the number of points.
   Int_t N                       = 0;
   const TList &datapoints       = GetDataRef();
   const NucDBDataPoint * p      = 0;
   const NucDBBinnedVariable * v = 0;

   std::vector<double> values;

   for(int i = 0;i<datapoints.GetEntries();i++) {
      p = (const NucDBDataPoint*)datapoints.At(i);
      if(p) {
         v = (const NucDBBinnedVariable*)p->GetBinVariable(name);
         if(v){
            Double_t vmean = v->GetMean();
            values.push_back(vmean);
            N++;
         }
      }
   }
   // sort beques unique_copy only considers consecutive duplicates only.
   std::sort(values.begin(),values.end());

   // copy uniqes to vector provided
   std::unique_copy(values.begin(),values.end(),std::back_inserter(vect));
   //vect->erase(std::unique(vect->begin(),vect->end()),vect->end());

   for(int i = 0; i < vect.size(); i++ ) {
      double aval = vect[i];
      int nsame = std::count(values.begin(),values.end(), aval);
      counts.push_back(nsame); 
   }

   //std::cout << " counts " << counts.size() << " vect " << vect.size() << std::endl;
   //for(int i = 0; i< vect->size() ; i++) {
   //   std::cout << name << "[" << i << "] = " << vect->at(i) << "\n";
   //}
   return N;
}
//______________________________________________________________________________

void NucDBMeasurement::PrintBreakDown(const char * var, int nmax) const {
   // prints meta data about the distribution of data points
   // A table is created for the uniq values of the supplied variable :
   // The table is limited to N values (incase the supplied variable is never uniqe)
   //
   // var  #_points <var1> <var2> <var3> ...
   std::vector<double> var_values ;
   std::vector<int> var_counts ;
   GetUniqueBinnedVariableValues(var,var_values,var_counts);

   unsigned int n1 = var_values.size();
   unsigned int n2 = var_counts.size();
   //assert( n1 == n2 ) ;

   std::cout << std::setw(10) ;
   std::cout << var; 
   std::cout << std::setw(4) << " "; 
   std::cout << std::setw(11) ;
   std::cout << std::left ;
   std::cout << "N_points" << std::endl; 

   for(int i = 0; i < n1 && i<=nmax; i++) {
      std::cout << std::setw(10);
      std::cout << var_values[i];
      std::cout << std::setw(4) << " "; 
      std::cout << std::setw(11) << std::left;
      std::cout << var_counts[i] << std::endl;
   }


}

//______________________________________________________________________________
TGraphErrors * NucDBMeasurement::BuildGraph(const char * varName ) {
   //if(fGraph) delete fGraph;
   fGraph=0;
   NucDBDataPoint * point = 0;
   NucDBBinnedVariable * var = 0;
   fGraph = new TGraphErrors(fNumberOfDataPoints);
   fGraph->SetName(Form("%sVS%s",GetName(),varName));
   for(int i = 0; i < fNumberOfDataPoints;i++) {
      point = (NucDBDataPoint *) fDataPoints.At(i);
      var = point->FindVariable(varName);
      if( var ) {
         point->CalculateTotalError();
         fGraph->SetPoint(i,var->GetMean(),point->GetValue());
         fGraph->SetPointError(i,0.0,point->GetTotalError().GetError());
      } else {
         Error("BuildGraph",Form("Variable, %s, not found!",varName));
         fGraph->SetPoint(i,0,0);
         fGraph->SetPointError(i,0,1);
         break;
      }
   }
   if(var){
      fGraph->GetXaxis()->SetTitle(var->GetTitle());
      fGraph->SetTitle(Form("%s Vs %s",GetTitle(),var->GetTitle()));
   }
   fGraph->SetMarkerColor(GetColor());
   fGraph->SetLineColor(GetColor());
   fGraph->SetMarkerStyle(20);
   //fGraph->SetLineStyle(1);
   //fGraph->SetLineWidth(0);
   fGraph->SetDrawOption("aep");
   fGraphs.Add(fGraph);
      return(fGraph);
   }
//_____________________________________________________________________________

TGraphErrors * NucDBMeasurement::BuildKinematicGraph(const char * var1Name , const char * var2Name ) {
      //if(fGraph) delete fGraph;
      fGraph=0;
      NucDBDataPoint * point = 0;
      NucDBBinnedVariable * var1 = 0;
      NucDBBinnedVariable * var2 = 0;
      fGraph = new TGraphErrors(fNumberOfDataPoints);
      fGraph->SetName(Form("%sVS%s",var1Name,var2Name));
      for(int i = 0; i < fNumberOfDataPoints;i++) {
         point = (NucDBDataPoint *) fDataPoints.At(i);
         var1 = point->FindVariable(var1Name);
         var2 = point->FindVariable(var2Name);
         if( var1 && var2 ) {
            fGraph->SetPoint(i,var1->GetMean(),var2->GetMean());
            fGraph->SetPointError(i,0.0,0.0);
         } else {
            Error("BuildGraph",Form("Variable, %s or %s, not found!",var1Name,var2Name));
            fGraph->SetPoint(i,0,0);
            fGraph->SetPointError(i,0,1);
            break;
         }
      }
      if(var1 && var2){
         fGraph->GetXaxis()->SetTitle(var1->GetTitle());
         fGraph->GetYaxis()->SetTitle(var2->GetTitle());
         fGraph->SetTitle(Form("%s Vs %s",var1->GetTitle(),var2->GetTitle()));
      }
      fGraph->SetMarkerColor(GetColor());
      fGraph->SetLineColor(GetColor());
      fGraph->SetMarkerStyle(20);
      //fGraph->SetLineStyle(1);
      //fGraph->SetLineWidth(0);
      fGraph->SetDrawOption("aep");
      fGraphs.Add(fGraph);
      return(fGraph);


}
//_____________________________________________________________________________

