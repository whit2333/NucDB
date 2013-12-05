#ifndef NucDBFuncs_HH
#define NucDBFuncs_HH 1

#ifndef Mproton
#define Mproton 0.938272046
#endif 
#include "TComplex.h"

#include <cmath>

namespace NucDB{
   namespace Kine{

        /** Bjorken scaling variable x */
        double xBjorken_Etheta(double Ebeam,double Eprime,double th, double M=Mproton);

        /** Bjorken scaling variable x */
        double xBjorken_Q2nu(double Q2,double nu, double M=Mproton);

        double xBjorken_WQsq(double W, double Qsq, double Mtarg=Mproton);
        double xBjorken_WQsq_proton(double W, double Qsq);

	/** Qsquared   */
	double Qsquared_Wx(double W, double x, double M = Mproton);

        /**  */	
        double Qsquared_Etheta(double Ebeam, double Eprime, double th );

        /** Invariant Mass squared as a function of Q2 and nu 
	 *  \f$ W^2(Q^2,\nu)  \f$   */	
        double W2_Q2nu(double Q2, double nu, double M = Mproton );

        /** Invariant Mass as a function of Q2 and nu */	
        double W_Q2nu(double Q2, double nu, double M = Mproton );

        /** Invariant Mass as a function of Q2 and x */	
        double W2_xQ2(double x, double Q2, double M = Mproton );

        /** Invariant Mass as a function of Q2 and x */	
        double W_xQ2(double x, double Q2, double M = Mproton );

        double W_xQ2_proton(double x, double Q2);

        /** Nachman scaling variable \f$ \xi \f$ */
        double xiNachman_xQ2(double x, double Q2, double M=Mproton);

        double nu_xQ2(double x, double Q2, double M=Mproton  );
        double nu_xQ2_proton(double x, double Q2);


   } // end Kine namespace
} // end NucDB namespace


#endif

