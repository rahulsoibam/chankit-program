#include "include.h"


double complex polylog(int n, int m, double x)
/* calculates the polylogarithm function of orders n and m of x */
{
	double complex z,v[6],sk,sj;

        int fct[5]={1,1,2,6,24};
	int sgn[5]={1,-1,1,-1,1};
	int index[32]={0,1,2,3,4,0,0,0,0,0,0,5,6,7,0,0,0,0,0,0,0,8,9,0,0,0,0,0,0,0,0,10};
	int nc[11]={0,24,26,28,30,22,24,26,19,22,17};
	
	double u[5],s1[5][5],c[5][5],a[31][11];
      	double z0=0.;
	double z1=1.;
	double hf=z1/2.;
	double c1=4.*z1/3.;
	double c2=z1/3.;

	double x1,h,alfa;
	int je,ke,le;
	int m1,n1,it;
	double q,r,b1,b2,b0;
	
	s1[1][1]=1.6449340668482;
	s1[1][2]=1.2020569031596;
	s1[1][3]=1.0823232337111;
	s1[1][4]=1.0369277551434;
	s1[2][1]=1.2020569031596;
	s1[2][2]=2.7058080842778e-1;
	s1[2][3]=9.6551159989444e-2;
	s1[3][1]=1.0823232337111;
	s1[3][2]=9.6551159989444e-2;
	s1[4][1]=1.0369277551434;
	c[1][1]=1.6449340668482;
	c[1][2]=1.2020569031596;
	c[1][3]=1.0823232337111;
	c[1][4]=1.0369277551434;
	c[2][1]=0.0000000000000;
	c[2][2]=-1.8940656589945;
	c[2][3]=-3.0142321054407;
	c[3][1]=1.8940656589945;
	c[3][2]=3.0142321054407;
	c[4][1]=0.0000000000000;
	a[0][1]=.96753215043498;
	a[1][1]=.16607303292785;
	a[2][1]=.02487932292423;
	a[3][1]=.00468636195945;
	a[4][1]=.00100162749616;
	a[5][1]=.00023200219609;
	a[6][1]=.00005681782272;
	a[7][1]=.00001449630056;
	a[8][1]=.00000381632946;
	a[9][1]=.00000102990426;
	a[10][1]=.00000028357538;
	a[11][1]=.00000007938705;
	a[12][1]=.00000002253670;
	a[13][1]=.00000000647434;
	a[14][1]=.00000000187912;
	a[15][1]=.00000000055029;
	a[16][1]=.00000000016242;
	a[17][1]=.00000000004827;
	a[18][1]=.00000000001444;
	a[19][1]=.00000000000434;
	a[20][1]=.00000000000131;
	a[21][1]=.00000000000040;
	a[22][1]=.00000000000012;
	a[23][1]=.00000000000004;
	a[24][1]=.00000000000001;
	a[0][2]=.95180889127832;
	a[1][2]=.43131131846532;
	a[2][2]=.10002250714905;
	a[3][2]=.02442415595220;
	a[4][2]=.00622512463724;
	a[5][2]=.00164078831235;
	a[6][2]=.00044407920265;
	a[7][2]=.00012277494168;
	a[8][2]=.00003453981284;
	a[9][2]=.00000985869565;
	a[10][2]=.00000284856995;
	a[11][2]=.00000083170847;
	a[12][2]=.00000024503950;
	a[13][2]=.00000007276496;
	a[14][2]=.00000002175802;
	a[15][2]=.00000000654616;
	a[16][2]=.00000000198033;
	a[17][2]=.00000000060204;
	a[18][2]=.00000000018385;
	a[19][2]=.00000000005637;
	a[20][2]=.00000000001735;
	a[21][2]=.00000000000536;
	a[22][2]=.00000000000166;
	a[23][2]=.00000000000052;
	a[24][2]=.00000000000016;
	a[25][2]=.00000000000005;
	a[26][2]=.00000000000002;
	a[0][3]=.98161027991365;
	a[1][3]=.72926806320726;
	a[2][3]=.22774714909321;
	a[3][3]=.06809083296197;
	a[4][3]=.02013701183064;
	a[5][3]=.00595478480197;
	a[6][3]=.00176769013959;
	a[7][3]=.00052748218502;
	a[8][3]=.00015827461460;
	a[9][3]=.00004774922076;
	a[10][3]=.00001447920408;
	a[11][3]=.00000441154886;
	a[12][3]=.00000135003870;
	a[13][3]=.00000041481779;
	a[14][3]=.00000012793307;
	a[15][3]=.00000003959070;
	a[16][3]=.00000001229055;
	a[17][3]=.00000000382658;
	a[18][3]=.00000000119459;
	a[19][3]=.00000000037386;
	a[20][3]=.00000000011727;
	a[21][3]=.00000000003687;
	a[22][3]=.00000000001161;
	a[23][3]=.00000000000366;
	a[24][3]=.00000000000116;
	a[25][3]=.00000000000037;
	a[26][3]=.00000000000012;
	a[27][3]=.00000000000004;
	a[28][3]=.00000000000001;
	a[0][4]=1.0640521184614;
	a[1][4]=1.0691720744981;
	a[2][4]=.41527193251768;
	a[3][4]=.14610332936222;
	a[4][4]=.04904732648784;
	a[5][4]=.01606340860396;
	a[6][4]=.00518889350790;
	a[7][4]=.00166298717324;
	a[8][4]=.00053058279969;
	a[9][4]=.00016887029251;
	a[10][4]=.00005368328059;
	a[11][4]=.00001705923313;
	a[12][4]=.00000542174374;
	a[13][4]=.00000172394082;
	a[14][4]=.00000054853275;
	a[15][4]=.00000017467795;
	a[16][4]=.00000005567550;
	a[17][4]=.00000001776234;
	a[18][4]=.00000000567224;
	a[19][4]=.00000000181313;
	a[20][4]=.00000000058012;
	a[21][4]=.00000000018579;
	a[22][4]=.00000000005955;
	a[23][4]=.00000000001911;
	a[24][4]=.00000000000614;
	a[25][4]=.00000000000197;
	a[26][4]=.00000000000063;
	a[27][4]=.00000000000020;
	a[28][4]=.00000000000007;
	a[29][4]=.00000000000002;
	a[30][4]=.00000000000001;
	a[0][5]=.97920860669175;
	a[1][5]=.08518813148683;
	a[2][5]=.00855985222013;
	a[3][5]=.00121177214413;
	a[4][5]=.00020722768531;
	a[5][5]=.00003996958691;
	a[6][5]=.00000838064065;
	a[7][5]=.00000186848945;
	a[8][5]=.00000043666087;
	a[9][5]=.00000010591733;
	a[10][5]=.00000002647892;
	a[11][5]=.00000000678700;
	a[12][5]=.00000000177654;
	a[13][5]=.00000000047342;
	a[14][5]=.00000000012812;
	a[15][5]=.00000000003514;
	a[16][5]=.00000000000975;
	a[17][5]=.00000000000274;
	a[18][5]=.00000000000077;
	a[19][5]=.00000000000022;
	a[20][5]=.00000000000006;
	a[21][5]=.00000000000002;
	a[22][5]=.00000000000001;
	a[0][6]=.95021851963952;
	a[1][6]=.29052529161433;
	a[2][6]=.05081774061716;
	a[3][6]=.00995543767280;
	a[4][6]=.00211733895031;
	a[5][6]=.00047859470550;
	a[6][6]=.00011334321308;
	a[7][6]=.00002784733104;
	a[8][6]=.00000704788108;
	a[9][6]=.00000182788740;
	a[10][6]=.00000048387492;
	a[11][6]=.00000013033842;
	a[12][6]=.00000003563769;
	a[13][6]=.00000000987174;
	a[14][6]=.00000000276586;
	a[15][6]=.00000000078279;
	a[16][6]=.00000000022354;
	a[17][6]=.00000000006435;
	a[18][6]=.00000000001866;
	a[19][6]=.00000000000545;
	a[20][6]=.00000000000160;
	a[21][6]=.00000000000047;
	a[22][6]=.00000000000014;
	a[23][6]=.00000000000004;
	a[24][6]=.00000000000001;
	a[0][7]=.95064032186777;
	a[1][7]=.54138285465171;
	a[2][7]=.13649979590321;
	a[3][7]=.03417942328207;
	a[4][7]=.00869027883583;
	a[5][7]=.00225284084155;
	a[6][7]=.00059516089806;
	a[7][7]=.00015995617766;
	a[8][7]=.00004365213096;
	a[9][7]=.00001207474688;
	a[10][7]=.00000338018176;
	a[11][7]=.00000095632476;
	a[12][7]=.00000027313129;
	a[13][7]=.00000007866968;
	a[14][7]=.00000002283195;
	a[15][7]=.00000000667205;
	a[16][7]=.00000000196191;
	a[17][7]=.00000000058018;
	a[18][7]=.00000000017246;
	a[19][7]=.00000000005151;
	a[20][7]=.00000000001545;
	a[21][7]=.00000000000465;
	a[22][7]=.00000000000141;
	a[23][7]=.00000000000043;
	a[24][7]=.00000000000013;
	a[25][7]=.00000000000004;
	a[26][7]=.00000000000001;
	a[0][8]=.98800011672229;
	a[1][8]=.04364067609601;
	a[2][8]=.00295091178278;
	a[3][8]=.00031477809720;
	a[4][8]=.00004314846029;
	a[5][8]=.00000693818230;
	a[6][8]=.00000124640350;
	a[7][8]=.00000024293628;
	a[8][8]=.00000005040827;
	a[9][8]=.00000001099075;
	a[10][8]=.00000000249467;
	a[11][8]=.00000000058540;
	a[12][8]=.00000000014127;
	a[13][8]=.00000000003492;
	a[14][8]=.00000000000881;
	a[15][8]=.00000000000226;
	a[16][8]=.00000000000059;
	a[17][8]=.00000000000016;
	a[18][8]=.00000000000004;
	a[19][8]=.00000000000001;
	a[0][9]=.95768506546350;
	a[1][9]=.19725249679534;
	a[2][9]=.02603370313918;
	a[3][9]=.00409382168261;
	a[4][9]=.00072681707110;
	a[5][9]=.00014091879261;
	a[6][9]=.00002920458914;
	a[7][9]=.00000637631144;
	a[8][9]=.00000145167850;
	a[9][9]=.00000034205281;
	a[10][9]=.00000008294302;
	a[11][9]=.00000002060784;
	a[12][9]=.00000000522823;
	a[13][9]=.00000000135066;
	a[14][9]=.00000000035451;
	a[15][9]=.00000000009436;
	a[16][9]=.00000000002543;
	a[17][9]=.00000000000693;
	a[18][9]=.00000000000191;
	a[19][9]=.00000000000053;
	a[20][9]=.00000000000015;
	a[21][9]=.00000000000004;
	a[22][9]=.00000000000001;
	a[0][10]=.99343651671347;
	a[1][10]=.02225770126826;
	a[2][10]=.00101475574703;
	a[3][10]=.00008175156250;
	a[4][10]=.00000899973547;
	a[5][10]=.00000120823987;
	a[6][10]=.00000018616913;
	a[7][10]=.00000003174723;
	a[8][10]=.00000000585215;
	a[9][10]=.00000000114739;
	a[10][10]=.00000000023652;
	a[11][10]=.00000000005082;
	a[12][10]=.00000000001131;
	a[13][10]=.00000000000259;
	a[14][10]=.00000000000061;
	a[15][10]=.00000000000015;
	a[16][10]=.00000000000004;
	a[17][10]=.00000000000001;
   
	if((n<1)||(n>4)||(m<1)||(m>4)||((n+m)>5)) return 0.;

	if(x==1.) z=s1[n][m];

	else if((x>2.)||(x<-1.))
	{
		x1=1./x;
		h=c1*x1+c2;
		alfa=h+h;
		v[0]=1.;
		v[1]=clog(-x+I*z0);
	
		for(le=2;le<=n+m;le++) v[le]=v[1]*v[le-1]/le;
       		
		sk=0.;

		for(ke=0;ke<=m-1;ke++)
		{
			m1=m-ke;
			r=pow(x1,m1)/(fct[m1]*fct[n-1]);
			sj=0.;
			for(je=0;je<=ke;je++)
			{
       				n1=n+ke-je;
       				le=index[10*n1+m1-10];
				b1=0.;
				b2=0.;
				for(it=nc[le];it>=0;it--)
				{
					b0=a[it][le]+alfa*b1-b2;
					b2=b1;
					b1=b0;
   				}
       				q=(fct[n1-1]/fct[ke-je])*(b0-h*b2)*r/pow(m1,n1);
				sj+=v[je]*q;
   			}
			sk+=sgn[ke]*sj;
   		}
		sj=0.;
		for(je=0;je<=n-1;je++) sj+=v[je]*c[n-je][m];
		z=sgn[n]*sk+sgn[m]*(sj+v[n+m]);
	}
	else if(x>hf)
	{
		x1=1.-x;
		h=c1*x1+c2;
		alfa=h+h;
		v[0]=1.;
		u[0]=1.;
		v[1]=clog(x1+I*z0);
		u[1]=clog(x);
       		for(le=2;le<=m;le++) v[le]=v[1]*v[le-1]/le;
       		for(le=2;le<=n;le++) u[le]=u[1]*u[le-1]/le;
		sk=0.;
		for(ke=0;ke<=n-1;ke++) 
		{
			m1=n-ke;
			r=pow(x1,m1)/fct[m1];
			sj=0.;
			for(je=0;je<=m-1;je++)
			{
				n1=m-je;
				le=index[10*n1+m1-10];
				b1=0;
				b2=0;
				for(it=nc[le];it>=0;it--)
 				{
					b0=a[it][le]+alfa*b1-b2;
       					b2=b1;
					b1=b0;
   				}
				q=sgn[je]*(b0-h*b2)*r/pow(m1,n1);
				sj+=v[je]*q;
   			}
			sk=sk+u[ke]*(s1[m1][m]-sj);
   		}
		z=sk+sgn[m]*u[n]*v[m];
       }
       else
       {
		le=index[10*n+m-10];
		h=c1*x+c2;
		alfa=h+h;
		b1=0;
		b2=0;
		for(it=nc[le];it>=0;it--)
		{
			b0=a[it][le]+alfa*b1-b2;
			b2=b1;
			b1=b0;
   		}
		z=(b0-h*b2)*pow(x,m)/(fct[m]*pow(m,n));
       }
       return z;
}

/*--------------------------------------------------------------------*/

double Li2(double x)
/* calculates the dilogarithm function of x */
{
	double pisq6=pow((4.*atan(1.)),2.)/6.;
	double x_0 = -0.3;
	double x_1 = 0.25;
	double x_2 = 0.51;
	if (x == 1.) return pisq6;
	if (x <= x_0)
	{ 
		double temp = log(fabs(1.-x));
		return -Li2(-x/(1.-x)) - temp*temp/2. ; 
	}
	else if (x < x_1)
	{
		double z = - log(1.-x);
		double temp = z*(1.-z/4.*(1.-z/9.*(1.-z*z/100.*(1.-5.*z*z/294.*(1.-7.*z*z/360.*(1.-5.*z*z/242.*(1.-7601.*z*z/354900.*(1.-91.*z*z/4146.*(1.-3617.*z*z/161840.)))))))));
		return temp; 
	}
	else if (x < x_2) return - Li2(-x) + Li2(x*x)/2. ;
	else 
	{ 
		return pisq6 - Li2(1.-x) - log(fabs(x))*log(fabs(1.-x)) ; 
	}
}

/*--------------------------------------------------------------------*/

double Li3(double x)
/* calculates the trilogarithm function of x */
{
	double pisq6=16.*pow(atan(1.),2.)/6.;
	double x_0 = -1.;
	double x_1 = -0.85;
	double x_2 = 0.25;
	double x_3 = 0.63;
	double x_4 =  1.;
	if (x == 1.) return zeta3;
	if (x == -1.) return - 0.75 * zeta3;
	if (x <= x_0)
	{ 
		double lnx = log(-x);
		return Li3(1./x) - pisq6*lnx - lnx*lnx*lnx/6.;
	}
	else if (x < x_1)
	{
		return Li3(x*x)/4. - Li3(-x);
	}
	else if (x < x_2)
	{
		double z = - log(1.-x);
		double temp = z*(1.-3.*z/8.*(1.-17.*z/81.*(1.-15.*z/136.
						  *(1.-28.*z/1875.*(1.+5.*z/8.*(1.-304.*z/7203.
						  *(1.+945.*z/2432.*(1.-44.*z/675.*(1.+7.*z/24.
						  *(1.-26104.*z/307461.*(1.+1925.*z/8023.
						  *(1.-53598548.*z/524808375.
						  *(1.+22232925.*z/107197096.
							)))))))))))));
		return temp; 
	}
	else if (x < x_3)
	{
		return Li3(x*x)/4. - Li3(-x);
	}
	else if (x < x_4)
	{
		double ln1x = log(1.-x); 
		return -Li3(1.-x) - Li3(-x/(1.-x)) + zeta3 + pisq6*ln1x - log(x)*ln1x*ln1x/2. + ln1x*ln1x*ln1x/6.; 
	}
	else 
	{ 
		double lnx = log(x);
		return Li3(1./x) + 2.*pisq6*lnx - lnx*lnx*lnx/6.;
	}
}

/*--------------------------------------------------------------------*/

double Li4(double x)
/* calculates the quadrilogarithm function of x */
{
	return polylog(3,1,x);
}

/*--------------------------------------------------------------------*/

double complex CLi2(double complex x)
/* calculates the dilogarithm function of x, extended to complex numbers */
{
	double pisq6=pow((4.*atan(1.)),2.)/6.;

	double x_0 = -0.30;
	double x_1 = 0.25;
	double x_2 = 0.51;
	if (x == 1.) return pisq6;
	if (creal(x) >= x_2) 
	{ 
		return pisq6 - CLi2(1.-x) - clog(x)*clog(1.-x);
	}
	if ((fabs(cimag(x)) > 1.) || (creal(x)*creal(x) + cimag(x)*cimag(x) > 1.2))
	{
		return - CLi2(1./x) - 0.5 * clog(-x) * clog(-x) - pisq6;
	}
	if (creal(x) <= x_0)
	{ 
		double complex zz = clog(1.-x);
		return -CLi2(-x/(1.-x)) - zz*zz/2. ;
	}
 	else if (creal(x) < x_1)
	{
		double complex z = - clog(1.-x);
		double complex temp = z*(1.-z/4.*(1.-z/9.*(1.-z*z/100.
		*(1.-5.*z*z/294.*(1.-7.*z*z/360.
		*(1.-5.*z*z/242.*(1.-7601.*z*z/354900.
		*(1.-91.*z*z/4146.*(1.-3617.*z*z/161840.)))))))));
		return temp;
	}
		else return - CLi2(-x) + CLi2(x*x)/2.;
}
/*--------------------------------------------------------------------*/

double complex CLi3(double complex x)
/* calculates the trilogarithm function of x, extended to complex numbers */
{
	return polylog(2,1,x);
}

/*--------------------------------------------------------------------*/

double Cl2(double x)
/* calculates the Cl2 function of x */
{
	return cimag(CLi2(cos(x)+I*sin(x)));
}

/*--------------------------------------------------------------------*/

double Cl3(double x)
/* calculates the Cl3 function of x */
{
	return creal(CLi3(cos(x)+I*sin(x)));
}

/*--------------------------------------------------------------------*/

double max(double x, double y)
/* returns the maximum between x and y */
{
	if(x<y) return y; else return x;
}

/*--------------------------------------------------------------------*/

double min(double x, double y)
/* returns the minimum between x and y */
{
	if(x<y) return x; else return y;
}

/*-----------------------------------------------------------------------*/
/* Modified Bessel functions of order 0,1,2,3,4 */
/*-----------------------------------------------------------------------*/

double I0(double x)
/* calculates the Modified Bessel functions of first type and of order 0 of x */
{
	double y;

	double absx=fabs(x);

	if (absx < 3.75) 
	{
		y=x/3.75;
		y=y*y;
		return 1.+y*(3.5156229+y*(3.0899424+y*(1.2067492+y*(0.2659732+y*(0.360768e-1+y*0.45813e-2)))));
 	} 
	else
	{
		y=3.75/absx;
		return (exp(absx)/sqrt(absx))*(0.39894228+y*(0.1328592e-1+y*(0.225319e-2+y*(-0.157565e-2+y*(0.916281e-2+y*(-0.2057706e-1+y*(0.2635537e-1+y*(-0.1647633e-1+y*0.392377e-2))))))));
 	}
}

double I1(double x)
/* calculates the Modified Bessel functions of first type and of order 1 of x */
{
	double y,tmp;
	double I1=0.;

	double absx=fabs(x);

	if (absx < 3.75) 
	{
		y=x/3.75;
		y=y*y;
				I1 = absx*(0.5+y*(0.87890594+y*(0.51498869+y*(0.15084934+y*(0.2658733e-1+y*(0.301532e-2+y*0.32411e-3))))));
	}
	else
	{
		y=3.75/absx;
	  		tmp=0.2282967e-1+y*(-0.2895312e-1+y*(0.1787654e-1-y*0.420059e-2));
				tmp=0.39894228+y*(-0.3988024e-1+y*(-0.362018e-2+y*(0.163801e-2+y*(-0.1031555e-1+y*tmp))));
				I1*=(exp(absx)/sqrt(absx));
	}
	if(x<0.) return -I1; else return I1;
}

double K0(double x)
/* calculates the Modified Bessel functions of second type and of order 0 of x */
{
	double y;

	if (x <= 2.) 
	{
		y=x*x/4.;
		return (-log(x/2.)*I0(x))+(-0.57721566+y*(0.42278420+y*(0.23069756+y*(0.3488590e-1+y*(0.262698e-2+y*(0.10750e-3+y*0.74e-5))))));
	} 
	else 
	{
		y=2./x;
				return (exp(-x)/sqrt(x))*(1.25331414+y*(-0.7832358e-1+y*(0.2189568e-1+y*(-0.1062446e-1+y*(0.587872e-2+y*(-0.251540e-2+y*0.53208e-3))))));
		}
}

double K1(double x)
/* calculates the Modified Bessel functions of second type and of order 1 of x */
{
 	double y;

	if (x <= 2.) 
	{
		y=x*x/4.;
		return (log(x/2.)*I1(x))+(1./x)*(1.+y*(0.15443144+y*(-0.67278579+y*(-0.18156897+y*(-0.1919402e-1+y*(-0.110404e-2+y*(-0.4686e-4)))))));
	}
	else 
	{
		y=2./x;
		return (exp(-x)/sqrt(x))*(1.25331414+y*(0.23498619+y*(-0.3655620e-1+y*(0.1504268e-1+y*(-0.780353e-2+y*(0.325614e-2+y*(-0.68245e-3)))))));
		}
}

double K2(double x)
/* calculates the Modified Bessel functions of second type and of order 2 of x */
{
	return K0(x)+2./x*K1(x);
}

double K3(double x)
/* calculates the Modified Bessel functions of second type and of order 3 of x */
{
	return K1(x)+4./x*K2(x);
}

double K4(double x)
/* calculates the Modified Bessel functions of second type and of order 4 of x */
{
	return K2(x)+6./x*K3(x);
}

double Lbessel(double x)
/* calculates L(x)=K2(x)/x */
{
	return K2(x)/x;
}

double Mbessel(double x)
/* calculates M(x)=(3*K3(x)+K1(x))/4x */
{
	return (0.75*K3(x)+0.25*K1(x))/x;
}

double Nbessel(double x)
/* calculates N(x)=(K4(x)+K2(x))/2x */
{
	return (0.5*K4(x)+0.5*K2(x))/x;
}

double K0exp(double x, double z)
/* calculates the extended Modified Bessel functions of second type and of order 0 of x and z */
{
	double y;

	if (x <= 2.) 
	{
		y=x*x/4.;
		return ((-log(x/2.)*I0(x))+(-0.57721566+y*(0.42278420+y*(0.23069756+y*(0.3488590e-1+y*(0.262698e-2+y*(0.10750e-3+y*0.74e-5)))))))*exp(z)*sqrt(z);
	} 
	else 
	{
		y=2./x;
				return (exp(z-x)/sqrt(x/z))*(1.25331414+y*(-0.7832358e-1+y*(0.2189568e-1+y*(-0.1062446e-1+y*(0.587872e-2+y*(-0.251540e-2+y*0.53208e-3))))));
	}
}

double K1exp(double x,double z)
/* calculates the extended Modified Bessel functions of second type and of order 1 of x and z */
{
 	double y;

	if (x <= 2.) 
	{
		y=x*x/4.;
		return ((log(x/2.)*I1(x))+(1./x)*(1.+y*(0.15443144+y*(-0.67278579+y*(-0.18156897+y*(-0.1919402e-1+y*(-0.110404e-2+y*(-0.4686e-4))))))))*exp(z)*sqrt(z);
	}
	else 
	{
		y=2./x;
		return (exp(z-x)/sqrt(x/z))*(1.25331414+y*(0.23498619+y*(-0.3655620e-1+y*(0.1504268e-1+y*(-0.780353e-2+y*(0.325614e-2+y*(-0.68245e-3)))))));
	}
}

double K2exp(double x,double z)
/* calculates the extended Modified Bessel functions of second type and of order 2 of x and z */
{
	return K0exp(x,z)+2./x*K1exp(x,z);
}

/*--------------------------------------------------------------------*/

double expcor(double x)
/* calculates the exponential of x, with limited extension */
{
	if(x>100.) return exp(100.);
	else if(x<-100.) return 0.;
	else return exp(x);
}

/*--------------------------------------------------------------------*/

double kron(int x, int y)
/* kronecker symbol of x and y */
{
	if(x==y) return 1.; else return 0.;
}

/*--------------------------------------------------------------------*/

int test_integer(char name[])
/* tests if the string "name" is an integer, and return 1 if so, 0 otherwise */
{
	char *testint;
	strtol(name,&testint,10);
	return (*testint=='\0');
}

/*--------------------------------------------------------------------*/

int test_file(char *name)
{
/* tests if the file "name" exist, and return 1 if so, 0 otherwise */
	struct stat buffer;
	return (stat(name,&buffer) == 0);
}
