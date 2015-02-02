/*! Plots the coverage in Q^2 vs x of g1p measurments.
 
 */
Int_t g1p_kinematic_coverage(){

   NucDBManager * manager = NucDBManager::GetManager();
   TCanvas * c = new TCanvas();
   c->SetLogy(1);

   TMultiGraph * mg = manager->GetKinematicMultiGraph("g1p","x","Qsquared");
   TList * measurementsList = manager->GetMeasurements("g1p");
   TList * graphList = mg->GetListOfGraphs();

   mg->Draw("a");
   mg->GetXaxis()->SetTitle("x");
   mg->GetYaxis()->SetTitle("Q^{2}");
   mg->Draw("a");

   TLegend * leg = new TLegend(0.7, 0.75, 0.95, 0.95);
   leg->SetHeader("F_{2}^{p} measurments");

   for (int i = 0; i < measurementsList->GetEntries(); i++) {
      NucDBMeasurement * ameas = (NucDBMeasurement*)measurementsList->At(i);
      TGraphErrors * agraph = (TGraphErrors*)graphList->At(i);
      //agraph->SetFillColorAlpha(kRed,0.5);
      //agraph->SetFillStyle(4050);
      leg->AddEntry(agraph, Form("%s g1p", ameas->GetExperimentName()), "ep");
   }

   leg->Draw();

   c->SaveAs("examples/g1p_kinematic_coverage.png");
   c->SaveAs("examples/g1p_kinematic_coverage.pdf");
   return 0;
}
