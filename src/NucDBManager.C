#include "NucDBManager.h"


ClassImp(NucDBManager)

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
   if(!opt) fFile = new TFile(Form("%s/NucDB.root",NUCDB_DATA_DIR),"READ");
   else fFile = new TFile(Form("%s/NucDB.root",NUCDB_DATA_DIR),"UPDATE");

   fFile->cd();

   Load();
   


//    fFile->GetObject("Experiments",fExpDir);
//    if(!fExpDir) fExpDir = fFile->mkdir("Experiments","Experiments");
// 
//       fFile->cd();
//       fCalcDir=0;
//       fFile->GetObject("Calculations",fCalcDir);
//       //if(!fCalcDir) fCalcDir = new TDirectory("Calculations","Calculations");
//       if(!fCalcDir) fCalcDir = fFile->mkdir("Calculations","Calculations");
// 
//       fFile->cd();
//       fPapersDir=0;
//       fFile->GetObject("Papers",fPapersDir);
//       //if(!fPapersDir) fPapersDir = new TDirectory("Papers","Papers");
//       if(!fPapersDir) fPapersDir = fFile->mkdir("Papers","Papers");

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
         printf(" Experiment Not found\n");
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

   void NucDBManager::SavePaper(NucDBPaper * p) {
      if(!fFile){ Error("SavePaper"," NO FILE OPENED!!! "); }
      fPapers->Add(p);
      SaveDatabase();
//       else {
//          if(p) {
//             fPapersDir->cd();
//             p->Write(p->GetName(), kWriteDelete);
//          }
//          else printf(" NULL pointer\n");
//       }
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

//       else {
//          if(exp) {
//             fExpDir->cd();
//             exp->Write(exp->GetName(), kWriteDelete);
//          }
//          else printf(" NULL pointer\n");
//       }
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
//          NucDBExperiment * anExp = 0;
// 	 fExperiments->Clear();
// 	 TList * keys = fExpDir->GetListOfKeys();     
//          for(int i=0;i<keys->GetEntries();i++){
// 	    if( !strcmp(  ((TKey*)keys->At(i))->GetClassName(),"NucDBExperiment") ) {
// 	       fExpDir->GetObject( ((TKey*)keys->At(i))->GetName(),anExp ) ;
// //	       std::cout << anExp << "\n";
// 	       fExperiments.Add(anExp);
// 	    }
// 	 }
// 	 return(fExperiments);
//       }
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

Int_t NucDBManager::ListMeasurementsByExperiment() {
   fDatabase->ListMeasurementsByExperiment();
   return(0);   
}
//_____________________________________________________________________________

Int_t NucDBManager::ListMeasurements() {
      TList * exps = GetExperiments();
      std::set<std::string> measurements;
      std::set<std::string>::iterator it;

      std::pair<std::set<std::string>::iterator,bool> ret;
      for(int i=0;i<exps->GetEntries();i++) {
         NucDBExperiment * anExp = (NucDBExperiment*)exps->At(i);
	 TList * expMeas = anExp->GetMeasurements();
         for(int j=0;j<expMeas->GetEntries();j++) {
            NucDBMeasurement * aMeas = (NucDBMeasurement*)expMeas->At(j);
	    ret = measurements.insert(aMeas->GetName());
	 }
      }
      for (it=measurements.begin(); it!=measurements.end(); it++)
         std::cout << " " << *it << "\n";
//      const char *sql = "select quantity from measurements "
//                       /* "WHERE "*/;
//      res = fSQLServer->Query(sql);
//      int nrows = res->GetRowCount();
/*      printf("\nGot %d rows in result\n", nrows);*/
//      int nfields = res->GetFieldCount();
//      for (int i = 0; i < nfields; i++)
//         printf("%40s", res->GetFieldName(i));
//      printf("\n");
//      for (int i = 0; i < nfields*40; i++)
//         printf("=");
//      printf("\n");
//      for (int i = 0; i < nrows; i++) {
//         row = res->Next();
//         for (int j = 0; j < nfields; j++) {
//            printf("%40s", row->GetField(j));
//         }
//         printf("\n");
//         delete row;
//      }
//      delete res;
//      printf("\n");
//      return(nrows);
      return(0);  
   }
