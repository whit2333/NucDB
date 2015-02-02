#include "NucDBManager.h"
#include <iomanip>
#include <string>       // std::string
#include <iostream>     // std::cout
#include <sstream>      // std::stringstream
#include "TColor.h"
#include "TROOT.h"
#include "TGraphAsymmErrors.h"

ClassImp(NucDBManager)
//_____________________________________________________________
// A description of the class starts with the line above, and
// will take place here !
//

NucDBManager * NucDBManager::fgDBManager = 0;
//_____________________________________________________________________________

NucDBManager::NucDBManager(Int_t opt) {
   fDatabase = 0;
   fVerbosity = 2;

   fStandardUnits.Clear();
   fStandardUnits.Add(new NucDBEnergyUnit());
   fStandardUnits.Add(new NucDBMomentumUnit());
   fStandardUnits.Add(new NucDBMassUnit());
   fStandardUnits.Add(new NucDBXSectionUnit());

   fFile = 0;
   if(!opt) fFile = new TFile(Form("%s/NucDB.root",NucDB_DATA_DIR),"READ");
   else fFile = new TFile(Form("%s/NucDB.root",NucDB_DATA_DIR),"UPDATE");

   fFile->cd();

   Load();

}
//_____________________________________________________________________________

NucDBManager::~NucDBManager(){
      fFile->Write();
      fFile->Close();
      fgDBManager=0;
}
//_____________________________________________________________________________

Int_t NucDBManager::Load(const char * n){
      fDatabase = dynamic_cast<NucDBDatabase*>(gROOT->FindObject(n));
      if(!fDatabase) {
         fDatabase = new NucDBDatabase();
         std::cout << " Created new Database \n";
      }
      fExperiments = fDatabase->GetExperiments();
      fMeasurements = fDatabase->GetMeasurements();
      fPapers = fDatabase->GetPapers();
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
      if(!fFile){ Error("GetExperiment"," NO FILE OPENED!!!"); return(0); }
      NucDBExperiment * exp = 0;
      exp =  dynamic_cast<NucDBExperiment*>(fExperiments->FindObject(Form("%s",expName)));
      if(!exp){
         //printf(" Experiment Not found\n");
         exp = 0;
      }
      return(exp);
   }
//_____________________________________________________________________________

NucDBPaper * NucDBManager::GetPaper(const char * name) {
   if(!fFile){ printf(" NO FILE OPENED!!! \n"); return(0); }
   NucDBPaper * p = 0;
   p =  dynamic_cast<NucDBPaper*>(fPapers->FindObject(Form(Form("%s",name),p)));
   if(!p){
      printf(" Paper Not found\n");
      p = 0;
   }
      return(p);
}
//_____________________________________________________________________________

TList * NucDBManager::GetMeasurements(const char * measurement) {
      TList * expList = GetExperiments(); 
      TList * measList = new TList();
      measList->Clear();
      for(int i = 0;i<expList->GetEntries();i++) {
         NucDBMeasurement * aMeas = ((NucDBExperiment*)expList->At(i))->GetMeasurement(measurement);
         if(aMeas) measList->Add(aMeas);
      }
      return(measList);
   }
//_____________________________________________________________________________

TMultiGraph *      NucDBManager::GetMultiGraph(const char * measurement, const char * var){
   TMultiGraph * mg = new TMultiGraph();
   TList * meas = GetMeasurements(measurement);
   for(int i = 0 ; i < meas->GetEntries(); i++) {
      NucDBMeasurement * am = (NucDBMeasurement*)meas->At(i);
      TGraphErrors * gr = am->BuildGraph(var);
      mg->Add(gr,"p");
   }
   return(mg);
}
//_____________________________________________________________________________
TMultiGraph *      NucDBManager::GetKinematicMultiGraph(const char * measurement, const char * var1 ,const char * var2 ){
   TMultiGraph * mg   = new TMultiGraph();
   TList       * meas = GetMeasurements(measurement);
   for(int i = 0 ; i < meas->GetEntries(); i++) {
      NucDBMeasurement * am = (NucDBMeasurement*)meas->At(i);
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
      fPapers->Add(p);
      SaveDatabase();
}
//_____________________________________________________________________________

void NucDBManager::SaveExperiment(NucDBExperiment * exp) {
      if(!fFile){ printf(" NO FILE OPENED!!! \n"); }
      // Check to see if experiment already exists
      NucDBExperiment * anexp = (NucDBExperiment*)fExperiments->FindObject(exp->GetName());
      // if it does not, then add it.
      if(exp != anexp) fExperiments->Add(exp);
      AddNewMeasurements(exp);
      SaveDatabase();

}
//_____________________________________________________________________________

void   NucDBManager::AddNewMeasurements(NucDBExperiment * exp){
   TList * ms = exp->GetMeasurements();
   TObjString * mname = 0;
   for (int i = 0; i < ms->GetEntries(); i++) {
      NucDBMeasurement * ameas = (NucDBMeasurement *) ms->At(i);
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
	 return(0);
      } else if( !fDatabase ){
         Error("GetExperiments()"," no database ");
	 return(0);
      } else if( !fExperiments ){
         fExperiments = fDatabase->GetExperiments();
      }
      return(fExperiments);

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

Int_t NucDBManager::ListMeasurementsByExperiment(const char * measurement)  {
   // No argument
   if( !strcmp("",measurement) ) {
      TList * exps = GetExperiments();
      for(int i = 0; i< exps->GetEntries();i++){
         NucDBExperiment * exp  = (NucDBExperiment*)exps->At(i);
         std::cout << " - " << exp->GetName() << "\n";
         TList * meas = exp->GetMeasurements();
         for(int j = 0; j<  meas->GetEntries();j++){
            NucDBMeasurement * am = (NucDBMeasurement*)(meas->At(j));
            std::cout << "   - " << am->GetName() << "\n";
         }
      }
   } else {
      TList * exps = GetExperiments();
      for(int i = 0; i< exps->GetEntries();i++){
         NucDBExperiment * exp  = (NucDBExperiment*)exps->At(i);
         if(exp->GetMeasurement(measurement) ) {
            std::cout << " - " << exp->GetName() << "\n";
            TList * meas = exp->GetMeasurements();
            for(int j = 0; j<  meas->GetEntries();j++){
               NucDBMeasurement * am = (NucDBMeasurement*)(meas->At(j));
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
      NucDBExperiment * anExp = (NucDBExperiment*)exps->At(i);
      TList * expMeas = anExp->GetMeasurements();
      for(int j=0;j<expMeas->GetEntries();j++) {
         NucDBMeasurement * aMeas = (NucDBMeasurement*)expMeas->At(j);
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
      TList * refs = new TList();
      TList * exps = GetExperiments();
      for(int i=0;i<exps->GetEntries();i++) {
         NucDBExperiment * anExp = (NucDBExperiment*)exps->At(i);
         refs->AddAll(anExp->GetRefs());
      }
      refs->Print();
      return( refs );
}
//_____________________________________________________________________________

