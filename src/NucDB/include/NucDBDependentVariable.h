#ifndef NucDBDependentVariable_HH
#define NucDBDependentVariable_HH 1

#include "TF1.h"
#include "NucDBFuncs.h"
#include "NucDBBinnedVariable.h"
#include "NucDBUnits.h"
#include <vector>

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
      TList                 fVariables;
      TList                 fDependentVariableNames;

   protected:
      Int_t                 fNDepVars;
      Double_t              x[4];      //
      Double_t              fVar_limits[4][2];
      //NucDBBinnedVariable * fVars[4];  //->
      Double_t              p[4];      //
      TF1 *                 fFunction; //!


   public :
      NucDBDependentVariable(const char * n ="depVar",const char * t = "dep var");
      virtual ~NucDBDependentVariable();
      NucDBDependentVariable& operator=(const NucDBDependentVariable& v) ;
      NucDBDependentVariable(const NucDBDependentVariable& v) ;

      void                  SetVariable(Int_t n, NucDBBinnedVariable * avar );
      NucDBBinnedVariable * GetVariable(Int_t n );
      Double_t              GetValue();
      Double_t              FindMaximum();
      Double_t              FindMinimum();
      void                  Calculate(); // *MENU* 
      void                  SetNDependentVariables(Int_t n){ fNDepVars = n;}
      Int_t                 GetNDependentVariables() const { return(fNDepVars);}
      void                  SetFunction(TF1 * f) { fFunction = f;}
      TF1 *                 GetFunction() const { return fFunction; }

      void                  SetFunc1(double (*pt2func)(double)){ fFunc1 = pt2func;}
      void                  SetFunc2(double (*pt2func)(double,double)){ fFunc2 = pt2func;}
      void                  SetFunc3(double (*pt2func)(double,double,double)){ fFunc3 = pt2func;}

      double              (*fFunc1) (double);  //->
      double              (*fFunc2) (double,double); //->
      double              (*fFunc3) (double,double,double); //->

      ClassDef(NucDBDependentVariable,2)
};

class NucDBThetaDV_rad  : public NucDBDependentVariable {
   public:
      NucDBThetaDV_rad(const char * n = "theta_rad",const char * t = "#theta (rad)") : NucDBDependentVariable(n,t) {
         NucDBBinnedVariable    * th = new NucDBBinnedVariable("theta","#theta (deg)");
         SetNDependentVariables(1);
         SetVariable(0,th);
         SetFunc1( &NucDB::Kine::to_radians );
      }
      virtual ~NucDBThetaDV_rad(){}

      ClassDef(NucDBThetaDV_rad,1);
};

class NucDBThetaDV_deg  : public NucDBDependentVariable {
   public:
      NucDBThetaDV_deg(const char * n = "theta",const char * t = "#theta (deg)") : NucDBDependentVariable(n,t) {
         NucDBBinnedVariable    * th = new NucDBBinnedVariable("theta_rad","#theta (rad)");
         SetNDependentVariables(1);
         SetVariable(0,th);
         SetFunc1( &NucDB::Kine::to_radians );
      }
      virtual ~NucDBThetaDV_deg(){}

      ClassDef(NucDBThetaDV_deg,1);
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
      virtual ~NucDBInvariantMassDV(){}

      ClassDef(NucDBInvariantMassDV,1);
};

class NucDBInvariantMassDV_Q2nu  : public NucDBDependentVariable {
   public:
      NucDBInvariantMassDV_Q2nu(const char * n = "W",const char * t = "W") : NucDBDependentVariable(n,t) {
         NucDBBinnedVariable    * Q2 = new NucDBBinnedVariable("Qsquared","Q^{2}");
         NucDBBinnedVariable    * nu = new NucDBBinnedVariable("nu","nu");
         SetNDependentVariables(2);
         SetVariable(0,Q2);
         SetVariable(1,nu);
         SetFunc2( &NucDB::Kine::W_Q2nu_proton );
      }
      virtual ~NucDBInvariantMassDV_Q2nu(){}

      ClassDef(NucDBInvariantMassDV_Q2nu,1);
};

class NucDBW2DV_Q2nu  : public NucDBDependentVariable {
   public:
      NucDBW2DV_Q2nu(const char * n = "W2",const char * t = "W^{2}") : NucDBDependentVariable(n,t) {
         NucDBBinnedVariable    * Q2 = new NucDBBinnedVariable("Qsquared","Q^{2}");
         NucDBBinnedVariable    * nu = new NucDBBinnedVariable("nu","nu");
         SetNDependentVariables(2);
         SetVariable(0,Q2);
         SetVariable(1,nu);
         SetFunc2( &NucDB::Kine::W2_Q2nu_proton );
      }
      virtual ~NucDBW2DV_Q2nu(){}

      ClassDef(NucDBW2DV_Q2nu,1);
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
      virtual ~NucDBPhotonEnergyDV(){}

      ClassDef(NucDBPhotonEnergyDV,1);
};

/**  Concrete imp of NucDBDependentVariable for invariant mass.
 *   This is a Work around until I figure out how to handle the function pointer stream...?
 */
class NucDBxBjorkenDV  : public NucDBDependentVariable {
   public:
      NucDBxBjorkenDV(const char * n = "x",const char * t = "x") : NucDBDependentVariable(n,t) {
         NucDBBinnedVariable    * W  = new NucDBBinnedVariable("W","W");
         W->SetBinValueSize(1.6,0.1);
         NucDBBinnedVariable    * Q2 = new NucDBBinnedVariable("Qsquared","Q^{2}");
         Q2->SetBinValueSize(3.0,0.5);
         SetNDependentVariables(2);
         SetVariable(0,W);
         SetVariable(1,Q2);
         SetFunc2( &NucDB::Kine::xBjorken_WQsq_proton );
      }
      virtual ~NucDBxBjorkenDV(){}

      ClassDef(NucDBxBjorkenDV,2);
};

/**  Q2 as a function of E,E', and theta.
 */
class NucDBQ2DV_EEpTheta  : public NucDBDependentVariable {
   public:
      NucDBQ2DV_EEpTheta(const char * n = "Qsquared",const char * t = "Q^{2}") : NucDBDependentVariable(n,t) {
         NucDBBinnedVariable    * Ebeam  = new NucDBBinnedVariable("E","E",1.0);
         NucDBBinnedVariable    * Eprime = new NucDBBinnedVariable("Eprime","E'",0.0);
         NucDBBinnedVariable    * theta  = new NucDBBinnedVariable("theta_rad","#theta",0.1);
         SetNDependentVariables(3);
         SetVariable(0,Ebeam);
         SetVariable(1,Eprime);
         SetVariable(2,theta);
         SetFunc3( &NucDB::Kine::Qsquared_Etheta );
      }
      virtual ~NucDBQ2DV_EEpTheta(){}

      ClassDef(NucDBQ2DV_EEpTheta,2);
};


#endif



