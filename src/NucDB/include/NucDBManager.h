#ifndef NucDBManager_HH
#define NucDBManager_HH 1
#include "NucDBConfig.h"

#include "TNamed.h"
#include "TObject.h"
#include "TROOT.h"
#include <iostream>
#include "TList.h"
#include "NucDBExperiment.h"
#include "TFile.h"
#include "NucDBUnits.h"
#include "TKey.h"
#include "TDirectory.h"
#include <set>
#include <string>
#include "NucDBPaper.h"
#include "TBrowser.h"
#include "TObjString.h"
#include "NucDBDatabase.h"
#include "TMultiGraph.h"
#include "NucDBReference.h"

class NucDBManager : public TObject {

   private:
      NucDBDatabase * fDatabase;
      TList * fExperiments;
      TList * fPapers;
      TList * fMeasurements; /// List TObjString names
      TList * fCalculations; /// List TObjString names

      TList   fStandardUnits;
      Int_t   fVerbosity;

      std::vector<int> fColors;
      std::vector<int> fMarkers;
      std::vector<int> fFillStyles;

      Int_t fi_color;
      Int_t fi_marker;


      std::string  fFileName = "";
      TFile      * fFile     = nullptr;
      TDirectory * fExpDir{};
      TDirectory * fPapersDir{};
      TDirectory * fCalcDir{};

      static NucDBManager * fgDBManager;

      /** Adds a new TObjString to fMeasurments list if measurment
       *  does not exist. This is done in order to list the available 
       *  measurments and not every single time there is an instance of the 
       *  measurement from a different experiment.
       */
      void   AddNewMeasurements(NucDBExperiment * exp);

   public :

      /*! If opt is not zero it is opened in UPDATE mode, otherwise it is readonly.  */ 
      NucDBManager() : NucDBManager(0) { }
      NucDBManager(Int_t opt) : NucDBManager("",opt) { }
      NucDBManager(const char * file, Int_t opt);

      static NucDBManager * GetManager(int opt = 0){
         if(!fgDBManager) { fgDBManager = new NucDBManager(opt);
}
         return(fgDBManager);
      }

      ~NucDBManager();

      Int_t   GetVerbosity(){return fVerbosity;}
      void    SetVerbosity(Int_t v) {fVerbosity = v;}

      Int_t   Load(const char * n = "NucDB-database");
      void    SaveDatabase();

      /** Gets an experiment by name. 
      */
      NucDBExperiment *  GetExperiment(const char * expName);

      /** Gets a paper by name. 
      */
      NucDBPaper *       GetPaper(const char * name);

      /** Returns the list of experiments.  */
      TList *            GetExperiments();

      TList *            GetPapers();

      /** Get a new list of measurments with that name, (eg, F2p,g1p ).
       *  It combs through all experiments looking for the measurment.
       */
      TList *            GetMeasurements(const char * measurement);
      TList *            GetMeasurementCalculations(const char * measurement);
      std::vector<NucDBMeasurement*>  GetAllMeasurements(const char * measurement);
      
      std::vector<NucDBMeasurement*>       GetMeasurements(NucDB::Process proc);

      /** Returns a TMultiGraph of the measurment for the given variable. */
      TMultiGraph *      GetMultiGraph(const char * measurement, const char * var = "x");
      TMultiGraph *      GetKinematicMultiGraph(const char * measurement, const char * var1 = "x",const char * var2 = "Qsquared");

      /** Saves data in root file: data/NucDB.root. 
      */
      void               SaveExperiment(NucDBExperiment * exp);

      /** Saves data in root file: data/NucDB.root. 
      */
      void               SavePaper(NucDBPaper * p) ;

      /** Prints the experiments which are available in the database. 
       *  Returns the total number of experiments
       */
      Int_t              ListExperiments();
      Int_t              ListPapers();

      TList * GetRefs(); 

      Int_t  NextColor() {
         if( fColors.size() == 0 ) { return 1; // black
}
         // returns next color in list
         Int_t col = fColors[fi_color];
         fi_color++;
         if( (unsigned int)fi_color == fColors.size() ) { fi_color = 0;
}
         return col;
      }
      Int_t  NextMarker() {
         if( fMarkers.size() == 0 ) { return 20; // filled circle
}
         // returns next color in list
         Int_t mark = fMarkers[fi_marker];
         fi_marker++;
         if( (unsigned int)fi_marker == fMarkers.size() ) { fi_marker = 0;
}
         return mark;
      }
      void SetMarkers(const std::vector<int> & m) { fMarkers =  m; }
      void SetColors(const std::vector<int> & m) { fColors =  m; }

      /** Prints the measurements which are stored in the database. 
       *  A measurement would be, for example, F1p, g2P, F2C/F2D,...
       *  Returns the total number of measurements
       */
      Int_t              ListMeasurements(const char * n = "");
      Int_t              ListCalculations(const char * n = "");

      /** Prints the measurements for each experiment.
       *  If a measurment name is provided, only experments with that measurement are listed.
       */ 
      Int_t              ListMeasurementsByExperiment(const char * measurement = ""); 

      ClassDef(NucDBManager,1)
};

#endif 
