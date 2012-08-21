#include "NucDBDataPoint.h"

ClassImp(NucDBDiscreteVariable)
//_____________________________________________________________________________

NucDBDiscreteVariable::NucDBDiscreteVariable(const char* name , const char* title) : TNamed(name,title) {
      fValue=0;
   }
//_____________________________________________________________________________

NucDBDiscreteVariable::~NucDBDiscreteVariable(){
}
//_____________________________________________________________________________

void NucDBDiscreteVariable::Print() {
      std::cout << " " << GetName() << " = " << fValue << "\n";
   }
//_____________________________________________________________________________
//_____________________________________________________________________________



ClassImp(NucDBBinnedVariable)
//_____________________________________________________________________________

NucDBBinnedVariable::NucDBBinnedVariable(const char* name, const char* title): TNamed(name, title) {
      fMinimum  = 0.0;
      fMaximum  = 0.0;
      fMean     = 0.0;
      fAverage  = 0.0;
   }
//_____________________________________________________________________________

NucDBBinnedVariable::~NucDBBinnedVariable(){}
//_____________________________________________________________________________

NucDBBinnedVariable::NucDBBinnedVariable(const NucDBBinnedVariable& v) {
      SetNameTitle(v.GetName(),v.GetTitle());
      fMinimum  = v.fMinimum;
      fMaximum  = v.fMaximum;
      fMean     = v.fMean;
      fAverage  = v.fAverage;
   }
//_____________________________________________________________________________

NucDBBinnedVariable& NucDBBinnedVariable::operator=(const NucDBBinnedVariable& v) {
      if ( this != &v) {  
         SetNameTitle(v.GetName(),v.GetTitle());
         fMinimum  = v.fMinimum;
         fMaximum  = v.fMaximum;
         fMean     = v.fMean;
         fAverage  = v.fAverage;
      }
      return *this;    // Return ref for multiple assignment
   }
//_____________________________________________________________________________

bool NucDBBinnedVariable::BinsOverlap(const NucDBBinnedVariable &var) const {
//       std::cout << " Min " <<  var.GetMinimum() 
//                 << " <" << fMinimum 
//                 << " < " << var.GetMaximum() << "\n";
      if(fMinimum > var.GetMinimum()  && fMinimum < var.GetMaximum()) return true;
      else if(fMaximum > var.GetMinimum()  && fMaximum < var.GetMaximum()) return true;
      else return false;
   }
//_____________________________________________________________________________

void      NucDBBinnedVariable::SetBinValueSize(Double_t val, Double_t size) {
      fMean = val;
      fAverage = val;
      SetBinMinimum(val-size/2.0);
      SetBinMaximum(val+size/2.0);
}
//_____________________________________________________________________________

void      NucDBBinnedVariable::Print() {
      std::cout << "  |" << GetName() << "|=" << fAverage << "       "
                << "  " << fMinimum << " < " << GetName() << " < " << fMaximum << "\n";
   }
//_____________________________________________________________________________
//_____________________________________________________________________________





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
   (*this) = v;
}
//_____________________________________________________________________________

NucDBErrorBar NucDBErrorBar::operator=(const NucDBErrorBar &v){
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

NucDBErrorBar NucDBErrorBar::operator+(const NucDBErrorBar &other) const {
    return NucDBErrorBar(*this) += other;
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

ClassImp(NucDBDataPoint)
//_____________________________________________________________________________
NucDBDataPoint::NucDBDataPoint(Double_t val, Double_t err) {
      fUnit = 0;
      fValue = val;
      fDimension=1;
      fDiscreteVariables.Clear();
      fVariables.Clear();
      fBinnedVariables.Clear();
      fName=" ";
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
      std::cout << fName.Data() << " = " << fValue << " +- " << fTotalError.GetError() << "\n";
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
      } else {
         printf(" variable, %s, already exists",var->GetName());
      }
   }
//_____________________________________________________________________________

//_____________________________________________________________________________

//_____________________________________________________________________________
