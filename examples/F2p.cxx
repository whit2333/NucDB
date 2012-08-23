/** First test of binned variable selection */
Int_t F2p()
{

   gSystem->Load("libNucDB");
   TCanvas * c = new TCanvas();

   NucDBManager * manager = NucDBManager::GetManager();

   NucDBBinnedVariable * Qsq = new NucDBBinnedVariable("Qsquared", "Q^{2} 3.0<Q^{2}<4.0");
   Qsq->SetBinMinimum(3.0);
   Qsq->SetBinMaximum(4.0);
   NucDBBinnedVariable * Qsq2 = new NucDBBinnedVariable("Qsquared", "Q^{2} 4.0<Q^{2}<5.0");
   Qsq2->SetBinMinimum(4.0);
   Qsq2->SetBinMaximum(5.0);

   NucDBMeasurement * F2p = manager->GetExperiment("OLDSLAC")->GetMeasurement("F2p");
   NucDBMeasurement * F2pQsq1 = new NucDBMeasurement("F2pQsq1", Form("F_{2}^{p} %s", Qsq->GetTitle()));
   NucDBMeasurement * F2pQsq2 = new NucDBMeasurement("F2pQsq2", Form("F_{2}^{p} %s", Qsq2->GetTitle()));

   F2p->BuildGraph();
   F2p->fGraph->Draw("ap");

   F2pQsq1->AddDataPoints(F2p->FilterWithBin(Qsq));
   F2pQsq1->BuildGraph();
   F2pQsq1->fGraph->SetMarkerColor(4);
   F2pQsq1->fGraph->SetLineColor(4);
   F2pQsq1->fGraph->Draw("p");

   F2pQsq2->AddDataPoints(F2p->FilterWithBin(Qsq2));
   F2pQsq2->BuildGraph();
   F2pQsq2->fGraph->SetMarkerColor(2);
   F2pQsq2->fGraph->SetLineColor(2);
   F2pQsq2->fGraph->Draw("p");

   c->SaveAs("examples/F2p.png");


   return(0);
}