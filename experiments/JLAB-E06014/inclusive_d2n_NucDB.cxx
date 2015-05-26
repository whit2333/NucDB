Int_t inclusive_d2n_NucDB(){

   NucDBManager * dbman = NucDBManager::GetManager(1);
   if(!dbman) return -1;

   NucDBExperiment  * exp = 0;
   NucDBMeasurement * meas = 0;

   exp = dbman->GetExperiment("JLAB-E06014");
   if(!exp ) {
      exp = new NucDBExperiment("JLAB-E06014","JLAB-E06014");
   }

   NucDBBinnedVariable    Ebeam("Ebeam","E_{beam}",5.9,0.001);    // unknown bin size
   NucDBBinnedVariable    theta("theta","#theta_{#pi}",45.0,0.25); // unknown bin size
   NucDBBinnedVariable    T(  "T","T_{#pi}"); // unknown bin size
   NucDBBinnedVariable    Ppi("P","p_{#pi}");
   NucDBBinnedVariable    PTpi("PT","p_{T}");
   NucDBBinnedVariable    Epi("E","E_{#pi}");

   NucDBDataPoint   * apoint = 0 ;
   NucDBDataPoint  point = NucDBDataPoint();
   point.AddBinVariable(&Ebeam);
   point.AddBinVariable(&theta);
   point.AddBinVariable(&T);
   point.AddBinVariable(&Epi);
   point.AddBinVariable(&Ppi);
   point.AddBinVariable(&PTpi);

   double th = 45.0*degree; 
   double col[3]; 

   // -----------------------------------------------------------
   // 
   meas = exp->GetMeasurement("sigma_3He(e,pi+)X");
   if(!meas){
      meas= new NucDBMeasurement("sigma_3He(e,pi+)X","#frac{d#sigma}{dp_{#pi}d#Omega}");
      exp->AddMeasurement(meas);
   }
   meas->ClearDataPoints();

   // -----------------------------------------------------------
   // 5.9 GeV beam data
   // < P (MeV) > < xs (pb/MeV/sr) > < stat error (pb/MeV/sr) > 
   std::ifstream file("experiments/JLAB-E06014/pion/Positive/cs_5-pass.dat");
   while( !(file.eof()) ){

      file >> col[0] >> col[1] >> col[2];
      if(file.eof())  break;

      double p_pi   = col[0]/1000.0;
      double e_tot  = TMath::Sqrt(p_pi*p_pi + M_pion*M_pion/(GeV*GeV));
      double Tkine  = e_tot - M_pion/GeV;
      double pt     = p_pi*TMath::Sin(th);

      Ebeam.SetValueSize(   5.9, 0.001);
      Ppi.SetValueSize(    p_pi, 0.001);
      Epi.SetValueSize(   e_tot, 0.001);
      T.SetValueSize(     Tkine, 0.001);
      PTpi.SetValueSize(     pt, 0.001);

      point.SetValue( col[1] ); 
      point.SetStatError(col[2]); 
      point.CalculateTotalError();
      meas->AddDataPoint(new NucDBDataPoint(point));
   }
   file.close();

   // -----------------------------------------------------------
   // 4.7 GeV beam data
   // < P (MeV) > < xs (pb/MeV/sr) > < stat error (pb/MeV/sr) > 
   std::ifstream file("experiments/JLAB-E06014/pion/Positive/cs_4-pass.dat");
   while( !(file.eof()) ){

      file >> col[0] >> col[1] >> col[2];
      if(file.eof())  break;

      double p_pi   = col[0]/1000.0;
      double e_tot  = TMath::Sqrt(p_pi*p_pi + M_pion*M_pion/(GeV*GeV));
      double Tkine  = e_tot - M_pion/GeV;
      double pt     = p_pi*TMath::Sin(th);

      Ebeam.SetValueSize(   4.7, 0.001);
      Ppi.SetValueSize(    p_pi, 0.001);
      Epi.SetValueSize(   e_tot, 0.001);
      T.SetValueSize(     Tkine, 0.001);
      PTpi.SetValueSize(     pt, 0.001);

      point.SetValue( col[1] ); 
      point.SetStatError(col[2]); 
      point.CalculateTotalError();
      meas->AddDataPoint(new NucDBDataPoint(point));
   }
   file.close();



   // -----------------------------------------------------------
   // 
   meas = exp->GetMeasurement("sigma_3He(e,pi-)X");
   if(!meas){
      meas= new NucDBMeasurement("sigma_3He(e,pi-)X","#frac{d#sigma}{dp_{#pi}d#Omega}");
      exp->AddMeasurement(meas);
   }
   meas->ClearDataPoints();
   // -----------------------------------------------------------
   // 5.9 GeV beam data
   // < P (MeV) > < xs (pb/MeV/sr) > < stat error (pb/MeV/sr) > 
   std::ifstream file("experiments/JLAB-E06014/pion/Negative/cs_5-pass.dat");
   while( !(file.eof()) ){

      file >> col[0] >> col[1] >> col[2];
      if(file.eof())  break;

      double p_pi   = col[0]/1000.0;
      double e_tot  = TMath::Sqrt(p_pi*p_pi + M_pion*M_pion/(GeV*GeV));
      double Tkine  = e_tot - M_pion/GeV;
      double pt     = p_pi*TMath::Sin(th);

      Ebeam.SetValueSize(   5.9, 0.001);
      Ppi.SetValueSize(    p_pi, 0.001);
      Epi.SetValueSize(   e_tot, 0.001);
      T.SetValueSize(     Tkine, 0.001);
      PTpi.SetValueSize(     pt, 0.001);

      point.SetValue( col[1] ); 
      point.SetStatError(col[2]); 
      point.CalculateTotalError();
      meas->AddDataPoint(new NucDBDataPoint(point));
   }
   file.close();

   // -----------------------------------------------------------
   // 4.7 GeV beam data
   // < P (MeV) > < xs (pb/MeV/sr) > < stat error (pb/MeV/sr) > 
   std::ifstream file("experiments/JLAB-E06014/pion/Negative/cs_4-pass.dat");
   while( !(file.eof()) ){

      file >> col[0] >> col[1] >> col[2];
      if(file.eof())  break;

      double p_pi   = col[0]/1000.0;
      double e_tot  = TMath::Sqrt(p_pi*p_pi + M_pion*M_pion/(GeV*GeV));
      double Tkine  = e_tot - M_pion/GeV;
      double pt     = p_pi*TMath::Sin(th);

      Ebeam.SetValueSize(   4.7, 0.001);
      Ppi.SetValueSize(    p_pi, 0.001);
      Epi.SetValueSize(   e_tot, 0.001);
      T.SetValueSize(     Tkine, 0.001);
      PTpi.SetValueSize(     pt, 0.001);

      point.SetValue( col[1] ); 
      point.SetStatError(col[2]); 
      point.CalculateTotalError();
      meas->AddDataPoint(new NucDBDataPoint(point));
   }
   file.close();


   dbman->SaveExperiment(exp);
   return 0;
}

