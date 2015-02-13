#ifndef NucDBUtil_HH
#define NucDBUtil_HH 1

#include "TList.h"
#include "TLegend.h"
#include "TMultiGraph.h"
#include "NucDBBinnedVariable.h"
#include "NucDBMeasurement.h"

namespace NucDB {

   void ApplyFilterOnList(NucDBBinnedVariable * var, TList * list);
   TMultiGraph * CreateMultiGraph(TList * list, const char * var);
   void FillLegend(TLegend * leg, TList * list, TMultiGraph * mg );

}


#endif
