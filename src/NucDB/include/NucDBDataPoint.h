#ifndef NucDBDataPoint_HH
#define NucDBDataPoint_HH 1

#include "TNamed.h"
#include "TList.h"
#include <iostream>
#include "NucDBUnits.h"
#include "TBrowser.h"
#include "NucDBDependentVariable.h"
#include "NucDBBinnedVariable.h"
#include "NucDBErrorBar.h"


/*! Base class for a data point.
 *
 */
class NucDBDataPoint : public TNamed {
   private:
      Int_t          fDimension;
      Int_t          fNSortVariables;
      TString        fSortingVariable;
      TString        fSortingVariable2;

   protected:
      Double_t       fValue;
      TList          fVariables;    // Variables calculated from other variables. These are also added to the binned variables list.
      TList          fDiscreteVariables;     // Discrete variables
      TList          fBinnedVariables;       // Binned variables (including dependent variables);
      NucDBErrorBar  fStatisticalError;
      NucDBErrorBar  fSystematicError;
      NucDBUnit      fUnit;
      mutable NucDBErrorBar fTotalError;

   public :
      NucDBDataPoint(Double_t val=0.0, Double_t err=0.0, Double_t syst=0.0);
      virtual ~NucDBDataPoint();

      NucDBDataPoint(const NucDBDataPoint& v) ;
      NucDBDataPoint(NucDBDataPoint&&) = default;
      //NucDBDataPoint& operator=(const NucDBDataPoint&) = default;
      NucDBDataPoint& operator=(NucDBDataPoint&&) = default;
      //virtual ~NucDBDataPoint() = default;

      NucDBDataPoint& operator=(const NucDBDataPoint& v) ;

      /** Calculates the weighted mean and sets the error to the variance of the mean. */
      const NucDBDataPoint& operator+=(const NucDBDataPoint& v) ;
      const NucDBDataPoint& operator+(const NucDBDataPoint& v) const ;

      const NucDBDataPoint& operator-=(const NucDBDataPoint& v) ;
      const NucDBDataPoint& operator-(const NucDBDataPoint& v) const ;

      bool  operator==(const NucDBDataPoint & rhs) const ; 
      bool  operator!=(const NucDBDataPoint & rhs) const ; 
      bool  operator> (const NucDBDataPoint & rhs) const ; 
      bool  operator< (const NucDBDataPoint & rhs) const ; 
      bool  operator>=(const NucDBDataPoint & rhs) const ; 
      bool  operator<=(const NucDBDataPoint & rhs) const ; 

      void  Multiply(Double_t v){
         fStatisticalError.Multiply(v);
         fSystematicError.Multiply(v);
         fTotalError.Multiply(v);
         fValue *= v;
         // todo unit?
      }

      void          SetSortingVariable(const char * v  = "x"){fSortingVariable = v;}
      const char *  GetSortingVariable() const {return fSortingVariable.Data();}
      void          SetSortingVariable2(const char * v  = "x"){fSortingVariable2 = v;}
      const char *  GetSortingVariable2() const {return fSortingVariable2.Data();}

      void  SetSortPriorities(const std::vector<std::string> & names);
      Int_t GetNSortVariables() const { return fNSortVariables; }

      NucDBBinnedVariable * GetSortPriority(int p) const;

      Bool_t  IsSortable() const { return kTRUE; }
      Int_t   Compare(const TObject *obj) const ;



      Bool_t  IsFolder() const { return kTRUE; }
      void    Browse(TBrowser* b) {
         b->Add(&fVariables,            "fVariables");
         b->Add(&fDiscreteVariables,    "fDiscreteVariables");
         b->Add(&fBinnedVariables,      "fBinnedVariables");
         b->Add(&fStatisticalError,     "StatisticalError");
         b->Add(&fSystematicError,      "SystematicError");
         b->Add(&fTotalError,           "TotalError");
         b->Add(&fUnit,"Unit");
      }

      virtual void Print(Option_t * opt = "") const ; // *MENU*

      Double_t  GetValue() const { return fValue ; }
      void      SetValue(Double_t v) { fValue = v; }

      /** These are the only ways to set the errors. */
      void  SetStatError(const NucDBErrorBar& er){fStatisticalError = er; }
      void  SetSystError(const NucDBErrorBar& er){fSystematicError = er; }

      void  SetStatError(double v){fStatisticalError.SetError(v); }
      void  SetSystError(double v){ fSystematicError.SetError(v); }

      void  SetUncertainty(double v){fStatisticalError.SetError(v); }

      const NucDBErrorBar& GetStatError() const { return(fStatisticalError); }
      const NucDBErrorBar& GetSystError() const { return(fSystematicError); }
      const NucDBErrorBar& GetTotalError() const {
         fTotalError = fSystematicError+fStatisticalError; 
         return(fTotalError);
      }

      double GetError() const { return( GetTotalError().GetError() ); }

      /** Set the values of the total using the current
       *  systematic and statistical errors
       */
      void                     CalculateTotalError();
      void                     CalculateDependentVariables(); // *MENU*

      void                     ListVariables(); // *MENU*
      NucDBBinnedVariable *    FindVariable(const char * name);
      TList *                  GetBinnedVariables() { return(&fBinnedVariables);}
      Int_t                    GetDimension() const { return(fDimension);}

      const TList&             GetBinnedVariablesRef() const { return(fBinnedVariables);}

      void                     SetUnit(NucDBUnit * u) { fUnit = *u; }
      NucDBUnit *              GetUnit(){return &fUnit;}

      Int_t                    AddDependentVariable(NucDBDependentVariable * var);
      NucDBDependentVariable * GetDependentVariable(const char * name);
      void                     AddDiscreteVariable(NucDBDiscreteVariable * var);
      NucDBDiscreteVariable *  GetDiscreteVariable(const char * name);
      void                     AddBinVariable(NucDBBinnedVariable * var);
      NucDBBinnedVariable *    GetBinVariable(const char * name) const ;
      void                     AddVariable(NucDBVariable * var);
      NucDBVariable *          GetVariable(const char * name) const ;

      ClassDef(NucDBDataPoint,4)
};


#endif

