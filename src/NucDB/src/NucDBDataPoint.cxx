#include "NucDBDataPoint.h"
#include <iomanip>

NucDBDataPoint::NucDBDataPoint(Double_t val, Double_t err, Double_t syst)
   : fDimension(1), fNSortVariables(1), 
   fSortingVariable("x"), fSortingVariable2("Qsquared"),
   fValue(val)
{
   fUnit = 0;

   fDiscreteVariables.Clear();
   fVariables.Clear();
   fBinnedVariables.Clear();

   fTotalError.Clear();
   fSystematicError.Clear();
   fStatisticalError.Clear();

   fStatisticalError.SetError(err);
   fSystematicError.SetError(syst);

   CalculateTotalError();
}
//_____________________________________________________________________________

NucDBDataPoint::~NucDBDataPoint()
{ }
//_____________________________________________________________________________

NucDBDataPoint::NucDBDataPoint(const NucDBDataPoint& v) : TNamed(v)
{
   fUnit      = v.fUnit;
   fValue     = v.fValue;
   fDimension = v.fDimension;
   fNSortVariables = v.fNSortVariables;
   fDataSet   = v.fDataSet;

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

   CalculateTotalError();

}
//_____________________________________________________________________________

NucDBDataPoint& NucDBDataPoint::operator=(const NucDBDataPoint &v)
{
   if (this == &v) return *this; 

   fUnit      = v.fUnit;
   fValue     = v.fValue;
   fDimension = v.fDimension;
   fNSortVariables = v.fNSortVariables;
   fDataSet   = v.fDataSet;

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
   CalculateTotalError();

   return *this;
}
//_____________________________________________________________________________

const NucDBDataPoint& NucDBDataPoint::operator+=(const NucDBDataPoint &v)
{
   // Calculates the weighted mean and variance of the mean

   CalculateTotalError(); 
   Double_t num = fValue/TMath::Power(fStatisticalError.GetError(),2.0)
                  + v.fValue/TMath::Power(v.fStatisticalError.GetError(),2.0);
   Double_t den = 1.0/TMath::Power(fStatisticalError.GetError(),2.0)
                  + 1.0/TMath::Power(v.fStatisticalError.GetError(),2.0);
   fValue       = num/den;
   Double_t err = TMath::Sqrt(1.0/den);

   // The systematic error is set to zero and all the error is pushed to the systematic
   fStatisticalError.SetError(err);
   //fTotalError.SetError(err);

   Double_t sys1 = fSystematicError.GetError();
   Double_t sys2 = v.fSystematicError.GetError();

   fSystematicError.SetError((sys1+sys2)/2.0);

   // Merge the bins
   for(int i=0; i<fBinnedVariables.GetEntries();i++) {
      NucDBBinnedVariable* binVar1 = (NucDBBinnedVariable*)fBinnedVariables.At(i); 
      NucDBBinnedVariable* binVar2 = v.GetBinVariable(binVar1->GetName()); 
      if(!binVar2){
         Error("NucDBDataPoint::operator+=","Could not find matching bin variable %s",binVar1->GetName());
         continue;
      }
      (*binVar1) += (*binVar2);
   }

   CalculateTotalError(); 
   return *this;
}
//_____________________________________________________________________________

const NucDBDataPoint& NucDBDataPoint::operator-=(const NucDBDataPoint &v)
{
   // Calculates the weighted mean and variance of the mean

   CalculateTotalError(); 
   Double_t num = fValue/TMath::Power(fStatisticalError.GetError(),2.0)
                  - v.fValue/TMath::Power(v.fStatisticalError.GetError(),2.0);
   Double_t den = 1.0/TMath::Power(fStatisticalError.GetError(),2.0)
                  + 1.0/TMath::Power(v.fStatisticalError.GetError(),2.0);
   fValue       = num/den;
   Double_t err = TMath::Sqrt(1.0/den);

   // The systematic error is set to zero and all the error is pushed to the systematic
   fStatisticalError.SetError(err);
   //fTotalError.SetError(err);

   Double_t sys1 = fSystematicError.GetError();
   Double_t sys2 = v.fSystematicError.GetError();

   fSystematicError.SetError((sys1+sys2)/2.0);

   // Merge the bins
   for(int i=0; i<fBinnedVariables.GetEntries();i++) {
      NucDBBinnedVariable* binVar1 = (NucDBBinnedVariable*)fBinnedVariables.At(i); 
      NucDBBinnedVariable* binVar2 = v.GetBinVariable(binVar1->GetName()); 
      if(!binVar2){
         Error("NucDBDataPoint::operator-=","Could not find matching bin variable");
         continue;
      }
      (*binVar1) -= (*binVar2);
   }
   CalculateTotalError(); 
   return *this;
}
//_____________________________________________________________________________

const NucDBDataPoint& NucDBDataPoint::operator+(const NucDBDataPoint &v) const
{
   return( NucDBDataPoint(*this) += v );
}
//______________________________________________________________________________

const NucDBDataPoint& NucDBDataPoint::operator-(const NucDBDataPoint &v) const
{
   return( NucDBDataPoint(*this) -= v );
}
//_____________________________________________________________________________

void NucDBDataPoint::CalculateTotalError()
{
   fTotalError = fSystematicError+fStatisticalError; 
}
//_____________________________________________________________________________

void NucDBDataPoint::Print(Option_t * opt) const
{
   std::cout << std::setw(6) << GetName() << " = " << GetValue()
             << " +- " << GetStatError().GetError() << "(stat)" 
             << " +- " << GetSystError().GetError() << "(sys) ";
   std::cout << " dataset(" << fDataSet << ")\n";
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

bool   NucDBDataPoint::operator==(const NucDBDataPoint & rhs) const
{ 
   NucDBBinnedVariable * var1  = GetBinVariable(GetSortingVariable());
   NucDBBinnedVariable * var2  = rhs.GetBinVariable(GetSortingVariable());
   if(!var1 || !var2) {
      Error("Compare","Could not find both bin variables. Check sorting variable name.");
      return false;
   }
   double a0 = var1->GetMean();
   double a1 = var2->GetMean();
   if( a0 != a1 ) return false; 

   //NucDBBinnedVariable * var3  = GetBinVariable(GetSortingVariable2());
   //NucDBBinnedVariable * var4  = rhs.GetBinVariable(GetSortingVariable2());
   //if(!var3 || !var4) {
   //   Error("Compare","Could not find both bin variables. Check sorting variable name.");
   //   return false;
   //}
   //double b0 = var3->GetMean();
   //double b1 = var4->GetMean();
   //if( b0 != b1 ) return false; 
   return true;
}
//_____________________________________________________________________________

bool   NucDBDataPoint::operator!=(const NucDBDataPoint & rhs) const
{ 
   return( !((*this) == rhs) );
}
//_____________________________________________________________________________

bool   NucDBDataPoint::operator<(const NucDBDataPoint & rhs) const
{ 
   NucDBBinnedVariable * var1  = GetBinVariable(GetSortingVariable());
   NucDBBinnedVariable * var2  = rhs.GetBinVariable(GetSortingVariable());
   if(!var1 || !var2) {
      Error("Compare","Could not find both bin variables. Check sorting variable name.");
      return false;
   }
   double a0 = var1->GetMean();
   double a1 = var2->GetMean();
   if( a0 < a1 ){
      return true; 
   }
   //NucDBBinnedVariable * var3  = GetBinVariable(GetSortingVariable2());
   //NucDBBinnedVariable * var4  = rhs.GetBinVariable(GetSortingVariable2());
   //if(!var3 || !var4) {
   //   Error("Compare","Could not find both bin variables. Check sorting variable name.");
   //   return false;
   //}
   //double b0 = var3->GetMean();
   //double b1 = var4->GetMean();
   //if( a0 == a1 ) if( b0 < b1 ) return true; 
   //std::cout << GetSortingVariable() << " : "  << var1->GetMean() << " < " <<  var2->GetMean() << " = " << (var1->GetMean() < var2->GetMean()) << std::endl;
   return false;
}
//_____________________________________________________________________________

bool   NucDBDataPoint::operator>(const NucDBDataPoint & rhs) const
{ 
   return( !((*this) <= rhs) );
}
//_____________________________________________________________________________

bool   NucDBDataPoint::operator<=(const NucDBDataPoint & rhs) const
{ 
   if( ((*this) == rhs) )  return true;
   if( ((*this) <  rhs) )  return true;
   return false;
}
//_____________________________________________________________________________

bool   NucDBDataPoint::operator>=(const NucDBDataPoint & rhs) const
{ 
   return( !((*this) < rhs) );
}
//______________________________________________________________________________

NucDBBinnedVariable * NucDBDataPoint::GetSortPriority(int p) const
{
   // returns the first binned variable found with sort priority p.
   for(int i = 0; i<fBinnedVariables.GetEntries(); i++) {
      NucDBBinnedVariable* aVar = ((NucDBBinnedVariable*)fBinnedVariables.At(i));
      if( aVar->GetSortPriority() == p) {
        return aVar;
      }
   }
   return 0;
}
//_____________________________________________________________________________

void NucDBDataPoint::SetSortPriorities(const std::vector<std::string> & names)
{
   for(int i = 0;i<fBinnedVariables.GetEntries();i++) {
      ((NucDBBinnedVariable*)fBinnedVariables.At(i))->SetSortPriority(-1);
   }
   fNSortVariables = names.size();
   int n_sort = fNSortVariables - 1;
   for( auto name : names ) {
      NucDBBinnedVariable * avar = GetBinVariable(name.c_str());
      if( avar ) {
         avar->SetSortPriority(n_sort) ;
         n_sort--;
      }
   }
}
//______________________________________________________________________________

Int_t   NucDBDataPoint::Compare(const TObject *obj) const
{ 
   NucDBDataPoint * dbobj = (NucDBDataPoint*)obj; 
   if( (*this) > (*dbobj) ) return 1;
   if( (*this) < (*dbobj) ) return -1;
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
      Error("AddBinVariable","variable, %s, already exists",var->GetName());
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
      Error("AddDiscreteVariable","variable, %s, already exists",var->GetName());
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
      Error("AddVariable","variable, %s, already exists",var->GetName());
   }
}
//_____________________________________________________________________________

NucDBDependentVariable* NucDBDataPoint::GetDependentVariable(const char * name)
{
   for(int i = 0;i<fBinnedVariables.GetEntries();i++) {
      NucDBDependentVariable* var = dynamic_cast<NucDBDependentVariable*>(fBinnedVariables.At(i));
      if(!var) continue;
      if( !strcmp( var->GetName(),name) ) 
         return(var);
   }
   return(0);
}
//_____________________________________________________________________________

Int_t NucDBDataPoint::AddDependentVariable(NucDBDependentVariable * var)
{ 

   if( ! GetDependentVariable(var->GetName()) ) {

      for(int j = 0; j<var->GetNDependentVariables(); j++) {
         NucDBBinnedVariable * v    = var->GetVariable(j);
         if(v) {
            NucDBBinnedVariable * vdat = GetBinVariable(v->GetName());
            if(!vdat) vdat = (NucDBBinnedVariable*)GetDependentVariable(v->GetName());
            if(vdat){
               var->SetVariable(j,vdat);
            } else {
               Error("CalculateVariable","Datapoint has no variable %s for calculating %s", v->GetName(), var->GetName());
               return -3;
            }
         } else {
            Error("CalculateVariable","Could not find concrete DV class variable");
            return -2;
         }
      }
      //AddVariable(var);
      AddBinVariable(var);
   } else {
      Error("AddDependentVariable","variable, %s, already exists",var->GetName());
      return -1;
   }
   return 0;
}
//_____________________________________________________________________________

void NucDBDataPoint::CalculateDependentVariables()
{
   NucDBDependentVariable * var = 0;
   for(int i = 0;i<fBinnedVariables.GetEntries(); i++ ){
      var = dynamic_cast<NucDBDependentVariable*>(fBinnedVariables.At(i));
      if(var) {
         //std::cout << "calculating" << std::endl;
         var->Calculate();
      }
   }
}
//_____________________________________________________________________________

void  NucDBDataPoint::ListVariables()
{
   std::cout << "Variables: \n";
   for(int i = 0;i<fVariables.GetEntries();i++) {
      std::cout << " - " << fVariables.At(i)->GetName() << "\n";
   }
   for(int i = 0;i<fBinnedVariables.GetEntries();i++) {
      std::cout << " - " << fBinnedVariables.At(i)->GetName() << "\n";
   }
}
//_____________________________________________________________________________

