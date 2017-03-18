#include "NucDBExperiment.h"

ClassImp(NucDBExperiment)
//_____________________________________________________________________________
NucDBExperiment::NucDBExperiment(const char * name ,const char * title) : TNamed(name,title) {
   fMeasurements.Clear();
}
//_____________________________________________________________________________

NucDBExperiment::~NucDBExperiment()= default;
//_____________________________________________________________________________

void NucDBExperiment::AddMeasurement(NucDBMeasurement* meas){
   if(meas)
   {
      meas->SetExperimentName( GetName() );
      fMeasurements.Add(meas);
   }
}
//_____________________________________________________________________________

NucDBMeasurement * NucDBExperiment::GetMeasurement(const char * name) {
   NucDBMeasurement * meas = nullptr;
   for(int i = 0;i<fMeasurements.GetEntries();i++) {
      if( !strcmp(((NucDBMeasurement *)fMeasurements.At(i))->GetName(),name) ) {
         meas = (NucDBMeasurement *)fMeasurements.At(i) ;
         break;
      }
   }
   //if(!meas) std::cout << " Measurement named " << name << " NOT FOUND!\n";
   return(meas);
}
//_____________________________________________________________________________

std::vector<NucDBMeasurement*> NucDBExperiment::GetMeasurements(NucDB::Process proc) {
   std::vector<NucDBMeasurement*> measList;
   for(int i = 0;i<fMeasurements.GetEntries();i++) {
      auto* meas = (NucDBMeasurement *)fMeasurements.At(i);
      if( meas->IsProcess(proc) ) {
         measList.push_back(meas);
      }
   }
   return(measList);
}
//_____________________________________________________________________________

void NucDBExperiment::PlotMeasurements(const char * var ){
   auto * c1 = new TCanvas( Form("%splots",GetName()),Form("%s plots",GetTitle() ) );
   c1->Divide(2,fMeasurements.GetEntries()/2);
   for(int i =0; i<fMeasurements.GetEntries();i++) {
      c1->cd(i+1);
      TGraph * gr = ((NucDBMeasurement*)fMeasurements.At(i))->BuildGraph(var);
      gr->Draw("ap");
   }
}
//_____________________________________________________________________________

void NucDBExperiment::Print(Option_t * opt) const {
   std::cout << "=============================================" << std::endl;
   std::cout << "   " << GetName() << " : " << GetTitle() << std::endl;;
   std::cout << "=============================================" << std::endl;;
   //std::cout << "  title = " << GetTitle() << "\n";
   //std::cout << "+++++ Measurements +++++\n";
   for(int i =0; i<fMeasurements.GetEntries();i++)
      ((NucDBMeasurement*)fMeasurements.At(i))->Print(opt);
}
//______________________________________________________________________________
void NucDBExperiment::PrintRefs(Option_t * opt ) {
   TList * ref = GetRefs();
   ref->Print();
}


//_____________________________________________________________________________
