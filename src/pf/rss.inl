//====================================================================================//
//                                                                                    //
//                        Reacheable State Space Structure                            //
//                                                                                    //
//====================================================================================//
//  This File:   rss_inl.h                  Language: C++  (xlC and CC)               //
//  Software:    PEPS 2002                   Advisor: Brigitte.Plateau@imag.fr        //
//  Error Codes: 2200                                                                 //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 10/oct/02                        by: paulof@inf.pucrs.br             //
//====================================================================================//

     //--------------------------------------------------------------------------//
     //                         Empty Constructor                                //
     //--------------------------------------------------------------------------//
inline RSS::RSS()
{
 strcpy(file_name, "\0");
 f_parameters.create();
 dimension = 0;
 originals = 0;
 reachable = 0;
 states.create();
 remain.create();
}
     //--------------------------------------------------------------------------//
     //                         Sized Constructor                                //
     //--------------------------------------------------------------------------//
inline RSS::RSS(const int dim)
{
 f_parameters.create();
 dimension = dim;
 originals = dim;
 reachable = 0;
 states.create(originals);
 remain.create(originals);
 remain.put();
 strcpy(file_name, "\0");
}
     //--------------------------------------------------------------------------//
     //                          Copy Constructor                                //
     //--------------------------------------------------------------------------//
inline RSS::RSS(const RSS & v)
{
 f_parameters.copy(v.f_parameters);
 dimension = v.dimension;
 originals = v.originals;
 reachable = v.reachable;
 states.copy(v.states);
 remain.copy(v.remain);
 strcpy(file_name, "\0");
}
     //--------------------------------------------------------------------------//
     //                            Destructor                                    //
     //--------------------------------------------------------------------------//
inline RSS::~RSS()
{
 if (dimension != 0) {
   f_parameters.erase();
   dimension = 0;
   originals = 0;
   reachable = 0;
   states.erase();
   remain.erase();
 }
 strcpy(file_name, "\0");
}

     //--------------------------------------------------------------------------//
     //                         Empty Intialization                              //
     //--------------------------------------------------------------------------//
inline void RSS::create()
{
 strcpy(file_name, "\0");
 f_parameters.create();
 dimension = 0;
 originals = 0;
 reachable = 0;
 states.create();
 remain.create();
}
     //--------------------------------------------------------------------------//
     //                         Sized Intialization                              //
     //--------------------------------------------------------------------------//
inline void RSS::create(const int dim)
{
 if (dimension != dim) {
   erase();
   f_parameters.erase();
   dimension = dim;
   originals = dim;
   reachable = 0;
   states.create(originals);
   remain.create(originals);
   remain.put();
 }
 strcpy(file_name, "\0");
}
     //--------------------------------------------------------------------------//
     //                         Sized Intialization                              //
     //--------------------------------------------------------------------------//
inline void RSS::create(const aut_st & s)
{
 int dim = s.state_space();
 if (dimension != dim) {
   erase();
   f_parameters.create(s.size());
   dimension = dim;
   originals = dim;
   reachable = 0;
   states.create(originals);
   remain.create(originals);
   remain.put();
 }
 strcpy(file_name, "\0");
}
     //--------------------------------------------------------------------------//
     //                     Copy and Sized Intialization                         //
     //--------------------------------------------------------------------------//
inline void RSS::copy(const RSS & v)
{
 if (dimension != v.dimension) {
   erase();
   f_parameters.copy(v.f_parameters);
   dimension = v.dimension;
   originals = v.originals;
   reachable = v.reachable;
   states.copy(v.states);
   remain.copy(v.remain);
 }
 strcpy(file_name, "\0");
}
     //--------------------------------------------------------------------------//
     //                            Destruction                                   //
     //--------------------------------------------------------------------------//
inline void RSS::erase()
{
 if (dimension != 0) {
   f_parameters.erase();
   dimension = 0;
   originals = 0;
   reachable = 0;
   states.erase();
   remain.erase();
 }
 strcpy(file_name, "\0");
}

     //--------------------------------------------------------------------------//
     //                       It assigns a file name                             //
     //--------------------------------------------------------------------------//
inline void RSS::baptise(const peps_name file)
{
 strcpy(file_name, file);
}
     //--------------------------------------------------------------------------//
     //                       It returns the file name                           //
     //--------------------------------------------------------------------------//
inline void RSS::name(peps_name file) const
{
 strcpy(file, file_name);
}
     //--------------------------------------------------------------------------//
     //                     There are no remaining states                        //
     //--------------------------------------------------------------------------//
inline bool RSS::is_empty() const
{
 return(originals == 0);
}
        //--------------------------------------------------------------------------//
        //                  The number of remaining states                          //
        //--------------------------------------------------------------------------//
inline int RSS::size() const
{
 return(dimension);
}
     //--------------------------------------------------------------------------//
     //                     The number of original states                        //
     //--------------------------------------------------------------------------//
inline int RSS::orig_size() const
{
 return(originals);
}
     //--------------------------------------------------------------------------//
     //                    The number of reachable states                        //
     //--------------------------------------------------------------------------//
inline int RSS::reach_size() const
{
 return(reachable);
}
     //--------------------------------------------------------------------------//
     //                  The original state 's' is reachable                     //
     //--------------------------------------------------------------------------//
inline bool RSS::has(const int s) const
{
#ifdef _PEPS_DEBUG_
  if (s >= originals)
    Programming_Error("index out of range", 2201)
#endif
 return(states[s]);
}
     //--------------------------------------------------------------------------//
     //               The original state 's' was not eliminated                  //
     //--------------------------------------------------------------------------//
inline bool RSS::remains(const int s) const
{
#ifdef _PEPS_DEBUG_
  if (s >= originals)
    Programming_Error("index out of range", 2202)
#endif
 return(remain[s]);
}

     //--------------------------------------------------------------------------//
     //                      It copies 'v' to 'remain'                           //
     //--------------------------------------------------------------------------//
     //  Restriction: this function should NEVER eliminate reachable states.     // 
     //--------------------------------------------------------------------------//
inline void RSS::reduce(const bools & v)
{
#ifdef _PEPS_DEBUG_
  if (v.size() != originals)
    Programming_Error("wrong vector size for reduction", 2203)
  for (int i=0; i<originals; i++)
    if (!v[i] && states[i])
      Programming_Error("removing reachable states", 2204)
#endif
  remain.copy(v);
  dimension = remain.has();
}
     //--------------------------------------------------------------------------//
     //                Parameters of the reachability function                   //
     //--------------------------------------------------------------------------//
     //   The next two functions provide an access to the subset of the automata //
     // that are parameters to the reachability function.                        //
     //--------------------------------------------------------------------------//
inline void RSS::put_dep(const aut_set & s)
{
#ifdef _PEPS_DEBUG_
  if (s.size() != FTB::ftb.aut())
    Programming_Error("incosistent parameters for the RSS", 2209)
#endif
 f_parameters.copy(s);
}
void RSS::get_dep(aut_set & s) const
{
 s.copy(f_parameters);
}
