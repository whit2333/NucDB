#include "NucDBMeasurement.h"
#include <assert.h>
#include <iostream>
#include <iomanip>
#include <map>
#include <string>
#include "TGraphAsymmErrors.h"
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
}
//______________________________________________________________________________



NucDBMeasurement::NucDBMeasurement(const char * name,const char * title) :
   TNamed(name,title), TAttMarker(1,20,1.3)
   //Color_t color, Style_t style, Size_t msize
{
   fNumberOfDataPoints=0;
   fDataPoints.Clear();
   fExperimentName="";
}
//_____________________________________________________________________________

NucDBMeasurement::~NucDBMeasurement()
{ }
//_____________________________________________________________________________

NucDBMeasurement::NucDBMeasurement(const NucDBMeasurement& v):
  TNamed(v),TAttLine(v), TAttFill(v), TAttMarker(v)
{
   //SetNameTitle(v.GetName(),v.GetTitle());
   //fColor = v.GetColor();
   SetExperimentName(v.GetExperimentName());
   fNumberOfDataPoints = v.fNumberOfDataPoints;

   TList * datapoints = (TList *)(v.fDataPoints.Clone());
   fDataPoints.AddAll( datapoints );

   fFilterBins.AddAll((TList*)v.fFilterBins.Clone());
   //fBinnedVariables.AddAll(v.GetBinnedVariables());
   //fDependentVariables.AddAll(v.GetDependentVariables());
   fGraphs.AddAll(v.GetGraphs());
}
//_____________________________________________________________________________

void  NucDBMeasurement::Browse(TBrowser* b)
{
   b->Add(&fDataPoints     , "Data Points");
   b->Add(&fGraphs         , "Graphs");
   b->Add(&fReferences     , "Refs");
   //if(fGraph)b->Add(fGraph , "vs x");
}
//______________________________________________________________________________

void NucDBMeasurement::ClearDataPoints()
{
   fDataPoints.Clear();
   fNumberOfDataPoints=0;
   fSets.clear();
   fNSets = 0;
}
//_____________________________________________________________________________

void NucDBMeasurement::AddDataPoint(NucDBDataPoint *point)
{
   if(point) {
      point->SetNameTitle(Form("p%d",fNumberOfDataPoints),Form("p%d",fNumberOfDataPoints));
      point->SetDataSet(fNSets);
      fDataPoints.Add(point);
      fNumberOfDataPoints++;
   } else {
      Error("AddDataPoint","NULL dat point");
   } 
}
//_____________________________________________________________________________

void NucDBMeasurement::RemoveDataPoint(NucDBDataPoint *point)
{
   if(!point) {
      Error("RemoveDataPoint","NULL datapoint provided");
   } else {
      fDataPoints.Remove(point);
      fNumberOfDataPoints--;
   } 
}
//_____________________________________________________________________________

void NucDBMeasurement::AddDataPoints(TList * listOfPoints, bool clear)
{
   if(listOfPoints) {
      if(clear) ClearDataPoints();
      for(int i = 0; i < listOfPoints->GetEntries();i++) 
         AddDataPoint(new NucDBDataPoint(*((NucDBDataPoint*)listOfPoints->At(i))));
   } else {
      Error("AddDataPoints","NULL list of points");
   } 
}
//_____________________________________________________________________________

void NucDBMeasurement::RemoveDataPoints(TList * listOfPoints)
{
   if(listOfPoints) {
      for(int i = 0; i < listOfPoints->GetEntries();i++) 
         RemoveDataPoint((NucDBDataPoint*)listOfPoints->At(i));
   } else {
      Error("RemoveDataPoints","NULL list of points");
   } 
}
//_____________________________________________________________________________

TList *  NucDBMeasurement::MergeDataPoints(const std::vector<int> & points, bool modify)
{
   //  merge  points by the vector of number indicating how many points to merge 
   // A vector of {3,2,5} will merge 3 bins, then the next 2, then the next 5.
   // After which no m ore merging is done.
   // sorting shold be done before hand.
   TList * list = new TList();
   NucDBDataPoint * mergedPoint = 0;
   Int_t n_merged = 0;
   Int_t Nmax = fDataPoints.GetEntries();
   for(auto Nmerge : points ) {
      int i = 0;
      if(n_merged >= Nmax ) break;
      while(i < Nmerge) {
         if(n_merged >= Nmax ) break;
         NucDBDataPoint * point = (NucDBDataPoint*)fDataPoints.At(n_merged);
         if(i%Nmerge == 0){
            mergedPoint = new NucDBDataPoint(*point);
            list->Add(mergedPoint);
         } else {
            (*mergedPoint) += (*point);
         }
         n_merged++;
         i++;
      }
   }
   // Add all the remaining points
   while(n_merged < fDataPoints.GetEntries()) {
      NucDBDataPoint * point = (NucDBDataPoint*)fDataPoints.At(n_merged);
      mergedPoint = new NucDBDataPoint(*point);
      list->Add(mergedPoint);
      n_merged++;
   }
   if(modify) {
      AddDataPoints(list,true);
   }
   return list;
}
//______________________________________________________________________________

TList * NucDBMeasurement::MergeDataPoints(
      unsigned int n,
      const char * var,
      bool modify)
{
   SortBy(var);
   TList * list = new TList();
   list->Clear();
   if(n<1) {
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
//______________________________________________________________________________

NucDBDataPoint * NucDBMeasurement::MergeDataPoints(
      NucDBBinnedVariable * var, 
      NucDBBinnedVariable * var2, 
      bool                  modify)
{
   if(!var) {
      std::cout << "Null binned variable provided!" << std::endl;
      return 0;
   }
   TList * plist = FilterWithBin(var);
   if(!plist) {
      std::cout << "Null list returned!" << std::endl;
      return 0;
   }
   if(var2) {
      plist = NucDB::FilterDataPoints(plist,var2);
   }
   if( plist->GetEntries() <= 0 ) {
      std::cout << "Empty list of data points!" << std::endl;
      return 0;
   }
   NucDBDataPoint * aPoint  = NucDB::MergeDataPoints(plist);
   if(modify) {
      RemoveDataPoints(plist);
      AddDataPoint(aPoint);
   }
   return aPoint;
}
//______________________________________________________________________________

TList * NucDBMeasurement::MergeNeighboringDataPoints(unsigned int n, const char * var, double dx, bool modify) {
   // Same as above but only adjacent merges bins if they are within dx of each other.
   // Note that the argument n provides the limit on the number of adjacent bins 
   // that can be merged.
   //SortBy(var);

   TList * list = new TList();
   list->Clear();
   if(n<1) {
      Error("MergeDataPoints","Wrong number of bins to merge. ");
      return list;
   }
   NucDBDataPoint * mergedPoint = 0;
   unsigned int i = 0;
   unsigned int nMerged = 0;
   double mean_0 = 0.0;
   double mean_1 = 0.0;
   NucDBBinnedVariable * previous_var = 0;
   while(i < fDataPoints.GetEntries()) {
      NucDBDataPoint * point = (NucDBDataPoint*)fDataPoints.At(i);
      NucDBBinnedVariable * var1 = point->GetBinVariable(var);
      if(!var1) {
         Error("MergeDataPoints(unsigned int, const char*, double,bool)","Could not find variable %s",var);
         continue;
      }
      mean_1 = var1->GetMean();
      if(nMerged == 0 || i%n == 0){
         mergedPoint = new NucDBDataPoint(*point);
         list->Add(mergedPoint);
         nMerged = 0;
         nMerged++;
         mean_0 = mean_1;
      } else {
         if( TMath::Abs(mean_1 - mean_0) < dx ) {
            // merge the neighboring point
            (*mergedPoint) += (*point);
            nMerged++;
         } else {
            // otherwise we reset the merged point counter to and create a  new starting datapoint
            mergedPoint = new NucDBDataPoint(*point);
            list->Add(mergedPoint);
            nMerged = 0;
            nMerged++;
            mean_0 = mean_1;
         }

      }
      i++;
      previous_var = var1;
   }
   if(modify) {
      fDataPoints.SetOwner(false);
      AddDataPoints(list,true);
      fDataPoints.SetOwner(true);
   }
   return list;
}
//______________________________________________________________________________

TList * NucDBMeasurement::MergeDataPoints(
      unsigned int n, 
      const std::vector<NucDBBinnedVariable>& bin_sizes, 
      bool modify)
{
   // Merge any data points where all the bin means fall within the provided bin sizes.
   // Only the size of each of the bin_sizes is used. 
   // Using the operator- for the datapoint/bins 

   TList * datapoints = (TList*)(fDataPoints.Clone());
   //datapoints->Print();
   datapoints->SetOwner(false);

   TList * list = new TList();
   list->Clear();
   if(n<1) {
      Error("MergeDataPoints","Wrong number of bins to merge.");
      return list;
   }

   NucDBDataPoint* p1 = nullptr;
   NucDBDataPoint* p2 = nullptr;
   NucDBDataPoint* mergedPoint = nullptr;
   int nMerged = 0;

   TIter next(datapoints);
   while( (p1  = (NucDBDataPoint*)next()) ) {

      mergedPoint = new NucDBDataPoint(*p1);
      list->Add(mergedPoint);
      datapoints->Remove(p1);
      nMerged = 0;
      nMerged++;
      bool merge_bins = true;

      // Loop over the remaining un-merged data points
      TIter next2( datapoints );
      while( (p2 = (NucDBDataPoint*)next2()) ) {

         merge_bins = true;
         NucDBDataPoint delta_p(*p1);
         delta_p -= (*p2);

         for(auto bin : bin_sizes){
            double bin_half_width = bin.GetBinSize();
            auto   bin_delta      = delta_p.GetBinVariable(bin.GetName());
            double diff           = bin_delta->GetMean();
            //bin_delta->Print();
            if( diff*diff > bin_half_width*bin_half_width ) {
               merge_bins = false;
               break;
            }
         }
         if( merge_bins ){
            //delta_p.Print();
            (*mergedPoint) += (*p2);
            nMerged++;
            datapoints->Remove(p2);
         }
         if( n <= nMerged ){
            //std::cout << "reached " << n << std::endl;
            //mergedPoint->Print();
            next  = TIter(datapoints);
            break;
         }
      }
      next  = TIter(datapoints);
   }

   //datapoints->Print();
   if(modify) {
      fDataPoints.SetOwner(false);
      AddDataPoints(list,true);
      fDataPoints.SetOwner(true);
   }
   return list;
}
//______________________________________________________________________________

TList *  NucDBMeasurement::MergeNeighboringDataPoints(
      unsigned int   n,
      const char *   varname, double         dx,
      const char *   varname2, double         dx2,
      bool           modify )
{
   // Same as above but only adjacent merges bins if they are within dx of each other.
   // note that the argument n provides the limit on the number of adjacent bins 
   // that can be merged.
   //SortBy(varname,varname2);
   TList * list = new TList();
   list->Clear();
   if(n<1) {
      Error("MergeDataPoints","Wrong number of bins to merge. ");
      return list;
   }

   NucDBDataPoint *      mergedPoint   = 0;
   unsigned int          i             = 0;
   unsigned int          nMerged       = 0;
   NucDBBinnedVariable * previous_var  = 0;
   NucDBBinnedVariable * previous_var2 = 0;
   double mean_0  = 0.0;
   double mean_1  = 0.0;
   double mean2_0 = 0.0;
   double mean2_1 = 0.0;

   while( i < fDataPoints.GetEntries() ) {

      NucDBDataPoint      * point = (NucDBDataPoint*)fDataPoints.At(i);
      NucDBBinnedVariable * var1  = point->GetBinVariable(varname);
      NucDBBinnedVariable * var2  = point->GetBinVariable(varname2);

      if(!var1) {
         Error("MergeDataPoints(unsigned int, const char*, double,bool)","Could not find variable %s",varname);
         continue;
      }
      if(!var2) {
         Error("MergeDataPoints(unsigned int, const char*, double,bool)","Could not find variable %s",varname2);
         continue;
      }
      mean_1   = var1->GetMean();
      mean2_1  = var2->GetMean();
      mean_0   = mean_1;
      mean2_0  = mean2_1;

      if( nMerged == 0 || i%n == 0 ){
         mergedPoint = new NucDBDataPoint(*point);
         list->Add(mergedPoint);
         nMerged = 0;
         nMerged++;
         mean_0  = mean_1;
         mean2_0 = mean2_1;
      } else {
         mean_0  = mergedPoint->GetBinVariable(varname)->GetMean();
         mean2_0 = mergedPoint->GetBinVariable(varname2)->GetMean();
         if( (TMath::Abs(mean_0 - mean_1) < dx) && (TMath::Abs(mean2_0 - mean2_1) < dx2) ) {
            // merge the neighboring point
            (*mergedPoint) += (*point);
            nMerged++;
         } else {
            // otherwise we reset the merged point counter to and create a new starting datapoint
            mergedPoint = new NucDBDataPoint(*point);
            list->Add(mergedPoint);
            nMerged = 0;
            nMerged++;
            mean_0 = mean_1;
            mean2_0 = mean2_1;
         }
      }
      i++;
      //previous_var  = var1;
      //previous_var2 = var2;
   }
   if(modify) {
      AddDataPoints(list,true);
   }
   return list;
}
//_____________________________________________________________________________

void NucDBMeasurement::SortBy(const std::vector<std::string> & names)
{
   // Sort the data by the bins.
   // The vector are the names of bins.
   // The very first bin is sorted by the bin mean, the 
   // subsequent bins are sorted by the bin center. In this way a weak
   // ordering can be easily achieved assuming many of the bins are the same.
   fDataPoints.SetOwner(false);
   for(int i = 0; i < fDataPoints.GetEntries();i++) {
      NucDBDataPoint * point = GetDataPoint(i);
      point->SetSortPriorities(names);
   }
   TList * sortedlist  = NucDB::StableSort(fDataPoints);
   AddDataPoints(sortedlist,true);
   fDataPoints.SetOwner(true);
}
//_____________________________________________________________________________

void NucDBMeasurement::SortBy(const char * n, const char * n2)
{
   // Sort data according the value of the binned variable
   std::string name2 = n2;
   fDataPoints.SetOwner(false);
   for(int i = 0; i < fDataPoints.GetEntries();i++) {
      NucDBDataPoint * point = (NucDBDataPoint*)fDataPoints.At(i);
      point->SetSortingVariable(n);
      point->SetSortingVariable2(n2);
      //point->Print();
   }
   if( !(name2.empty()) ) {
      TList * sortedlist  = NucDB::StableSort(&fDataPoints);
      AddDataPoints(sortedlist,true);
   } else {
      fDataPoints.Sort();
   }
   fDataPoints.SetOwner(true);
}
//_____________________________________________________________________________

void  NucDBMeasurement::Multiply(const char * v)
{
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

NucDBMeasurement * NucDBMeasurement::CreateMeasurementFilteredWithBin(NucDBBinnedVariable const * bin)
{
   NucDBMeasurement * m = new NucDBMeasurement(
         GetName(),
         GetTitle() );
   m->AddDataPoints(FilterWithBin(bin));
   m->SetExperimentName(GetExperimentName());
   return m;
}  
//______________________________________________________________________________

NucDBMeasurement * NucDBMeasurement::NewMeasurementWithFilter(NucDBBinnedVariable const * bin)
{
   // same as CreateMeasurementFilteredWithBin above
   return( CreateMeasurementFilteredWithBin(bin) ); 
}
//______________________________________________________________________________

NucDBMeasurement * NucDBMeasurement::GetDataSet(int i_set)
{
   NucDBMeasurement * m = new NucDBMeasurement(
         GetName(),
         GetTitle() );
   m->SetExperimentName(GetExperimentName());

   for(int i = 0; i < fDataPoints.GetEntries();i++) {
      NucDBDataPoint      * point = (NucDBDataPoint*)fDataPoints.At(i);
      if(point->GetDataSet() == i_set) {
         m->AddDataPoint(new NucDBDataPoint(*point));
      }
   }
   return m;
}
//______________________________________________________________________________

TList * NucDBMeasurement::CreateMeasurementsWithUniqueBins(const std::vector<double> & vect, const char * var)
{
   TList * list = new TList();
   int i = 0;
   for(double val : vect )  {
      NucDBBinnedVariable aVar(var,var,val,0.001);
      NucDBMeasurement * aMeas = new NucDBMeasurement(Form("%s_%d",GetName(),i),Form("%s %d",GetTitle(),i));
      aMeas->AddDataPoints( FilterWithBin(&aVar) );
      list->Add(aMeas);
      i++;
   }
   return list;
}
//______________________________________________________________________________

TList * NucDBMeasurement::CreateMeasurementsWithUniqueBins(const char * var)
{
   std::vector<double> unique_values;
   GetUniqueBinnedVariableValues(var,unique_values );
   return( CreateMeasurementsWithUniqueBins(unique_values,var) );
}

//_____________________________________________________________________________

TList *  NucDBMeasurement::FilterWith(NucDBVariable const *v)
{
   TList * list = new TList();
   list->Clear();
   if(!v) {
      printf(" NULL NucDBVariable pointer. Returning list with no entries. \n");
      return list;
   }
   for(int i = 0; i < fDataPoints.GetEntries();i++) {
      NucDBDataPoint * point = (NucDBDataPoint*)fDataPoints.At(i);
      NucDBVariable  * var    = point->GetVariable(v->GetName());
      if(var){
         if ( (*v) == (*var) ) list->Add(point);
      }
   }
   return list;
}
//_____________________________________________________________________________

TList * NucDBMeasurement::FilterWith(NucDBDiscreteVariable const *v)
{
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

TList *  NucDBMeasurement::FilterWithBin(NucDBBinnedVariable const *bin)
{
   TList * list = new TList();
   list->Clear();
   if(!bin) {
      printf(" NULL NucDBBinnedVariable pointer. Returning list with no entries. \n");
      return list;
   }
   for(int i = 0; i < fDataPoints.GetEntries();i++) {
      NucDBDataPoint      * point = (NucDBDataPoint*)fDataPoints.At(i);
      NucDBBinnedVariable * var   = point->GetBinVariable(bin->GetName());
      if(var){
         if( bin->Contains(var->GetMean()) ){
         //if( (*var) == (*bin) ){
            list->Add(point);
         }
         continue;
      }
      NucDBDependentVariable * dvar = point->GetDependentVariable(bin->GetName());
      if(dvar){
         if ( (*dvar) == (*bin) ) {
            list->Add(point);
         }
      }
   }
   return list;
}
//_____________________________________________________________________________

TList *  NucDBMeasurement::ApplyFilterWithBin(NucDBBinnedVariable *bin)
{
   TList * list = FilterWithBin(bin); 
   if(list->GetEntries() <= 0 ) {
      fDataPoints.SetOwner(false);
      ClearDataPoints();
      fDataPoints.SetOwner(true);
      return list;
   }
   AddDataPoints(list,true);
   fFilterBins.Add(bin);
   return list;
}
//_____________________________________________________________________________

TList *  NucDBMeasurement::ApplyFilterWith(NucDBDiscreteVariable *v)
{
   TList * list = FilterWith(v); 
   AddDataPoints(list,true);
   fFilterBins.Add(v);
   return list;
}
//_____________________________________________________________________________

TList *  NucDBMeasurement::ApplyFilterWith(NucDBVariable *v)
{
   TList * list = FilterWith(v); 
   AddDataPoints(list,true);
   fFilterBins.Add(v);
   return list;
}
//______________________________________________________________________________

void NucDBMeasurement::Print(Option_t * opt ) const
{
   TString opt1         = opt; opt1.ToLower();
   Bool_t printData     = opt1.Contains("data") ? kTRUE  : kFALSE;
   Bool_t printComments = opt1.Contains("comm") ? kTRUE  : kFALSE;
   Bool_t printRefs     = opt1.Contains("ref")  ? kTRUE  : kFALSE;
   Bool_t printvars     = opt1.Contains("v")  ? kTRUE  : kFALSE;
   std::cout << " --------------------------\n";
   std::cout << " " << GetName() << "\n";
   std::cout << "     title       : " << GetTitle() << "\n";
   std::cout << "     Experiment  : " << GetExperimentName() << std::endl;
   std::cout << "     NDataPoints : " << fNumberOfDataPoints <<  ", in " << fNSets << " data-sets\n";
   std::cout << "     Type        : " << fType <<  "\n";
   std::cout << "     Processes   : " ; for(const auto p:fProcesses){ std::cout << p << " ";}; std::cout <<  "\n";
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

void NucDBMeasurement::PrintData(Option_t * opt) const
{
   Print();
   for(int i=0; i<fDataPoints.GetEntries(); i++){
      NucDBDataPoint * aPoint = (NucDBDataPoint*)fDataPoints.At(i);
      std::cout << "[" << i << "] " << GetName();
      aPoint->Print();
   }
}
//______________________________________________________________________________

void NucDBMeasurement::PrintTable(std::ostream& stream) const
{
   // Print a data table

   if(fNumberOfDataPoints>0){
      const NucDBDataPoint * p0 = (const NucDBDataPoint*)fDataPoints.At(0);
      const TList& vars = p0->GetBinnedVariablesRef(); 
      for(int j=0;j<vars.GetEntries();j++) {
         NucDBBinnedVariable * var = (NucDBBinnedVariable*)vars.At(j);
         std::string column = var->GetName();
         column += "_min";
         stream << std::setw(14) << column << " " ;
         column = var->GetName();
         stream << std::setw(14) << column << " " ;
         column += "_max";
         stream << std::setw(14) << column << " " ;
      }
      std::string column =  GetName();
      stream <<  std::setw(14) << column << " " ;
      column += "_StatErr";
      stream << std::setw(14) << column << " " ;
      column =  GetName();
      column += "_SystErr";
      stream << std::setw(14) << column  ;
      stream << std::endl;
   }

   for(int i=0; i<fDataPoints.GetEntries(); i++){

      NucDBDataPoint * aPoint = (NucDBDataPoint*)fDataPoints.At(i);

      const TList& vars = aPoint->GetBinnedVariablesRef(); 

      for(int j=0;j<vars.GetEntries();j++) {
         NucDBBinnedVariable * var = (NucDBBinnedVariable*)vars.At(j);
         stream << std::setw(14) << var->GetMinimum() << " " ;
         stream << std::setw(14) << var->GetMean() << " " ;
         stream << std::setw(14) << var->GetMaximum() << " " ;
      }
      stream <<  std::setw(14) << aPoint->GetValue() << " " 
             << std::setw(14) << aPoint->GetStatError().GetError() << " "  
             << std::setw(14) << aPoint->GetSystError().GetError() ;
      stream << std::endl;
   }
}
//_____________________________________________________________________________

void  NucDBMeasurement::ListVariables()
{
   if(fDataPoints.GetEntries()>0) {
      NucDBDataPoint * aPoint = (NucDBDataPoint*)fDataPoints.At(0);
      aPoint->ListVariables();
   }
}
//_____________________________________________________________________________

NucDBBinnedVariable* NucDBMeasurement::GetBinnedVariable(const char * name)
{
   if(fDataPoints.GetEntries()>0) {
      NucDBDataPoint * aPoint = (NucDBDataPoint*)fDataPoints.At(0);
      return(dynamic_cast<NucDBBinnedVariable*>(aPoint->GetBinVariable(name)));
   }
   //for(int i = 0;i<fBinnedVariables.GetEntries();i++) {
   //   if( !strcmp( ((NucDBBinnedVariable*)fBinnedVariables.At(i))->GetName(),name) ) 
   //      return((NucDBBinnedVariable*)fBinnedVariables.At(i));
   //}
   return(0);
}
//______________________________________________________________________________

Int_t NucDBMeasurement::AddDependentVariable(NucDBDependentVariable * var)
{
   // Adding variables to already measurment with existing data.
   if(!var) return -1;
   const TList * datapoints = GetDataPoints();
   NucDBDataPoint      * p    = 0;
   NucDBDependentVariable * vcopy = 0;
   for(int i = 0;i<datapoints->GetEntries();i++) {
      p = (NucDBDataPoint*)datapoints->At(i);
      if(p) {
         vcopy = new NucDBDependentVariable(*var);
         p->AddDependentVariable(vcopy);
         vcopy->Calculate();
         //p->CalculateDependentVariables();
         //p->Print();
         //var->Print();
      }
   }
   return 0;
   //if(CalculateVariable(var)) {
   //   Error("AddDependentVariable","Could not add variable");

   //   //fDependentVariables.Add(var);
   //}
}
//______________________________________________________________________________

NucDBDependentVariable* NucDBMeasurement::GetDependentVariable(const char * name)
{
   if(fDataPoints.GetEntries()>0) {
      NucDBDataPoint * aPoint = (NucDBDataPoint*)fDataPoints.At(0);
      return(dynamic_cast<NucDBDependentVariable*>(aPoint->GetBinVariable(name)));
   }
   //for(int i = 0;i<fDependentVariables.GetEntries();i++) {
   //   if( !strcmp( ((NucDBBinnedVariable*)fDependentVariables.At(i))->GetName(),name) ) 
   //      return((NucDBBinnedVariable*)fDependentVariables.At(i));
   //}
   return(0);
}
//______________________________________________________________________________

//Int_t NucDBMeasurement::CalculateVariable(NucDBDependentVariable * var){
//   if(!var) return -1;
//
//   // Perhaps this should be in the datapoint class?
//
//   const TList * datapoints = GetDataPoints();
//   NucDBDataPoint      * p    = 0;
//   NucDBBinnedVariable * v   = 0;
//   NucDBBinnedVariable * vdat = 0;
//   for(int i = 0;i<datapoints->GetEntries();i++) {
//      p = (NucDBDataPoint*)datapoints->At(i);
//      if(p) {
//         for(int j = 0; j<var->GetNDependentVariables(); j++) {
//            v    = var->GetVariable(j);
//            if(v) {
//               vdat = (NucDBBinnedVariable*)p->GetBinVariable(v->GetName());
//               if(!vdat) vdat = (NucDBBinnedVariable*)p->GetDependentVariable(v->GetName());
//               if(vdat){
//                  var->SetVariable(j,vdat);
//               } else {
//                  Error("CalculateVariable","Datapoint has no variable %s for calculating %s", v->GetName(), var->GetName());
//                  return -3;
//               }
//            } else {
//              Error("CalculateVariable","Could not find concrete DV class variable");
//              return -2;
//            }
//         }
//         var->Calculate();
//         p->AddDependentVariable(var);
//         //p->Print();
//         //var->Print();
//      }
//   }
//   return 0;
//}
//______________________________________________________________________________

NucDBDiscreteVariable* NucDBMeasurement::GetDiscreteVariable(const char * name)
{
   if(fDataPoints.GetEntries()>0) {
      NucDBDataPoint * aPoint = (NucDBDataPoint*)fDataPoints.At(0);
      return(dynamic_cast<NucDBDiscreteVariable*>(aPoint->GetDiscreteVariable(name)));
   }
   //for(int i = 0;i<fDiscreteVariables.GetEntries();i++) {
   //   if( !strcmp( ((NucDBDiscreteVariable*)fDiscreteVariables.At(i))->GetName(),name) ) 
   //      return((NucDBDiscreteVariable*)fDiscreteVariables.At(i));
   //}
   return(0);
}
//______________________________________________________________________________

Double_t NucDBMeasurement::GetBinnedVariableMean(const char * name)
{
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

Double_t NucDBMeasurement::GetBinnedVariableVariance(const char * name)
{
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

Double_t NucDBMeasurement::GetBinnedVariableMax(const char * name)
{
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

Double_t NucDBMeasurement::GetBinnedVariableMin(const char * name)
{

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

Int_t NucDBMeasurement::GetUniqueBinnedVariableValues(const char * name, std::vector<double>& vect) const
{
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

Int_t NucDBMeasurement::GetUniqueBinnedVariableValues(
      const char * name,std::vector<double> & vect,
      std::vector<int> & counts) const
{
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

void NucDBMeasurement::PrintBreakDown(const char * var, int nmax) const
{
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

TGraphErrors * NucDBMeasurement::BuildSystematicErrorBand(const char * varName, double offset)
{
   //if(fGraph) delete fGraph;
   TGraphErrors        * gr    = 0;
   NucDBDataPoint      * point = 0;
   NucDBBinnedVariable * var   = 0;
   gr = new TGraphErrors(fNumberOfDataPoints);
   for(int i = 0; i < fNumberOfDataPoints;i++) {
      point = (NucDBDataPoint *) fDataPoints.At(i);
      var = point->FindVariable(varName);
      if( var ) {
         point->CalculateTotalError();
         double err = point->GetSystError().GetError();
            gr->SetPoint(i,var->GetMean(),err+offset);
            gr->SetPointError(i,0.0,err);
         if( std::isinf(err) || std::isnan(err) ) {
            gr->SetPoint(i,var->GetMean(),0.0+offset);
            gr->SetPointError(i,0.0,0.0);
         }
      } else {
         Error("BuildGraph","Variable, %s, not found!",varName);
         gr->SetPoint(i,0,0);
         gr->SetPointError(i,0,0);
         break;
      }
   }
   gr->SetMarkerColor(GetMarkerColor());
   gr->SetLineColor(GetLineColor());
   gr->SetMarkerStyle(GetMarkerStyle());
   gr->SetDrawOption("aep");
   fGraphs.Add(gr);
   return(gr);
}
//______________________________________________________________________________

TGraphErrors * NucDBMeasurement::BuildSystematicErrorBand(const char * varName, std::function<double(double)> fn, double offset)
{
   //if(fGraph) delete fGraph;
   TGraphErrors        * gr    = 0;
   NucDBDataPoint      * point = 0;
   NucDBBinnedVariable * var   = 0;
   gr = new TGraphErrors(fNumberOfDataPoints);
   for(int i = 0; i < fNumberOfDataPoints;i++) {
      point = (NucDBDataPoint *) fDataPoints.At(i);
      var = point->FindVariable(varName);
      double multiplier = 1.0;
      if( var ) {
         point->CalculateTotalError();
         double err = point->GetSystError().GetError();
         multiplier = fn( var->GetMean() );
         gr->SetPoint(i,var->GetMean(), multiplier*err + offset);
         gr->SetPointError(i,0.0,multiplier*err);
      } else {
         Error("BuildGraph","Variable, %s, not found!",varName);
         gr->SetPoint(i,0,0);
         gr->SetPointError(i,0,0);
         break;
      }
   }
   gr->SetMarkerColor(GetMarkerColor());
   gr->SetLineColor(GetLineColor());
   gr->SetMarkerStyle(GetMarkerStyle());
   gr->SetDrawOption("aep");
   fGraphs.Add(gr);
   return(gr);
}
//______________________________________________________________________________

TGraphErrors * NucDBMeasurement::BuildGraph(const char * varName , bool syst_err)
{
   //if(fGraph) delete fGraph;
   TGraphErrors        * gr    = 0;
   NucDBDataPoint      * point = 0;
   NucDBBinnedVariable * var   = 0;
   gr = new TGraphErrors(fNumberOfDataPoints);
   gr->SetName(Form("%sVS%s",GetName(),varName));
   for(int i = 0; i < fNumberOfDataPoints;i++) {
      point = (NucDBDataPoint *) fDataPoints.At(i);
      var = point->FindVariable(varName);
      if( var ) {
         point->CalculateTotalError();
         double err = point->GetStatError().GetError();
         if( syst_err ) { 
            err += point->GetSystError().GetError();
         }
         gr->SetPoint(i,var->GetMean(),point->GetValue());
         gr->SetPointError(i,0.0,err);
      } else {
         Error("BuildGraph","Variable, %s, not found!",varName);
         gr->SetPoint(i,0,0);
         gr->SetPointError(i,0,1);
         break;
      }
   }
   if(var && gr){
      gr->GetXaxis()->SetTitle(var->GetTitle());
      gr->SetTitle(Form("%s Vs %s",GetTitle(),var->GetTitle()));
   }
   gr->SetMarkerColor(GetMarkerColor());
   gr->SetLineColor(GetLineColor());
   gr->SetMarkerStyle(GetMarkerStyle());
   gr->SetDrawOption("aep");
   fGraphs.Add(gr);
   return(gr);
}
//_____________________________________________________________________________

TGraphErrors * NucDBMeasurement::BuildOrderedGraph(const char * varName, bool syst_err)
{
   //if(fGraph) delete fGraph;
   TGraphErrors        * gr    = 0;
   NucDBDataPoint      * point = 0;
   NucDBBinnedVariable * var   = 0;
   gr = new TGraphErrors(fNumberOfDataPoints);
   //gr->SetName(Form("%sVS%s",GetName(),varName));
   for(int i = 0; i < fNumberOfDataPoints; i++) {

      point = (NucDBDataPoint*)fDataPoints.At(i);
      var   = point->FindVariable(varName);
      if( var ){

         point->CalculateTotalError();
         double err = point->GetStatError().GetError();
         if( syst_err ) { 
            err += point->GetSystError().GetError();
         }
         //gr->SetPoint(i,i,var->GetMean(),point->GetValue());
         gr->SetPoint(i,var->GetMean(),i);
         gr->SetPointError(i,0.0,err);
      } else {
         Error("BuildGraph","Variable, %s, not found!",varName);
         gr->SetPoint(i,0,i);
         gr->SetPointError(i,0,1);
         break;
      }
   }
   if(var && gr){
      gr->GetXaxis()->SetTitle(var->GetTitle());
      gr->SetTitle(Form("%s Vs %s",GetTitle(),var->GetTitle()));
   }
   gr->SetMarkerColor(GetMarkerColor());
   gr->SetLineColor(GetLineColor());
   gr->SetMarkerStyle(GetMarkerStyle());
   gr->SetDrawOption("aep");
   fGraphs.Add(gr);
   return(gr);
}
//______________________________________________________________________________

TGraph * NucDBMeasurement::BuildKinematicGraph(const char * var1Name , const char * var2Name )
{
   TGraphAsymmErrors        * gr    = 0;
   NucDBDataPoint      * point = 0;
   NucDBBinnedVariable * var1  = 0;
   NucDBBinnedVariable * var2  = 0;
   gr = new TGraphAsymmErrors(fNumberOfDataPoints);
   gr->SetName(Form("%sVS%s",var1Name,var2Name));
   for(int i = 0; i < fNumberOfDataPoints;i++) {
      point = (NucDBDataPoint *) fDataPoints.At(i);
      var1 = point->FindVariable(var1Name);
      var2 = point->FindVariable(var2Name);
      if( var1 && var2 ) {
         gr->SetPoint(i,var1->GetMean(),var2->GetMean());
         gr->SetPointError(i,var1->GetBinSizeLow(),var1->GetBinSizeHigh(),var2->GetBinSizeLow(),var2->GetBinSizeHigh());
      } else {
         Error("BuildGraph","Variable, %s or %s, not found!",var1Name,var2Name);
         gr->SetPoint(i,0,0);
         gr->SetPointError(i,0,0,0,0);
         break;
      }
   }
   if(var1 && var2){
      gr->GetXaxis()->SetTitle(var1->GetTitle());
      gr->GetYaxis()->SetTitle(var2->GetTitle());
      gr->SetTitle(Form("%s Vs %s",var1->GetTitle(),var2->GetTitle()));
   }
   gr->SetMarkerColor(GetMarkerColor());
   gr->SetLineColor(GetLineColor());
   gr->SetMarkerStyle(GetMarkerStyle());
   //gr->SetLineStyle(1);
   //gr->SetLineWidth(0);
   gr->SetDrawOption("aep");
   fGraphs.Add(gr);
   return(gr);
}
//_____________________________________________________________________________

TMultiGraph * NucDBMeasurement::BuildGraphUnique(
      const char * var,
      const char * uniqueVar,
      TLegend    * leg )
{
   // returns a multi graph for unique values plotted vs var
   TMultiGraph * mg = new TMultiGraph();

   NucDBManager * dbman = NucDBManager::GetManager();

   std::vector<double> unique_var;
   std::vector<int> unique_var_counts;
   GetUniqueBinnedVariableValues(uniqueVar,unique_var,unique_var_counts);
   if( unique_var.size() == 0 ){
      Warning("BuildGraphUnique","No unique variable values for %s",uniqueVar);
      return mg;
   }
   //if(fGraph) delete fGraph;
   TGraphErrors        * gr    = 0;
   NucDBDataPoint      * point = 0;
   NucDBBinnedVariable * var1   = 0;
   NucDBBinnedVariable * var2   = 0;

   for(int j =0; j< unique_var.size();j++) {

      NucDBBinnedVariable var0( uniqueVar, uniqueVar, unique_var[j],0.01);

      gr = new TGraphErrors(unique_var_counts[j]);
      gr->SetName(Form("%sVS%s_%d",GetName(),var,j));

      std::cout << " unique var " << unique_var_counts[j] << std::endl;

      int nPoints = 0;

      for(int i = 0; i < fNumberOfDataPoints;i++) {

         point = (NucDBDataPoint *) fDataPoints.At(i);
         var1  = point->FindVariable(var);
         var2  = point->FindVariable(uniqueVar);

         if( !var ) {
            Error("BuildGraph","Variable, %s, not found!",var);
            gr->SetPoint(i,0,0);
            gr->SetPointError(i,0,1);
            continue;
         }
         if( !var2 ) {
            Error("BuildGraph","Variable, %s, not found!",uniqueVar);
            gr->SetPoint(i,0,0);
            gr->SetPointError(i,0,1);
            continue;
         }

         //var0.Dump();
         //var2->Dump();

         if( var2->GetMean() == unique_var[j] ) {
            //std::cout << " equal bins" << std::endl;
            // variable is in bin
            if(nPoints >= unique_var_counts[j]) {
               Error("BuildGraphUnique","Too many points for unique value");
               continue;
            }
            point->CalculateTotalError();
            gr->SetPoint( nPoints,var1->GetMean(),point->GetValue());
            gr->SetPointError(nPoints,0.0,point->GetTotalError().GetError());
            nPoints++;
            if(leg) {
               if(leg->GetListOfPrimitives()->GetEntries() == j) {
                  leg->AddEntry(gr,Form("%s %s = %.2f", GetExperimentName(),var0.GetTitle(),unique_var[j]), "p" );
               }
            }
         }
      } // loop on points

      Int_t acol = dbman->NextColor();
      gr->SetMarkerColor( acol );
      gr->SetLineColor(  acol );
      gr->SetMarkerStyle(dbman->NextMarker());
      //gr->SetMarkerColor(GetMarkerColor());
      //gr->SetLineColor(GetLineColor());
      //gr->SetMarkerStyle(GetMarkerStyle());
      mg->Add(gr,"p");

   } // unique variable
   return mg;
}
//______________________________________________________________________________
