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
void NucDBVariable::Print(Option_t *option) const {
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

void NucDBDiscreteVariable::Print(Option_t *option) const {
   std::cout << " " << GetName() << " = " << fValue << "\n";
}
//_____________________________________________________________________________



ClassImp(NucDBBinnedVariable)
//_____________________________________________________________________________
NucDBBinnedVariable::NucDBBinnedVariable(
      const char * name, 
      const char * title, 
      Double_t     y, 
      Double_t     dy) :
   TNamed(name,title), 
   fMinimum(y-dy),
   fMaximum(y+dy),
   fMean(y),
   fAverage(y),
   fSortPriority(-1)
{
   SetBinMinimum(fMinimum);
}
//_____________________________________________________________________________
NucDBBinnedVariable::~NucDBBinnedVariable(){ }
//_____________________________________________________________________________

NucDBBinnedVariable::NucDBBinnedVariable(const NucDBBinnedVariable& v) {
   SetNameTitle(v.GetName(),v.GetTitle());
   fMinimum  = v.GetMinimum();
   fMaximum  = v.GetMaximum();
   fMean     = v.GetMean();
   fAverage  = v.GetAverage();
   fCenter   = v.GetCenter();
   fSortPriority = v.fSortPriority; 
}
//_____________________________________________________________________________
const NucDBBinnedVariable& NucDBBinnedVariable::operator+=(const NucDBBinnedVariable& v) {
   // a weight for finding where the mean should be.
   double weight = GetBinSize()/v.GetBinSize(); 
   if(fMinimum > v.GetMinimum() ) fMinimum = v.GetMinimum();
   if(fMaximum < v.GetMaximum() ) fMaximum = v.GetMaximum();
   SetBinMaximum(fMaximum);
   SetBinMinimum(fMinimum);
   fMean    = (weight*fMean + v.fMean)/(weight+1);

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
      fSortPriority = v.fSortPriority; 
   }
   return *this;    // Return ref for multiple assignment
}
//_____________________________________________________________________________

bool NucDBBinnedVariable::BinsOverlap(const NucDBBinnedVariable &var) const {
   //       std::cout << " Min " <<  var.GetMinimum() 
   //                 << " <" << fMinimum 
   //                 << " < " << var.GetMaximum() << "\n";
   if( fMinimum >= var.GetMinimum()  && fMinimum <= var.GetMaximum()) return true;
   else if(fMaximum >= var.GetMinimum()  && fMaximum <= var.GetMaximum()) return true;
   else if( var.GetMinimum() >= GetMinimum()  && var.GetMinimum() <= GetMaximum()) return true;
   else if( var.GetMaximum() >= GetMinimum()  && var.GetMaximum() <= GetMaximum()) return true;
   else return false;
}
//_____________________________________________________________________________
bool NucDBBinnedVariable::Contains(double v) const {
   if( ( v >= GetMinimum() ) && ( v < GetMaximum() ) ) return true;
   return false;
}
bool NucDBBinnedVariable::IsBelow(double v) const {
   if( GetMaximum() < v ) return true;
   return false;
}
bool NucDBBinnedVariable::IsAbove(double v) const {
   if( GetMinimum() > v ) return true;
   return false;
}
//_____________________________________________________________________________
void NucDBBinnedVariable::SetBinValueSize(Double_t val, Double_t size) {
   fMean = val;
   SetBinMinimum(val-size/2.0);
   SetBinMaximum(val+size/2.0);
}
//______________________________________________________________________________
void NucDBBinnedVariable::SetAverage(      Double_t val) {
   // keep the bin size but shift the center
   // Note the mean stays the same.
   double size = GetBinSize();
   fAverage = val; 
   fCenter = val; 
   fMinimum=val-size/2.0;
   fMaximum=val+size/2.0;
   if(fMean<fMinimum ) {
      Error("SetAverage","Mean less than bin min");
   }
   if(fMean>fMaximum ) {
      Error("SetAverage","Mean greater than bin max");
   }
}
//_____________________________________________________________________________

void      NucDBBinnedVariable::Print() {
   std::cout << " |" << GetName() << "|=" << fAverage << " "
             << " (" << fMinimum << " < " << GetName() << " < " << fMaximum << ")\n" ;
             //<< " sort: " << fSortPriority << "\n";
}
//_____________________________________________________________________________

