Int_t d2_chiral_soliton_model(Int_t aNumber = 0){

   NucDBManager     * dbman = NucDBManager::GetManager(1);
   NucDBCalculation * calc  = 0;
   NucDBMeasurement * meas  = 0;
   NucDBReference   * ref   = 0;
   NucDBPaper       * paper = 0;

   NucDBBinnedVariable * Q2 = new NucDBBinnedVariable("Qsquared","Q^{2}");
   NucDBDataPoint        point;
   point.AddBinVariable(Q2);
   
   // -------------------------------------------------------------
   paper = dbman->GetPaper("Chiral Soliton Model d2");
   if(!paper) {
      paper = new NucDBPaper("Chiral Soliton Model d2","Chiral Soliton Model d2");
   }
   paper->SetAuthor("Gamberg and Weigel");
   ref   = new NucDBReference("Chiral Soliton Model d2","Polarized structure function g2 in the c.m. bag model");
   ref->SetURL("");
   ref->SetDOI("");
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
   meas->AddRef(ref);
   meas->ClearDataPoints();
   meas->SetExperimentName("CSM - Gamberg and Weigel");

   // No errors
   NucDBErrorBar err1(0.0000001);
   NucDBErrorBar err2(0.0000001);
   point.SetStatError(err1); 
   point.SetSystError(err2); 

   Q2->SetValueSize(3.0,0.001);
   point.SetValue(0.00486181576); 
   point.CalculateTotalError();
   meas->AddDataPoint(new NucDBDataPoint(point));

   Q2->SetValueSize(5.0,0.001);
   point.SetValue(0.00435609369); 
   point.CalculateTotalError();
   meas->AddDataPoint(new NucDBDataPoint(point));

   Q2->SetValueSize(10.0,0.001);
   point.SetValue(0.00381175587); 
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
   meas->SetExperimentName("CSM - Gamberg and Weigel");
   meas->AddRef(ref);

   Q2->SetValueSize(3.0,0.001);
   point.SetValue(-0.00184613729); 
   point.CalculateTotalError();
   meas->AddDataPoint(new NucDBDataPoint(point));

   Q2->SetValueSize(5.0,0.001);
   point.SetValue(-0.00166798373); 
   point.CalculateTotalError();
   meas->AddDataPoint(new NucDBDataPoint(point));

   Q2->SetValueSize(10.0,0.001);
   point.SetValue(-0.00145107071); 
   point.CalculateTotalError();
   meas->AddDataPoint(new NucDBDataPoint(point));

   dbman->SavePaper(paper);
   return 0;
}

