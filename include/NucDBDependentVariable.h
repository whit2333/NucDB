#ifndef NucDBDependentVariable_HH
#define NucDBDependentVariable_HH 1

#include "TF1.h"
#include "NucDBFuncs.h"
#include "NucDBBinnedVariable.h"
#include "NucDBUnits.h"

/** A binned variable which is dependent upon other variable.
 *  For example \f$ W^2(x,Q^2) \f$ would require two binned variables, \f$ x \f$ and \f$ Q^2 \f$.
 *  
 *  The Binned variables
 *  
 *  In addition to the input bins, a function needs to be set with SetFunction(TF1*). The function
 *  should calculate the value as function of the two variables.
 *  
 *  Use SetVariable to set the ordering of the TF1::EvalPar(double*x,double*p) varibale arguments. 
 *  Note that no parameters are explicity used and that this is up to how they are used in the TF1 constructor. 
 *  See examples/depend_variable.cxx   
 */
class NucDBDependentVariable : public NucDBBinnedVariable {
private:
   /// This might be a bit redundant because it will stream these binnedvariables again..
   TList                 fDependentVariables;
   TList                 fDependentVariableNames;

protected:
   Int_t                 fNDepVars;
   Double_t              x[4];      //->
   Double_t              x_low[4];  //->
   Double_t              x_high[4]; //->
   NucDBBinnedVariable * fVars[4];  //->
   Double_t              p[4];      //->
   TF1 *                 fFunction; //!


public :
   NucDBDependentVariable(const char * n ="depVar",const char * t = "dep var");
   ~NucDBDependentVariable();

   void                  SetVariable(Int_t n, NucDBBinnedVariable * avar );
   NucDBBinnedVariable * GetVariable(Int_t n );
   Double_t              GetValue();
   Double_t              FindMaximum();
   Double_t              FindMinimum();
   void                  Calculate(); // *MENU* 
   void                  SetNDependentVariables(Int_t n){ fNDepVars = n;}
   Int_t                 GetNDependentVariables(){ return(fNDepVars);}
   void                  SetFunction(TF1 * f) { fFunction = f;}
   TF1 *                 GetFunction(){ return fFunction; }

   void                  SetFunc1(double (*pt2func)(double)){ fFunc1 = pt2func;}
   void                  SetFunc2(double (*pt2func)(double,double)){ fFunc2 = pt2func;}
   void                  SetFunc3(double (*pt2func)(double,double,double)){ fFunc3 = pt2func;}

   double              (*fFunc1) (double);  //->
   double              (*fFunc2) (double,double); //->
   double              (*fFunc3) (double,double,double); //->

ClassDef(NucDBDependentVariable,1)
};


/**  Concrete imp of NucDBDependentVariable for invariant mass.
 *   This is a Work around until I figure out how to handle the function pointer stream...?
 */
class NucDBInvariantMassDV  : public NucDBDependentVariable {
public:
   NucDBInvariantMassDV(const char * n = "W",const char * t = "W") : NucDBDependentVariable(n,t) {
      NucDBBinnedVariable    * x  = new NucDBBinnedVariable("x","x");
      x->SetBinValueSize(0.6,0.1);
      NucDBBinnedVariable    * Q2 = new NucDBBinnedVariable("Qsquared","Q^{2}");
      Q2->SetBinValueSize(3.0,0.5);
      SetNDependentVariables(2);
      SetVariable(0,x);
      SetVariable(1,Q2);
      SetFunc2( &NucDB::Kine::W_xQ2_proton );
   }
   ~NucDBInvariantMassDV(){}

ClassDef(NucDBInvariantMassDV,1);
};

/**  Concrete imp of NucDBDependentVariable for invariant mass.
 *   This is a Work around until I figure out how to handle the function pointer stream...?
 */
class NucDBPhotonEnergyDV  : public NucDBDependentVariable {
public:
   NucDBPhotonEnergyDV(const char * n = "nu",const char * t = "#nu") : NucDBDependentVariable(n,t) {
      NucDBBinnedVariable    * x  = new NucDBBinnedVariable("x","x");
      x->SetBinValueSize(0.6,0.1);
      NucDBBinnedVariable    * Q2 = new NucDBBinnedVariable("Qsquared","Q^{2}");
      Q2->SetBinValueSize(3.0,0.5);
      SetNDependentVariables(2);
      SetVariable(0,x);
      SetVariable(1,Q2);
      SetFunc2( &NucDB::Kine::nu_xQ2_proton);
   }
   ~NucDBPhotonEnergyDV(){}

ClassDef(NucDBPhotonEnergyDV,1);
};


#endif



