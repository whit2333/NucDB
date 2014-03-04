#include "NucDBMeasurement.h"

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
void NucDBMeasurement::SortBy(const char * n){
   for(int i = 0; i < fDataPoints.GetEntries();i++) {
      NucDBDataPoint * point = (NucDBDataPoint*)fDataPoints.At(i);
      point->SetSortingVariable(n);
   }
   fDataPoints.Sort();
}
//_____________________________________________________________________________
TList *  NucDBMeasurement::MergeDataPoints(unsigned int n){
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
   return list;
}
//_____________________________________________________________________________
NucDBMeasurement * NucDBMeasurement::CreateMeasurementFilteredWithBin(NucDBBinnedVariable const * bin) {
   NucDBMeasurement * m = new NucDBMeasurement(Form("%s_%s",this->GetName(),bin->GetName()),
         Form("%s %s",this->GetTitle(),bin->GetTitle()) );
   m->AddDataPoints(this->FilterWithBin(bin));
   return m;
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
void NucDBMeasurement::Print() const {
   std::cout << "   --------------------------\n";
   std::cout << "   " << GetName() << "\n";
   std::cout << "   --------------------------\n";
   std::cout << "  title = " << GetTitle() << "\n";
   std::cout << "  fNumberOfDataPoints = " << fNumberOfDataPoints <<  "\n";
}
//_____________________________________________________________________________

void NucDBMeasurement::PrintData() const {
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
NucDBBinnedVariable* NucDBMeasurement::GetDependentVariable(const char * name) {
   for(int i = 0;i<fDependentVariables.GetEntries();i++) {
      if( !strcmp( ((NucDBBinnedVariable*)fDependentVariables.At(i))->GetName(),name) ) 
         return((NucDBBinnedVariable*)fDependentVariables.At(i));
   }
   return(0);
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
void NucDBMeasurement::GetUniqueBinnedVariableValues(const char * name,std::vector<double> * vect){
   if(!vect) return;
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
            vect->push_back(vmean);
            N++;
         }
      }
   }
   std::sort(vect->begin(),vect->end());
   Int_t n1 = vect->size();
   vect->erase(std::unique(vect->begin(),vect->end()),vect->end());
   Int_t n2 = vect->size();
   //std::cout << " out of " << n1 << " values, " << n2 << " are unique\n";
   //for(int i = 0; i< vect->size() ; i++) {
   //   std::cout << name << "[" << i << "] = " << vect->at(i) << "\n";
   //}
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

