//====================================================================================//
//                                                                                    //
//                              Class Full Matrix                                     //
//                                                                                    //
//====================================================================================//
//  This File:   fullmat_inl.h              Language: C++  (xlC and CC)               //
//  Software:    PEPS 3.1                    Advisor: Brigitte.Plateau@imag.fr        //
//  Error Codes: 3500                                                                 //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 28/may/97                        by: Paulo.Fernandes@imag.fr         //
//====================================================================================//

     //--------------------------------------------------------------------------//
     //                            Empty Constructor                             //
     //--------------------------------------------------------------------------//
inline full_mat::full_mat()
{
 dimension  = 0;
}
     //--------------------------------------------------------------------------//
     //                            Sized Constructor                             //
     //--------------------------------------------------------------------------//
inline full_mat::full_mat(const int d)
{
 dimension = d;
 mat = new rp*[dimension];
 for (int i=0; i<dimension; i++)
   mat[i] = new rp[dimension];
}
     //--------------------------------------------------------------------------//
     //                             Copy Constructor                             //
     //--------------------------------------------------------------------------//
inline full_mat::full_mat(const full_mat & m)
{
 int i, j;

 dimension  = m.dimension;
 mat = new rp*[dimension];
 for (i=0; i<dimension; i++)
   mat[i] = new rp[dimension];
 for (i=0; i<dimension; i++)
   for (j=0; j<dimension; j++)
     mat[i][j] = m.mat[i][j];
}
     //--------------------------------------------------------------------------//
     //                                Destructor                                //
     //--------------------------------------------------------------------------//
inline full_mat::~full_mat()
{
 if (dimension != 0) {
   for (int i=0; i<dimension; i++)
     delete[] mat[i];
   delete[] mat;
   dimension  = 0;
 }
}

     //--------------------------------------------------------------------------//
     //                           Empty Intialization                            //
     //--------------------------------------------------------------------------//
inline void full_mat::create()
{
 dimension  = 0;
}
     //--------------------------------------------------------------------------//
     //                           Sized Intialization                            //
     //--------------------------------------------------------------------------//
inline void full_mat::create(const int d)
{
 if (dimension != d) {
   erase();
   dimension = d;
   mat = new rp*[dimension];
   for (int i=0; i<dimension; i++)
     mat[i] = new rp[dimension];
 }
}
     //--------------------------------------------------------------------------//
     //                       Copy and Sized Intialization                       //
     //--------------------------------------------------------------------------//
inline void full_mat::copy(const full_mat & m)
{
 int i, j;

 dimension  = m.dimension;
 mat = new rp*[dimension];
 for (i=0; i<dimension; i++)
   mat[i] = new rp[dimension];
 for (i=0; i<dimension; i++)
   for (j=0; j<dimension; j++)
     mat[i][j] = m.mat[i][j];
}
     //--------------------------------------------------------------------------//
     //                               Destruction                                //
     //--------------------------------------------------------------------------//
inline void full_mat::erase()
{
 if (dimension != 0) {
   for (int i=0; i<dimension; i++)
     delete[] mat[i];
   delete[] mat;
   dimension  = 0;
 }
}

     //--------------------------------------------------------------------------//
     //                         The order of the matrix                          //
     //--------------------------------------------------------------------------//
inline int full_mat::size() const
{
 return(dimension);
}
     //--------------------------------------------------------------------------//
     //                         The order of the matrix                          //
     //--------------------------------------------------------------------------//
inline int full_mat::od() const
{
 return(dimension);
}
     //--------------------------------------------------------------------------//
     //                     The number of non-zero elements                      //
     //--------------------------------------------------------------------------//
inline int full_mat::nz() const
{
 int i, j;
 int answer = 0;

 for (i=0; i<dimension; i++)
   for (j=0; j<dimension; j++)
     if (mat[i][j] != zero)
       answer++;
 return(answer);
}
     //--------------------------------------------------------------------------//
     //                            The order is zero                             //
     //--------------------------------------------------------------------------//
inline bool full_mat::is_empty() const
{
 return(dimension == 0);
}
     //--------------------------------------------------------------------------//
     //                       There are negative elements                        //
     //--------------------------------------------------------------------------//
inline bool full_mat::has_negatives() const
{
 int i, j;
 bool answer = false;

 for (i=0; i<dimension; i++)
   for (j=0; j<dimension; j++)
     if (mat[i][j] < zero) {
       answer = true;
       i = dimension;            // exit the outer loop ('i')
       break;
     }
 return(answer);
}
     //--------------------------------------------------------------------------//
     //                          It returns an element                           //
     //--------------------------------------------------------------------------//
inline rp full_mat::coord(const int i, const int j) const
{
#ifdef _PEPS_DEBUG_
  if ((i >= dimension) || (j >= dimension))
    Programming_Error("index out of range", 3501)
#endif
 return(mat[i][j]);
}
     //--------------------------------------------------------------------------//
     //                           It stores an element                           //
     //--------------------------------------------------------------------------//
inline void full_mat::coord(const int i, const int j, const rp v)
{
#ifdef _PEPS_DEBUG_
  if ((i >= dimension) || (j >= dimension))
    Programming_Error("index out of range", 3502)
#endif
 mat[i][j] = v;
}
     //--------------------------------------------------------------------------//
     //                       It sets all elements to zero                       //
     //--------------------------------------------------------------------------//
inline void full_mat::reset()
{
 int i, j;
 for (i=0; i<dimension; i++)
   for (j=0; j<dimension; j++)
     mat[i][j] = zero;
}
     //--------------------------------------------------------------------------//
     //                         It performs 'this *= f'                          //
     //--------------------------------------------------------------------------//
inline void full_mat::scale(const rp f)
{
 int i, j;
 for (i=0; i<dimension; i++)
   for (j=0; j<dimension; j++)
     mat[i][j] *= f;
}
