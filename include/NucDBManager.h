#ifndef NucDBManager_HH
#define NucDBManager_HH
#include "TObject.h"
#include "TSQLServer.h"
#include <iostream>

class NucDBManager : public TObject {
public :
   static NucDBManager * GetManager(){
      if(!fgDBManager) fgDBManager = new NucDBManager();
      return(fgDBManager);
   }

   ~NucDBManager(){
      fgDBManager=0;
   }

   TSQLServer * fSQLServer;

protected :
   static NucDBManager * fgDBManager;

   NucDBManager() {
      fSQLServer = TSQLServer::Connect("mysql://localhost/nuclearDB", "bjorken", "drell");
      if(!fSQLServer) std::cout << " xxxxxx FAILED TO CONNECT TO SERVER !!! \n";
   }


ClassDef(NucDBManager,1)
};

#endif 
