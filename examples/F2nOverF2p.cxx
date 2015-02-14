Int_t F2nOverF2p(Int_t aNumber = 0) {

   NucDBManager * dbman = NucDBManager::GetManager();


   NucDBBinnedVariable * Q2_bin = new NucDBBinnedVariable("Qsquared","Q^2",500,490);

   TList * meas_list = dbman->GetMeasurements("F2n/F2p");
   //NucDB::ApplyFilterOnList(Q2_bin,meas_list);
   meas_list->Print();

   NucDBMeasurement * meas = (NucDBMeasurement*)meas_list->At(2);
   meas->MergeNeighboringDataPoints(50,"x",0.01,true);

   meas = (NucDBMeasurement*)meas_list->At(1);
   meas->MergeNeighboringDataPoints(50,"x",0.01,true);

   meas = (NucDBMeasurement*)meas_list->At(0);
   meas->MergeNeighboringDataPoints(50,"x",0.01,true);

   TMultiGraph * mg = NucDB::CreateMultiGraph(meas_list,"x");
   TLegend * leg = new TLegend(0.65,0.7,0.88,0.88);

   NucDB::FillLegend(leg,meas_list,mg);

   mg->Draw("a");
   mg->GetXaxis()->SetTitle("x");
   mg->GetXaxis()->CenterTitle(true);
   mg->GetYaxis()->SetRangeUser(0.0,1.1);
   mg->GetXaxis()->SetLimits(0.0001,1.0);
   mg->Draw("a");
   leg->Draw();

   return 0;
}
