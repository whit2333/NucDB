Int_t d2n_NucDB() {
   // Results of g1He3 and g2He3 for two beam energies can be found in
   // Final-E06014-g1_g2.dat. These results are from Matt Posik
   // Errors are statistical only.

   NucDBManager     * dbman = NucDBManager::GetManager(1);
   NucDBExperiment  * exp   = 0;
   NucDBMeasurement * meas  = 0;
   NucDBMeasurement * meas2 = 0;

   //NucDBReference   * ref = new NucDBReference("d2n_longpaper","");
   //ref->SetDOI("http://dx.doi.org/10.1103/PhysRevD.64.112006");
   //ref->SetRefType(NucDBReference::kPublished);

   // d2n Experiment
   exp = dbman->GetExperiment("d2n");
   if(!exp ) {
      exp = new NucDBExperiment("d2n","d2n E06014");
   }

   // g1He3
   meas = exp->GetMeasurement("g1He3");
   if(meas) delete meas;
   meas = 0;
   if(!meas){
      meas= new NucDBMeasurement("g1He3","g_{1}^{^{3}He}");
      exp->AddMeasurement(meas);
   }
   //meas->AddRef(ref);

   // g2He3
   meas2 = exp->GetMeasurement("g2He3");
   if(meas2) delete meas2;
   meas2 = 0;
   if(!meas2){
      meas2= new NucDBMeasurement("g2He3","g_{2}^{^{3}He}");
      exp->AddMeasurement(meas2);
   }
   //meas->AddRef(ref);

   NucDBBinnedVariable    Ebeam("Ebeam","E_{beam}");
   NucDBBinnedVariable    x("x","x");
   NucDBBinnedVariable    Q2("Qsquared","Q^{2}");
   NucDBDiscreteVariable  A("A","A");
   NucDBDiscreteVariable  Z("Z","Z");

   // This is the local working data point we manipulate 
   // then pass a copy when adding to the measurement, ie,
   // NucDBMeasurement::AddDataPoint(new NucDBDataPoint(point) );
   NucDBDataPoint      point;
   point.AddBinVariable(&Q2);
   point.AddBinVariable(&x);
   point.AddBinVariable(&Ebeam);
   point.AddDiscreteVariable(&A);
   point.AddDiscreteVariable(&Z);

   // Helium-3
   Z.SetValue(2);
   A.SetValue(3);


   // Open the data file
   std::ifstream ifs ("Final-E06014-g1_g2.dat", std::ifstream::in);
   if( !(ifs.good()) ) { return -1; }
   std::string   line = "";

   // Ignore the first two lines
   std::getline(ifs,line);
   std::cout << line << std::endl;
   std::getline(ifs,line);
   std::cout << line << std::endl;

   Int_t     Nlines = 16;   // 16 data points for each energy
   Double_t  x_1,Q2_1;
   Double_t  x_2,Q2_2;
   Double_t  g1,g1_err;
   Double_t  g2,g2_err;

   //  First beam energy
   Ebeam.SetValueSize(5.9,0.001);
   for(int i = 0; i < Nlines; i++ ) {

      if( ifs.good() ) {
         ifs >> Q2_1;
         ifs >>  x_1;
         ifs >> g1;
         ifs >> g1_err;

         ifs >> Q2_2;
         ifs >>  x_2;
         ifs >> g2;
         ifs >> g2_err;
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
      point.SetValue(g1); 
      NucDBErrorBar err1(g1_err);
      NucDBErrorBar err2(0.0);
      point.SetStatError(err1); 
      point.SetSystError(err2); 
      point.CalculateTotalError();
      meas->AddDataPoint(new NucDBDataPoint(point));

      // g2
      Q2.SetValueSize(Q2_2,0.001);
      x.SetValueSize(x_2,0.001);
      point.SetValue(g2); 
      NucDBErrorBar err3(g2_err);
      NucDBErrorBar err4(0.0);
      point.SetStatError(err3); 
      point.SetSystError(err4); 
      point.CalculateTotalError();
      meas2->AddDataPoint(new NucDBDataPoint(point));
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

         ifs >> Q2_2;
         ifs >>  x_2;
         ifs >> g2;
         ifs >> g2_err;
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
      point.SetValue(g1); 
      NucDBErrorBar err1(g1_err);
      NucDBErrorBar err2(0.0);
      point.SetStatError(err1); 
      point.SetSystError(err2); 
      point.CalculateTotalError();
      meas->AddDataPoint(new NucDBDataPoint(point));

      // g2
      Q2.SetValueSize(Q2_2,0.001);
      x.SetValueSize(x_2,0.001);
      point.SetValue(g2); 
      NucDBErrorBar err3(g2_err);
      NucDBErrorBar err4(0.0);
      point.SetStatError(err3); 
      point.SetSystError(err4); 
      point.CalculateTotalError();
      meas2->AddDataPoint(new NucDBDataPoint(point));
   }

   meas->Print( "");
   meas2->Print("");

   dbman->SaveExperiment(exp);

   return 0;
}

