//====================================================================================//
//                                                                                    //
//                              Class Permutation Jumps                               //
//                                                                                    //
//====================================================================================//
//  This File:   jumps.C                    Language: C++  (xlC and CC)               //
//  Software:    PEPS 3.1                    Advisor: Brigitte.Plateau@imag.fr        //
//  Error Codes: 5200                                                                 //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 14/feb/00                        by: Paulo.Fernandes@imag.fr         //
//====================================================================================//
#include "peps.h"

     //--------------------------------------------------------------------------//
     //      It computes the jumps with the sizes 's' and the original order     //
     //--------------------------------------------------------------------------//
void jumps::fill(const aut_st & s)
{
 int left  = 1;
 int right = 1;

#ifdef _PEPS_DEBUG_
  if (dimension != s.size())
    Programming_Error("bad sized parameters", 5201)
#endif
 create(s.size());
 for (int i=0; i<dimension; i++) {
   n_left[i]                = left;
   n_right[dimension-(i+1)] = right;
   left  *= s.sz(i);
   right *= s.sz(dimension-(i+1));
   n_jump[dimension-(i+1)] = right;
 }
}
     //--------------------------------------------------------------------------//
     //       It computes the jumps with the sizes 's' and the order 'l'         //
     //--------------------------------------------------------------------------//
void jumps::fill(const aut_st & s, const aut_list & l)
{
#ifdef _PEPS_DEBUG_
  if (!(l.is_full()))
    Programming_Error("incomplete list", 5202)
  if (l.has() != s.size())
    Programming_Error("bad sized list", 5203)
  if (dimension != s.size())
    Programming_Error("bad sized parameters", 5204)
#endif
 int left  = 1;
 int right = 1;

 create(s.size());
 for (int i=0; i<dimension; i++) {               // 'l.pos[i]' is the automaton id
   n_left[l.aut(i)]                = left;       // of the automaton ranked in the
   n_right[l.aut(dimension-(i+1))] = right;      // 'i'-th position of the list 'l'
   left  *= s.sz(l.aut(i));
   right *= s.sz(l.aut(dimension-(i+1)));
   n_jump[l.aut(dimension-(i+1))] = right;
 }
}
     //--------------------------------------------------------------------------//
     //           It computes the jumps only for the automaton 'a'               //
     //                 with the sizes 's' and the order 'l'                     //
     //--------------------------------------------------------------------------//
void jumps::fill(const aut_id a, const aut_st & s, const aut_list & l)
{
#ifdef _PEPS_DEBUG_
  if (a >= dimension)
    Programming_Error("index out of range", 5205)
  if (!(l.is_full()))
    Programming_Error("incomplete list", 5206)
  if (l.has() != s.size())
    Programming_Error("bad sized list", 5207)
  if (dimension != s.size())
    Programming_Error("bad sized sizes", 5208)
#endif
 int left  = 1;
 int right = 1;

 for (int i=0; i<dimension; i++) {               // 'l.pos[i]' is the automaton id
   if (l.aut(i) == a)                            // of the automaton ranked in the
     n_left[l.aut(i)]              = left;       // 'i'-th position of the list 'l'
   if (l.aut(dimension-(i+1)) == a)
     n_right[l.aut(dimension-(i+1))] = right;
   left  *= s.sz(l.aut(i));
   right *= s.sz(l.aut(dimension-(i+1)));
   if (l.aut(dimension-(i+1)) == a)
     n_jump[l.aut(dimension-(i+1))] = right;
 }
}
     //--------------------------------------------------------------------------//
     //                      Output on 'fout' file stream                        //
     //--------------------------------------------------------------------------//
void jumps::write(ofstream & fout) const
{
 fout << dimension << "\n";
 for (int i=0; i<dimension; i++)
   fout << n_left[i] << " " << n_right[i] << " " << n_jump[i] << "\n";
}
     //--------------------------------------------------------------------------//
     //                      Input from 'fin' file stream                        //
     //--------------------------------------------------------------------------//
void jumps::read(ifstream & fin)
{
 int i;

 fin >> i;
 create(i);
 for (i=0; i<dimension; i++) {
   fin >> n_left[i];
   fin >> n_right[i];
   fin >> n_jump[i];
 }
}
     //--------------------------------------------------------------------------//
     //              Memory space used by 'this' (in memory units)               //
     //--------------------------------------------------------------------------//
int jumps::mem_use() const
{
 int answer = 4;
 answer += dimension * 12;
 return(answer);
}
