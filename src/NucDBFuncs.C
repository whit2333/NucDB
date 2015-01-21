#include "NucDBFuncs.h"
#include <iostream>


namespace NucDB{
   namespace Kine{

      double to_radians(double th){
         return( th*TMath::Pi()/180.0);
      }
      double to_degrees(double th){
         return( th*180.0/TMath::Pi());
      }
      double xBjorken_Etheta(double Ebeam, double Eprime,double th, double M) {
         return( (2*Ebeam*Eprime*TMath::Power(sin(th/2.),2))/((Ebeam - Eprime)*M) );
      }


      double xBjorken_Q2nu(double Q2,double nu, double M){
         return( Q2/(2.0*M*nu));
      }

      double xBjorken_WQsq(double W, double Qsq, double Mtarg)
      {
         return(Qsq / (W * W + Qsq - Mtarg * Mtarg));
      }

      double xBjorken_WQsq_proton(double W, double Qsq)
      {
         return xBjorken_WQsq(W,Qsq,Mproton);
      }

      double Qsquared_Wx(double W, double x, double M ){
         return((-TMath::Power(M,2) + TMath::Power(W,2))/(-1 + 1/x) );
      }


      double Qsquared_Etheta(double Ebeam, double Eprime, double th  ){
         return( 4.0*Ebeam*Eprime*TMath::Power(TMath::Sin(th/2.0),2.0) );
      }


      double W2_Q2nu(double Q2, double nu, double M  ){
         return( TMath::Power(M,2.0) + 2.0*M*nu - Q2  );
      }
      double W2_Q2nu_proton(double Q2, double nu) { return(W2_Q2nu(Q2,nu)); }

      double W_Q2nu(double Q2, double nu, double M  ){
         double W2 = W2_Q2nu(Q2,nu,M);
         if(W2<0.0) return(0.0);
         return( TMath::Sqrt( W2 ) );
      }
      double W_Q2nu_proton(double Q2, double nu) { return(W_Q2nu(Q2,nu)); }

      double W2_xQ2(double x, double Q2, double M  ){
         return( TMath::Power(M,2) - Q2 + Q2/x );
      }
      double W2_xQ2_proton(double Q2, double nu) { return(W2_xQ2(Q2,nu)); }

      double W_xQ2(double x, double Q2, double M  ){
         double W2 = W2_xQ2(x,Q2,M);
         if(W2<0.0) return(0.0);
         return(TMath::Sqrt(W2 ) );
      }
      double W_xQ2_proton(double x, double Q2){ return(W_xQ2(x,Q2)); }

      double xiNachman_xQ2(double Q2, double nu, double M){
         return( Q2/(M*nu*(1.0 + sqrt(1.0 + Q2/TMath::Power(nu,2)))) );
      }

      double nu_xQ2(double x, double Q2, double M  ){
         return( Q2/(2.0*M*x) );
      }
      double nu_xQ2_proton(double x, double Q2 ){
         return( nu_xQ2(x,Q2) );
      }


   } // end Kine namespace
} // end NucDB namespace

