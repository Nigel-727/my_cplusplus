/* MYGENRIC.HPP // Saj�t generikus adatszerkezetek template-tel
*	�rta: Bakk Zolt�n <nigel@valerie.inf.elte.hu>
 *		1999. m�jus 27-t�l
*  K�rnyezet: DOS; >>>>>>>>> Ford�that�: Borland C++3.1;
 */

#ifndef __MYGENRIC_HPP
#define __MYGENRIC_HPP

#include <iostream.h>
#include <stddef.h>
#include <assert.h>
#include "MAINMATH.HPP"
#include "MAINTYPE.H"

template <class T,class Pr> class MyPriorityQueueItem;
template <class T,class Pr> class MyPriorityQueue; // el�deklar�ci�
template <class T> class MyQueue;
template <class T> class MyStack;
template <class T> class MyList;
template <class T> class MyListItem;

     // ------------------------ MyPriorityQueueItem -----------------------------
     template <class T,class Pr> // A priorit�si sor elemt�pusa
     class MyPriorityQueueItem   // priv�t oszt�ly
     {
       friend class MyPriorityQueue< T,Pr >;
       friend ostream& operator<< (ostream&,MyPriorityQueueItem<T,Pr>&);
     /*
       friend class MyQueue   < MyPriorityQueueItem< T,Pr > >;
       friend class MyList    < MyPriorityQueueItem< T,Pr > >;
       friend class MyListItem< MyPriorityQueueItem< T,Pr > >;
     */
      private:
	T  data;
	Pr priority;
      public:
	MyPriorityQueueItem(const T& d,const Pr& pr) : data(d), priority(pr) {}
	MyPriorityQueueItem(const MyPriorityQueueItem< T,Pr >& item) : data(item.data), priority(item.priority) {}
       ~MyPriorityQueueItem() {}

	T*  GetpData()     { return &data; }
	Pr* GetpPriority() { return &priority; }

	MyPriorityQueueItem& operator= (const MyPriorityQueueItem<T,Pr>& item)
	  { data=item.data; priority=item.priority; return *this;}

  // A k�vetkez� oper�torok a  priority  szerinti rendezetts�get seg�tik:
	int operator== (const MyPriorityQueueItem<T,Pr>& item) const
	  { return priority==item.priority; }
	int operator!= (const MyPriorityQueueItem<T,Pr>& item) const
	  { return priority!=item.priority; }
	int operator<  (const MyPriorityQueueItem<T,Pr>& item) const
	  { return priority<item.priority; }
	int operator>  (const MyPriorityQueueItem<T,Pr>& item) const
	  { return priority>item.priority; }
	int operator<= (const MyPriorityQueueItem<T,Pr>& item) const
	  { return priority<item.priority || priority==item.priority; }
	int operator>= (const MyPriorityQueueItem<T,Pr>& item) const
	  { return priority>item.priority || priority==item.priority; }
  // ...
     };

// --------------------------- GLOB�LIS F�GGV�NY ----------------------------
template <class T,class Pr>
ostream& operator<< (ostream& os,MyPriorityQueueItem<T,Pr>& item)
{
  os << "(" << item.data << "," << item.priority << ")";
  return os;
}

// ------------------------------- MyListItem -------------------------------
template <class T>
class MyListItem // list�k eleme, priv�t oszt�ly
{
 friend class MyList<T>; // el�g is, ha csak innen l�tszik:
 friend ostream& operator<< (ostream& os,const MyList<T>& ml);
 private:
   T* data;
   MyListItem *next;

   MyListItem(const T& e,MyListItem *n=0) : next(n)
     { data = new T(e); }

   ~MyListItem() { delete data; }
};

// ------------------------------- MyList -----------------------------------
template <class T>
class MyList   // egyir�ny� lista
{
 friend ostream& operator<< (ostream& os,const MyList<T>&);
 private:
   size_t size;
   MyListItem<T> *first,*last,*current;
// ELIST fail;

 public:
// enum ELIST { EhasnotMember, EOutOfMemory };
// ELIST GetFail() { return fail; }

   MyList() : size(0),first(0),last(0),current(0) {}
   MyList(const MyList<T>& L);  // copy-constructor
   ~MyList() { Clear(); }

   size_t GetSize() const { return size; }

   T& Value()
     { return *(current->data); } // Elem�rt�k() �s ElemM�dos�t() egyben

   bool isEmpty() const { return bool(size==0); } // �res-e?

   // Az al�bbi k�t fv. az �ress� teszi a list�t -- �res()
   void Clear();
   MyList& operator= (int i)
    { if (i==0) Clear();
      return *this;
    }

   int  PushFront(const T&e);    // Elej�re(e) - besz�r
   int  PushBack (const T& e);    // V�g�re (e) - besz�r
				 // u.ez lesz: MyList operator+ (const T&e)

   int     Remove();              // az aktu�lisat t�rli; vissza: sikeress�g
   size_t  Remove(const T& e);    // e �rt�k� eleme(ke)t t�rli - vissza: h�ny ilyen volt

   size_t  hasMember(const T& e) const; // e h�nyszor van benne a list�ban?

   MyList& operator= (const MyList< T >& L); // Lista=Lista1
   int     operator==(const MyList< T >& L) const; // Lista==Lista1?

   void GoFront() { current=first; }  // Els�re()   - �ll
   void GoBack () { current=last;}    // Utols�ra() - �ll

   T* operator() (); // visszaadja az aktu�lisra mutat�t �s a k�vetkez�re l�p
		     // (ez �ll�tja a current-et)

   void Print() const; // ki�rja az eg�sz list�t (ostream)


//  . . . . . . . . . . . .  Kieg�sz�t�sek: . . . . . . . . . . .  . . . . .

   T* Front() const
     { return first ? (first->data) : 0; }
   T& operator[] (size_t i); // Elem�rt�k(), ElemM�dos�t() - az i. listaelemre
			     // Kell m�g: i. elemet t�r�ld ki!

   // Az al�bbiak n�vekv�be rendezett list�ra vonatkoznak:
   int  InsertBehind(const T&); // nagys�g szerint, az elej�r�l indulva, a legutols� e �rt�k� elem m�g� besz�r

// m�g nincs k�sz:
   void Sort() {return;}   // T-nek kell legyenek <,== oper�torai
   int  InsertBefore() {return 0;} //

//  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
};

// :::::::::::::::::::::::::: GLOB�LIS FV-EK ::::::::::::::::::::::::::::::::
template <class T>
ostream& operator<< (ostream& os,const MyList<T>& ml)
{
  if (ml.size==0) {
    os << "EMPTY ";
  }
  else {
    MyListItem<T> *p = ml.first;
    for (; p; p=p->next)
      os << *(p->data) << ' ';
  }
  return os;
}

// :::::::::::::::::::::::::::: MyList ::::::::::::::::::::::::::::::::::::::
template <class T>
MyList<T>::MyList(const MyList<T>& L): size(0),first(0),last(0),current(0)
{
  MyListItem<T> *pL = L.first; //lehet, mert egyazon oszt�lyba tartoznak
  while (pL) {
    PushBack(*(pL->data)); // m�r k�sz tagf�ggv�nyt haszn�l fel
    pL=pL->next;
  }
}

// :::::::::::::::::::::::::::: MyList ::::::::::::::::::::::::::::::::::::::
template <class T>
MyList<T>& MyList<T>::operator= (const MyList<T>& L)
{
#ifdef DEBUG
  cout << "\n\tMyList<T>& MyList<T>::operator= (const MyList<T>& L) h�v�sa\n";
#endif
  if (&L != this) { // m�r l�tez� objektumok eset�n ezt a hib�t sem lehet kiz�rni
    Clear(); // jelen objektum adatainak t�rl�se
    MyListItem<T> *pL = L.first; //lehet, mert egyazon oszt�lyba tartoznak
    while (pL) {
      PushBack(*(pL->data)); // m�r k�sz tagf�ggv�nyt haszn�l fel
      pL=pL->next;
    }
  }
  return *this;
}

// :::::::::::::::::::::::::::: MyList ::::::::::::::::::::::::::::::::::::::
template <class T>
int MyList<T>::PushBack(const T& e)
{
  MyListItem<T> *uj=new MyListItem<T>(e);
  if (size==0) first=current=uj;
  else last->next = uj;
  last=uj; ++size;
  return SUCCESS;
}

// :::::::::::::::::::::::::::: MyList ::::::::::::::::::::::::::::::::::::::
template <class T>
size_t MyList<T>::Remove(const T& e)
{
  if (size<1) return 0;
  size_t db=0;               // ah�nyat t�rl�nk, lesz a fv.�rt�k
  MyListItem<T> *p = first;
  while (p && *(p->data)==e) { // am�g az elej�n van a t�rlend� �rt�k
    MyListItem <T> *tmp=p->next;
    if (p==last) last=0; // ha az utols�t is kit�r�lt�k volna
    if (current==last) current=0;
    delete p; --size; ++db;
    p=first=tmp;
  }
  // mostm�r m�s �rt�kre mutat a p:
  if (p) {
    MyListItem<T> *prev=p;
    for (p=p->next;p;p=p->next) {
      if (*(p->data) == e) {
	while (*(p->data) == e) {
	  MyListItem<T> *tmp=p;
	  p=p->next;
	  if (tmp==last) last=prev;  // ha az utols�t is t�r�lt�k volna
	  if (tmp==current) current=prev;
	  delete tmp; --size; ++db;
	}
	prev->next=p; // az els�, m�r nem e �rt�k� elem
      } // if...
      prev=p;
    } // for...
  }
  return db; // h�nyat t�r�lt�nk
}

// :::::::::::::::::::::::::::: MyList ::::::::::::::::::::::::::::::::::::::
template <class T>
size_t MyList<T>::hasMember(const T& e) const
{
  int founds=0;
  if (size==0) return 0;
  MyListItem<T> *p = first;
  for (; p; p=p->next)
     if (*(p->data)==e) ++founds;
  return founds;
}

// :::::::::::::::::::::::::::: MyList ::::::::::::::::::::::::::::::::::::::
template <class T>
void MyList<T>::Clear()
{
  if (size==0) return;
  MyListItem<T> *p=first,*tmp;
  while (p) {
    tmp=p->next;
    delete p;
    p=tmp;
  }
  size=0; first=last=current=0;
}

// :::::::::::::::::::::::::::: MyList ::::::::::::::::::::::::::::::::::::::
template <class T>
void MyList<T>::Print() const
{
  cout << *this;
}

// :::::::::::::::::::::::::::: MyList ::::::::::::::::::::::::::::::::::::::
template <class T>
T* MyList<T>::operator() ()
{
  MyListItem<T> *akt = current;
  if (!current) { current=first; return 0; }
  else current=current->next;
  return akt->data;
}

// :::::::::::::::::::::::::::: MyList ::::::::::::::::::::::::::::::::::::::
template <class T>
int MyList<T>::PushFront(const T&e)
{
  MyListItem<T> *uj=new MyListItem<T>(e);
  if (size==0) current=last=uj;
  else uj->next = first;
  first=uj; ++size;
  return SUCCESS;
}

// :::::::::::::::::::::::::::: MyList ::::::::::::::::::::::::::::::::::::::
template <class T>
int MyList<T>::Remove() // LEHET BAKI BENNE!
{
  if ( size==0 || !current ) return FAIL;
  MyListItem<T> *tmp=current->next;
  if (current==first)  // els� t�rl�se
    first=tmp;
  if (!tmp) // utols� t�rl�se
    tmp=last=first;
  delete current;  --size;
  current=tmp;
  return SUCCESS;
}

// :::::::::::::::::::::::::::: MyList ::::::::::::::::::::::::::::::::::::::
template <class T>
T& MyList<T>::operator[] (size_t i)
{
  if ( i>size ) i=size; // sajnos NULL nem lehet lvalue, ez�rt vmit mindenk�pp vissza kell adni
  MyListItem<T> *curr=first;
  for (size_t ii=0; ii<i ; ++ii || curr, curr=curr->next ) ;
  assert( curr ); // ha minden adat O.K. akkor nem is kell ez az ellen�rz�s
  return *(curr->data);
}

// :::::::::::::::::::::::::::: MyList ::::::::::::::::::::::::::::::::::::::
template <class T>
int MyList<T>::operator==(const MyList<T>& L) const
{
  if (this == &L) return 1;
  if (size != L.size) return 0;
  MyListItem<T> *p,*pL;
  for (p=first,pL=L.first; *(p->data)==*(pL->data); p=p->next, pL=pL->next) ;
  return (!p && !pL); // ha mind2-nek egyszerre lett v�ge, akkor voltak egyenl�ek
}


// :::::::::::::::::::::::::::: MyList ::::::::::::::::::::::::::::::::::::::
template <class T>
int MyList<T>::InsertBehind(const T& e) // LEHET BAKI BENNE!
{
  MyListItem<T> *p,*prev,*uj;
  for (p=first; p && e>=*(p->data) ; p=p->next ) prev=p;
  uj=new MyListItem<T>(e,p);
  if (p==first) first=uj;
  else {
    if (!p) last=uj;
    prev->next=uj;
  }
  ++size;
  return SUCCESS;
}


// -------------------------------- MyStack ---------------------------------
template <class T>
class MyStack
{
  friend ostream& operator<< (ostream& os,const MyStack<T>& ms);
 private:
  MyList<T> Lista;
  size_t max_size;
 public:
  MyStack(size_t ms=0) : max_size(ms), Lista() {}
  MyStack(const MyStack<T>& S) : Lista(S.Lista), max_size(S.max_size) {}
 ~MyStack() {}

  size_t GetSize()    const { return Lista.GetSize(); }
  size_t GetMaxSize() const { return max_size; }
  void   Clear()            { Lista.Clear(); }          // �res()
  bool   isEmpty()    const { return Lista.isEmpty(); } // �res?()
  bool   isFull()     const // Tele-e()
    { if (max_size) // ha van korl�t
	if (Lista.GetSize() >= max_size) return true; // megtelt
      return false;
    }

  int Push(const T&); // Verembe()
  int Pop(T&);        // Veremb�l()

   T* Top() const         // Tet�()
    { return Lista.Front(); }

  int operator==     (const MyStack<T>& Q) const  // Azonos?()
    { return Lista==Q.Lista;  } // a max_size �rt�kekt�l eltekint�nk

  MyStack& operator= (const MyStack<T>&); // Verem = Verem1

  void Print() const // Ki: Verem -- megj: "�rdekes", hogy a vermet an�lk�l �rhatjuk ki, hogy kiolvasn�nk
    { cout << *this; }

};


// :::::::::::::::::::::::::: GLOB�LIS FV-EK ::::::::::::::::::::::::::::::::
template <class T>
ostream& operator<< (ostream& os,const MyStack<T>& ms)
{
  cout << ms.Lista;
}

// :::::::::::::::::::::::::::::::: MyStack :::::::::::::::::::::::::::::::::
template <class T>
int MyStack<T>::Push(const T& e)
{
  if ( isFull() ) return FAIL;
  return Lista.PushFront(e);
}

// :::::::::::::::::::::::::::::::: MyStack :::::::::::::::::::::::::::::::::
template <class T>
int MyStack<T>::Pop(T& e)
{
  if ( Lista.GetSize()<=0 ) return FAIL;
  Lista.GoFront(); e=Lista.Value(); Lista.Remove(); // !!!: �ll�tja Lista.current-et
  return SUCCESS;
}

// :::::::::::::::::::::::::::::::: MyStack :::::::::::::::::::::::::::::::::
template <class T>
MyStack<T>& MyStack<T>::operator= (const MyStack<T>& S)
{
  if ( &S != this ) {
    Lista = S.Lista;                     // Lista.operator=(MyList&) h�v�sa
    max_size =
      (!S.max_size || !max_size) ? 0 : max(S.max_size,max_size);
  }
  return *this;
}

// ---------------------------- MySet ---------------------------------------
template <class T>
class MySet
{
 friend ostream& operator<< (ostream& os,const MySet<T>& ms);
 private:
  size_t max_size; // a halmaz max. elemsz�ma
    // size tag nem kell, mert uaz. lenne mint a tagoszt�ly (Lista) m�rete
  MyList<T> Lista; // list�ban t�roljuk a halmaz elemeit

 public: // max_size==0 eset�n NINCS(!) korl�t:
  MySet(size_t maxsize=0)  : max_size(maxsize), Lista() {}
  MySet(const MySet<T>& H) : Lista(H.Lista), max_size (H.max_size) {} // copy-constructor

  size_t GetMaxSize() const { return max_size;}
  size_t GetSize()    const { return Lista.GetSize(); }

  void   Clear()            { Lista.Clear(); }
  MySet& operator= (int i)
    { if (i==0) Lista.Clear();
      return *this;
    }
  bool   isEmpty()    const { return bool(Lista.GetSize()==0); }
  bool   isFull ()
    { if (max_size) // ha van korl�t
	if (Lista.GetSize() >= max_size) return true; // hiba van: MEGTELT
      return false;
    }

  bool hasMember(const T& elem) const // benne van-e a halmazban?
    {  return bool(Lista.hasMember(elem)); }
  MySet& Include  (const T& elem); // egy elemet hozz�ad
				// u.ez lesz: MySet& operator+= (const T& elem)
  MySet& Exclude  (const T& elem); // egy elemet t�r�l
				// u.ez lesz: MySet& operator-= (const T& elem)

  MySet& operator =  (const MySet<T>&); // �rt�kad�s halmazzal
  MySet  operator +  (      MySet<T>&); // uni�      (nem const, mert MyList::operator()-ral v�ltoztatgatjuk a MySet::Lista.current-et)
  MySet  operator -  (      MySet<T>&); // k�l�nbs�g (uaz.)
  int    operator == (const MySet<T>&); // k�t halmaz egyenl�-e

  MySet  operator - (const T&); // halmaz - elem
  MySet  operator + (const T&); // halmaz + elem
    // kell m�g   T + MySet (elem + halmaz), amit HOL VAL�S�TAN�NK MEG?   (!)

  MySet& operator += (const T&); // halmaz+=elem
  MySet& operator -= (const T&); // halmaz-=elem

//  T* operator() (); // iter�tor

  void Print() const // ki�rja az eg�sz halmazt a k�perny�re
    { cout << *this; }
};

// :::::::::::::::::::::::::: GLOB�LIS FV-EK ::::::::::::::::::::::::::::::::
template <class T>
ostream& operator<< (ostream& os,const MySet<T>& ms)
{
  if ( ms.Lista.GetSize()<=0 ) return os << ms.Lista; // os << "EMPTY";
  os << "{ " << ms.Lista << "}";
  return os;
}

// :::::::::::::::::::::::::::::::::: MySet :::::::::::::::::::::::::::::::::
template <class T>
MySet<T>& MySet<T>::Include(const T& elem)
{
  if ( !isFull() && !Lista.hasMember(elem) )
    Lista.PushBack(elem); // sikertelens�gn�l 0-t kellene visszaadni!!!
  return *this;
}

// :::::::::::::::::::::::::::::::::: MySet :::::::::::::::::::::::::::::::::
template <class T>
MySet<T>& MySet<T>::Exclude(const T& elem)
{
  if ( 0<Lista.GetSize() )
    Lista.Remove(elem);
//  return Lista.Remove(elem) ? SUCCESS : FAIL; // sikeress�g
  return *this;
}

// :::::::::::::::::::::::::::::::::: MySet :::::::::::::::::::::::::::::::::
template <class T>
MySet<T>& MySet<T>::operator= (const MySet<T>& H)
{
  if ( &H != this ) {
    Lista = H.Lista;                     // Lista.operator=(MyList&) h�v�sa
    max_size =
      (!H.max_size || !max_size) ? 0 : max(H.max_size,max_size);
  }
  return *this;
}

// :::::::::::::::::::::::::::::::::: MySet :::::::::::::::::::::::::::::::::
template <class T>
MySet<T> MySet<T>::operator+ (      MySet<T>& H)
{
  MySet unio(*this); // copy-construktor h�v�dik meg
  unio.max_size =
      (!H.max_size || !unio.max_size) ? 0 : H.max_size+unio.max_size;
  // a MyList oszt�ly iter�tor�t haszn�ljuk fel az egyes elemek el�r�s�hez:
  T *p;
  Lista.GoFront();
  while ( p=H.Lista() ) {
    if ( !unio.hasMember(*p) ) unio.Include(*p);
  }
  return unio;
}

// :::::::::::::::::::::::::::::::::: MySet :::::::::::::::::::::::::::::::::
template <class T>
MySet<T> MySet<T>::operator- (      MySet<T>& H)
{
  MySet metszet(*this); // ebb�l vonjuk ki azokat az elemeket amelyek H-ban is megvannak
  metszet.max_size =
    (!H.max_size || !metszet.max_size) ? 0 : max(H.max_size,metszet.max_size);
  T *p;
#ifdef DEBUG
  cout << "MySet<T> MySet<T>::operator-  (const MySet<T>& H) <<  h�v�sa\n";
#endif
  H.Lista.GoFront();
  while ( p=H.Lista() ) {
#ifdef DEBUG
  cout << *p << " ";
#endif
    if ( metszet.hasMember(*p) ) metszet.Exclude(*p);
  }
#ifdef DEBUG
  cout << endl;
#endif
  return metszet;
}

// :::::::::::::::::::::::::::::::::: MySet :::::::::::::::::::::::::::::::::
template <class T>
int MySet<T>::operator== (const MySet<T>& H)
{
  if (this == &H) return 1;
  if (Lista.GetSize()!=H.GetSize()) return 0;
  T *p;
  Lista.GoFront();
  while ( p=Lista() ) {
    if ( !H.hasMember(*p) )
      return 0; // ha van ami nincs benne a m�sikban, akkor nem egyenl�k
  }
  return 1;
}

// :::::::::::::::::::::::::::::::::: MySet :::::::::::::::::::::::::::::::::
template <class T>
MySet<T> MySet<T>::operator+ (const T& elem)
{
  MySet H(*this);
  H.Include(elem);
  return H;
}

// :::::::::::::::::::::::::::::::::: MySet :::::::::::::::::::::::::::::::::
template <class T>
MySet<T> MySet<T>::operator- (const T& elem)
{
  MySet H(*this);
  H.Exclude(elem);
  return H;
}

// :::::::::::::::::::::::::::::::::: MySet :::::::::::::::::::::::::::::::::
template <class T>
MySet<T>& MySet<T>::operator+= (const T& elem)
{
  Include(elem);
  return *this;
}

// :::::::::::::::::::::::::::::::::: MySet :::::::::::::::::::::::::::::::::
template <class T>
MySet<T>& MySet<T>::operator-= (const T& elem)
{
  Exclude(elem);
  return *this;
}

// ------------------------------- MyQueue ----------------------------------
template <class T>
class MyQueue
{
 protected:
  size_t max_size; // a halmaz max. elemsz�ma
    // size tag nem kell, mert uaz. lenne mint a tagoszt�ly (Lista) m�rete
  MyList<T> Lista; // v�ltozatoss�g kedv��rt itt is egy list�t deleg�lunk

 public:
  MyQueue(size_t maxsize=0)  : max_size(maxsize), Lista() {}
  MyQueue(const MyQueue<T>& Q) : Lista(Q.Lista), max_size (Q.max_size) {} // copy-constructor
  ~MyQueue() {}

  size_t GetSize()    const { return Lista.GetSize(); }
  size_t GetMaxSize() const { return max_size; }
  void   Clear()            { Lista.Clear(); }          // �res()
  bool   isEmpty()    const { return Lista.isEmpty(); } // �res-e()
  bool   isFull()     const // Tele-e()
    { if (max_size) // ha van korl�t
	if (Lista.GetSize() >= max_size) return true; // megtelt
      return false;
    }

   T*    Front() const         // Els�() - az els� elem �rt�ke
    { return Lista.Front(); }

  virtual int operator== (const MyQueue<T>& Q) const
    { return Lista==Q.Lista;  } // a max_size �rt�kekt�l eltekint�nk
  virtual int Push(const T& e);    // Sorba(e) // vissza: sikeress�g
				   // u.ez lesz: MyQueue::operator+ (MyQueue)
  virtual int Pop(T& e);   // Sorb�l(e) : e-be �rja a sor els� elem�t �s t�rli is a sorb�l// vissza: sikeress�g

  void   Print() const // Ki: Sor
    { Lista.Print(); } // uez. lesz: friend ostream& operator(ostream& os, const MySet&)

  MyQueue& operator= (const MyQueue<T>& Q); // Sor = Sor1

  virtual int Remove(const T& e); // Elt�vol�tSorb�l(e) // vissza: h�ny volt benne
};

// :::::::::::::::::::::::::::::::: MyQueue :::::::::::::::::::::::::::::::::
template <class T>
int MyQueue<T>::Push(const T& e)
{
  if ( isFull() ) return FAIL;
  return Lista.PushBack(e); // sikeress�g
}

// :::::::::::::::::::::::::::::::: MyQueue :::::::::::::::::::::::::::::::::
template <class T>
int MyQueue<T>::Pop(T& e)
{
  if ( Lista.GetSize()<=0 ) return FAIL;
  Lista.GoFront(); e=Lista.Value(); Lista.Remove(); // !!!: �ll�tja Lista.current-et
  return SUCCESS;
}

// :::::::::::::::::::::::::::::::: MyQueue :::::::::::::::::::::::::::::::::
template <class T>
MyQueue<T>& MyQueue<T>::operator= (const MyQueue<T>& Q)
{
  if ( &Q != this ) {
    Lista = Q.Lista;                     // Lista.operator=(MyList&) h�v�sa
    max_size =
      (!Q.max_size || !max_size) ? 0 : max(Q.max_size,max_size);
  }
  return *this;
}

// :::::::::::::::::::::::::::::::: MyQueue :::::::::::::::::::::::::::::::::
template <class T>
int MyQueue<T>::Remove(const T& e)
{
  return Lista.Remove(e);
}

// -------------------------- MyPriorityQueue -------------------------------
// Priorit�si sor,  priorit�s szerinti �br�zol�ssal:
template <class T,class Pr>
class MyPriorityQueue : public MyQueue< MyPriorityQueueItem< T,Pr > >
{
 public:
  MyPriorityQueue(size_t maxsize=0) : MyQueue< MyPriorityQueueItem< T,Pr > >(maxsize)
    {} // k�sz.
  MyPriorityQueue(const MyPriorityQueue< T,Pr >& PQ) : MyQueue< MyPriorityQueueItem< T, Pr> >(PQ)
    {} // k�sz.
  ~MyPriorityQueue() {}

  // Azt akarom, hogy az al�bbi �r�k�lt fv-ek itt ne m�k�djenek:
			    T*  Front()              { return 0; }
			    int Push(const T& e)     { return FAIL; }
			    int Pop (T& e)           { return FAIL; }
  // Helyett�k:
//  Pr* FrontPriority() { return &(MyQueue< MyPriorityQueueItem< T,Pr > >::Front()->priority); }
//  T*  FrontValue()    { return &(MyQueue< MyPriorityQueueItem< T,Pr > >::Front()->data); }
    Pr* FrontPriority() { return (MyQueue< MyPriorityQueueItem< T,Pr > >::Front()->GetpPriority()); }
    T*  FrontValue()    { return (MyQueue< MyPriorityQueueItem< T,Pr > >::Front()->GetpData()); }
  int Push(const T& e,const Pr& pr);
  int Pop (T& e,Pr& pr);

  int operator==     (const MyPriorityQueue<T,Pr>&) const; // ezt

  int Remove(const T& e); // Elt�vol�tPrSorb�l(e)

};

// :::::::::::::::::::::::::::: MyPriorityQueue :::::::::::::::::::::::::::::
template <class T,class Pr>
int MyPriorityQueue<T,Pr>::Push(const T& e,const Pr& pr)
{ // !!! sajnos itt explicit hivatkoz�s kell MyQueue::Lista-ra
  if (isFull()) return FAIL;
  MyPriorityQueueItem<T,Pr> uj(e,pr);
  return Lista.InsertBehind(uj); // rendezett besz�r�s az utols� azonos �rt�k� elem m�g�
}

// :::::::::::::::::::::::::::: MyPriorityQueue :::::::::::::::::::::::::::::
template <class T,class Pr>
int MyPriorityQueue<T,Pr>::Pop (T& e,Pr& pr)
{ // !!! sajnos itt explicit hivatkoz�s kell MyQueue::Lista-ra
  if ( Lista.GetSize()<=0 ) return FAIL;
  Lista.GoFront();
  e =Lista.Value().data;
  pr=Lista.Value().priority;
  Lista.Remove(); // !!!: �ll�tja Lista.current-et
  return SUCCESS;
}

// :::::::::::::::::::::::::::: MyPriorityQueue :::::::::::::::::::::::::::::
template <class T,class Pr>
int MyPriorityQueue<T,Pr>::operator== (const MyPriorityQueue<T,Pr>& PQ) const
{ // !!! sajnos itt explicit hivatkoz�s kell MyQueue::Lista-ra
  if (this == &PQ) return 1;
  if (Lista.GetSize() != PQ.GetSize()) return 0;
  MyPriorityQueueItem<T,Pr> *p,*pPQ;
  Lista.GoFront(); PQ.Lista.GoFront(); // sajnos ezek �ll�tj�k MyList::current-et
  p=Lista(); pPQ=PQ.Lista();
  while (p && pPQ) {
#ifdef DEBUG
    cout << "|" << p->data << "," << p->priority << "|=="
	 << "|" << pPQ->data << "," << pPQ->priority << "| ";
#endif
    if ( p->data     != pPQ->data )     return 0;
    if ( p->priority != pPQ->priority ) return 0;
    p=Lista(); pPQ=PQ.Lista();
  }
  return !p && !pPQ; // ha mindkett�nek egyszerre lett v�ge, akkor voltak egyenl�ek
}

// :::::::::::::::::::::::::::: MyPriorityQueue :::::::::::::::::::::::::::::
template <class T,class Pr>
int MyPriorityQueue<T,Pr>::Remove(const T& e)
{
 // Probl�ma: a listaelemek k�z�tti rel�ci�k a priorit�sok k�z�tti rel�ci�kat
 // jelentik, (mivel a MyList a MyPriorityQueueItem<T,Pr>-rel lett
 // param�terezve �gy nem h�vhat� egyszer�en csak a Lista.Remove(e)
 // Helyette v�gig kell seperni a list�n �s a ->data �rt�keket vizsg�lni csak:

  if ( Lista.GetSize()<1 ) return 0;
  size_t db=0;
  MyList< MyPriorityQueueItem<T,Pr> > UjLista;
  MyPriorityQueueItem<T,Pr> *p;
  Lista.GoFront(); // sajnos ez �ll�tja MyList::current-et
  p=Lista();
  while (p) {
    if ( p->data == e ) ++db;
    else UjLista.PushBack(*p);
    p=Lista();
  }
  Lista=UjLista;
  return db;
}
#endif