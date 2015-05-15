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
   if(!meas){
      meas= new NucDBMeasurement("sigma_12C(gamma,pi-)X","#frac{d#sigma}{dE_{#pi}d#Omega}");
      exp->AddMeasurement(meas);
   }
   meas->ClearDataPoints();

   NucDBBinnedVariable    Ebeam("Ebeam","E_{beam}");
   NucDBBinnedVariable    T("Tkin","T_{#pi}");
   NucDBBinnedVariable    Ppi("P","p_{#pi}");
   NucDBBinnedVariable    PTpi("PT","p_{T}");
   NucDBBinnedVariable    Epi("E","E_{#pi}");
   NucDBBinnedVariable    theta("theta","theta_{#pi}");

   NucDBDataPoint   * apoint = 0 ;
   NucDBDataPoint  point = NucDBDataPoint();
   point.AddBinVariable(&Ebeam);
   point.AddBinVariable(&theta);
   point.AddBinVariable(&T);
   point.AddBinVariable(&Epi);
   point.AddBinVariable(&Ppi);
   point.AddBinVariable(&PTpi);

   double scale = 1000.0;
   double x,y;

   std::ifstream file("experiments/Yerevan/pion_data_alanakian/alanakian_piminus_20deg.dat");
   while( !(file.eof()) ){
      file >> x >> y ;
      double e_tot     = M_pion/GeV + x;
      double p_pi      = TMath::Sqrt(e_tot*e_tot - M_pion*M_pion/(GeV*GeV));
      double theta_pi  = 20.0;
      double pt_pi     = p_pi*TMath::Sin(theta_pi*degree);

      theta.SetValueSize( theta_pi, 0.001);
      Ebeam.SetValueSize(   4.5, 0.001);
      Ppi.SetValueSize(    p_pi, 0.001);
      PTpi.SetValueSize(   pt_pi, 0.001);
      Epi.SetValueSize(   e_tot, 0.001);
      T.SetValueSize(         x, 0.001);

      // E/p is the jacobian dp/dE
      point.SetValue( (p_pi*p_pi)*y*scale/e_tot ); 
      // Set the errors
      point.SetStatError((p_pi*p_pi)*y*scale*0.05/e_tot); 
      point.CalculateTotalError();
      //point.Print();
      // copy the point and add it to the measurement
      meas->AddDataPoint(new NucDBDataPoint(point));
      //y_sigma.push_back(y); 
      //y_sigma.push_back(y/e_tot/e_tot/e_tot); 
      //y_sigma.push_back(TMath::Power(p_pi,3.0)*y); 
   }

   std::ifstream file("experiments/Yerevan/pion_data_alanakian/alanakian_piminus_40deg.dat");
   while( !(file.eof()) ){
      file >> x >> y ;
      double e_tot  = M_pion/GeV + x;
      double p_pi   = TMath::Sqrt(e_tot*e_tot - M_pion*M_pion/(GeV*GeV));
      double theta_pi  = 40.0;
      double pt_pi     = p_pi*TMath::Sin(theta_pi*degree);
      theta.SetValueSize( theta_pi, 0.001);
      Ebeam.SetValueSize(   4.5, 0.001);
      Ppi.SetValueSize(    p_pi, 0.001);
      PTpi.SetValueSize(   pt_pi, 0.001);
      Epi.SetValueSize(   e_tot, 0.001);
      T.SetValueSize(         x, 0.001);

      // E/p is the jacobian dp/dE
      point.SetValue( (p_pi*p_pi)*y*scale/e_tot ); 
      // Set the errors
      point.SetStatError((p_pi*p_pi)*y*0.05*scale/e_tot); 
      point.CalculateTotalError();
      //point.Print();
      // copy the point and add it to the measurement
      meas->AddDataPoint(new NucDBDataPoint(point));
      //y_sigma.push_back(y); 
      //y_sigma.push_back(y/e_tot/e_tot/e_tot); 
      //y_sigma.push_back(TMath::Power(p_pi,3.0)*y); 
   }

   std::ifstream file("experiments/Yerevan/pion_data_alanakian/alanakian_piminus_60deg.dat");
   while( !(file.eof()) ){
      file >> x >> y ;
      double e_tot  = M_pion/GeV + x;
      double p_pi   = TMath::Sqrt(e_tot*e_tot - M_pion*M_pion/(GeV*GeV));
      double theta_pi  = 60.0;
      double pt_pi     = p_pi*TMath::Sin(theta_pi*degree);
      theta.SetValueSize(  theta_pi, 0.001);
      Ebeam.SetValueSize(   4.5, 0.001);
      Ppi.SetValueSize(    p_pi, 0.001);
      PTpi.SetValueSize(   pt_pi, 0.001);
      Epi.SetValueSize(   e_tot, 0.001);
      T.SetValueSize(         x, 0.001);

      // E/p is the jacobian dp/dE
      point.SetValue( (p_pi*p_pi)*y*scale/e_tot ); 
      // Set the errors
      point.SetStatError((p_pi*p_pi)*y*0.05*scale/e_tot); 
      point.CalculateTotalError();
      //point.Print();
      // copy the point and add it to the measurement
      meas->AddDataPoint(new NucDBDataPoint(point));
      //y_sigma.push_back(y); 
      //y_sigma.push_back(y/e_tot/e_tot/e_tot); 
      //y_sigma.push_back(TMath::Power(p_pi,3.0)*y); 
   }

   std::ifstream file("experiments/Yerevan/pion_data_alanakian/alanakian_piminus_90deg.dat");
   double x,y;
   while( !(file.eof()) ){
      file >> x >> y ;
      double e_tot  = M_pion/GeV + x;
      double p_pi   = TMath::Sqrt(e_tot*e_tot - M_pion*M_pion/(GeV*GeV));
      double theta_pi  = 90.0;
      double pt_pi     = p_pi*TMath::Sin(theta_pi*degree);
      theta.SetValueSize(  theta_pi, 0.001);
      theta.SetValueSize(  90.0, 0.001);
      Ebeam.SetValueSize(   4.5, 0.001);
      Ppi.SetValueSize(    p_pi, 0.001);
      PTpi.SetValueSize(   pt_pi, 0.001);
      Epi.SetValueSize(   e_tot, 0.001);
      T.SetValueSize(         x, 0.001);

      // E/p is the jacobian dp/dE
      point.SetValue( (p_pi*p_pi)*y*scale/e_tot ); 
      // Set the errors
      point.SetStatError((p_pi*p_pi)*y*0.05*scale/e_tot); 
      point.CalculateTotalError();
      //point.Print();
      // copy the point and add it to the measurement
      meas->AddDataPoint(new NucDBDataPoint(point));
      //y_sigma.push_back(y); 
      //y_sigma.push_back(y/e_tot/e_tot/e_tot); 
      //y_sigma.push_back(TMath::Power(p_pi,3.0)*y); 
   }

   std::ifstream file("experiments/Yerevan/pion_data_alanakian/alanakian_piminus_120deg.dat");
   while( !(file.eof()) ){
      file >> x >> y ;
      double e_tot  = M_pion/GeV + x;
      double p_pi   = TMath::Sqrt(e_tot*e_tot - M_pion*M_pion/(GeV*GeV));
      double theta_pi  = 120.0;
      double pt_pi     = p_pi*TMath::Sin(theta_pi*degree);
      theta.SetValueSize(  theta_pi, 0.001);
      Ebeam.SetValueSize(   4.5, 0.001);
      Ppi.SetValueSize(    p_pi, 0.001);
      PTpi.SetValueSize(   pt_pi, 0.001);
      Epi.SetValueSize(   e_tot, 0.001);
      T.SetValueSize(         x, 0.001);

      // E/p is the jacobian dp/dE
      point.SetValue( (p_pi*p_pi)*y*scale/e_tot ); 
      // Set the errors
      point.SetStatError((p_pi*p_pi)*y*0.05*scale/e_tot); 
      point.CalculateTotalError();
      //point.Print();
      // copy the point and add it to the measurement
      meas->AddDataPoint(new NucDBDataPoint(point));
      //y_sigma.push_back(y); 
      //y_sigma.push_back(y/e_tot/e_tot/e_tot); 
      //y_sigma.push_back(TMath::Power(p_pi,3.0)*y); 
   }


   meas = exp->GetMeasurement("sigma_12C(gamma,pi+)X");
   if(!meas){
      meas= new NucDBMeasurement("sigma_12C(gamma,pi+)X","#frac{d#sigma}{dE_{#pi}d#Omega}");
      exp->AddMeasurement(meas);
   }
   meas->ClearDataPoints();

   std::ifstream file("experiments/Yerevan/pion_data_alanakian/alanakian_piplus_20deg.dat");
   while( !(file.eof()) ){
      file >> x >> y ;
      double e_tot  = M_pion/GeV + x;
      double p_pi   = TMath::Sqrt(e_tot*e_tot - M_pion*M_pion/(GeV*GeV));
      double theta_pi  = 20.0;
      double pt_pi     = p_pi*TMath::Sin(theta_pi*degree);
      theta.SetValueSize(  theta_pi, 0.001);
      Ebeam.SetValueSize(   4.5, 0.001);
      Ppi.SetValueSize(    p_pi, 0.001);
      PTpi.SetValueSize(   pt_pi, 0.001);
      Epi.SetValueSize(   e_tot, 0.001);
      T.SetValueSize(         x, 0.001);

      // E/p is the jacobian dp/dE
      point.SetValue( (p_pi*p_pi)*y*scale/e_tot ); 
      // Set the errors
      point.SetStatError((p_pi*p_pi)*y*scale*0.05/e_tot); 
      point.CalculateTotalError();
      //point.Print();
      // copy the point and add it to the measurement
      meas->AddDataPoint(new NucDBDataPoint(point));
      //y_sigma.push_back(y); 
      //y_sigma.push_back(y/e_tot/e_tot/e_tot); 
      //y_sigma.push_back(TMath::Power(p_pi,3.0)*y); 
   }

   std::ifstream file("experiments/Yerevan/pion_data_alanakian/alanakian_piplus_40deg.dat");
   while( !(file.eof()) ){
      file >> x >> y ;
      double e_tot  = M_pion/GeV + x;
      double p_pi   = TMath::Sqrt(e_tot*e_tot - M_pion*M_pion/(GeV*GeV));
      double theta_pi  = 40.0;
      double pt_pi     = p_pi*TMath::Sin(theta_pi*degree);
      theta.SetValueSize(  theta_pi, 0.001);
      Ebeam.SetValueSize(   4.5, 0.001);
      Ppi.SetValueSize(    p_pi, 0.001);
      PTpi.SetValueSize(   pt_pi, 0.001);
      Epi.SetValueSize(   e_tot, 0.001);
      T.SetValueSize(         x, 0.001);

      // E/p is the jacobian dp/dE
      point.SetValue( (p_pi*p_pi)*y*scale/e_tot ); 
      // Set the errors
      point.SetStatError((p_pi*p_pi)*y*0.05*scale/e_tot); 
      point.CalculateTotalError();
      //point.Print();
      // copy the point and add it to the measurement
      meas->AddDataPoint(new NucDBDataPoint(point));
      //y_sigma.push_back(y); 
      //y_sigma.push_back(y/e_tot/e_tot/e_tot); 
      //y_sigma.push_back(TMath::Power(p_pi,3.0)*y); 
   }

   std::ifstream file("experiments/Yerevan/pion_data_alanakian/alanakian_piplus_60deg.dat");
   while( !(file.eof()) ){
      file >> x >> y ;
      double e_tot  = M_pion/GeV + x;
      double p_pi   = TMath::Sqrt(e_tot*e_tot - M_pion*M_pion/(GeV*GeV));
      double theta_pi  = 60.0;
      double pt_pi     = p_pi*TMath::Sin(theta_pi*degree);
      theta.SetValueSize(  theta_pi, 0.001);
      Ebeam.SetValueSize(   4.5, 0.001);
      Ppi.SetValueSize(    p_pi, 0.001);
      PTpi.SetValueSize(   pt_pi, 0.001);
      Epi.SetValueSize(   e_tot, 0.001);
      T.SetValueSize(         x, 0.001);

      // E/p is the jacobian dp/dE
      point.SetValue( (p_pi*p_pi)*y*scale/e_tot ); 
      // Set the errors
      point.SetStatError((p_pi*p_pi)*y*0.05*scale/e_tot); 
      point.CalculateTotalError();
      //point.Print();
      // copy the point and add it to the measurement
      meas->AddDataPoint(new NucDBDataPoint(point));
      //y_sigma.push_back(y); 
      //y_sigma.push_back(y/e_tot/e_tot/e_tot); 
      //y_sigma.push_back(TMath::Power(p_pi,3.0)*y); 
   }

   std::ifstream file("experiments/Yerevan/pion_data_alanakian/alanakian_piplus_90deg.dat");
   double x,y;
   while( !(file.eof()) ){
      file >> x >> y ;
      double e_tot  = M_pion/GeV + x;
      double p_pi   = TMath::Sqrt(e_tot*e_tot - M_pion*M_pion/(GeV*GeV));
      double theta_pi  = 90.0;
      double pt_pi     = p_pi*TMath::Sin(theta_pi*degree);
      theta.SetValueSize(  theta_pi, 0.001);
      Ebeam.SetValueSize(   4.5, 0.001);
      Ppi.SetValueSize(    p_pi, 0.001);
      PTpi.SetValueSize(   pt_pi, 0.001);
      Epi.SetValueSize(   e_tot, 0.001);
      T.SetValueSize(         x, 0.001);

      // E/p is the jacobian dp/dE
      point.SetValue( (p_pi*p_pi)*y*scale/e_tot ); 
      // Set the errors
      point.SetStatError((p_pi*p_pi)*y*0.05*scale/e_tot); 
      point.CalculateTotalError();
      //point.Print();
      // copy the point and add it to the measurement
      meas->AddDataPoint(new NucDBDataPoint(point));
      //y_sigma.push_back(y); 
      //y_sigma.push_back(y/e_tot/e_tot/e_tot); 
      //y_sigma.push_back(TMath::Power(p_pi,3.0)*y); 
   }

   std::ifstream file("experiments/Yerevan/pion_data_alanakian/alanakian_piplus_120deg.dat");
   while( !(file.eof()) ){
      file >> x >> y ;
      double e_tot  = M_pion/GeV + x;
      double p_pi   = TMath::Sqrt(e_tot*e_tot - M_pion*M_pion/(GeV*GeV));
      double theta_pi  = 120.0;
      double pt_pi     = p_pi*TMath::Sin(theta_pi*degree);
      theta.SetValueSize(  theta_pi, 0.001);
      Ebeam.SetValueSize(   4.5, 0.001);
      Ppi.SetValueSize(    p_pi, 0.001);
      PTpi.SetValueSize(   pt_pi, 0.001);
      Epi.SetValueSize(   e_tot, 0.001);
      T.SetValueSize(         x, 0.001);

      // E/p is the jacobian dp/dE
      point.SetValue( (p_pi*p_pi)*y*scale/e_tot ); 
      // Set the errors
      point.SetStatError((p_pi*p_pi)*y*0.05*scale/e_tot); 
      point.CalculateTotalError();
      //point.Print();
      // copy the point and add it to the measurement
      meas->AddDataPoint(new NucDBDataPoint(point));
      //y_sigma.push_back(y); 
      //y_sigma.push_back(y/e_tot/e_tot/e_tot); 
      //y_sigma.push_back(TMath::Power(p_pi,3.0)*y); 
   }

   dbman->SaveExperiment(exp);
   return 0;
}

