#ifdef __CLING__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;
#pragma link off all typedef;
#pragma link off all namespace;

#pragma link C++ nestedclass;
#pragma link C++ nestedtypedef;

//#pragma link C++ defined_in NucDB;

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

#pragma link C++ function NucDB::GetConfidenceIntervals;
#pragma link C++ class std::function<double(const double*,const double*)>+;
#pragma link C++ class std::function<double(double*,double*)>+;

#pragma link C++ class NucDBManager+;
#pragma link C++ global fgDBManager;
#pragma link C++ class NucDBDatabase+;

#pragma link C++ class NucDBUnit+;
#pragma link C++ class NucDBInverseUnit+;
#pragma link C++ class NucDBEnergyUnit+;
#pragma link C++ class NucDBEnergyMeV+;
#pragma link C++ class NucDBMomentumUnit+;
#pragma link C++ class NucDBMassUnit+;
#pragma link C++ class NucDBXSectionUnit+;

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

#pragma link C++ class NucDB::Chi2Function+;

//#pragma link C++ class std::array<std::string,2>+;
//#pragma link C++ class std::vector<std::array<double,2>>+;
//
//#pragma link C++ function NucDB::GetConfidenceIntervals<2>(const TMatrixDSym&, double, double, double, std::function<double(const double*, const double* )> , const std::vector<double>&, const std::vector<NucDBDataPoint*>& , const std::array<std::string,2>&);
//#pragma link C++ function NucDB::GetConfidenceIntervals<2>(const TMatrixDSym&, double, double, double, std::function<double(const double *, const double * )> , const std::vector<double>&, const std::vector<std::array<double,2>>& );
//#pragma link C++ function NucDB::GetConfidenceIntervals<2>(const TMatrixDSym&, double, double, double, std::function<double(const double *, const double * )> ,const std::vector<double>& , const TH1*, unsigned int, std::array<double,2> );

//#pragma link C++ function NucDB::GetConfidenceIntervals(const TMatrixDSym&, double, double, double, std::function<double(const double *, const double * )> , const int, const std::vector<double>&, const std::vector<std::vector<double>>& );
//#pragma link C++ function NucDB::GetConfidenceIntervals(const TMatrixDSym&, double, double, double, std::function<double(const double *, const double * )>, const int, const std::vector<double>& , const TH1*, int, std::vector<double> );

#endif
