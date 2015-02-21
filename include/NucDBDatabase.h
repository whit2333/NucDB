#ifndef NucDBDatabase_HH
#define NucDBDatabase_HH 1
#include "NucDBExperiment.h"
#include "NucDBMeasurement.h"
#include "NucDBPaper.h"
#include "TObjString.h"
#include "TNamed.h"

/** Database object saved to file.
 *  This is the top container which is streamed to file. 
 */
class NucDBDatabase : public TNamed {
   protected:
      TList fExperiments;
      TList fMeasurements;
      TList fPapers;

   public:
      NucDBDatabase(const char * n ="NucDB-database",const char * t = "NucDB Database") : TNamed(n,t){}
      virtual ~NucDBDatabase(){}

      Bool_t IsFolder() const { return kTRUE; }
      void   Browse(TBrowser* b) {
         b->Add(&fExperiments, "Experiments");
         b->Add(&fMeasurements, "Measurements");
         b->Add(&fPapers, "Papers");
      }

      void ListExperiments() const ;   // *MENU*
      void ListMeasurements() const ;  // *MENU*
      void ListPapers() const ;        // *MENU*
      void Print() const ;             // *MENU*

      TList * GetExperiments(){return(&fExperiments);}
      TList * GetMeasurements(){return(&fMeasurements);}
      TList * GetPapers(){return(&fPapers);}

      ClassDef(NucDBDatabase,1)
};



#endif
