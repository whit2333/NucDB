#include "NucDBDependentVariable.h"

ClassImp(NucDBDependentVariable)
//_____________________________________________________________________________

NucDBDependentVariable::NucDBDependentVariable(const char * n ,const char * t) 
   : NucDBBinnedVariable(n,t) {
      fNDepVars=2;
      fFunction=0;
      for(int i =0;i<4;i++){
         x[i]=0;p[i]=0;fVars[i]=0;x_low[i]=0;x_high[i]=0;
      }
      fFunc1 = 0;
      fFunc2 = 0;
      fFunc3 = 0;
   }
//_____________________________________________________________________________

NucDBDependentVariable::~NucDBDependentVariable(){}
//_____________________________________________________________________________

void NucDBDependentVariable::SetVariable(Int_t n, NucDBBinnedVariable * avar ){
      if(n>=fNDepVars || n>=4){ 
         Error("SetVariable","variable index is too large ! "); 
      } else {
         if( n < fDependentVariables.GetEntries() ) {
            fDependentVariables.RemoveAt(n);
            fDependentVariables.AddAt(avar,n);
         } else {
            fDependentVariables.Add(avar);
         }
         /// holders for values
         fVars[n] = avar;
	 x[n]=avar->GetMean();
	 x_low[n]=avar->GetBinMinimum();
	 x_high[n]=avar->GetBinMaximum();
      }
   }
//_____________________________________________________________________________

NucDBBinnedVariable * NucDBDependentVariable::GetVariable(Int_t n ){
   NucDBBinnedVariable *var = 0;
   if( n >= fDependentVariables.GetEntries() ) {
      Error("GetVariable","variable index number is too big");
      return(0);
   }
   var = (NucDBBinnedVariable *) fDependentVariables.At(n);
   return(var);
}
//_____________________________________________________________________________

Double_t NucDBDependentVariable::GetValue(){
   if(fDependentVariables.GetEntries() < fNDepVars) {
      Error("GetValue","not enough variables to calculate value");
      return(0.0);
   }
   if(fNDepVars ==2 && fFunc2 != 0 ) {
      return ( (*fFunc2)(x[0],x[1]) );
   }
//       if(!fFunction) {
//          printf("null function\n");
//          return (0);
//       } else {
// 	 SetBinMinimum(fFunction->EvalPar(x_low,p));
// 	 SetBinMaximum(fFunction->EvalPar(x_high,p));
//          fMean = fFunction->EvalPar(x,p);
// 	 fAverage=GetMean();
//       }
   return(-99999);
}
//_____________________________________________________________________________

void NucDBDependentVariable::Calculate(){
   /// \todo determine the min and max 
   if( fDependentVariables.GetEntries() >= fNDepVars ) {
      SetBinValueSize(GetValue(),0.1);
   } else {
      SetBinValueSize(0.0,0.1);
   }
   Print();
}
//_____________________________________________________________________________


ClassImp(NucDBInvariantMassDV)

//_____________________________________________________________________________
