#include "NucDBDatabase.h"

ClassImp(NucDBDatabase)
//_____________________________________________________________________________

void NucDBDatabase::Print() const {
      std::cout << "Database : " << GetName() << " , title = " << GetTitle() << "\n";
      ListExperiments();
      ListPapers();
   }
//_____________________________________________________________________________

void NucDBDatabase::ListExperiments() const {
      std::cout << "Experiments:\n";
      for(int i = 0; i< fExperiments.GetEntries();i++){
         NucDBExperiment * exp  = (NucDBExperiment*)fExperiments.At(i);
         std::cout << " - " << exp->GetName() << "\n";
      }
   }
//_____________________________________________________________________________

void NucDBDatabase::ListMeasurements() const {
      std::cout << "Measurements:\n";
      for(int i = 0; i< fMeasurements.GetEntries();i++){
         TObjString * meas  = (TObjString*)fMeasurements.At(i);
         std::cout << " - " << meas->GetName() << "\n";
      }
   }

//_____________________________________________________________________________

void NucDBDatabase::ListPapers() const {
      std::cout << "Papers:\n";
      for(int i = 0; i< fPapers.GetEntries();i++){
         NucDBPaper * pap  = (NucDBPaper*)fPapers.At(i);
         std::cout << " - " << pap->GetName() << "\n";
      }
   }
//_____________________________________________________________________________
