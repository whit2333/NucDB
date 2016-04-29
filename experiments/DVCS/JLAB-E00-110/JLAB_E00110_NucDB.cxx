void JLAB_E00110_NucDB()
{
   NucDBManager * dbman = NucDBManager::GetManager(1);

   auto experiment = dbman->GetExperiment("JLAB-E00110");
   if( !experiment ){
      experiment = new NucDBExperiment("JLAB-E00110","JLAB E00110");

   }
   auto meas = new NucDBMeasurement("sigma_p","d#sigma/dxdtd#phi dQ^{2}");
   meas->ClearDataPoints();
   experiment->AddMeasurement(meas);

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

   const int Ncols = 5;
   std::array<double,Ncols> x      ;
   std::array<double,Ncols> Q2     ;
   std::array<double,Ncols> t      ;
   std::array<double,Ncols> sig    ;
   std::array<double,Ncols> eStat  ;
   std::array<double,Ncols> eSys1  ;
   std::array<double,Ncols> eSys2  ;

   std::string line;
   std::stringstream iss(line);
   // ----------------------------------------------------------------
   std::ifstream afile("experiments/DVCS/JLAB-E00-110/table_VII.dat");

   std::getline(afile, line);
   iss = std::stringstream(line);
   for(int i = 0; i< Ncols; i++ ) {
      iss >> x[i];
      //std::cout << "x = " << x[i] << std::endl;
   }
   std::getline(afile, line);
   iss = std::stringstream(line);
   for(int i = 0; i< Ncols; i++ ) {
      iss >> Q2[i];
      //std::cout << "Q2= " << Q2[i] << std::endl;
   }
   std::getline(afile, line);
   iss = std::stringstream(line);
   for(int i = 0; i< Ncols; i++ ) {
      iss >> t[i];
      //std::cout << "t = " << t[i] << std::endl;
   }

   double phi = 0;
   while (std::getline(afile, line))
   {
      iss = std::stringstream(line);
      std::cout << line << std::endl;

      iss >> phi;
      //std::cout << "phi = " << phi << std::endl;

      double x1,x2,x3;
      for(int i = 0; i< Ncols; i++ ) {
         iss >> x1;
         iss >> x2;
         iss >> x3;
         std::cout << x1 <<std::endl;
         std::cout << x2 <<std::endl;
         std::cout << x3 <<std::endl;
         sig[i] = x1;
         eStat[i] = x2;
         eSys1[i] = x3;
         std::cout << "sig = " << sig[i] << " " << eStat[i]  << " " << eSys1[i] << std::endl;;
      }

      std::getline(afile, line);
      iss = std::stringstream(line);
      std::cout << line << std::endl;
      for(int i = 0; i< Ncols; i++ ) {
         iss >> eSys2[i];
         //std::cout << "sys2= " << eSys2[i] << std::endl;;
      }
      for(int i = 0; i< Ncols; i++ ) {

         x_var.SetValueSize(   x[i], 0.001);
         t_var.SetValueSize(   t[i], 0.001);
         Q2_var.SetValueSize( Q2[i], 0.001);
         phi_var.SetValueSize(  phi, 0.001);

         point.SetValue( sig[i]  ); 
         // Set the errors
         point.SetStatError( eStat[i] ); 
         point.SetSystError( NucDBErrorBar(eSys1[i],eSys2[i]) ); 
         point.CalculateTotalError();
         point.Print();
         // copy the point and add it to the measurement
         meas->AddDataPoint(new NucDBDataPoint(point));
      }
   }
   afile.close();

   // ----------------------------------------------------------------
   afile = std::ifstream("experiments/DVCS/JLAB-E00-110/table_VIII.dat");

   std::getline(afile, line);
   iss = std::stringstream(line);
   for(int i = 0; i< Ncols; i++ ) {
      iss >> x[i];
      //std::cout << "x = " << x[i] << std::endl;
   }
   std::getline(afile, line);
   iss = std::stringstream(line);
   for(int i = 0; i< Ncols; i++ ) {
      iss >> Q2[i];
      //std::cout << "Q2= " << Q2[i] << std::endl;
   }
   std::getline(afile, line);
   iss = std::stringstream(line);
   for(int i = 0; i< Ncols; i++ ) {
      iss >> t[i];
      //std::cout << "t = " << t[i] << std::endl;
   }

   while (std::getline(afile, line))
   {
      iss = std::stringstream(line);
      std::cout << line << std::endl;

      iss >> phi;
      //std::cout << "phi = " << phi << std::endl;

      double x1,x2,x3;
      for(int i = 0; i< Ncols; i++ ) {
         iss >> x1;
         iss >> x2;
         iss >> x3;
         std::cout << x1 <<std::endl;
         std::cout << x2 <<std::endl;
         std::cout << x3 <<std::endl;
         sig[i] = x1;
         eStat[i] = x2;
         eSys1[i] = x3;
         std::cout << "sig = " << sig[i] << " " << eStat[i]  << " " << eSys1[i] << std::endl;;
      }

      std::getline(afile, line);
      iss = std::stringstream(line);
      std::cout << line << std::endl;
      for(int i = 0; i< Ncols; i++ ) {
         iss >> eSys2[i];
         //std::cout << "sys2= " << eSys2[i] << std::endl;;
      }
      for(int i = 0; i< Ncols; i++ ) {

         x_var.SetValueSize(   x[i], 0.001);
         t_var.SetValueSize(   t[i], 0.001);
         Q2_var.SetValueSize( Q2[i], 0.001);
         phi_var.SetValueSize(  phi, 0.001);

         point.SetValue( sig[i]  ); 
         // Set the errors
         point.SetStatError( eStat[i] ); 
         point.SetSystError( NucDBErrorBar(eSys1[i],eSys2[i]) ); 
         point.CalculateTotalError();
         point.Print();
         // copy the point and add it to the measurement
         meas->AddDataPoint(new NucDBDataPoint(point));
      }
   }
   auto gr = meas->BuildGraph("phi");
   gr->Draw("aep");

   dbman->SaveExperiment(experiment);
}
