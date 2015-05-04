NucDB
=====

An interface to experimental and theoretical data for nuclear and particle physics.

Naming Convention
-----------------

###Measurement Names

Differential cross sections are named "sigma" and follow the usual shorthand reaction notation.
Nuclear targets are labeled by A then their periodic table symbol.

    "sigma_12C(gamma,pi+)X" = differential cross section for 12C + gamma --> pi+ + X


Installing
------------

###Environment
You should configure your environment like the following example for bash.

    #NucDB
    export NucDB_DIR=$HOME/work/NucDB
    export NucDB_DATA_DIR=$NucDB_DIR/data
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$NucDB_DIR/lib
    export PYTHONPATH=$PYTHONPATH:$NucDB_DIR/python

###Building
Should be as simple as:

    make

###Configuring ROOT
Added libraries in $HOME/.rootlogon.C or other logon script.

    gSystem->AddIncludePath(" -I$NucDB_DIR/include ");
    gSystem->Load("$NucDB_DIR/lib/libNucDB.so");

In your $HOME/.rootlogon.py you might want to add something like this

    import os
    from ROOT import gROOT,gSystem
    gSystem.Load( 'libNucDB' )

###Building the database
Once ROOT has the libraries you can build the database.

    make database

###Errors
If you have any trouble, please email Whitney Armstrong at whit@temple.edu.



Documentation
-------------

The general structure:
    Manager
    ->Experiments
      ->Measurements
        ->DataPoints
        ->References
      ->Papers
    ->Papers
      ->Calculations
        ->Measurements
      ->Papers


checkout [the doc page](http://quarks.temple.edu/~whit/code/nucdb/ "NucDB")

Also the look at the ["official" webiste](http://whit2333.github.com/NucDB "NucDB")

