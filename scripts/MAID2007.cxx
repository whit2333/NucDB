{
   NucDBManager * mg = NucDBManager::GetManager();
   NucDBExperiment * exp = mg->GetExperiment("MAID2007");
   if(exp){
	   exp->PlotMeasurements("energy");
   }

}
