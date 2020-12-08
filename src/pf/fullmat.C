//====================================================================================//
//                                                                                    //
//                              Class Full Matrix                                     //
//                                                                                    //
//====================================================================================//
//  This File:   fullmat.C                  Language: C++  (xlC and CC)               //
//  Software:    PEPS 3.1                    Advisor: Brigitte.Plateau@imag.fr        //
//  Doc:         3500                                                                 //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 14/feb/00                        by: Paulo.Fernandes@imag.fr         //
//====================================================================================//
#include "peps.h"

     //--------------------------------------------------------------------------//
     //         It reduces 'this' keeping only the row and columns in 'v'        //
     //--------------------------------------------------------------------------//
void full_mat::reduce(const bools & v)
{
#ifdef _PEPS_DEBUG_
  if (v.size() != dimension)
    Programming_Error("wrong size in reduction", 3503)
#endif
 rp  ** new_mat;
 int    i,j;
 int    new_dim;

 new_dim = v.has();                  // gets the number of rows and columns to keep
 new_mat = new rp*[new_dim];         // allocates of the new two-dimensional array
 for (i=0; i<new_dim; i++)
   new_mat[i] = new rp[new_dim];
 for (i=0; i<dimension; i++)         // copies of the rows and columns to keep
   if (v[i])
     for (j=0; j<dimension; j++)
       if (v[j])
         new_mat[i][j] = mat[i][j];
 delete[] mat;                       // replaces old array for the new
 dimension = new_dim;
 mat = new_mat;
}

     //--------------------------------------------------------------------------//
     // Solution of linear system in 'x = y this' considering only the first 'k' //
     //             rows and columns (the answer is the vector 'y')              //
     //--------------------------------------------------------------------------//
     // Algorithm:  - It performs a LU decomposition of the first 'k' rows and   //
     //             columns, storing the matrices in the variable 'tmp';         //
     //             - It solves the linear system 'x * (LU)^-1', storing the     //
     //             answer in the output parameter 'y';                          //
     //--------------------------------------------------------------------------//
     // Restriction: - 'k' must be less than or equal than the 'this' dimension; //
     //              - vectors 'x' and 'y' must be allocated for at least 'k'    //
     //              positions (this restriction is not tested);                 //
     //--------------------------------------------------------------------------//
     // Note: This procedure was implemented according to the LU decomposition,  //
     // backward and forward substitution algorithms presented in the book       //
     // "Numerical Recipes in C" (Press, Vetterling, Teukolsky & Flannery), in   //
     // section "2.3. LU decomposition and its applications" (page 43).          //
     //--------------------------------------------------------------------------//
void full_mat::solve(const rp * x, rp * y, const int k) const
{
#ifdef _PEPS_DEBUG_
  if (k > dimension)
    Programming_Error("index out of range in solution", 3504)
#endif
 rp    th = zero; // performs a complete decomposition (threshold = 0.0)
 rp ** tmp;       // temporary matrix to store the matrices L and U
 rp    pivot;     // diagonal element of 'this' of the row being treated
 int   i, j, h;   // counters for rows, columns and partial rows of the LU matrix (resp.

 //-------------------------------------------------------------------------------------
 //--------------------------- lu decomposition of 'this' in 'tmp' (in the Crout format)
 tmp = new rp*[k];                  // allocates temporary structure 'tmp'
 for (i=0; i<k; i++)
   tmp[i] = new rp[k];
 for (i=0; i<k; i++)                // copies 'this' to 'tmp'
   for (j=0; j<k; j++)
     if (absv(mat[i][j]) < th)         // only values greater than
       tmp[i][j] = zero;               // the threshold are kept
     else
       tmp[i][j] = mat[i][j];
 for (i=1; i<k; i++)                // for the second to the last row
   for (j=0; j<i; j++) {               // eliminates the nonzero elements before
                                       //   the diagonal
     if (absv(tmp[j][j]) > close_to_zero)      // aproximation if an exact inversion is
       pivot = tmp[i][j] / tmp[j][j];          //   impossible (replaces zero diagonal
     else                                      //   elements by one)
       pivot = zero;
     tmp[i][j] = pivot;
     for (h=j+1; h<k; h++)
       tmp[i][h] -= pivot * tmp[j][h];
   }

 //-------------------------------------------------------------------------------------
 //------------------------- solution of the linear system 'x * (LU)^-1' (result in 'y')
 for (j=0; j<k; j++)                 // initializes 'y' with 'x'
   y[j] = x[j];
 
 for (j=0; j<k; j++) {               // forward substitution (solves 'x' = z 'U')
   for (i=0; i<j; i++)
     y[j] -= tmp[i][j] * y[i];          // the vector 'x' is stored in the variable 'y'
   if (absv(tmp[j][j]) > close_to_zero) //   before the forward substitution, the
     y[j] /= tmp[j][j];                 //   vector z is also stored in 'y', but after
   else                                 //   the forward substitution
     y[j] /= close_to_zero;
 }
 for (j=k-1; j>=0; j--)                 // backward substituition (solves z = 'y' 'L')
   for (i=j+1; i<k; i++)
     y[j] -= tmp[i][j] * y[i];          // the vector z is stored in 'y' before
                                        //   the backward substituition
 for (i=0; i<k; i++)
   delete[] tmp[i];                     // erases the temporary matrix 'tmp'
 delete[] tmp;
}

     //--------------------------------------------------------------------------//
     //                       Output on 'fout' file stream                       //
     //--------------------------------------------------------------------------//
void full_mat::write(ofstream & fout) const
{
 int i, j;

 fout << dimension << "\n";
 for (i=0; i<dimension; i++)
   for (j=0; j<dimension; j++)
     fout << mat[i][j] << " ";
   cout << "\n";
}
     //--------------------------------------------------------------------------//
     //                      Input from 'fin' file stream                        //
     //--------------------------------------------------------------------------//
void full_mat::read(ifstream & fin)
{
 int i, j;

 fin >> i;
 create(i);
 for (i=0; i<dimension; i++)
   for (j=0; j<dimension; j++)
     fin >> mat[i][j];
}
     //--------------------------------------------------------------------------//
     //              Memory space used by 'this' (in memory units)               //
     //--------------------------------------------------------------------------//
int full_mat::mem_use() const
{
 int answer = 4;
 answer += dimension * dimension * 8;
 return(answer);
}
