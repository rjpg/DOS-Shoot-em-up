//-------------------(c) 1996 X-prog 
#include <dos.h>
#include <conio.h>
#include <math.h>
#include <mem.h>
#include <stdio.h>
#include <alloc.h>
#include <process.h>
//#include "crystal.h"

#define RADTODEG (180./M_PI)

void tiro (int iy,int ix,int fy,int fx);

/*-----------------------------------------------------------------------*/
/*-------------------------------INICIO.H--------------------------------*/
/*-----------------------------------------------------------------------*/
/*-----------------------------------------------------------------------*/
/*--------------------------ecras globais [y,x]--------------------------*/
char ecra[200][320]; // ecra virtual

char*** mkmat(int f,int n,int k)
	{
	char ***w;
	int i,x;

	w=(char***)calloc(f,sizeof(char));
	for (x=0;x<f;x++)
		w[i]=(char**)calloc(n,sizeof(char));
	for (x=0;x<f;x++)
		for (i=0;i<n;i++)
			w[x][i]=(char *)calloc (k,sizeof(char));
	return w;
	}

/*-------------------------------Turtle----------------------------------*/
class turtle
	{
	private:
		double xdelta, ydelta;
	protected:
		double x, y;
		int heading;
	public:
		turtle ();
		~turtle ();
		int  fw(int& ye,int& xe,int len);
		void lt (int ang);
		void rt (int ang);
		void inixy (int iy,int ix);

		int tgetx(void);

		};
	turtle :: turtle ()
		{
		y=0;
		x=0;
		ydelta =0.;
		xdelta = 1.;
		heading = 0;
		}
	void turtle :: inixy (int iy,int ix)
		{
		y=iy;
		x=ix;
		}
	turtle :: ~turtle ()
		{}

	int  turtle :: fw (int& ye,int& xe,int len)
		{
		double m;
		double oldx = x, oldy =y;
		x += (double) len *xdelta;
		y += (double) len *ydelta;
		ye=y;
		xe=x;

		if (y==oldy)
			if (x>oldx)
				return 3;
			else
				return 7;

		m=(x-oldx)/(y-oldy);
		/*gotoxy (0,10);
		printf ("m:%f\n",m);*/
		if (m<2&&m>=0.5)
			if (x>oldx)
				return 4;
			else
				return 8;

		if (m<0.5&&m>-0.5)
			if (y>oldy)
				return 5;
			else
				return 1;

		if (m<-0.5&&m>-2)
			if (x>oldx)
				return 2;
			else
				return 6;


		if (m<-2)
			if (x>oldx)
				return 3;
			else
				return 7;
		if (m>2)
			if (x>oldx)
				return 3;
			else
				return 7;

		}
	void turtle :: lt (int ang)
	{
	heading += ang;
	xdelta = cos (heading / RADTODEG);
	ydelta = sin (heading / RADTODEG);
	}
	void turtle :: rt (int ang)
	{
	lt (-ang);
	}

/*-------------------------------tipos-----------------------------------*/

	typedef unsigned char linha[320];

	struct proptmp
	{
	int posx,posy,id,tamx,tamy;
	};

/*-----------------------------------------------------------------------*/
void novpal (void);
void setvideomode (int mode);  //iniciar modo grafico 0x13
void pormatriz(void); // passar do ecra virtual para o ecra verdadeiro
void novapal (int start,int registers,char *pal);
void novpal (void);
void getreg(int reg,int *r,int *g,int *b);

/*-------------------------------Class MASTER----------------------------*/
class master/*:public turtle*/
	{
	public:
		master();
		~master();
		virtual int movdisp (void);
		virtual void up (int casas);
		virtual void down (int casas);
		virtual void left (int casas);
		virtual void right (int casas);
		virtual int explo (void);
		virtual proptmp getpro (void);
		virtual int energe (void);
	};
master :: master () {}
master :: ~master () {}
int master :: movdisp (void) {int  i; return i;}
void master :: up (int casas) {}
void master :: down (int casas) {}
void master :: left (int casas) {}
void master :: right (int casas) {}
int master :: explo (void) {return 0;}
int master :: energe (void) {return 0;}
proptmp master :: getpro (void) { proptmp  i; return i;}


/*-------------------------Variaveis globais-----------------------------*/
const maxsprit = 70;
master *all[maxsprit];
int cf=0,typefire=0;
FILE *h;
/*-----------------------------------------------------------------------*/

class tipo1 : public master
	{
	private :
		proptmp nos;
		char sprit [2][10][10];
		int f,lat;
	public:
		tipo1 (int y,int x);
		~tipo1 ();
		int movdisp (void);
		proptmp getpro (void);
	};
tipo1 :: tipo1 (int y,int x)
	{
	lat=3;
	int sx,sy;
	f=0;
	nos.id = 7;
	nos.posx=x;
	nos.posy=y;
	nos.tamx=10;
	nos.tamy=10;
	for (f=0;f<=1;f++)
	for (sy=0;sy!=10;sy++)
		for (sx=0;sx!=10;sx++)
			sprit [f][sy][sx]=100+(f*3);
	}
tipo1 :: ~tipo1 () {}

int tipo1 :: movdisp (void)
	{
	proptmp pi;
	int y,x;
	if (f>1) f=0;
	nos.posy++;
	nos.posx+=lat;
	for (y=0; y!=nos.tamy && nos.posy+y<200 ;y++)
		for (x=0; x!=nos.tamx && nos.posx+x<320  ;x++)
			ecra[nos.posy+y][nos.posx+x]=sprit[f][y][x];

	if (nos.posy >= 200)
		nos.id  = 6;
	f++;
	if (nos.posx+10>=320 || nos.posx <= 1) lat=lat*(-1);
	pi=all[0]->getpro();
	if (pi.tamx+pi.posx >= nos.posx &&
	    pi.posx <= nos.tamx+nos.posx &&
	    pi.tamy+pi.posy >= nos.posy &&
	    pi.posy <= nos.tamy+nos.posy )
		{
		typefire=1;
		nos.id =6;
		}
	return nos.id;
	}

proptmp tipo1 :: getpro (void)
	{
	return nos;
	}





/*-----------------------------------------------------------------------*/

class explosao : public master
	{
	private :
		proptmp nos;
		char sprit1 [5][10][10];
		char sprit2 [6][30][30];
		char ***sprit3 /*[6][60][60]*/;

		int f,nive,xf;
	public:
		explosao (int y,int x,int grau);
		~explosao ();
		int movdisp (void);
		proptmp getpro (void);
	};

explosao :: explosao (int y,int x,int grau)
	{
	int frame,sy,sx;

	switch (grau)
	       {
		case 1 :
	for (frame=0;frame!=5;frame++)
	for (sy=0;sy!=10;sy++)
		for (sx=0;sx!=10;sx++)
			sprit1 [frame][sy][sx]=40+frame;
	f=0;
	xf=0;
	nos.id = 7;
	nos.tamx=10;
	nos.tamy=10;
	nos.posx=x-nos.tamy/2;
	nos.posy=y-nos.tamx/2;
	nive=grau;
	break;
		case 2 :
	if ((h=fopen("explo2.im","rt"))!=NULL)
	{
	fread(sprit2,5400,1,h);
	fclose(h);
	}
	f=0;
	xf=0;
	nos.id = 7;
	nos.tamx=30;
	nos.tamy=30;
	nos.posx=x-nos.tamx/2;
	nos.posy=y-nos.tamx/2;
	nive=grau;
	break;
	/*	case 3:
	sprit3=mkmat(6,60,60);
	if ((h=fopen("explo3.im","rt"))!=NULL)
	{
	fread(sprit3,21600,1,h);
	fclose(h);
	}
	f=0;
	xf=0;
	nos.id = 7;
	nos.tamx=60;
	nos.tamy=60;
	nos.posx=x-nos.tamx/2;
	nos.posy=y-nos.tamx/2;
	nive=grau;
	break;*/
	}
	}

explosao :: ~explosao () {}

int explosao :: movdisp (void)
	{
	int y,x;
	switch (nive)
	{
	case 1 :
	for (y=0; y!=nos.tamy;y++)
		for (x=0; x!=nos.tamx;x++)
			if (nos.posy+y>0 &&
			nos.posy+y<190&&
			nos.posx+x>0&&
			nos.posx+x<320)
			ecra[nos.posy+y][nos.posx+x]=sprit1[f][y][x]+ecra[nos.posy+y][nos.posx+x];
	break;
	case 2 :
	for (y=0; y!=nos.tamy;y++)
		for (x=0; x!=nos.tamx;x++)
			if (nos.posy+y>0 &&
			nos.posy+y<190&&
			nos.posx+x>0&&
			nos.posx+x<320)
			if (sprit2[f][y][x])
			ecra[nos.posy+y][nos.posx+x]=sprit2[f][y][x];
	break;
	/*case 3 :
	for (y=0; y!=nos.tamy;y++)
		for (x=0; x!=nos.tamx;x++)
			if (nos.posy+y>0 &&
			nos.posy+y<190&&
			nos.posx+x>0&&
			nos.posx+x<320)
			if (sprit3[f][y][x])
			ecra[nos.posy+y][nos.posx+x]=sprit3[f][y][x];
	break; */
	}
	if (xf<5) f++; else f--;
	xf++;
	if (xf==10) nos.id=6;
	return nos.id;
	}

proptmp explosao :: getpro (void)
	{
	return nos;
	}
/*---------------------------------nada ---------------------------------*/
class nada : public master
	{
	private :
		proptmp x;
	public:
		nada (void);
		~nada ();
		int movdisp (void);
		proptmp getpro (void);
	};
nada :: nada (void)
	{
	x.id = 0;
	x.posx=0;
	x.posy=0;
	x.tamx=0;
	x.tamy=0;
	}

nada :: ~nada () {}

int nada :: movdisp (void)
	{
	return x.id;
	}

proptmp nada :: getpro (void)
	{
	return x;
	}
/*---------------------------sub classe nave-----------------------------*/
class nave : public master
	{
	private:
		proptmp nos;
		char sprit[20][20];
		int nova;
	public:
		nave (int y,int x);
		~nave();
		int movdisp (void);
		void up (int casas);
		void down (int casas);
		void left (int casas);
		void right (int casas);
		int explo (void);
		proptmp getpro (void);
		int energe (void);
	};
nave :: nave (int y,int x)
	{
	int *buf;
	nos.posy=y;
	nos.posx=x;
	nos.id=7;
	nos.tamy=20;
	nos.tamx=20;
	nova = 0;
	int sx,sy;
	if ((h=fopen("X-NAVE.im","rt"))!=NULL)
	{
	fread(sprit,400,1,h);
	fclose(h);
	}
	}

nave :: ~nave() {}

int nave :: movdisp (void)
	{
	int y,x;
	if (nova++>30 || nova%2)
	for (y=0;y!=nos.tamy;y++)
		for (x=0;x!=nos.tamx;x++)
			if (sprit[y][x]!=0)
			ecra[nos.posy+y][nos.posx+x]=sprit[y][x];
	if (nova==35) nos.id=1;
	return nos.id;
	}

void nave :: up (int casas)
	{
	if (nos.posy-casas >= 1)
		nos.posy=nos.posy-casas;
	}

void nave :: down (int casas)
	{
	if (nos.posy+casas <  200 - nos.tamy)
		nos.posy=nos.posy+casas;
	}

void nave :: left (int casas)
	{
	if (nos.posx-casas >= 1)
		nos.posx=nos.posx-casas;
	}

void nave :: right (int casas)
	{
	if (nos.posx+casas < 320 - nos.tamx )
		nos.posx=nos.posx+casas;
	}
int nave :: explo (void)
	{
	return 2;
	}
proptmp nave :: getpro (void)
	{
	return nos;
	}
int nave :: energe (void)
	{
	return 0;
	}
/*-------------------------fim da sub classe nave------------------------*/

class fire : public master
	{
	private :
		proptmp nos;
		char sprit [10][5];
	public:
		fire (int y,int x);
		~fire ();
		int movdisp (void);
		int explo (void);
		proptmp getpro (void);
		int energe (void);
	};

fire :: fire (int y,int x)
	{
	nos.id=1;
	nos.posy=y;
	nos.posx=x;
	nos.tamy=10;
	nos.tamx=5;
	int sx,sy;

	for (sy=0;sy!=10;sy++)
		for (sx=0;sx!=5;sx++)
			sprit [sy][sx]=0;

	for (sx=0;sx!=9;sx++)
			sprit [sx][2]=4;
	for (sx=2;sx!=10;sx++)
			{
			sprit [sx][1]=5;
			sprit [sx][3]=5;
			}
	sprit [9][0]=6;
	sprit [9][4]=6;
	}

fire :: ~fire () {}
 
int fire :: movdisp (void)
	{
	int y,x;
	nos.posy-=11;

	for (y=0;y!=nos.tamy;y++)
		for (x=0;x!=nos.tamx;x++)
			{
			if (nos.posy+y>0)
			if (sprit[y][x])
			ecra[nos.posy+y][nos.posx+x]=sprit[y][x];
			}

	if (nos.posy <= 1)
		nos.id  = 6;
	return nos.id;
	}
int fire :: explo (void)
	{
	return 1;
	}

proptmp fire :: getpro (void)
	{
	return nos;
	}
int fire :: energe (void)
	{
	return 0;
	}


/*-------------------------------------------------------------------------*/

class fireen : public master
	{
	private :
		proptmp nos;
		char sprit [10][5];
	public:
		fireen (int y,int x);
		~fireen ();
		int movdisp (void);
		int explo (void);
		proptmp getpro (void);
		int energe (void);
	};

fireen :: fireen (int y,int x)
	{
	nos.id=2;
	nos.posy=y;
	nos.posx=x;
	nos.tamy=10;
	nos.tamx=5;
	int sx,sy;

	for (sy=0;sy!=10;sy++)
		for (sx=0;sx!=5;sx++)
			sprit [sy][sx]=0;

	for (sx=1;sx!=10;sx++)
			sprit [sx][2]=24;
	for (sx=0;sx!=9;sx++)
			{
			sprit [sx][1]=25;
			sprit [sx][3]=25;
			}
	sprit [0][0]=26;
	sprit [0][4]=26;
	}

fireen :: ~fireen () {}

int fireen :: movdisp (void)
	{
	int y,x;
	nos.posy+=5;

	for (y=0;y!=nos.tamy;y++)
		for (x=0;x!=nos.tamx;x++)
			{
			if (nos.posy+y<200)
			if (sprit[y][x])
			ecra[nos.posy+y][nos.posx+x]=sprit[y][x];
			}

	if (nos.posy >= 200)
		nos.id  = 6;
	return nos.id;
	}
int fireen :: explo (void)
	{
	return 1;
	}

proptmp fireen :: getpro (void)
	{
	return nos;
	}
int fireen :: energe (void)
	{
	return 0;
	}
/*---------------------------Nave inimiga--------------------------------*/
class enim1 : public master
	{
	private :
		proptmp nos;
		char sprit[2][30][20];
		int ac,vida,f;

	public:
		enim1 (void);
		~enim1 ();
		int movdisp (void);
		int explo (void);
		proptmp getpro (void);
		int energe (void);
	};

enim1 :: enim1 (void)
	{
	ac=0;
	nos.id=2;
	nos.posy=2;
	nos.posx=0;
	nos.tamy=30;
	nos.tamx=20;
	vida = 2;
	f=1;


	if ((h=fopen("en1.im","rt"))!=NULL)
	{
	fread(sprit,1200,1,h);
	fclose(h);
	}


      }

enim1 :: ~enim1 () {}

int enim1 :: movdisp (void)
	{
	int y,x;
	nos.posy+=2;
	nos.posx+=3;


	for (y=0; y!=nos.tamy ;y++)
		for (x=0; x!=nos.tamx  ;x++)
			 if (nos.posy+y>0 &&
			 nos.posy+y<200&&
			 nos.posx+x>0&&
			 nos.posx+x<320)
			 if (sprit[f][y][x]!=0)
			ecra[nos.posy+y][nos.posx+x]=sprit[f][y][x];


	if (nos.posy >=220)
		nos.id  = 6;
	if (ac++==10)
	{tiro (nos.posy+nos.tamy,nos.posx+nos.tamx/2,1,1);ac=0;}

	if (f)
		f=0;
	else
		f=1;

	return nos.id;
	}

int enim1 :: explo (void)
	{
	return 2;
	}

proptmp enim1 :: getpro (void)
	{
	return nos;
	}
int enim1 :: energe (void)
	{
	return vida--;
	}




/*---------------------------nave enimiga 2------------------------------*/
class enim2 : public master
	{
	private :
		proptmp nos;
		char sprit1 [60][40];
		char sprit2 [50][50];
		char sprit3 [40][60];
		int ac,vida,rap;
		turtle a;

	public:
		enim2 (int y,int x,int r);
		~enim2 ();
		int movdisp (void);
		int explo (void);
		proptmp getpro (void);
		int energe (void);
	};

enim2 :: enim2 (int y,int x,int r)
	{

	a.inixy (y,x);
	a.lt (90);

	if ((h=fopen("en2.im","rt"))!=NULL)
	{
	fread(sprit1,2400,1,h);
	rap=r;
	ac=0;
	vida = 4;
	fread(sprit3,2400,1,h);
	nos.id=2;
	nos.posy=y;
	nos.posx=x;
	nos.tamy=50;
	nos.tamx=40;
	a.inixy (y,x);
	fread(sprit2,2500,1,h);
	fclose(h);
	}
	}

enim2 :: ~enim2 () {}

int enim2 :: movdisp (void)
	{
	int sy,sx;
	int direc;

	ac++;

	if (ac<180)
	a.rt (3);
	else
	a.lt (3);
	if (ac>330)
	{
	a.rt(3);
	}

	direc=a.fw(nos.posy,nos.posx,rap);
	switch (direc)
		{
			case 1 :
			nos.tamy=60;
			nos.tamx=40;
			for (sy=0;sy<nos.tamy;sy++)
				for (sx=0;sx<nos.tamx;sx++)

					if (nos.posy+sy>0 &&
					nos.posy+sy<190&&
					nos.posx+sx>0&&
					nos.posx+sx<320)
					if (sprit1[60-sy][sx]!=0)
					ecra[nos.posy+sy][nos.posx+sx]=sprit1[60-sy][sx];
					break;
			case 2 :
			nos.tamy=50;
			nos.tamx=50;
			for (sy=0;sy<nos.tamy;sy++)
				for (sx=0;sx<nos.tamx;sx++)
					if (nos.posy+sy>0 &&
					nos.posy+sy<200&&
					nos.posx+sx>0&&
					nos.posx+sx<320)
					if (sprit2[50-sy][sx]!=0)
					ecra[nos.posy+sy][nos.posx+sx]=sprit2[50-sy][sx];
					break;
			case 3 :
			nos.tamy=40;
			nos.tamx=60;
			for (sy=0;sy<nos.tamy;sy++)
				for (sx=0;sx<nos.tamx;sx++)
					if (nos.posy+sy>0 &&
					nos.posy+sy<200 &&
					nos.posx+sx>0 &&
					nos.posx+sx<320)
					if (sprit3[sy][sx]!=0)
					ecra[nos.posy+sy][nos.posx+sx]=sprit3[sy][sx];
					break;
			case 4 :
			nos.tamy=50;
			nos.tamx=50;
			for (sy=0;sy<nos.tamy;sy++)
				for (sx=0;sx<nos.tamx;sx++)
					if (nos.posy+sy>0 &&
					nos.posy+sy<200 &&
					nos.posx+sx>0 &&
					nos.posx+sx<320)
					if (sprit2[sy][sx]!=0)
					ecra[nos.posy+sy][nos.posx+sx]=sprit2[sy][sx];
					break;
			case 5 :
			nos.tamy=60;
			nos.tamx=40;
			for (sy=0;sy<nos.tamy;sy++)
				for (sx=0;sx<nos.tamx;sx++)
					if (nos.posy+sy>0 &&
					nos.posy+sy<200 &&
					nos.posx+sx>0 &&
					nos.posx+sx<320)
					if (sprit1[sy][sx]!=0)
					ecra[nos.posy+sy][nos.posx+sx]=sprit1[sy][sx];
					break;
			case 6 :
			nos.tamy=50;
			nos.tamx=50;
			for (sy=0;sy<nos.tamy;sy++)
				for (sx=0;sx<nos.tamx;sx++)
					if (nos.posy+sy>0 &&
					nos.posy+sy<200 &&
					nos.posx+sx>0 &&
					nos.posx+sx<320)
					if (sprit2[sy][50-sx]!=0)
					ecra[nos.posy+sy][nos.posx+sx]=sprit2[sy][50-sx];
					break;
			case 7 :
			nos.tamy=40;
			nos.tamx=60;
			for (sy=0;sy<nos.tamy;sy++)
				for (sx=0;sx<nos.tamx;sx++)
					if (nos.posy+sy>0 &&
					nos.posy+sy<200 &&
					nos.posx+sx>0 &&
					nos.posx+sx<320)
					if (sprit3[sy][60-sx]!=0)
					ecra[nos.posy+sy][nos.posx+sx]=sprit3[sy][60-sx];
					break;
			case 8 :
			nos.tamy=50;
			nos.tamx=50;
			for (sy=0;sy<nos.tamy;sy++)
				for (sx=0;sx<nos.tamx;sx++)
					if (nos.posy+sy>0 &&
					nos.posy+sy<200 &&
					nos.posx+sx>0 &&
					nos.posx+sx<320)
					if (sprit2[50-sy][50-sx]!=0)
					ecra[nos.posy+sy][nos.posx+sx]=sprit2[50-sy][50-sx];
					break;


		}
	if (ac>380)
		nos.id=6;

	return nos.id;


	}
int enim2 :: explo (void)
	{
	return 2;
	}

proptmp enim2 :: getpro (void)
	{
	return nos;
	}
int enim2 :: energe (void)
	{
	vida--;
	return vida;
	}

/*-------------------------------Fundo-----------------------------------*/
class fundo
	{
	protected:
	char far *f0;
	int v;
	public:
	fundo (int rap);
	~fundo ();
	void actf(void);
	void velo(int rap);
	};
fundo :: fundo (int rap)
	{
	v=rap;
	if (! ( f0 = (char far *)farmalloc(64000) ) )
	{
	printf ("mem");
	exit(1);
	}


        if ((h=fopen("x-fundo.im","rt"))!=NULL)
                         fread(f0,1,64000,h);

        fclose ( h);
        v=rap;
	}
fundo :: ~fundo ()
	{
	farfree(f0);
	}

void fundo :: actf(void)
	{
        char far *fv;
        if (! ( fv = (char far *)farmalloc(64000) ) )
	{
	printf ("mem");
	exit(1);
	}
	memcpy(fv,f0+(64000-(320*v)),320*v);
	memcpy(fv+(320*v),f0,(64000-(320*v)));
	memcpy(f0,fv,64000);
	memcpy(ecra,f0,64000);
        farfree(fv);

        //memcpy(ecra,f0,64000);
	}
void fundo :: velo(int rap)
	{
	v=v+rap;
	if (v<=0) v=1;
	}



/*-----------------------------------------------------------------------*/
void setvideomode (int mode)  //iniciar modo grafico 0x13
{
	_AH = 0x00;
	_AL = mode;
	geninterrupt (0x10);
}

/*-----------------------------------------------------------------------
void point (int x, int y, int color) // por um ponto no ecra
{
 screen[x][y]=color;
}

/*-----------------------------------------------------------------------*/
void limparmatriz(void) // limpar o ecra virtual
{
register short int col,lin;
for (col =1 ;col!=200;col++)
	for (lin=1;lin!=320;lin++)
	 ecra[col][lin]=0;
}

/*-----------------------------------------------------------------------*/
void pormatriz(void) // passar do ecra virtual para o ecra verdadeiro
{
linha far *screen = (linha far *)0xA0000000L; //ecra veredadeiro

movmem(ecra,screen,sizeof(ecra));

/*register int col,lin;
	for (col =1 ;col<200;col++)
	for (lin=1;lin<320;lin++)
		screen[col][lin]=ecra[col][lin];*/
}

/*------------------------------------------------------------------------*/
void novapal (int start,int registers,char *pal)
	{
	_ES = FP_SEG(pal);
	_DX = FP_OFF(pal);
	_BX = start;
	_CX = registers;
	_AX = 0x1012;
	geninterrupt(0x10);
	}
/*------------------------------------------------------------------------*/
void novpal (void)
	{
	char *x;
	if ((h=fopen("x-pal.xpa","rt"))!=NULL)
	{
	fread(x,768,1,h);
	fclose(h);
	}
	int i;
	char aux;
	for (i=0;i!=768;i++)
	 x[i]=(x[i]>>2);
	for (i=0;i!=765;i+=3)
	 {
	 aux=x[i];
	 x[i]=x[i+2];
	 x[i+2]=aux;
	 }
	novapal(0,256,x);
	}
/*-----------------------------------------------------------------------*/
void getreg(int reg,int *r,int *g,int *b)
{
	_AX=0x1015;
	_BL=reg;
	geninterrupt(0x10);
	*r=_DH;
	*g=_CH;
	*b=_CL;
}

/*-----------------------------------------------------------------------*/
/*------------------------------fim.h------------------------------------*/
/*-----------------------------------------------------------------------*/


/*----------------------------retorna pos livre--------------------------*/

int livre (void);
void colisoes (void);

void tiro (int iy,int ix,int fy,int fx)
	 {
	 int x;
	 if (x=livre())
	 all[x]=new fireen(iy,ix);
	 }

void disparar (void)
	{ int x;

switch (typefire)
{

	case 0 :
	if (x=livre())all[x]=new fire(all[0]->getpro().posy,all[0]->getpro().posx
	+  (all[0]->getpro().tamx/2)-3);
	break;

	case 1 :
	if (x=livre())all[x]=new fire(all[0]->getpro().posy,all[0]->getpro().posx
	+  (all[0]->getpro().tamx/2)-3);
	if (x=livre())all[x]=new fire(all[0]->getpro().posy+3,all[0]->getpro().posx
	+  (all[0]->getpro().tamx/2)-3+10);
	if (x=livre())all[x]=new fire(all[0]->getpro().posy+3,all[0]->getpro().posx
	+  (all[0]->getpro().tamx/2)-3-10);
	break;
}

	}


/*----------------------------scroling-----------------------------------*/


/*void fundo (int fr)
{
 int i,x;
 for (i=0;i!=200;i++)
	{
	for (x=0;x!=320;x++)
		if (!fr)
		ecra[i][x]=cor-i;
		else
		ecra[i][x]=cor;

	}
}

*/



int main (void)
{


 /*char *Fichier="c:demodemo.mod";    /* Nom du fichier … jouer */
 /*int port=220;
 char irq=5;
 char dma=1;
 int dev=THE_BEST;*/

 //USEGUS();
 //USESP();
 //USESB();
 //USEDAC();
 //USESPK();
 //USEADL();


 /*DETECTSND(&dev,&port,&irq,&dma); /* Cherche la meilleur cartes sonore
				    dans les variables d'environnements   */

 /*FLOADMOD(Fichier);               /* Charge le fichier MOD                */

 /*SETMOD(1800,dev,port,irq,dma);   /* Pr‚pare le MOD … 18kHz avec la carte
				    son trouv‚                            */

 /*STARTMOD();                      /* Commence la musique                  */



fundo fu(2);
int fpx=1,fps=1,s[2],v;
struct  time t;
gettime(&t);

setvideomode (0x13);
novpal();
short int x;
char tecla;
for (x=0;x!=maxsprit;x++) // vector a nada
     all[x] = new nada;

all[0] = new nave(150,150); // pos[0] nossa nave

while (tecla != 's')
	{


	while (!kbhit())
		{


		/*limparmatriz(); //limpar o ecra virtual*/



		fu.actf();

		colisoes();

		for (x=0;x!=maxsprit;x++) //por os sprites no ecra virtual
		     if (all[x]->movdisp()== 6) {delete all[x];all[x]= new nada;}

		/*if (state)
			{
			FM_TransposeMusic(DEFAULT);
			FM_SetDriverClockRate(music);
			FM_SetInstrumentTable(music);
			FM_PlayMusic(music);
			}*/

		if (all[0]->getpro().id==0)
			{typefire=0;delete all[0]; all[0]=new nave (150,150);}

		do{                //Respeitar o retra;amento vertical
		v=inportb(0x3DA);
		}while (v & 0x8);

		do {
		v=inportb(0x3DA);
		}while (!(v & 0x8));


		pormatriz();   // passar do ecra virtual para o ecra verdadeiro

		gotoxy (1,23);
		printf ("FPS:%d\n",fpx);
		s[0]=t.ti_sec;
		if (s[0] != s[1])
		{
			fpx=fps;
			fps=0;
		}
		s[1]=t.ti_sec;
		gettime(&t);
		fps ++;


		}


	tecla = getch();
	switch (tecla)
		{
		case '8' : all[0]->up (3);break;
		case '5' : all[0]->down (3);break;
		case '4' : all[0]->left (3);break;
		case '6' : all[0]->right (3);break;
		case '7' : all[0]->up (2);all[0]->left (2);break;
		case '1' : all[0]->down (2);all[0]->left (2);break;
		case '9' : all[0]->up (2);all[0]->right (2);break;
		case '3' : all[0]->down (2);all[0]->right (2);break;
		case ' ' :     /* CT_Sound(TURN_OFF);
				state = 0;
				CT_SetMusicStatusByteAddress(FP_SEG(&state), FP_OFF(&state));
				CT_PlaySound(FP_SEG(music), (FP_OFF(music) + 0x1A));*/
				disparar(); break;
		case 'e' : if (x=livre())all[x]=new enim1;else;;break;
		case 'm' : if (x=livre())all[x]=new tipo1(1,100);break;
		case 'w' : if (x=livre())all[x]=new enim2(-10,-10,3);break;
		case 'r' : if (x=livre())all[x]=new enim2(100,-50,3);break;
		case '+' : fu.velo(1);break;
		case '-' : fu.velo(-1);break;

		}

	}

/* STOPMOD();                      /* Arrˆte la musique et remet le PC dans
				   son ‚tat initial			  */

/* UNLOADMOD();                    /* Rend la m‚moire utilis‚ pour charger
				   le MOD au DOS		          */


nosound(); // so por seguransa
setvideomode (0x3);
/*FM_ResetDriver();*/
}

int livre (void)
{
int i=1;
 while (i!=maxsprit-1 && all[i]->getpro().id!=0)
    i++;
if (i>=maxsprit) i=0;
	else delete all[i];
return i;
}


void colisoes (void)
	{
	int i,x,gi,gx;
	proptmp pi,px;
	for (i=0;i!=maxsprit;i++)
		for (x=i+1;x!=maxsprit;x++)
		{
		pi=all[i]->getpro();
		px=all[x]->getpro();

		if (pi.id==1 && px.id==2 ||  pi.id==2 && px.id==1)


	if (pi.tamx+pi.posx >= px.posx &&
	    pi.posx <= px.tamx+px.posx &&
	    pi.tamy+pi.posy >= px.posy &&
	    pi.posy <= px.tamy+px.posy )
			{
			 if (!all[i]->energe())
			     {
			 gi=all[i]->explo();
			 delete all[i];
			 all[i]=new explosao(pi.posy+pi.tamy/2,pi.posx+pi.tamx/2,gi);
			     }
			 if (!all[x]->energe())
			     {
			 gx=all[x]->explo();
			 delete all[x];
			 all[x]=new explosao(px.posy+px.tamy/2,px.posx+px.tamx/2,gx);
			     }
			}
		}
	}

