Int_t d2_MIT_bag_model(Int_t aNumber = 0){

   NucDBManager     * dbman = NucDBManager::GetManager(1);
   NucDBCalculation * calc  = 0;
   NucDBMeasurement * meas  = 0;
   NucDBReference   * ref   = 0;
   NucDBPaper       * paper = 0;

   NucDBBinnedVariable * Q2 = new NucDBBinnedVariable("Qsquared","Q^{2}");
   NucDBDataPoint        point;
   point.AddBinVariable(Q2);
   
   // -------------------------------------------------------------
   paper = dbman->GetPaper("MIT Bag Model d2");
   if(!paper) {
      paper = new NucDBPaper("MIT Bag Model d2","MIT Bag model predictions for g2");
   }
   paper->SetAuthor("Xiangdong Ji and Peter Unrau");
   ref   = new NucDBReference("MIT Bag Model","Q**2 dependence of the proton's G1 structure function sum rule");
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
   meas->SetExperimentName("MIT Bag Model");
   meas->AddRef(ref);

   Q2->SetValueSize(5.0,0.001);
   point.SetValue(0.01); 
   NucDBErrorBar err1(0.0000001);
   NucDBErrorBar err2(0.0000001);
   point.SetStatError(err1); 
   point.SetSystError(err2); 
   point.CalculateTotalError();
   meas->AddDataPoint(new NucDBDataPoint(point));

   // d2n
   meas = calc->GetMeasurement("d2n");
   if(!meas){
      meas= new NucDBMeasurement("d2n","d_{2}^{n}");
      calc->AddMeasurement(meas);
   }
   meas->ClearDataPoints();
   meas->AddRef(ref);
   meas->SetExperimentName("MIT Bag Model");

   Q2->SetValueSize(5.0,0.001);
   point.SetValue(0.0); 
   NucDBErrorBar err1(0.0000001);
   NucDBErrorBar err2(0.0000001);
   point.SetStatError(err1); 
   point.SetSystError(err2); 
   point.CalculateTotalError();
   meas->AddDataPoint(new NucDBDataPoint(point));

   // d2d
   meas = calc->GetMeasurement("d2d");
   if(!meas){
      meas= new NucDBMeasurement("d2d","d_{2}^{d}");
      calc->AddMeasurement(meas);
   }
   meas->ClearDataPoints();
   meas->AddRef(ref);
   meas->SetExperimentName("MIT Bag Model");

   Q2->SetValueSize(5.0,0.001);
   point.SetValue(0.005); 
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

   paper->Print();

   dbman->SavePaper(paper);
   return 0;
}

