/* MYGENRIC.HPP // Saját generikus adatszerkezetek template-tel
*	Írta: Bakk Zoltán <nigel@valerie.inf.elte.hu>
 *		1999. május 27-től //
*  Környezet: DOS; >>>>>>>>> Fordítható: Borland C++3.1;
 */

#ifndef __MYGENRIC_HPP
#define __MYGENRIC_HPP

#include <iostream.h>
#include <stddef.h>
#include <assert.h>
#include "MAINMATH.HPP"
#include "MAINTYPE.H"

template <class T,class Pr> class MyPriorityQueueItem;
template <class T,class Pr> class MyPriorityQueue; // elődeklaráció
template <class T> class MyQueue; //
template <class T> class MyStack;
template <class T> class MyList;
template <class T> class MyListItem;

     // ------------------------ MyPriorityQueueItem -----------------------------
     template <class T,class Pr> // A prioritási sor elemtípusa
     class MyPriorityQueueItem   // privát osztály
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

  // A következő operátorok a  priority  szerinti rendezettséget segítik:
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

// --------------------------- GLOBÁLIS FÜGGVÉNY ----------------------------
template <class T,class Pr>
ostream& operator<< (ostream& os,MyPriorityQueueItem<T,Pr>& item)
{
  os << "(" << item.data << "," << item.priority << ")";
  return os;
}

// ------------------------------- MyListItem -------------------------------
template <class T>
class MyListItem // listák eleme, privát osztály
{
 friend class MyList<T>; // elég is, ha csak innen látszik:
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
class MyList   // egyirányú lista
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
     { return *(current->data); } // ElemÉrték() és ElemMódosít() egyben

   bool isEmpty() const { return bool(size==0); } // üres-e?

   // Az alábbi két fv. az üressé teszi a listát -- Üres()
   void Clear();
   MyList& operator= (int i)
    { if (i==0) Clear();
      return *this;
    }

   int  PushFront(const T&e);    // Elejére(e) - beszúr
   int  PushBack (const T& e);    // Végére (e) - beszúr
				 // u.ez lesz: MyList operator+ (const T&e)

   int     Remove();              // az aktuálisat törli; vissza: sikeresség
   size_t  Remove(const T& e);    // e értékű eleme(ke)t törli - vissza: hány ilyen volt

   size_t  hasMember(const T& e) const; // e hányszor van benne a listában?

   MyList& operator= (const MyList< T >& L); // Lista=Lista1
   int     operator==(const MyList< T >& L) const; // Lista==Lista1?

   void GoFront() { current=first; }  // Elsőre()   - áll
   void GoBack () { current=last;}    // Utolsóra() - áll

   T* operator() (); // visszaadja az aktuálisra mutatót és a következőre lép
		     // (ez állítja a current-et)

   void Print() const; // kiírja az egész listát (ostream)


//  . . . . . . . . . . . .  Kiegészítések: . . . . . . . . . . .  . . . . .

   T* Front() const
     { return first ? (first->data) : 0; }
   T& operator[] (size_t i); // ElemÉrték(), ElemMódosít() - az i. listaelemre
			     // Kell még: i. elemet töröld ki!

   // Az alábbiak növekvőbe rendezett listára vonatkoznak:
   int  InsertBehind(const T&); // nagyság szerint, az elejéről indulva, a legutolsó e értékű elem mögé beszúr

// még nincs kész:
   void Sort() {return;}   // T-nek kell legyenek <,== operátorai
   int  InsertBefore() {return 0;} //

//  . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
};

// :::::::::::::::::::::::::: GLOBÁLIS FV-EK ::::::::::::::::::::::::::::::::
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
  MyListItem<T> *pL = L.first; //lehet, mert egyazon osztályba tartoznak
  while (pL) {
    PushBack(*(pL->data)); // már kész tagfüggvényt használ fel
    pL=pL->next;
  }
}

// :::::::::::::::::::::::::::: MyList ::::::::::::::::::::::::::::::::::::::
template <class T>
MyList<T>& MyList<T>::operator= (const MyList<T>& L)
{
#ifdef DEBUG
  cout << "\n\tMyList<T>& MyList<T>::operator= (const MyList<T>& L) hívása\n";
#endif
  if (&L != this) { // már létező objektumok esetén ezt a hibát sem lehet kizárni
    Clear(); // jelen objektum adatainak törlése
    MyListItem<T> *pL = L.first; //lehet, mert egyazon osztályba tartoznak
    while (pL) {
      PushBack(*(pL->data)); // már kész tagfüggvényt használ fel
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
  size_t db=0;               // ahányat törlünk, lesz a fv.érték
  MyListItem<T> *p = first;
  while (p && *(p->data)==e) { // amíg az elején van a törlendő érték
    MyListItem <T> *tmp=p->next;
    if (p==last) last=0; // ha az utolsót is kitöröltük volna
    if (current==last) current=0;
    delete p; --size; ++db;
    p=first=tmp;
  }
  // mostmár más értékre mutat a p:
  if (p) {
    MyListItem<T> *prev=p;
    for (p=p->next;p;p=p->next) {
      if (*(p->data) == e) {
	while (*(p->data) == e) {
	  MyListItem<T> *tmp=p;
	  p=p->next;
	  if (tmp==last) last=prev;  // ha az utolsót is töröltük volna
	  if (tmp==current) current=prev;
	  delete tmp; --size; ++db;
	}
	prev->next=p; // az első, már nem e értékű elem
      } // if...
      prev=p;
    } // for...
  }
  return db; // hányat töröltünk
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
  if (current==first)  // első törlése
    first=tmp;
  if (!tmp) // utolsó törlése
    tmp=last=first;
  delete current;  --size;
  current=tmp;
  return SUCCESS;
}

// :::::::::::::::::::::::::::: MyList ::::::::::::::::::::::::::::::::::::::
template <class T>
T& MyList<T>::operator[] (size_t i)
{
  if ( i>size ) i=size; // sajnos NULL nem lehet lvalue, ezért vmit mindenképp vissza kell adni
  MyListItem<T> *curr=first;
  for (size_t ii=0; ii<i ; ++ii || curr, curr=curr->next ) ;
  assert( curr ); // ha minden adat O.K. akkor nem is kell ez az ellenőrzés
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
  return (!p && !pL); // ha mind2-nek egyszerre lett vége, akkor voltak egyenlőek
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
  void   Clear()            { Lista.Clear(); }          // Üres()
  bool   isEmpty()    const { return Lista.isEmpty(); } // Üres?()
  bool   isFull()     const // Tele-e()
    { if (max_size) // ha van korlát
	if (Lista.GetSize() >= max_size) return true; // megtelt
      return false;
    }

  int Push(const T&); // Verembe()
  int Pop(T&);        // Veremből()

   T* Top() const         // Tető()
    { return Lista.Front(); }

  int operator==     (const MyStack<T>& Q) const  // Azonos?()
    { return Lista==Q.Lista;  } // a max_size értékektől eltekintünk

  MyStack& operator= (const MyStack<T>&); // Verem = Verem1

  void Print() const // Ki: Verem -- megj: "Érdekes", hogy a vermet anélkül írhatjuk ki, hogy kiolvasnánk
    { cout << *this; }

};


// :::::::::::::::::::::::::: GLOBÁLIS FV-EK ::::::::::::::::::::::::::::::::
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
  Lista.GoFront(); e=Lista.Value(); Lista.Remove(); // !!!: állítja Lista.current-et
  return SUCCESS;
}

// :::::::::::::::::::::::::::::::: MyStack :::::::::::::::::::::::::::::::::
template <class T>
MyStack<T>& MyStack<T>::operator= (const MyStack<T>& S)
{
  if ( &S != this ) {
    Lista = S.Lista;                     // Lista.operator=(MyList&) hívása
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
  size_t max_size; // a halmaz max. elemszáma
    // size tag nem kell, mert uaz. lenne mint a tagosztály (Lista) mérete
  MyList<T> Lista; // listában tároljuk a halmaz elemeit

 public: // max_size==0 esetén NINCS(!) korlát:
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
    { if (max_size) // ha van korlát
	if (Lista.GetSize() >= max_size) return true; // hiba van: MEGTELT
      return false;
    }

  bool hasMember(const T& elem) const // benne van-e a halmazban?
    {  return bool(Lista.hasMember(elem)); }
  MySet& Include  (const T& elem); // egy elemet hozzáad
				// u.ez lesz: MySet& operator+= (const T& elem)
  MySet& Exclude  (const T& elem); // egy elemet töröl
				// u.ez lesz: MySet& operator-= (const T& elem)

  MySet& operator =  (const MySet<T>&); // értékadás halmazzal
  MySet  operator +  (      MySet<T>&); // unió      (nem const, mert MyList::operator()-ral változtatgatjuk a MySet::Lista.current-et)
  MySet  operator -  (      MySet<T>&); // különbség (uaz.)
  int    operator == (const MySet<T>&); // két halmaz egyenlő-e

  MySet  operator - (const T&); // halmaz - elem
  MySet  operator + (const T&); // halmaz + elem
    // kell még   T + MySet (elem + halmaz), amit HOL VALÓSÍTANÁNK MEG?   (!)

  MySet& operator += (const T&); // halmaz+=elem
  MySet& operator -= (const T&); // halmaz-=elem

//  T* operator() (); // iterátor

  void Print() const // kiírja az egész halmazt a képernyőre
    { cout << *this; }
};

// :::::::::::::::::::::::::: GLOBÁLIS FV-EK ::::::::::::::::::::::::::::::::
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
    Lista.PushBack(elem); // sikertelenségnél 0-t kellene visszaadni!!!
  return *this;
}

// :::::::::::::::::::::::::::::::::: MySet :::::::::::::::::::::::::::::::::
template <class T>
MySet<T>& MySet<T>::Exclude(const T& elem)
{
  if ( 0<Lista.GetSize() )
    Lista.Remove(elem);
//  return Lista.Remove(elem) ? SUCCESS : FAIL; // sikeresség
  return *this;
}

// :::::::::::::::::::::::::::::::::: MySet :::::::::::::::::::::::::::::::::
template <class T>
MySet<T>& MySet<T>::operator= (const MySet<T>& H)
{
  if ( &H != this ) {
    Lista = H.Lista;                     // Lista.operator=(MyList&) hívása
    max_size =
      (!H.max_size || !max_size) ? 0 : max(H.max_size,max_size);
  }
  return *this;
}

// :::::::::::::::::::::::::::::::::: MySet :::::::::::::::::::::::::::::::::
template <class T>
MySet<T> MySet<T>::operator+ (      MySet<T>& H)
{
  MySet unio(*this); // copy-construktor hívódik meg
  unio.max_size =
      (!H.max_size || !unio.max_size) ? 0 : H.max_size+unio.max_size;
  // a MyList osztály iterátorát használjuk fel az egyes elemek eléréséhez:
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
  MySet metszet(*this); // ebből vonjuk ki azokat az elemeket amelyek H-ban is megvannak
  metszet.max_size =
    (!H.max_size || !metszet.max_size) ? 0 : max(H.max_size,metszet.max_size);
  T *p;
#ifdef DEBUG
  cout << "MySet<T> MySet<T>::operator-  (const MySet<T>& H) <<  hívása\n";
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
      return 0; // ha van ami nincs benne a másikban, akkor nem egyenlők
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
  size_t max_size; // a halmaz max. elemszáma
    // size tag nem kell, mert uaz. lenne mint a tagosztály (Lista) mérete
  MyList<T> Lista; // változatosság kedvéért itt is egy listát delegálunk

 public:
  MyQueue(size_t maxsize=0)  : max_size(maxsize), Lista() {}
  MyQueue(const MyQueue<T>& Q) : Lista(Q.Lista), max_size (Q.max_size) {} // copy-constructor
  ~MyQueue() {}

  size_t GetSize()    const { return Lista.GetSize(); }
  size_t GetMaxSize() const { return max_size; }
  void   Clear()            { Lista.Clear(); }          // Üres()
  bool   isEmpty()    const { return Lista.isEmpty(); } // Üres-e()
  bool   isFull()     const // Tele-e()
    { if (max_size) // ha van korlát
	if (Lista.GetSize() >= max_size) return true; // megtelt
      return false;
    }

   T*    Front() const         // Első() - az első elem értéke
    { return Lista.Front(); }

  virtual int operator== (const MyQueue<T>& Q) const
    { return Lista==Q.Lista;  } // a max_size értékektől eltekintünk
  virtual int Push(const T& e);    // Sorba(e) // vissza: sikeresség
				   // u.ez lesz: MyQueue::operator+ (MyQueue)
  virtual int Pop(T& e);   // Sorból(e) : e-be írja a sor első elemét és törli is a sorból// vissza: sikeresség

  void   Print() const // Ki: Sor
    { Lista.Print(); } // uez. lesz: friend ostream& operator(ostream& os, const MySet&)

  MyQueue& operator= (const MyQueue<T>& Q); // Sor = Sor1

  virtual int Remove(const T& e); // EltávolítSorból(e) // vissza: hány volt benne
};

// :::::::::::::::::::::::::::::::: MyQueue :::::::::::::::::::::::::::::::::
template <class T>
int MyQueue<T>::Push(const T& e)
{
  if ( isFull() ) return FAIL;
  return Lista.PushBack(e); // sikeresség
}

// :::::::::::::::::::::::::::::::: MyQueue :::::::::::::::::::::::::::::::::
template <class T>
int MyQueue<T>::Pop(T& e)
{
  if ( Lista.GetSize()<=0 ) return FAIL;
  Lista.GoFront(); e=Lista.Value(); Lista.Remove(); // !!!: állítja Lista.current-et
  return SUCCESS;
}

// :::::::::::::::::::::::::::::::: MyQueue :::::::::::::::::::::::::::::::::
template <class T>
MyQueue<T>& MyQueue<T>::operator= (const MyQueue<T>& Q)
{
  if ( &Q != this ) {
    Lista = Q.Lista;                     // Lista.operator=(MyList&) hívása
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
// Prioritási sor,  prioritás szerinti ábrázolással:
template <class T,class Pr>
class MyPriorityQueue : public MyQueue< MyPriorityQueueItem< T,Pr > >
{
 public:
  MyPriorityQueue(size_t maxsize=0) : MyQueue< MyPriorityQueueItem< T,Pr > >(maxsize)
    {} // kész.
  MyPriorityQueue(const MyPriorityQueue< T,Pr >& PQ) : MyQueue< MyPriorityQueueItem< T, Pr> >(PQ)
    {} // kész.
  ~MyPriorityQueue() {}

  // Azt akarom, hogy az alábbi örökölt fv-ek itt ne működjenek:
			    T*  Front()              { return 0; }
			    int Push(const T& e)     { return FAIL; }
			    int Pop (T& e)           { return FAIL; }
  // Helyettük:
//  Pr* FrontPriority() { return &(MyQueue< MyPriorityQueueItem< T,Pr > >::Front()->priority); }
//  T*  FrontValue()    { return &(MyQueue< MyPriorityQueueItem< T,Pr > >::Front()->data); }
    Pr* FrontPriority() { return (MyQueue< MyPriorityQueueItem< T,Pr > >::Front()->GetpPriority()); }
    T*  FrontValue()    { return (MyQueue< MyPriorityQueueItem< T,Pr > >::Front()->GetpData()); }
  int Push(const T& e,const Pr& pr);
  int Pop (T& e,Pr& pr);

  int operator==     (const MyPriorityQueue<T,Pr>&) const; // ezt

  int Remove(const T& e); // EltávolítPrSorból(e)

};

// :::::::::::::::::::::::::::: MyPriorityQueue :::::::::::::::::::::::::::::
template <class T,class Pr>
int MyPriorityQueue<T,Pr>::Push(const T& e,const Pr& pr)
{ // !!! sajnos itt explicit hivatkozás kell MyQueue::Lista-ra
  if (isFull()) return FAIL;
  MyPriorityQueueItem<T,Pr> uj(e,pr);
  return Lista.InsertBehind(uj); // rendezett beszúrás az utolsó azonos értékű elem mögé
}

// :::::::::::::::::::::::::::: MyPriorityQueue :::::::::::::::::::::::::::::
template <class T,class Pr>
int MyPriorityQueue<T,Pr>::Pop (T& e,Pr& pr)
{ // !!! sajnos itt explicit hivatkozás kell MyQueue::Lista-ra
  if ( Lista.GetSize()<=0 ) return FAIL;
  Lista.GoFront();
  e =Lista.Value().data;
  pr=Lista.Value().priority;
  Lista.Remove(); // !!!: állítja Lista.current-et
  return SUCCESS;
}

// :::::::::::::::::::::::::::: MyPriorityQueue :::::::::::::::::::::::::::::
template <class T,class Pr>
int MyPriorityQueue<T,Pr>::operator== (const MyPriorityQueue<T,Pr>& PQ) const
{ // !!! sajnos itt explicit hivatkozás kell MyQueue::Lista-ra
  if (this == &PQ) return 1;
  if (Lista.GetSize() != PQ.GetSize()) return 0;
  MyPriorityQueueItem<T,Pr> *p,*pPQ;
  Lista.GoFront(); PQ.Lista.GoFront(); // sajnos ezek állítják MyList::current-et
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
  return !p && !pPQ; // ha mindkettőnek egyszerre lett vége, akkor voltak egyenlőek
}

// :::::::::::::::::::::::::::: MyPriorityQueue :::::::::::::::::::::::::::::
template <class T,class Pr>
int MyPriorityQueue<T,Pr>::Remove(const T& e)
{
 // Probléma: a listaelemek közötti relációk a prioritások közötti relációkat
 // jelentik, (mivel a MyList a MyPriorityQueueItem<T,Pr>-rel lett
 // paraméterezve így nem hívható egyszerüen csak a Lista.Remove(e)
 // Helyette végig kell seperni a listán és a ->data értékeket vizsgálni csak:

  if ( Lista.GetSize()<1 ) return 0;
  size_t db=0;
  MyList< MyPriorityQueueItem<T,Pr> > UjLista;
  MyPriorityQueueItem<T,Pr> *p;
  Lista.GoFront(); // sajnos ez állítja MyList::current-et
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
