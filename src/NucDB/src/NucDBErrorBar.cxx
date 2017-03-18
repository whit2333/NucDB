#include "NucDBErrorBar.h"


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
NucDBErrorBar::~NucDBErrorBar()= default;
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

