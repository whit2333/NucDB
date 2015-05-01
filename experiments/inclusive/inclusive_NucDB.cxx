Int_t inclusive_NucDB(){

   NucDBManager * dbman = NucDBManager::GetManager(1);
   if(!dbman) return -1;

   NucDBExperiment * exp = 0;
   NucDBMeasurement * meas = 0;
   exp = dbman->GetExperiment("Yerevan");
   if(!exp ) {
      exp = new NucDBExperiment("Yerevan","Yerevan");
   }
   meas = exp->GetMeasurement("sigma_12C(gamma,pi-)X");
   if(meas) delete meas;
   meas = 0;
   if(!meas){
      meas= new NucDBMeasurement("sigma_12C(gamma,pi-)X","#frac{d#sigma}{dE_{#pi}d#Omega}");
      exp->AddMeasurement(meas);
   }

   NucDBBinnedVariable    Ebeam("Ebeam","E_{beam}");
   NucDBBinnedVariable    T("Tkin","T_{#pi}");
   NucDBBinnedVariable    Ppi("P","p_{#pi}");
   NucDBBinnedVariable    Epi("E","E_{#pi}");
   NucDBBinnedVariable    theta("theta","theta_{#pi}");

   NucDBDataPoint   * apoint = 0 ;
   NucDBDataPoint  point = NucDBDataPoint();
   point.AddBinVariable(&Ebeam);
   point.AddBinVariable(&theta);
   point.AddBinVariable(&T);
   point.AddBinVariable(&Epi);
   point.AddBinVariable(&Ppi);

   std::ifstream file("experiments/inclusive/pion_data_alanakian/alanakian_piminus_20deg.dat");
   double x,y;
   while( !(file.eof()) ){
      file >> x >> y ;
      double e_tot  = M_pion/GeV + x;
      double p_pi   = TMath::Sqrt(e_tot*e_tot - M_pion*M_pion/(GeV*GeV));
      theta.SetValueSize(20.0,0.001);
      Ebeam.SetValueSize(4.5,0.001);
      Ppi.SetValueSize(p_pi,0.001);
      Epi.SetValueSize(e_tot,0.001);
      T.SetValueSize(x,0.001);
      // E/p is the jacobian dp/dE
      point.SetValue((p_pi)*y); 
      // Set the errors
      point.SetStatError((p_pi)*y*0.05); 
      point.CalculateTotalError();
      //point.Print();
      // copy the point and add it to the measurement
      apoint = new NucDBDataPoint(point);
      //apoint->Print();
      //apoint->ListVariables();
      meas->AddDataPoint(apoint);
      //y_sigma.push_back(y); 
      //y_sigma.push_back(y/e_tot/e_tot/e_tot); 
      //y_sigma.push_back(TMath::Power(p_pi,3.0)*y); 
   }

   dbman->SaveExperiment(exp);
   return 0;
}

