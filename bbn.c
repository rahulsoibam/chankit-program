#include "include.h"

/*----------------------------------------------------*/

int linearize(struct relicparam* paramrelic, double T9, double reacparam[NNUCREAC][8], double f[], double r[], int loop, int inc, int ip, double dt, double Y0[], double Y[], double dY_dt[], double H, double rhob)
/* solves for new abundances using gaussian elimination with back substitution */
{
	/* Number of nuclides (#n1,#n2,#n3,#n4) for each of the 11 reaction types */
	double nn1[11]={ 1.,1.,1.,1.,1.,2.,3.,2.,1.,1.,2.};
	double nn2[11]={ 0.,1.,1.,0.,1.,0.,0.,1.,1.,1.,0.};
	double nn3[11]={ 0.,0.,1.,0.,0.,1.,0.,0.,1.,0.,2.};
	double nn4[11]={ 1.,1.,1.,2.,2.,1.,1.,1.,2.,3.,1.};
	
	int i,j,k,l,n,i1,j1,ind;
	double cn1,cn2,cn3,cn4,rn1,rn2,rn3,rn4,yY[NNUC];
	int fail;
	double bdln;
	int ierror;
	int c0 = 0;
	int type[NNUCREAC],n1[NNUCREAC],n2[NNUCREAC],n3[NNUCREAC],n4[NNUCREAC];
	double rev[NNUCREAC],q9[NNUCREAC];
	double a[NNUC][NNUC],b[NNUC],yx[NNUC];
	int icnvm;
	double x[NNUC], a0[NNUC][NNUC], cx, sum, xdy, t;
	int nord,test;
	
	
	for (i=0;i<NNUCREAC;i++) 
	{
		type[i]=(int)reacparam[i][1];
		n1[i]=(int)reacparam[i][2];
		n2[i]=(int)reacparam[i][3];
		n3[i]=(int)reacparam[i][4];
		n4[i]=(int)reacparam[i][5];
		rev[i]=reacparam[i][6];
		q9[i]=reacparam[i][7];
	}

	for(i=0;i<NNUC;i++) for(j=0;j<NNUC;j++) a[i][j]=0.;

	for (n=0;n<NNUCREAC;n++)
	{
		ind=type[n];
		i=n1[n];
		j=n2[n];
		k=n3[n];
		l=n4[n];
		if (i < NNUC && l < NNUC)
		{
			rn1=nn1[ind];
			rn2=nn2[ind];
			rn3=nn3[ind];
			rn4=nn4[ind];
			
			switch(ind) 
			{
				case 0:	{ /* (1,0,0,1) type */
					cn1=f[n];
					cn2=0.;
					cn3=0.;
					cn4=r[n];
					break;}

				case 1: { /* (1,1,0,1) type */
					r[n]=rev[n]*1e10*pow(T9,1.5)*exp(-q9[n]/T9)*f[n];
					f[n]=rhob*f[n];
					cn1=Y[j]*f[n]/2.;
					cn2=Y[i]*f[n]/2.;
					cn3=0.;
					cn4=r[n];
					break;}

				case 2:	{ /* (1,1,1,1) type */
					f[n]=rhob*f[n];
					r[n]=rev[n]*exp(-q9[n]/T9)*f[n];
					cn1=Y[j]*f[n]/2.;
					cn2=Y[i]*f[n]/2.;
					cn3=Y[l]*r[n]/2.;
					cn4=Y[k]*r[n]/2.;
					break;}

				case 3:	{ /* (1,0,0,2) type */
					cn1=f[n];
					cn2=0.;
					cn3=0.;
					cn4=Y[l]*r[n]/2.;
					break;}

				case 4:	{ /* (1,1,0,2) type */
					f[n]=rhob*f[n];
					r[n]=rev[n]*exp(-q9[n]/T9)*f[n];
					cn1=Y[j]*f[n]/2.;
					cn2=Y[i]*f[n]/2.;
					cn3=0.;
					cn4=Y[l]*r[n]/2.;
					break;}

				case 5:	{ /* (2,0,1,1) type */
					f[n]=rhob*f[n];
					r[n]=rev[n]*exp(-q9[n]/T9)*f[n];
					cn1=Y[i]*f[n]/2.;
					cn2=0.;
					cn3=Y[l]*r[n]/2.;
					cn4=Y[k]*r[n]/2.;
					break;}

				case 6:	{ /* (3,0,0,1) type */
					r[n]=rev[n]*1.e20*pow(T9,1.5)*pow(T9,1.5)*exp(-q9[n]/T9)*f[n];
					f[n]=rhob*rhob*f[n];
					cn1=Y[i]*Y[i]*f[n]/6.;
					cn2=0.;
					cn3=0.;
					cn4=r[n];
					break;}
		
				case 7:	{ /* (2,1,0,1) type */
					r[n]=rev[n]*1.e20*pow(T9,1.5)*pow(T9,1.5)*exp(-q9[n]/T9)*f[n];
					f[n]=rhob*rhob*f[n];
					cn1=Y[j]*Y[i]*f[n]/3.;
					cn2=Y[i]*Y[i]*f[n]/6.;
					cn3=0.;
					cn4=r[n];
					break;}

				case 8:	{ /* (1,1,1,2) type */
					f[n]=rhob*f[n];
					r[n]=rev[n]*1.e-10*pow(T9,-1.5)*rhob*exp(-q9[n]/T9)*f[n];
					cn1=Y[j]*f[n]/2.;
					cn2=Y[i]*f[n]/2.;
					cn3=Y[l]*Y[l]*r[n]/6.;
					cn4=Y[k]*Y[l]*r[n]/3.;
					break;}

				case 9:	{ /* (1,1,0,3) type */
					f[n]=rhob*f[n];
					r[n]=rev[n]*1.e-10*pow(T9,-1.5)*rhob*exp(-q9[n]/T9)*f[n];
					cn1=Y[j]*f[n]/2.;
					cn2=Y[i]*f[n]/2.;
					cn3=0.;
					cn4=Y[l]*Y[l]*r[n]/6.;
					break;}

				case 10:{ /* (2,0,2,1) type */
					f[n]=rhob*f[n];
					r[n]=rev[n]*1.e-10*pow(T9,-1.5)*rhob*exp(-q9[n]/T9)*f[n];
					cn1=Y[i]*f[n]/2.;
					cn2=0.;
					cn3=Y[l]*Y[k]*r[n]/3.;
					cn4=Y[k]*Y[k]*r[n]/6.;}
			}

			i=NNUC-1-i;
			j=NNUC-1-j;
			k=NNUC-1-k;
			l=NNUC-1-l;
			
			if(j<NNUC) a[j][i]+=rn2*cn1;
			if(k<NNUC) a[k][i]-=rn3*cn1;
			a[i][i]+=rn1*cn1;
			a[l][i]-=rn4*cn1;
			
			if (j<NNUC) 
			{
				a[j][j]+=rn2*cn2;
				if(k<NNUC) a[k][j]-=rn3*cn2;
				a[i][j]+=rn1*cn2;
				a[l][j]-=rn4*cn2;
			}
			
			if (k<NNUC)
			{
				if(j<NNUC) a[j][k]-=rn2*cn3;
				a[k][k]+=rn3*cn3;
				a[i][k]-=rn1*cn3;
				a[l][k]+=rn4*cn3;
			}
			if(j<NNUC) a[j][l]-=rn2*cn4;
			if(k<NNUC) a[k][l]+=rn3*cn4;
			a[i][l]-=rn1*cn4;
			a[l][l]+=rn4*cn4;
		}
	}
	
	bdln=H*3.*1.e-5;
	
	for(i=0;i<NNUC;i++)
	{
		i1=NNUC-1-i;
		for(j=0;j<NNUC;j++)
		{
			j1=NNUC-1-j;
			if(fabs(a[j][i])<bdln*Y0[j1]/Y0[i1]) a[j][i]=0.;
			else a[j][i]*=dt;
		}
		a[i][i]+=1.;
		b[i1]=Y0[i];
	}

	if(loop==1) icnvm=ip; else icnvm=c0;
	
	nord=0;
	fail=0;
	for(i=0;i<NNUC;i++)
	{
		x[i]=b[i];
		yx[i]=0.;
	}
	
	if(icnvm==inc) for(i=0;i<NNUC;i++) for(j=0;j<NNUC;j++) a0[j][i]=a[j][i];

	for(i=0;i<NNUC;i++)
	{
		if(a[i][i]==0.)
		{
			fail=i;
			return fail;
		}

		for(j=i+1;j<NNUC;j++)
		{
			if(a[j][i]!=0.)
			{
				cx=a[j][i]/a[i][i];
				for(k=i+1;k<NNUC;k++) a[j][k]-=cx*a[i][k];
				a[j][i]=cx;
				x[j]-=cx*x[i];
			}
		}
	}

	do
	{	x[NNUC-1]/=a[NNUC-1][NNUC-1];
		yx[NNUC-1]+=x[NNUC-1];
		
		for(i=NNUC-2;i>=0;i--)
		{
			sum=0.;
			for(j=i+1;j<NNUC;j++) sum+=a[i][j]*x[j];
			x[i]=(x[i]-sum)/a[i][i];
			yx[i]+=x[i];
		}

		test=1;
	
		if(icnvm==inc) 
		{
			for(i=0;i<NNUC;i++) 
			{
				if(yx[i]!=0.) 
				{
					xdy=fabs(x[i]/yx[i]);
					if(xdy>2.e-4) 
					{
						if(nord<1) 
						{
							nord++;
							
							for(j=0;j<NNUC;j++) 
							{
								t = 0.;
								for(k=0;k<NNUC;k++) t+=a0[j][k]*yx[k];
								x[j]=b[j]-t;
							}

							for(j=1;j<NNUC;j++) for(k=j+1;k<NNUC;k++) x[k]-=a[k][j]*x[j];
							break;
						}
						else
						{
							fail=-1;
							ierror=i;
							return fail;
						}
					}
					else test=0;
				}
				else test=0;
			}
		}
		else test=0;
	}
	while(test);

	for(i=0;i<NNUC;i++) 
	{
		yY[i]=yx[NNUC-i-1];
		dY_dt[i]=(yY[i]-Y0[i])/dt;
	}

#ifdef DEBUG
	if(fail!=0)
	{
		if(fail==-1) printf("y(%d) failed to converge\n",ierror);
		if(fail>=1) printf("%d th diagonal term equals zero\n",fail);
	}
#endif		
	return fail;
}

/*----------------------------------------------------*/

int nucl(int err, struct relicparam* paramrelic, double *eta, double *H2_H, double *He3_H2, double *Yp, double *Li7_H, double *Li6_Li7, double *Be7_H)
/* Main routine with computes the abundance ratios H2_H, ..., Be7_H as well as the baryon-to-photon ratio eta, using the parameters contained in paramrelic. The err parameter is a switch to choose if the central (err=0), high (err=1) or low (err=2) values of the nuclear rates is used. */
{
	int i;
	double f[NNUCREAC],r[NNUCREAC];
	double sd;
	double rhod, sum_Y;
	double sum_dY_dt, sum_ZY, dsd_dT9, dphie_dT9, dlna3_dT9, dphie_dlna3, dphie_dZY, sum_DeltaMdY_dt, sum_ZdY_dt;
	double cosh1, cosh2, cosh3, cosh4, cosh5, cosh6, cosh7, sinh1, sinh2, sinh3, sinh4, sinh5, sinh6, sinh7;
	double T90,h_eta0,phie0;
	double dtl;
	int loop;
	double dh_dt, dphie_dt, dT9_dt, dlnT9_dt;
	double dT90_dt, dh_dt0, dphie_dt0;
	double dY_dt0[NNUC],dY_dt[NNUC],Y0[NNUC],Y[NNUC];
	double dtmin;
	double z;
	double ratioH[NNUC];
	double H;

	/* Nuclides: 0=n, 1=p, 2=H2, 3=H3, 4=He3, 5=He4, 6=Li6, 7=Li7, 8=Be7, 9=Li8, 10=B8, 11=Be9, 12=B10, 13=B11, 14=C11, 15=B12, 16=C12, 17=N12, 18=C13, 19=N13, 20=C14, 21=N14, 22=O14, 23=N15, 24=O15, 25=O16 */
	
	double Am[NNUC] = {1., 1., 2., 3., 3., 4., 6., 7., 7., 8., 8., 9., 10., 11., 11., 12., 12., 12., 13., 13., 14., 14., 14., 15., 15., 16.}; /* Atomic number A */
		
	double Zm[NNUC] = { 0., 1., 1., 1., 2., 2., 3., 3., 4., 3., 5., 4., 5., 5., 6., 5., 6., 7., 6., 7., 6., 7., 8., 7., 8., 8.}; /* Charge number Z */
		
	double Dm[NNUC] = {8.071388, 7.289028, 13.135825, 14.949915, 14.931325, 2.424931, 14.0864, 14.9078, 15.7696, 20.9464, 22.9212, 11.34758, 12.05086, 8.6680, 10.6506, 13.3690, 0., 17.3382, 3.125036, 5.3455, 3.019916, 2.863440, 8.006521, 0.101439, 2.8554, -4.737036}; /* mass excess DeltaM */	

	double reacparam[NNUCREAC][8] = 
	{
/* type: 0-10, each type has a unique (#n1,#n2,#n3,#n4) quartet */
/* n1: incoming nuclide number */
/* n2: incoming light nuclide number */
/* n3: outgoing light nuclide number */
/* n4: outgoing nuclide number */
/* rev: reverse reaction coefficient */
/* q: energy release in reaction */

/*   reac# type n1 n2 n3 n4  rev   q   */
	1.,0.,0.,-1.,-1.,1.,0.,0.,		// n -> p
	2.,0.,3.,-1.,-1.,4.,0.,0.,		// H3 -> e- + v + He3
	3.,3.,9.,-1.,-1.,5.,0.,0.,		// Li8 -> e- + v + 2He4
	4.,0.,15.,-1.,-1.,16.,0.,0.,		// B12 -> e- + B + C12
	5.,0.,20.,-1.,-1.,21.,0.,0.,		// C14 -> e- + v + N14
	6.,3.,10.,-1.,-1.,5.,0.,0.,		// B8 -> e+ + v + 2He4
	7.,0.,14.,-1.,-1.,13.,0.,0.,		// C11 -> e+ + v + B11
	8.,0.,17.,-1.,-1.,16.,0.,0.,		// N12 -> e+ + v + C12
	9.,0.,19.,-1.,-1.,18.,0.,0.,		// N13 -> e+ + v + C13
	10.,0.,22.,-1.,-1.,21.,0.,0.,		// O14 -> e+ + v + N14
	11.,0.,24.,-1.,-1.,23.,0.,0.,		// O15 -> e+ + v + N15
	12.,1.,1.,0.,-1.,2.,0.471,25.82,	// H + n -> g + H2
	13.,1.,2.,0.,-1.,3.,1.63,72.62,		// H2 + n -> g + H3
	14.,1.,4.,0.,-1.,5.,2.61,238.81,	// He3 + n -> g + He4
	15.,1.,6.,0.,-1.,7.,1.19,84.17,		// Li6 + n -> g + Li7
	16.,2.,4.,0.,1.,3.,1.002,8.863,		// He3 + n -> p + H3
	17.,2.,8.,0.,1.,7.,0.998,19.081,	// Be7 + n -> p + Li7
	18.,2.,6.,0.,3.,5.,1.070,55.494,	// Li6 + n -> a + H3
	19.,4.,8.,0.,-1.,5.,4.70,220.39,	// Be7 + n -> a + He4
	20.,1.,2.,1.,-1.,4.,1.63,63.750,	// H2 + p -> g + He3
	21.,1.,3.,1.,-1.,5.,2.61,229.932,	// H3 + p -> g + He4
	22.,1.,6.,1.,-1.,8.,1.19,65.054,	// Li6 + p -> g + Be7
	23.,2.,6.,1.,4.,5.,1.07,46.631,		// Li6 + p -> a + He3
	24.,4.,7.,1.,-1.,5.,4.69,201.291,	// Li7 + p -> a + He4
	25.,1.,5.,2.,-1.,6.,1.53,17.118,	// H2 + a -> p + Li6
	26.,1.,5.,3.,-1.,7.,1.11,28.640,	// H3 + a -> p + Li7
	27.,1.,5.,4.,-1.,8.,1.11,18.423,	// He3 + a -> p + Be7
	28.,5.,2.,-1.,0.,4.,1.73,37.935,	// H2 + d -> p + He3
	29.,5.,2.,-1.,1.,3.,1.73,46.798,	// H2 + d -> n + H3
	30.,2.,3.,2.,0.,5.,5.54,204.117,	// H3 + d -> n + He4
	31.,2.,4.,2.,1.,5.,5.55,212.980,	// He3 + d -> p + He4
	32.,10.,4.,-1.,1.,5.,3.39,149.230,	// He3 + He3 -> 2p + He4
	33.,8.,7.,2.,0.,5.,9.95,175.476,	// Li7 + d -> n + a + He4
	34.,8.,8.,2.,1.,5.,9.97,194.557,	// Be7 + d -> p + a + He4
	35.,1.,7.,0.,-1.,9.,1.31,23.59,		// Li7 + n -> g + Li8
	36.,1.,12.,0.,-1.,13.,3.04,132.95,	// B10 + n -> g + B11
	37.,1.,13.,0.,-1.,15.,2.34,39.10,	// B11 + n -> g + B12
	38.,2.,14.,0.,1.,13.,1.002,32.080,	// C11 + n -> p + B11
	39.,2.,12.,0.,5.,7.,0.758,32.382,	// B10 + n -> a + Li7
	40.,1.,8.,1.,-1.,10.,1.30,1.595,	// Be7 + p -> g + B8
	41.,1.,11.,1.,-1.,12.,0.973,76.427,	// Be9 + p -> g + B10
	42.,1.,12.,1.,-1.,14.,3.03,100.840,	// B10 + p -> g + C11
	43.,1.,13.,1.,-1.,16.,7.01,185.173,	// B11 + p -> g + C12
	44.,1.,14.,1.,-1.,17.,2.33,6.975,	// C11 + p -> g + N12
	45.,2.,15.,1.,0.,16.,3.00,146.08,	// B12 + p -> n + C12
	46.,2.,11.,1.,5.,6.,0.618,24.674,	// Be9 + p -> a + Li6
	47.,2.,12.,1.,5.,8.,0.754,13.301,	// B10 + p -> a + Be7
	48.,2.,15.,1.,5.,11.,0.292,79.89,	// B12 + p -> a + Be9
	49.,1.,6.,5.,-1.,12.,1.58,51.753,	// Li6 + a -> g + B10
	50.,1.,7.,5.,-1.,13.,4.02,100.538,	// Li7 + a -> g + B11
	51.,1.,8.,5.,-1.,14.,4.02,87.539,	// Be7 + a -> g + C11
	52.,2.,10.,5.,1.,14.,3.08,86.00,	// B8 + a -> p + C11
	53.,2.,9.,5.,0.,13.,3.07,76.96,		// Li8 + a -> n + B11
	54.,2.,11.,5.,0.,16.,10.3,66.160,	// Be9 + a -> n + C12
	55.,2.,11.,2.,0.,12.,2.07,50.63,	// Be9 + d -> n + B10
	56.,2.,12.,2.,1.,13.,6.44,107.13,	// B10 + d -> p + B11
	57.,2.,13.,2.,0.,16.,14.9,159.36,	// B11 + d -> n + C12
	58.,7.,5.,0.,-1.,11.,0.584,18.260,	// He4 + a + n -> g + Be9
	59.,6.,5.,-1.,-1.,16.,2.00,84.420,	// He4 + 2a -> g + C12
	60.,8.,9.,1.,0.,5.,3.58,177.73,		// Li8 + p -> n + a + He4
	61.,8.,10.,0.,1.,5.,3.58,218.82,	// B8 + n -> p + a + He4
	62.,8.,11.,1.,2.,5.,0.807,7.555,	// Be9 + p -> d + a + He4
	63.,9.,13.,1.,-1.,5.,3.50,100.753,	// B11 + p -> 2a + Be4
	64.,9.,14.,0.,-1.,5.,3.49,132.83,	// C11 + n -> 2a + He4
	65.,1.,16.,0.,-1.,18.,0.886,57.41,	// C12 + n -> g + C13
	66.,1.,18.,0.,-1.,20.,3.58,94.88,	// C13 + n -> g + C14
	67.,1.,21.,0.,-1.,23.,2.71,125.74,	// N14 + n -> g + N15
	68.,2.,19.,0.,1.,18.,1.002,34.846,	// N13 + n -> p + C13
	69.,2.,21.,0.,1.,20.,3.003,7.263,	// N14 + n -> p + C14
	70.,2.,24.,0.,1.,23.,1.002,41.037,	// O15 + n -> p + N15
	71.,2.,24.,0.,5.,16.,0.709,98.661,	// O15 + n -> a + C12
	72.,1.,16.,1.,-1.,19.,0.884,22.553,	// C12 + p -> g + N13
	73.,1.,18.,1.,-1.,21.,1.19,87.621,	// C13 + p -> g + N14
	74.,1.,20.,1.,-1.,23.,0.900,118.452,	// C14 + p -> g + N15
	75.,1.,19.,1.,-1.,22.,3.57,53.706,	// N13 + p -> g + O14
	76.,1.,21.,1.,-1.,24.,2.70,84.678,	// N14 + p -> g + O15
	77.,2.,23.,1.,-1.,25.,3.62,140.734,	// N15 + p -> g + O16
	78.,2.,23.,1.,5.,16.,0.706,57.623,	// N15 + p -> a + C12
	79.,1.,16.,5.,-1.,25.,5.13,83.111,	// C12 + a -> g + O16
	80.,2.,12.,5.,1.,18.,9.36,47.16,	// B10 + a -> p + C13
	81.,2.,13.,5.,1.,20.,11.0,9.098,	// B11 + a -> p + C14
	82.,2.,14.,5.,1.,21.,3.68,33.915,	// C11 + a -> p + N14
	83.,3.,17.,5.,1.,24.,4.26,111.87,	// N12 + a -> p + O15
	84.,3.,19.,5.,1.,25.,5.81,60.557,	// N13 + a -> p + O16
	85.,2.,12.,5.,0.,19.,9.34,12.287,	// B10 + a -> n + N13
	86.,2.,13.,5.,0.,21.,3.67,1.835,	// B11 + a -> n + N14
	87.,2.,15.,5.,0.,23.,4.25,88.47,	// B12 + a -> n + N15
	88.,2.,18.,5.,0.,25.,5.79,25.711	// C13 + a -> n + O16
	};
		
	for(i=0;i<=NNUCREAC-1;i++) 
	{
		f[i] = 0.;
		r[i] = 0.;
	}

	double cy=0.1;
	double ct=0.01;
	double T9i=100.;
	double T9f=0.01;
	double Ytmin =1.e-30;
	int inc=50;
	double dt0=1.e-4;
	
	int ltime=0;
	int nitmax=1000;
	int is=1;
	int ip=inc;
	int it=0;
	int fail=0;

	double T9=T9i;
	double Tnu=T9;
	double t=1./pow(T9*0.09615,2.);
	double dt=dt0;

	if (15.011 / T9 > 58.)
	{
		Y[0] = 1.e-25;
		Y[1] = 1.;
	} 
	else if (15.011 / T9 < -58.)
	{
		Y[0] = 1.;
		Y[1] = 1.e-25;
	} 
	else 
	{
		Y[0] = 1. / (exp(15.011 / T9) + 1.);
		Y[1] = 1. / (exp(-15.011 / T9) + 1.);
	}

	Y0[0]=Y[0];
	Y0[1]=Y[1];

	z=5.929862032115561/T9;
	double h_eta=paramrelic->eta0*33683.*2.75;
	double phie=h_eta*(Y[1]*1.784e-5)/(pow(z,3.)*0.5*(Lbessel(z)-Lbessel(2.*z)*2.+Lbessel(3.*z)*3.-Lbessel(4.*z)*4.+Lbessel(5.*z)*5.-Lbessel(6.*z)*6.+Lbessel(7.*z)*7.));
	double rhob0=h_eta*pow(T9,3.);	
	double rho_gamma,drho_gamma_dT9,P_gamma,rho_epem;
	double drho_epem_dT9,drho_epem_dphie,P_epem,rho_neutrinos,rho_baryons;
	double dM_epem_dT9,dN_epem_dphie;
	
	Y[2] =Y[0]*Y[1]*rhob0*exp(25.82/T9)/(pow(T9,1.5)*4.71e9);
	Y0[2]=Y[2];
	for (i = 4; i <= NNUC; ++i) 
	{
		Y[i-1]=Ytmin;
		Y0[i-1]=Y[i-1];
	}
	rate_weak(err,f);



	while(ltime == 0)
	{
		for(loop=1;loop<=2;loop++)
		{
			rhod=dark_density(T9/11.605,paramrelic);		
			sd=dark_entropy(T9/11.605,paramrelic)/ 1.1605e10;
			dsd_dT9=dark_entropy_derivative(T9/11.605,paramrelic)/11.605/1.1605e10;

			z=5.929862032115561/T9;
			Tnu=pow(h_eta*T9*T9*T9/rhob0,4./3.)*T9i;
			if (phie<=17.)
			{
				cosh1=cosh(phie);
				cosh2=cosh(phie*2.);
				cosh3=cosh(phie*3.);
				cosh4=cosh(phie*4.);
				cosh5=cosh(phie*5.);
				cosh6=cosh(phie*6.);
				cosh7=cosh(phie*7.);
				sinh1=sinh(phie);
				sinh2=sinh(phie*2.);
				sinh3=sinh(phie*3.);
				sinh4=sinh(phie*4.);
				sinh5=sinh(phie*5.);
				sinh6=sinh(phie*6.);
				sinh7=sinh(phie*7.);
			}
			else
			{
				cosh1=0.;
				cosh2=0.;
				cosh3=0.;
				cosh4=0.;
				cosh5=0.;
				cosh6=0.;
				cosh7=0.;
				sinh1=0.;
				sinh2=0.;
				sinh3=0.;
				sinh4=0.;
				sinh5=0.;
				sinh6=0.;
				sinh7=0.;
			}

			rho_gamma=8.418*pow(T9,4.);
			
			drho_gamma_dT9=rho_gamma*4./T9;
			
			P_gamma=rho_gamma/3.;
				rho_epem=(Mbessel(z)*cosh1-Mbessel(2.*z)*cosh2+Mbessel(3.*z)*cosh3-Mbessel(4.*z)*cosh4+Mbessel(5.*z)*cosh5-Mbessel(6.*z)*cosh6+Mbessel(7.*z)*cosh7)*3206.; /* rho_e+ + rho_e- */
			
			drho_epem_dT9=z/T9*3206.*(Nbessel(z)*cosh1-Nbessel(2.*z)*2.*cosh2+Nbessel(3.*z)*3.*cosh3-Nbessel(4.*z)*4.*cosh4+Nbessel(5.*z)*5.*cosh5-Nbessel(6.*z)*6.*cosh6+Nbessel(7.*z)*7.*cosh7); /* d(rho_e+ + rho_e-)/d(T9) */
			
			drho_epem_dphie=(Mbessel(z)*sinh1-Mbessel(2.*z)*2.*sinh2+Mbessel(3.*z)*3.*sinh3-Mbessel(4.*z)*4.*sinh4+Mbessel(5.*z)*5.*sinh5-Mbessel(6.*z)*6.*sinh6+Mbessel(7.*z)*7.*sinh7)*3206.; /* d(rho_e+ + rho_e-)/d(phie) */
			
			P_epem=(Lbessel(z)*cosh1/z-Lbessel(2.*z)*cosh2/(z*2.)+Lbessel(3.*z)*cosh3/(z*3.)-Lbessel(4.*z)*cosh4/(z*4.)+Lbessel(5.*z)*cosh5/(z*5.)-Lbessel(6.*z)*cosh6/(z*6.)+Lbessel(7.*z)*cosh7/(z*7.))*3206.; /* P_e+ + P_e- */
			
			rho_neutrinos=paramrelic->nbnu*pow(T9i,4.)*7.366*pow(h_eta*T9*T9*T9/rhob0,4./3.);		
						
			rho_baryons=h_eta*T9*T9*T9;
			dM_epem_dT9=-(z*z*z/T9)*(sinh1*(Lbessel(z)*3.-z*Mbessel(z))-sinh2*(Lbessel(2.*z)*3.-z*2.*Mbessel(2.*z))+sinh3*(Lbessel(3.*z)*3.-z*3.*Mbessel(3.*z))-sinh4*(Lbessel(4.*z)*3.-z*4.*Mbessel(4.*z))+sinh5*(Lbessel(5.*z)*3.-z*5.*Mbessel(5.*z))-sinh6*(Lbessel(6.*z)*3.-z*6.*Mbessel(6.*z))+sinh7*(Lbessel(7.*z)*3.-z*7.*Mbessel(7.*z))); /* d(pi^2 (hbar*c)^3 (ne- - ne+)*z^3 / 2(m c^2)^3) / d(T9) */
			
			dN_epem_dphie=z*z*z*(cosh1*Lbessel(z)-cosh2*2.*Lbessel(2.*z)+cosh3*3.*Lbessel(3.*z)-cosh4*4.*Lbessel(4.*z)+cosh5*5.*Lbessel(5.*z)-cosh6*6.*Lbessel(6.*z)+cosh7*7.*Lbessel(7.*z));
			if(dN_epem_dphie!=0.) dN_epem_dphie=1./dN_epem_dphie; /* d(pi^2/2 N_A (hbar*c/k)^3 h sum Z_i Y_i)/d(phie) */
			
			H=sqrt(Gn*8.*pi/3.*(rho_gamma+rho_epem+rho_neutrinos+rho_baryons+rhod));
			
			rate_pn(err,paramrelic,f,r,T9);
			
			rate_all(err,f,T9);
		
			fail=linearize(paramrelic,T9,reacparam,f,r,loop,inc,ip,dt,Y0,Y,dY_dt,H,rho_baryons);

			if(fail>0) return 0;
			
			sum_Y=0.;
			sum_ZY=0.;
			sum_dY_dt=0.;
			sum_DeltaMdY_dt=0.;
			sum_ZdY_dt=0.;

			for (i=0;i<NNUC;i++)
			{
				sum_Y+=Y[i];
				sum_ZY+=Zm[i]*Y[i];
				sum_dY_dt+=dY_dt[i];
				sum_DeltaMdY_dt+=Dm[i]*dY_dt[i];
				sum_ZdY_dt+=Zm[i]*dY_dt[i];
			}
		
			dphie_dT9=dN_epem_dphie*(-1.07e-4*h_eta*sum_ZY/T9-dM_epem_dT9);
			dphie_dlna3=-dN_epem_dphie*3.568e-5*h_eta*sum_ZY;
			dphie_dZY=dN_epem_dphie*3.568e-5*h_eta;
						dlna3_dT9=-(drho_gamma_dT9+drho_epem_dT9+drho_epem_dphie*dphie_dT9+rho_baryons*1.388e-4*sum_Y+T9*1e9*dsd_dT9)/(rho_gamma+P_gamma+rho_epem+P_epem+rho_baryons*(9.25e-5*T9*sum_Y+1.388e-4*T9*sum_dY_dt/(H*3.)+sum_DeltaMdY_dt/(H*3.))+T9*1.e9*sd+drho_epem_dphie*(dphie_dlna3+dphie_dZY*sum_ZdY_dt/(H*3.)));

			dT9_dt=3.*H/dlna3_dT9;
			dlnT9_dt=dT9_dt/T9;
			dh_dt=-h_eta*(H*3.+dlnT9_dt*3.);
			dphie_dt=dphie_dT9*dT9_dt+dphie_dlna3*(H*3.)+dphie_dZY*sum_ZdY_dt;

			if (T9 <= T9f || dt < fabs(1e-16 / dlnT9_dt) || ip == inc) 
			{
				it++;
				for (i=0;i<NNUC;i++) ratioH[i]=Y[i]/Y[1];
				ratioH[1]=Y[1]*Am[1];
				ratioH[5]=Y[5]*Am[5];
				for(i=0;i<=8;i++) ratioH[9]+=ratioH[i];
				ratioH[9]-=1.;
				*eta = h_eta / 33683.;
				if((it==nitmax)||(ip<inc)) ltime = 1;
			}

			if(loop==1)
			{
				if(ip==inc) ip=0;
				ip++;
				is++;
				if(is>3)
				{
					dtmin=fabs(1./dlnT9_dt)*ct;
					for (i=0;i<NNUC;i++)
					{
						if ((dY_dt[i]!=0.)&&(Y[i]>Ytmin)) 
						{
							dtl=(fabs(Y[i]/dY_dt[i]))*cy*(pow(log10(Y[i])/log10(Ytmin),2.)+1.);
							if (dtl<dtmin) dtmin=dtl;
						}
					}
					if (dtmin>dt*1.5) dtmin=dt*1.5;
					dt=dtmin;
				}
				t+=dt;
			
				T90=T9;
				h_eta0=h_eta;
				phie0=phie;
				
				dT90_dt=dT9_dt;
				dh_dt0=dh_dt;
				dphie_dt0=dphie_dt;
	
				T9=T90+dT90_dt*dt;
				h_eta=h_eta0+dh_dt0*dt;
				phie=phie0+dphie_dt0*dt;
	
				for (i=0;i<NNUC;i++) 
				{
					Y0[i]=Y[i];
					dY_dt0[i]=dY_dt[i];
					Y[i]=Y0[i]+dY_dt0[i]*dt;
					if(Y[i]<Ytmin) Y[i]=Ytmin;
				}
			}
			else /* if(loop==2) */
			{
				T9=T90+(dT9_dt+dT90_dt)*0.5*dt;
				h_eta=h_eta0+(dh_dt+dh_dt0)*0.5*dt;
				phie=phie0+(dphie_dt+dphie_dt0)*0.5*dt;

				for (i=0;i<NNUC;i++) 
				{
					Y[i]=Y0[i]+(dY_dt[i]+dY_dt0[i])*0.5*dt;
					if (Y[i]<Ytmin) Y[i]=Ytmin;
				}
			}
		}	
	}
		
	ratioH[7]+=ratioH[8];
	ratioH[4]+=ratioH[3];
	
	*H2_H=ratioH[2];
	*He3_H2=ratioH[4]/ratioH[2];
	*Yp=ratioH[5];
	*Li7_H=ratioH[7];
	*Li6_Li7=ratioH[6]/ratioH[7];
	*Be7_H=ratioH[8];
	
	return fail;
}

/*----------------------------------------------------*/

int bbn_excluded(int err, struct relicparam* paramrelic)
/* "container" function which computes the abundances of the elements using the parameters of paramrelic and compares them to observational limits. The err parameter is a switch to choose if the central (err=0), high (err=1) or low (err=2) values of the nuclear rates is used. Returns 1 if the considered BBN scenario is allowed, 0 otherwise. */
{	 
	double eta,H2_H,He3_H2,Yp,Li7_H,Li6_Li7,Be7_H;
		
	if(nucl(err,paramrelic, &eta, &H2_H, &He3_H2, &Yp, &Li7_H, &Li6_Li7, &Be7_H)==0)
	{
#ifdef DEBUG
		printf("Yp\t\t H2/H\t\t He3/H2\t\t Li7/H\t\t Li6/Li7\t Be7/H\n");
		printf("%.3e\t %.3e\t %.3e\t %.3e\t %.3e\t %.3e\n",Yp,H2_H,He3_H2,Li7_H,Li6_Li7,Be7_H);	
#endif		
		if(isnan(Yp)||isnan(H2_H)||isnan(He3_H2)||isnan(Li7_H)||isnan(Li6_Li7)||isnan(Be7_H)) return -1;
		if((Yp<0.258)&&((H2_H>1.2e-5)&&(H2_H<5.3e-5))&&(He3_H2<1.52)&&(Li7_H>0.85e-10)&&(Li6_Li7<0.66)) return 0; /* Conservative intervals from hep-ph/0604251 */ 
		else return 1;
	}
	else return -1;
}
