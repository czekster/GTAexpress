//====================================================================================//
//                                                                                    //
//                                Class SAN Matrix                                    //
//                                                                                    //
//====================================================================================//
//  This File:   sanmat.C                   Language: C++  (xlC and CC)               //
//  Software:    PEPS 3.1                    Advisor: Brigitte.Plateau@imag.fr        //
//  Error Codes: 3300                                                                 //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change:  7/feb/01                        by: Anne.Benoit@imag.fr             //
//====================================================================================//
#include "peps.h"

     //--------------------------------------------------------------------------//
     //                    Initialization from a full matrix                     //
     //--------------------------------------------------------------------------//
     //    The input parameter 'm' contains the all full format matrix elements  //
     // stored by columns, i.e., all elements of the first column, than all      //
     // elements of the second column, and so on.                                //
     //--------------------------------------------------------------------------//
void san_mat::create(const elem m[], const int d)
{
#ifdef _PEPS_DEBUG_
  if (d == 0)
    Programming_Error("creation of zero dimension matrix", 3303)
#endif
 int nzero = 0;     // counter for the number of nonzero elements
 int sq_d = d*d;    // square of the dimension
 int i, j, k;       // counters for rows, columns and nonzero elements

 for (k=0; k<sq_d; k++)                      // counts the number of nonzero elements
   if (m[k] != zero)
     nzero++;
 create(nzero, d, Sparse);                   // allocates structures for a sparse type
 for (k=0,nzero=0,j=0; j<dimension; j++) {   // stores the nonzero elements
   columns[j] = nzero;
   for (i=0; i<d; i++,k++)                   // for all elements of each column
     if (m[k] != zero) {                        // if its a nonzero element
       elements[nzero] = m[k];                     // stores it!
       rows[nzero]     = i;
       nzero++;
     }
   }
 columns[dimension] = nzero;
 classify();                                 // tries to change the matrix type to
}                                            //   empty, identity or binary

     //--------------------------------------------------------------------------//
     //                    Initialization from a full matrix                     //
     //--------------------------------------------------------------------------//
void san_mat::create(elem **m, const int d)
{
#ifdef _PEPS_DEBUG_
  if (d == 0)
    Programming_Error("creation of zero dimension matrix", 3303)
#endif
 int nzero = 0;     // counter for the number of nonzero elements
 int sq_d = d*d;    // square of the dimension
 int i, j, k, l;       // counters for rows, columns and nonzero elements

 for (k=0; k<d; k++)                        // counts the number of nonzero elements
   for (l=0; l<d; l++)
     if (m[k][l] != zero)
       nzero++;
// nzero = d;       
 create(nzero, d, Sparse);                   // allocates structures for a sparse type
/* for(i=0; i<d; i++){
   elements[i] = 1;
   rows[i] = i;
   columns[i] = 0;
 } 
 columns[i] = d;
*/ 
 for (l=0, j=0, nzero=0; l<d; l++) {        // stores the nonzero elements
   columns[j] = nzero;
   for (i=0; i<d; i++){                   // for all elements of each column
     if (m[i][l] != zero) {                        // if its a nonzero element
       elements[nzero] = m[i][l];                     // stores it!
       rows[nzero]     = i;
       nzero++;
     }
   }
   j++;
 }
 columns[d] = nzero;

 classify();                                 // tries to change the matrix type to
}                                            //   empty, identity or binary

     //--------------------------------------------------------------------------//
     //                    Initialization from a full matrix                     //
     //--------------------------------------------------------------------------//
// @RGP
/*
void san_mat::create(elem **m, const int d, mat_typ t)
{
#ifdef _PEPS_DEBUG_
  if (d == 0)
    Programming_Error("creation of zero dimension matrix", 3303)
#endif
 int nzero = 0;     // counter for the number of nonzero elements
 int sq_d = d*d;    // square of the dimension
 int i, j, k, l;       // counters for rows, columns and nonzero elements

 for (k=0; k<d; k++)                        // counts the number of nonzero elements
   for (l=0; l<d; l++)
     if (m[k][l] != zero)
       nzero++;

 create(nzero, d, t);                   // allocates structures

 for (l=0, j=0, nzero=0; l<d; l++) {        // stores the nonzero elements
   columns[j] = nzero;
   for (i=0; i<d; i++){                   // for all elements of each column
     if (m[i][l] != zero) {                        // if its a nonzero element
		 if (t == Sparse)
			elements[nzero] = m[i][l];                     // stores it!
       rows[nzero]     = i;
       nzero++;
     }
   }
   j++;
 }
 columns[d] = nzero;

 classify();                                 // tries to change the matrix type to
}                                            //   empty, identity or binary
*/
     //--------------------------------------------------------------------------//
     //                      There are functional elements                       //
     //--------------------------------------------------------------------------//
     // Note: only sparse matrices can have functional elements.                 //
     //--------------------------------------------------------------------------//
bool san_mat::is_functional() const
{
 int  i;
 bool answer = false;

 if (type == Sparse)                   // for all elements of a sparse matrix
   for (i=0; i<nz_number; i++)
     if (!(elements[i].is_const())) {     // if at least one functional element is found
       answer = true;                        // the answer is true
       break;
     }
 return(answer);
}
     //--------------------------------------------------------------------------//
     //     The union of the arguments of all functional elements of 'this'      //
     //             contains at least all automata but automaton 'a'             //
     //--------------------------------------------------------------------------//
     // Note: only sparse matrices can have functional elements.                 //
     //--------------------------------------------------------------------------//
bool san_mat::is_clique(const aut_id a) const
{
 int     i;
 aut_set param;             // parameters of the matrix
 aut_set buf;               // buffer for the parameters of each functional element
 bool    answer = false;

 if (type == Sparse) {
   param.create(FTB::ftb.g_aut());  // allocates the subset of matrix parameters
   param.take_out();                // initializes 'param' as an empty subset
   buf.create(FTB::ftb.g_aut());    // allocates the subset of element parameters
   for (i=0; i<nz_number; i++) {    // for all elements of a sparse matrix
     elements[i].params(buf);          // gets the the element parameters
     param.plus(buf);                  // adds to the matrix parameters
   }
   param.put_in(a);                 // includes automaton 'a' in the matrix parameters
   answer = param.is_full();        // if the matrix parameters plus automaton 'a'
 }                                  //   is a full subset the answer is 'true'
 return(answer);
}
     //--------------------------------------------------------------------------//
     //                       There are negative elements                        //
     //--------------------------------------------------------------------------//
     // Note: only sparse matrices can have negative elements.                   //
     //--------------------------------------------------------------------------//
bool san_mat::has_negatives() const
{
 bool answer = false;

 if (type == Sparse)                 // for all elements of a sparse matrix
   for (int i=0; i<nz_number; i++)
     if (elements[i] < zero) {          // searchs for a negative element
       answer = true;
       break;
     }
 return(answer);
}
     //--------------------------------------------------------------------------//
     //                       There are diagonal elements                        //
     //--------------------------------------------------------------------------//
bool san_mat::has_diag() const
{
 int  j, k;
 bool answer = false;

 switch (type) {
   case Sparse   : 
   case Binary   : for (j=0; j<dimension; j++)
                     for (k=columns[j]; k<columns[j+1]; k++)
                       if (rows[k] == j) {
                         answer = true;
                         j = dimension;     // stops the loop 'j'
                         break;             // stops the loop 'k'
                       }
                   break;
   case Identity : answer = true;
                   break;
   case Empty    : answer = false;
 }
 return(answer);
}
     //--------------------------------------------------------------------------//
     //                       The parameters of the matrix                       //
     //--------------------------------------------------------------------------//
     // Note: only sparse matrices can have functional elements.                 //
     //--------------------------------------------------------------------------//
     // Obs.: the reference set of 's' is the current number of automata.        //
     //--------------------------------------------------------------------------//
void san_mat::params(aut_set & s) const
{
 aut_set buf;                   // buffer for the parameters of each functional element

 s.create(FTB::ftb.g_aut());      // allocates the subset of matrix parameters
 s.take_out();                    // initializes 's' as an empty subset
 if (type == Sparse) {
   buf.create(FTB::ftb.g_aut());  // allocates the subset of element parameters
   for (int i=0; i<nz_number; i++) {   // for all elements of a sparse matrix
     elements[i].params(buf);             // gets its parameters
     s.plus(buf);                         // adds to the matrix parameters
   }
 }
}
     //--------------------------------------------------------------------------//
     //                       It returns the element 'i,j'                       //
     //--------------------------------------------------------------------------//
elem san_mat::coord(const int i, const int j) const
{
#ifdef _PEPS_DEBUG_
  if ((i >= dimension) || (j >= dimension))
    Programming_Error("index out of range", 3304)
#endif
 int  k;
 elem buf;

 buf = zero;
 switch (type) {
 case Sparse   : for (k=columns[j]; k<columns[j+1]; k++)
                   if (rows[k] == i) {
                     buf = elements[k];
                     break;
                   }
                 break;
 case Binary   : for (k=columns[j]; k<columns[j+1]; k++)
                   if (rows[k] == i) {
                     buf = one;
                     break;
                   }
                 break;
 case Identity : if (i == j)
                   buf = one;
                 break;
 case Empty    : buf = zero;
 }
 return(buf);
}
     //--------------------------------------------------------------------------//
     //              It returns the evaluation of the element 'i,j'              //
     //--------------------------------------------------------------------------//
     // Obs.: the reference set of 's' is the current number of automata.        //
     //--------------------------------------------------------------------------//
rp san_mat::coord(const int i, const int j, const aut_st & s) const
{
#ifdef _PEPS_DEBUG_
  if ((i >= dimension) || (j >= dimension))
    Programming_Error("index out of range", 3305)
  if (s.size() != FTB::ftb.aut())
    Programming_Error("wrong sized parameters", 3306)
#endif
 elem buf;

 buf = coord(i, j);
 return(buf.get(s));
}

     //--------------------------------------------------------------------------//
     //                          It performs 'this *= f'                         //
     //--------------------------------------------------------------------------//
     // Note: empty matrices do not change with this function.                   //
     //--------------------------------------------------------------------------//
void san_mat::scale(const rp f)
{
 int i;
 switch (type) {
   case Sparse   : for (i=0; i<nz_number; i++)         // multiplies all elements
                     elements[i] *= f;
                   break;
   case Binary   : type = Sparse;                      // transforms it in a sparse
                   elements = new elem[nz_number];     //   matrix with all elements
                   for (i=0; i<nz_number; i++)         //   equal to 'one * f'
                     elements[i] = f;
                   break;
   case Identity : type = Sparse;                      // transforms it in a sparse
                   elements = new elem[dimension];     //   matrix with all diagonal
                   rows     = new int[dimension];      //   elements equal to 'one * f'
                   columns  = new int[dimension+1];
                   for (i=0; i<dimension; i++) {
                     elements[i] = f;
                     rows[i] = i;
                     columns[i] = i;
                   }
                   columns[i] = i;
 }
}
     //--------------------------------------------------------------------------//
     // It tries to eliminate functionnal elements according to the state map 'm'//
     //--------------------------------------------------------------------------//
     // Algorithm Principle: This function tries to solve each functional        //
     // element according to the known values due to a grouping process. Each    //
     // row of the matrix will correspond to a local state of the grouped        //
     // automata. The automata that have been grouped ('known') and which local  //
     // states correspond to which grouped automaton row ('values') are taken    //
     // from the input parameter 'm' (a state map object - see 'st_map' class).  //
     //                                                                          //
     //    In the process of elimination of functions, some elements may become  //
     // zero. Such elements will no longer be stored in the data structures. The //
     // arrays 'elements' and 'rows' will be rewritten keeping only the nonzero  //
     // elements. The array columns must be recomputed according to the new      //
     // number of nonzero elements in each column. A temporary for the new array //
     // 'columns' will be created ('new_columns'). Due to the possible reduction //
     // of the number of nonzero elements, the arrays 'elements' and 'rows' can  //
     // be over dimensioned. In fact, they keep their old size but the actual    //
     // number of nonzero elements can be reduced.                               //
     //                                                                          //
     // Algorithm: - creates and initializes the objects 'known' and 'values';   //
     //            - visit all nonzero elements of the matrix:                   //
     //              - tries to evaluate functional elements;                    //
     //              - if the element still is nonzero (either still a function, //
     //              either a constant not equal to zero):                       //
     //                - copies it to the nonzero elements vector;               //
     //            - tries to change the matrix type (function 'classify');      //
     //--------------------------------------------------------------------------//
     // Note: only sparse matrices can have functional elements.                 //
     //--------------------------------------------------------------------------//
void san_mat::solve_functions(const st_map & m)
{
#ifdef _PEPS_DEBUG_
  if (m.size() != dimension)
    Programming_Error("wrong state map", 3310)
#endif
 int     j, k;                  // counters for the columns and nonzero elements
 aut_st  values;                // local state values to evaluate elements
 aut_set known;                 // original automata which states are known
 int   * new_columns;           // new array with the begining of columns
 int     nzero = 0;             // new number of nonzero elements

 if (type == Sparse) {                  // if the matrix is sparse
   new_columns = new int[dimension+1];     // allocates new array 'columns'
   known.create(FTB::ftb.aut());           // allocates subset of 'known' automata
   known.take_out();                       // initializes 'known' as an empty subset
   m.get(known);                           // gets the original automata corresponding
                                           //   to 'this' (information in the state map)
   values.create(FTB::ftb.aut());          // allocates state vector
   values.reset();                         // resets (sets to the first global state)
   for (j=0; j<dimension; j++) {           // for all columns
     new_columns[j] = nzero;                   // store the begining of the column
     for (k=columns[j]; k<columns[j+1]; k++) { // for all elements in the column 'j'
       if (elements[k].is_func()) {               // if it is a function
         m.get(rows[k], values);                     // gets the local states corresp.
                                                     //   to the element row ('rows[k]')
         elements[k].solve(known, values);           // tries to evaluate it
       }
       if (elements[k] != zero) {                 // if the element is not zero
         elements[nzero] = elements[k];              // store it!
         rows[nzero] = rows[k];
         nzero++;
       }                                          // otherwise do not store it!
     }
   }
   new_columns[dimension] = nzero;
   delete[] columns;                    // replace old 'columns' by 'new_columns'
   columns = new_columns;
   nz_number = nzero;                   // redefines the number of nonzero elements
   classify();                          // tries to change the matrix type
 }
}

     //--------------------------------------------------------------------------//
     //                     It removes the diagonal elements                     //
     //--------------------------------------------------------------------------//
     // Algorithm Principle: The arrays 'elements' and 'rows' will be rewritten  //
     // keeping only the nondiagonal nonzero elements. The array columns must be //
     // recomputed according to the new number of nonzero elements in each       //
     // column. A temporary object for the new array 'columns' will be created   //
     // ('new_columns'). Due to the possible reduction of the number of nonzero  //
     // elements, the arrays 'elements' and 'rows' can be over dimensioned. In   //
     // fact, they keep their old size but the actual number of nonzero elements //
     // can be reduced.                                                          //
     //--------------------------------------------------------------------------//
     // Note: this function tests if the number of nonzero elements becomes zero //
     // in order to mark the matrix as empty.                                    //
     //--------------------------------------------------------------------------//
void san_mat::cut_diag()
{
 int * new_columns;       // temporary for the new array 'columns'
 int   nzero = 0;         // new number of nonzero elements
 int   j, k;              // counters for columns and nonzero elements

 switch (type) {
 case Sparse   : 
 case Binary   : new_columns = new int[dimension+1];
                 for (j=0; j<dimension; j++) {             // for all columns
                   new_columns[j] = nzero;                    // sets its begining
                   for (k=columns[j]; k<columns[j+1]; k++)    // for all column elements
                     if (rows[k] != j) {                         // if its not diagonal
                       if (type == Sparse)
                         elements[nzero] = elements[k];             // store it
                       rows[nzero] = rows[k];
                       nzero++;
                     }
                 }
                 new_columns[dimension] = nzero;           // replace old 'columns'
                 delete[] columns;                         //   by 'new_columns'
                 columns = new_columns;
                 if (nzero != 0)                           // if the matrix is not empty
                   nz_number = nzero;                         // redefines the number of
                 else {                                       //   nonzero elements
                   delete[] elements;                      // else
                   delete[] rows;                             // delete all data
                   type = Empty;                              //   structures and sets
                   nz_number = 0;                             //   the matrix type to
                 }                                            //   empty
                 break;
 case Identity : type = Empty;
                 nz_number = 0;
 }
}
     //--------------------------------------------------------------------------//
     //                  It removes rows and columns not in 'v'                  //
     //--------------------------------------------------------------------------//
     // Algorithm:- for sparse and binary matrices:                              //
     //             - computes the relationship between old and new matrix rows; //
     //             - creates a temporary 'san_mat' object ('temp');             //
     //             - for all columns to be kept:                                //
     //               - if the element belongs to a row to be kept:              //
     //                 - copies the element to the 'temp' object;               //
     //             - (re)classify the 'temp' object;                            //
     //             - copies the 'temp' object to 'this';                        //
     //                                                                          //
     //           - for identity and empty matrices just change the dimension    //
     //           and number of nonzero elements;                                //
     //                                                                          //
     // Obs.: the copy of 'temp' to 'this' avoids a potential waste of space in  //
     // the arrays 'elements' and rows' due to the reduction of the number of    //
     // nonzero elements.                                                        //
     //--------------------------------------------------------------------------//
void san_mat::reduce(const bools & v)
{
#ifdef _PEPS_DEBUG_
  if (v.size() != dimension)
    Programming_Error("wrong matrix reduction", 3311)
#endif
 san_mat temp;           // temporary object to the new matrix
 int   * new_coord;      // relationship between old and new matrix rows/columns
 int     nzero = 0;      // new number of nonzero elements
 int     dim   = 0;      // new dimension of 'this'
 int     j, k;           // counters for columns and nonzero elements

 switch (type) {
 case Sparse   : 
 case Binary   : new_coord = new int[dimension];      // allocates new coordinates
                 for (j=0; j<dimension; j++)          // computes new coordinates
                   if (v[j]) {                        //   and new dimension
                     new_coord[j] = dim;
                     dim++;
                   }
                 temp.create(nz_number, dim, type);    // allocates the new matrix
                 for (j=0, dim=0; j<dimension; j++)    // for all old columns
                   if (v[j]) {                            // if the column is in 'v'
                     temp.columns[dim] = nzero;              // store its begining
                     for (k=columns[j]; k<columns[j+1]; k++) // for all column elements
                       if (v[rows[k]]) {                        // if it belongs to a
                         if (type == Sparse)                    //   row in 'v'
                           temp.elements[nzero] = elements[k];     // copy it to 'temp'
                         temp.rows[nzero] = new_coord[rows[k]];
                         nzero++;                                  // inc. new nz_number
                       }
                     dim++;                                  // inc. new columns counter
                   }
                 temp.columns[dim] = nzero;            // last columns array position
                 temp.nz_number = nzero;               // new number of nonzero elements
                 delete[] new_coord;                   // erases new coordinates array
                 temp.classify();                      // (re)classify the 'temp' object
                 copy(temp);                           // copies the 'temp' to 'this'
                 break;
 case Identity : dimension = v.has();                  // stores new dimension
                 nz_number = dimension;                // stores new nz_number
                 break;
 case Empty    : dimension = v.has();                  // stores new dimension
 }
}
     //--------------------------------------------------------------------------//
     //                 It changes the type of 'this' to 'sparse'                //
     //--------------------------------------------------------------------------//
void san_mat::declassify()
{
 int k;    // counter for the nonzero elements (rows and columns to identities)

 switch (type) {
 case Binary   : elements = new elem[nz_number];   // creates the 'elements' array
                 for (k=0; k<nz_number; k++)
                   elements[k] = one;              // and fills it with ones
                 break;
 case Identity : elements = new elem[nz_number];   // creates the 'elements' array
                 rows     = new int[nz_number];    // creates the 'rows' array
                 columns  = new int[dimension+1];  // creates the 'columns' array
                 for (k=0; k<nz_number; k++) {
                   elements[k] = one;              // and put one in each diagonal
                   rows[k]     = k;                //   element
                   columns[k]  = k;
                 }
                 columns[k] = k;
                 break;
 case Empty    : columns  = new int[dimension+1];  // creates the 'columns' array
                 for (k=0; k<dimension+1; k++)     // and informs that there are no
                   columns[k] = 0;                 //   elements
 }
}

     //--------------------------------------------------------------------------//
     //  It tries to change the type of 'this' either from 'sparse' to 'empty',  //
     //  'Identity' or 'binary', either from 'binary' to 'empty' or 'Identity'.  //
     //--------------------------------------------------------------------------//
void san_mat::classify()
{
 mat_typ new_type;                // new matrix type
 int     j, k;                    // counters for columns and nonzero elements
 bool    bin = true;              // the matrix is binary
 bool    dgn = true;              // the matrix has only diagonal elements
 bool    emp = (nz_number == 0);  // the matrix has only zero elements
 

 //---------------------------------------------- for sparse matrices that are not empty
 if ((type == Sparse) && !emp) {
   for (j=0; j<dimension; j++)
     for (k=columns[j]; k<columns[j+1]; k++) {  // looks for 
       if (elements[k] != one) {                   // an element that is not one
         bin = false;
         j = dimension;                               // exits outter loop (columns)
         break;                                       // exits inner loop (elements)
       }
       if (rows[k] != j)                           // non diagonal elements
         dgn = false;
     }
 }
 //---------------------------------------------- for binary matrices that are not empty
 if ((type == Binary) && !emp) {
   for (j=0; j<dimension; j++)
     for (k=columns[j]; k<columns[j+1]; k++)   
       if (rows[k] != j)                           // non diagonal elements
         dgn = false;
 }
 //--------------------------------------------------------- chooses the new matrix type
 if (emp)
   new_type = Empty;                        // if all elements are zero
 else
   if (bin)                                 // if all nonzero elements are one
     if (dgn && (nz_number == dimension))   //     all nonzero elements are diagonal,
       new_type = Identity;                 //     the number of nonzero elements is
     else                                   //     equal to the dimension
       new_type = Binary;                   // if all nonzero elements are one
   else
     new_type = Sparse;                     // there are nonzero elements diff. from one
 //------------------------------------------------------- erases unnecessary structures
 switch (type) {
 case Sparse   : if (new_type != Sparse) {
                   delete[] elements;
                   if (new_type != Binary) {
                     delete[] rows;
                     delete[] columns;
                   }
                 }
                 break;
 case Binary   : if (new_type != Binary) {
                   delete[] rows;
                   delete[] columns;
                 }
 }
 type = new_type;                           // sets the new type
}

 
     //--------------------------------------------------------------------------//
     //  The next five functions perform operations in the matrix which imply    //
     // changes of the functional element identifiers from the old reference set //
     // (indexes in the 'FTB::ftb' object) to a new reference set (indexes in    //
     // the 'scrt::sft'). These operations are implemented by the operators of   //
     // the class 'elem' (file 'elem.h').                                        //
     //--------------------------------------------------------------------------//

     //--------------------------------------------------------------------------//
     //                     It only modifies the func. id's                      //
     //--------------------------------------------------------------------------//
     // Note: only sparse matrices can have functional elements.                 //
     //--------------------------------------------------------------------------//
void san_mat::assign()
{
 if (type == Sparse)
   for (int k=0; k<nz_number; k++)
     if (!(elements[k].is_const()))
       elements[k].sto(elements[k]);
}
     //--------------------------------------------------------------------------//
     //           It copies 'm' to 'this' and modifies the func. id's            //
     //--------------------------------------------------------------------------//
void san_mat::assign(const san_mat & m)
{
 copy(m);
 assign();
}
     //--------------------------------------------------------------------------//
     //           It performs 'this' = tensor product of 'n' matrices            //
     //                        and modif. the func. id's                         //
     //--------------------------------------------------------------------------//
     // Algorithm Principle: Each element of 'this' will be the product of 'n'   //
     // elements (one element from each matrix 'mat[k]'). Two state vectors with //
     // the coordinates of each 'mat[k]' ('st_row' and 'st_col') are used to     //
     // index the rows and the columns of the resulting matrix ('this').         //
     //                                                                          //
     // Algorithm: - computes the dimension and the number of nonzero elements   //
     //            of the resulting matrix, gets the sizes and the type of each  //
     //            component matrix;                                             //
     //            - if the resulting matrix is not obviously an empty or        //
     //            identity matrix:                                              //
     //            - computes each element of the resulting matrix as a product  //
     //            of 'n' elements taken each one from each component matrix;    //
     //                                                                          //
     // Note: the state space of the resulting matrix is visited with 'i' and    //
     // 'j' (for rows and columns, respectively) and also with the state vectors //
     // 'st_row' and 'st_col'. The function 'next' (see class 'aut_st', file     //
     // 'autst.h') visit all combined states of a 'n' local states in a lexical  //
     // order.                                                                   //
     //--------------------------------------------------------------------------//
void san_mat::t_prod(const san_mat mat[], const int n)
{
 bool    ident = true;   // a flag to say if all the 'n' matrices are identities
 bool    empt  = false;  // a flag to say if one of the 'n' matrices is empty
 int     k;              // counter for the matrices to be multiplied ('0 <= k < n')
 aut_st  sizes(n);       // the size of each matrix to be multiplied

 elem  * vec_buf;        // a buffer to store 'n' elements to be multiplied (operands)
 elem    buf;            // a buffer for the elements of the resulting matrix
                         // 'buf' is equal to the product of 'n' elements in 'vec_buf'

 int     i, j;           // counters for rows and columns of the resulting matrix
 aut_st  st_row(n);      // state vector for the rows of the resulting matrix
 aut_st  st_col(n);      // state vector for the columns of the resulting matrix

 int     dim = 1;        // the dimension of the resulting matrix
 int     nzero = 1;      // the number of nonzero elements of the resulting matrix

 //-------------------------------------------------------------------------------------
 for (k=0; k<n; k++) {
   ident = (ident && (mat[k].type == Identity));  // all component mat. are identities
   empt  = (empt || (mat[k].type == Empty));      // one of the component mat. is empty
   dim   *= mat[k].dimension;                     // dimension of the resulting matrix
   nzero *= mat[k].nz_number;                     // nz. number of the resulting matrix
   sizes.put(k, mat[k].dimension);                // the size of each component matrix
 }
 if (empt)                                        // the resulting matrix is empty
   create(0, dim, Empty);
 else
   if (ident)                                     // the resulting matrix is an identity
     create(dim, dim, Identity);
   else {
   //-----------------------------------------------------------------------------------
     create(nzero, dim, Sparse);                  // alocates the resulting matrix
     vec_buf = new elem[n];                       // alocates the operands vector
     nzero = 0;
     st_col.reset();
     for (j=0; j<dimension; j++) {                // for all columns of the result. mat.
       columns[j] = nzero;
       st_row.reset();
       for (i=0; i<dimension; i++) {                 // for all rows of the result. mat.
         for (k=0; k<n; k++)
           vec_buf[k] = mat[k].coord(st_row.st(k),st_col.st(k));  // gets the operands
         buf.mul(vec_buf, n);                                     // multiplies it
         if (buf != zero) {                                       // if is not zero
           elements[nzero] = buf;                                    // store it!
           rows[nzero] = i;
           nzero++;
         }
         st_row.next(sizes);                         // go to next row
       }
       st_col.next(sizes);                        // go to next column
     }
     columns[dimension] = nzero;
     nz_number = nzero;
     delete[] vec_buf;
   }
}
     //--------------------------------------------------------------------------//
     //              It performs 'this = tensor sum of 'n' matrices              //
     //                        and modif. the func. id's                         //
     //--------------------------------------------------------------------------//
     // Algorithm Principle: Each element of 'this' will be:                     //
     //   - either one of the 'mat[k]' elements or zero (non diagonal elements)  //
     //   - either the sum of 'n' elements (one from each 'mat[k]' matrix).      //
     //   Two state vectors with the coordinates of each 'mat[k]' ('st_row' and  //
     // 'st_col') are used to index the rows and the columns of the resulting    //
     // matrix ('this').                                                         //
     //                                                                          //
     // Algorithm: - computes the dimension and the number of nonzero elements   //
     //            of the resulting matrix, gets the sizes and the type of each  //
     //            component matrix;                                             //
     //            - if the resulting matrix is not obviously an empty or        //
     //            identity matrix:                                              //
     //               - computes each element of the resulting matrix;           //
     //                                                                          //
     // Note: the state space of the resulting matrix is visited with 'i' and    //
     // 'j' (for rows and columns, respectively) and also for the state vectors  //
     // 'st_row' and 'st_col'. The function 'next' (see class 'aut_st', file     //
     // 'autst.h') visit all combined states of a 'n' local states in a lexical  //
     // order.                                                                   //
     //--------------------------------------------------------------------------//
void san_mat::t_sum(const san_mat mat[], const int n)
{
 bool    ident = true;   // a flag to say if all the 'n' matrices are identities
 bool    empt  = false;  // a flag to say if all of the 'n' matrices are empty
 int     k;              // counter for the matrices to be multiplied ('0 <= k < n')
 aut_st  sizes(n);       // the size of each matrix to be multiplied

 elem  * vec_buf;        // a buffer to store 'n' elements to be added (operands)
 elem    buf;            // a buffer for the elements of the resulting matrix
                         // 'buf' is equal to the product of 'n' elements in 'vec_buf'

 int     i, j;           // counters for rows and columns of the resulting matrix
 aut_st  st_row(n);      // state vector for the rows of the resulting matrix
 aut_st  st_col(n);      // state vector for the columns of the resulting matrix

 int     dim = 1;        // the dimension of the resulting matrix
 int     nzero = 0;      // the number of nonzero elements of the resulting matrix

 bool    loc;            // the element 'i,j' represents a local move
 int     mark;           // the matrix that moves (if 'loc' is 'true')

 //-------------------------------------------------------------------------------------
 for (k=0; k<n; k++) {
   ident = (ident && (mat[k].type == Identity));  // all component mat. are identities
   empt  = (empt  && (mat[k].type == Empty));     // all component mat. are empty
   dim   *= mat[k].dimension;                     // dimension of the resulting matrix
   sizes.put(k, mat[k].dimension);                // the size of each component matrix
 }
 for (k=0; k<n; k++)                                      // estimated number of 
   nzero += (mat[k].nz_number * dim) / mat[k].dimension;  //   nonzero elements
 //-------------------------------------------------------------------------------------
 if (empt)                                        // the resulting matrix is empty
   create(0, dim, Empty);
 else
   if (ident)                                     // the resulting matrix is an identity
     create(dim, dim, Identity);
   else {
     //---------------------------------------------------------------------------------
     create(nzero, dim, Sparse);                  // allocates the resulting matrix
     vec_buf = new elem[n];                       // allocates the array of operands
     nzero = 0;
     st_col.reset();
     for (j=0; j<dimension; j++) {                // for all columns
       columns[j] = nzero;
       st_row.reset();
       for (i=0; i<dimension; i++) {                 // for all elements in the columns
         if (j == i) {                                  // if it's a diagonal element
           for (k=0; k<n; k++)                             // adds 'n' elements
             vec_buf[k] = mat[k].coord(st_row.st(k),st_col.st(k));   // one from each
           buf.add(vec_buf, n);                                      // component matrix
         }
         else {                                         // if it's a nondiagonal element
           for (k=0, loc=false; ((k<n) && !(loc)); k++)    // verifies if it represents
             if (st_row.st(k) != st_col.st(k)) {           //   the transition of a
               mark = k;                                   //   single automaton
               loc = true;
             }
           for (k=mark+1; k<n; k++)                        // searches for other
             if (st_row.st(k) != st_col.st(k))             //   local transitions
               loc = false;
                                                           // if only the 'mark'-th
           if (loc)                                        //   moves
             buf.sto(mat[mark].coord(st_row.st(mark),st_col.st(mark)));  // gets its
           else                                                          // element
             buf = zero;                                   // otherwise, gets zero
         }
         if (buf != zero) {                             // if the computed element is
           elements[nzero] = buf;                       //   not zero, store it!
           rows[nzero] = i;
           nzero++;
         }
         st_row.next(sizes);                            // next element in the column
       }
       st_col.next(sizes);                           // next column
     }
     columns[dimension] = nzero;
     nz_number = nzero;
     delete[] vec_buf;
   }
}
     //--------------------------------------------------------------------------//
     //                It performs 'this' += sum of '2n' matrices                //
     //                        and modif. the func. id's                         //
     //--------------------------------------------------------------------------//
     //   This function adds to 'this' (containing the local transition matrix)  //
     // the synchronizing matrices of 'n' events. Since each event has to two    //
     // matrices, this function performs the addition of '1+(2*n)' matrices, the //
     // first matrix - 'this' - storing the result.                              //
     //                                                                          //
     // Obs.: This function is used when a 'n' synchronized events become local  //
     // to a grouped automaton.                                                  //
     //--------------------------------------------------------------------------//
void san_mat::add(const san_mat matp[], const san_mat matm[], const int n)
{
#ifdef _PEPS_DEBUG_
  for (int x=0; x<n; x++)
    if ((dimension != matp[x].dimension) || (dimension != matm[x].dimension))
      Programming_Error("adding matrices with different sizes", 3312)
#endif
 san_mat temp;              // a temporary object for the new (resulting) matrix
 elem    buf;               // a buffer for each calculated element
 elem  * vec_buf;           // a buffer to store '2n+1' elements to be added (operands)
 int     nzero = nz_number; // the number of nonzero elements of the resulting matrix
 int     i, j;              // counters of the resulting matrix rows and columns
 int     k;                 // counter for the events ('0 <= k < n')
 int     l;                 // counter to access the 'vec_buf' ('0 <= l < 2n+1')

 if (n == 0)                            // trivial case: there nothing to add
   for (k=0; k<nz_number; k++)
     elements[k].sto(elements[k]);
 else {                                 // normal case: perform the sum
   for (k=0; k<n; k++)                                // estimates the nz. number
     nzero += matp[k].nz_number + matm[k].nz_number;
   vec_buf = new elem[(2*n)+1];                       // allocates the buffer to add
   temp.create(nzero, dimension, Sparse);             // allocates the temporary matrix
   nzero = 0;
   for (j=0; j<dimension; j++) {                      // for all columns
     temp.columns[j] = nzero;
     for (i=0; i<dimension; i++) {                       // for all column elements
       vec_buf[0] = coord(i, j);
       for (k=0, l=1; k<n; k++) {                           // fills the 'vec_buf'
         vec_buf[l] = matp[k].coord(i,j); l++;
         vec_buf[l] = matm[k].coord(i,j); l++;
       }
       buf.add(vec_buf, l);                                 // adds
       if (buf != zero) {                                   // if it is not zero 
         temp.elements[nzero] = buf;
         temp.rows[nzero] = i;                                 // store it!
         nzero++;
       }
     }
   }
   temp.columns[dimension] = nzero;
   temp.nz_number = nzero;
   delete[] vec_buf;
   copy(temp);
 }
}
     //--------------------------------------------------------------------------//
     //                        Print on 'fout' file stream                       //
     //--------------------------------------------------------------------------//
void san_mat::print(ofstream & fout) const
{
 int i, j, k;

 switch (type) {
 case Sparse   : fout << "sparse matrix ('X' for constant elements, 'F' for functions)\n";
                 for (i=0; i<dimension; i++) {
                   fout << "| ";
                   for (j=0; j<dimension; j++) {
                     if ( (coord(i,j).get() != zero) || (coord(i,j).is_func()) )
                       if (coord(i,j).is_func())
                         fout << "F ";
                       else
                         fout << "X ";
                     else
                       fout << "0 ";
                   }
                   fout << "|\n";
                 }
                 fout << "(row)    value    (original value)\n";
                 for (k=0; k<nz_number; k++) {
                   fout << "(" << rows[k] << ") ";
                   elements[k].print(fout);
                 }
		 break;
 case Binary   : fout << "binary matrix:\n";
                 for (i=0; i<dimension; i++) {
                   fout << "| ";
                   for (j=0; j<dimension; j++) {
                     fout << coord(i,j).get() << " ";
                   }
                   fout << "|\n";
                 }
		 break;
 case Identity : fout << dimension << "X" << dimension << " identity matrix\n";
		 break;
 case Empty    : fout << dimension << "X" << dimension << " empty matrix\n";
		 break;
 default       : fout << "Programming Error - unexpected matrix type";
 }
}

     //--------------------------------------------------------------------------//
     //                       Output on 'fout' file stream                       //
     //--------------------------------------------------------------------------//
void san_mat::write(ofstream & fout) const
{
 int j, k;

 fout << int(type) << " " << dimension << " " << nz_number << "\n";
 if (type == Sparse)
   for (k=0; k<nz_number; k++)
     elements[k].write(fout);
 if ((type == Sparse) || (type == Binary)) {
   for (k=0; k<nz_number; k++)
     fout << rows[k] << " ";
   fout << "\n";
   for (j=0; j<=dimension; j++)
     fout << columns[j] << " ";
   fout << "\n";
 }
}
     //--------------------------------------------------------------------------//
     //                       Output on cout                                     //
     //--------------------------------------------------------------------------//
void san_mat::write() const
{
 int j, k;

 cout << int(type) << " " << dimension << " " << nz_number << "\n";
 if (type == Sparse)
   for (k=0; k<nz_number; k++)
     elements[k].write();
 if ((type == Sparse) || (type == Binary)) {
   for (k=0; k<nz_number; k++)
     cout << rows[k] << " ";
   cout << "\n";
   for (j=0; j<=dimension; j++)
     cout << columns[j] << " ";
   cout << "\n";
 }
}
     //--------------------------------------------------------------------------//
     //                      Input from 'fin' file stream                        //
     //--------------------------------------------------------------------------//
void san_mat::read(ifstream & fin)
{
 int typ, j, k;

 fin >> typ;
 fin >> j;
 fin >> k;
 create(k, j, mat_typ(typ));
 if (type == Sparse)
   for (k=0; k<nz_number; k++)
     elements[k].read(fin);
 if ((type == Sparse) || (type == Binary)) {
   for (k=0; k<nz_number; k++)
     fin >> rows[k];
   for (j=0; j<=dimension; j++)
     fin >> columns[j];
 }
}
     //--------------------------------------------------------------------------//
     //              Memory space used by 'this' (in memory units)               //
     //--------------------------------------------------------------------------//
int san_mat::mem_use() const
{
 int answer = 12;
 if (type == Sparse)
   answer += nz_number * 16;
 if ((type == Sparse) || (type == Binary)) {
   answer += nz_number * 4;
   answer += (dimension+1) * 4;
 }
 return(answer);
}

      //--------------------------------------------------------------------------//
      //                    Internal Function of allocation                       //
      //--------------------------------------------------------------------------//
void san_mat::alloc()
{
 if (type == Sparse)
   elements  = new elem[nz_number];
 if ((type == Sparse) || (type == Binary)) {
   rows      = new m_coord[nz_number];
   columns   = new m_coord[dimension+1];
 }
}
      //--------------------------------------------------------------------------//
      //                Internal Function of tranfer contents                     //
      //--------------------------------------------------------------------------//
void san_mat::trans(const san_mat & m)
{
 int j, k;

 if (type == Sparse)
   for (k=0; k<nz_number; k++)
     elements[k] = m.elements[k];
 if ((type == Sparse) || (type == Binary)) {
   for (k=0; k<nz_number; k++)
     rows[k]     = m.rows[k];
   for (j=0; j<=dimension; j++)
     columns[j]  = m.columns[j];
 }
}
      //--------------------------------------------------------------------------//
      //                  Internal Function of desallocation                      //
      //--------------------------------------------------------------------------//
void san_mat::desal()
{
 if (type == Sparse)
   delete[] elements;
 if ((type == Sparse) || (type == Binary)) {
   delete[] rows;
   delete[] columns;
 }
}

     //--------------------------------------------------------------------------//
     //      It returns the line index of the next nonzero element, or "-1" if   //
     //  there is no more nonzero elements							             //
     //--------------------------------------------------------------------------//
int san_mat::get_elem_lin(const int k)
{
 int nLin = -1;

 if (k < nz_number)		// If there is more nonzero elements
 {
	 switch (type)
	 {
	 case Sparse   :
	 case Binary   :
		 nLin = rows[k];
		 break;
	 case Identity :
		 nLin = k;
		 break;
	 case Empty    :
		 break;
	 }
 }

 return(nLin);
}

     //--------------------------------------------------------------------------//
     //      It returns the line index of the next nonzero element, or "-1" if   //
     //  there is no more nonzero elements							             //
     //--------------------------------------------------------------------------//
int san_mat::get_elem_col(const int k)
{
 int nCol = -1;

 if (k < nz_number)		// If there is more nonzero elements
 {
	 switch (type)
	 {
	 case Sparse   :
	 case Binary   :
		 nCol = 0;
		 while ( ((k>columns[nCol]) && (k>=columns[nCol+1])) || (columns[nCol] == columns[nCol+1]) )
			 nCol++;
		 break;
	 case Identity :
		 nCol = k;
		 break;
	 case Empty    :
		 break;
	 }
 }

 return(nCol);
}

     //--------------------------------------------------------------------------//
     //                       It returns the next nonzero element, or zero if    //
     //  there is no more nonzero elements							             //
     //--------------------------------------------------------------------------//
rp san_mat::get_elem_value(const int k, const aut_st & s) const
{
 rp rpValue = 0;
 
 if (k < nz_number)		// If there is more nonzero elements
 {
	 switch (type)
	 {
	 case Sparse   :
		 //cout << "SPARE.\n";
	     	 rpValue = elements[k].get(s);
             /*
             cout << endl;
             for (int jj = 0; jj < s.dimension; jj++) {
                cout << " " << s.val(jj);
             }
             cout << " rpValue = " << rpValue << " k = " << k << endl;
             */
		 break;
	 case Binary   :
		 //cout << "BINARY.\n";
		 rpValue = 1;
		 break;
	 case Identity :
		 //cout << "IDENTITY.\n";
		 rpValue = 1;
		 break;
	 case Empty    :
		 //cout << "EMPTY.\n";
		 break;
	 }
 }

 return(rpValue);
}

rp san_mat::evaluation(int k) {
    return (elements[k].get());
}
