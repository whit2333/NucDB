Int_t A1p()
{

   gSystem->Load("libNucDB");

   TCanvas * c = new TCanvas();
   NucDBManager * manager = NucDBManager::GetManager();

   TLegend * leg = new TLegend(0.1, 0.7, 0.48, 0.9);
   leg->SetHeader("A_{1}^{p} measurments");

   TList * measurementsList = manager->GetMeasurements("A1p");
   for (int i = 0; i < measurementsList->GetEntries(); i++) {
      NucDBMeasurement * g1p = (NucDBMeasurement*)measurementsList->At(i);
      TGraphErrors * graph = g1p->BuildGraph("x");
      //graph->SetLineColor(1);
      //graph->SetMarkerColor(1);
      leg->AddEntry(graph, Form("%s A1p", g1p->GetExperimentName().Data()), "ep");
      if (i == 0)graph->Draw("ap");
      graph->Draw("p");
   }

   //leg->Draw();

   c->SaveAs("examples/A1p.png");
   c->SaveAs("examples/A1p.pdf");

   return(0);
}
