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
   fErrorPlus  = TMath::Sqrt(TMath::Power(fErrorPlus ,2.0) + TMath::Power(v.fErrorPlus ,2.0) );
   fErrorMinus = TMath::Sqrt(TMath::Power(fErrorMinus,2.0) + TMath::Power(v.fErrorMinus,2.0) );
   fTotalError = TMath::Sqrt(TMath::Power(fTotalError,2.0) + TMath::Power(v.fTotalError,2.0) );
   return *this;
}
//_____________________________________________________________________________
NucDBErrorBar& NucDBErrorBar::operator+(const NucDBErrorBar &v) const {
   return( NucDBErrorBar(*this) += v );
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
   fTotalError = 0.0;
   fErrorPlus  = 0.0;
   fErrorMinus = 0.0;
}
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
NucDBDataPoint::~NucDBDataPoint(){
}
//_____________________________________________________________________________
NucDBDataPoint::NucDBDataPoint(const NucDBDataPoint& v) {
   fUnit      = v.fUnit;
   fValue     = v.fValue;
   fDimension = v.fDimension;

   // not sure if this is the best way to make a deep copy. 
   TList * alist      = 0;
   alist              = (TList*)v.fDiscreteVariables.Clone();
   fDiscreteVariables.AddAll(alist);
   alist              = (TList*)v.fVariables.Clone();
   fVariables.AddAll(alist);
   alist              = (TList*)v.fBinnedVariables.Clone();
   fBinnedVariables.AddAll(alist);

   fSystematicError   = v.fSystematicError;
   fStatisticalError  = v.fStatisticalError;
   fTotalError        = v.fTotalError;

}
//_____________________________________________________________________________
void NucDBDataPoint::CalculateTotalError(){
   //Double_t sys = fSystematicError.GetError();
   //Double_t stat = fStatisticalError.GetError();
   //fTotalError = SetError(sys+stat);
   fTotalError = fSystematicError+fStatisticalError; 
}
//_____________________________________________________________________________

void NucDBDataPoint::Print(){
   std::cout << GetName() << " = " << GetValue() << " +- " << GetTotalError()->GetError() << "\n";
   for(int i=0; i<fBinnedVariables.GetEntries();i++) {
      ((NucDBBinnedVariable*)fBinnedVariables.At(i))->Print(); 

   }
}
//_____________________________________________________________________________

NucDBBinnedVariable* NucDBDataPoint::FindVariable(const char * name) {
   NucDBBinnedVariable * avar = 0;
   if( !avar ) avar = GetBinVariable(name);
   if( !avar ) avar = GetDependentVariable(name);
/*   if( !avar ) avar = GetDiscreteVariable(name);*/
   return(avar);
}
//_____________________________________________________________________________

NucDBBinnedVariable* NucDBDataPoint::GetBinVariable(const char * name) {
      for(int i = 0;i<fBinnedVariables.GetEntries();i++) {
          if( !strcmp( ((NucDBBinnedVariable*)fBinnedVariables.At(i))->GetName(),name) ) 
             return((NucDBBinnedVariable*)fBinnedVariables.At(i));
      }
      return(0);
   }//_____________________________________________________________________________

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

NucDBDiscreteVariable* NucDBDataPoint::GetDiscreteVariable(const char * name) {
      for(int i = 0;i<fDiscreteVariables.GetEntries();i++) {
          if( !strcmp( ((NucDBDiscreteVariable*)fDiscreteVariables.At(i))->GetName(),name) ) 
             return((NucDBDiscreteVariable*)fDiscreteVariables.At(i));
      }
      return(0);
   }
//_____________________________________________________________________________

void  NucDBDataPoint::ListVariables(){
   std::cout << "Variables: \n";
      for(int i = 0;i<fVariables.GetEntries();i++) {
         std::cout << " - " << fVariables.At(i)->GetName() << "\n";
      }
      for(int i = 0;i<fBinnedVariables.GetEntries();i++) {
         std::cout << " - " << fBinnedVariables.At(i)->GetName() << "\n";
      }
}
//_____________________________________________________________________________
