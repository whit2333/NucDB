Int_t E665_NucDB(){

   NucDBManager * dbman = NucDBManager::GetManager(1);

   NucDBExperiment * exp = 0;
   NucDBMeasurement * meas = 0;
   NucDBReference * ref = new NucDBReference("","");
   ref->SetDOI("http://dx.doi.org/10.1103/PhysRevD.64.112006");
   ref->SetRefType(NucDBReference::kPublished);

   exp = dbman->GetExperiment("E665");
   if(!exp ) {
      exp = new NucDBExperiment("E665","E665");
   }
   meas = exp->GetMeasurement("xsPlusxsbarOver2");
   if(meas) delete meas;
   meas = 0;
   if(!meas){
      meas= new NucDBMeasurement("xsPlusxsbarOver2","(xs(x)+x#bar{s}(x))/2");
      exp->AddMeasurement(meas);
   }
   meas->AddRef(ref);

   //NucDBBinnedVariable    Ebeam("Ebeam","E_{beam}");
   NucDBBinnedVariable    x("x","x");
   NucDBBinnedVariable    Q2("Qsquared","Q^{2}");
   //NucDBBinnedVariable    Pt("Pt","p_{T}");
   //NucDBDiscreteVariable  A("A","A");
   //NucDBDiscreteVariable  Z("Z","Z");

   NucDBDataPoint   * apoint = 0 ;
   NucDBDataPoint  point;
   point.AddBinVariable(&Q2);
   point.AddBinVariable(&x);


   // --------------------------
   // mu^2 = 4.0 (GeV/c)^2
   Q2.SetValueSize(4.0,0.001);
   x.SetValueSize(0.01,0.001);
   point.SetValue(0.178); 
   NucDBErrorBar err1(0.016);
   NucDBErrorBar err2(0.008);
   point.SetStatError(err1); 
   point.SetSystError(err2); 
   point.CalculateTotalError();
   meas->AddDataPoint(new NucDBDataPoint(point));

   x.SetValueSize(0.05,0.001);
   point.SetValue(0.111); 
   err1 = NucDBErrorBar(0.009);
   err2 = NucDBErrorBar(0.005);
   point.SetStatError(err1); 
   point.SetSystError(err2); 
   point.CalculateTotalError();
   meas->AddDataPoint(new NucDBDataPoint(point));

   x.SetValueSize(0.1,0.001);
   point.SetValue(0.072); 
   err1 = NucDBErrorBar(0.005);
   err2 = NucDBErrorBar(0.003);
   point.SetStatError(err1); 
   point.SetSystError(err2); 
   point.CalculateTotalError();
   meas->AddDataPoint(new NucDBDataPoint(point));

   x.SetValueSize(0.2,0.001);
   point.SetValue(0.030); 
   err1 = NucDBErrorBar(0.003);
   err2 = NucDBErrorBar(0.001);
   point.SetStatError(err1); 
   point.SetSystError(err2); 
   point.CalculateTotalError();
   meas->AddDataPoint(new NucDBDataPoint(point));
   //--------------------------
   //meas->PrintData();

   dbman->SaveExperiment(exp);

   return 0;
}

