/*! Plots the coverage in Q^2 vs x of F2p measurments.
 
 */
Int_t g2p_kinematic_coverage(){

   NucDBManager * manager = NucDBManager::GetManager();
   TCanvas * c = new TCanvas();
   c->SetLogy(1);

   TMultiGraph * mg = manager->GetKinematicMultiGraph("g2p","x","Qsquared");
   TList * measurementsList = manager->GetMeasurements("g2p");
   TList * graphList = mg->GetListOfGraphs();

   mg->Draw("a");
   mg->GetXaxis()->SetTitle("x");
   mg->GetYaxis()->SetTitle("Q^{2}");
   mg->Draw("a");

   TLegend * leg = new TLegend(0.7, 0.75, 0.95, 0.95);
   leg->SetHeader("g_{2}^{p} measurments");

   for (int i = 0; i < measurementsList->GetEntries(); i++) {
      NucDBMeasurement * ameas = (NucDBMeasurement*)measurementsList->At(i);
      TGraphErrors * agraph = (TGraphErrors*)graphList->At(i);
      leg->AddEntry(agraph, Form("%s g2p", ameas->GetExperimentName().Data()), "ep");
   }

   leg->Draw();

   c->SaveAs("examples/g2p_kinematic_coverage.png");

}