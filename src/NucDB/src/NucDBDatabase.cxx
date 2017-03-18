#include "NucDBDatabase.h"

ClassImp(NucDBDatabase)
//_____________________________________________________________________________

void NucDBDatabase::Print(Option_t *option) const {
      std::cout << "Database : " << GetName() << " , title = " << GetTitle() << "\n";
      ListExperiments();
      ListPapers();
   }
//_____________________________________________________________________________

void NucDBDatabase::ListExperiments() const {
      std::cout << "Experiments:\n";
      for(int i = 0; i< fExperiments.GetEntries();i++){
         auto * exp  = dynamic_cast<NucDBExperiment*>(fExperiments.At(i));
         std::cout << " - " << exp->GetName() << "\n";
      }
   }
//_____________________________________________________________________________

void NucDBDatabase::ListMeasurements() const {
      std::cout << "Measurements:\n";
      for(int i = 0; i< fMeasurements.GetEntries();i++){
         auto * meas  = dynamic_cast<TObjString*>(fMeasurements.At(i));
         std::cout << " - " << meas->GetName() << "\n";
      }
   }

//_____________________________________________________________________________

void NucDBDatabase::ListPapers() const {
      std::cout << "Papers:\n";
      for(int i = 0; i< fPapers.GetEntries();i++){
         auto * pap  = dynamic_cast<NucDBPaper*>(fPapers.At(i));
         std::cout << " - " << pap->GetName() << "\n";
      }
   }
//_____________________________________________________________________________
