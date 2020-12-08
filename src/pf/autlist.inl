//====================================================================================//
//                                                                                    //
//                              Class Automata List                                   //
//                                                                                    //
//====================================================================================//
//  This File:   autlist_inl.h              Language: C++  (xlC and CC)               //
//  Software:    PEPS 3.1                    Advisor: Brigitte.Plateau@imag.fr        //
//  Error Codes: 1200                                                                 //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change:  7/feb/01                        by: Anne.Benoit@imag.fr             //
//====================================================================================//

     //--------------------------------------------------------------------------//
     //                          Empty Constructor                               //
     //--------------------------------------------------------------------------//
inline aut_list::aut_list()
{
 dimension = 0;
 lsize     = 0;
}
     //--------------------------------------------------------------------------//
     //                          Sized Constructor                               //
     //--------------------------------------------------------------------------//
inline aut_list::aut_list(const int dim)
{
 dimension = dim;
 lsize     = 0;
 list      = new aut_id[dimension];
}
     //--------------------------------------------------------------------------//
     //                           Copy Constructor                               //
     //--------------------------------------------------------------------------//
inline aut_list::aut_list(const aut_list & l)
{
 dimension = l.dimension;
 lsize     = l.lsize;
 list      = new aut_id[dimension];
 for (int i=0; i<dimension; i++)
   list[i] = l.list[i];
}
     //--------------------------------------------------------------------------//
     //                              Destructor                                  //
     //--------------------------------------------------------------------------//
inline aut_list::~aut_list()
{
 erase();
}

     //--------------------------------------------------------------------------//
     //                           Empty Intialization                            //
     //--------------------------------------------------------------------------//
inline void aut_list::create()
{
 dimension = 0;
 lsize     = 0;
 cut = 0;
}
     //--------------------------------------------------------------------------//
     //                           Sized Intialization                            //
     //--------------------------------------------------------------------------//
inline void aut_list::create(const int dim)
{
 if (dimension != dim) {
   erase();
   dimension = dim;
   list = new aut_id[dimension];
 }
 lsize = 0;
 cut = 0;
}
     //--------------------------------------------------------------------------//
     //                           Sized Intialization                            //
     //--------------------------------------------------------------------------//
inline void aut_list::invert(int *f, int e, aut_list & o)
{
 int idx = 0;
 for (int i = f[e]; i < dimension; i++) {
   //cout << i << ". inserindo na posicao " << (idx) << " valor " << (CND::cnd.s_B_order[e].aut(i)) << endl;
   insert(idx, o.aut(i));
   idx++;
 }
 //for (int i = f[e] - 1; i >= 0; i--) {
 for (int i = 0; i < f[e]; i++) {
   //cout << i << ". inserindo na posicao " << (idx) << " valor " << CND::cnd.s_B_order[e].aut(i) << endl;
   insert(idx, o.aut(i));
   idx++;
 }
//cout << "" << endl;
}
     //--------------------------------------------------------------------------//
     //                       Copy and Sized Intialization                       //
     //--------------------------------------------------------------------------//
inline void aut_list::copy(const aut_list & l)
{
 create(l.dimension);
 lsize = l.lsize;
 cut = l.cut;
 for (int i=0; i<dimension; i++)
   list[i] = l.list[i];
}
     //--------------------------------------------------------------------------//
     //                            Destruction                                   //
     //--------------------------------------------------------------------------//
inline void aut_list::erase()
{
 if (dimension != 0) { delete[] list;
                       dimension = 0;
                       lsize     = 0;
                       cut       = 0; }
}

     //--------------------------------------------------------------------------//
     //         Total number of automata in E (maximum list size)                //
     //--------------------------------------------------------------------------//
inline int aut_list::size() const
{
 return(dimension);
}
     //--------------------------------------------------------------------------//
     //                       No automata in the list                            //
     //--------------------------------------------------------------------------//
inline bool aut_list::is_empty() const
{
 return(lsize == 0);
}
     //--------------------------------------------------------------------------//
     //                    All automata are in the list                          //
     //--------------------------------------------------------------------------//
inline bool aut_list::is_full() const
{
 return(lsize == dimension);
}

     //--------------------------------------------------------------------------//
     //               Current number of automata in the list                     //
     //--------------------------------------------------------------------------//
inline int aut_list::has() const
{
 return(lsize);
}
     //--------------------------------------------------------------------------//
     //          Current number of automata in E and not in the list             //
     //--------------------------------------------------------------------------//
inline int aut_list::has_not() const
{
 return(dimension - lsize);
}
     //--------------------------------------------------------------------------//
     //                    Automaton 'a' is in the list                          //
     //--------------------------------------------------------------------------//
inline bool aut_list::has(const aut_id a) const
{
#ifdef _PEPS_DEBUG_
  if (a >= dimension)
    Programming_Error("invalid automaton id", 1201)
#endif
 bool answer = false;
 for (aut_id i=0; i<lsize; i++)
   if (list[i] == a) {
     answer = true;
     break;
 }
 return(answer);
}
     //--------------------------------------------------------------------------//
     //                  Automaton 'a' is not in the list                        //
     //--------------------------------------------------------------------------//
inline bool aut_list::has_not(const aut_id a) const
{
#ifdef _PEPS_DEBUG_
  if (a >= dimension)
    Programming_Error("invalid automaton id", 1202)
#endif
 bool answer = true;
 for (int i=0; i<lsize; i++)
   if (list[i] == a) {
     answer = false;
     break;
 }
 return(answer);
}
     //--------------------------------------------------------------------------//
     //                 The biggest automaton id in the list                     //
     //--------------------------------------------------------------------------//
inline aut_id aut_list::biggest() const
{
 aut_id answer = no_aut;
 for (int i=0; i<lsize; i++)
   if (list[i] > answer)
     answer = list[i];
 return(answer);
}

     //--------------------------------------------------------------------------//
     //                    The p-th automaton of the list                        //
     //--------------------------------------------------------------------------//
inline aut_id aut_list::aut(const int p) const
{
#ifdef _PEPS_DEBUG_
  if (p >= lsize)
    Programming_Error("index out of range", 1203)
#endif
 return(list[p]);
}
     //--------------------------------------------------------------------------//
     //             The position of automaton 'a' in the list                    //
     //--------------------------------------------------------------------------//
inline aut_id aut_list::pos(const aut_id a) const
{
#ifdef _PEPS_DEBUG_
  if (a >= dimension)
    Programming_Error("invalid automaton id", 1204)
#endif
 int answer = no_aut;
 for (int i=0; i<lsize; i++)
   if (list[i] == a) {
     answer = i;
     break;
   }
 return(answer);
}
     //--------------------------------------------------------------------------//
     //                It removes all automata from the list                     //
     //--------------------------------------------------------------------------//
inline void aut_list::take_out()
{
 lsize = 0;
 cut = 0;
}
     //--------------------------------------------------------------------------//
     //                It returns the preferencial cut value (split              //
     //--------------------------------------------------------------------------//
inline int aut_list::get_cut()
{
 return (cut);
}
