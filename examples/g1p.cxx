Int_t g1p()
{

   NucDBManager * manager = NucDBManager::GetManager();

   TCanvas * c = new TCanvas();
   c->Divide(2, 2);

//    NucDBBinnedVariable * Qsq = new NucDBBinnedVariable("Qsquared","Q^{2} 3.0<Q^{2}<4.0");
//    NucDBMeasurement * s = new NucDBMeasurement("F2pQsq1",Form("F_{2}^{p} %s",Qsq->GetTitle()));
//    Qsq->SetBinMinimum(3.0);
//    Qsq->SetBinMaximum(4.0);

   /// Get the g1p measurments from all experiments.
   TList *   g1p_measurements = manager->GetMeasurements("g1p");

   TGraphErrors *     aGraph  = 0;
   NucDBMeasurement * aMeas   = 0;
   TMultiGraph *      mg      = 0; 
   c->cd(1);

   TString variable = "x";
   mg = new TMultiGraph;
   mg->SetTitle("g_{1}^{p} ; x; ");
   for (int i = 0;  i < g1p_measurements->GetEntries(); i++) {
      aMeas  = (NucDBMeasurement*)(g1p_measurements->At(i));
      aGraph = aMeas->BuildGraph(variable.Data());
      mg->Add(aGraph,"ep");
   }
   
   mg->Draw("a");

   c->cd(2);
   TString variable = "Qsquared";
   mg = new TMultiGraph;
   mg->SetTitle("g_{1}^{p} ; Q^{2}; ");
   for (int i = 0 ; i < g1p_measurements->GetEntries(); i++) {
      aMeas = (NucDBMeasurement*)g1p_measurements->At(i);
      aGraph = aMeas->BuildGraph(variable.Data());
      mg->Add(aGraph,"ep");
   }
   mg->Draw("a");

   c->cd(3);
   TString variable = "W";
   mg = new TMultiGraph;
   mg->SetTitle("g_{1}^{p} ; W; ");
   for (int i = 0; i < g1p_measurements->GetEntries(); i++) {
      aMeas = (NucDBMeasurement*)g1p_measurements->At(i);
      aGraph = aMeas->BuildGraph(variable.Data());
      mg->Add(aGraph,"ep");
   }
   mg->Draw("a");

   c->cd(4);
   TString variable = "nu";
   mg = new TMultiGraph;
   mg->SetTitle("g_{1}^{p} ; #nu; ");
   for (int i = 0; i < g1p_measurements->GetEntries(); i++) {
      aMeas = (NucDBMeasurement*)g1p_measurements->At(i);
      aGraph = aMeas->BuildGraph(variable.Data());
      mg->Add(aGraph,"ep");
   }
   mg->Draw("a");

   c->SaveAs("examples/g1p.png");

   return(0);
}
