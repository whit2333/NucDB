/** First test of binned variable selection */
Int_t test3() {

   gSystem->Load( "libNucDB" );
   NucDBManager * manager = NucDBManager::GetManager();

   NucDBBinnedVariable * Qsq = new NucDBBinnedVariable("Qsquared","Q^{2} 3.0<Q^{2}<4.0");
   Qsq->SetBinMinimum(3.0);
   Qsq->SetBinMaximum(4.0);
   NucDBBinnedVariable * Qsq2 = new NucDBBinnedVariable("Qsquared","Q^{2} 4.0<Q^{2}<5.0");
   Qsq2->SetBinMinimum(4.0);
   Qsq2->SetBinMaximum(5.0);

   NucDBMeasurement * F2p = manager->GetExperiment("OLDSLAC")->GetMeasurement("F2p");
   NucDBMeasurement * F2pQsq1 = new NucDBMeasurement("F2pQsq1",Form("F_{2}^{p} %s",Qsq->GetTitle()));
   NucDBMeasurement * F2pQsq2 = new NucDBMeasurement("F2pQsq2",Form("F_{2}^{p} %s",Qsq2->GetTitle()));

   TLegend * leg = new TLegend(0.1,0.7,0.48,0.9);
   leg->SetHeader("F2p");

   F2p->BuildGraph();
   F2p->fGraph->Draw("ap");

   leg->AddEntry(F2p->fGraph,Form("%s %s",F2p->fExperiment.Data(),F2p->GetTitle() ),"ep");

   F2pQsq1->AddDataPoints(F2p->FilterWithBin(Qsq));
   F2pQsq1->BuildGraph();
   F2pQsq1->fGraph->SetMarkerColor(4);
   F2pQsq1->fGraph->SetLineColor(4);
   F2pQsq1->fGraph->Draw("p");
   leg->AddEntry(F2pQsq1->fGraph,Form("%s %s",F2p->fExperiment.Data(),F2pQsq1->GetTitle() ),"ep");

   F2pQsq2->AddDataPoints(F2p->FilterWithBin(Qsq2));
   F2pQsq2->BuildGraph();
   F2pQsq2->fGraph->SetMarkerColor(2);
   F2pQsq2->fGraph->SetLineColor(2);
   F2pQsq2->fGraph->Draw("p");
   leg->AddEntry(F2pQsq2->fGraph,Form("%s %s",F2p->fExperiment.Data(),F2pQsq2->GetTitle() ),"ep");

   InSANEStructureFunctions * lhaSFs = new LHAPDFStructureFunctions();
/*   ((LHAPDFStructureFunctions*)lhaSFs)->SetPDFDataSet("MSTW2008nlo68cl");*/
/*   ((LHAPDFStructureFunctions*)lhaSFs)->SetPDFDataSet("cteq4m");*/
   ((LHAPDFStructureFunctions*)lhaSFs)->SetPDFDataSet("cteq65");
   InSANEStructureFunctions * f1f2SFs = new F1F208StructureFunctions();
//   f1f2SFs->PlotSFs(70.0);

   TF1 * xF2p = new TF1("xF2p", lhaSFs, &InSANEStructureFunctions::EvaluateF2p, 
               0, 1, 1,"InSANEStructureFunctions","EvaluateF2p");
   leg->AddEntry(xF2p,"F2p LHAPDF ","l");

   TF1 * xF2pA = new TF1("xF2pA", f1f2SFs, &InSANEStructureFunctions::EvaluateF2p, 
               0, 1, 1,"InSANEStructureFunctions","EvaluateF2p");
   leg->AddEntry(xF2pA,"F2p F1F209","l");

   xF2p->SetParameter(0,4.5);
   xF2pA->SetParameter(0,4.5);
   xF2pA->SetLineColor(3);
   xF2p->Draw("same");
   xF2pA->Draw("same");

   leg->Draw();
   lhaSFs->PlotSFs(4.5);

return(0);
}