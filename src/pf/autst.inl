//====================================================================================//
//                                                                                    //
//                              Class Automata States                                 //
//                                                                                    //
//====================================================================================//
//  This File:   autst.inl                  Language: C++  (xlC and CC)               //
//  Software:    PEPS 3.1                    Advisor: Brigitte.Plateau@imag.fr        //
//  Error Codes: 1300                                                                 //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 07/nov/02                        by: paulof@inf.pucrs.br             //
//====================================================================================//

     //--------------------------------------------------------------------------//
     //                          Empty Constructor                               //
     //--------------------------------------------------------------------------//
inline aut_st::aut_st()
{
 dimension = 0;
}
     //--------------------------------------------------------------------------//
     //                          Sized Constructor                               //
     //--------------------------------------------------------------------------//
inline aut_st::aut_st(const int dim)
{
 dimension = dim;
 info = new st_id[dimension];
}
     //--------------------------------------------------------------------------//
     //                           Copy Constructor                               //
     //--------------------------------------------------------------------------//
inline aut_st::aut_st(const aut_st & s)
{
 dimension = s.dimension;
 info = new st_id[dimension];
 for (aut_id i=0; i<dimension; i++)
   info[i] = s.info[i];
}
     //--------------------------------------------------------------------------//
     //                              Destructor                                  //
     //--------------------------------------------------------------------------//
inline aut_st::~aut_st()
{
 erase();
}
     //--------------------------------------------------------------------------//
     //                           Empty Intialization                            //
     //--------------------------------------------------------------------------//
inline void aut_st::create()
{
 dimension = 0;
}
     //--------------------------------------------------------------------------//
     //                           Sized Intialization                            //
     //--------------------------------------------------------------------------//
inline void aut_st::create(const int dim)
{
 if (dimension != dim) { erase();
                         dimension = dim;
                         info = new int[dimension]; }
}
     //--------------------------------------------------------------------------//
     //                       Copy and Sized Intialization                       //
     //--------------------------------------------------------------------------//
inline void aut_st::copy(const aut_st & s)
{
 create(s.dimension);
 for (aut_id i=0; i<dimension; i++)
   info[i] = s.info[i];
}
     //--------------------------------------------------------------------------//
     //                            Destruction                                   //
     //--------------------------------------------------------------------------//
inline void aut_st::erase()
{
 if (dimension != 0) { delete[] info;
                       dimension = 0; }
}

     //--------------------------------------------------------------------------//
     //                      Total number of automata in E                       //
     //--------------------------------------------------------------------------//
inline int aut_st::size() const
{
 return(dimension);
}
     //--------------------------------------------------------------------------//
     //              Total number of automata in E is equal to zero              //
     //--------------------------------------------------------------------------//
inline bool aut_st::is_empty() const
{
 return(dimension == 0);
}

//====================================================================================//
//                       Functions used only for automata sizes                       //
//====================================================================================//

     //--------------------------------------------------------------------------//
     //                      The size of the automaton 'a'                       //
     //--------------------------------------------------------------------------//
inline int aut_st::sz(const aut_id a) const
{
#ifdef _PEPS_DEBUG_
  if (a >= dimension)
    Programming_Error("index out of range", 1301)
#endif
 return(info[a]);
}
     //--------------------------------------------------------------------------//
     //                    Puts the size of the automaton 'a'                    //
     //--------------------------------------------------------------------------//
inline void aut_st::put(const aut_id a, const st_id s)
{
#ifdef _PEPS_DEBUG_
  if (a >= dimension)
    Programming_Error("index out of range", 1302)
#endif
 info[a] = s;
}

     //--------------------------------------------------------------------------//
     //                   Calculates the product state space                     //
     //--------------------------------------------------------------------------//
inline int aut_st::state_space() const
{
 int answer = 1;
 for (aut_id i=0; i<dimension; i++)
   answer *= info[i];
 return(answer);
}
     //--------------------------------------------------------------------------//
     //        Calculates the product state space for the automata in 's'        //
     //--------------------------------------------------------------------------//
inline int aut_st::state_space(const aut_set & s) const
{
#ifdef _PEPS_DEBUG_
  if (s.size() != dimension)
    Programming_Error("bad state_space call", 1303)
#endif
 int answer = 1;
 for (aut_id i=0; i<dimension; i++)
   if (s.has(i))
     answer *= info[i];
 return(answer);
}

//====================================================================================//
//                     Functions used only to automata states                         //
//====================================================================================//

     //--------------------------------------------------------------------------//
     //                     The pointer to the states array                      //
     //--------------------------------------------------------------------------//
inline int * aut_st::st_addr() const
{
 return(info);
}
     //--------------------------------------------------------------------------//
     //                      The state of the automaton 'a'                      //
     //--------------------------------------------------------------------------//
inline st_id aut_st::st(const aut_id a) const
{
#ifdef _PEPS_DEBUG_
 if (a >= dimension)
    Programming_Error("index out of range", 1304)
#endif
 return(info[a]);
}
     //------------------------------------------------------------+-------------//
     //      The lexical order is used to rank the global states.  | 0, 0, 0, 0  //
     //                                                            | 0, 0, 0, 1  //
     //   Each automata state space is coded from 0 to its size-1. | 0, 0, 1, 0  //
     //  These sizes are stored in the input parameter 's' (type   | 0, 0, 1, 1  //
     //  'aut_st') of all 'next' and 'rank' functions).            | 0, 1, 0, 0  //
     //                                                            | 0, 1, 0, 1  //
     //  - The 'reset' functions assign to 'this' the first state  | 0, 1, 1, 0  //
     //  of the lexical order, i.e., 'this' [0, ... , 0];          | 0, 1, 1, 1  //
     //  - The 'next' functions replace the state in 'this' by its | 1, 0, 0, 0  //
     //  successor (if 'this' is the last state the function       | 1, 0, 0, 1  //
     //  returns the first state);                                 | 1, 0, 1, 0  //
     //  - The 'rank' functions give the rank of 'this' in the     | 1, 0, 1, 1  //
     //  lexical order (0 is the first state);                     | 1, 1, 0, 0  //
     //                                                            | 1, 1, 0, 1  //
     //    For example, if we have four automata of size 2, the    | 1, 1, 1, 0  //
     //  lexical order is presented on the column: ---------->>    | 1, 1, 1, 1  //
     //------------------------------------------------------------+-------------//

     //--------------------------------------------------------------------------//
     //                          Resets all automata                             //
     //--------------------------------------------------------------------------//
inline void aut_st::reset()
{
 for (aut_id i=0; i<dimension; i++)
   info[i] = fst_st;
}
     //--------------------------------------------------------------------------//
     //                    Replaces 'this' by its successor                      //
     //--------------------------------------------------------------------------//
inline void aut_st::next(const aut_st & s)
{
#ifdef _PEPS_DEBUG_
  if (s.size() != dimension)
    Programming_Error("bad next call", 1305)
#endif
 for (aut_id i=dimension-1; i>no_aut; i--)      // Points the automata states
 {                                              // from the last to the first
   info[i]++;                         // increments the state of the pointed automaton
   if (info[i] == s.info[i])          // if the increment is invalid
     info[i] = fst_st;                //   (above the maximum size) reset this automaton
   else                               //   and points to the previous automaton
     break;                           // else (the increment is valid)
 }                                    //   stops the loop
}
     //--------------------------------------------------------------------------//
     //                    Replaces 'this' by its successor                      //
     //--------------------------------------------------------------------------//
inline void aut_st::next_lex(const aut_st & s)
{
#ifdef _PEPS_DEBUG_
  if (s.size() != dimension)
    Programming_Error("bad next call", 1305)
#endif
 for (aut_id i=0; i<dimension; i++)      // Points the automata states
 {                                              // from the last to the first
   info[i]++;                         // increments the state of the pointed automaton
   if (info[i] == s.info[i])          // if the increment is invalid
     info[i] = fst_st;                //   (above the maximum size) reset this automaton
   else                               //   and points to the previous automaton
     break;                           // else (the increment is valid)
 }                                    //   stops the loop
}
     //--------------------------------------------------------------------------//
     //   Resets partially the automata before (left) or after (right) aut. 'a'  //
     //--------------------------------------------------------------------------//
inline void aut_st::reset(const aut_id a, const l_r lr)
{
#ifdef _PEPS_DEBUG_
  if (a >= dimension)
    Programming_Error("index out of range", 1306)
#endif
 aut_id i;
 if (lr == Left)                                  // before (left) automaton 'a'
   for (i=0; i<a; i++)
     info[i] = fst_st;
 else                                             // after (right) automaton 'a'
   for (i=a+1; i<dimension; i++)
     info[i] = fst_st;
}
     //--------------------------------------------------------------------------//
     //   Replaces the left (or right) side of 'a' in 'this' by its successor    //
     //--------------------------------------------------------------------------//
inline void aut_st::next(const aut_st & s, const aut_id a, const l_r lr)
{
 aut_id i;

#ifdef _PEPS_DEBUG_
  if (s.size() != dimension)
    Programming_Error("bad next call", 1307)
  if (a >= dimension)
    Programming_Error("index out of range", 1308)
#endif
 if (lr == Left)                                  // before (left) automaton 'a'
   for (i=a-1; i>no_aut; --i) {
     info[i]++;
     if (info[i] == s.info[i])                    // see the algorithm in the
       info[i] = fst_st;                          // function 'Replaces 'this' 
     else                                         // by its successor' above
       break;
   }
 else                                             // after (right) automaton 'a'
   for (i=dimension-1; i>a; --i) {
     info[i]++;
     if (info[i] == s.info[i])                    // see the algorithm in the
       info[i] = fst_st;                          // function 'Replaces 'this'
     else                                         // by its successor' above
       break;
   }
}

inline bool aut_st::next_aut(const aut_st & s, int a)
{
 bool answer = false;

#ifdef _PEPS_DEBUG_
  if (s.size() != dimension)
    Programming_Error("bad next call", 1305)
#endif

   info[a]++;                         // increments the state of the pointed automaton
   if (info[a] == s.info[a])          // if the increment is invalid 
     info[a] = fst_st;                //   (above the maximum size) reset this automaton
   else                               //   and points to the previous automaton
     answer = true;                  // else (the increment is valid)

   return answer;
}

     //--------------------------------------------------------------------------//
     //     It replaces the state of 'this' by its successor in the lexical      //
     //   order defined using a list 'l' of automata in E an recalculates the    //
     //     rank 'r' in the lexical order defined by the 'jumps' object 'j'      //
     //--------------------------------------------------------------------------//
     // Implementation: according to the order 'l' visits the automata from the  //
     // last to the first, trying to increment the individual state of each      //
     // automaton. An increment is invalid when the individual state of a given  //
     // automaton becomes equal to its maximal size (expressed in the input      //
     // parameter 's'). If an invalid increment is done, the individual state of //
     // the automaton is reset and the previous automata (according to 'l') is   //
     // visited. If a valid increment could be done the function stops. The      //
     // recalculation of the rank 'r' is done by increments and decrements       //
     // corresponding to the increments and resets performed in the individual   //
     // states. The values of such increments and decrements express a lexical   //
     // order (used to generated the 'jumps' object). Each increment corresponds //
     // to the adition of the right state space (function 'nright'). Each reset  //
     // corresponds to the subtraction of the right state space times the        //
     // automata size (function 'njump').                                        //
     //--------------------------------------------------------------------------//
inline void aut_st::next(const aut_list & l, const aut_st & s, const jumps & j, int & r)
{
#ifdef _PEPS_DEBUG_
  if (l.size() != dimension)
    Programming_Error("bad next call", 1309)
  if (s.size() != dimension)
    Programming_Error("bad next call", 1310)
  if (j.size() != dimension)
    Programming_Error("bad next call", 1311)
#endif
 for (int i=l.has()-1; i>no_aut; i--) // Visits the list positions from the last to
 {                                                                          // the first
   int id = l.aut(i);
   info[l.aut(i)]++;                         // inc. the state of the current automaton
   r += j.nright(l.aut(i));
   if (info[l.aut(i)] == s.info[l.aut(i)]) { // if the increment is invalid 
     info[l.aut(i)] = fst_st;                //   (above the maximum size) resets this
     r -= j.njump(l.aut(i));                 //   automaton (visits the previous)
   }
   else                                      // if the increment is valid
     break;                                  //   stops the loop (exit the function)
 }
}
     //------------------------------------------------------------+-------------//
     //     Other lexical orders may be used to rank the states    | 0, 0, 0, 0  //
     //        (defined using a list 'l' of automata in E)         | 0, 0, 1, 0  //
     //                                                            | 1, 0, 0, 0  //
     //   In the following five functions, the positions of the    | 1, 0, 1, 0  //
     // automata are given by the list 'l' ('l' must have the same | 0, 1, 0, 0  //
     // dimension as 'this'). The lexical order is taken according | 0, 1, 1, 0  //
     // to the list 'l'.                                           | 1  1  0  0  //
     //                                                            | 1, 1, 1, 0  //
     // For the example above, if 'l' is { 3, 1, 0, 2 }, the new   | 0, 0, 0, 1  //
     // lexical order is represented in the column: ---------->>   | 0, 0, 1, 1  //
     //                                                            | 1, 0, 0, 1  //
     //   If 'l' is strictly included in the set of reference E,   | 1, 0, 1, 1  //
     // 'l' behaves as a mask on E, which means that the functions | 0, 1, 0, 1  //
     // work only on the automata in 'l'. The lexical order is now | 0, 1, 1, 1  //
     // defined on a subset of states where the automata in E-'l'  | 1  1  0  1  //
     // are constant.                                              | 1, 1, 1, 1  //
     //------------------------------------------------------------+-------------//

     //--------------------------------------------------------------------------//
     //                  Resets the automata of the list 'l'                     //
     //--------------------------------------------------------------------------//
inline void aut_st::reset(const aut_list & l)
{
#ifdef _PEPS_DEBUG_
  if (l.size() != dimension)
    Programming_Error("bad reset call", 1312)
#endif
 for (int i=0; i<l.has(); i++)
        info[l.aut(i)] = fst_st;
}
     //--------------------------------------------------------------------------//
     //  Replaces 'this' by its successor in the lexical order defined with 'l'  //
     //--------------------------------------------------------------------------//
inline void aut_st::next(const aut_list & l, const aut_st & s)
{
#ifdef _PEPS_DEBUG_
  if (l.size() != dimension)
    Programming_Error("bad next call", 1313)
  if (s.size() != dimension)
    Programming_Error("bad next call", 1314)
#endif
 for (int i=l.has()-1; i>no_aut; --i)              // Points the list positions
 {                                                 // from the last to the first
   info[l.aut(i)]++;                       // inc. the state of automaton corresponding
                                           // to the pointed position of the list 'l'
   if (info[l.aut(i)] == s.info[l.aut(i)]) // if the increment is invalid 
     info[l.aut(i)] = fst_st;              //   (above the maximum size) reset this aut.
   else                                    //   and points to the previous 
     break;                                // else (the increment is valid)
 }                                         //   stops the loop (the advancement is done)
}
     //--------------------------------------------------------------------------//
     //    Resets the the automata before (left) or after (right) the 'a'-th     //
     //                      automaton of the list 'l'                           //
     //--------------------------------------------------------------------------//
inline void aut_st::reset(const aut_list & l, const int a, const l_r lr)
{
#ifdef _PEPS_DEBUG_
  if (l.size() != dimension)
    Programming_Error("bad reset call", 1315)
  if (a >= l.has())
    Programming_Error("index out of range", 1316)
#endif
 int i;
 if (lr == Left)                        // before (left) automaton pointed by 'list[a]'
   for (i=0; i<a; i++)
     info[l.aut(i)] = fst_st;
 else                                   // after (right) automaton pointed by 'list[a]'
   for (i=a+1; i<l.has(); i++)
     info[l.aut(i)] = fst_st;
}
     //--------------------------------------------------------------------------//
     //   Replaces 'this' by its successor in a lexical order defined with the   //
     //    sublist of 'l' before (left) or after (right) the 'a'-th automaton    //
     //--------------------------------------------------------------------------//
inline void aut_st::next(const aut_list & l, const aut_st & s, const aut_id a,
                                                               const l_r lr)
{
#ifdef _PEPS_DEBUG_
  if (l.size() != dimension)
    Programming_Error("bad next call", 1317)
  if (s.size() != dimension)
    Programming_Error("bad next call", 1318)
  if (a >= l.has())
    Programming_Error("index out of range", 1319)
#endif
 int i;
 if (lr == Left)                        // before (left) automaton pointed by 'list[a]'
   for (i=a-1; i>no_aut; --i) {
     info[l.aut(i)]++;
     if (info[l.aut(i)] == s.info[l.aut(i)])       // see the algorithm in the
       info[l.aut(i)] = fst_st;                    // function 'Replaces 'this' by
     else                                          // its successor in the lexical
       break;                                      // order defined with 'l' above
   }
 else                                   // after (right) automaton pointed by 'list[a]'
   for (i=dimension-1; i>a; --i) {
     info[l.aut(i)]++;
     if (info[l.aut(i)] == s.info[l.aut(i)])       // see the algorithm in the
       info[l.aut(i)] = fst_st;                    // function 'Replaces 'this' by
     else                                          // its successor in the lexical
       break;                                      // order defined with 'l' above
   }
}

//====================================================================================//
//                     Functions used only to automata labels                         //
//====================================================================================//

     //--------------------------------------------------------------------------//
     //                   Puts the value 'v' to automaton 'a'                    //
     //--------------------------------------------------------------------------//
inline void aut_st::stamp(const aut_id a, const int v)
{
#ifdef _PEPS_DEBUG_
  if (a >= dimension)
    Programming_Error("index out of range", 1320)
#endif
 info[a] = v;
};
     //--------------------------------------------------------------------------//
     //                      Initialize 'info[a]' with 'a'                       //
     //--------------------------------------------------------------------------//
inline void aut_st::stamp()
{
 for (aut_id i=0; i<dimension; i++)
   info[i] = i;
}

     //--------------------------------------------------------------------------//
     //                     Gets the value of automaton 'a'                      //
     //--------------------------------------------------------------------------//
inline int aut_st::val(const aut_id a) const
{
#ifdef _PEPS_DEBUG_
  if (a >= dimension)
    Programming_Error("index out of range", 1321)
#endif
 return(info[a]);
}
     //--------------------------------------------------------------------------//
     //                    Gets the biggest value in 'this'                      //
     //--------------------------------------------------------------------------//
inline int aut_st::biggest() const
{
 int answer = min_integer;
 for (aut_id i=0; i<dimension; i++)
   if (answer < info[i])
     answer = info[i];
 return(answer);
}
     //--------------------------------------------------------------------------//
     //                   Gets the smallest value in 'this'                      //
     //--------------------------------------------------------------------------//
inline int aut_st::smallest() const
{
 int answer = max_integer;
 for (aut_id i=0; i<dimension; i++)
   if (answer > info[i])
     answer = info[i];
 return(answer);
}

     //--------------------------------------------------------------------------//
     //                      The value 'v' is in 'this'                          //
     //--------------------------------------------------------------------------//
inline bool aut_st::has(const int v) const
{
 bool answer = false;
 for (aut_id i=0; i<dimension; i++)
   if (v == info[i]) {
     answer = true;
     break;
   }
 return(answer);
}

     //--------------------------------------------------------------------------//
     //                     The times that 'v' is in 'this'                      //
     //--------------------------------------------------------------------------//
inline int aut_st::count_rep(const int v, aut_id f, aut_id l) const
{
 int answer = 0;
 for (aut_id i=f; i<=l; i++)
   if (v == info[i])
     answer++;
 return(answer);
}
