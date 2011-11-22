#include <stdio.h>
#include <getopt.h>
#include "NucDBDataPoint.h"
#include "NucDBManager.h"
#include "TRint.h"

#define PACKAGE    "getoptex"
#define VERSION    "0.0.1"

/*! \mainpage Nucleon Database NucDB


    \subpage install

    \subpage intro

    \subpage tutorial
    (incomplete)
    
    \subpage addingdata

 */ 
/*! \page install Installation

   \section requirements Requirements 
    - ROOT
    - Python
    - MySQL database access

   \section git Download the code
Grab it from the git repository with the following command:
\code
   git clone http://quarks.temple.edu/~git/NucDB.git
\endcode

   \section envvariables Configure your environment

      Add lib directory to $LD_LIBRARY_PATH and python directory to $PYTHON_PATH in your $HOME/.bashrc
   \code
# NucDB
  export NucDB_DIR=$HOME/work/NucDB 
  export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$NucDB_DIR/lib
  export PYTHON_PATH=$PYTHON_PATH:$NucDB_DIR/python
   \endcode
   \section rootlogon Configure your ROOT logon scripts

   Put thins in your $HOME/.rootlogon.C
   \code
  /// NucDB Libraries
  gSystem->AddIncludePath("-I$NucDB_DIR/include");
  gSystem->Load("$NucDB_DIR/lib/libNucDB.so");
   \endcode
   In your $HOME/.rootlogon.py you might want to add something like this
   \code 
   from ROOT import gROOT,gSystem
   gSystem.Load( 'libNucDB' )
   \endcode


   \section compile  Compile and Build Database
   Compile and build the database
   \code
   make
   make database
   \endcode


 */ 
/*! \page intro Introduction to NucDB

  \image html TBrowser_NucDB.png 

   Here is the abstracted structure of NucDB :

   - Experiment
     - Measurement
       - Datapoint
         - Statistical Error
         - Systematic Error
         - Binned variable

  Each item above can be considered an array


\todo Create a way of defining units!

 */ 
/*! \page addingdata Adding New Data to NucDB

  \section addingfirststep First Step: Gathering data

  So you want to add more data to NucDB? First thin you should gather the best most complete 
  data for each measurment/experiment. A good source of experimental data is <a href="http://durpdg.dur.ac.uk/">
  The Durham HepData Project </a>.

  You will need the following information:
   - experiment name (eg, SLAC_E143,SMC,EMC). Note, try to use underscores instead of "-".
   - measurement name (eg, F2p, F1n, g1p/F1p)
   - data points and bin sizes
   - data point errors (statisitcal and systematic)
   - Any extra data on beam energies, spectrometer angles, etc... 

   \subsection addexample1 Example SLAC E143
   Say you want to add data on \f$ R = \frac{\sigma_L}{\sigma_T} \f$ from the SLAC E143 experiment
   from <a href="http://durpdg.dur.ac.uk/cgi-hepdata/struct3/E143/PL452B194/R"> this data file </a> 
   from the <a href="http://durpdg.dur.ac.uk/">Durham HepData Project</a>.

   - Create a new experiment directory in $NucDB_DIR/experiments if it does not already exist. 
      This is where all data files used to fill the database will be stored. 
      \code cd experiments
      mkdir SLAC-E143
      cd SLAC-E143
      wget http://durpdg.dur.ac.uk/cgi-hepdata/struct3/E143/PL452B194/R  # downloads a raw data file
      mv R R.dat 
      \endcode
   - Clean up up data file so that the last lines are just the data we are interested in.
      (This just makes writing the extractor easier.) So we have text file, 
      $NucDB_DIR/experiments/SLAC-E143/R.dat, that looks like this:
    \code
EXPeriment      = SLAC-E-143                     
REACtion        = e- C --> e- X                                
Collaboration   = E143                       
Author          = Abe et al 
REFerence       = Phys. Lett. B452 (1999) 194                            
Additional info : Measurement of R (R = sig(L)/sig(T)) in deep inelastic 
                  electron scattering on a Carbon target at incident 
                  electron energy of 29 GeV.  The data cover the x range 
                  0.03 to 0.1 and Q**2 range 1.3 to 2.7 GeV**2.

==========================================================================     
      x          Q^2     epsilon     R             errors
                GeV^2                           stat.      sys.

   0.0325         .32      0.474     0.45       0.01       0.07
   0.0375        1.47      0.519     0.51       0.02       0.09
   0.0450        1.67      0.578     0.40       0.01       0.10
   0.0550        1.90      0.641     0.28       0.01       0.09
   0.0650        2.11      0.692     0.29       0.02       0.10
   0.0750        2.29      0.734     0.18       0.02       0.11
   0.0850        2.46      0.767     0.26       0.03       0.10
   0.0950        2.60      0.795     0.25       0.03       0.14
   0.1050        2.73      0.818     0.17       0.03       0.13

    \endcode

   - The next step will be creating the extractor using the python class NucDBRawDataExtractor. 
   - For organization purposes create the extractor in experiments directory with the name exp_NucDB.py
   - For this example we would create the extractor as
     \code touch experiments/SLAC-E143_NucDB.py \endcode

  \section creatingextractor Second step: Create Extractor

   Next you will want to create a concrete class inheriting the NucDBRawDataExtractor. 
   If your data file has multiple measurements per row, it is probably best to inherit your concrete 
   class for the first measurment and only change the column indicies as need (see \ref RSS_NucDB.py).
   
    \subsection addexample2 Extractor for SLAC E143 "R" Measurement
  
   A good starting point to copy and paste would be the following code

   \include exp_NucDB.py

   You want to run the extractor from the $NucDB_DIR so that it can then be added to the makefile. 
   \code python experiments/SLAC-E143_NucDB.py \endcode
   At this point you will likely have to debug. 
   Once this is done send me an email (whit@temple.edu) with your additions.
   Or better yet, setup your git repository so I can pull from it and send me a pull request!
   (see <a href="http://blog.mhartl.com/2008/10/14/setting-up-your-git-repositories-for-open-source-projects-at-github/">this</a> if you want to understand why or what this means)
   
   
   \todo Figure out how to deal with evolved data in a nice way 

 

 */ 
/*! \page tutorial A Brief Tutorial

   \section usage Using NucDB 

   \subsection themanager The Manager
   The singleton class for database management. 
   \include usage.cxx
   
   See \ref NucDBManager for complete doc.

   \subsection expandmeas Experiments and Measurements
   See \ref NucDBExperiment and \ref NucDBMeasurement for complete doc.

   \section usageexamples Examples demonstrating usage

   \subsection example1 F2p
   Plot the unpolarized structure fucntion \f$ F_2^p(x) \f$.
   From examples/F2p.cxx
   \include F2p.cxx
   
   \subsection example2 \ref A1p.cxx
   Plot the virtual Compton scattering asymmetry \f$ A_1^p(x) \f$..
   From examples/A1p.cxx
   \include A1p.cxx

  
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
