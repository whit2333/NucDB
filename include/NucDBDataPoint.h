#ifndef NucDBDataPoint_HH
#define NucDBDataPoint_HH 1

#include "TNamed.h"
#include "TObject.h"
#include "TList.h"
#include <iostream>
#include "NucDBUnits.h"
#include "TBrowser.h"
#include "NucDBDependentVariable.h"
#include "NucDBBinnedVariable.h"

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
      NucDBErrorBar(const NucDBErrorBar& v) ;
      const NucDBErrorBar& operator=(const NucDBErrorBar& v) ;
      /** Adds in quadrature. */
      const NucDBErrorBar& operator+=(const NucDBErrorBar& v) ;
      /** Adds in quadrature. */
      const NucDBErrorBar& operator+(const NucDBErrorBar& v) const ;

      /** sets the error where value = number +- error
       */
      void  SetError(Double_t err) { SetErrorSize(err*2.0); } 

      /** error_plus = error_minus = err/2 
       */
      void SetErrorSize(Double_t err) { fTotalError=err;fErrorPlus=err/2.0;fErrorMinus=err/2.0; }

      /** Total Error = plus + minus; 
       */
      void SetErrorSize(Double_t plus, Double_t minus){fErrorPlus=plus; fErrorMinus=minus; fTotalError=plus+minus; }

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
      void     Clear();
      void     Print() const ; // *MENU*

      ClassDef(NucDBErrorBar,1)
};



/** Base class for a data point.
 *  
 */
class NucDBDataPoint : public TNamed {
   private:
      Int_t          fDimension;
      TString        fSortingVariable;

   protected:
      Double_t       fValue;
      TList          fVariables;
      TList          fDiscreteVariables;
      TList          fBinnedVariables;
      NucDBErrorBar  fStatisticalError;
      NucDBErrorBar  fSystematicError;
      NucDBUnit      fUnit;
      mutable NucDBErrorBar fTotalError;

   public :
      NucDBDataPoint(Double_t val=0.0, Double_t err=0.0);
      virtual ~NucDBDataPoint();
      const NucDBDataPoint& operator=(const NucDBDataPoint& v) ;

      /** Calculates the weighted mean and sets the error to the variance of the mean. */
      const NucDBDataPoint& operator+=(const NucDBDataPoint& v) ;
      const NucDBDataPoint& operator+(const NucDBDataPoint& v) const ;

      void          SetSortingVariable(const char * v  = "x"){fSortingVariable = v;}
      const char *  GetSortingVariable() const {return fSortingVariable.Data();}

      Bool_t  IsSortable() const { return kTRUE; }
      Int_t   Compare(const TObject *obj) const ;

      /** Performs deep copy. */
      NucDBDataPoint(const NucDBDataPoint& v) ;

      Bool_t  IsFolder() const { return kTRUE; }
      void    Browse(TBrowser* b) {
         b->Add(&fVariables,"fVariables");
         b->Add(&fDiscreteVariables,"fDiscreteVariables");
         b->Add(&fBinnedVariables,"fBinnedVariables");
         b->Add(&fStatisticalError,"StatisticalError");
         b->Add(&fSystematicError,"SystematicError");
         b->Add(&fTotalError,"TotalError");
         b->Add(&fUnit,"Unit");
      }

      virtual void Print() const ; // *MENU*

      Double_t  GetValue() const { return fValue ; }
      void      SetValue(Double_t v) { fValue = v; }

      /** These are the only ways to set the errors. */
      void  SetStatError(const NucDBErrorBar& er){fStatisticalError = er; }
      void  SetSystError(const NucDBErrorBar& er){fSystematicError = er; }

      const NucDBErrorBar& GetStatError() const { return(fStatisticalError); }
      const NucDBErrorBar& GetSystError() const { return(fSystematicError); }
      const NucDBErrorBar& GetTotalError() const {
         fTotalError = fSystematicError+fStatisticalError; 
         return(fTotalError);
      }

      /** Set the values of the total using the current
       *  systematic and statistical errors
       */
      void                     CalculateTotalError();
      void                     CalculateDependentVariables(); // *MENU*

      void                     ListVariables(); // *MENU*
      NucDBBinnedVariable *    FindVariable(const char * name);
      TList *                  GetBinnedVariables() { return(&fBinnedVariables);}
      Int_t                    GetDimension() const { return(fDimension);}

      void                     SetUnit(NucDBUnit * u) { fUnit = *u; }
      NucDBUnit *              GetUnit(){return &fUnit;}

      void                     AddDependentVariable(NucDBDependentVariable * var);
      NucDBDependentVariable * GetDependentVariable(const char * name);
      void                     AddDiscreteVariable(NucDBDiscreteVariable * var);
      NucDBDiscreteVariable *  GetDiscreteVariable(const char * name);
      void                     AddBinVariable(NucDBBinnedVariable * var);
      NucDBBinnedVariable *    GetBinVariable(const char * name) const ;
      void                     AddVariable(NucDBVariable * var);
      NucDBVariable *          GetVariable(const char * name) const ;

      ClassDef(NucDBDataPoint,2)
};


#endif

