//====================================================================================//
//                                                                                    //
//                            Class Scratch Funtion Table                             //
//                                                                                    //
//====================================================================================//
//  This File:   scrft.h                    Language: C++  (xlC and CC)               //
//  Software:    PEPS 3.1                    Advisor: Brigitte.Plateau@imag.fr        //
//  Error Codes: 4200                                                                 //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 28/may/97                        by: Paulo.Fernandes@imag.fr         //
//====================================================================================//
//   This class is used for a single static object: 'scr_ft::sft'. The Scratch        //
// Funtion Table stores the functions during the descriptor conversions (automata     //
// grouping and descriptor normalization - CND). This structure is a buffer for the   //
// generation of a new function table.                                                //
//                                                                                    //
//   The functions of the descriptor are stored in the function table ('FTB::ftb').   //
// When a conversion of the descriptor is done, new functions must be generated.      //
// These new functions are temporarily stored in a "scratch table". The size of this  //
// table cannot be calculated before the descriptor conversion process is done, so    //
// each inclusion of the new function tests if the current size of the table          //
// table (variable 'number') has reach the limit (variable 'dimension'). In such a    //
// case, the table size is increased of 'step' entries, where 'step' is the initial   //
// size of the table. Note that resizing the table implies a relocation of all table  //
// contents, which can have a considerable computation cost.                          //
//                                                                                    //
//   This table contains in the array fields 'e_list' and 'oper' the list of the SAN  //
// elements used to generate the new function and the concatenation operation done    //
// (performing the grouping is done step by step, so that only one operation - sum or //
// product - is done at each step. It is a 'plus' when agregating a tensor sum of     //
// matrices - local terms - or 'times' when agregating a tensor product). These       //
// informations are used to avoid the generation of identical functions. The formalae //
// ('func_tree' object) of each new function is stored in the array field 'table'.    //
//====================================================================================//

class scr_ft
{
public:
 inline scr_ft();                                                   // Empty Constructor
 inline scr_ft(const int dim);                                      // Sized Constructor
 inline ~scr_ft();                                                         // Destructor

 inline void create();                                            // Empty Intialization
 inline void create(const int dim);                               // Sized Intialization
 inline void create(FTB & ftb);                                    // Copy Intialization
 inline void erase();                                                     // Destruction

 inline bool is_empty() const;                // The current number of functions is zero
 inline bool is_full() const;               // There is no more places for new functions
 inline int  size() const;                            // The maximum number of functions
 inline int  has() const;                             // The current number of functions

 inline func_id put(const func_tree & t);     // It Includes function 't' into the table
 inline void    get(const func_id f, func_tree & t) const;

                                   // It copies the function located in entry 'f' to 't'

        //--------------------------------------------------------------------------//
        //          Searches for an identical function already generated            //
        //--------------------------------------------------------------------------//
        //     It returns the function entry generated as the concatenation of the  //
        // elements in 'e' using the operation 'o' (if no function matches, it      //
        // returns 'no_func').                                                      //
        //--------------------------------------------------------------------------//
        func_id search(const elem_list & e, const oper o) const;

        //==========================================================================//
        //     The input parameters of the next five functions refer to the table   //
        // entries (function id's) of 'FTB::ftb'. The output parameter (return)     //
        // refers to the table entries of 'scr_ft::sft'.                            //
        //==========================================================================//
        func_id sto(const func_id f);    // It includes in 'this' the function entry 'f'

        func_id add(const func_id f, const rp c);
                   // It includes the sum of the function entry 'f' and the constant 'c'
        func_id mul(const func_id f, const rp c);
               // It includes the product of the function entry 'f' and the constant 'c'

        func_id add(const func_id f[], const int n, const rp c);
                                              // It includes the sum of the 'n' function
                                                   // entries ('f') and the constant 'c'
        func_id mul(const func_id f[], const int n, const rp c);
                                          // It includes the product of the 'n' function
                                                   // entries ('f') and the constant 'c'

        //--------------------------------------------------------------------------//
        //                          Static variable                                 //
        //--------------------------------------------------------------------------//
        static scr_ft sft;

        //--------------------------------------------------------------------------//
        //                          Data Structures                                 //
        //--------------------------------------------------------------------------//
private: int         dimension; // the maximum number of functions
         int         number;    // the current number of functions
         int         step;      // the number of functions to add if 'dimension<number'
         func_tree * table;     // the table of function trees
         elem_list * e_list;    // the elements used to generate the function
         oper      * op;        // the operation used to generate the function

      //--------------------------------------------------------------------------//
      //                           Internal Functions                             //
      //--------------------------------------------------------------------------//
         void alloc();                            // Internal Function of allocation
         void desal();                         // Internal Function of desallocation
         void realloc();                       // Internal function of re-allocation
};
