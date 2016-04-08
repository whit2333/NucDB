/*! Plots the coverage in Q^2 vs x of g1p measurments.
 
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
   leg->SetBorderSize(1);
   //leg->SetMargin(0.3);
   leg->SetEntrySeparation(0.05);

   for (int i = 0; i < measurementsList->GetEntries(); i++) {
      NucDBMeasurement * ameas = (NucDBMeasurement*)measurementsList->At(i);
      TGraphErrors * agraph = (TGraphErrors*)graphList->At(i);
      int a_color = manager->NextColor();
      agraph->SetMarkerColor(a_color);
      agraph->SetLineColor(a_color);
      //agraph->SetFillColorAlpha(kRed,0.8);
      agraph->SetFillColorAlpha(a_color,0.8);
      //agraph->SetFillStyle(4050);
      leg->AddEntry(agraph, Form("%s g2p", ameas->GetExperimentName()), "ep");
   }

   leg->Draw();

   c->SaveAs("examples/g2p_kinematic_coverage.png");
   c->SaveAs("examples/g2p_kinematic_coverage.pdf");
   return 0;
}
