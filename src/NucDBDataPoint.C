#include "NucDBDataPoint.h"

ClassImp(NucDBErrorBar)
//_____________________________________________________________________________
//
NucDBErrorBar::NucDBErrorBar(Double_t err){
   fTotalError = 2.0*err;
   fErrorPlus  = err;
   fErrorMinus = err;
}
//_____________________________________________________________________________
NucDBErrorBar::NucDBErrorBar(Double_t errp, Double_t errm){
   fErrorPlus  = errp;
   fErrorMinus = TMath::Abs(errm);
   fTotalError = fErrorPlus + fErrorMinus;
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
const NucDBErrorBar& NucDBErrorBar::operator=(const NucDBErrorBar &v){
   if (this != &v) {
      fTotalError = v.fTotalError;
      fErrorPlus  = v.fErrorPlus;
      fErrorMinus = v.fErrorMinus;;
   }
   return *this;
}
//_____________________________________________________________________________
const NucDBErrorBar& NucDBErrorBar::operator+=(const NucDBErrorBar &v){
   // Not sure what the best way is to add asymmetric error bars, but here is a way
   // which keeps the errors asymmetric once an asymmetry is introduced.
   // - Calculate the asymmetry for each errorbar
   // - Calculate the weighted mean Asymmetry.
   // - Add the total errorin quaderature. 
   // - Use the mean asymmetry to get the plus and minus errors  

   double etot1 = fErrorPlus   + fErrorMinus;
   if(etot1==0) etot1 = 1.0e-20;
   double etot2 = v.fErrorPlus + v.fErrorMinus;
   if(etot2==0) etot2 = 1.0e-20;
   double eA1 = (fErrorPlus-fErrorMinus)/(etot1);
   double eA2 = (v.fErrorPlus-v.fErrorMinus)/(etot2);
   double eAavg = (eA1/TMath::Power(etot1/2.0,2.0) + eA2/TMath::Power(etot2/2.0,2.0))/
                  (1.0/TMath::Power(etot1/2.0,2.0) + 1.0/TMath::Power(etot2/2.0,2.0));
   double etotQuad = TMath::Sqrt(etot1*etot1 + etot2*etot2)/2.0;
   fErrorPlus  = etotQuad*(1.0 + eAavg);
   fErrorMinus = etotQuad*(1.0 - eAavg);
   //fErrorPlus  = TMath::Sqrt(TMath::Power(fErrorPlus ,2.0) + TMath::Power(v.fErrorPlus ,2.0) );
   //fErrorMinus = TMath::Sqrt(TMath::Power(fErrorMinus,2.0) + TMath::Power(v.fErrorMinus,2.0) );
   //std::cout << fTotalError << " and " << v.fTotalError << std::endl;
   fTotalError = TMath::Sqrt(TMath::Power(etot1,2.0) + TMath::Power(etot2,2.0) );
   return *this;
}
//_____________________________________________________________________________
const NucDBErrorBar& NucDBErrorBar::operator+(const NucDBErrorBar &v) const {
   return( NucDBErrorBar(*this) += v );
}
//_____________________________________________________________________________
void NucDBErrorBar::Print(Option_t * opt) const {
   std::cout << "Error Bar : " << GetName() << "\n";
   std::cout << "\t TotalError  = " << fTotalError << "\n";
   std::cout << "\t Error plus  = " << fErrorPlus << "\n";
   std::cout << "\t Error minus = " << fErrorMinus << "\n";
}
//_____________________________________________________________________________
void NucDBErrorBar::Clear(Option_t * opt){
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
   fSortingVariable = "x";

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
NucDBDataPoint::NucDBDataPoint(const NucDBDataPoint& v) : TNamed(v) {
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
   //CalculateTotalError();

}
//_____________________________________________________________________________
const NucDBDataPoint& NucDBDataPoint::operator=(const NucDBDataPoint &v){
   if (this == &v) return *this; 

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
   //CalculateTotalError();

   return *this;
}
//_____________________________________________________________________________
const NucDBDataPoint& NucDBDataPoint::operator+=(const NucDBDataPoint &v){
   // Calculates the weighted mean and variance of the mean

   CalculateTotalError(); 
   Double_t num = fValue/TMath::Power(fTotalError.GetError(),2.0)
                  + v.fValue/TMath::Power(v.fTotalError.GetError(),2.0);
   Double_t den = 1.0/TMath::Power(fTotalError.GetError(),2.0)
                  + 1.0/TMath::Power(v.fTotalError.GetError(),2.0);
   fValue       = num/den;
   Double_t err = TMath::Sqrt(1.0/den);

   // The systematic error is set to zero and all the error is pushed to the systematic
   fStatisticalError.SetError(err);
   fTotalError.SetError(err);
   fSystematicError.SetError(0.0);

   // Merge the bins
   for(int i=0; i<fBinnedVariables.GetEntries();i++) {
      NucDBBinnedVariable* binVar1 = (NucDBBinnedVariable*)fBinnedVariables.At(i); 
      NucDBBinnedVariable* binVar2 = v.GetBinVariable(binVar1->GetName()); 
      if(!binVar2){
         Error("NucDBDataPoint::operator+=","Could not find matching bin variable");
         continue;
      }
      (*binVar1) += (*binVar2);
   }

   CalculateTotalError(); 
   return *this;
}
//_____________________________________________________________________________
const NucDBDataPoint& NucDBDataPoint::operator+(const NucDBDataPoint &v) const {
   return( NucDBDataPoint(*this) += v );
}
//_____________________________________________________________________________
void NucDBDataPoint::CalculateTotalError(){
   //Double_t sys = fSystematicError.GetError();
   //Double_t stat = fStatisticalError.GetError();
   //fTotalError = SetError(sys+stat);
   fTotalError = fSystematicError+fStatisticalError; 
}
//_____________________________________________________________________________

void NucDBDataPoint::Print(Option_t * opt) const {
   std::cout << GetName() << " = " << GetValue() << " +- " << GetTotalError().GetError() << "\n";
   for(int i=0; i<fBinnedVariables.GetEntries();i++) {
      ((NucDBBinnedVariable*)fBinnedVariables.At(i))->Print(); 
   }
   for(int i=0; i<fDiscreteVariables.GetEntries();i++) {
      ((NucDBDiscreteVariable*)fDiscreteVariables.At(i))->Print(); 
   }
   for(int i=0; i<fVariables.GetEntries();i++) {
      //((NucDBVariable*)fVariables.At(i))->Print(); 
   }
}
//_____________________________________________________________________________
Int_t   NucDBDataPoint::Compare(const TObject *obj) const { 
   NucDBDataPoint      * dbobj = (NucDBDataPoint*)obj; 
   NucDBBinnedVariable * var1  = GetBinVariable(GetSortingVariable());
   NucDBBinnedVariable * var2  = dbobj->GetBinVariable(GetSortingVariable());
   if(!var1 || !var2) {
      Error("Compare","Could not find both bin variables. Check sorting variable name.");
      return 0;
   }
   if(var1->GetMean() > var2->GetMean() ) return 1;
   if(var1->GetMean() < var2->GetMean() ) return -1;
   return 0;
}
//______________________________________________________________________________
NucDBBinnedVariable* NucDBDataPoint::FindVariable(const char * name) {
   NucDBBinnedVariable * avar = 0;
   if( !avar ) avar = GetBinVariable(name);
   if( !avar ) avar = GetDependentVariable(name);
   /*   if( !avar ) avar = GetDiscreteVariable(name);*/
   return(avar);
}
//_____________________________________________________________________________

NucDBBinnedVariable* NucDBDataPoint::GetBinVariable(const char * name) const {
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
NucDBDiscreteVariable* NucDBDataPoint::GetDiscreteVariable(const char * name) {
      for(int i = 0;i<fDiscreteVariables.GetEntries();i++) {
          if( !strcmp( ((NucDBDiscreteVariable*)fDiscreteVariables.At(i))->GetName(),name) ) 
             return((NucDBDiscreteVariable*)fDiscreteVariables.At(i));
      }
      return(0);
   }
//_____________________________________________________________________________
void NucDBDataPoint::AddDiscreteVariable(NucDBDiscreteVariable * var) { 
   if( ! GetDiscreteVariable(var->GetName()) ) {
      fDiscreteVariables.Add(var);
      //fDimension++;
   } else {
      printf(" variable, %s, already exists",var->GetName());
   }
}
//_____________________________________________________________________________
NucDBVariable* NucDBDataPoint::GetVariable(const char * name) const {
      for(int i = 0;i<fVariables.GetEntries();i++) {
          if( !strcmp( ((NucDBVariable*)fVariables.At(i))->GetName(),name) ) 
             return((NucDBVariable*)fVariables.At(i));
      }
      return(0);
   }
//_____________________________________________________________________________
void NucDBDataPoint::AddVariable(NucDBVariable * var) { 
   if( ! GetVariable(var->GetName()) ) {
      fVariables.Add(var);
      //fDimension++;
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
         AddBinVariable(var);
      } else {
         Error("AddDependentVariable","variable, %s, already exists",var->GetName());
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

