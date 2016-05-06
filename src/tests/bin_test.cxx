void bin_test(){

   NucDBBinnedVariable  ebeam0_bin("Ebeam", "Ebeam", 0.4,  0.1 );
   NucDBBinnedVariable  ebeam1_bin("Ebeam", "Ebeam", 1.4,  0.7 );
   NucDBBinnedVariable  ebeam2_bin( ebeam1_bin );
   ebeam2_bin -=  ebeam0_bin;
   ebeam2_bin.Print();

}
