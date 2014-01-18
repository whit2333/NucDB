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
      NucDBVariable(const NucDBVariable& v){
         fUnit  = v.GetUnit();
         fValue = v.GetValue();
      }

      virtual void         Print(); // *MENU*

      virtual Double_t     GetValue() const {return(fValue);}
      virtual void         Setvalue(Int_t v){fValue = v;} // *MENU*

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

      NucDBDiscreteVariable(const NucDBDiscreteVariable& v){
         SetNameTitle(v.GetName(),v.GetTitle());
         fUnit  = v.GetUnit();
         fValue = v.GetValue();
      }

      NucDBDiscreteVariable& operator=(const NucDBDiscreteVariable& v){
      }

      void         Print(); // *MENU*

      Int_t             GetValue() const {return(fValue);}
      void              Setvalue(Int_t v){fValue = v;} // *MENU* 

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
      NucDBBinnedVariable(const char* name = "BinnedVariable", const char* title = "A Binned Variable");
      NucDBBinnedVariable(const NucDBBinnedVariable& v) ;
      virtual ~NucDBBinnedVariable();
      NucDBBinnedVariable& operator=(const NucDBBinnedVariable& v) ;

      /** returns true if the bin values overlap */
      bool BinsOverlap(const NucDBBinnedVariable &var) const ;
      bool operator==(const NucDBBinnedVariable &other) const {return(BinsOverlap(other));}
      bool operator!=(const NucDBBinnedVariable &other) const { return !(*this == other);}

      Double_t    GetBinMinimum() const {return(fMinimum);}
      void        SetBinMinimum(Double_t val)  {fMinimum =val;}
      Double_t    GetBinMaximum() const {return(fMaximum);}
      void        SetBinMaximum(Double_t val) {fMaximum =val;}
      Double_t    GetMinimum() const {return(fMinimum);}
      Double_t    GetMaximum() const {return(fMaximum);}
      Double_t    GetMean() const {return(fMean);}
      void        SetMean(Double_t val) {fMean =val;}
      Double_t    GetAverage() const {return(fAverage);}
      void        SetAverage(Double_t val) {fAverage =val;}
      void        SetBinValueSize(Double_t val, Double_t size);
      void        SetValueSize(Double_t val, Double_t size){ SetBinValueSize(val,size);}
      void        Print() ; // *MENU*
      void        SetUnit(const NucDBUnit& u) { fUnit = u; }
      const NucDBUnit&   GetUnit(){return fUnit;}

      ClassDef(NucDBBinnedVariable,3)
};

#endif

