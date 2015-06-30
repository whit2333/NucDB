Int_t test4() {

   gSystem->Load( "libNucDB" );
   NucDBManager * manager = NucDBManager::GetManager();

   NucDBBinnedVariable * Qsq = new NucDBBinnedVariable("Qsquared","Q^{2} 3.0<Q^{2}<4.0");
   Qsq->SetBinMinimum(3.0);
   Qsq->SetBinMaximum(4.0);
   NucDBBinnedVariable * Qsq2 = new NucDBBinnedVariable("Qsquared","Q^{2} 4.0<Q^{2}<5.0");
   Qsq2->SetBinMinimum(4.0);
   Qsq2->SetBinMaximum(5.0);

   NucDBMeasurement * g1p = manager->GetExperiment("SLAC_E155")->GetMeasurement("g1p");
   NucDBMeasurement * g1pQsq1 = new NucDBMeasurement("g1pQsq1",Form("g_{1}^{p} %s",Qsq->GetTitle()));
   NucDBMeasurement * g1pQsq2 = new NucDBMeasurement("g1pQsq2",Form("g_{1}^{p} %s",Qsq2->GetTitle()));

   TLegend * leg = new TLegend(0.1,0.7,0.48,0.9);
   leg->SetHeader("g1p");

   g1p->BuildGraph();
   g1p->fGraph->Draw("ap");

   leg->AddEntry(g1p->fGraph,Form("%s %s",g1p->fExperiment.Data(),g1p->GetTitle() ),"ep");

   g1pQsq1->SetDataPoints(g1p->FilterWithBin(Qsq));
   g1pQsq1->BuildGraph();
   g1pQsq1->fGraph->SetMarkerColor(4);
   g1pQsq1->fGraph->SetLineColor(4);
   g1pQsq1->fGraph->Draw("p");
   leg->AddEntry(g1pQsq1->fGraph,Form("%s %s",g1p->fExperiment.Data(),g1pQsq1->GetTitle() ),"ep");

   g1pQsq2->SetDataPoints(g1p->FilterWithBin(Qsq2));
   g1pQsq2->BuildGraph();
   g1pQsq2->fGraph->SetMarkerColor(2);
   g1pQsq2->fGraph->SetLineColor(2);
   g1pQsq2->fGraph->Draw("p");
   leg->AddEntry(g1pQsq2->fGraph,Form("%s %s",g1p->fExperiment.Data(),g1pQsq2->GetTitle() ),"ep");

    InSANEPolarizedStructureFunctionsFromPDFs * pSFs = new InSANEPolarizedStructureFunctionsFromPDFs();
   pSFs->SetPolarizedPDFs( new BBPolarizedPDFs);

    InSANEPolarizedStructureFunctionsFromPDFs * pSFsA = new InSANEPolarizedStructureFunctionsFromPDFs();
   pSFsA->SetPolarizedPDFs( new DNS2005PolarizedPDFs);

/*   InSANEStructureFunctions * lhaSFs = new LHAPDFStructureFunctions();*/
/*   ((LHAPDFStructureFunctions*)lhaSFs)->SetPDFDataSet("MSTW2008nlo68cl");*/
/*   ((LHAPDFStructureFunctions*)lhaSFs)->SetPDFDataSet("cteq4m");*/
/*   ((LHAPDFStructureFunctions*)lhaSFs)->SetPDFDataSet("cteq65");*/
/*   InSANEStructureFunctions * f1f2SFs = new F1F208StructureFunctions();*/
//   f1f2SFs->PlotSFs(70.0);

   TF1 * xg1p = new TF1("xg1p", pSFs, &InSANEPolarizedStructureFunctions::Evaluateg1p, 
               0, 1, 1,"InSANEPolarizedStructureFunctions","Evaluateg1p");

   TF1 * xg1pA = new TF1("xg1pA", pSFsA, &InSANEPolarizedStructureFunctions::Evaluateg1p, 
               0, 1, 1,"InSANEPolarizedStructureFunctions","Evaluateg1p");

   xg1p->SetParameter(0,4.5);
   xg1pA->SetParameter(0,4.5);
   xg1pA->SetLineColor(3);
   xg1p->Draw("same");
   xg1pA->Draw("same");

   leg->AddEntry(xg1p,"g1p BB ","l");
   leg->AddEntry(xg1pA,"g1p DNS2005 ","l");

   leg->Draw();
   lhaSFs->PlotSFs(4.5);

return(0);
}