Int_t pion_production(){

gSystem->Load( "libNucDB" );

   TCanvas * c = new TCanvas();
NucDBManager * manager = NucDBManager::GetManager();

TLegend * leg = new TLegend(0.1,0.7,0.48,0.9);
leg->SetHeader("The Legend Title");

TList * measurementsList = manager->GetMeasurements("sig_p(pi+)");
for( int i=0; i< measurementsList->GetEntries();i++) {
   NucDBMeasurement * ameas = (NucDBMeasurement*)measurementsList->At(i);
   ameas->BuildGraph("z");
   leg->AddEntry(ameas->fGraph,Form("%s %s",ameas->fExperiment.Data(),ameas->GetName()),"ep");
   if(i==0)ameas->fGraph->Draw("ap");
   ameas->fGraph->Draw("p");
}

TList * measurementsList = manager->GetMeasurements("sig_p(pi-)");
for( int i=0; i< measurementsList->GetEntries();i++) {
   NucDBMeasurement * ameas = (NucDBMeasurement*)measurementsList->At(i);
   ameas->BuildGraph("z");
   leg->AddEntry(ameas->fGraph,Form("%s %s",ameas->fExperiment.Data(),ameas->GetName()),"ep");
   if(i==0)ameas->fGraph->Draw("p");
   ameas->fGraph->Draw("p");
}

TList * measurementsList = manager->GetMeasurements("sig_d(pi+)");
for( int i=0; i< measurementsList->GetEntries();i++) {
   NucDBMeasurement * ameas = (NucDBMeasurement*)measurementsList->At(i);
   ameas->BuildGraph("z");
   leg->AddEntry(ameas->fGraph,Form("%s %s",ameas->fExperiment.Data(),ameas->GetName()),"ep");
   if(i==0)ameas->fGraph->Draw("p");
   ameas->fGraph->Draw("p");
}

TList * measurementsList = manager->GetMeasurements("sig_d(pi-)");
for( int i=0; i< measurementsList->GetEntries();i++) {
   NucDBMeasurement * ameas = (NucDBMeasurement*)measurementsList->At(i);
   ameas->BuildGraph("z");
   leg->AddEntry(ameas->fGraph,Form("%s %s",ameas->fExperiment.Data(),ameas->GetName()),"ep");
   if(i==0)ameas->fGraph->Draw("p");
   ameas->fGraph->Draw("p");
}

leg->Draw();
c->SaveAs("examples/pion_production.png");

return(0);
}