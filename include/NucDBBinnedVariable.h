#ifndef NucDBBinnedVariable_HH
#define NucDBBinnedVariable_HH 1

#include "TNamed.h"
#include "TObject.h"
#include "TList.h"
#include <iostream>
#include "NucDBUnits.h"
#include "TBrowser.h"

/** Variable Base Class.
 * All variables have a
 *   - unit
 *   - value
 *   - name
 *   - print method
 */
class NucDBVariable : public TNamed {
   protected:
      NucDBUnit  fUnit;
      Double_t   fValue;

   public:
      NucDBVariable(const char* n = "aVariable", const char* t = "A Variable");
      virtual ~NucDBVariable();
      NucDBVariable(const NucDBVariable& v):TNamed(v){
         fUnit  = v.GetUnit();
         fValue = v.GetValue();
      }
      NucDBVariable& operator=(const NucDBVariable& v){
         TNamed::operator=(v);
         if (this != &v) {
            fUnit  = v.GetUnit();
            fValue = v.GetValue();
         }
         return *this;
      }
      bool operator==(const NucDBVariable &other) const {return(other.fValue == fValue );}
      bool operator!=(const NucDBVariable &other) const { return !(*this == other);}

      virtual void         Print(); // *MENU*

      virtual Double_t     GetValue() const {return(fValue);}
      void                 SetValue(Double_t v){fValue = v;} // *MENU*

      void                 SetUnit(const NucDBUnit& u) { fUnit = u; }
      const NucDBUnit&     GetUnit() const {return fUnit;}

      ClassDef(NucDBVariable,1)
};

/** A Discrete variable stored in a single integer value.
 *
 */
class NucDBDiscreteVariable : public TNamed {
   protected:
      NucDBUnit  fUnit;
      Int_t      fValue;

   public:
      NucDBDiscreteVariable(const char* name = "DiscreteVariable", const char* title = "A Discrete Variable") ;
      virtual ~NucDBDiscreteVariable();

      bool operator==(const NucDBDiscreteVariable &other) const {return(other.fValue == fValue );}
      bool operator!=(const NucDBDiscreteVariable &other) const { return !(*this == other);}

      NucDBDiscreteVariable(const NucDBDiscreteVariable& v):TNamed(v){
         fUnit  = v.GetUnit();
         fValue = v.GetValue();
      }

      NucDBDiscreteVariable& operator=(const NucDBDiscreteVariable& v){
         if (this != &v) {
            fUnit  = v.GetUnit();
            fValue = v.GetValue();
         }
         return *this;
      }

      void         Print(); // *MENU*

      Int_t             GetValue() const {return(fValue);}
      void              SetValue(Int_t v){fValue = v;} // *MENU* 

      void              SetUnit(const NucDBUnit& u) { fUnit = u; }
      const NucDBUnit&  GetUnit() const {return fUnit;}

      ClassDef(NucDBDiscreteVariable,2)
};



/** A binned variable, eg: Qsquared, x, W, etc.
 *
 */
class NucDBBinnedVariable : public TNamed {
   protected:
      Double_t  fMinimum;
      Double_t  fMaximum;
      Double_t  fMean;
      Double_t  fAverage;
      NucDBUnit fUnit;

   public :
      NucDBBinnedVariable(const char* name = "BinnedVariable", const char* title = "A Binned Variable", Double_t y = 1.0, Double_t dy = 0.001);
      NucDBBinnedVariable(const NucDBBinnedVariable& v) ;
      virtual ~NucDBBinnedVariable();
      NucDBBinnedVariable& operator=(const NucDBBinnedVariable& v) ;

      const NucDBBinnedVariable& operator+=(const NucDBBinnedVariable& v) ;
      const NucDBBinnedVariable& operator+( const NucDBBinnedVariable& v) const ;

      /** returns true if the bin values overlap */
      bool BinsOverlap(const NucDBBinnedVariable &var) const ;
      bool operator==(const NucDBBinnedVariable &other) const {return(BinsOverlap(other));}
      bool operator!=(const NucDBBinnedVariable &other) const { return !(*this == other);}

      Double_t    GetBinMinimum() const {return(fMinimum);}
      Double_t    GetBinMaximum() const {return(fMaximum);}
      Double_t    GetMinimum()    const {return(fMinimum);}
      Double_t    GetMaximum()    const {return(fMaximum);}
      Double_t    GetMean()       const {return(fMean);}
      Double_t    GetAverage()    const {return(fAverage);}
      void        SetBinMinimum(   Double_t val) {fMinimum =val;}
      void        SetBinMaximum(   Double_t val) {fMaximum =val;}
      void        SetMean(         Double_t val) {fMean =val;}
      void        SetAverage(      Double_t val) {fAverage =val;}
      void        SetBinValueSize( Double_t val, Double_t size);
      void        SetValueSize(    Double_t val, Double_t size){ SetBinValueSize(val,size);}
      void        SetMeanLimits(   Double_t val, Double_t min, Double_t max){ SetMean(val);SetAverage(val);SetBinMinimum(min);SetBinMaximum(max);}
      void        SetUnit(   const NucDBUnit& u) { fUnit = u; }
      void        Print() ; // *MENU*
      const NucDBUnit&   GetUnit(){return fUnit;}

      ClassDef(NucDBBinnedVariable,3)
};

#endif

