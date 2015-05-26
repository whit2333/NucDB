Int_t inclusive_pion_NucDB(){

   NucDBManager * dbman = NucDBManager::GetManager(1);
   if(!dbman) return -1;

   NucDBExperiment * exp = 0;
   NucDBMeasurement * meas = 0;

   // ----------------------------------------------
   exp = dbman->GetExperiment("SLAC-WISER");
   if(!exp ) {
      exp = new NucDBExperiment("SLAC-WISER","SLAC-WISER");
   }

   // ----------------------------------------------
   //
   //NucDBReference * ref = new NucDBReference(
   //      "DESY inclusive pi- electro-production",
   //      "Production of Negative Pions from Hydrogen, Deuterium and Carbon by High-Energy Electrons");
   //ref->SetURL("http://inspirehep.net/record/108675");
   //ref->SetDOI("http://dx.doi.org/10.1016/0375-9474(76)90674-6");


   // ----------------------------------------------
   //
   NucDBBinnedVariable    Ebeam("Ebeam","E_{beam}");
   NucDBBinnedVariable    Tpi("Tkin","T_{#pi}");
   NucDBBinnedVariable    Ppi("P","p_{#pi}");
   NucDBBinnedVariable    PT("PT","p_{T#pi}");
   NucDBBinnedVariable    Epi("E","E_{#pi}");
   NucDBBinnedVariable    theta("theta","theta_{#pi}",13.0,1.57/2.0);
   NucDBDataPoint       * apoint = 0 ;
   NucDBDataPoint         point = NucDBDataPoint();
   point.AddBinVariable(&Ebeam);
   point.AddBinVariable(&theta);
   point.AddBinVariable(&Tpi);
   point.AddBinVariable(&Epi);
   point.AddBinVariable(&Ppi);
   point.AddBinVariable(&PT);

   double scale = 1000.0;
   double k0,P_max,Plab,P_T,P_L,k_min,y,ey1,ey2;
   std::string line;

   // ----------------------------------------------
   // p target  pi-
   meas = exp->GetMeasurement("sigma_p(gamma,pi-)X");
   if(!meas){
      meas= new NucDBMeasurement("sigma_p(gamma,pi-)X","#frac{d#sigma}{dE_{#pi}d#Omega}");
      exp->AddMeasurement(meas);
      //meas->AddRef(ref);
   }
   meas->ClearDataPoints();

   std::ifstream file("experiments/SLAC-WISER/wiser/pi_minus_proton.txt");
   if( !(file.good()) ) return -4;
   std::getline(file,line);

   while( !(file.eof()) ){

      file >> k0 >> P_max >> Plab >> P_T >> P_L >> k_min >> y >> ey1 ;

      if( file.eof() ) break;
      double th_pi = TMath::ASin(P_T/Plab) ;
      //double sym_err = (TMath::Abs(ey1) + TMath::Abs(ey2))/2.0;
      //double p_pi   = x;//TMath::Sqrt(e_tot*e_tot - M_pion*M_pion/(GeV*GeV));
      double E_pi   = TMath::Sqrt(Plab*Plab + M_pion*M_pion/(GeV*GeV));
      double T_pi   = E_pi - M_pion/(GeV);

      theta.SetValueSize(  th_pi/degree, 0.001);
      Ebeam.SetValueSize(   k0, 0.001);
      Ppi.SetValueSize(     Plab, 0.001);
      PT.SetValueSize(      P_T,  0.001);
      Epi.SetValueSize(     E_pi, 0.001);
      Tpi.SetValueSize(     T_pi, 0.001);

      point.SetValue( y*scale ); 
      point.SetStatError(ey1*scale); 
      point.CalculateTotalError();
      // copy the point and add it to the measurement
      meas->AddDataPoint(new NucDBDataPoint(point));
   }

   // ----------------------------------------------
   // d target  pi-
   meas = exp->GetMeasurement("sigma_d(gamma,pi-)X");
   if(!meas){
      meas= new NucDBMeasurement("sigma_d(gamma,pi-)X","#frac{d#sigma}{dE_{#pi}d#Omega}");
      exp->AddMeasurement(meas);
      //meas->AddRef(ref);
   }
   meas->ClearDataPoints();

   std::ifstream file("experiments/SLAC-WISER/wiser/pi_minus_deuteron.txt");
   if( !(file.good()) ) return -4;
   std::getline(file,line);

   while( !(file.eof()) ){

      file >> k0 >> P_max >> Plab >> P_T >> P_L >> k_min >> y >> ey1 ;

      if( file.eof() ) break;
      double th_pi = TMath::ASin(P_T/Plab) ;
      //double sym_err = (TMath::Abs(ey1) + TMath::Abs(ey2))/2.0;
      //double p_pi   = x;//TMath::Sqrt(e_tot*e_tot - M_pion*M_pion/(GeV*GeV));
      double E_pi   = TMath::Sqrt(Plab*Plab + M_pion*M_pion/(GeV*GeV));
      double T_pi   = E_pi - M_pion/(GeV);

      theta.SetValueSize(  th_pi/degree, 0.001);
      Ebeam.SetValueSize(   k0, 0.001);
      Ppi.SetValueSize(     Plab, 0.001);
      PT.SetValueSize(      P_T,  0.001);
      Epi.SetValueSize(     E_pi, 0.001);
      Tpi.SetValueSize(     T_pi, 0.001);

      point.SetValue( y*scale ); 
      point.SetStatError(ey1*scale); 
      point.CalculateTotalError();
      // copy the point and add it to the measurement
      meas->AddDataPoint(new NucDBDataPoint(point));
   }

   // ----------------------------------------------
   // p target  pi+
   meas = exp->GetMeasurement("sigma_p(gamma,pi+)X");
   if(!meas){
      meas= new NucDBMeasurement("sigma_p(gamma,pi+)X","#frac{d#sigma}{dE_{#pi}d#Omega}");
      exp->AddMeasurement(meas);
      //meas->AddRef(ref);
   }
   meas->ClearDataPoints();

   std::ifstream file("experiments/SLAC-WISER/wiser/pi_plus_proton.txt");
   if( !(file.good()) ) return -4;
   std::getline(file,line);
   std::getline(file,line);
   while( !(file.eof()) ){

      file >> k0 >> P_max >> Plab >> P_T >> P_L >> k_min >> y >> ey1 ;

      if( file.eof() ) break;
      double th_pi = TMath::ASin(P_T/Plab) ;
      //double sym_err = (TMath::Abs(ey1) + TMath::Abs(ey2))/2.0;
      //double p_pi   = x;//TMath::Sqrt(e_tot*e_tot - M_pion*M_pion/(GeV*GeV));
      double E_pi   = TMath::Sqrt(Plab*Plab + M_pion*M_pion/(GeV*GeV));
      double T_pi   = E_pi - M_pion/(GeV);

      theta.SetValueSize(  th_pi/degree, 0.001);
      Ebeam.SetValueSize(   k0, 0.001);
      Ppi.SetValueSize(     Plab, 0.001);
      PT.SetValueSize(      P_T,  0.001);
      Epi.SetValueSize(     E_pi, 0.001);
      Tpi.SetValueSize(     T_pi, 0.001);

      point.SetValue( y*scale ); 
      point.SetStatError(ey1*scale); 
      point.CalculateTotalError();
      // copy the point and add it to the measurement
      meas->AddDataPoint(new NucDBDataPoint(point));
   }

   // ----------------------------------------------
   // d target  pi+
   meas = exp->GetMeasurement("sigma_d(gamma,pi+)X");
   if(!meas){
      meas= new NucDBMeasurement("sigma_d(gamma,pi+)X","#frac{d#sigma}{dE_{#pi}d#Omega}");
      exp->AddMeasurement(meas);
      //meas->AddRef(ref);
   }
   meas->ClearDataPoints();

   std::ifstream file("experiments/SLAC-WISER/wiser/pi_plus_deuteron.txt");
   if( !(file.good()) ) return -4;
   std::getline(file,line);
   std::getline(file,line);
   while( !(file.eof()) ){

      file >> k0 >> P_max >> Plab >> P_T >> P_L >> k_min >> y >> ey1 ;

      if( file.eof() ) break;
      double th_pi = TMath::ASin(P_T/Plab) ;
      //double sym_err = (TMath::Abs(ey1) + TMath::Abs(ey2))/2.0;
      //double p_pi   = x;//TMath::Sqrt(e_tot*e_tot - M_pion*M_pion/(GeV*GeV));
      double E_pi   = TMath::Sqrt(Plab*Plab + M_pion*M_pion/(GeV*GeV));
      double T_pi   = E_pi - M_pion/(GeV);

      theta.SetValueSize(  th_pi/degree, 0.001);
      Ebeam.SetValueSize(   k0, 0.001);
      Ppi.SetValueSize(     Plab, 0.001);
      PT.SetValueSize(      P_T,  0.001);
      Epi.SetValueSize(     E_pi, 0.001);
      Tpi.SetValueSize(     T_pi, 0.001);

      point.SetValue( y*scale ); 
      point.SetStatError(ey1*scale); 
      point.CalculateTotalError();
      // copy the point and add it to the measurement
      meas->AddDataPoint(new NucDBDataPoint(point));
   }

   exp->Print();

   dbman->SaveExperiment(exp);
   return 0;
}

