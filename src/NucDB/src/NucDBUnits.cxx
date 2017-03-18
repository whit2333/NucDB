#include "NucDBUnits.h"

NucDBUnit::NucDBUnit(const char * name,const char * title)
      :TNamed(name,title),
      fhbarc2(0.38939129),
      fhbarc(0.197),
      fhbar(6.58e-25),
      fSpeedOfLight(2.99792458e23)
      {
        fConversionFactor=1.0;
      }
//______________________________________________________________________________

NucDBUnit::~NucDBUnit()
= default;
//______________________________________________________________________________

NucDBUnit& NucDBUnit::operator*=(const NucDBUnit& rhs)
{
  TString name(Form("%s%s",this->GetName(),rhs.GetName()));
  TString title(Form("%s %s",this->GetTitle(),rhs.GetTitle()));
  //NucDBUnit combined(name.Data(),title.Data());
  this->SetNameTitle(name.Data(),title.Data());
  Double_t factor = this->GetConversionFactor();
  factor *=  rhs.GetConversionFactor() ;
  this->SetConversionFactor( factor );
  return *this;
}
//______________________________________________________________________________









