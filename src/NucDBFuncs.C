#include "NucDBFuncs.h"


namespace NucDB{
   namespace Kine{

      double xBjorken_Etheta(double Ebeam, double Eprime,double th, double M) {
         return( (2*Ebeam*Eprime*pow(sin(th/2.),2))/((Ebeam - Eprime)*M) );
      }


      double xBjorken_Q2nu(double Q2,double nu, double M){
         return( Q2/(2.0*M*nu));
      }


      double Qsquared_Wx(double W, double x, double M ){
         return((-pow(M,2) + pow(W,2))/(-1 + 1/x) );
      }


      double Qsquared_Etheta(double Ebeam, double Eprime, double th  ){
         return((4.0*Ebeam*Eprime*pow(sin(th/2.),2)) );
      }


      double W2_Q2nu(double Q2, double nu, double M  ){
         return( pow(M,2) + 2.0*M*nu - Q2  );
      }


      double W_Q2nu(double Q2, double nu, double M  ){
         return( sqrt(W2_Q2nu(Q2,nu,M) ) );
      }

      double W2_xQ2(double x, double Q2, double M  ){
         return( pow(M,2) - Q2 + Q2/x );
      }

      double W_xQ2(double x, double Q2, double M  ){
         return( W2_xQ2(x,Q2,M) );
      }

      double W_xQ2_proton(double x, double Q2){ return(W_xQ2(x,Q2)); }

      double xiNachman_xQ2(double Q2, double nu, double M){
         return( Q2/(M*nu*(1.0 + sqrt(1.0 + Q2/pow(nu,2)))) );
      }

      double nu_xQ2(double x, double Q2, double M  ){
         return( Q2/(2.0*M*x) );
      }
      double nu_xQ2_proton(double x, double Q2 ){
         return( nu_xQ2(x,Q2) );
      }


   } // end Kine namespace
} // end NucDB namespace

