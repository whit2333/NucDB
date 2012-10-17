Int_t A1n()
{

   gSystem->Load("libNucDB");

   TCanvas * c = new TCanvas();
   NucDBManager * manager = NucDBManager::GetManager();

   TLegend * leg = new TLegend(0.1, 0.7, 0.48, 0.9);
   leg->SetHeader("The Legend Title");

   TList * measurementsList = manager->GetMeasurements("A1n");
   for (int i = 0; i < measurementsList->GetEntries(); i++) {
      NucDBMeasurement * g1p = (NucDBMeasurement*)measurementsList->At(i);
      TGraphErrors * graph = g1p->BuildGraph("x");
      leg->AddEntry(graph, Form("%s A1p", g1p->GetExperimentName().Data()), "ep");
      if (i == 0)graph->Draw("ap");
      graph->Draw("p");
   }

   leg->Draw();

   c->SaveAs("examples/A1n.png");

   return(0);
}
