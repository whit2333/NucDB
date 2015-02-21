#include "NucDBUtil.h"

#include "TList.h"
#include "TLegend.h"
#include "TMultiGraph.h"
#include "NucDBMeasurement.h"
#include "NucDBDataPoint.h"
#include "NucDBBinnedVariable.h"

#include <iostream>
#include <string>

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
      // creates a multi graph from a list of measurments
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
      // Fills the legend with  the list of measurements associated with the multigraph
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

   /** From the list of measurement for the experiment name.
    *  Returns the first one found.
    */
   NucDBMeasurement * GetExperiment(const char * exp_name, TList * meas_list){
      NucDBMeasurement * aMeas = 0;
      if(!meas_list) {
         std::cout << "Measurement list is Null" << std::endl;
         return 0;
      }
      for(int i = 0; i<meas_list->GetEntries();i++) {
         aMeas = (NucDBMeasurement*)meas_list->At(i);
         if( !strcmp(exp_name,aMeas->GetExperimentName()) ) {
            return aMeas;
         }
      }
      return 0;
   }

   NucDBDataPoint *  MergeDataPoints(TList * points){
      if(!points) {
         std::cout << "Null points list" << std::endl;
         return 0;
      }
      if(points->GetEntries() <= 0 ) {
         std::cout << "Empty points list" << std::endl;
         return 0;
      }

      NucDBDataPoint * mergedPoint = 0;
      for(unsigned int i = 0; i< points->GetEntries(); i++) {
         NucDBDataPoint * point = (NucDBDataPoint*)points->At(i);
         if(i == 0){
            mergedPoint = new NucDBDataPoint(*point);
         } else {
            (*mergedPoint) += (*point);
         }
      }
      return mergedPoint;
   }

//_____________________________________________________________________________
   TList * FilterDataPoints(TList * list, NucDBBinnedVariable * var){
      TList * plist = new TList();
      if(!var) {
         std::cout << "NULL NucDBVariable pointer" << std::endl;
         return list;
      }
      for(int i = 0; i < list->GetEntries();i++) {
         NucDBDataPoint * point = (NucDBDataPoint*)list->At(i);
         NucDBBinnedVariable * var2    = point->GetBinVariable(var->GetName());
         if(var2){
            if ( (*var) == (*var2) ) plist->Add(point);
         }
      }
      return plist;
   }
//_____________________________________________________________________________
   bool CompareDataPoint(const NucDBDataPoint *lhs, const NucDBDataPoint *rhs) {
      NucDBBinnedVariable * var3  = lhs->GetBinVariable(lhs->GetSortingVariable2());
      NucDBBinnedVariable * var4  = rhs->GetBinVariable(rhs->GetSortingVariable2());
      if(!var3 || !var4) {
         Error("Compare","Could not find both bin variables. Check sorting variable name.");
         return false;
      }
      double b0 = var3->GetCenter();
      double b1 = var4->GetCenter();
      if( !(var3->Contains(b1)) && (var3->IsBelow(b1))  )  true;
      if( !(var4->Contains(b0)) && (var4->IsBelow(b0))  )  false;
      //lhs->Print();
      //if( (*lhs) < (*rhs) ) {
      //   std::cout << " is less than " << std::endl;
      //} else {
      //   std::cout << " is not less than " << std::endl;
      //}
      //rhs->Print();

          return( (*lhs) < (*rhs) );
   }
//_____________________________________________________________________________
   void StableSort(TList * list) { 
      if(!list) {
         std::cout << "NULL list to sort" << std::endl;
         return;
      }
      std::vector<NucDBDataPoint*> templist;
      Int_t n1 = list->GetEntries();
      for(int i = 0; i < list->GetEntries();i++) {
         NucDBDataPoint * point = (NucDBDataPoint*)list->At(i);
         templist.push_back(point);
      }
      list->Clear();
      std::stable_sort (templist.begin(), templist.end(), CompareDataPoint );
      Int_t n2 =  templist.size();
      for(int i = 0; i< templist.size() ; i++ )  { 
         templist[i]->Print();
         list->Add( templist[i] );
      }
      if( n1 != n2 ) {
         std::cout << " ERROR sorting counts not equal! " << std::endl;
      }
   }


}

