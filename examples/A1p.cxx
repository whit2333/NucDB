Int_t A1p()
{

   gSystem->Load("libNucDB");

   TCanvas * c = new TCanvas();
   NucDBManager * manager = NucDBManager::GetManager();

   TLegend * leg = new TLegend(0.1, 0.7, 0.48, 0.9);
   leg->SetHeader("The Legend Title");

   TList * measurementsList = manager->GetMeasurements("A1p");
   for (int i = 0; i < measurementsList->GetEntries(); i++) {
      NucDBMeasurement * g1p = (NucDBMeasurement*)measurementsList->At(i);
      g1p->BuildGraph();
      leg->AddEntry(g1p->fGraph, Form("%s A1p", g1p->fExperiment.Data()), "ep");
      if (i == 0)g1p->fGraph->Draw("ap");
      g1p->fGraph->Draw("p");
   }

   leg->Draw();

   c->SaveAs("examples/A1p.png");

   return(0);
}