Int_t inclusive_piminus_NucDB(){

   NucDBManager * dbman = NucDBManager::GetManager(1);
   if(!dbman) return -1;

   NucDBExperiment * exp = 0;
   NucDBMeasurement * meas = 0;

   // ----------------------------------------------
   exp = dbman->GetExperiment("DESY");
   if(!exp ) {
      exp = new NucDBExperiment("DESY","DESY");
   }

   // ----------------------------------------------
   //
   NucDBReference * ref = new NucDBReference(
         "DESY inclusive pi- electro-production",
         "Production of Negative Pions from Hydrogen, Deuterium and Carbon by High-Energy Electrons");
   ref->SetURL("http://inspirehep.net/record/108675");
   ref->SetDOI("http://dx.doi.org/10.1016/0375-9474(76)90674-6");


   // ----------------------------------------------
   //
   NucDBBinnedVariable    Ebeam("Ebeam","E_{beam}");
   NucDBBinnedVariable    Tpi("Tkin","T_{#pi}");
   NucDBBinnedVariable    Ppi("P","p_{#pi}");
   NucDBBinnedVariable    PTpi("PT","p_T");
   NucDBBinnedVariable    Epi("E","E_{#pi}");
   NucDBBinnedVariable    theta("theta","theta_{#pi}",13.0,1.57/2.0);
   NucDBDataPoint       * apoint = 0 ;
   NucDBDataPoint         point = NucDBDataPoint();
   point.AddBinVariable(&Ebeam);
   point.AddBinVariable(&theta);
   point.AddBinVariable(&Tpi);
   point.AddBinVariable(&Epi);
   point.AddBinVariable(&Ppi);
   point.AddBinVariable(&PTpi);

   double scale = 1000.0;
   double x,y,ey1,ey2;

   // ----------------------------------------------
   // C12 target 
   meas = exp->GetMeasurement("sigma_12C(e,pi-)X");
   if(!meas){
      meas= new NucDBMeasurement("sigma_12C(e,pi-)X","#frac{d#sigma}{dE_{#pi}d#Omega}");
      exp->AddMeasurement(meas);
      meas->AddRef(ref);
   }
   meas->ClearDataPoints();

   // E = 2.68
   std::ifstream file("experiments/DESY/inclusive/electroproduction_12C_pi_minus_13deg_2.68GeV.png.dat");
   if( !(file.good()) ) return -4;
   while( !(file.eof()) ){

      file >> x >> y >> ey1 >> ey2;

      double sym_err = (TMath::Abs(ey1) + TMath::Abs(ey2))/2.0;
      double p_pi   = x;//TMath::Sqrt(e_tot*e_tot - M_pion*M_pion/(GeV*GeV));
      double E_pi   = TMath::Sqrt(p_pi*p_pi + M_pion*M_pion/(GeV*GeV));
      double T_pi   = E_pi - M_pion/(GeV);
      double PT_pi  = TMath::Sin(13.0*degree)*p_pi;

      PTpi.SetValueSize(   PT_pi, 0.001);
      theta.SetValueSize(   13.0, 1.57/2.0);
      Ebeam.SetValueSize(   2.68, 0.001);
      Ppi.SetValueSize(     p_pi, 0.001);
      Epi.SetValueSize(     E_pi, 0.001);
      Tpi.SetValueSize(     T_pi, 0.001);

      point.SetValue( y*scale ); 
      point.SetStatError(sym_err*scale); 
      point.CalculateTotalError();
      // copy the point and add it to the measurement
      meas->AddDataPoint(new NucDBDataPoint(point));
   }

   // E = 3.5
   file = std::ifstream("experiments/DESY/inclusive/electroproduction_12C_pi_minus_13deg_3.5GeV.png.dat");
   if( !(file.good()) ) return -4;
   while( !(file.eof()) ){

      file >> x >> y >> ey1 >> ey2;

      double sym_err = (TMath::Abs(ey1) + TMath::Abs(ey2))/2.0;
      double p_pi   = x;//TMath::Sqrt(e_tot*e_tot - M_pion*M_pion/(GeV*GeV));
      double E_pi   = TMath::Sqrt(p_pi*p_pi + M_pion*M_pion/(GeV*GeV));
      double T_pi   = E_pi - M_pion/(GeV);
      double PT_pi  = TMath::Sin(13.0*degree)*p_pi;

      PTpi.SetValueSize(   PT_pi, 0.001);
      theta.SetValueSize(  13.0, 1.57/2.0);
      Ebeam.SetValueSize(   3.5, 0.001);
      Ppi.SetValueSize(     p_pi, 0.001);
      Epi.SetValueSize(     E_pi, 0.001);
      Tpi.SetValueSize(     T_pi, 0.001);

      point.SetValue( y*scale ); 
      point.SetStatError(sym_err*scale); 
      point.CalculateTotalError();
      // copy the point and add it to the measurement
      meas->AddDataPoint(new NucDBDataPoint(point));
   }


   // E = 4.5
   file = std::ifstream("experiments/DESY/inclusive/electroproduction_12C_pi_minus_13deg_4.5GeV.png.dat");
   if( file.good() ) while( !(file.eof()) ){

      file >> x >> y >> ey1 >> ey2;

      double sym_err = (TMath::Abs(ey1) + TMath::Abs(ey2))/2.0;
      double p_pi   = x;//TMath::Sqrt(e_tot*e_tot - M_pion*M_pion/(GeV*GeV));
      double E_pi   = TMath::Sqrt(p_pi*p_pi + M_pion*M_pion/(GeV*GeV));
      double T_pi   = E_pi - M_pion/(GeV);
      double PT_pi  = TMath::Sin(13.0*degree)*p_pi;

      PTpi.SetValueSize(   PT_pi, 0.001);

      theta.SetValueSize(  13.0, 1.57/2.0);
      Ebeam.SetValueSize(   4.5, 0.001);
      Ppi.SetValueSize(     p_pi, 0.001);
      Epi.SetValueSize(     E_pi, 0.001);
      Tpi.SetValueSize(     T_pi, 0.001);

      point.SetValue( y*scale ); 
      point.SetStatError(sym_err*scale); 
      point.CalculateTotalError();
      // copy the point and add it to the measurement
      meas->AddDataPoint(new NucDBDataPoint(point));
   }

   // E = 5.0
   file = std::ifstream("experiments/DESY/inclusive/electroproduction_12C_pi_minus_13deg_5.0GeV.png.dat");
   if( !(file.good()) ) return -4;
   while( !(file.eof()) ){

      file >> x >> y >> ey1 >> ey2;

      double sym_err = (TMath::Abs(ey1) + TMath::Abs(ey2))/2.0;
      double p_pi   = x;//TMath::Sqrt(e_tot*e_tot - M_pion*M_pion/(GeV*GeV));
      double E_pi   = TMath::Sqrt(p_pi*p_pi + M_pion*M_pion/(GeV*GeV));
      double T_pi   = E_pi - M_pion/(GeV);
      double PT_pi  = TMath::Sin(13.0*degree)*p_pi;

      PTpi.SetValueSize(   PT_pi, 0.001);

      theta.SetValueSize(  13.0, 1.57/2.0);
      Ebeam.SetValueSize(   5.0, 0.001);
      Ppi.SetValueSize(     p_pi, 0.001);
      Epi.SetValueSize(     E_pi, 0.001);
      Tpi.SetValueSize(     T_pi, 0.001);

      point.SetValue( y*scale ); 
      point.SetStatError(sym_err*scale); 
      point.CalculateTotalError();
      // copy the point and add it to the measurement
      meas->AddDataPoint(new NucDBDataPoint(point));
   }

   // E = 6.0
   file = std::ifstream("experiments/DESY/inclusive/electroproduction_12C_pi_minus_13deg_6.0GeV.png.dat");
   if( !(file.good()) ) return -4;
   while( !(file.eof()) ){

      file >> x >> y >> ey1 >> ey2;

      double sym_err = (TMath::Abs(ey1) + TMath::Abs(ey2))/2.0;
      double p_pi   = x;//TMath::Sqrt(e_tot*e_tot - M_pion*M_pion/(GeV*GeV));
      double E_pi   = TMath::Sqrt(p_pi*p_pi + M_pion*M_pion/(GeV*GeV));
      double T_pi   = E_pi - M_pion/(GeV);
      double PT_pi  = TMath::Sin(13.0*degree)*p_pi;

      PTpi.SetValueSize(   PT_pi, 0.001);

      theta.SetValueSize(  13.0, 1.57/2.0);
      Ebeam.SetValueSize(   6.0, 0.001);
      Ppi.SetValueSize(     p_pi, 0.001);
      Epi.SetValueSize(     E_pi, 0.001);
      Tpi.SetValueSize(     T_pi, 0.001);

      point.SetValue( y*scale ); 
      point.SetStatError(sym_err*scale); 
      point.CalculateTotalError();
      // copy the point and add it to the measurement
      meas->AddDataPoint(new NucDBDataPoint(point));
   }


   // ----------------------------------------------
   // proton target
   meas = exp->GetMeasurement("sigma_p(e,pi-)X");
   if(!meas){
      meas= new NucDBMeasurement("sigma_p(e,pi-)X","#frac{d#sigma}{dE_{#pi}d#Omega}");
      exp->AddMeasurement(meas);
      meas->AddRef(ref);
   }
   meas->ClearDataPoints();

   // E = 2.68
   file = std::ifstream("experiments/DESY/inclusive/electroproduction_p_pi_minus_13deg_2.68GeV.png.dat");
   if( !(file.good()) ) return -4;
   while( !(file.eof()) ){

      file >> x >> y >> ey1 >> ey2;

      double sym_err = (TMath::Abs(ey1) + TMath::Abs(ey2))/2.0;
      double p_pi   = x;//TMath::Sqrt(e_tot*e_tot - M_pion*M_pion/(GeV*GeV));
      double E_pi   = TMath::Sqrt(p_pi*p_pi + M_pion*M_pion/(GeV*GeV));
      double T_pi   = E_pi - M_pion/(GeV);
      double PT_pi  = TMath::Sin(13.0*degree)*p_pi;

      PTpi.SetValueSize(   PT_pi, 0.001);

      theta.SetValueSize(  13.0, 1.57/2.0);
      Ebeam.SetValueSize(   2.68, 0.001);
      Ppi.SetValueSize(     p_pi, 0.001);
      Epi.SetValueSize(     E_pi, 0.001);
      Tpi.SetValueSize(     T_pi, 0.001);

      // E/p is the jacobian dp/dE
      point.SetValue( y*scale ); 
      point.SetStatError(sym_err*scale); 
      point.CalculateTotalError();
      // copy the point and add it to the measurement
      meas->AddDataPoint(new NucDBDataPoint(point));
   }

   // E = 3.5
   file = std::ifstream("experiments/DESY/inclusive/electroproduction_p_pi_minus_13deg_3.5GeV.png.dat");
   if( !(file.good()) ) return -4;
   while( !(file.eof()) ){

      file >> x >> y >> ey1 >> ey2;

      double sym_err = (TMath::Abs(ey1) + TMath::Abs(ey2))/2.0;
      double p_pi   = x;//TMath::Sqrt(e_tot*e_tot - M_pion*M_pion/(GeV*GeV));
      double E_pi   = TMath::Sqrt(p_pi*p_pi + M_pion*M_pion/(GeV*GeV));
      double T_pi   = E_pi - M_pion/(GeV);
      double PT_pi  = TMath::Sin(13.0*degree)*p_pi;

      PTpi.SetValueSize(   PT_pi, 0.001);

      theta.SetValueSize(  13.0, 1.57/2.0);
      Ebeam.SetValueSize(    3.5, 0.001);
      Ppi.SetValueSize(     p_pi, 0.001);
      Epi.SetValueSize(     E_pi, 0.001);
      Tpi.SetValueSize(     T_pi, 0.001);

      // E/p is the jacobian dp/dE
      point.SetValue( y*scale ); 
      point.SetStatError(sym_err*scale); 
      point.CalculateTotalError();
      // copy the point and add it to the measurement
      meas->AddDataPoint(new NucDBDataPoint(point));
   }

   // E = 4.5
   file = std::ifstream("experiments/DESY/inclusive/electroproduction_p_pi_minus_13deg_4.5GeV.png.dat");
   if( !(file.good()) ) return -4;
   while( !(file.eof()) ){

      file >> x >> y >> ey1 >> ey2;

      double sym_err = (TMath::Abs(ey1) + TMath::Abs(ey2))/2.0;
      double p_pi   = x;//TMath::Sqrt(e_tot*e_tot - M_pion*M_pion/(GeV*GeV));
      double E_pi   = TMath::Sqrt(p_pi*p_pi + M_pion*M_pion/(GeV*GeV));
      double T_pi   = E_pi - M_pion/(GeV);
      double PT_pi  = TMath::Sin(13.0*degree)*p_pi;

      PTpi.SetValueSize(   PT_pi, 0.001);

      theta.SetValueSize(  13.0, 1.57/2.0);
      Ebeam.SetValueSize(    4.5, 0.001);
      Ppi.SetValueSize(     p_pi, 0.001);
      Epi.SetValueSize(     E_pi, 0.001);
      Tpi.SetValueSize(     T_pi, 0.001);

      // E/p is the jacobian dp/dE
      point.SetValue( y*scale ); 
      point.SetStatError(sym_err*scale); 
      point.CalculateTotalError();
      // copy the point and add it to the measurement
      meas->AddDataPoint(new NucDBDataPoint(point));
   }

   // E = 5.0
   file = std::ifstream("experiments/DESY/inclusive/electroproduction_p_pi_minus_13deg_5.0GeV.png.dat");
   if( !(file.good()) ) return -4;
   while( !(file.eof()) ){

      file >> x >> y >> ey1 >> ey2;

      double sym_err = (TMath::Abs(ey1) + TMath::Abs(ey2))/2.0;
      double p_pi   = x;//TMath::Sqrt(e_tot*e_tot - M_pion*M_pion/(GeV*GeV));
      double E_pi   = TMath::Sqrt(p_pi*p_pi + M_pion*M_pion/(GeV*GeV));
      double T_pi   = E_pi - M_pion/(GeV);
      double PT_pi  = TMath::Sin(13.0*degree)*p_pi;

      PTpi.SetValueSize(   PT_pi, 0.001);

      theta.SetValueSize(  13.0, 1.57/2.0);
      Ebeam.SetValueSize(    5.0, 0.001);
      Ppi.SetValueSize(     p_pi, 0.001);
      Epi.SetValueSize(     E_pi, 0.001);
      Tpi.SetValueSize(     T_pi, 0.001);

      // E/p is the jacobian dp/dE
      point.SetValue( y*scale ); 
      point.SetStatError(sym_err*scale); 
      point.CalculateTotalError();
      // copy the point and add it to the measurement
      meas->AddDataPoint(new NucDBDataPoint(point));
   }

   // E = 6.0
   file = std::ifstream("experiments/DESY/inclusive/electroproduction_p_pi_minus_13deg_6.0GeV.png.dat");
   if( !(file.good()) )  return -4;
   while( !(file.eof()) ){

      file >> x >> y >> ey1 >> ey2;

      double sym_err = (TMath::Abs(ey1) + TMath::Abs(ey2))/2.0;
      double p_pi   = x;//TMath::Sqrt(e_tot*e_tot - M_pion*M_pion/(GeV*GeV));
      double E_pi   = TMath::Sqrt(p_pi*p_pi + M_pion*M_pion/(GeV*GeV));
      double T_pi   = E_pi - M_pion/(GeV);
      double PT_pi  = TMath::Sin(13.0*degree)*p_pi;

      PTpi.SetValueSize(   PT_pi, 0.001);

      theta.SetValueSize(  13.0, 1.57/2.0);
      Ebeam.SetValueSize(    6.0, 0.001);
      Ppi.SetValueSize(     p_pi, 0.001);
      Epi.SetValueSize(     E_pi, 0.001);
      Tpi.SetValueSize(     T_pi, 0.001);

      // E/p is the jacobian dp/dE
      point.SetValue( y*scale ); 
      point.SetStatError(sym_err*scale); 
      point.CalculateTotalError();
      // copy the point and add it to the measurement
      meas->AddDataPoint(new NucDBDataPoint(point));
   }


   // ----------------------------------------------
   // deuteron target
   meas = exp->GetMeasurement("sigma_d(e,pi-)X");
   if(!meas){
      meas= new NucDBMeasurement("sigma_d(e,pi-)X","#frac{d#sigma}{dE_{#pi}d#Omega}");
      exp->AddMeasurement(meas);
      meas->AddRef(ref);
   }
   meas->ClearDataPoints();

   // E = 3.5
   file = std::ifstream("experiments/DESY/inclusive/electroproduction_d_pi_minus_13deg_3.5GeV.png.dat");
   if( !(file.good()) ) return -4;
   while( !(file.eof()) ){

      file >> x >> y >> ey1 >> ey2;

      double sym_err = (TMath::Abs(ey1) + TMath::Abs(ey2))/2.0;
      double p_pi   = x;//TMath::Sqrt(e_tot*e_tot - M_pion*M_pion/(GeV*GeV));
      double E_pi   = TMath::Sqrt(p_pi*p_pi + M_pion*M_pion/(GeV*GeV));
      double T_pi   = E_pi - M_pion/(GeV);
      double PT_pi  = TMath::Sin(13.0*degree)*p_pi;

      PTpi.SetValueSize(   PT_pi, 0.001);

      theta.SetValueSize(  13.0, 1.57/2.0);
      Ebeam.SetValueSize(    3.5, 0.001);
      Ppi.SetValueSize(     p_pi, 0.001);
      Epi.SetValueSize(     E_pi, 0.001);
      Tpi.SetValueSize(     T_pi, 0.001);

      // E/p is the jacobian dp/dE
      point.SetValue( y*scale ); 
      point.SetStatError(sym_err*scale); 
      point.CalculateTotalError();
      // copy the point and add it to the measurement
      meas->AddDataPoint(new NucDBDataPoint(point));
   }

   // E = 4.5
   file = std::ifstream("experiments/DESY/inclusive/electroproduction_d_pi_minus_13deg_4.5GeV.png.dat");
   if( !(file.good()) ) return -4;
   while( !(file.eof()) ){

      file >> x >> y >> ey1 >> ey2;

      double sym_err = (TMath::Abs(ey1) + TMath::Abs(ey2))/2.0;
      double p_pi   = x;//TMath::Sqrt(e_tot*e_tot - M_pion*M_pion/(GeV*GeV));
      double E_pi   = TMath::Sqrt(p_pi*p_pi + M_pion*M_pion/(GeV*GeV));
      double T_pi   = E_pi - M_pion/(GeV);
      double PT_pi  = TMath::Sin(13.0*degree)*p_pi;

      PTpi.SetValueSize(   PT_pi, 0.001);

      theta.SetValueSize(  13.0, 1.57/2.0);
      Ebeam.SetValueSize(    4.5, 0.001);
      Ppi.SetValueSize(     p_pi, 0.001);
      Epi.SetValueSize(     E_pi, 0.001);
      Tpi.SetValueSize(     T_pi, 0.001);

      // E/p is the jacobian dp/dE
      point.SetValue( y*scale ); 
      point.SetStatError(sym_err*scale); 
      point.CalculateTotalError();
      // copy the point and add it to the measurement
      meas->AddDataPoint(new NucDBDataPoint(point));
   }

   // E = 5.0
   file = std::ifstream("experiments/DESY/inclusive/electroproduction_d_pi_minus_13deg_5.0GeV.png.dat");
   if( !(file.good()) ) return -4;
   while( !(file.eof()) ){

      file >> x >> y >> ey1 >> ey2;

      double sym_err = (TMath::Abs(ey1) + TMath::Abs(ey2))/2.0;
      double p_pi   = x;//TMath::Sqrt(e_tot*e_tot - M_pion*M_pion/(GeV*GeV));
      double E_pi   = TMath::Sqrt(p_pi*p_pi + M_pion*M_pion/(GeV*GeV));
      double T_pi   = E_pi - M_pion/(GeV);
      double PT_pi  = TMath::Sin(13.0*degree)*p_pi;

      PTpi.SetValueSize(   PT_pi, 0.001);

      theta.SetValueSize(  13.0, 1.57/2.0);
      Ebeam.SetValueSize(    5.0, 0.001);
      Ppi.SetValueSize(     p_pi, 0.001);
      Epi.SetValueSize(     E_pi, 0.001);
      Tpi.SetValueSize(     T_pi, 0.001);

      // E/p is the jacobian dp/dE
      point.SetValue( y*scale ); 
      point.SetStatError(sym_err*scale); 
      point.CalculateTotalError();
      // copy the point and add it to the measurement
      meas->AddDataPoint(new NucDBDataPoint(point));
   }

   // E = 6.0
   file = std::ifstream("experiments/DESY/inclusive/electroproduction_d_pi_minus_13deg_6.0GeV.png.dat");
   if( !(file.good()) )  return -4;
   while( !(file.eof()) ){

      file >> x >> y >> ey1 >> ey2;

      double sym_err = (TMath::Abs(ey1) + TMath::Abs(ey2))/2.0;
      double p_pi   = x;//TMath::Sqrt(e_tot*e_tot - M_pion*M_pion/(GeV*GeV));
      double E_pi   = TMath::Sqrt(p_pi*p_pi + M_pion*M_pion/(GeV*GeV));
      double T_pi   = E_pi - M_pion/(GeV);
      double PT_pi  = TMath::Sin(13.0*degree)*p_pi;

      PTpi.SetValueSize(   PT_pi, 0.001);

      theta.SetValueSize(  13.0, 1.57/2.0);
      Ebeam.SetValueSize(    6.0, 0.001);
      Ppi.SetValueSize(     p_pi, 0.001);
      Epi.SetValueSize(     E_pi, 0.001);
      Tpi.SetValueSize(     T_pi, 0.001);

      // E/p is the jacobian dp/dE
      point.SetValue( y*scale ); 
      point.SetStatError(sym_err*scale); 
      point.CalculateTotalError();
      // copy the point and add it to the measurement
      meas->AddDataPoint(new NucDBDataPoint(point));
   }



   exp->Print();

   dbman->SaveExperiment(exp);
   return 0;
}

