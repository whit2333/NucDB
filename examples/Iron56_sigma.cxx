Int_t Iron56_sigma(){

   Double_t beamEnergy  = 1.0;
   Double_t delta_Ebeam = 0.5;
   Double_t theta       = 15.0*degree;
   Double_t delta_theta = 10.0*degree;

   // For plotting
   Double_t Emin  = 0.001;
   Double_t Emax  = 1.0;//beamEnergy-0.1;
   Int_t npar     = 2;

   NucDBManager     * dbman   = NucDBManager::GetManager();
   NucDBExperiment  * exp  = 0;
   NucDBMeasurement * meas = 0;
   NucDBMeasurement * meas_Al = 0;

   // -----------------------------------------
   // Bins for filtering the data
   NucDBBinnedVariable * Ebeam = new NucDBBinnedVariable("E","E");
   Ebeam->SetBinMinimum(1.0);
   Ebeam->SetBinMaximum(6.0);
   Ebeam->SetAverage(4.5);

   NucDBBinnedVariable * thetaBin = new NucDBBinnedVariable("theta","#theta");
   thetaBin->SetBinMinimum(theta/degree - delta_theta/degree);
   thetaBin->SetBinMaximum(theta/degree + delta_theta/degree);
   thetaBin->SetAverage(theta/degree);
   thetaBin->SetMean(theta/degree);

   NucDBBinnedVariable * Ebeam = new NucDBBinnedVariable("E","E");
   Ebeam->SetBinMinimum(beamEnergy - delta_Ebeam);
   Ebeam->SetBinMaximum(beamEnergy + delta_Ebeam);
   Ebeam->SetAverage(beamEnergy);
   Ebeam->SetMean(beamEnergy);

   std::vector<double> beamEnergies;
   std::vector<double> thetas;

   // -----------------------------------------
   // 
   exp = dbman->GetExperiment("QES-Archive");
   if(!exp) return -1;
   meas = exp->GetMeasurement("sigma_56Fe");
   if(!meas) return -2;
   meas->ListVariables();
   meas_Al = exp->GetMeasurement("sigma_27Al");
   if(!meas_Al) return -3;

   // -----------------------------------------
   // 
   meas_Al->GetUniqueBinnedVariableValues("E",&beamEnergies);
   for( int i = 0; i< beamEnergies.size(); i++){
      std::cout << beamEnergies[i] << " GeV" << std::endl;
   }
   meas_Al->GetUniqueBinnedVariableValues("theta",&thetas);
   for( int i = 0; i< thetas.size(); i++){
      std::cout << thetas[i] << " degrees" << std::endl;
   }

   // -----------------------------------------
   // Filter the data.
   meas->ApplyFilterWithBin(Ebeam);
   meas->ApplyFilterWithBin(thetaBin);
   meas->Print();
   meas_Al->ApplyFilterWithBin(Ebeam);
   meas_Al->ApplyFilterWithBin(thetaBin);
   meas_Al->Print();

   beamEnergies.clear();
   meas->GetUniqueBinnedVariableValues("E",&beamEnergies);
   for( int i = 0; i< beamEnergies.size(); i++){
      std::cout << beamEnergies[i] << " GeV" << std::endl;
   }
   thetas.clear();
   meas->GetUniqueBinnedVariableValues("theta",&thetas);
   for( int i = 0; i< thetas.size(); i++){
      std::cout << thetas[i] << " degrees" << std::endl;
   }
   //meas->PrintData();

   Double_t Ebeam_mean = meas->GetBinnedVariableMean("E");
   Double_t theta_mean = meas->GetBinnedVariableMean("theta")*degree;

   // -----------------------------------
   // Plot
   TCanvas      * c  = new TCanvas("Iron56_sigma","Iron56_sigma");
   TMultiGraph * mg = new TMultiGraph();

   // -----------------
   // Data
   TGraphErrors * gr = meas->BuildGraph("nu");
   gr->SetMarkerColor(1);
   gr->SetLineColor(1);
   mg->Add(gr,"ep");

   TGraphErrors * gr2 = meas_Al->BuildGraph("nu");
   gr2->SetMarkerColor(2);
   gr2->SetLineColor(2);
   mg->Add(gr2,"ep");

   // -----------------
   // Born
   //InSANEInelasticRadiativeTail2 * fDiffXSec4 = new  InSANEInelasticRadiativeTail2();
   F1F209eInclusiveDiffXSec * fDiffXSec4 = new  F1F209eInclusiveDiffXSec();
   //InSANERadiator<InSANEInclusiveDiffXSec> * fDiffXSec4 = new  InSANERadiator<InSANEInclusiveDiffXSec>();
   //InSANECompositeDiffXSec * fDiffXSec4 = new  InSANECompositeDiffXSec();
   fDiffXSec4->SetBeamEnergy(Ebeam_mean);
   fDiffXSec4->SetTargetNucleus(InSANENucleus::Fe56());
   //fDiffXSec4->SetRegion4(true);
   fDiffXSec4->InitializePhaseSpaceVariables();
   fDiffXSec4->InitializeFinalStateParticles();
   fDiffXSec4->GetPhaseSpace()->GetVariableWithName("energy")->SetMinimum(0.01);
   fDiffXSec4->GetPhaseSpace()->GetVariableWithName("theta")->SetMinimum(0.0001);
   TF1 * sigma4 = new TF1("sigma4", fDiffXSec4, &InSANEInclusiveDiffXSec::PhotonEnergyDependentXSec, 
         Emin, Emax, npar,"InSANEInclusiveDiffXSec","PhotonEnergyDependentXSec");
   sigma4->SetParameter(0,theta_mean);
   sigma4->SetParameter(1,0.0);
   //sigma4->SetNpx(npx);
   sigma4->SetLineColor(kBlue);
   //leg->AddEntry(sigma4,"template","l");
   fDiffXSec4->Print();

   // ----------------
   // Radiated
   InSANERadiator<F1F209eInclusiveDiffXSec> * fDiffXSec2 = new  InSANERadiator<F1F209eInclusiveDiffXSec>();
   fDiffXSec2->SetIntegrationThreshold(1);
   fDiffXSec2->SetDeltaM(0.0);
   fDiffXSec2->SetBeamEnergy(Ebeam_mean);
   fDiffXSec2->SetTargetNucleus(InSANENucleus::Fe56());
   //fDiffXSec2->SetRegion4(true);
   fDiffXSec2->InitializePhaseSpaceVariables();
   fDiffXSec2->InitializeFinalStateParticles();
   fDiffXSec2->GetPhaseSpace()->GetVariableWithName("energy")->SetMinimum(0.01);
   fDiffXSec2->GetPhaseSpace()->GetVariableWithName("theta")->SetMinimum(0.0001);
   TF1 * sigma2 = new TF1("sigma2", fDiffXSec2, &InSANEInclusiveDiffXSec::PhotonEnergyDependentXSec, 
         Emin, Emax, npar,"InSANEInclusiveDiffXSec","PhotonEnergyDependentXSec");
   sigma2->SetParameter(0,theta_mean);
   sigma2->SetParameter(1,0.0);
   //sigma2->SetNpx(npx);
   sigma2->SetLineColor(kBlue);
   sigma2->SetLineStyle(2);

   TGraph * model1 = new TGraph(sigma4->DrawCopy("goff")->GetHistogram());
   //model1->SetLineColor(2);
   mg->Add(model1,"l");

   TGraph * model2 = new TGraph(sigma2->DrawCopy("goff")->GetHistogram());
   //model2->SetLineColor(4);
   mg->Add(model2,"l");

   InSANENucleus Al(13,27);
   fDiffXSec4->SetTargetNucleus(Al);
   fDiffXSec2->SetTargetNucleus(Al);

   model1 = new TGraph(sigma4->DrawCopy("goff")->GetHistogram());
   model1->SetLineColor(2);
   mg->Add(model1,"l");

   model1 = new TGraph(sigma2->DrawCopy("goff")->GetHistogram());
   model1->SetLineColor(2);
   model1->SetLineStyle(2);
   mg->Add(model1,"l");

   mg->Draw("a");

   c->SaveAs("/home/whit/public_html/images/cross_sections/Iron56_sigma.png");

   return 0;
}
