#ifndef NucDBDataPoint_HH
#define NucDBDataPoint_HH 1

#include "TNamed.h"
#include "TObject.h"
#include "TList.h"
#include <iostream>
#include "NucDBUnits.h"

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


   Int_t        GetValue() const {return(fValue);}
   void         Setvalue(Int_t v){fValue = v;} // *MENU* 
   void         Print(); // *MENU*
   void         SetUnit(NucDBUnit * u) { fUnit = *u; }
   NucDBUnit *  GetUnit(){return &fUnit;}

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

   /** Compares two binned variables returns true if they overlap */
   bool operator==(const NucDBBinnedVariable &other) const {return(BinsOverlap(other));}
   bool operator!=(const NucDBBinnedVariable &other) const { return !(*this == other);}

   Double_t  GetBinMinimum() const {return(fMinimum);}
   void      SetBinMinimum(Double_t val)  {fMinimum =val;}
   Double_t  GetBinMaximum() const {return(fMaximum);}
   void      SetBinMaximum(Double_t val) {fMaximum =val;}
   Double_t  GetMinimum() const {return(fMinimum);}
   Double_t  GetMaximum() const {return(fMaximum);}

   Double_t  GetMean() const {return(fMean);}
   void      SetMean(Double_t val) {fMean =val;}
   Double_t  GetAverage() const {return(fAverage);}
   void      SetAverage(Double_t val) {fAverage =val;}

   void      SetBinValueSize(Double_t val, Double_t size);
   void      Print() ; // *MENU*

   void SetUnit(NucDBUnit * u) { fUnit = *u; }
   NucDBUnit * GetUnit(){return &fUnit;}

ClassDef(NucDBBinnedVariable,3)
};




/** Base class for an error bar.
 * 
 */
class NucDBErrorBar : public TObject {
protected:
   /// total = plus + minus
   Double_t fTotalError;
   Double_t fErrorPlus;
   Double_t fErrorMinus;

public:
   NucDBErrorBar();
   virtual ~NucDBErrorBar();

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
   Double_t GetError() { return(fTotalError/2.0); }

   /** returns errorPlus + errorMinus */
   Double_t GetTotalError() const { return(fTotalError);}
   Double_t GetMinusError() const { return(fErrorMinus);}
   Double_t GetPlusError() const { return(fErrorPlus);}
   void     Clear();
   void     Print() const ; // *MENU*


ClassDef(NucDBErrorBar,1)
};


/** Base class for a data point
 */
class NucDBDataPoint : public TObject {
public :
   NucDBDataPoint(Double_t val=0.0, Double_t err=0.0) {
      fUnit = 0;
      fValue = val;
      fDimension=1;
      fDiscreteVariables.Clear();
      fVariables.Clear();
      fBinnedVariables.Clear();
      fName=" ";
      fTotalError.Clear();
      fSystematicError.Clear();
      fStatisticalError.Clear();

   }
   ~NucDBDataPoint(){}

   Double_t GetValue(){ return fValue ; }
   void     SetValue(Double_t v) { fValue = v; }
//protected:
   Double_t fValue;

   NucDBErrorBar fStatisticalError;
   NucDBErrorBar fSystematicError;
   NucDBErrorBar fTotalError;

public:
   NucDBErrorBar * GetStatError(){ return(&fStatisticalError); }   
   NucDBErrorBar * GetSystError(){ return(&fSystematicError); }   

   /** Set the values of the total using the current
    *  systematic and statistical errors
    */
   void CalculateTotalError(){
      Double_t sys = fSystematicError.GetError();
      Double_t stat = fStatisticalError.GetError();
      fTotalError.SetError(sys+stat);
   }

   TList fVariables;
   TList fDiscreteVariables;
   TList fBinnedVariables;
   Int_t fDimension;

   TString fName;

   void Print(){
      std::cout << fName.Data() << " = " << fValue << " +- " << fTotalError.GetError() << "\n";
      for(int i=0; i<fBinnedVariables.GetEntries();i++) {
         ((NucDBBinnedVariable*)fBinnedVariables.At(i))->Print(); 
         
      }
   }
   
   NucDBBinnedVariable* GetBinVariable(const char * name) {
      for(int i = 0;i<fBinnedVariables.GetEntries();i++) {
          if( !strcmp( ((NucDBBinnedVariable*)fBinnedVariables.At(i))->GetName(),name) ) 
             return((NucDBBinnedVariable*)fBinnedVariables.At(i));
      }
      return(0);
   }

   void AddBinVariable(NucDBBinnedVariable * var) { 
      if( ! GetBinVariable(var->GetName()) ) {
         fBinnedVariables.Add(var);
      } else {
         printf(" variable, %s, already exists",var->GetName());
      }

      
   } 

   void SetUnit(NucDBUnit * u) { fUnit =u; }
   NucDBUnit * GetUnit(){return fUnit;}
   NucDBUnit * fUnit;//! 


ClassDef(NucDBDataPoint,2)
};


#endif

