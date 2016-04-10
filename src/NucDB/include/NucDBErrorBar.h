#ifndef NucDBErrorBar_HH
#define NucDBErrorBar_HH 1

#include "TObject.h"
#include "TMath.h"

/** Base class for an error bar.
 */
class NucDBErrorBar : public TObject {

   protected:
      /// total = plus + minus
      Double_t fTotalError;
      Double_t fErrorPlus;
      Double_t fErrorMinus;

   public:
      NucDBErrorBar(Double_t err = 0.0);
      NucDBErrorBar(Double_t errp, Double_t errm);
      virtual ~NucDBErrorBar();
      
      void Multiply(Double_t v){
         fTotalError *= v;
         fErrorPlus  *= v;
         fErrorMinus *= v;
      }

      NucDBErrorBar(const NucDBErrorBar& v) ;
      const NucDBErrorBar& operator=(const NucDBErrorBar& v) ;
      /** Adds in quadrature. */
      const NucDBErrorBar& operator+=(const NucDBErrorBar& v) ;
      const NucDBErrorBar& operator+(const NucDBErrorBar& v) const ;

      /** sets the error where value = number +- error
       */
      void  SetError(Double_t err) { SetErrorSize(err*2.0); } 

      /** error_plus = error_minus = err/2 
       */
      void SetErrorSize(Double_t err) { fTotalError=err;fErrorPlus=err/2.0;fErrorMinus=err/2.0; }

      /** Total Error = plus + minus; 
       */
      void SetErrorSize(Double_t plus, Double_t minus){fErrorPlus=plus; fErrorMinus=TMath::Abs(minus); fTotalError=fErrorPlus+fErrorMinus; }

      /** Set the error range. 
       */
      void SetErrorRange(Double_t low, Double_t high){ SetErrorRange(low,high,(high+low)/2.0);}

      /** \todo add test for low < central < high
       */
      void SetErrorRange(Double_t low, Double_t high,Double_t central) {
         fErrorMinus = central - low; // should be that central > low
         fErrorPlus =  high - central; // should be that high > central
         fTotalError = fErrorMinus+fErrorPlus;
      }

      /** returns error  where value = number +- error */
      Double_t GetError() const { return(fTotalError/2.0); }

      /** returns errorPlus + errorMinus */
      Double_t GetTotalError() const { return(fTotalError);}
      Double_t GetMinusError() const { return(fErrorMinus);}
      Double_t GetPlusError() const { return(fErrorPlus);}
      void     Clear(Option_t * opt = "");
      void     Print(Option_t * opt = "") const ; // *MENU*

      ClassDef(NucDBErrorBar,1)
};


#endif

