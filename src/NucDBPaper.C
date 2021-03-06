#include "NucDBPaper.h"


//______________________________________________________________________________
NucDBPaper::NucDBPaper(const char * name,const char * title):TNamed(name,title){
   fCalculations.Clear();
   fCalculations.SetName("Calculations");
}
//______________________________________________________________________________
NucDBPaper::~NucDBPaper(){
}
//______________________________________________________________________________
NucDBCalculation * NucDBPaper::GetCalculation(const char * name) {
   NucDBCalculation * meas = 0;
   for(int i = 0;i<fCalculations.GetEntries();i++) {
      if( !strcmp(((NucDBCalculation *)fCalculations.At(i))->GetName(),name) ) {
         meas = (NucDBCalculation *)fCalculations.At(i) ;
         break;
      }
   }
   if(!meas) std::cout << " Calculation named " << name << " NOT FOUND!\n";
   return(meas);
}

