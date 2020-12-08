//====================================================================================//
//                                                                                    //
//                       Class Harwell-Boeing Format Matrix                           //
//                                                                                    //
//====================================================================================//
//  This File:   hbf_inl.h                  Language: C++  (xlC and CC)               //
//  Software:    PEPS 3.1                    Advisor: Brigitte.Plateau@imag.fr        //
//  Error Codes: 3600                                                                 //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change:  1/mar/99                        by: Paulo.Fernandes@imag.fr         //
//====================================================================================//

     //--------------------------------------------------------------------------//
     //                         Empty Constructor                                //
     //--------------------------------------------------------------------------//
HBF::HBF()
{
 dimension = 0;
 nz_number = 0;
}
     //--------------------------------------------------------------------------//
     //                         Sized Constructor                                //
     //--------------------------------------------------------------------------//
inline HBF::HBF(const int n, const int d)
{
 dimension = d;
 nz_number = n;
 elements  = new rp[nz_number];
 rows      = new int[nz_number];
 columns   = new int[dimension+1];
}
     //--------------------------------------------------------------------------//
     //                          Copy Constructor                                //
     //--------------------------------------------------------------------------//
inline HBF::HBF(const HBF & m)
{
 int k, j;

 if ((dimension != m.dimension) || (nz_number != m.nz_number)) {
   dimension = m.dimension;
   nz_number = m.nz_number;
   elements  = new rp[nz_number];
   rows      = new int[nz_number];
   columns   = new int[dimension+1];
 }
 for (k=0; k<nz_number; k++)
   elements[k] = m.elements[k];
 for (k=0; k<nz_number; k++)
   rows[k]     = m.rows[k];
 for (j=0; j<=dimension; j++)
   columns[j]  = m.columns[j];
}
     //--------------------------------------------------------------------------//
     //                            Destructor                                    //
     //--------------------------------------------------------------------------//
inline HBF::~HBF()
{
 if (dimension != 0) {
   delete[] elements;
   delete[] rows;
   delete[] columns;
   dimension = 0;
   nz_number = 0;
 }
 strcpy(file_name, "\0");
}
     //--------------------------------------------------------------------------//
     //                         Empty Intialization                              //
     //--------------------------------------------------------------------------//
inline void HBF::create()
{
 dimension = 0;
 nz_number = 0;
}
     //--------------------------------------------------------------------------//
     //                         Sized Intialization                              //
     //--------------------------------------------------------------------------//
inline void HBF::create(const int n, const int d)
{
 if ((dimension != d) || (nz_number != n)) {
   erase();
   dimension = d;
   nz_number = n;
   elements  = new rp[nz_number];
   rows      = new int[nz_number];
   columns   = new int[dimension+1];
 }
}
     //--------------------------------------------------------------------------//
     //                     Copy and Sized Intialization                         //
     //--------------------------------------------------------------------------//
inline void HBF::copy(const HBF & m)
{
 int k, i;

 dimension = m.dimension;
 nz_number = m.nz_number;
 elements  = new rp[nz_number];
 rows      = new int[nz_number];
 columns   = new int[dimension+1];
 for (k=0; k<nz_number; k++)
   elements[k] = m.elements[k];
 for (k=0; k<nz_number; k++)
   rows[k]     = m.rows[k];
 for (i=0; i<=dimension; i++)
   columns[i]  = m.columns[i];
}
     //--------------------------------------------------------------------------//
     //                            Destruction                                   //
     //--------------------------------------------------------------------------//
inline void HBF::erase()
{
 if (dimension != 0) {
   delete[] elements;
   delete[] rows;
   delete[] columns;
   dimension = 0;
   nz_number = 0;
 }
 strcpy(file_name, "\0");
}

     //--------------------------------------------------------------------------//
     //                       It assigns a file name                             //
     //--------------------------------------------------------------------------//
inline void HBF::baptise(const peps_name file)
{
 strcpy(file_name, file);
}
     //--------------------------------------------------------------------------//
     //                       It returns the file name                           //
     //--------------------------------------------------------------------------//
inline void HBF::name(peps_name file) const
{
 strcpy(file, file_name);
}
     //--------------------------------------------------------------------------//
     //                    The number of nonzero elements                        //
     //--------------------------------------------------------------------------//
inline int HBF::size() const
{
 return(nz_number);
}
     //--------------------------------------------------------------------------//
     //                 The matrix order (the number of rows)                    //
     //--------------------------------------------------------------------------//
inline int HBF::order() const
{
 return(dimension);
}
     //--------------------------------------------------------------------------//
     //                          The order is zero                               //
     //--------------------------------------------------------------------------//
inline bool HBF::is_empty() const
{
 return(dimension == 0);
}
     //--------------------------------------------------------------------------//
     //             The index of the first row (indicates the format)            //
     //--------------------------------------------------------------------------//
inline int HBF::first_row() const
{
 return(columns[0]);  // actually return the first column index
}                     //    (rows and columns indexes are equal)
