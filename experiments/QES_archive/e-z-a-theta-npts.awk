#!/sw/bin/awk -f
#usage ./e-z-a-theta-npts.awk  data-file 
#
# Recall the order
# 1  2  3    4    5    6      7          8
# Z, A, E, theta, nu, sig, sig_error, citation
	
#	set things up for first line
# 	when encountering the first line, store Z, A, E, theta and the citation
# 	in varibles

  { if (NR==1) {
  		lastz = $1;
  		lasta = $2;
  	 	laste = $3;
  	 	lastt = $4;
  	 	cite = $8;
  	 	printf "%s %3d %3d \n", "   Z and A: ",$1,$2;
  	 	printf "%s \n", "   E(GeV)   Theta   Npts  Citation";
  	 	}
  }
 # just count if nothing changes	 	
  { if($3==laste && $4==lastt && $1==lastz && $2==lasta && $8 == cite){
  		i=i+1;
  		}
  		
  }
  
# if anything changes then print the result and reset i
  {	if($3!=laste || $4!=lastt || $1!=lastz || $2!=lasta|| $8 != cite){
  		printf "%3d %3d  %8.3f %8.3f %3d %s %s \n",lastz,lasta, laste,lastt,i,"  ",cite;		# diagnostic line
  		lastz = $1;
  		lasta = $2;
  	 	laste = $3;
  	 	lastt = $4;
  	 	cite = $8;
  	 	i=1;
  	
  	 	 
	 }

}
END{
		# necessary to get the last set
  		printf "%3d %3d  %8.3f %8.3f %3d %s %s \n",$1,$2, laste,lastt,i,"  ",cite;	}
