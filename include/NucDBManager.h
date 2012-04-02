#ifndef NucDBManager_HH
#define NucDBManager_HH
#include "TObject.h"
#include "TSQLServer.h"
#include <iostream>
#include "TSQLResult.h"
#include "TSQLRow.h"
#include "TList.h"
#include "NucDBExperiments.h"
#include "TFile.h"
#include "NucDBUnits.h"
#include "TKey.h"
#include "TDirectory.h"
#include <set>
#include <string>
#include "NucDBPaper.h"


/** NucDB Database Manager 
 *
 */
class NucDBManager : public TObject {
protected:
   /** If opt is not zero it is opened in UPDATE mode, otherwise it is readonly.
    */ 
   NucDBManager(Int_t opt = 0) {
      fStandardUnits.Clear();
      fStandardUnits.Add(new NucDBEnergyUnit());
      fStandardUnits.Add(new NucDBMomentumUnit());
      fStandardUnits.Add(new NucDBMassUnit());
      fStandardUnits.Add(new NucDBXSectionUnit());
//      fSQLServer = TSQLServer::Connect("mysql://quarks.temple.edu/nuclearDB", "bjorken", "drell");
//      if(!fSQLServer) std::cout << " xxxxxx FAILED TO CONNECT TO SERVER !!! \n";
      fFile = 0;
      if(!opt) fFile = new TFile(Form("%s/NucDB.root",NUCDB_DATA_DIR),"READ");
      else fFile = new TFile(Form("%s/NucDB.root",NUCDB_DATA_DIR),"UPDATE");
      fExperiments = 0;
      fMeasurements = 0;

      fFile->cd();
      fExpDir=0;
      fFile->GetObject("Experiments",fExpDir);
      //if(!fExpDir) fExpDir = new TDirectory("Experiments","Experiments");
      if(!fExpDir) fExpDir = fFile->mkdir("Experiments","Experiments");

      fFile->cd();
      fCalcDir=0;
      fFile->GetObject("Calculations",fCalcDir);
      //if(!fCalcDir) fCalcDir = new TDirectory("Calculations","Calculations");
      if(!fCalcDir) fCalcDir = fFile->mkdir("Calculations","Calculations");

      fFile->cd();
      fPapersDir=0;
      fFile->GetObject("Papers",fPapersDir);
      //if(!fPapersDir) fPapersDir = new TDirectory("Papers","Papers");
      if(!fPapersDir) fPapersDir = fFile->mkdir("Papers","Papers");

   }

public :
   static NucDBManager * GetManager(Int_t opt = 0){
      if(!fgDBManager) fgDBManager = new NucDBManager(opt);
      return(fgDBManager);
   }

   ~NucDBManager(){
      fFile->Flush();
      fFile->Write();
      fFile->Close();
      fgDBManager=0;
   }

   NucDBExperiment * GetExperiment(const char * expName) {
      if(!fFile){ printf(" NO FILE OPENED!!! \n"); return(0); }
      NucDBExperiment * exp = 0;
      fExpDir->GetObject(Form("%s",expName),exp);
      if(!exp){
         printf(" Experiment Not found\n");
         exp = 0;
      }
      return(exp);
   }

   NucDBPaper * GetPaper(const char * name) {
      if(!fFile){ printf(" NO FILE OPENED!!! \n"); return(0); }
      NucDBPaper * p = 0;
      fPapersDir->GetObject(Form("%s",name),p);
      if(!p){
         printf(" Paper Not found\n");
         p = 0;
      }
      return(p);
   }

   /** Returns the list of experiments, calculations AND Papers.
    */
   TList * GetExperiments() {
//      TList * list = new TList();
//      list->Clear();
//      const char *sql = "select exp_name from experiments ORDER BY exp_name DESC"
                       /* "WHERE "*/;
//      res = fSQLServer->Query(sql);
//      int nrows = res->GetRowCount();
//      int nfields = res->GetFieldCount();
//      for (int i = 0; i < nrows; i++) {
//         row = res->Next();
//         NucDBExperiment * anExp = GetExperiment(row->GetField(0));
//         if(anExp) list->Add(anExp);
//         delete row;
//      }
//      delete res;
//      return(list);
      if(!fFile) {
         Error("GetExperiments()"," File Not open!");
	 return(0);
      } else {
         NucDBExperiment * anExp = 0;
	 if(!fExperiments) fExperiments = new TList();     
	 fExperiments->Clear();
	 TList * keys = fExpDir->GetListOfKeys();     
         for(int i=0;i<keys->GetEntries();i++){
//	    std::cout << ((TKey*)keys->At(i))->GetClassName() << "\n";
//	    std::cout << ((TKey*)keys->At(i))->GetName() << "\n";
	    if( !strcmp(  ((TKey*)keys->At(i))->GetClassName(),"NucDBExperiment") ) {
	       fExpDir->GetObject( ((TKey*)keys->At(i))->GetName(),anExp ) ;
//	       std::cout << anExp << "\n";
	       fExperiments->Add(anExp);
	    }
	 }
	 return(fExperiments);
      }
   }

   /** Get a list of measurments with that name, (eg, F2p,g1p ).
    */
   TList * GetMeasurements(const char * measurement) {
      TList * expList = GetExperiments(); 
      TList * measList = new TList();
      measList->Clear();
      for(int i = 0;i<expList->GetEntries();i++) {
         NucDBMeasurement * aMeas = ((NucDBExperiment*)expList->At(i))->GetMeasurement(measurement);
         if(aMeas) measList->Add(aMeas);
      }
      return(measList);
   }

   /** Saves data in root file: data/NucDB.root. 
    */
   void SaveExperiment(NucDBExperiment * exp) {
      if(!fFile){ printf(" NO FILE OPENED!!! \n"); }
      else {
         if(exp) {
            fExpDir->cd();
            exp->Write(exp->GetName(), kWriteDelete);
         }
         else printf(" NULL pointer\n");
      }
   }

   /** Saves data in root file: data/NucDB.root. 
    */
   void SavePaper(NucDBPaper * p) {
      if(!fFile){ printf(" NO FILE OPENED!!! \n"); }
      else {
         if(p) {
            fPapersDir->cd();
            p->Write(p->GetName(), kWriteDelete);
         }
         else printf(" NULL pointer\n");
      }
   }



   TFile * fFile;

   /** Prints the experiments which are available in the database. 
    *  
    *  Returns the total number of experiments
    */
   Int_t ListExperiments() {
      TList * exps = GetExperiments();
      exps->SetOwner(false);
      exps->Print();
      Int_t n = exps->GetEntries();
      delete exps;
      return(n);
//      const char *sql = "select exp_name from experiments "
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
   }

   /** Prints the measurements which are stored in the database. 
    *  
    *  A measurement would be, for example, F1p, g2P, F2C/F2D,...
    *  
    *  Returns the total number of measurements
    */
   Int_t ListMeasurements() {
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

   //TSQLServer * GetServer(){return(fSQLServer);}

private:
   TList * fExperiments;
   TList * fMeasurements;

protected :
//   TSQLServer * fSQLServer;
//   TSQLRow *row;
//   TSQLResult *res;

   TDirectory * fExpDir;
   TDirectory * fPapersDir;
   TDirectory * fCalcDir;

   TList fStandardUnits;

   static NucDBManager * fgDBManager;


ClassDef(NucDBManager,1)
};

#endif 
