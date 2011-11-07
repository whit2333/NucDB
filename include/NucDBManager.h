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


/** Database Manager 
 *
 */
class NucDBManager : public TObject {
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
//       TList* lst = fSQLServer->GetTablesList("%");
//       TIter next(lst);
//       TObject* obj;
//       Int_t tablecount =0;
//       while (obj = next()) {
//          tablecount++;
//          std::cout << " " << obj->GetName() << std::endl;
//       }
//       delete lst;
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

   static NucDBManager * fgDBManager;

   NucDBManager() {
      fSQLServer = TSQLServer::Connect("mysql://localhost/nuclearDB", "bjorken", "drell");
      if(!fSQLServer) std::cout << " xxxxxx FAILED TO CONNECT TO SERVER !!! \n";
      fFile = 0;
      fFile = new TFile("data/NucDB.root","UPDATE");
   }


ClassDef(NucDBManager,1)
};

#endif 
