#ifndef NucDBDependentVariable_HH
#define NucDBDependentVariable_HH 1

#include "NucDBDataPoint.h"
#include "TF1.h"


/** A binned variable which is dependent upon other variable.
 *  For example \f$ W^2(x,Q^2) \f$ would require two binned variables, \f$ x \f$ and \f$ Q^2 \f$.
 *  In addition to the input bins, a function needs to be set with SetFunction(TF1*). The function
 *  should calculate the value as function of the two variables.  
 *  Use SetVariable to set the ordering of the TF1::EvalPar(double*x,double*p) varibale arguments. 
 *  Note that no parameters are explicity used and that this is up to how they are used in the TF1 constructor. 
 *  See examples/depend_variable.cxx   
 */
class NucDBDependentVariable : public NucDBBinnedVariable {
public :
   NucDBDependentVariable(const char * n ="depVar",const char * t = "dep var"):NucDBBinnedVariable(n,t){
      fNDepVars=2;
      fFunction=0;
      for(int i =0;i<4;i++){
         x[i]=0;p[i]=0;fVars[i]=0;x_low[i]=0;x_high[i]=0;
      }
   }
   ~NucDBDependentVariable(){}

   void SetVariable(Int_t n, NucDBBinnedVariable * avar ){
      if(n>=fNDepVars){ printf(" variable index is too large ! \n"); }
      else {
         fVars[n] = avar;
	 x[n]=avar->GetMean();
	 x_low[n]=avar->GetBinMinimum();
	 x_high[n]=avar->GetBinMaximum();
      }
   }

   TF1 * fFunction;

   Double_t GetValue(){
      if(!fFunction) {
         printf("null function\n");
         return (0);
      } else {
	 SetBinMinimum(fFunction->EvalPar(x_low,p));
	 SetBinMaximum(fFunction->EvalPar(x_high,p));
         fMean = fFunction->EvalPar(x,p);
	 fAverage=GetMean();
      }
      return(fAverage);
   }

   void SetFunction(TF1 * f) { fFunction = f;}
   TF1 * GetFunction(){ return fFunction; }

protected:
   Int_t fNDepVars;
   Double_t x[4];
   Double_t x_low[4];
   Double_t x_high[4];
   NucDBBinnedVariable * fVars[4];
   Double_t p[4];


ClassDef(NucDBDependentVariable,1)
};

#endif



