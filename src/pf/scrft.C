//====================================================================================//
//                                                                                    //
//                            Class Scratch Funtion Table                             //
//                                                                                    //
//====================================================================================//
//  This File:   scrft.C                    Language: C++  (xlC and CC)               //
//  Software:    PEPS 3.1                    Advisor: Brigitte.Plateau@imag.fr        //
//  Error Codes: 4200                                                                 //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 25/jan/00                        by: Paulo.Fernandes@imag.fr         //
//====================================================================================//
#include "peps.h"


     //--------------------------------------------------------------------------//
     //                 static variable: The Scratch Function Table              //
     //--------------------------------------------------------------------------//
scr_ft scr_ft::sft = scr_ft();

     //--------------------------------------------------------------------------//
     //          Searches for an identical function already generated            //
     //--------------------------------------------------------------------------//
     //     It returns the function entry generated as the concatenation of the  //
     // elements in 'e' using the operation 'o' (if no function matches, it      //
     // returns 'no_func').                                                      //
     //--------------------------------------------------------------------------//
func_id scr_ft::search(const elem_list & e, const oper o) const
{
 func_id answer = no_func;

 for (int i=0; i<number; i++)
   if (o == op[i])
     if (e == e_list[i]) {
       answer = i;
       break;
     }
 return(answer);
}

     //==========================================================================//
     //     The input parameters of the next five functions refer to the table   //
     // entries (function id's) of 'FTB::ftb'. The output parameter (return)     //
     // refers to the table entries of 'scr_ft::sft'.                            //
     //==========================================================================//

     //--------------------------------------------------------------------------//
     //              It includes in 'this' the function entry 'f'                //
     //--------------------------------------------------------------------------//
func_id scr_ft::sto(const func_id f)
{
 func_id   answer;        // the entry of table 'scr_ft::sft' where 't' will be
 elem_list e(1);          // the list to store the unique function 'f'

 e.tail(f);                          // puts the 'FTB::ftb' function id in the list 'e'
 answer = search(e, No_oper);        // searches if the function is already in the table
 if (answer == no_func) {            // if the function is not yet in the table
   if (number == dimension)             // if the table is full
     realloc();                            // re-sizes the table
   FTB::ftb.get(f, table[number]);      // copies the function formulae to an entry
   e_list[number].copy(e);              // stores the 'FTB::ftb' function id
   op[number] = No_oper;                //    and the operation
   answer = number;                     // returns the 'scr_ft::sft' entry used
   number++;                            // increases the number of functions
 }
 return(answer);
}

     //--------------------------------------------------------------------------//
     //    It includes the sum of the function entry 'f' and the constant 'c'    //
     //--------------------------------------------------------------------------//
func_id scr_ft::add(const func_id f, const rp c)
{
 func_id   answer; // the entry of table 'scr_ft::sft' where 't' will be
 elem_list e(2);   // the list containing the function and the constant to add

 e.tail(f);                         // puts the 'FTB::ftb' function id in the list 'e'
 e.tail(c);                         // puts the constant in the list 'e'
 answer = search(e, Plus);          // searches if the function is already in the table
 if (answer == no_func) {           // if the function is not yet in the table
   if (number == dimension)               // if the table is full
     realloc();                              // re-sizes the table
   FTB::ftb.get(f, table[number]);        // copies the function to the entry 'number'
   table[number].show();
   table[number].concat(c, Plus);         // concatenates the constant
   e_list[number].copy(e);                // stores the concatenated elements
   op[number] = Plus;                     //    and the operation
   answer = number;                       // returns the 'scr_ft::sft' entry used
   number++;                              // increases the number of functions
 }
 return(answer);
}

     //--------------------------------------------------------------------------//
     //  It includes the product of the function entry 'f' and the constant 'c'  //
     //--------------------------------------------------------------------------//
func_id scr_ft::mul(const func_id f, const rp c)
{
 func_id   answer; // the entry of table 'scr_ft::sft' where 't' will be
 elem_list e(2);   // the list containing the function and the constant to multiply

 e.tail(f);                         // puts the 'FTB::ftb' function id in the list 'e'
 e.tail(c);                         // puts the constant in the list 'e'
 answer = search(e, Times);         // searches if the function is already in the table
 if (answer == no_func) {           // if the function is not yet in the table
   if (number == dimension)               // if the table is full
     realloc();                              // re-sizes the table
   FTB::ftb.get(f, table[number]);        // copies the function to the entry 'number'
   table[number].concat(c, Times);        // concatenates the constant
   e_list[number].copy(e);                // stores the concatenated elements
   op[number] = Times;                    //   and the operation
   answer = number;                       // returns the 'scr_ft::sft' entry used
   number++;                              // increases the number of functions
 }
 return(answer);
}

     //--------------------------------------------------------------------------//
     //        It includes the sum of the 'n' function entries ('f') and         //
     //                           the constant 'c'                               //
     //--------------------------------------------------------------------------//
func_id scr_ft::add(const func_id f[], const int n, const rp c)
{
 func_id   answer;  // the entry of 'scr_ft::sft' where the function will be included
 elem_list e(n+1);  // the list containing the functions and the constant to add
 func_tree buf;     // a buffer for the functions to be concatenated

 for (int i=0; i<n; i++)             // puts all 'FTB::ftb' function id's in
   e.tail(f[i]);                     //    the list 'e'
 e.sort();                           // sorts the function id's in the list 'e'
 if (c != zero)                      // if the constant is not zero
   e.tail(c);                           // puts the constant in the list 'e'
 answer = search(e, Plus);           // searches if the function is already in the table
 if (answer == no_func) {            // if the function is not yet in the table
   if (number == dimension)             // if the table is full
     realloc();                            // re-sizes the table
   FTB::ftb.get(f[0], table[number]);   // concatenates all 'n' function trees,
   for (int i=1; i<n; i++) {            //  storing in the entry 'number'
     FTB::ftb.get(f[i], buf);
     table[number].concat(buf, Plus);
   }
   if (c != zero)                       // if the constant is not zero
     table[number].concat(c, Plus);        // concatenates the constant 'c'
   e_list[number].copy(e);              // stores the concatenated elements
   op[number] = Plus;                   //    and the operation
   answer = number;                     // returns the 'scr_ft::sft' entry used
   number++;                            // increases the number of functions
 }
 return(answer);
}
     //--------------------------------------------------------------------------//
     //      It includes the product of the 'n' function entries ('f') and       //
     //                           the constant 'c'                               //
     //--------------------------------------------------------------------------//
func_id scr_ft::mul(const func_id f[], const int n, const rp c)
{
#ifdef _PEPS_DEBUG_
  if (c == zero)
    Programming_Error("constant function creation", 4202)
#endif
 func_id   answer;  // the entry of 'scr_ft::sft' where the function will be included
 elem_list e(n+1);  // the list of SAN elements to multiply
 func_tree buf;     // a buffer to the functions to be concatenated

 for (int i=0; i<n; i++)             // puts all 'FTB::ftb' function id's in
   e.tail(f[i]);                     //    the list 'e'
 e.sort();                           // sorts the function id's in the list 'e'
 if (c != one)                       // if the constant is not one
   e.tail(c);                           // puts the constant in the list 'e'
 answer = search(e, Times);          // searches if the function is already in the table
 if (answer == no_func) {            // if the function is not yet in the table
   if (number == dimension)             // if the table is full
     realloc();                            // re-sizes the table
   FTB::ftb.get(f[0], table[number]);   // concatenates all 'n' function trees,
   for (int i=1; i<n; i++) {            //  storing in the entry 'number'
     FTB::ftb.get(f[i], buf);
     table[number].concat(buf, Times);
   }
   if (c != one)                        // if the constant is not one
     table[number].concat(c, Times);       // concatenates the constant 'c'
   e_list[number].copy(e);              // stores the concatenated elements
   op[number] = Times;                  //    and the operation
   answer = number;                     // returns the 'scr_ft::sft' entry used
   number++;                            // increases the number of functions
 }
 return(answer);
}


     //--------------------------------------------------------------------------//
     //                       Internal Function of allocation                    //
     //--------------------------------------------------------------------------//
void scr_ft::alloc()
{
 table  = new func_tree[dimension];
 e_list = new elem_list[dimension];
 op     = new oper[dimension];
 for (int i=0; i<dimension; i++) {
   table[i].create();
   e_list[i].create();
 }
}
     //--------------------------------------------------------------------------//
     //                     Internal Function of desallocation                   //
     //--------------------------------------------------------------------------//
void scr_ft::desal()
{
 for (int i=0; i<dimension; i++) {
   table[i].erase();
   e_list[i].erase();
 }
 delete[] table;
 delete[] e_list;
 delete[] op;
}
     //--------------------------------------------------------------------------//
     //                     Internal Function of re-allocation                   //
     //--------------------------------------------------------------------------//
void scr_ft::realloc()
{
#ifdef _PEPS_DEBUG_
  if ((dimension != number) || (dimension == 0))
    Programming_Error("bad relocation call", 4203)
#endif
 int         b_dimension;
 func_tree * b_table;
 elem_list * b_e_list;
 oper      * b_op;
 int         i;

 b_dimension = dimension + step;          // compute the new size of the table
 b_table  = new func_tree[b_dimension];   // allocate the new arrays
 b_e_list = new elem_list[b_dimension];
 b_op     = new oper[b_dimension];

 for (i=0; i<number; i++) {             // copies the existing entries
   b_table[i].copy(table[i]);
   b_e_list[i].copy(e_list[i]);
   b_op[i] = op[i];
 }
 for (i=number; i<b_dimension; i++) {   // initializes the non-existing entries
   b_table[i].create();
   b_e_list[i].create();
 }
 desal();                               // erases the old arrays
 dimension = b_dimension;               // re-sizes the table
 table  = b_table;                      // assigns the new arrays to 'this'
 e_list = b_e_list;
 op     = b_op;
}
