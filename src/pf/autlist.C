//====================================================================================//
//                                                                                    //
//                              Class Automata List                                   //
//                                                                                    //
//====================================================================================//
//  This File:   autlist.C                  Language: C++  (xlC and CC)               //
//  Software:    PEPS 3.1                    Advisor: Brigitte.Plateau@imag.fr        //
//  Error Codes: 1200                                                                 //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change:  7/feb/01                        by: Anne.Benoit@imag.fr             //
//====================================================================================//
#include "peps.h"

     //--------------------------------------------------------------------------//
     //           It fills 's' with the automata before automaton 'a'            //
     //--------------------------------------------------------------------------//
void aut_list::before(const aut_id a, aut_set & s) const
{
#ifdef _PEPS_DEBUG_
  if (s.size() != dimension)
    Programming_Error("different sizes in a before call", 1205)
  if (a >= lsize)
    Programming_Error("invalid automaton id", 1206)
#endif
  int p = pos(a);
  s.take_out();
  for (int i=0; i<p; i++)
    s.put_in(list[i]);
}
     //--------------------------------------------------------------------------//
     //            It fills 's' with the automata after automaton 'a'            //
     //--------------------------------------------------------------------------//
void aut_list::after(const aut_id a, aut_set & s) const
{
#ifdef _PEPS_DEBUG_
  if (s.size() != dimension)
    Programming_Error("different sizes in a before call", 1207)
  if (a >= lsize)
    Programming_Error("invalid automaton id", 1208)
#endif
  int p = pos(a);
  s.take_out();
  for (int i=p+1; i<lsize; i++)
    s.put_in(list[i]);
}

     //--------------------------------------------------------------------------//
     //       It sorts the list according to the increasing sizes in 's'         //
     //--------------------------------------------------------------------------//
void aut_list::sort(const aut_st & s)
{
#ifdef _PEPS_DEBUG_
  if (s.size() != dimension)
    Programming_Error("different sizes in a sort call", 1209)
#endif
 int    last, i;
 aut_id temp;
 for (last=lsize-1; last>0; last--)           // bubble sort
   for (i=0; i<last; i++)
     if (s.sz(list[i]) > s.sz(list[i+1])) {
       temp      = list[i+1];
       list[i+1] = list[i];
       list[i]   = temp;
     }
}

     //--------------------------------------------------------------------------//
     //                Exchanges the p-th and the q-th automata                  //
     //--------------------------------------------------------------------------//
void aut_list::exchange(const int q, const int p)
{
#ifdef _PEPS_DEBUG_
  if ((p >= lsize) || (q >= lsize))
    Programming_Error("index(es) out of range", 1210)
#endif
 aut_id temp;
 temp    = list[p];
 list[p] = list[q];
 list[q] = temp;
}

     //--------------------------------------------------------------------------//
     //          It puts all automata of E in the list (normal order)            //
     //--------------------------------------------------------------------------//
void aut_list::put()
{
 for (lsize=0; lsize<dimension; lsize++)     // normal order:  0, 1, 2, ...
   list[lsize] = lsize;
}
     //--------------------------------------------------------------------------//
     //            It puts all automata of E in the list ('t' order)              //
     //--------------------------------------------------------------------------//
void aut_list::put(const order_types t)
{
 for (lsize=0; lsize<dimension; lsize++)
   if (t == Normal)
     list[lsize] = lsize;                 // normal order:  0, 1, 2, ...
   else
     list[lsize] = dimension - (lsize+1); // reverse order: 9, 8, 7, ...
 //list[0] = 2;
 //list[1] = 0;
 //list[2] = 3;
 //list[3] = 1;
}
     //--------------------------------------------------------------------------//
     //    It puts all automata of E in the list (increasing sizes in 's')       //
     //--------------------------------------------------------------------------//
void aut_list::put(const aut_st & s)
{
#ifdef _PEPS_DEBUG_
  if (s.size() != dimension)
    Programming_Error("different sizes in a put call", 1211)
#endif
 aut_set to_put(dimension);    // the automata to be included
 aut_id  a;                    // a counter to the automata
 aut_id  smallest;             // the automaton that will be included
 int     size;                 // the size of the automaton that will be included

 to_put.put_in();                              // puts all automata in 'to_put'
 for (lsize=0; lsize<dimension; lsize++) {     // to all list positions
   size = max_integer;
   for (a=0; a<dimension; a++)                 // choose the smallest automaton
     if (to_put.has(a))                        // among the automata not yet included
       if (s.sz(a) < size) {
         size     = s.sz(a);
         smallest = a;
       }
   list[lsize] = smallest;                     // included the smallest automaton
   to_put.take_out(smallest);                  // and take it out of 'to_put'
 }
}

     //--------------------------------------------------------------------------//
     //      It puts automata of the subset 's' in the list (normal order)       //
     //--------------------------------------------------------------------------//
void aut_list::put(const aut_set & s)
{
 create(s.size());
 for (aut_id a=0; a<dimension; a++)                // normal order:  0, 1, 2, ...
   if (s.has(a)) {
     list[lsize] = a;
     lsize++;
   }
}
     //--------------------------------------------------------------------------//
     //        It puts automata of the subset 's' in the list (any order)        //
     //--------------------------------------------------------------------------//
void aut_list::put(const aut_set & s, const order_types t)
{
 aut_id a;

 create(s.size());
 if (t == Normal)                                  // normal order:  0, 1, 2, ...
   for (a=0; a<dimension; a++)
     if (s.has(a)) {
       list[lsize] = a;
       lsize++;
     }
 else                                              // reverse order: 9, 8, 7, ...
   for (a=dimension-1; a>=0; a--)
     if (s.has(a)) {
       list[lsize] = a;
       lsize++;
     }
}
     //--------------------------------------------------------------------------//
     // It puts automata of the subset 's' in the list (increasing sizes in 's') //
     //--------------------------------------------------------------------------//
void aut_list::put(const aut_set & s, const aut_st & o)
{
#ifdef _PEPS_DEBUG_
  if (o.size() != s.size())
    Programming_Error("different sizes in a put call", 1212)
#endif
 aut_set to_put(dimension);    // the automata to be included
 aut_id  a;                    // a counter to the automata
 aut_id  smallest;             // the automaton that will be included
 int     size;                 // the size of the automaton that will be included

 create(s.size());                          // allocates the list
 to_put.copy(s);                            // puts all automata of 's' in 'to_put'
 for (lsize=0; lsize<dimension; lsize++) {  // to all list positions
   size = max_integer;
   for (a=0; a<dimension; a++)              // choose the smallest automaton
     if (to_put.has(a))                     // among the automata not yet included
       if (o.sz(a) < size) {
         size     = o.sz(a);
         smallest = a;
       }
   list[lsize] = smallest;                  // included the smallest automaton
   to_put.take_out(smallest);               // and take it out of 'to_put'
 }
}

     //--------------------------------------------------------------------------//
     //             It puts automaton 'a' at the head of the list                //
     //--------------------------------------------------------------------------//
void aut_list::ins_head(const aut_id a)
{
#ifdef _PEPS_DEBUG_
  if (a >= dimension)
    Programming_Error("invalid automaton id", 1213)
  if (has(a))
    Programming_Error("replicated automaton id", 1214)
#endif
 for (int i=lsize; i>0; i--)          // relocates the positions '0 .. lsize-1'
   list[i] = list[i-1];               // to the postions '1 .. lsize'
 list[0] = a;                         // puts automaton 'a' in the first position ('0')
 lsize++;
}
     //--------------------------------------------------------------------------//
     //             It puts automaton 'a' at the tail of the list                //
     //--------------------------------------------------------------------------//
void aut_list::ins_tail(const aut_id a)
{
#ifdef _PEPS_DEBUG_
  if (a >= dimension)
    Programming_Error("invalid automaton id", 1215)
  if (has(a))
    Programming_Error("replicated automaton id", 1216)
#endif
 list[lsize] = a;                // puts automaton 'a' in the last position ('lsize')
 lsize++;
}
     //--------------------------------------------------------------------------//
     //          It puts automaton 'a' at the p-th position of the list          //
     //--------------------------------------------------------------------------//
void aut_list::insert(const int p, const aut_id a)
{
#ifdef _PEPS_DEBUG_
  if (a >= dimension)
    Programming_Error("invalid automaton id", 1217)
  if (has(a))
    Programming_Error("replicated automaton id", 1218)
  if (p > lsize)
    Programming_Error("index out of range", 1219)
#endif
 for (int i=lsize; i>p; i--)        // relocates the positions 'p .. lsize-1'
   list[i] = list[i-1];             // to the postions 'p+1 .. lsize'
 list[p] = a;                       // puts automaton 'a' in the 'p'-th position
 lsize++;
}

     //--------------------------------------------------------------------------//
     //        It puts all automata of E not yet in the list at the head         //
     //--------------------------------------------------------------------------//
void aut_list::fill_head()
{
 aut_set aut_in,    // subset of automata already in the list
         to_put;    // subset of automata not yet in the list

 aut_in.copy(*this);                  // copies the list elements to a set object
 to_put.complement(aut_in);           // computes the automata not yet in the list
 for (int i=1; i<=lsize; i++)         // relocates the positions '0 .. lsize-1' to the
   list[dimension-i] = list[lsize-i]; // positions 'dimension-lsize-1 .. dimension-1'
 lsize = 0;
 for (aut_id a=0; a<dimension; a++)   // to all automata not yet in the list
   if (to_put.has(a)) {
     list[lsize] = a;                 // puts it from the first position ('0')
     lsize++;
   }
 lsize = dimension;
}
     //--------------------------------------------------------------------------//
     //        It puts all automata of E not yet in the list at the tail         //
     //--------------------------------------------------------------------------//
void aut_list::fill_tail()
{
 aut_set aut_in,    // subset of automata already in the list
         to_put;    // subset of automata not yet in the list
 
 aut_in.copy(*this);                  // copies the list elements to a set object
 to_put.complement(aut_in);           // computes the automata not yet in the list
 cut = lsize;
 for (aut_id a=0; a<dimension; a++)   // to all automata not yet in the list
   if (to_put.has(a)) {
     list[lsize] = a;                 // puts it from the last position ('lsize')
     lsize++;
   }
}
     //--------------------------------------------------------------------------//
     //       It puts the automata of the subset 's' not yet in the list         //
     //                  starting from the p-th position                         //
     //--------------------------------------------------------------------------//
void aut_list::fill(const int p, const aut_set s)
{
#ifdef _PEPS_DEBUG_
  if (s.size() != dimension)
    Programming_Error("replicated automaton id", 1220)
  if (p > lsize)
    Programming_Error("index out of range", 1221)
#endif
 aut_set aut_in,    // subset of automata already in the list
         to_put;    // subset of automata to insert in the list
 int     n;         // cardinality of 'to_put'
 int     q;         // counter for the elements to insert

 aut_in.copy(*this);                // copies the list elements to a set object
 to_put.copy(s);                    // puts all automata of 's' in 'to_put'
 to_put.minus(aut_in);              // removes the automata already in from 'to_put'
 n = to_put.has();                  // computes the cardinality of 'to_put'
 for (int i=1; i<=lsize-p; i++)     // relocates the positions 'p .. lsize-1'
   list[lsize-i+n] = list[lsize-i]; // to the positions 'lsize .. lsize+n'
 lsize += n;
 q = p;
 for (aut_id a=0; a<dimension; a++) // to all automata to insert in the list
   if (to_put.has(a)) {
     list[q] = a;                   // puts it from the 'p'-th position
     q++;
   }
}

     //--------------------------------------------------------------------------//
     //    It moves the automaton in the p-th position to the head of the list   //
     //--------------------------------------------------------------------------//
void aut_list::mov_head(const int p)
{
#ifdef _PEPS_DEBUG_
  if (p > lsize)
    Programming_Error("index out of range", 1222)
#endif
 aut_id a = list[p];           // relocates the positions '0 .. p-1'
 for (int i=p; i>0; i--)       // to the positions '1 .. p'
   list[i] = list[i-1];
 list[0] = a;                  // puts the 'p'-th automaton in the first position ('0')
}

     //-------------------------------------------------------------------------------------//
     //    It moves the constant matrices (from cut to end) to the beginning of the list    //
     //     leaving only identities to the shuffle part (only works for B_term)             //
     //-------------------------------------------------------------------------------------//
void aut_list::mov_cnst(const term_typ t, const int cut, const int e, const aut_list & l, int & r)
{
  int aut, c = 0;
  aut_list temp(l.size());
  assert(t == B_term);
  if (t == B_term) { //this will work only for B_term...
      for (int a=cut-1; a>=0; a--) { //for all automata before the cut
          aut = l.aut(a);
          temp.ins_head(aut);
          c++;
      }
      for (int a=cut; a<l.size(); a++) {
          aut = l.aut(a);
          if (CND::cnd.get_sf_type(e, aut) == cnst_mat) {
              temp.ins_tail(aut);
              c++;
          }
      }
      r = c;
  }
  temp.fill_tail();
  take_out();
  copy(temp);
}

     //--------------------------------------------------------------------------//
     //    It moves the identities to the end of the list                        //
     //--------------------------------------------------------------------------//
void aut_list::mov_idnt(const term_typ t, const int cut, const int e, const aut_list & l, int & r)
{
  int aut, c = 0;
  aut_list temp(l.size());
  assert(t == B_term || t == cnst_term);
  if (t == B_term) {
      for (int a=cut; a<l.size(); a++) { //insert functional dependable automata first
          aut = l.aut(a);
          temp.ins_tail(aut);
      }
      for (int a=cut-1; a>=0; a--) { //for all automata before the cut, insert all identities
          aut = l.aut(a);
          if (CND::cnd.get_sf_type(e, aut) == idnt_mat)
              temp.ins_tail(aut); //this will force the identities to avoid the nleft block in the shuffle part (by inserting the identities to the end of the aut_st)
      }
      r = (l.size()-temp.size());
  } else //constant term only at this point
  if (t == cnst_term) {
      for (int a=0; a<l.size(); a++) {
          aut = l.aut(a);
          if (CND::cnd.get_sf_type(e, aut) == idnt_mat)
              temp.insert(c++, aut);
      }
      r = (l.size()-c); //the new cut value
  }
  temp.fill_head();
  take_out();
  copy(temp);
}

     //--------------------------------------------------------------------------//
     //    It leaves only functions and identities to shuffle                    //
     //--------------------------------------------------------------------------//
void aut_list::mov_f_idnt(const term_typ t, const int cut, const int e, const aut_list & l, int & r)
{
  int aut, c = 0;
  aut_list temp(l.size());
  for (int a=0; a<l.size()-1; a++) { //puts all identities before that
      aut = l.aut(a);
      if (CND::cnd.get_sf_type(e, aut) == idnt_mat)
          temp.ins_tail(aut);
  }
  r = (l.size()-temp.size())+1;
  temp.ins_tail(l.aut(l.size()-1)); //puts the functional element in the last position...
  temp.fill_head(); //fill the rest of the positions
  take_out();
  copy(temp);
}
     
     //--------------------------------------------------------------------------//
     //    It moves the automaton in the p-th position to the tail of the list   //
     //--------------------------------------------------------------------------//
void aut_list::mov_tail(const int p)
{
#ifdef _PEPS_DEBUG_
  if (p > lsize)
    Programming_Error("index out of range", 1223)
#endif
 aut_id a = list[p];             // relocates the positions 'p+1 .. lsize-1'
 for (int i=p; i<lsize-1; i++)   // to the positions 'p .. lsize-2'
   list[i] = list[i+1];
 list[lsize-1] = a;        // puts the 'p'-th automaton in the last position ('lsize-1')
}
     //--------------------------------------------------------------------------//
     //    It moves the automaton in the p-th to the q-th position of the list   //
     //--------------------------------------------------------------------------//
void aut_list::move(const int p, const int q)
{
#ifdef _PEPS_DEBUG_
  if ((p >= lsize) || (q >= lsize))
    Programming_Error("index(es) out of range", 1224)
#endif
 aut_id a = list[p];
 if (p < q)                        // if it is a move forward ('p < q')
   for (int i=p; i<q; i++)         // relocates the positions 'p+1 .. q'
     list[i] = list[i+1];          // to the positions 'p .. q-1'
 else                              // if it is a move back ('p > q')
   for (int i=p; i>q; i--)         // relocates the positions 'q .. p-1'
     list[i] = list[i-1];          // to the positions 'q+1 .. p'
 list[q] = a;                      // puts the 'p'-th automaton in the 'q'-th position
}
     //--------------------------------------------------------------------------//
     //                      Output on 'fout' file stream                        //
     //--------------------------------------------------------------------------//
void aut_list::write(ofstream & fout) const
{
 fout << dimension << " " << lsize << "\n";
 for (int i=0; i<lsize; i++)
   fout << list[i] << " ";
 fout << "\n";
}
     //--------------------------------------------------------------------------//
     //                      Input from 'fin' file stream                        //
     //--------------------------------------------------------------------------//
void aut_list::read(ifstream & fin)
{
 int d;

 fin >> d;
 create(d);
 fin >> lsize;
 for (d=0; d<lsize; d++)
   fin >> list[d];
}
     //--------------------------------------------------------------------------//
     //              Memory space used by 'this' (in memory units)               //
     //--------------------------------------------------------------------------//
int aut_list::mem_use() const
{
 int answer = 8;
 answer += dimension * 4;
 return(answer);
}
