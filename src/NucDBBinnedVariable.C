#include "NucDBBinnedVariable.h"

ClassImp(NucDBVariable)
//_____________________________________________________________________________
NucDBVariable::NucDBVariable(const char*n, const char* t ) : TNamed(n,t) {
   fValue = 0.0;
}
//_____________________________________________________________________________
NucDBVariable::~NucDBVariable(){
}
//_____________________________________________________________________________
void NucDBVariable::Print() {
      std::cout << " " << GetName() << " = " << fValue << "\n";
}
//_____________________________________________________________________________



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
   fMinimum  = v.GetMinimum();
   fMaximum  = v.GetMaximum();
   fMean     = v.GetMean();
   fAverage  = v.GetAverage();
}
//_____________________________________________________________________________
const NucDBBinnedVariable& NucDBBinnedVariable::operator+=(const NucDBBinnedVariable& v) {
   if(fMinimum > v.GetMinimum() ) fMinimum = v.GetMinimum();
   if(fMaximum < v.GetMaximum() ) fMaximum = v.GetMaximum();
   fMean = (fMaximum + fMinimum)/2.0;
   fAverage = fMean;
   return *this;
}
//______________________________________________________________________________
const NucDBBinnedVariable& NucDBBinnedVariable::operator+( const NucDBBinnedVariable& v) const {
   return( NucDBBinnedVariable(*this) += v );
}
//______________________________________________________________________________
NucDBBinnedVariable& NucDBBinnedVariable::operator=(const NucDBBinnedVariable& v) {
   if ( this != &v) {  
      SetNameTitle(v.GetName(),v.GetTitle());
      fMinimum  = v.GetMinimum();
      fMaximum  = v.GetMaximum();
      fMean     = v.GetMean();
      fAverage  = v.GetAverage();
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

