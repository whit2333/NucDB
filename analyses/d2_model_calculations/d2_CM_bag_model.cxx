Int_t d2_CM_bag_model(Int_t aNumber = 0){

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
   if(!paper) {
      paper = new NucDBPaper("CM Bag Model d2","Center-of-Mass Bag model predictions for g2");
   }
   paper->SetAuthor("X.Song");
   ref   = new NucDBReference("CM Bag Model","Polarized structure function g2 in the c.m. bag model");
   ref->SetURL("http://inspirehep.net/record/417451");
   ref->SetDOI("http://dx.doi.org/10.1103/PhysRevD.54.1955");
   ref->SetRefType(NucDBReference::kPublished);

   // d2 for p,n,d
   calc = paper->GetCalculation("d2");
   if(!calc) {
      calc = new NucDBCalculation("d2","d2");
      paper->AddCalculation(calc);
   }

   // d2p
   meas = calc->GetMeasurement("d2p");
   if(!meas){
      meas= new NucDBMeasurement("d2p","d_{2}^{p}");
      calc->AddMeasurement(meas);
   }
   meas->ClearDataPoints();
   meas->SetExperimentName("CM Bag Model");
   meas->AddRef(ref);
   Q2->SetValueSize(5.0,0.001);
   point.SetValue(0.0174); 
   NucDBErrorBar err1(0.0000001);
   NucDBErrorBar err2(0.0000001);
   point.SetStatError(err1); 
   point.SetSystError(err2); 
   point.CalculateTotalError();
   meas->AddDataPoint(new NucDBDataPoint(point));

   // d2n
   meas = calc->GetMeasurement("d2n");
   if(meas) delete meas;
   meas = 0;
   if(!meas){
      meas= new NucDBMeasurement("d2n","d_{2}^{n}");
      calc->AddMeasurement(meas);
   }
   meas->ClearDataPoints();
   meas->SetExperimentName("CM Bag Model");
   meas->AddRef(ref);
   Q2->SetValueSize(5.0,0.001);
   point.SetValue(-0.00253); 
   NucDBErrorBar err1(0.0000001);
   NucDBErrorBar err2(0.0000001);
   point.SetStatError(err1); 
   point.SetSystError(err2); 
   point.CalculateTotalError();
   meas->AddDataPoint(new NucDBDataPoint(point));


   // d2d
   meas = calc->GetMeasurement("d2d");
   if(meas) delete meas;
   meas = 0;
   if(!meas){
      meas= new NucDBMeasurement("d2d","d_{2}^{d}");
      calc->AddMeasurement(meas);
   }
   meas->ClearDataPoints();
   meas->SetExperimentName("CM Bag Model");
   meas->AddRef(ref);
   Q2->SetValueSize(5.0,0.001);
   point.SetValue(0.00679); 
   NucDBErrorBar err1(0.0000001);
   NucDBErrorBar err2(0.0000001);
   point.SetStatError(err1); 
   point.SetSystError(err2); 
   point.CalculateTotalError();
   meas->AddDataPoint(new NucDBDataPoint(point));


   //exp = dbman->GetExperiment("CCFR");
   //if(!exp ) {
   //   exp = new NucDBExperiment("CCFR","CCFR");
   //}
   //NucDBBinnedVariable    Ebeam("Ebeam","E_{beam}");
   //NucDBBinnedVariable    x("x","x");
   //NucDBBinnedVariable    Pt("Pt","p_{T}");
   //NucDBDiscreteVariable  A("A","A");
   //NucDBDiscreteVariable  Z("Z","Z");


   dbman->SavePaper(paper);
   return 0;
}

