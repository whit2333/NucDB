Int_t d2n_NucDB(){

   NucDBManager     * dbman = NucDBManager::GetManager(1);
   NucDBExperiment  * exp   = dbman->GetExperiment("JLAB-E06014");
   if(!exp) return -1;
   NucDBMeasurement * meas  = 0;
   NucDBReference   * ref   = 0;

   
   // -------------------------------------------------------------

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


      Q2bin.SetValueSize(   Q2, 0.001);

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

   dbman->SaveExperiment(exp);

   return 0;
}
