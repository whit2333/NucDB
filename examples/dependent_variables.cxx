Int_t dependent_variables() {
/*
   NucDBBinnedVariable    * x  = new NucDBBinnedVariable("x","x");
   NucDBBinnedVariable    * Q2 = new NucDBBinnedVariable("Qsquared","Q^{2}");
   NucDBDependentVariable * W  = new NucDBDependentVariable("W","W");
   W->SetNDependentVariables(2);
   W->SetFunc2( &NucDB::Kine::W_xQ2_proton );
   x->SetBinValueSize(0.6,0.1);
   Q2->SetBinValueSize(3.0,0.5);
   
   W->SetVariable(0,x);
   W->SetVariable(1,Q2);

  x->Print();
   Q2->Print();

   std::cout << "  W = " << W->GetValue() << "\n";
//    if(W->fFunc1) std::cout << "1 " << W->fFunc1 << "\n";
//    if(W->fFunc2) std::cout << "2 " << W->fFunc2 << "\n";
//    if(W->fFunc3) std::cout << "3 " << W->fFunc3 << "\n";
   W->Dump();*/

   NucDBInvariantMassDV * W = new NucDBInvariantMassDV();
   W->Calculate();
   W->Print();

   return(0);
}