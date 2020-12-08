//====================================================================================//
//                                                                                    //
//                              Class Permutation Jumps                               //
//                                                                                    //
//====================================================================================//
//  This File:   jumps_inl.h                Language: C++  (xlC and CC)               //
//  Software:    PEPS 3.1                    Advisor: Brigitte.Plateau@imag.fr        //
//  Error Codes: 5200                                                                 //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 28/may/97                        by: Paulo.Fernandes@imag.fr         //
//====================================================================================//

     //--------------------------------------------------------------------------//
     //                          Empty Constructor                               //
     //--------------------------------------------------------------------------//
inline jumps::jumps()
{
 dimension = 0;
}
     //--------------------------------------------------------------------------//
     //                          Sized Constructor                               //
     //--------------------------------------------------------------------------//
inline jumps::jumps(const int dim)
{
 dimension = dim;
 n_left  = new int[dimension];
 n_right = new int[dimension];
 n_jump  = new int[dimension];
}
     //--------------------------------------------------------------------------//
     //                           Copy Constructor                               //
     //--------------------------------------------------------------------------//
inline jumps::jumps(const jumps & j)
{
 dimension = j.dimension;
 n_left  = new int[dimension];
 n_right = new int[dimension];
 n_jump  = new int[dimension];
 for (int i=0; i<dimension; i++) {
   n_left[i]  = j.n_left[i];
   n_right[i] = j.n_right[i];
   n_jump[i]  = j.n_jump[i];
 }
}
     //--------------------------------------------------------------------------//
     //                              Destructor                                  //
     //--------------------------------------------------------------------------//
inline jumps::~jumps()
{
 erase();
}

     //--------------------------------------------------------------------------//
     //                           Empty Intialization                            //
     //--------------------------------------------------------------------------//
inline void jumps::create()
{
 dimension = 0;
}
     //--------------------------------------------------------------------------//
     //                           Sized Intialization                            //
     //--------------------------------------------------------------------------//
inline void jumps::create(const int dim)
{
 if (dimension != dim) {
   erase();
   dimension = dim;
   n_left  = new int[dimension];
   n_right = new int[dimension];
   n_jump  = new int[dimension];
 }
}
     //--------------------------------------------------------------------------//
     //                       Copy and Sized Intialization                       //
     //--------------------------------------------------------------------------//
inline void jumps::copy(const jumps & j)
{
 if (dimension != j.dimension)
   create(j.dimension);
 for (int i=0; i<dimension; i++) {
   n_left[i]  = j.n_left[i];
   n_right[i] = j.n_right[i];
   n_jump[i]  = j.n_jump[i];
 }
}
     //--------------------------------------------------------------------------//
     //                            Destruction                                   //
     //--------------------------------------------------------------------------//
inline void jumps::erase()
{
 if (dimension != 0) {
   delete[] n_left;
   delete[] n_right;
   delete[] n_jump;
   dimension = 0; }
}

     //--------------------------------------------------------------------------//
     //                     Number of automata in the model                      //
     //--------------------------------------------------------------------------//
inline int jumps::size() const
{
 return(dimension);
}
     //--------------------------------------------------------------------------//
     //                    The allocation has not been done                      //
     //--------------------------------------------------------------------------//
inline bool jumps::is_empty() const
{
 return(dimension == 0);
}
     //--------------------------------------------------------------------------//
     //                          The value 'n_left[a]'                           //
     //--------------------------------------------------------------------------//
inline int jumps::nleft(const aut_id a) const
{
#ifdef _PEPS_DEBUG_
  if (a >= dimension)
    Programming_Error("index out of range", 5209)
#endif
 return(n_left[a]);
}
     //--------------------------------------------------------------------------//
     //                          The value 'n_right[a]'                          //
     //--------------------------------------------------------------------------//
inline int jumps::nright(const aut_id a) const
{
#ifdef _PEPS_DEBUG_
  if (a >= dimension)
    Programming_Error("index out of range", 5210)
#endif
 return(n_right[a]);
}
     //--------------------------------------------------------------------------//
     //                           The value 'n_jump[a]'                          //
     //--------------------------------------------------------------------------//
inline int jumps::njump(const aut_id a) const
{
#ifdef _PEPS_DEBUG_
  if (a >= dimension)
    Programming_Error("index out of range", 5211)
#endif
 return(n_jump[a]);
}
