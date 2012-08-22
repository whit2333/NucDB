#include "NucDBDataPoint.h"

ClassImp(NucDBErrorBar)
//_____________________________________________________________________________

NucDBErrorBar::NucDBErrorBar(){
      fTotalError=0.0;
      fErrorPlus=0.0;
      fErrorMinus=0.0;
}
//_____________________________________________________________________________

NucDBErrorBar::~NucDBErrorBar(){
}
//_____________________________________________________________________________

NucDBErrorBar::NucDBErrorBar(const NucDBErrorBar& v) {
      fTotalError = v.fTotalError;
      fErrorPlus  = v.fErrorPlus;
      fErrorMinus = v.fErrorMinus;
}
//_____________________________________________________________________________

NucDBErrorBar& NucDBErrorBar::operator=(const NucDBErrorBar &v){
   if (this != &v) {
      fTotalError = v.fTotalError;
      fErrorPlus  = v.fErrorPlus;
      fErrorMinus = v.fErrorMinus;;
   }
   return *this;
}
//_____________________________________________________________________________

NucDBErrorBar& NucDBErrorBar::operator+=(const NucDBErrorBar &v){
   fTotalError += v.fTotalError;
   fErrorPlus  += v.fErrorPlus;
   fErrorMinus += v.fErrorMinus;;
   return *this;
}

//_____________________________________________________________________________

NucDBErrorBar& NucDBErrorBar::operator+(const NucDBErrorBar &v) const {
    return NucDBErrorBar(*this) += v ;
  }
//_____________________________________________________________________________

void NucDBErrorBar::Print() const {
   std::cout << "Error Bar : " << GetName() << "\n";
   std::cout << "\t TotalError  = " << fTotalError << "\n";
   std::cout << "\t Error plus  = " << fErrorPlus << "\n";
   std::cout << "\t Error minus = " << fErrorMinus << "\n";
}
//_____________________________________________________________________________

void NucDBErrorBar::Clear(){
      fTotalError=0.0;
      fErrorPlus=0.0;
      fErrorMinus=0.0;
   }
//_____________________________________________________________________________
//_____________________________________________________________________________



ClassImp(NucDBDataPoint)
//_____________________________________________________________________________
NucDBDataPoint::NucDBDataPoint(Double_t val, Double_t err) {
      fUnit = 0;
      fValue = val;
      fDimension=1;
      fDiscreteVariables.Clear();
      fVariables.Clear();
      fBinnedVariables.Clear();
      fTotalError.Clear();
      fSystematicError.Clear();
      fStatisticalError.Clear();

   }
//_____________________________________________________________________________

NucDBDataPoint::~NucDBDataPoint(){}
//_____________________________________________________________________________

void NucDBDataPoint::CalculateTotalError(){
      Double_t sys = fSystematicError.GetError();
      Double_t stat = fStatisticalError.GetError();
      fTotalError.SetError(sys+stat);
   }
//_____________________________________________________________________________

void NucDBDataPoint::Print(){
      std::cout << GetName() << " = " << GetValue() << " +- " << fTotalError.GetError() << "\n";
      for(int i=0; i<fBinnedVariables.GetEntries();i++) {
         ((NucDBBinnedVariable*)fBinnedVariables.At(i))->Print(); 
         
      }
   }
//_____________________________________________________________________________

NucDBBinnedVariable* NucDBDataPoint::GetBinVariable(const char * name) {
      for(int i = 0;i<fBinnedVariables.GetEntries();i++) {
          if( !strcmp( ((NucDBBinnedVariable*)fBinnedVariables.At(i))->GetName(),name) ) 
             return((NucDBBinnedVariable*)fBinnedVariables.At(i));
      }
      return(0);
   }
//_____________________________________________________________________________

void NucDBDataPoint::AddBinVariable(NucDBBinnedVariable * var) { 
      if( ! GetBinVariable(var->GetName()) ) {
         fBinnedVariables.Add(var);
         fDimension++;
      } else {
         printf(" variable, %s, already exists",var->GetName());
      }
   }
//_____________________________________________________________________________

NucDBDependentVariable* NucDBDataPoint::GetDependentVariable(const char * name) {
      for(int i = 0;i<fVariables.GetEntries();i++) {
          if( !strcmp( ((NucDBDependentVariable*)fVariables.At(i))->GetName(),name) ) 
             return((NucDBDependentVariable*)fVariables.At(i));
      }
      return(0);
   }
//_____________________________________________________________________________

void NucDBDataPoint::AddDependentVariable(NucDBDependentVariable * var) { 
      if( ! GetDependentVariable(var->GetName()) ) {
         fVariables.Add(var);
      } else {
         Error("AddDependentVariable",Form(" variable, %s, already exists",var->GetName()));
      }
   }
//_____________________________________________________________________________

void NucDBDataPoint::CalculateDependentVariables(){
   NucDBDependentVariable * var = 0;
   for(int i = 0;i<fVariables.GetEntries(); i++ ){
      var = (NucDBDependentVariable*)fVariables.At(i);
      var->Calculate();
   }
}
//_____________________________________________________________________________
