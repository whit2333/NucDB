#include "NucDBUtil.h"

#include "TList.h"
#include "TLegend.h"
#include "TMultiGraph.h"
#include "NucDBMeasurement.h"
#include "NucDBDataPoint.h"
#include "NucDBBinnedVariable.h"
#include "NucDBManager.h"

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

   TList * FilterMeasurements(TList * list, NucDBBinnedVariable * var) {
      if(!var) {
         std::cout << "Error null bin variable" << std::endl;
         return 0;
      }
      if(!list) {
         std::cout << "Error null list" << std::endl;
         return 0;
      }
      TList * filtered_list = new TList();
      for(unsigned int i = 0; i<list->GetEntries();i++) {
         NucDBMeasurement * mes    = (NucDBMeasurement*)list->At(i);
         NucDBMeasurement * meas2  = mes->NewMeasurementWithFilter(var);
         if( meas2->GetNDataPoints() > 0 ) {
            filtered_list->Add(meas2);
         }
      }
      return filtered_list;
   }
   //______________________________________________________________________________

   std::vector<NucDBMeasurement*> FilterMeasurements( std::vector<NucDBMeasurement*> list, NucDBBinnedVariable * var )
   {
      std::vector<NucDBMeasurement*> filtered_list;
      if(!var) {
         std::cout << "Error null bin variable" << std::endl;
         return filtered_list;
      }
      for(auto mes  : list) {
         NucDBMeasurement * meas2  = mes->NewMeasurementWithFilter(var);
         if( meas2->GetNDataPoints() > 0 ) {
            filtered_list.push_back(meas2);
         }
      }
      return filtered_list;
   }


   TMultiGraph * CreateMultiGraph(TList * list, const char * var) {
      // creates a multi graph from a list of measurments
      if(!list) {
         std::cout << "Error null list" << std::endl;
         return 0;
      }
      NucDBManager * dbman = NucDBManager::GetManager();
      TMultiGraph * mg = new TMultiGraph();
      for(unsigned int i = 0; i<list->GetEntries();i++) {
         NucDBMeasurement * mes = (NucDBMeasurement*)list->At(i);
         TGraph * gr = mes->BuildGraph(var);
         if(gr) {
            Int_t color = dbman->NextColor();
            Int_t mark  = dbman->NextMarker();
            gr->SetMarkerColor(color);
            gr->SetLineColor(  color);
            gr->SetMarkerStyle(mark);
            mg->Add(gr,"p");
         }
      }
      return mg;
   }
//______________________________________________________________________________

   TMultiGraph * CreateMultiGraph(std::vector<NucDBMeasurement*> vec, const char * var){
      // creates a multi graph from a list of measurments
      NucDBManager * dbman = NucDBManager::GetManager();
      TMultiGraph  * mg = new TMultiGraph();
      for(auto mes : vec) {
         TGraph * gr = mes->BuildGraph(var);
         if(gr) {
            Int_t color = dbman->NextColor();
            Int_t mark  = dbman->NextMarker();
            gr->SetMarkerColor(color);
            gr->SetLineColor(  color);
            gr->SetMarkerStyle(mark);
            mg->Add(gr,"p");
         }
      }
      return mg;
   }
//______________________________________________________________________________
   
   TMultiGraph * CreateMultiKineGraph(std::vector<NucDBMeasurement*> vec, const char * var, const char * var2)
   {
      // creates a multi graph from a list of measurments
      NucDBManager * dbman = NucDBManager::GetManager();
      TMultiGraph  * mg = new TMultiGraph();
      for(auto mes : vec) {
         TGraph * gr = mes->BuildKinematicGraph(var,var2);
         if(gr) {
            Int_t color = dbman->NextColor();
            Int_t mark  = dbman->NextMarker();
            gr->SetMarkerColor(color);
            gr->SetLineColor(  color);
            gr->SetMarkerStyle(mark);
            mg->Add(gr,"p");
         }
      }
      return mg;
   }
//______________________________________________________________________________

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
//______________________________________________________________________________

   void FillLegend(TLegend * leg, std::vector<NucDBMeasurement*> vec, TMultiGraph * mg ) {
      // Fills the legend with  the list of measurements associated with the multigraph
      if(!leg) {
         std::cout << "Error null leg" << std::endl;
         return ;
      }
      if(!mg) {
        std::cout << "Error null multigraph" << std::endl;
        return ;
      }

      int i = 0;
      for(auto mes : vec) {
         TGraph * gr = (TGraph*)(mg->GetListOfGraphs()->At(i));
         if(!gr) {
            // just build the default graph for the style TODO: fixe this
           std::cout << " warning shouldn't be here in NucDB::FillLegend" <<std::endl;
            gr = mes->BuildGraph();
         }
         leg->AddEntry(gr,mes->GetExperimentName(),"lp");
         i++;
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
//______________________________________________________________________________

   std::vector<NucDBMeasurement*> RemoveExperiment(const char * name, std::vector<NucDBMeasurement*> meas_list)
   {
      std::vector<NucDBMeasurement*> ret_list;
      std::string n1 = name;
      //NucDBMeasurement * ret_meas = nullptr;
      
      int i = 0;
      for(auto m : meas_list ) {

         std::string n2 = m->GetExperimentName();
         //std::size_t found = n2.find(name);
         if( n1 == n2 ) {
            //std::cout << " found " << found << std::endl;
            //ret_meas = m;
            //meas_list.erase(meas_list.begin() + i);
         } else {
            ret_list.push_back(m);
         }
         i++;
      }
      return ret_list;
   }
   //______________________________________________________________________________
   
   NucDBMeasurement * RemoveExperiment(const char * name, TList * meas_list)
   {
      NucDBMeasurement * aMeas = 0;
      if(!meas_list) {
         std::cout << "Measurement list is Null" << std::endl;
         return 0;
      }
      for(int i = 0; i<meas_list->GetEntries();i++) {
         aMeas = (NucDBMeasurement*)meas_list->At(i);
         std::string n2 = aMeas->GetExperimentName();
         std::size_t found = n2.find(name);
         if (found!=std::string::npos) {
            meas_list->Remove(aMeas);
            return aMeas;
         }
      }
      return nullptr;
   }
   //______________________________________________________________________________

   NucDBMeasurement * FindExperiment(const char * name, TList * meas_list){
      // searches for name in the experinment name measurement
      // returns the first match
      NucDBMeasurement * aMeas = 0;
      if(!meas_list) {
         std::cout << "Measurement list is Null" << std::endl;
         return 0;
      }
      for(int i = 0; i<meas_list->GetEntries();i++) {
         aMeas = (NucDBMeasurement*)meas_list->At(i);
         std::string n2 = aMeas->GetExperimentName();
         std::size_t found = n2.find(name);
         if (found!=std::string::npos) {
            return aMeas;
         }
      }
      return 0;
   }
//______________________________________________________________________________

   NucDBMeasurement * Merge(TList * meas_list, const char * name )
   {
      if(!meas_list) {
         std::cout << "cannot merge: measurement list is Null" << std::endl;
         return 0;
      }
      if(meas_list->GetEntries() <= 0) {
         std::cout << "cannot merge: measurement list is empty" << std::endl;
         return 0;
      }

      NucDBMeasurement * merged_meas = new NucDBMeasurement(name,name);
      for(int i = 0; i<meas_list->GetEntries();i++) {
         NucDBMeasurement * aMeas = (NucDBMeasurement*)meas_list->At(i);
         merged_meas->AddDataPoints((TList*)aMeas->GetDataPoints()->Clone());
      }
      return merged_meas;
   }
//______________________________________________________________________________

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
         NucDBDataPoint * point = dynamic_cast<NucDBDataPoint*>(list->At(i));
         if(!point) {
            std::cout << "list does not contain data points." <<std::endl;
            continue;
         }
         NucDBBinnedVariable * var2    = point->GetBinVariable(var->GetName());
         if(var2){
            if ( (*var) == (*var2) ) plist->Add(point);
         }
      }
      return plist;
   }
   //_____________________________________________________________________________
   
   bool CompareDataPoint(const NucDBDataPoint *lhs, const NucDBDataPoint *rhs)
   {
      int n = lhs->GetNSortVariables();
      for(int i = n-1; i>=0; i--) {
         NucDBBinnedVariable * var3  = lhs->GetSortPriority(i);
         NucDBBinnedVariable * var4  = rhs->GetSortPriority(i);
         if(!var3 || !var4) {
            Error("Compare","Could not find both bin variables. Check sorting variable name.");
            return false;
         }
         double b0 = var3->GetMean();
         double b1 = var4->GetMean();
         if(b0!=b1) {
            //std::cout << var3->GetName() << b0 << " < " << b1 << std::endl;
            return( b0<b1 );
         }

         //if( var3->Contains(b1) ) {
         //   //std::cout << var3->GetName() << " contains " << var4->GetName() << std::endl;
         //}
         //if( !(var3->Contains(b1)) && (var3->IsBelow(b1))  )  true;
         //std::cout << " not true"; 
         //if( !(var4->Contains(b0)) && (var4->IsBelow(b0))  )  false;
         //std::cout << " not false"; 
      }
      //lhs->Print();
      //if( (*lhs) < (*rhs) ) {
      //   std::cout << " LHS is < " << std::endl;
      //} else {
      //   std::cout << " LHS is > " << std::endl;
      //}
      //rhs->Print();

      return( (*lhs) < (*rhs) );
   }
//_____________________________________________________________________________
   TList *  StableSort( const TList & list ) { 
      std::vector<NucDBDataPoint*> templist;
      Int_t n1 = list.GetEntries();
      for(int i = 0; i < list.GetEntries();i++) {
         NucDBDataPoint * point = (NucDBDataPoint*)list.At(i);
         templist.push_back(point);
      }
      //std::sort (templist.begin(), templist.end(), CompareDataPoint );
      std::stable_sort (templist.begin(), templist.end(), CompareDataPoint );
      Int_t n2 =  templist.size();
      TList * newlist  = new TList();
      for(int i = 0; i< templist.size() ; i++ )  { 
         //templist[i]->Print();
         newlist->Add( new NucDBDataPoint( *(templist[i]))  );
      }
      if( n1 != n2 ) {
         std::cout << " ERROR sorting counts not equal! " << std::endl;
      }
      return newlist;
   }

   void Print(const std::vector<NucDBMeasurement*>& measurements)
   {
      for(auto m : measurements) {
         m->Print();
      }
   }

   std::vector<NucDBMeasurement*> SelectType(NucDB::Type type,const std::vector<NucDBMeasurement*>& list)
   {
      std::vector<NucDBMeasurement*> results;
      for(auto m : list){
         if( m->IsType(type) ) {
            results.push_back(m);
         }
      }
      return results;
   }

}

