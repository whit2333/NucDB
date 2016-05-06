default : newbuild

newbuild :  
	@echo "*** In-source build no longer supported ***"
	@echo "Use cmake with out-of-source build. For example, "
	@echo "cd .."
	@echo "mkdir nucdb_build "
	@echo "cd nucdb_build"
	@echo "cmake ../NucDB/. -DCMAKE_INSTALL_PREFIX=/usr/local"
	@echo "make "
	@echo "make install"

nucdb : shared_libs
	@echo 
	@echo === doing nothing ===
	@echo 

# Shared Libraries
##############################################################################/

# Utilitiy
##############################################################################/

doc: 
	doxygen doc/Doxyfile_NucDB #	doxygen doc/Doxyfile_insaneweb
	@echo "HTML Documentation created"

.PHONY : clean printstuff doc snapshot database databaseclean

database:
	python experiments/OLDSLAC/OLDSLAC_NucDB.py
	python experiments/SLAC-E142/SLAC-E142_NucDB.py
	python experiments/SLAC-E143/SLAC-E143_NucDB.py
	python experiments/SLAC-E155/SLAC-E155_NucDB.py
	python experiments/SLAC-E154/SLAC-E154_NucDB.py
	python experiments/EMC/EMC_NucDB.py
	python experiments/SMC/SMC_NucDB.py
	python experiments/CERN-NA-2/CERN-NA-2_NucDB.py
	python experiments/HERMES/HERMES_NucDB.py
	python experiments/ZEUS/ZEUS_NucDB.py
	python experiments/BCDMS/BCDMS_NucDB.py
	python experiments/NMC/NMC_NucDB.py
	python experiments/COMPASS/COMPASS_NucDB.py
	python experiments/CLAS/CLAS_NucDB.py
	python experiments/CLAS/CLAS_g1_NucDB.py
	python experiments/CLAS/E1b_NucDB.py 
	python experiments/CLAS/E93009_NucDB.py
	#python experiments/CLAS/E93031_NucDB.py  # problematic?
	python experiments/CLAS/E91023_NucDB.py
	python experiments/JLAB-E94010/JLAB-E94010_NucDB.py
	python experiments/JLAB-E94110/JLAB-E94110_NucDB.py
	python experiments/JLAB-E97103/JLAB-E97103_NucDB.py
	python experiments/JLAB-E99117/JLAB-E99117_NucDB.py
	python experiments/JLAB-E99118/JLAB-E99118_NucDB.py
	python experiments/JLAB-E00002/JLAB-E00002_NucDB.py
	python experiments/JLAB-E00108/JLAB-E00108_NucDB.py
	python experiments/JLAB-E01012/JLAB-E01012_NucDB.py
	python experiments/JLAB-E03103/JLAB-E03103_NucDB.py
	python experiments/RSS/RSS_NucDB.py
	python experiments/RESData/RESData_SLAC_NucDB.py
	python experiments/QES_archive/QES_archive_NucDB.py
	root -b -q -l experiments/inclusive/inclusive_NucDB.cxx
	root -b -q -l experiments/CCFR/CCFR_NucDB.cxx
	root -b -q -l analyses/Osipenko_et_al/Osipenko_et_al_NucDB.cxx
	#./analyses/maid_all
	python analyses/LATTICE_Gockeler_et_al/LATTICE_NucDB.py
	root -b -q -l experiments/DESY/inclusive/inclusive_piminus_NucDB.cxx
	root -b -q -l experiments/JLAB-E06014/inclusive_d2n_NucDB.cxx 
	root -b -q -l experiments/DVCS/JLAB-E00-110/JLAB_E00110_NucDB.cxx


databaseclean:
	rm data/NucDB.root

snapshot:
	git archive HEAD --format=tar | gzip >NucDB-`date +%m_%d_%Y_`.tar.gz

#git archive HEAD --format=tar | gzip >InSANE-`date +%m_%d_%Y_`.tar.gz

clean:
	rm data/NucDB.root


