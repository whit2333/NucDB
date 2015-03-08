#ifdef __CINT__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link C++ all typedef;


#pragma link C++ namespace NucDB;
#pragma link C++ namespace NucDB::Kine;

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

#pragma link C++ nestedclass;
#pragma link C++ nestedtypedef;

#endif

