Int_t plot_x2g1g2He3(Double_t Q20 = 1.0){

   gSystem->Load("libWhitFit.so");

   InSANEPolarizedStructureFunctionsFromPDFs * sfs      = new InSANEPolarizedStructureFunctionsFromPDFs();
   InSANEPolarizedPartonDistributionFunctions * pdfs    = 0;
   LCWFPolarizedPartonDistributionFunctions  * LCWFpdfs = new LCWFPolarizedPartonDistributionFunctions();
   BBPolarizedPDFs                           * BBpdfs   = new BBPolarizedPDFs();

   //InSANEPolarizedStructureFunctionsFromPDFs * jamSFs = new InSANEPolarizedStructureFunctionsFromPDFs();
   WPolarizedEvolution<JAMPolarizedPDFs> * jamPDFs = new WPolarizedEvolution<JAMPolarizedPDFs>();
   InSANEPolarizedStructureFunctionsFromPDFs * jamSFs = new InSANEPolarizedStructureFunctionsFromPDFs();
   jamSFs->SetPolarizedPDFs(jamPDFs);
   jamPDFs->MakeGrid();
   jamPDFs->EvolveInit(Q20);
   jamPDFs->Evolve();

   pdfs = jamPDFs;
   sfs->SetPolarizedPDFs(pdfs);


   //WEvolvedg2<Wg2Model> * g2model = new WEvolvedg2<Wg2Model>;
   ////WEvolvedg2<InSANEPolarizedStructureFunctionsFromPDFs> * g2model = new WEvolvedg2<InSANEPolarizedStructureFunctionsFromPDFs>;
   ////WEvolvedg2_Singlet<Wg2Model> * g2model = new WEvolvedg2_Singlet<Wg2Model>;
   //g2model->SetQCDNUMFunc1(g2model);
   ////g2model->SetQCDNUMFunc1(sfs);
   //g2model->SetFunc(4);
   //g2model->SetType(4);
   //g2model->SetPolarizedPDFs(pdfs);
   //g2model->MakeGrid();
   //g2model->EvolveInit(Q20);
   //g2model->Evolve();

   // ------------------
   // input model at fixed scale
   //Wg2Model * g2model2 = new Wg2Model;
   //g2model2->SetPolarizedPDFs(pdfs);

   Int_t    npar = 1;
   Double_t xmin = 0.01;
   Double_t xmax = 0.98;

   TF1 * x2g2pWW = new TF1("x2g2pWW", sfs, &InSANEPolarizedStructureFunctions::Evaluatex2g2pWW, 
               xmin, xmax, npar,"InSANEPolarizedStructureFunctions",        "Evaluatex2g2pWW");
   x2g2pWW->SetParameter(0,4.3);

   // g1n with all HT
   TF1 * x2g1n = new TF1("x2g1n", sfs, &InSANEPolarizedStructureFunctions::Evaluatex2g1n, 
               xmin, xmax, npar,"InSANEPolarizedStructureFunctions",        "Evaluatex2g1n");
   x2g1n->SetParameter(0,4.3);

   TF1 * x2g2n = new TF1("x2g2n", sfs, &InSANEPolarizedStructureFunctions::Evaluatex2g2n, 
               xmin, xmax, npar,"InSANEPolarizedStructureFunctions",        "Evaluatex2g2n");
   x2g2n->SetParameter(0,4.3);

   TF1 * x2g2p_2 = new TF1("x2g2p_2", sfs, &InSANEPolarizedStructureFunctions::Evaluatex2g2p, 
               xmin, xmax, npar,"InSANEPolarizedStructureFunctions",           "Evaluatex2g2p");
   x2g2p_2->SetParameter(0,4.3);

   // ----------------------------
   //load_style("MultiSquarePlot");
   //gROOT->SetStyle("MultiSquarePlot");

   gStyle->SetLineStyleString(20,"20 20");
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

   // ---------------------------------------------------------------------------
   // Loop over data points and evaluate the model at each x and Q2
   NucDBMeasurement * g1n_model_meas     = new NucDBMeasurement("g1n","g1n Model");
   NucDBMeasurement * g1n_model_tw2_meas = new NucDBMeasurement("g1n_twist2","g1n Model");
   NucDBMeasurement * g1n_model_tw3_meas = new NucDBMeasurement("g1n_twist3","g1n Model twist2");
   NucDBMeasurement * g1n_model_tw4_meas = new NucDBMeasurement("g1n_twist4","g1n Model twist3");
   NucDBMeasurement * g1n_model_diff_meas     = new NucDBMeasurement("g1n_diff","g1n Model");

   NucDBMeasurement * g2n_model_meas     = new NucDBMeasurement("g2n","g2n Model");
   NucDBMeasurement * g2n_model_tw2_meas = new NucDBMeasurement("g2n_twist2","g2n Model");
   NucDBMeasurement * g2n_model_tw3_meas = new NucDBMeasurement("g2n_twist3","g2n Model twist2");
   NucDBMeasurement * g2n_model_tw4_meas = new NucDBMeasurement("g2n_twist4","g2n Model twist3");
   NucDBMeasurement * g2n_model_diff_meas     = new NucDBMeasurement("g2n_diff","g2n Model");

   // g1
   for(int i = 0; i < g1He3->GetNDataPoints(); i++ ) {

      NucDBDataPoint      * datapoint1 = (NucDBDataPoint*)g1He3->GetDataPoints()->At(i);
      NucDBBinnedVariable * x_var      = datapoint1->FindVariable("x");
      NucDBBinnedVariable * Q2_var     = datapoint1->FindVariable("Qsquared");
      NucDBDataPoint aPoint(*datapoint1);

      // -------------------------------------
      // g1
      // All HT
      Double_t  g1n_model_value     = sfs->g1n(x_var->GetMean(),Q2_var->GetMean());
      //Double_t  g1n_model_error = datapoint1->GetTotalError().GetError()*x_var->GetMean()*x_var->GetMean();
      aPoint.SetValue(g1n_model_value);
      g1n_model_meas->AddDataPoint(new NucDBDataPoint(aPoint));

      // Twist2
      Double_t  g1n_model_tw2_value = sfs->g1n_Twist2(x_var->GetMean(),Q2_var->GetMean());
      aPoint.SetValue(g1n_model_tw2_value);
      g1n_model_tw2_meas->AddDataPoint(new NucDBDataPoint(aPoint));

      // Twist3
      Double_t  g1n_model_tw3_value = sfs->g1n_Twist3(x_var->GetMean(),Q2_var->GetMean());
      aPoint.SetValue(g1n_model_tw3_value);
      g1n_model_tw3_meas->AddDataPoint(new NucDBDataPoint(aPoint));

      // Twist4
      Double_t  g1n_model_tw4_value = sfs->g1n_Twist4(x_var->GetMean(),Q2_var->GetMean());
      aPoint.SetValue(g1n_model_tw4_value);
      g1n_model_tw4_meas->AddDataPoint(new NucDBDataPoint(aPoint));

      // data - model 
      aPoint.SetValue( datapoint1->GetValue() - g1n_model_value );
      g1n_model_diff_meas->AddDataPoint(new NucDBDataPoint(aPoint));

   }

   NucDBMeasurement * g1n_model_meas_59 = g1n_model_meas->CreateMeasurementFilteredWithBin(&Ebeam1);
   NucDBMeasurement * g1n_model_meas_47 = g1n_model_meas->CreateMeasurementFilteredWithBin(&Ebeam2);
   g1n_model_meas_59->SetColor(kGreen-2);
   g1n_model_meas_47->SetColor(kGreen-2);

   NucDBMeasurement * g1n_model_tw2_meas_59 = g1n_model_tw2_meas->CreateMeasurementFilteredWithBin(&Ebeam1);
   NucDBMeasurement * g1n_model_tw2_meas_47 = g1n_model_tw2_meas->CreateMeasurementFilteredWithBin(&Ebeam2);
   g1n_model_tw2_meas_59->SetColor(kGreen-2);
   g1n_model_tw2_meas_47->SetColor(kGreen-2);

   NucDBMeasurement * g1n_model_tw3_meas_59 = g1n_model_tw3_meas->CreateMeasurementFilteredWithBin(&Ebeam1);
   NucDBMeasurement * g1n_model_tw3_meas_47 = g1n_model_tw3_meas->CreateMeasurementFilteredWithBin(&Ebeam2);
   g1n_model_tw3_meas_59->SetColor(kGreen-2);
   g1n_model_tw3_meas_47->SetColor(kGreen-2);

   NucDBMeasurement * g1n_model_tw4_meas_59 = g1n_model_tw4_meas->CreateMeasurementFilteredWithBin(&Ebeam1);
   NucDBMeasurement * g1n_model_tw4_meas_47 = g1n_model_tw4_meas->CreateMeasurementFilteredWithBin(&Ebeam2);
   g1n_model_tw4_meas_59->SetColor(6);
   g1n_model_tw4_meas_47->SetColor(6);

   NucDBMeasurement * g1n_model_diff_meas_59 = g1n_model_diff_meas->CreateMeasurementFilteredWithBin(&Ebeam1);
   NucDBMeasurement * g1n_model_diff_meas_47 = g1n_model_diff_meas->CreateMeasurementFilteredWithBin(&Ebeam2);
   g1n_model_diff_meas_59->SetColor(kBlue+1);
   g1n_model_diff_meas_47->SetColor(kRed+1);

   for(int i = 0; i < g2He3->GetNDataPoints(); i++ ) {

      NucDBDataPoint      * datapoint1 = (NucDBDataPoint*)g2He3->GetDataPoints()->At(i);
      NucDBBinnedVariable * x_var      = datapoint1->FindVariable("x");
      NucDBBinnedVariable * Q2_var     = datapoint1->FindVariable("Qsquared");
      NucDBDataPoint aPoint(*datapoint1);
      Double_t valExp = datapoint1->GetValue();

      // -------------------------------------
      // g2
      // All HT
      Double_t  g2n_model_value     = sfs->g2n(x_var->GetMean(),Q2_var->GetMean());
      //Double_t  g2n_model_error = datapoint1->GetTotalError().GetError()*x_var->GetMean()*x_var->GetMean();
      aPoint.SetValue(g2n_model_value);
      g2n_model_meas->AddDataPoint(new NucDBDataPoint(aPoint));

      // Twist2
      Double_t  g2n_model_tw2_value = sfs->g2n_Twist2(x_var->GetMean(),Q2_var->GetMean());
      aPoint.SetValue(g2n_model_tw2_value);
      g2n_model_tw2_meas->AddDataPoint(new NucDBDataPoint(aPoint));

      // Twist2
      Double_t  g2n_model_tw3_value = sfs->g2n_Twist3(x_var->GetMean(),Q2_var->GetMean());
      aPoint.SetValue(g2n_model_tw3_value);
      g2n_model_tw3_meas->AddDataPoint(new NucDBDataPoint(aPoint));

      // data - model 
      aPoint.SetValue( valExp - g2n_model_value );
      g2n_model_diff_meas->AddDataPoint(new NucDBDataPoint(aPoint));

   }
   NucDBMeasurement * g2n_model_meas_59 = g2n_model_meas->CreateMeasurementFilteredWithBin(&Ebeam1);
   NucDBMeasurement * g2n_model_meas_47 = g2n_model_meas->CreateMeasurementFilteredWithBin(&Ebeam2);
   g2n_model_meas_59->SetColor(kGreen-2);
   g2n_model_meas_47->SetColor(kGreen-2);

   NucDBMeasurement * g2n_model_tw2_meas_59 = g2n_model_tw2_meas->CreateMeasurementFilteredWithBin(&Ebeam1);
   NucDBMeasurement * g2n_model_tw2_meas_47 = g2n_model_tw2_meas->CreateMeasurementFilteredWithBin(&Ebeam2);
   g2n_model_tw2_meas_59->SetColor(kGreen-2);
   g2n_model_tw2_meas_47->SetColor(kGreen-2);

   NucDBMeasurement * g2n_model_tw3_meas_59 = g2n_model_tw3_meas->CreateMeasurementFilteredWithBin(&Ebeam1);
   NucDBMeasurement * g2n_model_tw3_meas_47 = g2n_model_tw3_meas->CreateMeasurementFilteredWithBin(&Ebeam2);
   g2n_model_tw3_meas_59->SetColor(kGreen-2);
   g2n_model_tw3_meas_47->SetColor(kGreen-2);

   NucDBMeasurement * g2n_model_diff_meas_59 = g2n_model_diff_meas->CreateMeasurementFilteredWithBin(&Ebeam1);
   NucDBMeasurement * g2n_model_diff_meas_47 = g2n_model_diff_meas->CreateMeasurementFilteredWithBin(&Ebeam2);
   g2n_model_diff_meas_59->SetColor(kBlue+1);
   g2n_model_diff_meas_47->SetColor(kRed+1);

   // ---------------------------------------------------------------------------
   TF2             * fx = new TF2("fx","x*x*y",0,1);
   TGraph          * gr1= 0;
   TGraphErrors    * gr = 0;
   TMultiGraph     * mg  = new TMultiGraph();
   TMultiGraph     * mg2 = new TMultiGraph();
   TMultiGraph     * mg3 = new TMultiGraph();
   TMultiGraph     * mg4 = new TMultiGraph();

   TLegend         * leg = new TLegend(0.6,0.7,0.94,0.94);
   //leg->SetFillStyle(0);
   //leg->SetFillColor(0);
   //leg->SetBorderSize(0);

   TLegend         * leg2 = new TLegend(0.6,0.7,0.94,0.94);
   //leg->SetFillStyle(0);
   //leg->SetFillColor(0);
   //leg->SetBorderSize(0);

   TF1             * f0 = new TF1("f0","x*0.0",0.2,1);
   f0->SetLineStyle(20);
   f0->SetLineColor(1);
   f0->SetLineWidth(1);

   TGraph * zero_line = new TGraph(f0->DrawCopy("goff")->GetHistogram());
   mg->Add( zero_line,"l");
   mg2->Add(zero_line,"l");
   mg3->Add(zero_line,"l");
   mg4->Add(zero_line,"l");

   // ------------------------------------------
   // 5.9 GeV plots
   // g1
   gr = g1He3_59->BuildGraph("x");
   gr->Apply(fx);
   mg->Add(gr,"p");
   leg->AddEntry(gr,"5.9 GeV","lp");

   gr = g1n_model_meas_59->BuildGraph("x");
   gr->Apply(fx);
   gr->SetMarkerStyle(24);
   mg->Add(gr,"p");
   leg->AddEntry(gr,"model 5.9 GeV data","lp");

   gr = g1n_model_tw2_meas_59->BuildGraph("x");
   gr->Apply(fx);
   gr->SetMarkerStyle(22);
   mg->Add(gr,"p");
   leg->AddEntry(gr,"Twist2 model 5.9 GeV data","lp");

   gr = g1n_model_tw3_meas_59->BuildGraph("x");
   gr->Apply(fx);
   gr->SetMarkerStyle(34);
   mg->Add(gr,"pl");
   leg->AddEntry(gr,"Twist3 model 5.9 GeV data","lp");

   gr = g1n_model_tw4_meas_59->BuildGraph("x");
   gr->Apply(fx);
   gr->SetMarkerStyle(33);
   mg->Add(gr,"p");
   leg->AddEntry(gr,"Twist4 model 5.9 GeV data","lp");

   gr = g1n_model_diff_meas_59->BuildGraph("x");
   gr->Apply(fx);
   gr->SetMarkerStyle(25);
   mg->Add(gr,"p");
   leg->AddEntry(gr,"data - model 5.9 GeV data","lp");

   //gr = g1n_model_diff_meas_47->BuildGraph("x");
   //gr->Apply(fx);
   //gr->SetMarkerStyle(25);
   //mg->Add(gr,"p");
   //leg->AddEntry(gr,"data - model 4.7 GeV data","lp");

   x2g1n->SetParameter(0,4.3);
   gr1 = new TGraph(x2g1n->DrawCopy("goff")->GetHistogram());
   mg->Add(gr1,"l");
   leg->AddEntry(gr1,"g1","l");

   // ------------------------------------------
   // 4.7 GeV plots
   // g1
   gr = g1He3_47->BuildGraph("x");
   gr->Apply(fx);
   mg3->Add(gr,"p");
   leg2->AddEntry(gr,"4.7 GeV","lp");

   gr = g1n_model_meas_47->BuildGraph("x");
   gr->Apply(fx);
   gr->SetMarkerStyle(24);
   mg3->Add(gr,"p");
   leg2->AddEntry(gr,"model 4.7 GeV data","lp");

   gr = g1n_model_tw2_meas_47->BuildGraph("x");
   gr->Apply(fx);
   gr->SetMarkerStyle(22);
   mg3->Add(gr,"p");
   leg2->AddEntry(gr,"Twist2 model 4.7 GeV data","lp");

   gr = g1n_model_tw3_meas_47->BuildGraph("x");
   gr->Apply(fx);
   gr->SetMarkerStyle(34);
   mg3->Add(gr,"pl");
   leg2->AddEntry(gr,"Twist3 model 4.7 GeV data","lp");

   gr = g1n_model_tw4_meas_47->BuildGraph("x");
   gr->Apply(fx);
   gr->SetMarkerStyle(33);
   mg3->Add(gr,"p");
   leg2->AddEntry(gr,"Twist4 model 4.7 GeV data","lp");

   gr = g1n_model_diff_meas_47->BuildGraph("x");
   gr->Apply(fx);
   gr->SetMarkerStyle(25);
   mg3->Add(gr,"p");
   leg2->AddEntry(gr,"data - model 4.7 GeV data","lp");

   x2g1n->SetParameter(0,3.2);
   gr1 = new TGraph(x2g1n->DrawCopy("goff")->GetHistogram());
   mg3->Add(gr1,"l");
   leg->AddEntry(gr1,"g1","l");

   // ------------------------------------------
   // g2 - 5.9 GeV
   gr = g2He3_59->BuildGraph("x");
   gr->Apply(fx);
   mg2->Add(gr,"p");

   //gr = g2He3_47->BuildGraph("x");
   //gr->Apply(fx);
   //mg2->Add(gr,"p");

   gr = g2n_model_meas_59->BuildGraph("x");
   gr->Apply(fx);
   gr->SetMarkerStyle(24);
   mg2->Add(gr,"p");
   //leg->AddEntry(gr,"model 5.9 GeV data","lp");

   gr = g2n_model_tw2_meas_59->BuildGraph("x");
   gr->Apply(fx);
   gr->SetMarkerStyle(22);
   mg2->Add(gr,"p");
   //leg->AddEntry(gr,"Twist2 model 5.9 GeV data","lp");

   gr = g2n_model_tw3_meas_59->BuildGraph("x");
   gr->Apply(fx);
   gr->SetMarkerStyle(34);
   mg2->Add(gr,"pl");
   //leg->AddEntry(gr,"Twist3 model 5.9 GeV data","lp");

   gr = g2n_model_diff_meas_59->BuildGraph("x");
   gr->Apply(fx);
   gr->SetMarkerStyle(25);
   mg2->Add(gr,"p");
   //leg->AddEntry(gr,"Twist3 model 5.9 GeV data","lp");

   //gr = g2n_model_diff_meas_47->BuildGraph("x");
   //gr->Apply(fx);
   //gr->SetMarkerStyle(25);
   //mg2->Add(gr,"p");
   ////leg->AddEntry(gr,"Twist3 model 5.9 GeV data","lp");

   x2g2n->SetParameter(0,4.3);
   gr1 = new TGraph(x2g2n->DrawCopy("goff")->GetHistogram());
   mg2->Add(gr1,"l");
   //leg->AddEntry(gr1,"g1","l");

   // ------------------------------------------
   // g2 4.7 GeV
   gr = g2He3_47->BuildGraph("x");
   gr->Apply(fx);
   mg4->Add(gr,"p");

   //gr = g2He3_47->BuildGraph("x");
   //gr->Apply(fx);
   //mg2->Add(gr,"p");

   gr = g2n_model_meas_47->BuildGraph("x");
   gr->Apply(fx);
   gr->SetMarkerStyle(24);
   mg4->Add(gr,"p");
   //leg->AddEntry(gr,"model 5.9 GeV data","lp");

   gr = g2n_model_tw2_meas_47->BuildGraph("x");
   gr->Apply(fx);
   gr->SetMarkerStyle(22);
   mg4->Add(gr,"p");
   //leg->AddEntry(gr,"Twist2 model 5.9 GeV data","lp");

   gr = g2n_model_tw3_meas_47->BuildGraph("x");
   gr->Apply(fx);
   gr->SetMarkerStyle(34);
   mg4->Add(gr,"pl");
   //leg->AddEntry(gr,"Twist3 model 5.9 GeV data","lp");

   gr = g2n_model_diff_meas_47->BuildGraph("x");
   gr->Apply(fx);
   gr->SetMarkerStyle(25);
   mg4->Add(gr,"p");
   //leg->AddEntry(gr,"Twist3 model 5.9 GeV data","lp");

   //gr = g2n_model_diff_meas_47->BuildGraph("x");
   //gr->Apply(fx);
   //gr->SetMarkerStyle(25);
   //mg2->Add(gr,"p");
   ////leg->AddEntry(gr,"Twist3 model 5.9 GeV data","lp");

   x2g2n->SetParameter(0,3.2);
   gr1 = new TGraph(x2g2n->DrawCopy("goff")->GetHistogram());
   mg4->Add(gr1,"l");

   //---------------------------------------
   TCanvas * c = new TCanvas();

   mg->Draw("a");
   mg->GetXaxis()->SetTitle("x");
   mg->GetXaxis()->CenterTitle(true);
   mg->GetXaxis()->SetLimits(0.2,1.0);
   mg->GetYaxis()->SetTitle("g_{1}^{^{3}He}");
   mg->GetYaxis()->CenterTitle(true);
   //mg->GetYaxis()->SetNdivisions(5,5,0,true);
   mg->GetYaxis()->SetRangeUser(-0.007,0.010);
   leg->Draw();

   c->SaveAs("results/plot_x2g1He3_59.png");

   //c->cd(2);
   mg2->Draw("a");
   mg2->GetXaxis()->SetTitle("x");
   mg2->GetXaxis()->CenterTitle(true);
   mg2->GetXaxis()->SetLimits(0.2,1.0);
   mg2->GetYaxis()->SetTitle("g_{2}^{^{3}He}");
   mg2->GetYaxis()->CenterTitle(true);
   //mg2->GetYaxis()->SetNdivisions(5,5,0,true);
   mg2->GetYaxis()->SetRangeUser(-0.007,0.010);
   leg->Draw();

   c->SaveAs("results/plot_x2g2He3_59.png");

   // E = 4.7 GeV
   mg3->Draw("a");
   mg3->GetXaxis()->SetTitle("x");
   mg3->GetXaxis()->CenterTitle(true);
   mg3->GetXaxis()->SetLimits(0.2,1.0);
   mg3->GetYaxis()->SetTitle("g_{1}^{^{3}He}");
   mg3->GetYaxis()->CenterTitle(true);
   //mg->GetYaxis()->SetNdivisions(5,5,0,true);
   mg3->GetYaxis()->SetRangeUser(-0.007,0.010);
   leg2->Draw();

   c->SaveAs("results/plot_x2g1He3_47.png");

   mg4->Draw("a");
   mg4->GetXaxis()->SetTitle("x");
   mg4->GetXaxis()->CenterTitle(true);
   mg4->GetXaxis()->SetLimits(0.2,1.0);
   mg4->GetYaxis()->SetTitle("g_{2}^{^{3}He}");
   mg4->GetYaxis()->CenterTitle(true);
   //mg2->GetYaxis()->SetNdivisions(5,5,0,true);
   mg4->GetYaxis()->SetRangeUser(-0.007,0.010);
   leg2->Draw();

   c->SaveAs("results/plot_x2g2He3_47.png");

   return 0;
}

