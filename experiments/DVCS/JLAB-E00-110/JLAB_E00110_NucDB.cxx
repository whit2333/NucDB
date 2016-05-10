
void add_DVCS_data(const char * fname, NucDBMeasurement * meas)
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
   double phi = 0;
   // ----------------------------------------------------------------
   //
   afile = std::ifstream(fname);

   std::getline(afile, line);
   iss = std::stringstream(line);
   for(int i = 0; i< Ncols; i++ ) {
      iss >> x[i];
   }
   std::getline(afile, line);
   iss = std::stringstream(line);
   for(int i = 0; i< Ncols; i++ ) {
      iss >> Q2[i];
   }
   std::getline(afile, line);
   iss = std::stringstream(line);
   for(int i = 0; i< Ncols; i++ ) {
      iss >> t[i];
   }

   while (std::getline(afile, line))
   {
      iss = std::stringstream(line);
      iss >> phi;

      int N_cols_read = 0;
      double x1,x2,x3;
      for(int i = 0; i< Ncols; i++ ) {
         if(!(iss >> x1)) {break;}
         iss >> x2;
         iss >> x3;
         sig[i] = x1;
         eStat[i] = x2;
         eSys1[i] = x3;
         N_cols_read++;
      }

      std::getline(afile, line);
      iss = std::stringstream(line);
      for(int i = 0; i< Ncols; i++ ) {
         if(!(iss >> eSys2[i])){
            break;
         }
      }
      for(int i = 0; i< N_cols_read; i++ ) {

         x_var.SetValueSize(   x[i], 0.001);
         t_var.SetValueSize(   t[i], 0.001);
         Q2_var.SetValueSize( Q2[i], 0.001);
         phi_var.SetValueSize(  phi, 0.001);

         point.SetValue( sig[i]  ); 
         // Set the errors
         point.SetStatError( eStat[i] ); 
         point.SetSystError( NucDBErrorBar(eSys1[i],eSys2[i]) ); 
         point.CalculateTotalError();
         //point.Print();
         // copy the point and add it to the measurement
         meas->AddDataPoint(new NucDBDataPoint(point));
      }
      //std::cout << " cols : " << N_cols_read << std::endl;
   }
   afile.close();
}

void JLAB_E00110_NucDB()
{
   NucDBManager * dbman = NucDBManager::GetManager(1);

   auto experiment = dbman->GetExperiment("JLAB-E00110");
   if( !experiment ){
      experiment = new NucDBExperiment("JLAB-E00110","JLAB E00110");

   }

   // ----------------------------------------------------------------
   // unpolarized cross section
   auto meas = experiment->GetMeasurement("sigma_p");
   if( !meas ) {
      meas = new NucDBMeasurement("sigma_p","d#sigma/dxdtd#phi dQ^{2}");
      experiment->AddMeasurement(meas);
   }
   meas->ClearDataPoints();
   meas->SetType( NucDB::Type::CrossSection );
   meas->SetProcesses( {NucDB::Process::DVCS, NucDB::Process::Exclusive} );

   meas->AddSet("Table VII");
   add_DVCS_data("experiments/DVCS/JLAB-E00-110/table_VII.dat",meas);

   meas->AddSet("Table VIII");
   add_DVCS_data("experiments/DVCS/JLAB-E00-110/table_VIII.dat",meas);

   meas->AddSet("Table XII");
   add_DVCS_data("experiments/DVCS/JLAB-E00-110/table_XII.dat",meas);

   meas->AddSet("Table XIII");
   add_DVCS_data("experiments/DVCS/JLAB-E00-110/table_XIII.dat",meas);
  
   // ----------------------------------------------------------------
   meas = experiment->GetMeasurement("delta_sigma_p");
   if( !meas ) {
      meas = new NucDBMeasurement("delta_sigma_p","#Deltad#sigma/dxdtd#phi dQ^{2}");
      experiment->AddMeasurement(meas);
   }
   meas->ClearDataPoints();
   meas->SetType( NucDB::Type::CrossSectionDifference );
   meas->SetProcesses( {NucDB::Process::DVCS, NucDB::Process::Exclusive} );

   meas->AddSet("Table IX");
   add_DVCS_data("experiments/DVCS/JLAB-E00-110/table_IX.dat",meas);

   meas->AddSet("Table X");
   add_DVCS_data("experiments/DVCS/JLAB-E00-110/table_X.dat",meas);

   meas->AddSet("Table XI");
   add_DVCS_data("experiments/DVCS/JLAB-E00-110/table_XI.dat",meas);

   meas->AddSet("Table XIV");
   add_DVCS_data("experiments/DVCS/JLAB-E00-110/table_XIV.dat",meas);

   meas->AddSet("Table XV");
   add_DVCS_data("experiments/DVCS/JLAB-E00-110/table_XV.dat",meas);
   
   // ----------------------------------------------------------------
   auto gr = meas->BuildGraph("phi");
   gr->Draw("aep");

   experiment->Print("data");

   dbman->SaveExperiment(experiment);
}
