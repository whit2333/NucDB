#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ all typedef;

#pragma link C++ nestedclass;
#pragma link C++ nestedtypedef;

#pragma link C++ namespace NucDB;
#pragma link C++ namespace NucDB::Kine;

#pragma link C++ enum NucDB::Type;
#pragma link C++ enum NucDB::Process;

#pragma link C++ function NucDB::Kine::to_radians(double);
#pragma link C++ function NucDB::Kine::to_degrees(double);

#pragma link C++ function NucDB::Kine::xBjorken_Etheta(double,double,double);
#pragma link C++ function NucDB::Kine::xBjorken_Q2nu(double,double,double);
#pragma link C++ function NucDB::Kine::xBjorken_WQ2(double,double,double);
#pragma link C++ function NucDB::Kine::xBjorken_WQ2_proton(double,double);

#pragma link C++ function NucDB::Kine::Qsquared_Wx(double,double,double);
#pragma link C++ function NucDB::Kine::Qsquared_Etheta(double,double,double);

#pragma link C++ function NucDB::Kine::W2_Q2nu(double,double,double);
#pragma link C++ function NucDB::Kine::W2_Q2nu_proton(double,double);
#pragma link C++ function NucDB::Kine::W_Q2nu(double,double,double);
#pragma link C++ function NucDB::Kine::W_Q2nu_proton(double,double);

#pragma link C++ function NucDB::Kine::W2_xQ2(double,double,double);
#pragma link C++ function NucDB::Kine::W2_xQ2_proton(double,double);
#pragma link C++ function NucDB::Kine::W_xQ2(double,double,double);
#pragma link C++ function NucDB::Kine::W_xQ2_proton(double,double);

#pragma link C++ function NucDB::Kine::xiNachman_xQ2(double,double,double);
#pragma link C++ function NucDB::Kine::nu_xQ2(double,double,double);
#pragma link C++ function NucDB::Kine::nu_xQ2_proton(double,double);

#pragma link C++ function NucDB::ApplyFilterOnList(NucDBBinnedVariable*,TList*);
#pragma link C++ function NucDB::FilterMeasurements(TList * , NucDBBinnedVariable *);
#pragma link C++ function NucDB::CreateMultiGraph(TList*,const char *);
#pragma link C++ function NucDB::FillLegend(TLegend *,TList*,TMultiGraph *);
#pragma link C++ function NucDB::GetExperiment(const char *,TList*);
#pragma link C++ function NucDB::FindExperiment(const char *,TList*);
#pragma link C++ function NucDB::MergeDataPoints(TList*);


#pragma link C++ class NucDBManager+;
#pragma link C++ global fgDBManager;
#pragma link C++ class NucDBDatabase+;

#pragma link C++ class NucDBVariable+;
#pragma link C++ class NucDBDiscreteVariable+;
#pragma link C++ class NucDBBinnedVariable+;
#pragma link C++ class NucDBErrorBar+;
#pragma link C++ class NucDBDataPoint+;
#pragma link C++ class NucDBDependentVariable+;

#pragma link C++ class std::vector<NucDBDataPoint*>+;

#pragma link C++ class NucDBThetaDV_rad+;
#pragma link C++ class NucDBThetaDV_deg+;
#pragma link C++ class NucDBInvariantMassDV+;
#pragma link C++ class NucDBInvariantMassDV_Q2nu+;
#pragma link C++ class NucDBW2DV_Q2nu+;
#pragma link C++ class NucDBPhotonEnergyDV+;
#pragma link C++ class NucDBxBjorkenDV+;
#pragma link C++ class NucDBQ2DV_EEpTheta+;

#pragma link C++ class NucDBMeasurement+;
#pragma link C++ class NucDBExperiment+;
#pragma link C++ class NucDBCalculation+;
#pragma link C++ class NucDBPaper+;
#pragma link C++ class NucDBReference+;

#pragma link C++ class NucDBUnit+;
#pragma link C++ class NucDBInverseUnit+;
#pragma link C++ class NucDBEnergyUnit+;
#pragma link C++ class NucDBEnergyMeV+;
#pragma link C++ class NucDBMomentumUnit+;
#pragma link C++ class NucDBMassUnit+;
#pragma link C++ class NucDBXSectionUnit+;


#endif
