#include "NucDBUtil.h"
#include <iostream>

namespace NucDB {

   void ApplyFilterOnList(NucDBBinnedVariable * var, TList * list) {
      if(!var) {
         std::cout << "Error null bin variable" << std::endl;
         return;
      }
      if(!list) {
         std::cout << "Error null list" << std::endl;
         return;
      }
      for(unsigned int i = 0; i<list->GetEntries();i++) {
         NucDBMeasurement * mes = (NucDBMeasurement*)list->At(i);
         mes->ApplyFilterWithBin(var);
      }
   }

   TMultiGraph * CreateMultiGraph(TList * list, const char * var) {
      if(!list) {
         std::cout << "Error null list" << std::endl;
         return 0;
      }
      TMultiGraph * mg = new TMultiGraph();
      for(unsigned int i = 0; i<list->GetEntries();i++) {
         NucDBMeasurement * mes = (NucDBMeasurement*)list->At(i);
         TGraph * gr = mes->BuildGraph(var);
         if(gr) {
            //gr->SetMarkerColor(1+i);
            //gr->SetLineColor(  1+i);
            gr->SetMarkerStyle(20+i);
            mg->Add(gr,"p");
         }
      }
      return mg;
   }

   void FillLegend(TLegend * leg, TList * list, TMultiGraph * mg ) {
      if(!list) {
         std::cout << "Error null list" << std::endl;
         return ;
      }
      if(!leg) {
         std::cout << "Error null leg" << std::endl;
         return ;
      }

      for(unsigned int i = 0; i<list->GetEntries();i++) {
         NucDBMeasurement * mes = (NucDBMeasurement*)list->At(i);
         TGraph * gr = 0;
         if(mg) {
            gr = (TGraph*)(mg->GetListOfGraphs()->At(i));
         }
         if(!gr) {
            // just build the default graph for the style TODO: fixe this
            gr = mes->BuildGraph();
         }
         leg->AddEntry(gr,mes->GetExperimentName(),"lp");
      }
   }

}

