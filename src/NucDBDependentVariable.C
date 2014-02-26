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
NucDBDependentVariable::~NucDBDependentVariable(){
}
//_____________________________________________________________________________
NucDBDependentVariable::NucDBDependentVariable(const NucDBDependentVariable& v) : NucDBBinnedVariable(v){
   (*this) = v;
}
//_____________________________________________________________________________
NucDBDependentVariable& NucDBDependentVariable::operator=(const NucDBDependentVariable& v) {
   if ( this != &v) {  
      NucDBBinnedVariable::operator=(v);
      fNDepVars = v.fNDepVars;
      for(int i =0;i<4;i++){
         x[i]      = v.x[i]     ;
         p[i]      = v.p[i]     ;
         fVars[i]  = v.fVars[i] ;
         x_low[i]  = v.x_low[i] ;
         x_high[i] = v.x_high[i];
      }
      TList * alist      = 0;
      alist              = (TList*)v.fDependentVariables.Clone();
      fDependentVariables.AddAll(alist);
      alist              = (TList*)v.fDependentVariableNames.Clone();
      fDependentVariableNames.AddAll(alist);

   }
   return *this;    // Return ref for multiple assignment
}
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
      SetMean(GetValue());
      SetBinMaximum(FindMaximum());
      SetBinMinimum(FindMinimum());
      SetAverage( (GetBinMaximum()+GetBinMinimum())/2.0 );

   } else {
      SetBinValueSize(0.0,0.1);
   }
/*   Print();*/
}
//_____________________________________________________________________________

Double_t   NucDBDependentVariable::FindMaximum(){
   Double_t res  = -99999;
   if(fDependentVariables.GetEntries() < fNDepVars) {
      Error("GetValue","not enough variables to calculate value");
      return(0.0);
   }
   if(fNDepVars ==2 && fFunc2 != 0 ) {
      Double_t temp = 0;
      temp = (*fFunc2)(x_low[0],x_low[1]);
      res = temp;
      if( temp > res ) res = temp;
      temp = (*fFunc2)(x_high[0],x_high[1]);
      if( temp > res ) res = temp;
      temp = (*fFunc2)(x_low[0],x_high[1]);
      if( temp > res ) res = temp;
      temp = (*fFunc2)(x_high[0],x_low[1]);
      if( temp > res ) res = temp;
   }
   return(res);
}
//_____________________________________________________________________________

Double_t   NucDBDependentVariable::FindMinimum(){
   Double_t res  = 99999;
   if(fDependentVariables.GetEntries() < fNDepVars) {
      Error("GetValue","not enough variables to calculate value");
      return(0.0);
   }
   if(fNDepVars ==2 && fFunc2 != 0 ) {
      Double_t temp = 0;
      temp = (*fFunc2)(x_low[0],x_low[1]);
      res = temp;
      if( temp < res ) res = temp;
      temp = (*fFunc2)(x_high[0],x_high[1]);
      if( temp < res ) res = temp;
      temp = (*fFunc2)(x_low[0],x_high[1]);
      if( temp < res ) res = temp;
      temp = (*fFunc2)(x_high[0],x_low[1]);
      if( temp < res ) res = temp;
   }
   return(res);
}


ClassImp(NucDBInvariantMassDV)

//_____________________________________________________________________________
