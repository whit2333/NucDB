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

NucDBMeasurement::~NucDBMeasurement(){ }
//_____________________________________________________________________________

void NucDBMeasurement::ClearDataPoints(){
      fDataPoints.Clear();
      fNumberOfDataPoints=0;
}
//_____________________________________________________________________________

void NucDBMeasurement::AddDataPoint(NucDBDataPoint *point) {
      if(point) {
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

TGraphErrors * NucDBMeasurement::BuildGraph(const char * varName ) {
      if(fGraph) delete fGraph;
      fGraph=0;
      fGraph= new TGraphErrors(fNumberOfDataPoints);
      for(int i = 0; i < fNumberOfDataPoints;i++) {
         NucDBDataPoint * point = (NucDBDataPoint *) fDataPoints.At(i);
         NucDBBinnedVariable * var = point->GetBinVariable(varName);
         if(i==0) fGraph->GetXaxis()->SetTitle(varName);
         fGraph->SetPoint(i,var->GetMean(),point->GetValue());
         fGraph->SetPointError(i,0.0,point->GetTotalError()->GetError());
      }
      fGraph->SetTitle(GetTitle());
      fGraph->SetMarkerColor(fColor);
      fGraph->SetLineColor(fColor);
      fGraph->SetMarkerStyle(20);
      fGraph->SetLineStyle(1);
      fGraph->SetLineWidth(2);
      fGraph->GetXaxis()->SetTitle(varName);      
      return(fGraph);
   }
//_____________________________________________________________________________

//_____________________________________________________________________________

