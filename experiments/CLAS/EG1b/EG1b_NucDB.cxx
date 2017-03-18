
void add_DVCS_data(const char * fname, NucDBMeasurement * meas)
{

   const int Ncols = 5;
   std::array<double,Ncols> x      ;
   std::array<double,Ncols> Q2     ;
   std::array<double,Ncols> W      ;
   std::array<double,Ncols> sig    ;
   std::array<double,Ncols> eStat  ;
   std::array<double,Ncols> eSys1  ;
   std::array<double,Ncols> eSys2  ;

   // ----------------------------------------------------------------
   //

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
         W_var.SetValueSize(   t[i], 0.001);
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

void EG1bNucDB()
{
   NucDBManager * dbman = NucDBManager::GetManager(1);

   auto experiment = dbman->GetExperiment("CLAS_EG1b");
   if( !experiment ){
      experiment = new NucDBExperiment("CLAS_EG1b","CLAS EG1b");
   }

   // ----------------------------------------------------------------
   // unpolarized cross section
   auto meas = experiment->GetMeasurement("A1p");
   if( !meas ) {
      meas = new NucDBMeasurement("A1p","A_{1}^{p}");
      experiment->AddMeasurement(meas);
   }
   meas->ClearDataPoints();
   meas->SetType( NucDB::Type::Asymmetry );
   meas->SetProcesses( {NucDB::Process::DIS, NucDB::Process::Inclusive} );

   auto afile = std::ifstream("experiments/CLAS/EG1b/EG1b_A1_g1_x.txt");
   std::string line;
   std::stringstream iss(line);
   double phi = 0;


   NucDBBinnedVariable  x_var(  "x","x");
   NucDBBinnedVariable  Q2_var( "Qsquared","Q^{2}");
   NucDBBinnedVariable  W_var("W","W");

   NucDBDataPoint  * apoint = 0 ;
   NucDBDataPoint  point = NucDBDataPoint();
   point.AddBinVariable(  &x_var);
   point.AddBinVariable(  &W_var);
   point.AddBinVariable( &Q2_var);
   
   // ----------------------------------------------------------------
   //
   auto gr = meas->BuildGraph("phi");
   gr->Draw("aep");

   experiment->Print();

   dbman->SaveExperiment(experiment);
}
