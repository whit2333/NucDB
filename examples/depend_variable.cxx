{
   NucDBManager * man = NucDBManager::GetManager();
   NucDBExperiment * exp = man->GetExperiment("SLAC_E143");
   if (!exp) return -1;
   NucDBMeasurement * g1p = exp->GetMeasurement("g1p");
   if (!g1p) return(-1);
   TList * points =  g1p->GetDataPointsList();
   TF2 f2("W2", "NucDB::Kine::W2_xQ2(x,y)", 0.3, 0.9, 3, 8);
   NucDBDataPoint * p = 0;
   NucDBBinnedVariable * bVar = 0;
   for (int i = 0; i < points->GetEntries(); i++) {

      NucDBDependentVariable * newVar = new NucDBDependentVariable("Wsquared", "W^{2}");
      newVar->SetFunction(&f2);
      p = (NucDBDataPoint*)points->At(i);

      bVar = p->GetBinVariable("x");
      newVar->SetVariable(0, bVar);
      bVar = p->GetBinVariable("Qsquared");
      newVar->SetVariable(1, bVar);
      std::cout << "x=" << p->GetBinVariable("x")->fMean
      << " ,Q2=" << bVar->fMean
      << " ... W2 = " << newVar->GetValue() << "\n";
      p->AddBinVariable(newVar);
   }
   TCanvas * c = new TCanvas("g1pVsW2", "g1p vs W2");
   TGraph * g  = g1p->BuildGraph("Wsquared");
   g->Draw("ap");
   c->SaveAs("examples/g1pVsW2.png");




}
