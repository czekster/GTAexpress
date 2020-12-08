//====================================================================================//
//                                                                                    //
//                                Class State Map                                     //
//                                                                                    //
//====================================================================================//
//  This File:   stmap_inl.h                Language: C++  (xlC and CC)               //
//  Software:    PEPS 3.1                    Advisor: Brigitte.Plateau@imag.fr        //
//  Error Codes: 5100                                                                 //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 28/jun/97                        by: Paulo.Fernandes@imag.fr         //
//====================================================================================//

     //--------------------------------------------------------------------------//
     //                            Empty Constructor                             //
     //--------------------------------------------------------------------------//
inline st_map::st_map()
{
 rows     = 0;
 automata = 0;
}
     //--------------------------------------------------------------------------//
     //                            Sized Constructor                             //
     //--------------------------------------------------------------------------//
inline st_map::st_map(const int r, const aut_id a)
{
 rows     = r;
 automata = a;
 alloc();
}
     //--------------------------------------------------------------------------//
     //                             Copy Constructor                             //
     //--------------------------------------------------------------------------//
inline st_map::st_map(const st_map & m)
{
 rows     = m.rows;
 automata = m.automata;
 alloc();
 trans(m);
}
     //--------------------------------------------------------------------------//
     //                               Destructor                                 //
     //--------------------------------------------------------------------------//
inline st_map::~st_map()
{
 if (automata != 0) {
   desal();
   automata = 0;
   rows     = 0;
 }
}

     //--------------------------------------------------------------------------//
     //                           Empty Intialization                            //
     //--------------------------------------------------------------------------//
inline void st_map::create()
{
 rows     = 0;
 automata = 0;
}
     //--------------------------------------------------------------------------//
     //                           Sized Intialization                            //
     //--------------------------------------------------------------------------//
inline void st_map::create(const int r, const aut_id a)
{
#ifdef _PEPS_DEBUG_
  if ((a == 0) || (r == 0))
    Programming_Error("inconsistent state map allocation", 5101)
#endif
  erase();
  rows     = r;
  automata = a;
  alloc();
}
     //--------------------------------------------------------------------------//
     //                       Copy and Sized Intialization                       //
     //--------------------------------------------------------------------------//
inline void st_map::copy(const st_map & m)
{
 create(m.rows, m.automata);
 trans(m);
}
     //--------------------------------------------------------------------------//
     //                               Destruction                                //
     //--------------------------------------------------------------------------//
inline void st_map::erase()
{
 if (rows != 0) {
   desal();
   rows     = 0;
   automata = 0;
 }
}

     //--------------------------------------------------------------------------//
     //               The number of states of the grouped automaton              //
     //--------------------------------------------------------------------------//
inline int st_map::size() const
{
 return(rows);
}
     //--------------------------------------------------------------------------//
     //               The number of automata that has been grouped               //
     //--------------------------------------------------------------------------//
inline int st_map::aut() const
{
 return(automata);
}
     //--------------------------------------------------------------------------//
     //               The number of states of the grouped automaton              //
     //--------------------------------------------------------------------------//
inline int st_map::row() const
{
 return(rows);
}
     //--------------------------------------------------------------------------//
     //                    The grouped automaton has no states                   //
     //--------------------------------------------------------------------------//
inline bool st_map::is_empty() const
{
 return(rows == 0);
}
     //--------------------------------------------------------------------------//
     //                     The grouped automaton is singular                    //
     //--------------------------------------------------------------------------//
inline bool st_map::is_singular() const
{
 return(automata == 1);
}
     //--------------------------------------------------------------------------//
     //           It fills 's' with the automata that have been grouped          //
     //--------------------------------------------------------------------------//
inline void st_map::get(aut_set & s) const
{
#ifdef _PEPS_DEBUG_
  if (s.size() != FTB::ftb.aut())
    Programming_Error("bad sized automata subset", 5102)
#endif
 for (aut_id a=0; a<automata; a++)
   s.put_in(position[a]);
}
     //--------------------------------------------------------------------------//
     //            It gets the 'p'-th automaton that has been grouped            //
     //--------------------------------------------------------------------------//
inline aut_id st_map::get(const int p) const
{
#ifdef _PEPS_DEBUG_
  if (p >= automata)
    Programming_Error("index out of range", 5103)
#endif
 return(position[p]);
}
     //--------------------------------------------------------------------------//
     //     It gets the local states of the entry 'r' of the array 'states'      //
     //--------------------------------------------------------------------------//
     // Obs.: the size of the output parameter 's' is the original number of     //
     // automata. Only the local states of the original automata used by 'this'  //
     // are filled with the original states corresponding the state 'r'.         //
     //--------------------------------------------------------------------------//
inline void st_map::get(const st_id r, aut_st & s) const
{
#ifdef _PEPS_DEBUG_
  if (s.size() != FTB::ftb.aut())
    Programming_Error("bad sized state vector", 5104)
  if (r >= rows)
    Programming_Error("index out of range", 5105)
#endif
 for (aut_id a=0; a<automata; a++)
   s.put(position[a], states[r][a]);
}

        //--------------------------------------------------------------------------//
        //           It gets the grouped state of the original states 's'           //
        //--------------------------------------------------------------------------//
inline int st_map::get(aut_st & s)
{
#ifdef _PEPS_DEBUG_
  if (s.size() != FTB::ftb.aut())
    Programming_Error("bad sized state vector", 5104)
#endif

 bool equal;  

 s.sort(position[0],position[automata-1]);
 for (int r=0; r<rows; r++){
   equal = 1;
   for(int a=0; a<automata; a++){  
     if(states[r][a]!=s.st(position[a]))
       equal = 0;
   }
   if(equal==1)
     return(r);
 }
 return(-1);
}
