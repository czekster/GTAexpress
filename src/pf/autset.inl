//====================================================================================//
//                                                                                    //
//                              Class Automata Subset                                 //
//                                                                                    //
//====================================================================================//
//  This File:   autset_inl.h               Language: C++  (xlC and CC)               //
//  Software:    PEPS 3.1                    Advisor: Brigitte.Plateau@imag.fr        //
//  Error Code:  1100                                                                 //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 28/may/97                        by: Paulo.Fernandes@imag.fr         //
//====================================================================================//
//                                                                                    //
//     An automata subset is an array of booleans where entry 'i' is true if and only //
//  if the automaton 'i' belongs to the subset and false otherwise. The automata are  //
//  numbered from 0 to N-1.                                                           //
//                                                                                    //
//  Note: This class is used for subsets of a set of reference E. This set of         //
//  reference E can be the total set of automata of the model or a particular subset  //
//  of the total set of automata. The automata range is 0..'dimension'-1, i.e., a     //
//  subset of contiguous automata, starting by 0. The variable 'dimension' contains   //
//  the number of elements in E.                                                      //
//====================================================================================//

     //--------------------------------------------------------------------------//
     //                          Empty Constructor                               //
     //--------------------------------------------------------------------------//
inline aut_set::aut_set()
{
 dimension = 0;
}
     //--------------------------------------------------------------------------//
     //                          Sized Constructor                               //
     //--------------------------------------------------------------------------//
inline aut_set::aut_set(const int dim)
{
 dimension = dim;
 set = new bool[dimension];
}
     //--------------------------------------------------------------------------//
     //                           Copy Constructor                               //
     //--------------------------------------------------------------------------//
inline aut_set::aut_set(const aut_set & s)
{
 dimension = s.dimension;
 set = new bool[dimension];
 for (aut_id i=0; i<dimension; i++)
   set[i] = s.set[i];
}
     //--------------------------------------------------------------------------//
     //                              Destructor                                  //
     //--------------------------------------------------------------------------//
inline aut_set::~aut_set()
{
 erase();
}

     //--------------------------------------------------------------------------//
     //                           Empty Intialization                            //
     //--------------------------------------------------------------------------//
inline void aut_set::create()
{
 dimension = 0;
}
     //--------------------------------------------------------------------------//
     //                           Sized Intialization                            //
     //--------------------------------------------------------------------------//
inline void aut_set::create(const int dim)
{
 if (dimension != dim) { erase();
                         dimension = dim;
                         set = new bool[dimension]; }
}
     //--------------------------------------------------------------------------//
     //                       Copy and Sized Intialization                       //
     //--------------------------------------------------------------------------//
inline void aut_set::copy(const aut_set & s)
{
 create(s.dimension);
 for (aut_id i=0; i<dimension; i++)
   set[i] = s.set[i];
}
     //--------------------------------------------------------------------------//
     //         Copy and Sized Intialization of the complementary of 's'         //
     //--------------------------------------------------------------------------//
inline void aut_set::complement(const aut_set & s)
{
 create(s.dimension);
 for (aut_id i=0; i<dimension; i++)
   set[i] = !(s.set[i]);
}
     //--------------------------------------------------------------------------//
     //     Copy and Sized Intialization of the members of the list 'l'          //
     //--------------------------------------------------------------------------//
inline void aut_set::copy(const aut_list & l)
{
 create(l.size());                   // the restriction of the
 for (aut_id i=0; i<dimension; i++)  // 'aut_list' class prevents
   set[i] = l.has(i);                // out of range accesses
}
     //--------------------------------------------------------------------------//
     //                            Destruction                                   //
     //--------------------------------------------------------------------------//
inline void aut_set::erase()
{
 if (dimension != 0) { delete[] set;
                       dimension = 0; }
}

     //--------------------------------------------------------------------------//
     //                      Total number of automata in E                       //
     //--------------------------------------------------------------------------//
inline int aut_set::size() const
{
 return(dimension);
}
     //--------------------------------------------------------------------------//
     //                      Elements in E that are true                         //
     //--------------------------------------------------------------------------//
inline int aut_set::inside() const
{
 int in = 0;
 for (aut_id i=0; i<dimension; i++)
   if (set[i])
     in++;
 return(in);
}
     //--------------------------------------------------------------------------//
     //                      'this' is the empty subset                          //
     //--------------------------------------------------------------------------//
inline bool aut_set::is_empty() const
{
 bool answer = true;
 for (aut_id i=0; i<dimension; i++)
   if (set[i]) { answer = false;
                 break; }
 return(answer);
}
     //--------------------------------------------------------------------------//
     //                        'this' is equal to E                              //
     //--------------------------------------------------------------------------//
inline bool aut_set::is_full() const
{
 bool answer = true;
 for (aut_id i=0; i<dimension; i++)
   if (!(set[i])) { answer = false;
                    break; }
 return(answer);
}
     //--------------------------------------------------------------------------//
     //                        Cardinality of 'this'                             //
     //--------------------------------------------------------------------------//
inline int aut_set::has() const
{
 int acc = 0;
 for (aut_id i=0; i<dimension; i++)
   if (set[i]) acc++;
 return(acc);
}
     //--------------------------------------------------------------------------//
     //                   Number of automata not in the subset                   //
     //--------------------------------------------------------------------------//
inline int aut_set::has_not() const
{
 int acc = 0;
 for (aut_id i=0; i<dimension; i++)
   if (!(set[i])) acc++;
 return(acc);
}
     //--------------------------------------------------------------------------//
     //                      Automaton 'a' is in the subset                      //
     //--------------------------------------------------------------------------//
inline bool aut_set::has(const aut_id a) const
{
#ifdef _PEPS_DEBUG_
  if (a >= dimension)
    Programming_Error("index out of range", 1101)
#endif
 return(set[a]);
}
     //--------------------------------------------------------------------------//
     //                    Automaton 'a' is not in the subset                    //
     //--------------------------------------------------------------------------//
inline bool aut_set::has_not(const aut_id a) const
{
#ifdef _PEPS_DEBUG_
  if (a >= dimension)
    Programming_Error("index out of range", 1102)
#endif
 return(!(set[a]));
}

     //--------------------------------------------------------------------------//
     //                    Includes all automata in the subset                   //
     //--------------------------------------------------------------------------//
inline void aut_set::put_in()
{
 for (aut_id i=0; i<dimension; i++)
   set[i] = true;
}
     //--------------------------------------------------------------------------//
     //                    Removes all automata from the subset                  //
     //--------------------------------------------------------------------------//
inline void aut_set::take_out()
{
 for (aut_id i=0; i<dimension; i++)
   set[i] = false;
}
     //--------------------------------------------------------------------------//
     //                   Includes automaton 'a' in the subset                   //
     //--------------------------------------------------------------------------//
inline void aut_set::put_in(const aut_id a)
{
#ifdef _PEPS_DEBUG_
  if (a >= dimension)
    Programming_Error("index out of range", 1103)
#endif
 set[a] = true;
}
     //--------------------------------------------------------------------------//
     //                   Removes automaton 'a' from the subset                  //
     //--------------------------------------------------------------------------//
inline void aut_set::take_out(const aut_id a)
{
#ifdef _PEPS_DEBUG_
  if (a >= dimension)
    Programming_Error("index out of range", 1104)
#endif
 set[a] = false;
}
     //--------------------------------------------------------------------------//
     //                Includes all automata of 's' with value 'v'               //
     //--------------------------------------------------------------------------//
inline void aut_set::put_in(const aut_st & s, const int v)
{
#ifdef _PEPS_DEBUG_
  if (s.size() != dimension)
    Programming_Error("bad put_in call", 1105)
#endif
 for (aut_id i=0; i<dimension; i++)
   set[i] = (s.val(i) == v);
}

     //--------------------------------------------------------------------------//
     //                       Union of subsets 'a' and 'b'                       //
     //--------------------------------------------------------------------------//
inline void aut_set::union_of(const aut_set & a, const aut_set & b)
{
#ifdef _PEPS_DEBUG_
  if (a.dimension != b.dimension)
    Programming_Error("bad union call", 1106)
#endif
 create(a.dimension);
 for (aut_id i=0; i<dimension; i++)
   set[i] = (a.set[i] || b.set[i]);
}
     //--------------------------------------------------------------------------//
     //                    Intersection of subsets 'a' and 'b'                   //
     //--------------------------------------------------------------------------//
inline void aut_set::intersection(const aut_set & a, const aut_set & b)
{
#ifdef _PEPS_DEBUG_
  if (a.dimension != b.dimension)
    Programming_Error("bad intersection call", 1107)
#endif
 create(a.dimension);
 for (aut_id i=0; i<dimension; i++)
   set[i] = (a.set[i] && b.set[i]);
}
     //--------------------------------------------------------------------------//
     //                       Union of subsets 'this' and 's'                    //
     //--------------------------------------------------------------------------//
inline void aut_set::plus(const aut_set & s)
{
#ifdef _PEPS_DEBUG_
  if (dimension != s.dimension)
    Programming_Error("bad inclusion call", 1108)
#endif
 for (aut_id i=0; i<dimension; i++)
   set[i] = (set[i] || s.set[i]);
}
     //--------------------------------------------------------------------------//
     //                        Removes subset 's' from 'this'                    //
     //--------------------------------------------------------------------------//
inline void aut_set::minus(const aut_set & s)
{
#ifdef _PEPS_DEBUG_
  if (dimension != s.dimension)
    Programming_Error("bad removal call", 1109)
#endif
 for (aut_id i=0; i<dimension; i++)
   set[i] = (set[i] && !(s.set[i]));
}
