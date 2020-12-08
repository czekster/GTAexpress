//====================================================================================//
//                                                                                    //
//                       Integration Functions Table Structure                        //
//                                                                                    //
//====================================================================================//
//  This File:   inf_inl.h                  Language: C++  (xlC and CC)               //
//  Software:    PEPS 3.1                    Advisor: Brigitte.Plateau@imag.fr        //
//  Error Code:  4400                                                                 //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 20/ago/97                        by: Paulo.Fernandes@imag.fr         //
//====================================================================================//

     //--------------------------------------------------------------------------//
     //                          Empty Constructor                               //
     //--------------------------------------------------------------------------//
inline INF::INF()
{
 strcpy(file_name, "\0");
 dimension = 0;
 number    = 0;
 domain.create();
}
     //--------------------------------------------------------------------------//
     //                          Sized Constructor                               //
     //--------------------------------------------------------------------------//
inline INF::INF(const int dim)
{
 dimension = dim;
 number    = 0;
 alloc();
 domain.create();
}
     //--------------------------------------------------------------------------//
     //                           Copy Constructor                               //
     //--------------------------------------------------------------------------//
inline INF::INF(const INF & t)
{
 dimension = t.dimension;
 number    = t.number;
 for (int i=0; i<dimension; i++) {
  table[i].copy(t.table[i]);
 }
 domain.copy(t.domain);
}
     //--------------------------------------------------------------------------//
     //                              Destructor                                  //
     //--------------------------------------------------------------------------//
inline INF::~INF()
{
 if (dimension != 0 ) {
   desal();
   dimension = 0;
   number    = 0;
 }
 domain.erase();
 strcpy(file_name, "\0");
}

     //--------------------------------------------------------------------------//
     //                           Empty Intialization                            //
     //--------------------------------------------------------------------------//
inline void INF::create()
{
 strcpy(file_name, "\0");
 dimension = 0;
 number    = 0;
 domain.create();
}
     //--------------------------------------------------------------------------//
     //                           Sized Intialization                            //
     //--------------------------------------------------------------------------//
inline void INF::create(const int dim)
{
 erase();
 dimension = dim;
 number    = 0;
 alloc();
 domain.create();
}
     //--------------------------------------------------------------------------//
     //                       Copy and Sized Intialization                       //
     //--------------------------------------------------------------------------//
inline void INF::copy(const INF & t)
{
 if (dimension != t.dimension) {
   erase();
   dimension = t.dimension;
   alloc();
 }
 number = t.number;
 for (int i=0; i<dimension; i++) {
  table[i].copy(t.table[i]);
 }
 domain.copy(t.domain);
}
     //--------------------------------------------------------------------------//
     //                            Destruction                                   //
     //--------------------------------------------------------------------------//
inline void INF::erase()
{
 if (dimension != 0) {
   desal();
   dimension = 0;
   number = 0;
 }
 domain.erase();
 strcpy(file_name, "\0");
}


     //--------------------------------------------------------------------------//
     //               The current number of functions is zero                    //
     //--------------------------------------------------------------------------//
inline bool INF::is_empty() const
{
 return(number == 0);
}
     //--------------------------------------------------------------------------//
     //               There is no more place for new functions                   //
     //--------------------------------------------------------------------------//
inline bool INF::is_full() const
{
 return(dimension == number);
}
     //--------------------------------------------------------------------------//
     //                          It assigns a file name                          //
     //--------------------------------------------------------------------------//
inline void INF::baptise(const peps_name file)
{
 strcpy(file_name, file);
}
     //--------------------------------------------------------------------------//
     //                         It returns the file name                         //
     //--------------------------------------------------------------------------//
inline void INF::name(peps_name file) const
{
 strcpy(file, file_name);
}
     //--------------------------------------------------------------------------//
     //                     The maximum number of functions                      //
     //--------------------------------------------------------------------------//
inline int INF::size() const
{
 return(dimension);
}
     //--------------------------------------------------------------------------//
     //                     The current number of functions                      //
     //--------------------------------------------------------------------------//
inline int INF::has() const
{
 return(number);
}
     //--------------------------------------------------------------------------//
     //                     The original state space product                     //
     //--------------------------------------------------------------------------//
inline int INF::ss() const
{
 return(domain.state_space());
}
     //--------------------------------------------------------------------------//
     //                   It Includes function 't' in 'this'                     //
     //--------------------------------------------------------------------------//
inline func_id INF::put(const func_tree & t)
{
#ifdef _PEPS_DEBUG_
  if (number == dimension)
    Programming_Error("INF structure full", 4401)
  if (!(t.well_def(true)))
    Programming_Error("Inclusion of a spurious function", 4402)
#endif

 table[number].copy(t);
 number++;
 return(number-1);
}
     //--------------------------------------------------------------------------//
     //            It copies the function located in entry 'f' to 't'            //
     //--------------------------------------------------------------------------//
inline void INF::get(const func_id f, func_tree & t) const
{
#ifdef _PEPS_DEBUG_
  if (f >= number)
    Programming_Error("index out of range", 4403)
#endif
 t.copy(table[f]);
}
     //--------------------------------------------------------------------------//
     //         It evaluates the function in entry 'f' for a global state        //
     //--------------------------------------------------------------------------//
     //    For the first function the global state is directly given by the      //
     // input parameter 's'. In this case the size of 's' must be equal to the   //
     // original number of automata (size of the variable 'domain').             //
     //--------------------------------------------------------------------------//
inline rp INF::eval(const func_id f, const aut_st & s) const
{
#ifdef _PEPS_DEBUG_
  if (f >= number)
    Programming_Error("index out of range", 4304)
  if (s.size() != domain.size())
    Programming_Error("wrong global state size", 4305)
#endif
 return(table[f].eval(s));
}
     //--------------------------------------------------------------------------//
     //                 It puts the sizes of the original automata               //
     //--------------------------------------------------------------------------//
inline void INF::put_domain(const aut_st & s)
{
 domain.copy(s);
}
     //--------------------------------------------------------------------------//
     //            It Fills 's' with the original sizes of the automata          //
     //--------------------------------------------------------------------------//
inline void INF::get_domain(aut_st & s) const
{
 s.copy(domain);
}

