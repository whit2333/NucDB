Int_t d2_inelastic(Int_t aNumber = 0){

   NucDBManager     * dbman = NucDBManager::GetManager(1);
   NucDBCalculation * calc  = 0;
   NucDBMeasurement * meas  = 0;
   NucDBReference   * ref   = 0;
   NucDBPaper       * paper = 0;

   NucDBBinnedVariable * Q2 = new NucDBBinnedVariable("Qsquared","Q^{2}");
   NucDBDataPoint        point;
   point.AddBinVariable(Q2);
   
   // -------------------------------------------------------------
   paper = dbman->GetPaper("CM Bag Model d2");
   if(paper) delete paper;
   paper = 0;
   paper = new NucDBPaper("Osipenko:2005nx","Global analysis of data on the proton structure function g(1) and extraction of its moments");
   paper->SetAuthor("Osipenko, et. al.");
   ref   = new NucDBReference("Global analysis of data on the proton structure function g(1) and extraction of its moments");
   ref->SetURL("http://inspirehep.net/record/677631");
   ref->SetDOI("http://dx.doi.org/10.1103/PhysRevD.71.054007");
   ref->SetRefType(NucDBReference::kPublished);

   // d2 for p,n,d
   calc = new NucDBCalculation("d2","d2");


   // ---------------------------------------------------------------
   // d2p
   std::ifstream datafile("analyses/Osipenko_et_al/d2_inelastic.txt");
   std::string line;
   std::getline(datafile,line); // skip first line

   meas = calc->GetMeasurement("d2p");
   if(meas) delete meas;
   meas = 0;
   if(!meas){
      meas= new NucDBMeasurement("d2p","d_{2}^{p}");
      calc->AddMeasurement(meas);
   }
   meas->AddRef(ref);

   double Q2_val;
   double d2_val;
   double stat_val;
   double syst_val;
   double junk;

   while( datafile.is_good) {
      datafile >> Q2 >> d2_val >> stat_val >> syst_val;
      if( datafile.eof() ) break;

      Q2->SetValueSize(Q2,0.001);
      point.SetValue(d2_val); 
      NucDBErrorBar err1(stat_val);
      NucDBErrorBar err2(syst_val);
      point.SetStatError(err1); 
      point.SetSystError(err2); 
      point.CalculateTotalError();
      meas->AddDataPoint(new NucDBDataPoint(point));
   }

   dbman->SavePaper(paper);
   return 0;
}

