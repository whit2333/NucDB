#ifndef NucDBUnits_HH
#define NucDBUnits_HH 1

#include "TNamed.h"
#include "TList.h"
#include "TString.h"

/** Units  
 *
 * Standard units:
 *  - Energy   - ( GeV )
 *  - Momentum - ( GeV/c )
 *  - Mass     - ( GeV/c^2 )
 *  - Cross Section ( micro-barn )
 *  - Length   - ( fm )
 *  - time     - ( s )
 *
 */
class NucDBUnit : public TNamed {
  public:
    NucDBUnit(const char * name ="aunit",const char * title="unknown");
    virtual ~NucDBUnit();

    //NucDBUnit(NucDBUnit&&) = default;
    //NucDBUnit& operator=(const NucDBUnit&) = default;
    //NucDBUnit& operator=(NucDBUnit&&) = default;

    NucDBUnit& operator*=(const NucDBUnit& rhs);

  public:
    /** Returns conversion factor from natural units, \f$ (\hbar c)^2 = 0.38939129 GeV^2 mb \f$ */
    Double_t GetHbarCsquared(){return fhbarc2;}

    /** Returns \f$ (\hbar c) = 0.197 GeV fm \f$ */
    Double_t GetHbarC() { return fhbarc; }

    /**  \f$ \hbar  = 6.58 10^(-25) GeV s \f$ */
    Double_t GetHbar() { return fhbar; }

    /** Returns speed of light \f$ c  = 2.99792458 10^(23) fm/s \f$ */
    Double_t GetC(){ return fSpeedOfLight; }

  public:
    /** virtual method which when implmeneted for a new unit
     *  should return a converted value in standard units.
     */
    virtual Double_t ConvertToStandardUnits(Double_t val) { return(fConversionFactor*val); }

    virtual Double_t  GetConversionFactor()const { return fConversionFactor ; }
    void      SetConversionFactor(Double_t f) { fConversionFactor = f; }
  protected:
    Double_t fConversionFactor;
    Double_t fhbarc2;// GeV^2 mb
    Double_t fhbarc;// GeV fm
    Double_t fhbar; //GeV s
    Double_t fSpeedOfLight; // fm s

    ClassDef(NucDBUnit,2)
};

class NucDBInverseUnit : public NucDBUnit {
  public:
    NucDBInverseUnit(NucDBUnit * unit = nullptr){
      if(unit) {
        SetInvertedUnit(unit);
      }
    }
    virtual ~NucDBInverseUnit()= default;

    void SetInvertedUnit(NucDBUnit * unit){
      if(!unit) printf(" Null unit to be inverted\n");
      else {
        SetNameTitle(Form("1/%s",unit->GetName()),Form("1/%s",unit->GetTitle()));
        fConversionFactor = 1.0/unit->GetConversionFactor();
      }
    }

    ClassDef(NucDBInverseUnit,1)
};

/** Standard energy unit, GeV */
class NucDBEnergyUnit : public NucDBUnit {
  public:
    NucDBEnergyUnit(const char * name ="energy",const char * title="GeV")
      :NucDBUnit(name,title) {
      }
    virtual ~NucDBEnergyUnit()= default;
    ClassDef(NucDBEnergyUnit,1)
};

class NucDBEnergyMeV : public NucDBEnergyUnit {
  public:
    NucDBEnergyMeV(const char * n = "MeV",const char * t ="MeV"):NucDBEnergyUnit(n,t){
      fConversionFactor = 1.0/1000.0;
    }
    virtual ~NucDBEnergyMeV()= default;

    ClassDef(NucDBEnergyMeV,1)
};

/** Standard momentum unit, GeV/c */
class NucDBMomentumUnit : public NucDBUnit {
  public:
    NucDBMomentumUnit(const char * name ="momentum",const char * title="GeV/c")
      :NucDBUnit(name,title) {
      }
    virtual ~NucDBMomentumUnit()= default;
    ClassDef(NucDBMomentumUnit,1)
};

/** Standard momentum unit, GeV/c */
class NucDBMassUnit : public NucDBUnit {
  public:
    NucDBMassUnit(const char * name ="mass",const char * title="GeV/c^2")
      :NucDBUnit(name,title) {
      }
    virtual ~NucDBMassUnit()= default;
    ClassDef(NucDBMassUnit,1)
};

/** Standard cross section unit, mili-barn */
class NucDBXSectionUnit : public NucDBUnit {
  public:
    NucDBXSectionUnit(const char * name ="xsection",const char * title="mb")
      :NucDBUnit(name,title) {
      }
    virtual ~NucDBXSectionUnit()= default;
    ClassDef(NucDBXSectionUnit,1)
};

#endif


