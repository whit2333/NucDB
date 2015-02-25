Int_t Osipenko_et_al_NucDB(Int_t aNumber = 0){

   NucDBManager     * dbman = NucDBManager::GetManager(1);
   NucDBCalculation * calc  = 0;
   NucDBMeasurement * meas  = 0;
   NucDBReference   * ref   = 0;
   NucDBPaper       * paper = 0;

   NucDBBinnedVariable * Q2 = new NucDBBinnedVariable("Qsquared","Q^{2}");
   NucDBDataPoint        point;
   point.AddBinVariable(Q2);
   
   // -------------------------------------------------------------
   paper = dbman->GetPaper("Osipenko2005nx");
   if(paper) { 
      paper->SetName("Osipenko2005nx");
      paper->SetTitle("Global analysis of data on the proton structure function g(1) and extraction of its moments");
   } else {
      paper = new NucDBPaper("Osipenko2005nx","Global analysis of data on the proton structure function g(1) and extraction of its moments");
   }
   paper->SetAuthor("Osipenko, et. al.");
   paper->GetCalculations()->Clear();

   ref   = new NucDBReference("Global analysis of data on the proton structure function g(1) and extraction of its moments");
   ref->SetURL("http://inspirehep.net/record/677631");
   ref->SetDOI("http://dx.doi.org/10.1103/PhysRevD.71.054007");
   ref->SetRefType(NucDBReference::kPublished);

   // d2 for p,n,d
   calc = paper->GetCalculation("d2");
   if(!calc) {
      calc = new NucDBCalculation("d2","d2");
      paper->AddCalculation(calc);
   }
   calc->ClearMeasurements();

   // ---------------------------------------------------------------
   // d2p
   std::ifstream datafile("analyses/Osipenko_et_al/d2_inelastic.txt");
   std::string line;
   std::getline(datafile,line); // skip first line

   meas = calc->GetMeasurement("d2p");
   if(!meas){
      meas= new NucDBMeasurement("d2p","d_{2}^{p}");
      calc->AddMeasurement(meas);
   }
   meas->SetExperimentName(paper->GetAuthor());
   meas->ClearDataPoints();
   meas->AddRef(ref);

   double Q2_val;
   double d2_val;
   double stat_val;
   double syst_val;
   double scale = 1.0e-3;


   while( datafile.good() ) {
      datafile >> Q2_val >> d2_val >> stat_val >> syst_val;
      if( datafile.eof() ) break;

      Q2->SetValueSize(Q2_val,0.001);
      point.SetValue(d2_val*scale); 
      NucDBErrorBar err1(stat_val*scale);
      NucDBErrorBar err2(syst_val*scale);
      point.SetStatError(err1); 
      point.SetSystError(err2); 
      point.CalculateTotalError();
      meas->AddDataPoint(new NucDBDataPoint(point));
   }

   dbman->SavePaper(paper);
   return 0;
}

