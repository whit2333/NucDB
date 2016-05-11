
void add_DVCS_data(const char * fname, std::vector<NucDBMeasurement*> meas)
{
   NucDBBinnedVariable  x_var(  "x","x");
   NucDBBinnedVariable  t_var(  "t","t");
   NucDBBinnedVariable  Q2_var( "Qsquared","Q^{2}");
   NucDBBinnedVariable  phi_var("phi","#phi");

   NucDBDataPoint  * apoint = 0 ;
   NucDBDataPoint  point = NucDBDataPoint();
   point.AddBinVariable(  &x_var);
   point.AddBinVariable(  &t_var);
   point.AddBinVariable( &Q2_var);
   point.AddBinVariable(&phi_var);

   int Nmeas = meas.size();
   const int Ncols = 5;
   std::array<double,Ncols> sig    ;
   std::array<double,Ncols> eStat  ;
   std::array<double,Ncols> eSys1  ;
   std::array<double,Ncols> eSys2  ;

   std::string line;
   std::stringstream iss(line);
   // ----------------------------------------------------------------
   //
   afile = std::ifstream(fname);

   std::getline(afile, line);
   std::cout << line;
   //iss = std::stringstream(line);
   //for(int i = 0; i< Ncols; i++ ) {
   //   iss >> x[i];
   //}
   std::getline(afile, line);
   //iss = std::stringstream(line);
   //for(int i = 0; i< Ncols; i++ ) {
   //   iss >> Q2[i];
   //}
   std::getline(afile, line);
   //iss = std::stringstream(line);
   //for(int i = 0; i< Ncols; i++ ) {
   //   iss >> t[i];
   //}

   // t
   while (std::getline(afile, line))
   {
      iss = std::stringstream(line);
      double var_min, var_max;
      double minust, xb, Q2; 

      if(!(iss >> var_min)) {break;}
      if(!(iss >> var_max)) {break;}

      if(!(iss >> minust)) {break;}
      if(!(iss >> xb)) {break;}
      if(!(iss >> Q2)) {break;}

      std::cout << line << std::endl;

      int N_cols_read = 0;
      double x1,x2,x3;
      for(int i = 0; i< Nmeas; i++ ) {
         if(!(iss >> x1)) {break;}
         if(!(iss >> x2)) {break;}
         if(!(iss >> x3)) {break;}
         sig[i]   = x1;
         eStat[i] = x2;
         eSys1[i] = x3;
         N_cols_read++;
      }
      x_var.SetValueSize(         xb, 0.001);
      t_var.SetValueSize(-1.0*minust, 0.001);
      Q2_var.SetValueSize(        Q2, 0.001);
      t_var.SetMeanLimits( -1.0*minust, var_min, var_max );

      for(int i = 0; i< N_cols_read; i++ ) {
         point.SetValue( sig[i]  ); 
         // Set the errors
         point.SetStatError( eStat[i] ); 
         point.SetSystError( NucDBErrorBar(eSys1[i]) ); 
         point.CalculateTotalError();
         //point.Print();
         // copy the point and add it to the measurement
         meas[i]->AddDataPoint(new NucDBDataPoint(point));
      }
   }


   // xb
   std::getline(afile, line);
   while (std::getline(afile, line))
   {
      iss = std::stringstream(line);
      double var_min, var_max;
      double minust, xb, Q2; 

      if(!(iss >> var_min)) {break;}
      if(!(iss >> var_max)) {break;}

      if(!(iss >> minust)) {break;}
      if(!(iss >> xb)) {break;}
      if(!(iss >> Q2)) {break;}

      std::cout << line << std::endl;

      int N_cols_read = 0;
      double x1,x2,x3;
      for(int i = 0; i< Nmeas; i++ ) {
         if(!(iss >> x1)) {break;}
         if(!(iss >> x2)) {break;}
         if(!(iss >> x3)) {break;}
         sig[i]   = x1;
         eStat[i] = x2;
         eSys1[i] = x3;
         N_cols_read++;
      }
      x_var.SetValueSize(         xb, 0.001);
      t_var.SetValueSize(-1.0*minust, 0.001);
      Q2_var.SetValueSize(        Q2, 0.001);
      x_var.SetMeanLimits( xb, var_min, var_max );

      for(int i = 0; i< N_cols_read; i++ ) {
         point.SetValue( sig[i]  ); 
         // Set the errors
         point.SetStatError( eStat[i] ); 
         point.SetSystError( NucDBErrorBar(eSys1[i]) ); 
         point.CalculateTotalError();
         //point.Print();
         // copy the point and add it to the measurement
         meas[i]->AddDataPoint(new NucDBDataPoint(point));
      }
   }

   // xb
   std::getline(afile, line);
   while (std::getline(afile, line))
   {
      iss = std::stringstream(line);
      double var_min, var_max;
      double minust, xb, Q2; 

      if(!(iss >> var_min)) {break;}
      if(!(iss >> var_max)) {break;}

      if(!(iss >> minust)) {break;}
      if(!(iss >> xb)) {break;}
      if(!(iss >> Q2)) {break;}

      std::cout << line << std::endl;

      int N_cols_read = 0;
      double x1,x2,x3;
      for(int i = 0; i< Nmeas; i++ ) {
         if(!(iss >> x1)) {break;}
         if(!(iss >> x2)) {break;}
         if(!(iss >> x3)) {break;}
         sig[i]   = x1;
         eStat[i] = x2;
         eSys1[i] = x3;
         N_cols_read++;
      }
      x_var.SetValueSize(         xb, 0.001);
      t_var.SetValueSize(-1.0*minust, 0.001);
      Q2_var.SetValueSize(        Q2, 0.001);
      Q2_var.SetMeanLimits( Q2, var_min, var_max );

      for(int i = 0; i< N_cols_read; i++ ) {
         point.SetValue( sig[i]  ); 
         // Set the errors
         point.SetStatError( eStat[i] ); 
         point.SetSystError( NucDBErrorBar(eSys1[i]) ); 
         point.CalculateTotalError();
         //point.Print();
         // copy the point and add it to the measurement
         meas[i]->AddDataPoint(new NucDBDataPoint(point));
      }
   }

   afile.close();
}
//______________________________________________________________________________

void HERMES_ALU_2012_NucDB()
{
   NucDBManager * dbman = NucDBManager::GetManager(1);

   auto experiment = dbman->GetExperiment("HERMES");
   if( !experiment ){
      experiment = new NucDBExperiment("HERMES","HERMES");

   }

   // ----------------------------------------------------------------
   // Asymmetries
   auto meas = experiment->GetMeasurement("ALU_I^sin(phi)");
   if( !meas ) {
      meas = new NucDBMeasurement("ALU_I^sin(phi)","A_{LU,I}^{sin(phi)}");
      experiment->AddMeasurement(meas);
   }
   meas->ClearDataPoints();
   meas->SetType( NucDB::Type::Asymmetry );
   meas->SetProcesses( {NucDB::Process::DVCS, NucDB::Process::Exclusive} );

   auto meas1 = experiment->GetMeasurement("ALU_DVCS^sin(phi)");
   if( !meas1 ) {
      meas1 = new NucDBMeasurement("ALU_DVCS^sin(phi)","A_{LU,DVCS}^{sin(phi)}");
      experiment->AddMeasurement(meas1);
   }
   meas1->ClearDataPoints();
   meas1->SetType( NucDB::Type::Asymmetry );
   meas1->SetProcesses( {NucDB::Process::DVCS, NucDB::Process::Exclusive} );

   auto meas2 = experiment->GetMeasurement("ALU_I^sin(2phi)");
   if( !meas2 ) {
      meas2 = new NucDBMeasurement("ALU_I^sin(2phi)","A_{LU,I}^{sin(2#phi)}");
      experiment->AddMeasurement(meas2);
   }
   meas2->ClearDataPoints();
   meas2->SetType( NucDB::Type::Asymmetry );
   meas2->SetProcesses( {NucDB::Process::DVCS, NucDB::Process::Exclusive} );

   // ----------------------------------------------------------------
   
   meas ->AddSet("Table 3");
   meas1->AddSet("Table 3");
   meas2->AddSet("Table 3");
   add_DVCS_data("experiments/DVCS/HERMES/ALU_2012/table3.txt", {meas,meas1,meas2} );

   meas ->AddSet("Table 5");
   meas1->AddSet("Table 5");
   meas2->AddSet("Table 5");
   add_DVCS_data("experiments/DVCS/HERMES/ALU_2012/table5.txt", {meas,meas1,meas2} );
   
   // ----------------------------------------------------------------
   TMultiGraph * mg = new TMultiGraph();
   auto gr = meas->BuildGraph("t");
   mg->Add(gr,"ep");
   gr = meas1->BuildGraph("t");
   mg->Add(gr,"ep");
   gr = meas2->BuildGraph("t");
   mg->Add(gr,"ep");
   mg->Draw("a");

   //meas->Print();
   experiment->Print();

   //dbman->SaveExperiment(experiment);
}
