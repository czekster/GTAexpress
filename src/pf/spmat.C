//====================================================================================//
//                                                                                    //
//                              Class Sparse Matrix                                   //
//                                                                                    //
//====================================================================================//
//  This File:   spmat.C                    Language: C++  (xlC and CC)               //
//  Software:    PEPS 3.1                    Advisor: Brigitte.Plateau@imag.fr        //
//  Doc:         mat.tex                                                              //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 14/feb/00                        by: Paulo.Fernandes@imag.fr         //
//====================================================================================//
#include "peps.h"

// ------------------------------------------------------------------- Empty Constructor
sp_mat::sp_mat()
{
 dimension = 0;
 nz_number = 0;
};
// ------------------------------------------------------------------- Sized Constructor
sp_mat::sp_mat(const int n, const int d)
{
 dimension = d;
 nz_number = n;
 elements  = new rp[nz_number];
 rows      = new int[nz_number];
 columns   = new int[dimension+1];
 diag      = new int[dimension];
};
//--------------------------------------------------------------------- Copy Constructor
sp_mat::sp_mat(const sp_mat & m)
{
 int i;

 dimension = m.dimension;
 nz_number = m.nz_number;
 elements  = new rp[nz_number];
 rows      = new int[nz_number];
 columns   = new int[dimension+1];
 diag      = new int[dimension];
 for (i=0; i<nz_number;  i++) {
   elements[i] = m.elements[i];
   rows[i]     = m.rows[i];
 }
 for (i=0; i<dimension; i++) {
   columns[i]  = m.columns[i];
   diag[i]     = m.diag[i];
 }
 columns[dimension] = m.columns[dimension];
};
// -------------------------------------------------------------------------- Destructor
sp_mat::~sp_mat()
{
 if (dimension != 0) {
   delete[] elements;
   delete[] rows;
   delete[] columns;
   delete[] diag;
   dimension = 0;
   nz_number = 0;
 }
};

//------------------------------------------------------------------------ Intialization
void sp_mat::create()
{
 dimension = 0;
 nz_number = 0;
};
//------------------------------------------------------------------------- Construction
void sp_mat::create(const int n, const int d)
{
 erase();
 dimension = d;
 nz_number = n;
 if (dimension == 0)
   cerr << "Programming Error - creation of an empty matrix (#### 3010 ####)";
 else {
   elements  = new rp[nz_number];
   rows      = new int[nz_number];
   columns   = new int[dimension+1];
   diag      = new int[dimension];
 }
};
//--------------------------------------------------------------------------------- Copy
void sp_mat::copy(const sp_mat & m)
{
 int i;

 create(m.nz_number, m.dimension);
 for (i=0; i<nz_number;  i++) {
   elements[i] = m.elements[i];
   rows[i]     = m.rows[i];
 }
 for (i=0; i<dimension; i++) {
   columns[i]  = m.columns[i];
   diag[i]     = m.diag[i];
 }
 columns[dimension] = m.columns[dimension];
};
//-------------------------------------------------------------------------- Destruction
void sp_mat::erase()
{
 if (dimension != 0) {
   delete[] elements;
   delete[] rows;
   delete[] columns;
   delete[] diag;
   dimension = 0;
   nz_number = 0;
 }
};

//--------------------------------------------------------- Give the order of the matrix
int sp_mat::size() const
{
 return(dimension);
};
//--------------------------------------------------------- Give the order of the matrix
int sp_mat::od() const
{
 return(dimension);
};
//------------------------------------------------- Give the number of non-zero elements
int sp_mat::nz() const
{
 return(nz_number);
};
//----------------------------------------------------------------------------- Is Empty
bool sp_mat::is_empty() const
{
 return(dimension == 0);
};
//---------------------------------------------------------- There are negative elements
bool sp_mat::has_negatives() const
{
 ///int  ne;
 bool answer = false;

 for (int i=0; i<nz_number; i++)
   if (elements[i] < zero) {
     answer = true;
     break;
 }
 return(answer);
};
//-------------------------------------------------------------------- Give a coordenate
rp sp_mat::coord(const int i, const int j) const
{
 rp buf = zero;

 for (int k=columns[j]; k<columns[j+1]; k++)
   if (rows[k] == i) {
     buf = elements[k];
     k = columns[j+1];
   }
 return(buf);
};
//------------------------------------------------------------------------------ Scaling
void sp_mat::scale(const rp f)
{
 for (int i=0; i<nz_number; i++)
   elements[i] *= f;
};
//--------------------------------------------------- Keep only the row and columns in v
void sp_mat::reduce(const bools & v)
{
 sp_mat temp;
 int  * new_coord = new int[dimension];
 ///rp     buf;
 int    nzero = 0;
 int    dim   = 0;
 int    j, k;

 if (dimension != v.size())
   cerr << "Programming Error - wrong state space to reduce a matrix (#### 3012 ####)";
 else
   for (j=0; j<dimension; j++)
     if (v[j]) { new_coord[j] = dim;
                 dim++; }
     else      { new_coord[j] = no_coord; }
   temp.create(nz_number, dim);
   for (j=0, dim=0; j<dimension; j++) {
     temp.columns[dim] = nzero;
     if (v[j]) {
       for (k=columns[j]; k<columns[j+1]; k++)
         if (v[rows[k]]) { temp.elements[nzero] = elements[k];
                           temp.rows[nzero] = new_coord[rows[k]];
                           nzero++; }
         dim++;
     }
   }
   temp.columns[dim] = nzero;
   temp.nz_number = nzero;
   copy(temp);
};

//-------------------------------------------- Copies a san_mat eliminating the function
void sp_mat::copy_elim(const f_aprox t, const san_mat & m, const st_map * s,
                                        const ev_id e,     const aut_id a)
{
 int      j, k;          // counters for columns and nonzero elements
 int      new_nz = 0;    // new number of nonzero elements
 rp     * new_elems;     // new array for nonzero elements
 int    * new_rows;      // new array for row position of nonzero elements
 aut_st * funcs;         // table of parameters for functional elements
                         // (used only if 't' is equal to 'lu_user')
 func_id  f;             // buffer to function id of functional elements

 funcs = new aut_st[FTB::ftb.has()];
 for (k=0; k<FTB::ftb.has(); k++)
   funcs[k].create();

 create(m.nz_number, m.dimension);
 //-------------------- copie dure
 switch (m.type) {
 case Sparse   : for (k=0; k<nz_number; k++) {
                   if (m.elements[k].is_func()) {
                     f = m.elements[k].f_get();
                     if (t == lu_user)
                       if (funcs[f].size() == 0)
                         m.elements[k].estimate(e, a, m.rows[k], s, t, funcs[f]);
                       else
                         m.elements[k].f_eval(funcs[f]);
                      else
                        m.elements[k].estimate(e, a, m.rows[k], s, t);
                   }
                   elements[k] = m.elements[k].get();
                   rows[k]     = m.rows[k];
                   if (absv(elements[k]) > close_to_zero)
                     new_nz++;
                 }
                 for (j=0; j<=dimension; j++)
                   columns[j] = m.columns[j];
                 break;
 case Binary   : for (k=0; k<nz_number; k++) {
                   elements[k] = one;
                   rows[k]     = m.rows[k];
                   new_nz++;
                 }
                 for (j=0; j<=dimension; j++)
                   columns[j] = m.columns[j];
                 break;
 case Identity : for (k=0; k<nz_number; k++) {
                   elements[k] = one;
                   rows[k]     = k;
                   columns[k]  = k;
                   new_nz++;
                 }
                 columns[k]    = k;
 }
 //-------------------- new alloc
 new_elems = new rp[new_nz];
 new_rows  = new int[new_nz];
 new_nz    = 0;
 //-------------------- copie fine
 for (j=0; j<dimension; j++)
   for (k=columns[j], columns[j] = new_nz; k<columns[j+1]; k++)
     if (absv(elements[k]) > close_to_zero) {
       new_elems[new_nz] = elements[k];
       new_rows[new_nz]  = rows[k];
       new_nz++;
     }
 columns[j] = new_nz;
 nz_number = new_nz;
 delete[] elements;
 elements = new_elems;
 delete[] rows;
 rows = new_rows;

 for (k=0; k<FTB::ftb.has(); k++)
   funcs[k].erase();
 delete[] funcs;
};

//------------------------------------------------ Adds diagonal elements with value 'v'
void sp_mat::add_diagonal(const rp v)
{
 rp  * new_elem;
 int * new_rows;
 int * new_cols;
 int   j, k;
 int   nz = 0;
 bool  diag_insert;

 new_elem = new  rp[nz_number+dimension];
 new_rows = new int[nz_number+dimension];
 new_cols = new int[dimension+1];

 for (j=0; j<dimension; j++) {               // for all columns ('j')
   new_cols[j] = nz;
   diag_insert = true;
   if (columns[j] == columns[j+1]) {            // if column 'j' was empty
     new_elem[nz] = v;                             // puts a diagonal element
     new_rows[nz] = j;
     nz++;
     diag_insert = false;
   }
   for (k=columns[j]; k<columns[j+1]; k++) {   // copies all elements of the column 'j'
     if ((rows[k] > j) && diag_insert) {          // if there were no diagonal element
       new_elem[nz] = v;                             // puts a diagonal element
       new_rows[nz] = j;
       nz++;
       diag_insert = false;
     }
     else
       if (rows[k] == j) {                        // if there were a diagonal element
         new_elem[nz] = elements[k] + v;             // adds 'v' to the diagonal
         new_rows[nz] = j;
         nz++;
         diag_insert = false;
       }
     new_elem[nz] = elements[k];                 // anyway, copies the non-diagonal
     new_rows[nz] = rows[k];                     // elements of the column 'j'
     nz++;
   }
 }
 new_cols[dimension] = nz;
 delete[] elements;
 elements = new_elem;
 delete[] rows;
 rows     = new_rows;
 delete[] columns;
 columns  = new_cols;
};

     //--------------------------------------------------------------------------//
     //                               Winglet                                    //
     //--------------------------------------------------------------------------//
//----------------------------------------- Puts diagonal elements with value 'v'
//---------------------------- 'v' is the smallest value of the diagonal elements
void sp_mat::winglet()
{
 rp  * new_elem;
 int * new_rows;
 int * new_cols;
 int   j, k;
 int   nz = 0;
 bool  diag_insert;

 new_elem = new  rp[nz_number+dimension];
 new_rows = new int[nz_number+dimension];
 new_cols = new int[dimension+1];

 rp min = 0;                               // finds the smallest (algebracally)
 for (j=0; j<dimension; j++)                     // diagonal element
   for (k=columns[j]; k<columns[j+1]; k++)
     if (rows[k] == j)
       if (elements[k] < min)
         min = elements[k];
  min += 1e-3;

 for (j=0; j<dimension; j++) {               // for all columns ('j')
   new_cols[j] = nz;
   diag_insert = true;
   if (columns[j] == columns[j+1]) {            // if column 'j' was empty
     new_elem[nz] = min;                            // puts a diagonal element
     new_rows[nz] = j;
     nz++;
     diag_insert = false;
   }
   for (k=columns[j]; k<columns[j+1]; k++) {   // copies all elements of the column 'j'
     if ((rows[k] > j) && diag_insert) {          // if there were no diagonal element
       new_elem[nz] = min;                            // puts a diagonal element
       new_rows[nz] = j;
       nz++;
       diag_insert = false;
     }
     else
       if (rows[k] == j) {                        // if there were a diagonal element
         new_elem[nz] = min;                          // puts 'v' to the diagonal
         new_rows[nz] = j;
         nz++;
         diag_insert = false;
       }
     new_elem[nz] = elements[k];                 // anyway, copies the non-diagonal
     new_rows[nz] = rows[k];                     // elements of the column 'j'
     nz++;
   }
 }
 new_cols[dimension] = nz;
 delete[] elements;
 elements = new_elem;
 delete[] rows;
 rows     = new_rows;
 delete[] columns;
 columns  = new_cols;
};

     //--------------------------------------------------------------------------//
     //              It performs the product 'v_out = v_in * this'               //
     //--------------------------------------------------------------------------//
void sp_mat::left_mult(rp v_out[], const rp v_in[]) const
{
 int j, k;

 for (j=0; j<dimension; j++) {
   v_out[j] = zero;
   for (k=columns[j]; k<columns[j+1]; k++)
     v_out[j] += elements[k] * v_in[rows[k]];
 }
}

//---------------------------------------------------------------------------- Transpose
void sp_mat::transpose()
{
 rp  * new_elem;
 int * new_rows;
 int * new_cols;
 ///int i, next;
 int j, k;

 new_elem = new  rp[nz_number];
 new_rows = new int[nz_number];
 new_cols = new int[dimension+1];

 for (j=0; j<dimension+1; j++)                 // counts the elements by row
   new_cols[j] = 0;
 for (k=0; k<nz_number; k++)
   new_cols[rows[k]+1]++;
 for (j=1; j<dimension; j++)
   new_cols[j+1] += new_cols[j];

 for (j=0; j<dimension; j++)                   // copies the elements
   for (k=columns[j]; k<columns[j+1]; k++) {
     new_rows[new_cols[rows[k]]] = j;
     new_elem[new_cols[rows[k]]] = elements[k];
     new_cols[rows[k]]++;
   }
 for (j=dimension-1; j>=0; j--)
   new_cols[j+1] = new_cols[j];
 new_cols[0] = 0;

 delete[] elements;
 elements = new_elem;
 delete[] rows;
 rows     = new_rows;
 delete[] columns;
 columns  = new_cols;
};

     //--------------------------------------------------------------------------//
     //                               Tychonov                                   //
     //--------------------------------------------------------------------------//
void sp_mat::tychonov(const sp_mat & m)
{
 rp buf;
 int i, j, k;

 create(m.dimension * m.dimension, m.dimension);
 nz_number = 0;
 for (j=0; j<dimension; j++) {
   //------------------------------------------
   columns[j] = nz_number;
   //-------------------------- before diagonal
   for (i=0; i<j; i++) {
     buf = coord(j,i);
     if (buf != zero) {
       elements[nz_number] = buf;
       rows[nz_number]     = i;
       nz_number++;
     }
   }
   //-------------------------- diagonal element
   diag[j] = nz_number;
  // if (j == 0)
     buf =  1e-8; // epsilon;
 //  else
  //   buf = zero;
   for (k=0; k<dimension; k++) {
     buf += m.coord(j, k) * m.coord(j, k);
   }
   if (buf != zero) {
     elements[nz_number] = buf;
     rows[nz_number]     = j;
     nz_number++;
   }
   //-------------------------- after diagonal
   for (i=j+1; i<dimension; i++) {
     buf = zero;
     for (k=0; k<dimension; k++) {
       buf += m.coord(i, k) * m.coord(j, k);
     }
     if (buf != zero) {
       elements[nz_number] = buf;
       rows[nz_number]     = i;
       nz_number++;
     }
   }
   //------------------------------------------
 }
 columns[dimension] = nz_number;
}

     //--------------------------------------------------------------------------//
     //                             LU decomposition                             //
     //--------------------------------------------------------------------------//
     //  falta comentar e refazer o inicio (tratamento da primeira coluna)
     //--------------------------------------------------------------------------//
bool sp_mat::lu_decomp(const rp th, const reg_typ t)
{
 bool  answer = true;
 rp  * new_elem;
 int * new_rows;
 int * new_cols;
 rp  * buf;
 int   fst;        // the first nonzero elemenet of the column in 'buf'
 int i,            // points the 'buf' vector elements
     h,            // points the 'buf' vector elements
     j,            // points the columns of the original and resulting matrix
     k,            // points the nonzero elements of the original matrix
     l;            // points the nonzero elements of the resulting matrix
 rp  pivot;

 new_elem = new  rp[dimension*dimension];
 new_rows = new int[dimension*dimension];
 new_cols = new int[dimension+1];
 buf      = new  rp[dimension];
 for (i=0; i<dimension; i++)
   buf[i] = zero;

 l = 0;
 new_cols[0] = l;
 if (rows[l] != 0) {                  // if the first diagonal element is zero
   new_elem[l] = zero;                    // includes a zero at the first position
   new_rows[l] = 0;
   l++;
 }
 for (k=0; k<columns[1]; k++) {       // first column copy
   new_elem[l] = elements[k];
   new_rows[l] = rows[k];
   l++;
 }
 diag[0] = 0;

 for (j=1; j<dimension; j++) {       // for all columns (from the second to the last)
   new_cols[j] = l;

   fst = dimension;                          // expands the column 'j' in 'buf'
   for (k=columns[j]; k<columns[j+1]; k++) {
     i = rows[k];
     buf[i] = elements[k];
     if (fst > i)
       fst = i;
   }
   for (i=fst; i<j; i++)         // for the nonzero elements before the diagonal element
     if (absv(buf[i]) < th)         // eliminates the elements
       buf[i] = zero;
     else {
       if (absv(new_elem[diag[i]]) > close_to_zero) { // aproximates the inverse
         pivot  = buf[i] / new_elem[diag[i]];         // to singular matrices
       }
       else
         switch (t) {
           case no_reg    : pivot = zero;             // no regulation (aproximates)
                            break;
           case shift_reg :
           case wingl_reg :
           case tychv_reg : pivot = zero;            // if a regulation was already done
                            answer = false;             // it is a bad decomposition
                            break;
           case aprox_reg : pivot = buf[i] / epsilon; // replaces zero by 'epsilon'
                            break;
           case one_reg   : pivot = buf[i];           // replaces zero by 'one'
         }
       buf[i] = pivot;
       for (h=diag[i]+1; h<new_cols[i+1]; h++)
         buf[new_rows[h]] -= pivot * new_elem[h];
     }

   for (i=0; i<dimension; i++) {         // puts the the column 'j' back from 'buf'
     if (i == j) {                           // diagonal elements
       diag[j] = l;                             // sets the limit before the diagonal
       new_elem[l] = buf[i];                    // always includes the diagonal elements
       new_rows[l] = i;                         // (even if they are zero!!!)
       l++;
     }
     else                                    // non diagonal elements
       if (absv(buf[i]) > th) {                 // includes only nonzero elements
         new_elem[l] = buf[i];
         new_rows[l] = i;
         l++;
       }
     buf[i] = zero;                         // puts the 'buf' element as zero
   }
 }
 //===============================================================================
 // if (absv(new_elem[diag[dimension-1]]) < close_to_zero)   // replaces null last
 //   new_elem[diag[dimension-1]] = one;                   // diag. element by one
 //===============================================================================
 new_cols[dimension] = l;
 nz_number = l;
 delete[] elements;
 elements = new rp[nz_number];
 for (k=0; k<nz_number; k++)
   elements[k] = new_elem[k];
 delete[] new_elem;
 delete[] rows;
 rows = new int[nz_number];
 for (k=0; k<nz_number; k++)
   rows[k] = new_rows[k];
 delete[] new_rows;
 for (j=1; j<=dimension; j++)
   columns[j] = new_cols[j];
 delete[] new_cols;
 delete[] buf;
 return(answer);
};

     //--------------------------------------------------------------------------//
     //                  (L D^-1 U)  -  SOR-like decomposition                   //
     //--------------------------------------------------------------------------//
     //  Makes 'this' the decomposition where the lower triangule matrix is kept //
     // and the upper triangular matrix is multiplicated by the inverse od the   //
     // diagonal.                                                                //
     //                                                                          //
     // Obs.: if a diagonal element is equal to zero replaces it by an epsilon   //
     // or by an one. The choice between 'epsilon' and 'one' is expressed by the //
     // input parameter 't' (LU decomposition type).                             //
     //--------------------------------------------------------------------------//
bool sp_mat::sor_decomp(const rp th, const reg_typ t)
{
 bool   answer = true;
 rp   * buf;
 int    i, j, k;
 sp_mat temp;

 temp.create(nz_number+dimension, dimension);
 buf = new rp[dimension];
 for (i=0; i<dimension; i++)
   buf[i] = zero;

 temp.nz_number = 0;
 for (j=0; j<dimension; j++) {           // for all columns
   for (k=columns[j]; k<columns[j+1]; k++)  // fills 'buf' temporary column
     buf[rows[k]] = elements[k];
   for (i=0; i<j; i++)                      // for the upper elements of the column
     if (coord(i, i) != zero)                  // if diagonal element is nonzero
       buf[i] /= coord(i, i);                     // divides it by the diagonal element
     else                                      // if diagonal element is null 
       switch (t) {
         case no_reg    :
         case shift_reg :
         case wingl_reg :
         case tychv_reg : buf[i] = zero;          // if there is no more regulation
                          answer = false;         //   to perform it is a bad decomp.
                          break;
         case aprox_reg : buf[i] /= epsilon;      // replaces zero diagonal by 'epsilon'
                          break;
         case one_reg   : buf[i] /= one;          // replaces zero diagonal by 'one'
       }
   temp.columns[j] = temp.nz_number;
   for (i=0; i<dimension; i++) {              // dump 'buf' column into 'temp'
     if (i == j) {
       temp.diag[j] = temp.nz_number;              // diagonal elements (always stored)
       temp.elements[temp.nz_number] = buf[i];
       temp.rows[temp.nz_number] = i;
       temp.nz_number++;
     }
     else {
       if (buf[i] > th) {                          // non-diagonal elements (stored if
         temp.elements[temp.nz_number] = buf[i];   //   greater than the threshold)
         temp.rows[temp.nz_number] = i;
         temp.nz_number++;
       }
     }
     buf[i] = zero;
   }
 }
 temp.columns[j] = temp.nz_number;
 copy(temp);                             // copies 'temp' to 'this'
 return(answer);
}


     //--------------------------------------------------------------------------//
     //                               LU solution                                //
     //--------------------------------------------------------------------------//
void sp_mat::LU_solve(const rp * x, rp * y) const
{
 switch (PRF::prf.sol_type()) {
   case both_lu : lu_solve(x, y); break;
   case just_l  : l_solve(x, y);  break;
   case just_u  : u_solve(x, y);  break;
 }
}


     //--------------------------------------------------------------------------//
     //                               LU solution                                //
     //--------------------------------------------------------------------------//
//-------------------------- Solve 'x = y * this' ('this' are the Doolittle LU matrices)
//--------------------------------------------------- options finding zero in diagonal:
//------------------------------------------------------ stops the loop (after keep 'y')
//------------------------------------------------------ result is unchanged 'x[j]'
//------------------------------------------------------ result is partial   'y[j]'
//------------------------------------------------------ replace zero by one
//------------------------------------------------------ replace zero by an epsilon
void sp_mat::lu_solve(const rp * x, rp * y) const
{
 int   j;     // conter for the vector positions (matrix columns)
 int   k;     // counter for the lu matrix nonzero elements

 for (j=0; j<dimension; j++)               // initializes 'x' with 'y'
   y[j] = x[j];

 for (j=0; j<dimension; j++)               // forward substitution (solves 'y = y U')
   for (k=columns[j]; k<diag[j]; k++)                   //   the answer is here ^
     y[j] -= elements[k] * y[rows[k]];

//------------------------------------------------------ stops the loop (after keep 'y')
 for (j=dimension-1; j>=0; j--) {         // backward substituition (solves 'y = y L')
                                                         //   the answer is here ^
   if (absv(elements[diag[j]]) > close_to_zero) { // if the diagonal element is zero
     for (k=diag[j]+1; k<columns[j+1]; k++)       // stops the backward substit. loop.
       y[j] -= elements[k] * y[rows[k]];          // After the null diag. element
     y[j] /= elements[diag[j]];                   // it keeps the old values of 'y',
   }                                              // previously computed solution
   else {                                         // of 'y = y U' (forward substit.)
     break;
   }
 }

//------------------------------------------------------ result is unchanged 'x[j]'
// for (j=dimension-1; j>=0; j--) {         // backward substituition (solves 'y = y L')
//                                                         //   the answer is here ^
//   if (absv(elements[diag[j]]) > close_to_zero) { // if the diagonal element is zero
//     for (k=diag[j]+1; k<columns[j+1]; k++)       // acts as the result is unchanged
//       y[j] -= elements[k] * y[rows[k]];          // (the answer stays with the 'x[j]'
//     y[j] /= elements[diag[j]];                   // value)
//   }
//   else {
//     y[j] = x[j];
//   }
// }

//------------------------------------------------------ result is unchanged 'y[j]'
// for (j=dimension-1; j>=0; j--) {         // backward substituition (solves 'y = y L')
//                                                         //   the answer is here ^
//   if (absv(elements[diag[j]]) > close_to_zero) { // if the diagonal element is zero
//     for (k=diag[j]+1; k<columns[j+1]; k++)       // acts as the result is unchanged
//       y[j] -= elements[k] * y[rows[k]];          // (the answer stays with the 'y[j]'
//     y[j] /= elements[diag[j]];                   // value - keeps the solution
//   }                                              // performed on the forward subst.)
// }

//------------------------------------------------------ replace zero by one
// for (j=dimension-1; j>=0; j--) {         // backward substituition (solves 'y = y L')
//                                                         //   the answer is here ^
//   for (k=diag[j]+1; k<columns[j+1]; k++)          // if the diagonal element is zero
//     y[j] -= elements[k] * y[rows[k]];             // acts as if it was an one
//   if (absv(elements[diag[j]]) > close_to_zero)
//     y[j] /= elements[diag[j]];
// }

//------------------------------------------------------ replace zero by an episilon
// for (j=dimension-1; j>=0; j--) {         // backward substituition (solves 'y = y L')
//                                                         //   the answer is here ^
//   for (k=diag[j]+1; k<columns[j+1]; k++)          // if the diagonal element is zero
//     y[j] -= elements[k] * y[rows[k]];             // acts as if it was an one
//   if (absv(elements[diag[j]]) > close_to_zero)
//     y[j] /= elements[diag[j]];
//   else
//     y[j] /= epsilon;
// }

}

     //--------------------------------------------------------------------------//
     //                                L solution                                //
     //--------------------------------------------------------------------------//
//----------------------------- Solve 'x = y * this' ('this' are the Doolittle L matrix)
void sp_mat::l_solve(const rp * x, rp * y) const
{
 int j;       // conter for the vector positions (matrix columns)
 int k;       // counter for the lu matrix nonzero elements

 for (j=0; j<dimension; j++)               // initializes 'x' with 'y'
   y[j] = x[j];

 for (j=dimension-1; j>=0; j--) {          // backward substituition (solves 'y = y L')
                                                          //   the answer is here ^
   if (absv(elements[diag[j]]) > close_to_zero) { // if the diagonal element is zero
     for (k=diag[j]+1; k<columns[j+1]; k++)       // acts as the result is unchanged
       y[j] -= elements[k] * y[rows[k]];          // (the answer stays with the 'x[j]'
     y[j] /= elements[diag[j]];                   // value)
   }
   else {
     y[j] = x[j];
   }
 }
}
     //--------------------------------------------------------------------------//
     //                                U solution                                //
     //--------------------------------------------------------------------------//
//----------------------------- Solve 'x = y * this' ('this' is the Doolittle U matrix)
void sp_mat::u_solve(const rp * x, rp * y) const
{
 int j;       // conter for the vector positions (matrix columns)
 int k;       // counter for the lu matrix nonzero elements

 for (j=0; j<dimension; j++)               // initializes 'x' with 'y'
   y[j] = x[j];

 for (j=0; j<dimension; j++)               // forward substitution (solves 'y = y U')
   for (k=columns[j]; k<diag[j]; k++)                   //   the answer is here ^
     y[j] -= elements[k] * y[rows[k]];
}

//----------------------------------------------------------------------- Write (output)
void sp_mat::write(ofstream & fout) const
{
 int i;

 fout << dimension << " " << nz_number << "\n";
 for (i=0; i<nz_number;  i++) fout << elements[i] << "\n"; 
 for (i=0; i<nz_number;  i++) fout << rows[i]     << " "; fout << "\n";
 for (i=0; i<=dimension; i++) fout << columns[i]  << " "; fout << "\n";
 for (i=0; i<dimension;  i++) fout << diag[i]     << " "; fout << "\n";
};
//------------------------------------------------------------------------- Read (input)
void sp_mat::read(ifstream & fin)
{
 int dim, nzero, i;

 fin >> dim;
 fin >> nzero;
 if (dim == 0)
   cerr << "Data Error - reading an empty matrix (#### 3015 ####)";
 else {
   create(nzero, dim);
   for (i=0; i<nz_number;  i++) fin >> elements[i];
   for (i=0; i<nz_number;  i++) fin >> rows[i];
   for (i=0; i<=dimension; i++) fin >> columns[i];
   for (i=0; i<dimension;  i++) fin >> diag[i];
 }
};
     //--------------------------------------------------------------------------//
     //              Memory space used by 'this' (in memory units)               //
     //--------------------------------------------------------------------------//
int sp_mat::mem_use() const
{
 int answer = 12;
 answer += nz_number * 16;
 answer += nz_number * 4;
 answer += (dimension+1) * 4;
 answer += dimension * 4;
 return(answer);
}
