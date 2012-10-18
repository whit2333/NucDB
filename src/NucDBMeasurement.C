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

void  NucDBMeasurement::ListVariables(){
   if(fDataPoints.GetEntries()>0) {
      NucDBDataPoint * aPoint = (NucDBDataPoint*)fDataPoints.At(0);
      aPoint->ListVariables();
   }
}
//_____________________________________________________________________________

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
            fGraph->SetPoint(i,var->GetMean(),point->GetValue());
            fGraph->SetPointError(i,0.0,point->GetTotalError()->GetError());
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

//_____________________________________________________________________________

