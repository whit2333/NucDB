#include "NucDBPaper.h"


//______________________________________________________________________________
NucDBPaper::NucDBPaper(const char * name,const char * title):TNamed(name,title){
   fCalculations.SetName("Calculations");
   fCalculations.SetOwner(true);
}
//______________________________________________________________________________
NucDBPaper::~NucDBPaper(){
   fCalculations.Clear();
}
//______________________________________________________________________________
NucDBCalculation * NucDBPaper::GetCalculation(const char * name) {
   NucDBCalculation * meas = nullptr;
   for(int i = 0;i<fCalculations.GetEntries();i++) {
      if( !strcmp((dynamic_cast<NucDBCalculation *>(fCalculations.At(i)))->GetName(),name) ) {
         meas = dynamic_cast<NucDBCalculation *>(fCalculations.At(i)) ;
         break;
      }
   }
   if(!meas) { std::cout << " Calculation named " << name << " NOT FOUND!\n";
}
   return(meas);
}
//______________________________________________________________________________

