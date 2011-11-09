#include <stdio.h>
#include <getopt.h>
#include "NucDBDataPoint.h"
#include "NucDBManager.h"
#include "TRint.h"

#define PACKAGE    "getoptex"
#define VERSION    "0.0.1"

/*! \mainpage Nucleon Database NucDB

    \section Introduction

    \subpage install

    \subpage howto

    \subpage tutorial

    \subpage addingdata   

    

 */ 
/*! \page install Installation

   \section requirements Requirements 
    - ROOT
    - Python
    - MySQL database access

   \section git Download the code

   \section envvariables Configure your environment
   put thins in your $HOME/.rootlogon.C
\code
  /// NucDB Libraries
  gSystem->AddIncludePath("-I${NucDB_DIR}/include");
  gSystem->Load("${NucDB_DIR}/lib/libNucDB.so");
\endcode
      Add lib directory to $LD_LIBRARY_PATH and python directory to $PYTHON_PATH in your $HOME/.bashrc
\code
# NucDB
  export NucDB_DIR=$HOME/work/NucDB 
  export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$NucDB_DIR/lib
  export PYTHON_PATH=$PYTHON_PATH:$NucDB_DIR/python    
\endcode

   \section compile  Compile and Build
   Compile and build the database
   \code
   make
   make database
   \endcode


 */ 
/*! \page howto How to use NucDB


  
  
 */ 
/*! \page addingdata Adding New Data to NucDB
  
  So you want to add more data.

 You will need the following things:
   - experiment name (eg, SLAC_E143,SMC,EMC)
   - measurement name (eg, F2p, F1n, g1p/F1p)
   - data points and bin sizes. Usually x and Q^2
   - data point errors (statisitcal and systematic)
   - 

   \todo Figure out how to deal with evolved data in a nice way 


 */ 
/*! \page tutorial A Brief Tutorial


  
 */ 
/*! \page install Installation

 */ 
void print_help(int exval);

int main(int argc, char *argv[]) {
 int opt;
 // no arguments given
 if(argc == 1) {
  fprintf(stderr, "This program needs arguments....\n\n");
  print_help(1);
 }
    TRint * theApp = new TRint("NucDB", &argc, argv);
    NucDBManager * man = 0;
 while((opt = getopt(argc, argv, "hVvaf:o:")) != -1) {
  switch(opt) {
   case 'h':
    print_help(0);
    break;
   case 'V':
    printf("%s %s\n\n", PACKAGE, VERSION); 
    return(0);
    break;
   case 'v':
    printf("%s: Verbose option is set `%c'\n", PACKAGE, optopt);
    break;
   case 'a':
    printf("List the measurments \n");
/*    man = NucDBManager::GetManager();*/
/*    theApp->Run(true);*/
    break;
   case 'f':
    printf("%s: Filename %s\n", PACKAGE, optarg);
    break;
   case 'o':
    printf("Output: %s\n", optarg);
    break;
   case ':':
    fprintf(stderr, "%s: Error - Option `%c' needs a value\n\n", PACKAGE, optopt);
    print_help(1);
    break;
   case '?':
    fprintf(stderr, "%s: Error - No such option: `%c'\n\n", PACKAGE, optopt);
    print_help(1);
   }
 }

 /* 
 // print all remaining options
 */
 for(; optind < argc; optind++)
  printf("argument: %s\n", argv[optind]);

 return 0;
}

void print_help(int exval) {
 printf("  Nuclear Database \n");
 printf("=========================================\n");
 printf("%s,%s show working getopt example\n", PACKAGE, VERSION); 
 printf("%s [-h] [-V] [-f FILE] [-o FILE]\n\n", PACKAGE);
 printf("  -h              print this help and exit\n");
 printf("  -V              print version and exit\n\n");
 printf("  -v              set verbose flag\n");
 printf("  -f FILE         set intput file\n");
 printf("  -o FILE         set output file\n\n");
 printf("=========================================\n");

}
