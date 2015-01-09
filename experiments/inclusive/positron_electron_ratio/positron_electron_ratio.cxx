Int_t positron_electron_ratio(){

   NucDBManager * dbman = NucDBManager::GetManager(1);
   if(!dbman) return -1;

   NucDBExperiment * exp = 0;
   NucDBMeasurement * meas = 0;

   exp = dbman->GetExperiment("SLAC-E143");
   if(!exp ) {
      exp = new NucDBExperiment("SLAC-E143","SLAC E143");
   }
   meas = exp->GetMeasurement("R_eplusOvereminus");
   if(meas) delete meas;
   meas = 0;
   if(!meas){
      meas= new NucDBMeasurement("R_eplusOvereminus","R(e+/e-)");
      exp->AddMeasurement(meas);
   }

   NucDBBinnedVariable    Ebeam("Ebeam","E_{beam}");
   NucDBBinnedVariable    x("x","x");
   NucDBBinnedVariable    Q2("Qsquared","Q^{2}");
   NucDBBinnedVariable    Pt("Pt","p_{T}");
   NucDBDiscreteVariable  A("A","A");
   NucDBDiscreteVariable  Z("Z","Z");

   NucDBDataPoint   * apoint = 0 ;
   NucDBDataPoint  point = NucDBDataPoint();
   point.AddBinVariable(&Ebeam);
   point.AddBinVariable(&Pt);
   point.AddBinVariable(&Q2);
   point.AddBinVariable(&x);

   std::ifstream file("experiments/inclusive/positron_electron_ratio/data_output/SLAC_E143NH3.dat");
   double xcol[8];
   while( !(file.eof()) ){
      for(int i = 0; i< 8;i++){
         file >> xcol[i] ;
      }

      Z.SetValue(xcol[0]);
      A.SetValue(xcol[1]);
      Ebeam.SetValueSize(xcol[2],0.001);
      x.SetValueSize(xcol[3],0.001);
      Q2.SetValueSize(xcol[4],0.001);
      Pt.SetValueSize(xcol[5],0.001);

      point.SetValue(xcol[6]); 
      // Set the errors
      point.GetStatError()->SetError(xcol[7]); 
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
   file.close();
   dbman->SaveExperiment(exp);
   return 0;
}

