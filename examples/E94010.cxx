Int_t E94010() {

   gSystem->Load( "libNucDB" );
   NucDBManager * manager = NucDBManager::GetManager();

   NucDBBinnedVariable * EbeamBin = new NucDBBinnedVariable("E","E^{beam} 0.5<E^{beam}<1.5");
   EbeamBin->SetBinMinimum(1.50*1000.0);
   EbeamBin->SetBinMaximum(6.5*1000.0);


   NucDBMeasurement * dsigi = manager->GetExperiment("JLAB_E94010")->GetMeasurement("sigi_He3");
   NucDBMeasurement * dsigf = manager->GetExperiment("JLAB_E94010")->GetMeasurement("sigf_He3");
   NucDBMeasurement * dsigfc = manager->GetExperiment("JLAB_E94010")->GetMeasurement("sigfc_He3");
   NucDBMeasurement * dsigi_low = new NucDBMeasurement("sigi_He3_low", Form("%s  %s" , dsigi->GetTitle(), EbeamBin->GetTitle() ));
   NucDBMeasurement * dsigf_low = new NucDBMeasurement("sigf_He3_low", Form("%s  %s" , dsigf->GetTitle(), EbeamBin->GetTitle() ));
   NucDBMeasurement * dsigfc_low = new NucDBMeasurement("sigfc_He3_low", Form("%s  %s" , dsigfc->GetTitle(), EbeamBin->GetTitle() ));

//    dsigi->BuildGraph("nu");
//    dsigi->fGraph->Draw("ap");

   dsigi_low->SetDataPoints(dsigi->FilterWithBin(EbeamBin));
   dsigi_low->BuildGraph("nu");
   dsigi_low->fGraph->SetMarkerColor(1);
   dsigi_low->fGraph->SetLineColor(1);
   dsigi_low->fGraph->Draw("ap");

   dsigf_low->SetDataPoints(dsigf->FilterWithBin(EbeamBin));
   dsigf_low->BuildGraph("nu");
   dsigf_low->fGraph->SetMarkerColor(2);
   dsigf_low->fGraph->SetLineColor(2);
   dsigf_low->fGraph->Draw("p");

   dsigfc_low->SetDataPoints(dsigfc->FilterWithBin(EbeamBin));
   dsigfc_low->BuildGraph("nu");
   dsigfc_low->fGraph->SetMarkerColor(3);
   dsigfc_low->fGraph->SetLineColor(3);
   dsigfc_low->fGraph->Draw("p");

return(0);
}