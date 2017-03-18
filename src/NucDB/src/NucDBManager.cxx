#include "NucDBManager.h"
#include <iomanip>
#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream>      // std::stringstream
#include "TColor.h"
#include "TROOT.h"
#include "TGraphAsymmErrors.h"


NucDBManager * NucDBManager::fgDBManager = nullptr;
//_____________________________________________________________________________

NucDBManager::NucDBManager(const char * file, Int_t opt) : fFileName(file)
{
   fDatabase  = nullptr;
   fExperiments  = nullptr;
   fPapers       = nullptr;
   fMeasurements = nullptr;
   fCalculations = nullptr;
   fVerbosity = 2;

   fi_color  = 0;
   fi_marker = 0;

   fColors.push_back(1);  // black
   fColors.push_back(2);  // red
   fColors.push_back(4);  // blue
   fColors.push_back(6);  // magenta
   fColors.push_back(9);  // purple
   fColors.push_back(8);  // green
   fColors.push_back(34); // gray-blue

   fMarkers.push_back(20);
   fMarkers.push_back(21);
   fMarkers.push_back(22);
   fMarkers.push_back(23);
   fMarkers.push_back(33);
   fMarkers.push_back(34);

   fStandardUnits.Clear();
   fStandardUnits.Add(new NucDBEnergyUnit());
   fStandardUnits.Add(new NucDBMomentumUnit());
   fStandardUnits.Add(new NucDBMassUnit());
   fStandardUnits.Add(new NucDBXSectionUnit());

   if( fFileName.empty() ) {
      if(!opt){
         fFile = new TFile(Form("%s/NucDB.root",NucDB_DATA_DIR),"READ");
      } else  {
         fFile = new TFile(Form("%s/NucDB.root",NucDB_DATA_DIR),"UPDATE");
      }
   } else {
      if(!opt){
         fFile = new TFile(fFileName.c_str(),"READ");
      } else  {
         fFile = new TFile(fFileName.c_str(),"UPDATE");
      }
   }

   fFile->cd();

   Load();
}
//______________________________________________________________________________

NucDBManager::~NucDBManager(){
      fFile->Write();
      fFile->Close();
      fgDBManager=nullptr;
}
//_____________________________________________________________________________

Int_t NucDBManager::Load(const char * n){
      fDatabase = dynamic_cast<NucDBDatabase*>(gROOT->FindObject(n));
      if(!fDatabase) {
         fDatabase = new NucDBDatabase();
         std::cout << " Created new Database \n";
      }
      fExperiments  = fDatabase->GetExperiments();
      fMeasurements = fDatabase->GetMeasurements();
      fPapers       = fDatabase->GetPapers();
      return(0);
   }
//_____________________________________________________________________________

void NucDBManager::SaveDatabase(){
      if(!fDatabase) {Error("SaveDatabase","Null fDatabase"); }
      else if ( fFile )  {
         fFile->cd();
         fDatabase->Write(fDatabase->GetName(), kWriteDelete);
      }
   }
//_____________________________________________________________________________

NucDBExperiment * NucDBManager::GetExperiment(const char * expName) {
      if(!fFile){ Error("GetExperiment"," NO FILE OPENED!!!"); return(nullptr); }
      NucDBExperiment * exp = nullptr;
      exp =  dynamic_cast<NucDBExperiment*>(fExperiments->FindObject(Form("%s",expName)));
      if(!exp){
         //printf(" Experiment Not found\n");
         exp = nullptr;
      }
      return(exp);
   }
//_____________________________________________________________________________

NucDBPaper * NucDBManager::GetPaper(const char * name) {
   if(!fFile){ printf(" NO FILE OPENED!!! \n"); return(nullptr); }
   NucDBPaper * p = nullptr;
   p =  dynamic_cast<NucDBPaper*>(fPapers->FindObject(Form(Form("%s",name),p)));
   if(!p){
      printf(" Paper Not found\n");
      p = nullptr;
   }
      return(p);
}
//_____________________________________________________________________________

TList * NucDBManager::GetMeasurements(const char * measurement)
{
      TList * expList = GetExperiments(); 
      auto * measList = new TList();
      measList->Clear();
      for(int i = 0;i<expList->GetEntries();i++) {
         NucDBMeasurement * aMeas = ((NucDBExperiment*)expList->At(i))->GetMeasurement(measurement);
         if(aMeas) measList->Add(aMeas);
      }
      return(measList);
}
//_____________________________________________________________________________

std::vector<NucDBMeasurement*> NucDBManager::GetMeasurements(NucDB::Process proc)
{
   TList * expList = GetExperiments(); 
   std::vector<NucDBMeasurement*> measList;
   for(int i = 0;i<expList->GetEntries();i++) {
      auto exp = ((NucDBExperiment*)expList->At(i));
      std::vector<NucDBMeasurement*> meass = exp->GetMeasurements(proc);
      for(auto m: meass){
         measList.push_back(m);
      }
   }
   return(measList);
}
//_____________________________________________________________________________

std::vector<NucDBMeasurement*>  NucDBManager::GetAllMeasurements(const char * measurement)
{
   std::vector<NucDBMeasurement*> measurements;
   TList * expList = GetExperiments(); 
   //TList * measList = new TList();
   //measList->Clear();
   for(int i = 0;i<expList->GetEntries();i++) {
      NucDBMeasurement * aMeas = ((NucDBExperiment*)expList->At(i))->GetMeasurement(measurement);
      if(aMeas) measurements.push_back(aMeas);
   }
   return(measurements);
}
//______________________________________________________________________________

      TList * NucDBManager::GetMeasurementCalculations(const char * measurement) {
   TList * papList = GetPapers(); 
   auto * measList = new TList();
   measList->Clear();
   for(int i = 0; i<papList->GetEntries(); i++) {
      auto * aPaper = (NucDBPaper*)papList->At(i);
      TList * calcs = aPaper->GetCalculations();
      for(int j = 0; j<calcs->GetEntries(); j++) {
         auto * aCalc = (NucDBCalculation*)calcs->At(j);
         NucDBMeasurement * aMeas = aCalc->GetMeasurement(measurement);
         if(aMeas) measList->Add(aMeas);
      }
   }
   return(measList);
}
//_____________________________________________________________________________

TMultiGraph *      NucDBManager::GetMultiGraph(const char * measurement, const char * var){
   auto * mg = new TMultiGraph();
   TList * meas = GetMeasurements(measurement);
   for(int i = 0 ; i < meas->GetEntries(); i++) {
      auto * am = (NucDBMeasurement*)meas->At(i);
      TGraphErrors * gr = am->BuildGraph(var);
      mg->Add(gr,"p");
   }
   return(mg);
}
//_____________________________________________________________________________
TMultiGraph *      NucDBManager::GetKinematicMultiGraph(const char * measurement, const char * var1 ,const char * var2 ){
   auto * mg   = new TMultiGraph();
   TList       * meas = GetMeasurements(measurement);
   for(int i = 0 ; i < meas->GetEntries(); i++) {
      auto * am = (NucDBMeasurement*)meas->At(i);
      TGraph * gr = am->BuildKinematicGraph(var1,var2);
      gr->SetFillColorAlpha(kRed,0.3);
      //gROOT->GetColor(40+i)->SetAlpha(1.0);
      //gr->SetFillColor(40+i);
      mg->Add(gr,"pe2");
   }
   return(mg);
}
//_____________________________________________________________________________
void NucDBManager::SavePaper(NucDBPaper * p) {
   if(!fFile){ Error("SavePaper"," NO FILE OPENED!!! "); }
      fPapers->Remove(p);
      fPapers->Add(p);
      SaveDatabase();
}
//_____________________________________________________________________________

void NucDBManager::SaveExperiment(NucDBExperiment * exp) {
      if(!fFile){ printf(" NO FILE OPENED!!! \n"); }
      // Check to see if experiment already exists
      auto * anexp = (NucDBExperiment*)fExperiments->FindObject(exp->GetName());
      // if it does not, then add it.
      if(exp != anexp) fExperiments->Add(exp);
      AddNewMeasurements(exp);
      SaveDatabase();

}
//_____________________________________________________________________________

void   NucDBManager::AddNewMeasurements(NucDBExperiment * exp){
   TList * ms = exp->GetMeasurements();
   TObjString * mname = nullptr;
   for (int i = 0; i < ms->GetEntries(); i++) {
      auto * ameas = (NucDBMeasurement *) ms->At(i);
      // find objstring if it exists
      mname = (TObjString*) fMeasurements->FindObject(ameas->GetName());
      // if there is no measurment by that name, at new objstring
      if(!mname) fMeasurements->Add(new TObjString(ameas->GetName()));
   }
}
//_____________________________________________________________________________
TList * NucDBManager::GetExperiments() {
   if(!fFile) {
      Error("GetExperiments()"," File Not open!");
      return(nullptr);
   } else if( !fDatabase ){
      Error("GetExperiments()"," no database ");
      return(nullptr);
   } else if( !fExperiments ){
      fExperiments = fDatabase->GetExperiments();
   }
   return(fExperiments);

}
//_____________________________________________________________________________
TList * NucDBManager::GetPapers() {
   if(!fFile) {
      Error("GetPapers()"," File Not open!");
      return(nullptr);
   } else if( !fDatabase ){
      Error("GetPapers()"," no database ");
      return(nullptr);
   } else if( !fPapers ){
      fPapers = fDatabase->GetPapers();
   }
   return(fPapers);

}
//_____________________________________________________________________________
Int_t NucDBManager::ListExperiments() {
   TList * exps = GetExperiments();
   exps->SetOwner(false);
   exps->Print();
   Int_t n = exps->GetEntries();
   //delete exps;
   return(n);
}
//_____________________________________________________________________________
Int_t NucDBManager::ListPapers() {
   TList * papers = GetPapers();
   papers->SetOwner(false);
   papers->Print();
   Int_t n = papers->GetEntries();
   //delete exps;
   return(n);
}
//_____________________________________________________________________________
Int_t NucDBManager::ListMeasurementsByExperiment(const char * measurement)  {
   // No argument
   if( !strcmp("",measurement) ) {
      TList * exps = GetExperiments();
      for(int i = 0; i< exps->GetEntries();i++){
         auto * exp  = (NucDBExperiment*)exps->At(i);
         std::cout << " - " << exp->GetName() << "\n";
         TList * meas = exp->GetMeasurements();
         for(int j = 0; j<  meas->GetEntries();j++){
            auto * am = (NucDBMeasurement*)(meas->At(j));
            std::cout << "   - " << am->GetName() << "\n";
         }
      }
   } else {
      TList * exps = GetExperiments();
      for(int i = 0; i< exps->GetEntries();i++){
         auto * exp  = (NucDBExperiment*)exps->At(i);
         if(exp->GetMeasurement(measurement) ) {
            std::cout << " - " << exp->GetName() << "\n";
            TList * meas = exp->GetMeasurements();
            for(int j = 0; j<  meas->GetEntries();j++){
               auto * am = (NucDBMeasurement*)(meas->At(j));
               std::cout << "   - " << am->GetName() << "\n";
            }
         }
      }
   }
   return(0);
}
//_____________________________________________________________________________
Int_t NucDBManager::ListMeasurements(const char * n ) {
   TList * exps = GetExperiments();

   std::set<std::string>                           measurements;
   std::set<std::string>::iterator                 it;
   std::pair<std::set<std::string>::iterator,bool> ret;

   for(int i=0;i<exps->GetEntries();i++) {
      auto * anExp = (NucDBExperiment*)exps->At(i);
      TList * expMeas = anExp->GetMeasurements();
      for(int j=0;j<expMeas->GetEntries();j++) {
         auto * aMeas = (NucDBMeasurement*)expMeas->At(j);
         TString aName            = aMeas->GetName();
         TString aTitle           = aMeas->GetTitle();
         std::stringstream ss;
         ss << " " << std::setw(15) << aName.Data() << "\t"  << aTitle.Data() ;
         if(n) {
            if(aName.BeginsWith(n)) {
               ret = measurements.insert(ss.str());
            }
         } else {
            ret    = measurements.insert(ss.str());
         }
      }
   }
   for (it=measurements.begin(); it!=measurements.end(); it++) {
      std::cout << " " << *it << std::endl;//"\t"  << *it_title << std::endl;
   }
   return(0);  
}
//_____________________________________________________________________________

TList * NucDBManager::GetRefs() {
      auto * refs = new TList();
      TList * exps = GetExperiments();
      for(int i=0;i<exps->GetEntries();i++) {
         auto * anExp = (NucDBExperiment*)exps->At(i);
         refs->AddAll(anExp->GetRefs());
      }
      refs->Print();
      return( refs );
}
//_____________________________________________________________________________
Int_t NucDBManager::ListCalculations(const char * n ) {
   TList * exps = GetPapers();
   if(!exps) return -1;

   std::set<std::string>                           Calculations;
   std::set<std::string>::iterator                 it;
   std::pair<std::set<std::string>::iterator,bool> ret;

   for(int i=0;i<exps->GetEntries();i++) {
      auto * anExp = (NucDBPaper*)exps->At(i);
      TList * expMeas = anExp->GetCalculations();
      for(int j=0;j<expMeas->GetEntries();j++) {
         auto * aCalc = (NucDBCalculation*)expMeas->At(j);
         TString aName            = aCalc->GetName();
         TString aTitle           = aCalc->GetTitle();
         std::stringstream ss;
         ss << " " << std::setw(15) << aName.Data() << "\t"  << aTitle.Data() ;
         if(n) {
            if(aName.BeginsWith(n)) {
               ret = Calculations.insert(ss.str());
            }
         } else {
            ret    = Calculations.insert(ss.str());
         }
      }
   }
   for (it=Calculations.begin(); it!=Calculations.end(); it++) {
      std::cout << " " << *it << std::endl;//"\t"  << *it_title << std::endl;
   }
   return(0);  
}
//_____________________________________________________________________________

