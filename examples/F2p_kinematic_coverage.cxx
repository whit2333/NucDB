/*! Plots the coverage in Q^2 vs x of F2p measurments.
 
 */
Int_t F2p_kinematic_coverage(){

   NucDBManager * manager = NucDBManager::GetManager();
   TCanvas * c = new TCanvas();
   c->SetLogy(1);

   TMultiGraph * mg = manager->GetKinematicMultiGraph("F2p","x","Qsquared");
   TList * measurementsList = manager->GetMeasurements("F2p");
   TList * graphList = mg->GetListOfGraphs();


   TLegend * leg = new TLegend(0.5, 0.7, 0.9, 0.9);
   leg->SetHeader("F_{2}^{p} measurments");

   for (int i = 0; i < measurementsList->GetEntries(); i++) {
      NucDBMeasurement * ameas = (NucDBMeasurement*)measurementsList->At(i);
      TGraphErrors * agraph = (TGraphErrors*)graphList->At(i);
      //agraph->SetMarkerColor(1);
      leg->AddEntry(agraph, Form("%s F2p", ameas->GetExperimentName().Data()), "ep");
   }

   mg->Draw("a");
   mg->GetXaxis()->SetTitle("x");
   mg->GetYaxis()->SetTitle("Q^{2}");
   mg->Draw("a");
   leg->Draw();

   c->SaveAs("examples/F2p_kinematic_coverage.png");

}
