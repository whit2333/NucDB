#include "NucDBDependentVariable.h"

ClassImp(NucDBDependentVariable)
//_____________________________________________________________________________

NucDBDependentVariable::NucDBDependentVariable(const char * n ,const char * t) 
   : NucDBBinnedVariable(n,t) {
      fNDepVars = 2;
      fFunction = nullptr;
      for(int i =0;i<4;i++){
         x[i]     = 0;
         p[i]     = 0;
         //fVars[i] = 0;
         for(int j =0;j<2;j++){
            fVar_limits[i][j] = 0.0;
         }
      }
      fFunc1 = nullptr;
      fFunc2 = nullptr;
      fFunc3 = nullptr;
      //fFunc4 = 0;
   }
//_____________________________________________________________________________
NucDBDependentVariable::~NucDBDependentVariable(){
   //for(int i =0;i<fNDepVars;i++){
   //   delete fVars[i];
   //   fVars[i] = 0;
   //}
}
//_____________________________________________________________________________
NucDBDependentVariable::NucDBDependentVariable(const NucDBDependentVariable& v) : NucDBBinnedVariable(v){
   if ( this != &v) {  
      (*this) = v;
   }
}
//_____________________________________________________________________________
NucDBDependentVariable& NucDBDependentVariable::operator=(const NucDBDependentVariable& v) {
   if ( this != &v) {  
      NucDBBinnedVariable::operator=(v);
      fNDepVars = v.fNDepVars;
      for(int i =0;i<4;i++){
         x[i] = v.x[i];
         for(int j =0;j<2;j++){
            fVar_limits[i][j] = v.fVar_limits[i][j];
         }
         //if(v.fVars[i]) fVars[i] = new NucDBBinnedVariable(*(v.fVars[i]));
      }
      TList * alist      = nullptr;
      alist              = (TList*)v.fVariables.Clone();
      fVariables.AddAll(alist);
      alist              = (TList*)v.fDependentVariableNames.Clone();
      fDependentVariableNames.AddAll(alist);
      fFunc1 = v.fFunc1;
      fFunc2 = v.fFunc2;
      fFunc3 = v.fFunc3;
      //fFunc4 = fFunc4;

   }
   return *this;    // Return ref for multiple assignment
}
//_____________________________________________________________________________

void NucDBDependentVariable::SetVariable(Int_t n, NucDBBinnedVariable * avar ){
   if(n>=fNDepVars || n>=4){ 
      Error("SetVariable","variable index is too large ! "); 
   } else {
      if( n < fVariables.GetEntries() ) {
         fVariables.RemoveAt(n);
         fVariables.AddAt(avar,n);
      } else {
         fVariables.Add(avar);
      }
      // holders for values
      //fVars[n] = avar;
      x[n]     = avar->GetMean();
      fVar_limits[n][0] = avar->GetBinMinimum();
      fVar_limits[n][1] = avar->GetBinMaximum();
   }
}
//_____________________________________________________________________________

NucDBBinnedVariable * NucDBDependentVariable::GetVariable(Int_t n ){
   NucDBBinnedVariable *var = nullptr;
   if( n >= fVariables.GetEntries() ) {
      Error("GetVariable","variable index number is too big");
      return(nullptr);
   }
   var = (NucDBBinnedVariable *) fVariables.At(n);
   return(var);
}
//_____________________________________________________________________________

Double_t NucDBDependentVariable::GetValue(){
   if(fVariables.GetEntries() < fNDepVars) {
      Error("GetValue","not enough variables to calculate value");
      return(0.0);
   }
   Double_t res = 0.0;
   if(fNDepVars ==1 && fFunc1 != nullptr ) {
      res = (*fFunc1)(x[0]);
   }
   if(fNDepVars ==2 && fFunc2 != nullptr ) {
      //std::cout << "x[0],x[1] = " << x[0] << "," << x[1] << std::endl;
      res =  (*fFunc2)(x[0],x[1]) ;
   }
   if(fNDepVars ==3 && fFunc3 != nullptr ) {
      //std::cout << "x[] = " << x[0] << "," << x[1]<< "," << x[2] << std::endl;
      res = (*fFunc3)(x[0],x[1],x[2]);
   }
   //std::cout << res << std::endl;
   fAverage = res;
   fMean = res;
//       if(!fFunction) {
//          printf("null function\n");
//          return (0);
//       } else {
// 	 SetBinMinimum(fFunction->EvalPar(x_low,p));
// 	 SetBinMaximum(fFunction->EvalPar(x_high,p));
//          fMean = fFunction->EvalPar(x,p);
// 	 fAverage=GetMean();
//       }
   return(res);
}
//_____________________________________________________________________________

void NucDBDependentVariable::Calculate(){
   if( fVariables.GetEntries() >= fNDepVars ) {
      SetMean(GetValue());
      SetBinMaximum( GetMean()+0.001);//FindMaximum() );
      SetBinMinimum( GetMean()-0.001);//FindMinimum() );
      SetAverage( GetMean() );//(GetBinMaximum()+GetBinMinimum())/2.0 );
   } else {
      Warning("Calculate","Failed to calculate variable %s",GetName());
      SetBinValueSize(0.0,0.1);
   }
   //Print();
}
//_____________________________________________________________________________
Double_t   NucDBDependentVariable::FindMaximum(){
   Double_t res  = fMean;
   if(fVariables.GetEntries() < fNDepVars) {
      Error("FindMaximum","not enough variables to calculate value");
      return(res);
   }

   Double_t temp = res;

   // 1 Dim
   for(int i1 = 0; i1 <2;i1++){ 
      if( fNDepVars == 1 && fFunc1 != nullptr ) {
         temp = (*fFunc1)(fVar_limits[0][i1]);
         if( temp > res ) res = temp;
      } else {

         // 2 Dim
         for(int i2 = 0; i2 <2;i2++) { 
            if( fNDepVars ==2 && fFunc2 != nullptr ) {
               temp = (*fFunc2)(fVar_limits[0][i1],fVar_limits[1][i2]);
               if( temp > res ) res = temp;
            } else {

               // 3 dim
               for(int i3 = 0; i3 <2;i3++){ 
                  if(fNDepVars ==3 && fFunc3 != nullptr ) {
                     temp = (*fFunc3)(fVar_limits[0][i1],fVar_limits[1][i2],fVar_limits[2][i3]);
                     if( temp > res ) res = temp;
                  } else {

                     // 4 Dim ... and that is it
                     //for(int i4 = 0; i4 <2;i4++){ 
                     //   if(fNDepVars ==4 && fFunc4 != 0 ) {
                     //      temp = (*fFunc4)(fVar_limits[0][i1],fVar_limits[1][i2],fVar_limits[2][i3],fVar_limits[3][i4]);
                     //      if( temp > res ) res = temp;
                     //   }
                     //} //i4
                  }
               } //i3
            }
         } //i2
      }
   } // i1

   return(res);
}
//_____________________________________________________________________________
Double_t   NucDBDependentVariable::FindMinimum(){
   Double_t res  = fMean;
   if(fVariables.GetEntries() < fNDepVars) {
      Error("FindMinimum","not enough variables to calculate value");
      return(res);
   }

   Double_t temp = res;

   // 1 Dim
   for(int i1 = 0; i1 <2;i1++){ 
      if( fNDepVars == 1 && fFunc1 != nullptr ) {
         temp = (*fFunc1)(fVar_limits[0][i1]);
         if( temp < res ) res = temp;
      } else {

         // 2 Dim
         for(int i2 = 0; i2 <2;i2++) { 
            if( fNDepVars ==2 && fFunc2 != nullptr ) {
               temp = (*fFunc2)(fVar_limits[0][i1],fVar_limits[1][i2]);
               if( temp < res ) res = temp;
            } else {

               // 3 dim
               for(int i3 = 0; i3 <2;i3++){ 
                  if(fNDepVars ==3 && fFunc3 != nullptr ) {
                     temp = (*fFunc3)(fVar_limits[0][i1],fVar_limits[1][i2],fVar_limits[2][i3]);
                     if( temp < res ) res = temp;
                  } else {

                     // 4 Dim ... and that is it
                     //for(int i4 = 0; i4 <2;i4++){ 
                     //   if(fNDepVars ==4 && fFunc4 != 0 ) {
                     //      temp = (*fFunc4)(fVar_limits[0][i1],fVar_limits[1][i2],fVar_limits[2][i3],fVar_limits[3][i4]);
                     //      if( temp < res ) res = temp;
                     //   }
                     //} //i4
                  }
               } //i3
            }
         } //i2
      }
   } // i1

   return(res);
}
//______________________________________________________________________________



ClassImp(NucDBInvariantMassDV)

//_____________________________________________________________________________
