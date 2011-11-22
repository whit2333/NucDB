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

/** Database Manager 
 *
 */
class NucDBManager : public TObject {
protected:
   NucDBManager() {
      fStandardUnits.Clear();
      fStandardUnits.Add(new NucDBEnergyUnit());
      fStandardUnits.Add(new NucDBMomentumUnit());
      fStandardUnits.Add(new NucDBMassUnit());
      fStandardUnits.Add(new NucDBXSectionUnit());
   
      fSQLServer = TSQLServer::Connect("mysql://quarks.temple.edu/nuclearDB", "bjorken", "drell");
      if(!fSQLServer) std::cout << " xxxxxx FAILED TO CONNECT TO SERVER !!! \n";
      fFile = 0;
      fFile = new TFile("data/NucDB.root","UPDATE");
   }


public :
   static NucDBManager * GetManager(){
      if(!fgDBManager) fgDBManager = new NucDBManager();
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
      fFile->GetObject(Form("%s",expName),exp);
      if(!exp){
         printf(" Experiment Not found\n");
         exp = 0;
      }
      return(exp);
   }

   /** returns the list of experiments */
   TList * GetExperiments() {
      TList * list = new TList();
      list->Clear();
      const char *sql = "select exp_name from experiments ORDER BY exp_name DESC"
                       /* "WHERE "*/;
      res = fSQLServer->Query(sql);
      int nrows = res->GetRowCount();
      int nfields = res->GetFieldCount();
      for (int i = 0; i < nrows; i++) {
         row = res->Next();
         NucDBExperiment * anExp = GetExperiment(row->GetField(0));
         if(anExp) list->Add(anExp);
         delete row;
      }
      delete res;
      return(list);
   }

   /** Get a list of measurments with that name, (eg, F2p,g1p ) */
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

   /** Saves data in root file: data/NucDB.root */
   void SaveExperiment(NucDBExperiment * exp) {
      if(!fFile){ printf(" NO FILE OPENED!!! \n"); }
      else {
         if(exp) {
            fFile->cd();
/*            exp->Write(GetName(), kWriteDelete);*/
            exp->Write(exp->GetName(), kWriteDelete);
/*            exp->Write(exp->GetName());*/
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
      const char *sql = "select exp_name from experiments "
                       /* "WHERE "*/;
      res = fSQLServer->Query(sql);
      int nrows = res->GetRowCount();
/*      printf("\nGot %d rows in result\n", nrows);*/
      int nfields = res->GetFieldCount();
      for (int i = 0; i < nfields; i++)
         printf("%40s", res->GetFieldName(i));
      printf("\n");
      for (int i = 0; i < nfields*40; i++)
         printf("=");
      printf("\n");
      for (int i = 0; i < nrows; i++) {
         row = res->Next();
         for (int j = 0; j < nfields; j++) {
            printf("%40s", row->GetField(j));
         }
         printf("\n");
         delete row;
      }
      delete res;
      printf("\n");
      return(nrows);
   }

   /** Prints the measurements which are stored in the database. 
    *  
    *  A measurement would be, for example, F1p, g2P, F2C/F2D,...
    *  
    *  Returns the total number of measurements
    */
   Int_t ListMeasurements() {
      const char *sql = "select quantity from measurements "
                       /* "WHERE "*/;
      res = fSQLServer->Query(sql);
      int nrows = res->GetRowCount();
/*      printf("\nGot %d rows in result\n", nrows);*/
      int nfields = res->GetFieldCount();
      for (int i = 0; i < nfields; i++)
         printf("%40s", res->GetFieldName(i));
      printf("\n");
      for (int i = 0; i < nfields*40; i++)
         printf("=");
      printf("\n");
      for (int i = 0; i < nrows; i++) {
         row = res->Next();
         for (int j = 0; j < nfields; j++) {
            printf("%40s", row->GetField(j));
         }
         printf("\n");
         delete row;
      }
      delete res;
      printf("\n");
      return(nrows);
   }

   TSQLServer * GetServer(){return(fSQLServer);}



protected :
   TSQLServer * fSQLServer;
   TSQLRow *row;
   TSQLResult *res;

   TList fStandardUnits;

   static NucDBManager * fgDBManager;


ClassDef(NucDBManager,1)
};

#endif 
