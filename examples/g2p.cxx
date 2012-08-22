/** First test of binned variable selection */
Int_t g2p() {

   gSystem->Load( "libNucDB" );
   TCanvas * c = new TCanvas();

   NucDBManager * manager = NucDBManager::GetManager();

   NucDBBinnedVariable * Qsq = new NucDBBinnedVariable("Qsquared","Q^{2} 3.0<Q^{2}<4.0");
   Qsq->SetBinMinimum(3.0);
   Qsq->SetBinMaximum(4.0);
   NucDBBinnedVariable * Qsq2 = new NucDBBinnedVariable("Qsquared","Q^{2} 4.0<Q^{2}<5.0");
   Qsq2->SetBinMinimum(4.0);
   Qsq2->SetBinMaximum(5.0);

   TList * g2p_meas = manager->GetMeasurements("g2p");

   for(int i = 0;i<g2p_meas->GetEntries() ;i++){
      NucDBMeasurement * g2p = (NucDBMeasurement*)g2p_meas->At(i);
      g2p->BuildGraph();
      if(i==0)g2p->fGraph->Draw("a,p,same");
      else g2p->fGraph->Draw("p");
   }
/*
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
*/
c->SaveAs("examples/F2p.png");


return(0);
}
