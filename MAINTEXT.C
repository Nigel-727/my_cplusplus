#define DEBUG 1
/*****************************************************************************
  MAINTEXT.C - Verselemzo rutinok a VERSLOVI.PRJ-hez
	Irta: Bakk Zoltan // nigel@valerie.inf.elte.hu
	 1998. novembereben - INF.RSZ beadando - deadline: NOV30.
	  Forditoprg.: Borland C++ v3.1
*****************************************************************************/
#include <assert.h>
#include <stdlib.h> // malloc()
#include <ctype.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include "maintype.h"
#include "maintext.h"

static void sylldemo(void); // a szotagolast mutatja be. (Csak teszteleshez)

/****************************************************************************/
Bool isvocal(char a)
{
  return strchr(HUNVOCALS,a)!=NULL;
}
/*--------------------------------------------------------------------------*/
Bool isconson(char a)
{
 return strchr(CONSONANTS,a)!=NULL;
}
/*--------------------------------------------------------------------------*/
Bool isshortvoc(char a)
{
 return strchr(HUNSHVOCALS,a)!=NULL; // ha koztuk van az (a)
}
/*--------------------------------------------------------------------------*/
Bool is2jegyu(const char *a)
{ // megj.: (*a) nem biztos, hogy '\0'-ra v‚gz“dik!!!
 static const char *kj[] = {"cs","dz","gy","ly","ny","sz","ty","zs"};
 static const int nkj = sizeof kj / sizeof kj[0];
 int i;
 //
  for (i=0; i<nkj && strncmp(a,kj[i],2)!=0; ++i) ;
  return i<nkj;
}
/*--------------------------------------------------------------------------*/
Bool ish2jegyu(const char *a) // hosszu ketjegyu
{
 static const char *kj[] = {"ccs","ddz","ggy","lly","nny","ssz","tty","zzs"};
 static const int nkj = sizeof kj / sizeof kj[0];
 int i;
 //
  for (i=0; i<nkj && strncmp(a,kj[i],3)!=0; ++i) ;
  return i<nkj ? TRUE : FALSE;
}
/*--------------------------------------------------------------------------*/
Bool is3jegyu(const char *a) // haromjegyu
{
 return !strncmp(a,"dzs",3);
}
/*--------------------------------------------------------------------------*/
Bool isshortsyl(const char *a) // rovid-e a szotag
{
 return isshortvoc(*(a+strlen(a)-1));
}
/*--------------------------------------------------------------------------*/
int kindofrim(const char *a,const char *b)
// megj.: feltetelezzuk, hogy nyelvtanilag helyes,magyar szotagok vannak a bemeneten
// amelyekben a vokalisok legalabbis azonosak
{
 if (strcmp(a,b)==0) return TISZTA; // szotagok azonosak=> tisztarim

 return SPASSZ; // spanyol: egyedul a vokalisok egyeztek meg
}
/*--------------------------------------------------------------------------*/
/* Rokon konszonans-e a ket massalhangzo? (konszonans: massalhangzo) */
Bool isrokkon(const char *a,const char *b)
{
 // parkereses modja:
 // megnezzuk melyik van elobb az abc-ben. ezt keressuk meg a tombok
 // sorainak elso elemei kozott. ha megvan, tovabb lehet keresni a sorban
 //
 const char *temp;

  if (!(isalpha(*a) && isalpha(*b))) return FALSE; // ha vmelyik nem is betu

  if (strcmp(a,b)>0)  // p1<=p2 -nek kell teljesulnie
	 temp=a,a=b,b=temp;

  if (!(strcmp(a,"b")==0 && strcmp(b,"p")==0))
	 if (!(strcmp(a,"c")==0 && (strcmp(b,"sz")==0 || strcmp(b,"z")==0)))
		if (!(strcmp(a,"cs")==0 && strcmp(b,"dzs")==0))
		if (!(strcmp(a,"sz")==0 && strcmp(b,"z")==0))
		  if (!(strcmp(a,"d")==0 && strcmp(b,"t")==0))
			 if (!(strcmp(a,"f")==0 && strcmp(b,"v")==0))
				if (!(strcmp(a,"g")==0 && strcmp(b,"k")==0))
				  if (!(strcmp(a,"gy")==0 && strcmp (b,"ty")==0))
					 if (!(strcmp(a,"l")==0 && strcmp (b,"r")==0))
						if (!(strcmp(a,"n")==0 && strcmp(b,"ny")==0))
						  if (!(strcmp(a,"s")==0 && strcmp(b,"zs")==0))
							 return FALSE;
  return TRUE; // igen, rokon massalhangzokrol van szo
}
/*--------------------------------------------------------------------------*/
int strcmpHUN(const char *a,const char *b)
{
 static const char *abc[]=
  {"a"," ","b","c","cs","d","dz","dzs","e","‚","f","g","gy","h","i","¡","j",
	"k","l","ly","m","n","ny","o","¢","”","‹","p","q","r","s","sz","t","ty",
	"u","£","","û","v","w","x","y","z","zs"};
 static const int nabc = sizeof abc / sizeof abc[0];
 int i,hola,holb;
 int kell;
 // ketjegyu msh: ny,gy,ty,dz,dzs,cs,sz,zs
 // hosszu(kettozott msh): meggy,pottyan,dinnye,gally,kell,fenn - stb.
 // !!!: j=ly a magyar rimek elemzesenel
  if (strcmp(a,b)==0) return 0; // ha egyenloek voltak

  hola=holb=nabc; kell=2;
  for (i=0;kell && i<nabc;++i) {
	  if (strcmp(a,abc[i])==0) hola=i,--kell;
	  if (strcmp(b,abc[i])==0) holb=i,--kell;
  }
  if (hola==nabc || holb==nabc) return strcmp(a,b); // normal hasonlitas

  return hola>holb ? 1 : -1; // 1: a>b -1: a<b
}
/*--------------------------------------------------------------------------*/
/* kisbetusse is alakit, magyar ekezetes nagybetuket is atalakitja */
char *strcpylHUN(char *s1,const char *s2)
{ // a magyar ekezetes betuk nagybetus megfeleloi // ASCII kodjai
 static const char spec[][2]=
	{{' ','µ'},//143
	 {'‚',''},//144
	 {'¡','Ö'},//141
	 {'¢','à'},//   /* hosszu o */
	 {'”','™'},//   /* rovid oe */
	 {'‹','Š'},//   /* hosszu oe */
	 {'£','é'},//   /* hosszu u */
	 {'','š'},//   /* rovid ue */
	 {'û','ë'}//    /* hosszu ue */
	};
 static const int nspec=sizeof spec / sizeof spec[0];
 int i,j;
  // mindegyik karakterre megvizsgaljuk, a nagyok kozott van-e - ha igen
  // a kisbetus megfelelojet masoljuk az uj sztringbe - ha nem, akkor az
  // eredetit, sima tolower()-rel
  j=0;
  while (*s2) { // vegigmegyunk az eredeti sztringen
	 // minden egyes karakterere megnezzuk, ekezetes nagybetu-e
	 for (i=0;i<nspec && spec[i][1]!=*s2;++i) ;
	 if (i<nspec) s1[j++]=spec[i][0];
		else s1[j++]=tolower(*s2);
	 ++s2;
  }
  s1[j]='\0';
  return s1; // s1-et nem szabad piszkalni
}
/*--------------------------------------------------------------------------*
char *strrev(char *a)
{
 int i,j;
 char temp;
 //
  for (i=0,j=strlen(a)-1;i<j;++i,--j)
	 temp=a[i], a[i]=a[j], a[j]=temp;
  return a;
}
*--------------------------------------------------------------------------*/
/*
void main()
{
 char a[4],p1[4],b[4],p2[4],tmp[80];
 Bool term=FALSE;
 //
  clrscr();
 punctfil(tmp,"alma,fa,polip!papagaj-csiga?shift");
  do {
	 printf("K‚rem a k‚t m ssalhangz¢t! : ");
	 scanf("%s%s",p1,p2);
	 // Ha nem ervenyes magyar betukrol van szo:
	 if (!isvocal(*p1) && !isconson(*p1) || !isvocal(*p2) && !isconson(*p2)) {
		printf("\nNem ‚rv‚nyes bet–t adt l meg!\n\n");
		continue;
	 }

	 strcpylHUN(a,p1); strcpylHUN(b,p2);

	 printf("\n\"%s\" ‚s \"%s\" rokonok? %d (1=IGEN 0=NEM)",a,b,isrokkon(a,b));
	 printf("\nMelyik a nagyobb? %d // 1: \"%s\"; -1: \"%s\"; 0: egyenlok)",strcmpHUN(a,b),a,b);
	 printf("\n\"%s\" mgh., msh.? %s,%s",a,BOOLSTR(isvocal(a[0])),BOOLSTR(isconson(a[0])));
	 if (strncmp(a,"dzs",3)==0) printf("\nAz egyetlen magyar 3 jegy– m ssalhangz¢t ¡rtad be!");
	 else
		printf("\n\"%s\" hossz£ 2jegy–, egyszer– 2jegy–? %s,%s",a,BOOLSTR(ish2jegyu(a)),BOOLSTR(is2jegyu(a)));

	 sylldemo();

	 printf("\n-- ESC --\n\n");
	 term=getch()==27;
  } while (!term);

}
*/

/*--------------------------------------------------------------------------*/
/*
void sylldemo()
{
 const int nszotag=80; // ennel tobb szotag biztos nem lesz egy sorban
 char sztag[nszotag], // enn‚l hosszabb sz¢tag nemigen lesz :-)
		sor[nszotag], *psyll,
// Benne taroljuk el a megtalalt szotagokat - dinamikusan:
	**pp_szotag=(char**)malloc(nszotag*sizeof(char*));
 int szotagi; // szotagokat szamoljuk benne
 //
  assert(pp_szotag);

#if DEBUG<2
  printf("K‚rem a sz¢tagolnival¢t: "); gets(sor);
  printf("Magyar kisbet–kkel: %s\n",strcpylHUN(sor,sor));
#elif DEBUG==2
  printf("Az eredeti sor:\n%s\n",strcpylHUN(sor,"Van h˜ komonDora, van b§ karIksa -"));
#endif
  if ((psyll=syllab(sor))!=NULL) {
	 strcpy(sztag,psyll);
	 szotagi=0;
	 while (psyll!=NULL) {
		assert(
		  pp_szotag[szotagi]=(char*)malloc(sizeof(char)*strlen(sztag)+1)
		);
		strcpy(pp_szotag[szotagi],sztag),++szotagi;
		if ((psyll=syllab(NULL))!=NULL) strcpy(sztag,psyll);
	 }
	 // Most jon a kiiras, helyes sorrendben:
	 printf("Sz¢tagolva: ");
	 while (--szotagi>=0) {
		printf("%s%s",pp_szotag[szotagi],szotagi?",":".");
		free(pp_szotag[szotagi]);
	 }
  }
  else
	 printf("Nem tartalmaz ‚rv‚nyes sz¢tagot!\n");

  free(pp_szotag);
}
*/

/*--------------------------------------------------------------------------*/
/* A szotagolo fv. Vigyazat! Nem ert magyarul. Igy pl. vezess->ve-zess, de:
  felall->fe-lall. Aki tudja, csinalja meg jobban! :-)
*/
char *syllab(const char *sor)
{
 static char	 *psor; // allokalunk egy teruletet, ahol a (sor)-t taroljuk
 static int  	 lensor; // taroljuk a meretet is, hogy ne kelljen mindig szamolgatni
 static int  	 si; // a (sor)-on valo lepkedeshez (--) hasznalt segedvaltozo
 static char atvitel[10]; // az elozo hivaskor talalt, a kovetkezo szotaghoz tartozo msh-k
 char    regiatvitel[10]; // ide mentjuk (atvitel) tartalmat. Ezt maj a szotag legvegehez kell csatolni.
 static char  szotag[10]; // a megtalalt szotag. (static - hogy vissza lehessen adni a cimet)
 char     	  tmpcon[10]; // az egymas melletti msh-k, ha mar volt mgh
 char		  tmpend[10]; // a szotag vegi karakterek, melyek biztosan a (szotag)-hoz tartoznak
 static int     atvi; // az (atvitel) tombben valo lepkedeshez
 int   regiatvi=atvi;
 int 	   	   szi=0; // a (szotag)-on valo lepkedeshez (++). (0: mindenkeppen a szotag elejerol kell indulnunk)
 int 	          tmpi=0; // a (tmpcon) tombben valo lepkedeshez
 int       	  endi=0; // a (tmpend)-ben valo lepkedeshez
 int  sepcon[2]; // a (tmpcon) feldolgozasahoz. Elemei: {kezdopoz,hossz}. Az szotag utolso magyar betujenek adatai.
 Bool wasvoc=FALSE, // volt-e mar vokalis. (elofeltetele az ervenyes szotagnak)
	 hit=FALSE, // talalt-e ervenyes szotagot
	 isvoc,iscon, // hogy ne kelljen mindig fv-t hivni
	 atveng=TRUE, // atvitelt engedelyezzuk-e. (mert lehet,hogy a sor vege vagy egy term.kar. miatt van vege a szotagnak)
	 simple; // a tempcon tartalma egyszeru eset volt-e
 int i; // szimpla segedvaltozo, indexeleshez
 // Megjegyzes:
 //  Az atvitel, a szotag rendes sorrendben vannak tarolva.
 //  A tmpcon-ba forditott sorrendben gyujtjuk a msh-kat.

  if (!sor && !psor) return NULL; // ha meg nem inicializaltuk es NULL-val hivtuk meg

  memset(szotag,0,sizeof szotag); memset(tmpcon,0,sizeof tmpcon);
  memset(sepcon,0,sizeof sepcon); memset(regiatvitel,0,sizeof regiatvitel);

 /* Ha most inicializalunk uj sztringgel: */
  if (sor!=NULL) {
	 if (psor) free(psor); // a regi altal lefoglalt helyet felszabaditjuk
	 assert(
		psor=(char*)malloc(sizeof(char)*(lensor=strlen(sor))+1)
	 );
	 assert( psor!=NULL ); // ha ez teljesul, nem hajtja vegre (= nem lep ki)
	 strcpy(psor,sor);
	 si=lensor-1; // a vegere allunk
	 atvi=0; // meg nincs atvitel
  }

  if (0<atvi) { // eloszor is bemasoljuk az elozo szotagtol orokolt msh-kat
	 memcpy(regiatvitel,atvitel,atvi);
	 atvi=0;
  }

  /*** Indulhat a feldolgozas: ***/

  while (0<=si) { // amig nem leptunk ki a (sor)-bol
    isvoc=isvocal(psor[si]); iscon=isconson(psor[si]);
    if (!isvoc && !iscon) { // terminalo karaktert talaltunk...
	    --si; // legalabbis tovabblepunk
	    if (wasvoc) break; // mikor mar volt maganhangzo is
    }
    else if (isvoc) { // maganhangzot talaltunk...
	   if (wasvoc) break;                 // mikor mar volt
	   else { // mikor meg nem volt
	     tmpend[endi++]=psor[si--];
	     hit=wasvoc=TRUE;
	   }
    }
    else { // massalhangzot talaltunk...
      if (!wasvoc) // ha meg nem olvastunk elotte maganhangzot...
	tmpend[endi++]=psor[si--]; // akkor csak siman masoljuk at
		else { // ha mar volt vokalis, akkor osszegyujtjuk oket:
	tmpi=0;
	do
	  tmpcon[tmpi++]=psor[si--];
	while (0<=si && (iscon=isconson(psor[si])));
	if (!iscon && !isvocal(psor[si]) || si<0) // amennyiben "nembetu" vagy indexhatar atlepes szakitotta meg
	       atveng=FALSE; // akkor atvinni nem lehet, mert nincs hova
      }
    }
  } /* while (0<=si) */

  if (hit) {	// van ervenyes szotag
	 if (tmpi>0) { // massalhangzokat kell feldolgozni
		tmpcon[tmpi]='\0'; strrev(tmpcon); // !!!: rendes sorrendbe forditjuk
		if (atveng==FALSE)
		  memcpy(szotag+szi,tmpcon,tmpi), szi+=tmpi;
		else {
		  simple=TRUE; // feltesszuk, hogy majd be tudjuk oket egyszeruen sorolni
		  /* Elagazas a feldolgozando massalhangzok szama szerint */
		  switch (tmpi)
		  {
			case 1:
			  szotag[szi++]=tmpcon[0]; // ez egyszeru: csak atmasoljuk
			  break;
			case 2:
			  // Ha a "cs","dz","ly","gy","ny","ty","sz","zs" betuk kozul valo:
			  if (is2jegyu(tmpcon))
				 memcpy(szotag+szi,tmpcon,tmpi), szi+=tmpi;
			  else {
				 szotag[szi++]=tmpcon[1]; // az masodikat masoljuk be
				 atvitel[atvi++]=tmpcon[0]; // az elsot atvisszuk
			  }
			  break;
			case 3:
			  // Ha a "dzs" beturol van szo, egyszeruen bemasoljuk a (szotag)-ba:
			  if (memcmp("dzs",tmpcon,tmpi)==0)
				 memcpy(szotag+szi,tmpcon,tmpi), szi+=tmpi;
			  // ha a (ccs,ddz,lly,ggy,nny,tty,ssz,zzs) hosszu 2jegyuek kozul valo
			  else if (ish2jegyu(tmpcon)) {
				 memcpy(szotag+szi,tmpcon+1,2), szi+=2;
				 memcpy(atvitel+atvi,tmpcon+1,2), atvi+=2;
			  }
			  // szs,zsz -> sz-s,z-sz
			  else if (memcmp("szs",tmpcon,tmpi)==0) {
				 szotag[szi]=tmpcon[2], ++szi;
				 memcpy(atvitel+atvi,tmpcon,2), atvi+=2;
			  }
			  else if (memcmp("zsz",tmpcon,tmpi)==0) {
				 memcpy(szotag+szi,tmpcon+1,2), szi+=2;
				 atvitel[atvi]=tmpcon[0], ++atvi;
			  }
			  else simple=FALSE; // ha meg nem tudta oket sehova sem besorolni
			  break;
			default: // ha meg tobb karakter volt tmpcon-ban
			  simple=FALSE;
			  break;
		  } /* switch (tmpi) {} */
		  if (!simple) { // ha eddig nem tudtuk a msh-kat feldolgozni
			 // akkor szet kell bontani oket magyar betukre:
			 i=0;
			 // az i-vel szamoljuk hany db ervenyes magyar betut talaltunk
			 while (i<tmpi) {
				// ha 3 jegyu ("dzs") vagy
				// ha hosszu 2jegyu ("ssz","lly","ggy",stb):
				if (memcmp(tmpcon+i,"dzs",3)==0 || ish2jegyu(tmpcon+i))
				  sepcon[0]=i, sepcon[1]=3, i+=3;
				// ha sima 2jegyu ("gy","ty","zs",stb):
				else if (is2jegyu(tmpcon+i))
				  sepcon[0]=i, sepcon[1]=2, i+=2;
				// 1jegyu, egyebkent:
				else
				  sepcon[0]=i, sepcon[1]=1, ++i;
			 } /* while i<tmpi */
			 // Most pedig szet kell oszatni a felismert betuket
			 // a (szotag) es az (atvitel) kozott -
			 // csak az utolso kerulhet a (szotag)-ba, a tobbi atvitel lesz:
			 memcpy(szotag+szi,tmpcon+sepcon[0],sepcon[1]), szi+=sepcon[1];
			 memcpy(atvitel+atvi,tmpcon,sepcon[0]), atvi+=sepcon[0];
		  } /* if (!simple) */
		} /* ha lehetseges az atvitel */
	 } /* ha volt massalhangzo a tmpcon tombben */
	 tmpend[endi]='\0'; // az strrev() miatt
	 memcpy(szotag+szi,strrev(tmpend),endi), szi+=endi;
	 memcpy(szotag+szi,regiatvitel,regiatvi), szi+=regiatvi;
	 szotag[szi]='\0';
  } /* if (hit == TRUE) // volt ervenyes szotag */
//  if (!hit) free(psor); // felszabad¡tjuk az eredeti (sor) hely‚t
  return hit ? szotag : NULL;
}
/*--------------------------------------------------------------------------*/
int strtok1(const char *s1,const char *s2)
{
 static char *sor; // e mutatot nem mozgatjuk
 static size_t ps; // hol tartottunk ezelott az s1-ben
 char *holtalalt; // hanyadik pozicio a sor kezdetetol (0-tol szamozva)
 //
  if (s1!=NULL) {
	 assert(
		sor=(char*)calloc(strlen(s1)+1,sizeof(char))
	 );
	 strcpy(sor,s1);
	 ps=0; // az elejere allunk
  } else if (sor==NULL) return 0; // ha NULL-val hivtuk es a (sor) is nulla volt
  // keressuk meg s2 karaktereinek elso elordulasat s1-ben
  if (!(holtalalt=strpbrk(sor+ps,s2))) { //ha mar nem talalunk benne ujabbat
	 free(sor); // akkor szabaditsuk fel a hely‚t!
	 return 0;
  }
  return ps=holtalalt-sor+1;
}
/*--------------------------------------------------------------------------*/
int strlenHUN(const char *szotag)
{
 int len=0;
 int i;
 //
  for (i=0;i<strlen(szotag);) {
    if (isvocal(szotag[i])) ++len, ++i;
    else
		if (isconson(szotag[i])) {
		  if (is2jegyu(szotag+i)) ++len, i+=2;
		  else if (ish2jegyu(szotag+i) && is3jegyu(szotag+i)) ++len, i+=3;
		  else ++len, ++i;
		}
		else return 0; // nem is betû!
  }
  return len;
}
/*--------------------------------------------------------------------------*/
int strfilt(char *cel,const char *forras,const char *marks)
{
 char *masolat, // mivel a (forras)-t nem valtoztathatjuk meg
      *uj, // amit a (cel)-ba kell majd atmasolni
      *p;
 int n;
 //
  if (!forras) return 0;

  assert(
	 uj=(char*)malloc(sizeof(char)*strlen(forras)+1)
  );
  memset(uj,'\0',strlen(forras));
  assert(
	 masolat=(char*)malloc(sizeof(char)*strlen(forras)+1)
  );
  strcpy(masolat,forras);

  p=strtok(masolat,marks);
  n=0;
  while (p) {
    ++n;
    strcat(uj,p);
    p=strtok(NULL,marks);
  }
  free(masolat);
  if (n) strcpy(cel,uj);
  free(uj);
  return n; // a torolt jelek szama
}
/*--------------------------------------------------------------------------*/
int strcmp1(const char *a,const char *b)
{
  while (*b && *a==*b) ++a,++b;
  return *b=='\0' ? 0 : *a<*b ? -1 : 1;
}
/*--------------------------------------------------------------------------*/
int strpattern(const char *str,const char *patterns[],int npatterns,
														struct T_pattern hits[],int maxhits)
{
 /* statikus valtozok */
 // Az inicializalo parameterek eltarolasahoz:
 static const char 	*oldstr, // mutato az inicializalo str -re
					**oldpatterns; // mutato az ininc. patterns -re
 static int 	oldnpatterns; // az inic. patterns elemszama
 // Az elozo felosztasra emlekezeshez:
 static struct T_pattern 	*oldhits; // az elozo talalatok tombje
 static int		oldnhits; // az elozo talalatok szama
 static int 	oldnmaxhits; // az elozo max. talalatok szama
 /* automatikus valtozok */
 int i, // az oldstr -en valo lepkedeshez
	  j; // az oldhits-tombben valo lepkedeshez
 int k; // altalanos cellal
 int nhits=0; // ahany talalat most van. Ez lesz a "return value".

  //
  /* Tortent-e konnyen felismerheto hibas parametermegadas? */
  if (str==NULL) // ha NULL-val hivtuk...
  { // ...de meg nem volt elotte meghivva:
	 if (oldstr==NULL) return 0;
	 // ...de uj mintakkal vagy uj mintaszammal hivtuk meg:
	 if (patterns!=NULL || npatterns!=0) return 0;
  }
  else // Ha most szeretnenk inicializalni...
  { // ...de egyaltalan nem adtuk meg a minta tomb jellemzoit:
	 if (patterns==NULL || npatterns==0 || hits==NULL || maxhits==0) return 0;
	 // Kulonben foglaljunk helyet egy talalattombnek:
	 assert(
		oldhits=(struct T_pattern*)calloc(maxhits,sizeof(struct T_pattern))
	 );
	 // Inicializalaskor "-1-ezzuk ki" a type adattagokat:
	 for (k=0;k<maxhits;++k) oldhits[k].type=-1;

	 oldnhits=0; // meg nem volt ervenyes felosztas
	 oldstr=str; // az inic. sztring cime
	 oldpatterns=patterns; //  a mintak tomnje
	 oldnpatterns=npatterns; // a minta tomb elemszama
  }
  /* Ha NEM VOLT - legalabbis feltuno - HIBA a parameter megadaskor: */

  // Kezdoertek beallitasa.
  // i-nek: ha van elozo felosztas, akkor kezdjunk
  // az utolso talalat kezdetetol, kulonben az oldstr elejetol.
  // j indexelje az utolso talalatot,
  // nhits kezdetben legyen az elozo talalatok(=particiok) szama.
  if (oldnhits>0) {
	 i=oldhits[oldnhits-1].pos;
	 j=nhits=oldnhits-1;
  } else i=j=nhits=0;
  // ciklus amig az oldstr sztringen belul vagyunk
  while (j>=0 && j<maxhits && i<strlen(oldstr)) {
	 // megkeressuk az ide illo mintat: k-val lepkedunk a minta tomb elemein
	 for (k=++oldhits[j].type; // az elozoleg e helyen talalt minta utan kezdjunk
					  k<oldnpatterns && strcmp1(oldstr+i,oldpatterns[k])!=0; ++k) ;
	 // ha talaltunk illeszkedo mintat:
	 if (k<oldnpatterns) {
		oldhits[j].pos=i; // melyik pozicion talaltuk
		oldhits[j].type=k; // melyik mintat
		i+=strlen(oldpatterns[k]); //az illeszkedo minta hosszaval lepunk elore
		++j; // kovetkezo talalat jon
		++nhits;
	 }
	 else // Nem talaltunk illeszkedo mintat
	 {
		oldhits[j].type=-1; // ezutan elolrol kell keresnunk az oldhits[j] -hez
		--j;
		--nhits;
		if (j>=0) i=oldhits[j].pos; // az elozo illeszkedes kezdetehez kell allni
	 } // nem talaltunk illeszkedo mintat.
  } // while (j>=0 && ...) - ciklus amig az oldstr sztringen belul vagyunk.
  /* Keveset talaltunk vagy sokat? */
  if (nhits<0) nhits=0;
  else if (nhits>maxhits) nhits=-1;
  // visszateres elott beallitjuk noldhits erteket:
  oldnhits= nhits>0 ? nhits : 0;
  /* Sokmindent nullazni kell, ha nem sikerult a felosztas */
  if (nhits<=0) {
	 oldstr=NULL; free(oldhits);
  }
  /* Ha sikerult a felosztas */
  else { // az oldhits atmasolasa a hits[]-be
	 for (k=0;k<nhits;++k)
		hits[k]=oldhits[k];
  }
  return nhits; // hany ervenyes mintara tudta felosztani az eredeti (str)-t
}
/*--------------------------------------------------------------------------*/