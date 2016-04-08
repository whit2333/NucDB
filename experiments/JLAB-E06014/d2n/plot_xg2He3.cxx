Int_t plot_xg2He3(){

   NucDBManager    * dbman = NucDBManager::GetManager();
   NucDBExperiment * exp   = dbman->GetExperiment("d2n");

   NucDBBinnedVariable    Ebeam1("Ebeam","E_{beam}");
   NucDBBinnedVariable    Ebeam2("Ebeam","E_{beam}");
   //NucDBBinnedVariable    x("x","x");
   //NucDBBinnedVariable    Q2("Qsquared","Q^{2}");
   //NucDBDiscreteVariable  A("A","A");
   //NucDBDiscreteVariable  Z("Z","Z");

   Ebeam1.SetValueSize(5.9,0.1);
   Ebeam2.SetValueSize(4.7,0.1);

   if(!exp) {
      std::cout << "experiment not found " << std::endl;
      return -2;
   }

   NucDBMeasurement * g1He3 = exp->GetMeasurement("g1He3");
   if(!g1He3) {
      std::cout << "measurement not found " << std::endl;
      return -3;
   }
   NucDBMeasurement * g2He3 = exp->GetMeasurement("g2He3");
   if(!g2He3) {
      std::cout << "measurement not found " << std::endl;
      return -3;
   }

   NucDBMeasurement * g1He3_59 = g1He3->CreateMeasurementFilteredWithBin(&Ebeam1);
   NucDBMeasurement * g1He3_47 = g1He3->CreateMeasurementFilteredWithBin(&Ebeam2);
   g1He3_59->SetColor(kBlue+1);
   g1He3_47->SetColor(kRed+1);

   NucDBMeasurement * g2He3_59 = g2He3->CreateMeasurementFilteredWithBin(&Ebeam1);
   NucDBMeasurement * g2He3_47 = g2He3->CreateMeasurementFilteredWithBin(&Ebeam2);
   g2He3_59->SetColor(kBlue+1);
   g2He3_47->SetColor(kRed+1);

   TF1             * fx = new TF1("fx","x*y",0,1);
   TGraphErrors    * gr = 0;
   TMultiGraph     * mg = new TMultiGraph();
   TMultiGraph     * mg2 = new TMultiGraph();
   TLegend         * leg = new TLegend(0.6,0.6,0.84,0.84);

   leg->SetFillColor(0);
   leg->SetBorderSize(0);

   gr = g1He3_59->BuildGraph("x");
   gr->Apply(fx);
   mg->Add(gr,"p");
   leg->AddEntry(gr,"5.9 GeV","lp");

   gr = g1He3_47->BuildGraph("x");
   gr->Apply(fx);
   mg->Add(gr,"p");
   leg->AddEntry(gr,"4.7 GeV","lp");

   gr = g2He3_59->BuildGraph("x");
   gr->Apply(fx);
   mg2->Add(gr,"p");

   gr = g2He3_47->BuildGraph("x");
   gr->Apply(fx);
   mg2->Add(gr,"p");

   //---------------------------------------
   TCanvas * c = new TCanvas();

   mg2->Draw("a");
   mg2->GetXaxis()->SetTitle("x");
   mg2->GetXaxis()->CenterTitle(true);
   mg2->GetYaxis()->SetTitle("g_{2}^{^{3}He}");
   mg2->GetYaxis()->CenterTitle(true);
   mg2->GetYaxis()->SetRangeUser(-0.01,0.01);
   leg->Draw();

   c->SaveAs("results/plot_xg2He3.png");

   return 0;
}

