Int_t g1p(){

   gSystem->Load( "libNucDB" );
   NucDBManager * manager = NucDBManager::GetManager();

   TLegend * leg = new TLegend(0.1,0.7,0.48,0.9);
   leg->SetHeader("The Legend Title");

   TList * measurementsList = manager->GetMeasurements("F2p");
   for( int i=0; i< measurementsList->GetEntries();i++) {
      NucDBMeasurement * g1p = (NucDBMeasurement*)measurementsList->At(i);
      g1p->BuildGraph();
      leg->AddEntry(g1p->fGraph,Form("%s F2p",g1p->fExperiment.Data()),"ep");
      if(i==0)g1p->fGraph->Draw("ap");
      g1p->fGraph->Draw("p");
   }

   leg->Draw();



return(0);
}
