	subroutine breakopen(filename,ns,npts,z,a,e,theta,nu,sig,sige,
     >	cite)
     	implicit none
!	breakopen the big files to create individual spectra
!	and count the number of points in each spectra
	integer i,j,ns
	real einc,tht,x,s,se
	character*40 filename
	integer npts(100)		! number of spectra,number of points
	integer z(100),a(100)	! z and a
	integer zed,atomic		! z and a
	real e(100),theta(100)	! energies and angles
	character*30 cite(100),c	! citation for each
	real nu(100,200),sig(100,200),sige(100,200)
	real laste,lastt
	logical eof

!
	open(unit=1,file=filename,status="unknown",err=10)
!	write(6,'(a)') filename
!	initialize some counters
	i=0		! For counting the number of distinct energies and angles
	laste = 0.	! last energy
	lastt = 0.	! last theta
	j = 0
	eof = .true.
	
	do while (eof)
	read(1,*,end=100) zed,atomic,einc,tht,x,s,se,c
!	write(6,*) zed,atomic,einc,tht,x,s,se,c
	i = i + 1
	if (einc .ne. laste .or. tht .ne. lastt) then
	i = 1	! reset i since we now hae a new energy or angle
	j = j + 1
	z(j) = zed
	a(j) = atomic
	e(j) = einc
	theta(j) = tht
	cite(j) = c
	nu(j,i) = x
	sig(j,i) = s
	sige(j,i) = se
	laste = einc
	lastt = tht
	else
	nu(j,i) = x
	sig(j,i) = s
	sige(j,i) = se
	npts(j) = i
	endif
	ns = j
	enddo
	
100	eof= .false.	
	close(unit=1)
!	now write data back out as a test

!	ns = j
!	do j=1,ns
!		do i=1,npts(j)
!	 	write(12,'(2(i3,1x),3(f8.3,1x),2(e10.3,1x),a30)')
!     >	 	z(j),a(j),e(j),theta(j),nu(j,i),sig(j,i),sige(j,i),cite(j)	 	
!	 	enddo
!	 enddo
	do j=1,ns
		write(6,*) j, npts(j),e(j),theta(j)
	enddo
	 return
	 
10	write(6,*)' Error opening file!!!'	 
	return
	
	end
