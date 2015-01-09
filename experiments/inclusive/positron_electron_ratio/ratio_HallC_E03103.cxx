Int_t ratio_HallC_E03103(){

   NucDBManager * dbman = NucDBManager::GetManager(1);
   if(!dbman) return -1;

   NucDBExperiment * exp = 0;
   NucDBMeasurement * meas = 0;

   exp = dbman->GetExperiment("HallC-E03103");
   if(!exp ) {
      exp = new NucDBExperiment("HallC-E03103","HallC E03103");
   }
   meas = exp->GetMeasurement("R_eplusOvereminus");
   if(meas) delete meas;
   meas = 0;
   if(!meas){
      meas= new NucDBMeasurement("R_eplusOvereminus","R(e+/e-)");
      exp->AddMeasurement(meas);
   }

   std::ifstream file("experiments/inclusive/positron_electron_ratio/data_output/HallC_E03103.dat");

   NucDBBinnedVariable    Ebeam("Ebeam","E_{beam}");
   NucDBBinnedVariable    Eprime("E","E");
   NucDBBinnedVariable    theta("theta","#theta");
   NucDBBinnedVariable    x("x","x");
   NucDBBinnedVariable    Q2("Qsquared","Q^{2}");
   NucDBBinnedVariable    Pt("Pt","p_{T}");
   NucDBDiscreteVariable  A("A","A");
   NucDBDiscreteVariable  Z("Z","Z");
   NucDBVariable         rl("rl","Radiation Length");
   rl.SetValue(0.23);

   NucDBDataPoint   * apoint = 0 ;
   NucDBDataPoint  point = NucDBDataPoint();
   point.AddBinVariable(&Ebeam);
   point.AddBinVariable(&Eprime);
   point.AddBinVariable(&theta);
   point.AddBinVariable(&Pt);
   point.AddBinVariable(&Q2);
   point.AddBinVariable(&x);
   point.AddDiscreteVariable(&A);
   point.AddDiscreteVariable(&Z);
   point.AddVariable(&rl);

   double xcol[10];
   while( !(file.eof()) ){
      for(int i = 0; i< 10;i++){
         file >> xcol[i] ;
      }

      Z.SetValue(xcol[0]);
      A.SetValue(xcol[1]);
      Ebeam.SetValueSize(xcol[2],0.001);
      Eprime.SetValueSize(xcol[3],0.001);
      theta.SetValueSize(xcol[4]/degree,0.001);
      x.SetValueSize(xcol[5],0.001);
      Q2.SetValueSize(xcol[6],0.001);
      Pt.SetValueSize(xcol[7],0.001);

      point.SetValue(xcol[8]); 
      point.GetStatError()->SetError(xcol[9]); 
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
   meas->PrintData();
   file.close();
   dbman->SaveExperiment(exp);
   return 0;
}

