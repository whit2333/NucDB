Int_t d2n_NucDB(){

   NucDBManager     * dbman = NucDBManager::GetManager(1);
   NucDBExperiment  * exp   = dbman->GetExperiment("JLAB-E06014");
   if(!exp ) {
      exp = new NucDBExperiment("JLAB-E06014","JLAB-E06014");
   }
   if(!exp) return -1;
   NucDBMeasurement * meas  = 0;
   NucDBReference   * ref   = 0;

   // -------------------------------------------------------------
   // The d2 results
   ref   = new NucDBReference("d2n paper","A Precision Measurement of the Neutron Twist-3 Matrix Element dn2: Probing Color Forces");
   ref->SetURL("http://inspirehep.net/record/1290559");
   ref->SetDOI("http://dx.doi.org/10.1103/PhysRevLett.113.022002");
   ref->SetRefType(NucDBReference::kPublished);

   meas = exp->GetMeasurement("d2n");
   if(!meas) {
      meas = new NucDBMeasurement("d2n","d_{2}^{n}");
      meas->AddRef(ref);
      exp->AddMeasurement(meas);
   }
   meas->ClearDataPoints();

   meas1 = exp->GetMeasurement("d2nbar");
   if(!meas1) {
      meas1 = new NucDBMeasurement("d2nbar","#bar{d}_{2}^{n}");
      meas1->AddRef(ref);
      exp->AddMeasurement(meas1);
   }
   meas1->ClearDataPoints();

   NucDBBinnedVariable * Q2bin = new NucDBBinnedVariable("Qsquared","Q^{2}");
   NucDBDataPoint        point;
   point.AddBinVariable(Q2bin);

   double col[11]; 
   double scale = 1.0e-5;
   std::string line;
   std::ifstream file("experiments/JLAB-E06014/d2n_results.txt");
   if( !(file.good()) ) return -4;
   std::getline(file,line);
   while( !(file.eof()) ){
      for(int i = 0;i<11;i++){
         file >> col[i];
      }
      if(file.eof())  break;

      double Q2           = col[0];
      double d2_meas      = col[1]*scale;
      double d2_meas_stat = col[2]*scale;
      double d2_meas_sys  = col[3]*scale;
      double d2_lowx      = col[4]*scale;
      double d2_lowx_sys  = col[5]*scale;
      double d2_el        = col[6]*scale;
      double d2_el_sys    = col[7]*scale;
      double d2_tot       = col[8]*scale;
      double d2_tot_stat  = col[9]*scale;
      double d2_tot_sys   = col[10]*scale;

      Q2bin->SetValueSize(   Q2, 0.001);
      point.SetValue( d2_tot ); 
      NucDBErrorBar err1(d2_tot_stat);
      NucDBErrorBar err2(d2_tot_sys);
      point.SetStatError(err1); 
      point.SetSystError(err2); 
      point.CalculateTotalError();
      meas->AddDataPoint(new NucDBDataPoint(point));

      point.SetValue( d2_meas ); 
      NucDBErrorBar err3(d2_meas_stat);
      NucDBErrorBar err4(d2_meas_sys);
      point.SetStatError(err3); 
      point.SetSystError(err4); 
      point.CalculateTotalError();
      meas1->AddDataPoint(new NucDBDataPoint(point));
   }
   file.close();
   meas->Print("data");
   exp->Print();

   //-----------------------------------------------------
   // Spin structure functions
   //-----------------------------------------------------
   // g1He3
   meas = exp->GetMeasurement("g1He3");
   if(!meas){
      meas= new NucDBMeasurement("g1He3","g_{1}^{^{3}He}");
      exp->AddMeasurement(meas);
   }
   meas->ClearDataPoints();

   // g2He3
   meas2 = exp->GetMeasurement("g2He3");
   if(!meas2){
      meas2= new NucDBMeasurement("g2He3","g_{2}^{^{3}He}");
      exp->AddMeasurement(meas2);
   }
   meas2->ClearDataPoints();

   NucDBBinnedVariable    Ebeam("Ebeam","E_{beam}");
   NucDBBinnedVariable    x("x","x");
   NucDBBinnedVariable    Q2("Qsquared","Q^{2}");
   NucDBDiscreteVariable  A("A","A");
   NucDBDiscreteVariable  Z("Z","Z");

   // This is the local working data point we manipulate 
   // then pass a copy when adding to the measurement, ie,
   // NucDBMeasurement::AddDataPoint(new NucDBDataPoint(point) );
   NucDBDataPoint point2;
   point2.AddBinVariable(&Q2);
   point2.AddBinVariable(&x);
   point2.AddBinVariable(&Ebeam);
   point2.AddDiscreteVariable(&A);
   point2.AddDiscreteVariable(&Z);
   //point2.AddDependentVariable(&W);
   // Helium-3
   Z.SetValue(2);
   A.SetValue(3);

   // Open the data file
   std::ifstream ifs ("experiments/JLAB-E06014/E06014-g1_g2.dat", std::ifstream::in);
   if( !(ifs.good()) ) { return -1; }
   //std::string   line = "";

   // Ignore the first two lines
   std::getline(ifs,line);
   std::cout << line << std::endl;
   std::getline(ifs,line);
   std::cout << line << std::endl;

   Int_t     Nlines = 16;   // 16 data points for each energy
   Double_t  x_1,Q2_1;
   Double_t  x_2,Q2_2;
   Double_t  g1,g1_err,g1_syst_err;
   Double_t  g2,g2_err,g2_syst_err;

   //  First beam energy
   Ebeam.SetValueSize(5.9,0.001);
   for(int i = 0; i < Nlines; i++ ) {

      if( ifs.good() ) {
         ifs >> Q2_1;
         ifs >>  x_1;
         ifs >> g1;
         ifs >> g1_err;
         ifs >> g1_syst_err;

         ifs >> Q2_2;
         ifs >>  x_2;
         ifs >> g2;
         ifs >> g2_err;
         ifs >> g2_syst_err;
      }
      // The two x and Q2 values should be the same
      if( x_1 != x_2 ) {
         std::cout << "Warning differing x values" << std::endl;
      }
      if( Q2_1 != Q2_2 ) {
         std::cout << "Warning differing Q2 values" << std::endl;
      }

      NucDBBinnedVariable * xvar  = point2.GetBinVariable("x");
      NucDBBinnedVariable * Q2var = point2.GetBinVariable("Qsquared");
      NucDBDependentVariable * W = point2.GetDependentVariable("W");
      if(!W) {
         W = new NucDBInvariantMassDV();
         point2.AddDependentVariable(W);
      }
      if(W) {
         W->SetVariable(0,xvar);
         W->SetVariable(1,Q2var);
      }

      // g1
      Q2.SetValueSize(Q2_1,0.001);
      x.SetValueSize(x_1,0.001);
      point2.SetValue(g1); 
      NucDBErrorBar err1(g1_err);
      NucDBErrorBar err2(g1_syst_err);
      point2.SetStatError(err1); 
      point2.SetSystError(err2); 
      point2.CalculateTotalError();
      point2.CalculateDependentVariables();
      meas->AddDataPoint(new NucDBDataPoint(point2));

      // g2
      Q2.SetValueSize(Q2_2,0.001);
      x.SetValueSize(x_2,0.001);
      point2.SetValue(g2); 
      NucDBErrorBar err3(g2_err);
      NucDBErrorBar err4(g2_syst_err);
      point2.SetStatError(err3); 
      point2.SetSystError(err4); 
      point2.CalculateTotalError();
      point2.CalculateDependentVariables();
      meas2->AddDataPoint(new NucDBDataPoint(point2));
   }
   // this finishes the last line which consists of just a \n
   std::getline(ifs,line);
   std::cout << line << std::endl;

   // Ignore next 3 lines
   std::getline(ifs,line);
   std::cout << line << std::endl;
   std::getline(ifs,line);
   std::cout << line << std::endl;
   std::getline(ifs,line);
   std::cout << line << std::endl;

   // Second beam energy
   Ebeam.SetValueSize(4.7,0.001);
   for(int i = 0; i < Nlines; i++ ) {

      if( ifs.good() ) {
         ifs >> Q2_1;
         ifs >>  x_1;
         ifs >> g1;
         ifs >> g1_err;
         ifs >> g1_syst_err;

         ifs >> Q2_2;
         ifs >>  x_2;
         ifs >> g2;
         ifs >> g2_err;
         ifs >> g2_syst_err;
      }
      // The two x and Q2 values should be the same
      if( x_1 != x_2 ) {
         std::cout << "Warning differing x values" << std::endl;
      }
      if( Q2_1 != Q2_2 ) {
         std::cout << "Warning differing Q2 values" << std::endl;
      }

      // g1
      Q2.SetValueSize(Q2_1,0.001);
      x.SetValueSize(x_1,0.001);
      point2.SetValue(g1); 
      NucDBErrorBar err1(g1_err);
      NucDBErrorBar err2(g1_syst_err);
      point2.SetStatError(err1); 
      point2.SetSystError(err2); 
      point2.CalculateTotalError();
      point2.CalculateDependentVariables();
      meas->AddDataPoint(new NucDBDataPoint(point2));

      // g2
      Q2.SetValueSize(Q2_2,0.001);
      x.SetValueSize(x_2,0.001);
      point2.SetValue(g2); 
      NucDBErrorBar err3(g2_err);
      NucDBErrorBar err4(g2_syst_err);
      point2.SetStatError(err3); 
      point2.SetSystError(err4); 
      point2.CalculateTotalError();
      point2.CalculateDependentVariables();
      meas2->AddDataPoint(new NucDBDataPoint(point2));
   }

   meas->Print("data");
   meas2->Print("data");

   dbman->SaveExperiment(exp);

   return 0;
}
