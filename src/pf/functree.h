//====================================================================================//
//                                                                                    //
//                              Class Function Tree                                   //
//                                                                                    //
//====================================================================================//
//  This File:   functree.h                 Language: C++  (xlC and CC)               //
//  Software:    PEPS 3.1                    Advisor: Brigitte.Plateau@imag.fr        //
//  Error Codes: 4100                                                                 //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 23/feb/02                        by: Paulo.Fernandes@imag.fr         //
//====================================================================================//
//    An object of this class is a representation of a function which MUST BE defined //
// on the product state space. It MUST have a value even for nonrecheable states.     //
// This restriction is implementation dependent and could be removed by a test        //
// preceeding each evaluation.                                                        //
//                                                                                    //
//    A function tree is a list of tokens representing a function stored using the    //
// reverse polish notation. The variable 'dimension' is the maximum number of tokens. //
// The current number of tokens is stored in the variable 'last'. The access to the   //
// tokens is encapsulated. A private variable ('mark') is used to read the list.      //
//                                                                                    //
//    Each token is composed of an operation and its parameter ('struct token'). The  //
// field 'op' represents the operation and the fields 'val','beg' and 'end' represent //
// the parameters. The non-used parameters are set to zero (the need of parameters is //
// specified by the field 'op').                                                      //
//                                                                                    //
//    For example, the function which returns the sum of two constants is composed of //
// three tokens: one for the each constant (two tokens with operation 'cnst') and one //
// token with operation 'plus' (see more examples below).                             //
//                                                                                    //
//    The reverse polish notation allows the function evaluation using a stack. Each  //
// token pushes or pops values in the stack. The operations arguments are the values  //
// (one or two) on the 'top' of the stack. The valid tokens are:                      //
//                                                                                    //
// Arithmetic Tokens:       'plus' , 'minus' , 'times' and 'slash'                    //
// =================                                                                  //
//    These operations perform repectively the sum, the subtraction, the product and  //
// the division of the two values on the top of the stack (two pops). The result is   //
// stored on the top of the stack (one push).                                         //
//                                                                                    //
// Min-Max Tokens:          'min' and 'max'                                          //
// ==============                                                                     //
//    These operations replace the two values on the top of the stack by the smallest //
// (respectively the largest) of the values (two pops and one push).                  //
//                                                                                    //
// Comparison Tokens:       'eq' , 'neq' , 'gret' , 'less' , 'geq' and 'leq'          //
// =================                                                                  //
//    These operations perform a comparison (respectively equal, not equal, greater   //
// than, less than, greater or equal than, less or equal than) between the two values //
// values on the top of the stack (two pops). If the comparison is successfull a      //
// logical result (1.0 or 0.0) is stored on the top of the stack (one push).          //
//                                                                                    //
// Logical Tokens:          'and' , 'or' , 'xor' , 'imp' , 'dimp' , 'not'             //
// ==============                                                                     //
//    The operations 'and', 'or' and 'xor' perform respectively the logical product,  //
// the logical sum and the exclusive or operation of the two values on the top of the //
// stack (two pops). The result is stored on the top of the stack (one push). In      //
// addtion, the operation 'imp' performs the implication of the value in below the    //
// top of the stack to the value at the top of the stack (two pops), puting the       //
// result at the top of stack (one push). The 'dimp' operation performs the double    //
// implication of the two values of the top of the stack (two pops) storing the       //
// result in the top of the stack (one push). The operation 'not' replaces the value  //
// on the top of the stack by its logical inversion (one pop and one push). The       //
// result of the logical operations are always 1.0 or 0.0, respectively 'true' or     //
// 'false'.                                                                           //
//                                                                                    //
// Automata State Tokens:   'st(beg)' , 'cs_st(val,beg,end)'                          //
// =====================                                                              //
//    The operation 'st' performs the storage of the internal id of the local state   //
// of the automaton 'beg' on the top of the stack (one push).                         //
//    The operation 'cs_st' counts how many automata in the range 'beg..end' have     //
// their current state equal to 'val'. The result is stored on the top of the stack   //
// (one push).                                                                        //
//                                                                                    //
// Automata Reward Tokens:  'rw(beg)' , 's_rw(beg,end)'      , 'p_rw(beg,end)' ,      //
// ======================               'cs_rw(val,beg,end)' , 'cp_rw(val,beg,end)'   //
//                                                                                    //
//    The operation 'rw' performs the storage of the reward of the current state of   //
// the automaton 'beg' (the parameter) on the top of the stack (one push).            //
//    The operation 's_rw' adds the rewards of all automata in the range 'beg..end'.  //
// The result is stored on the top of the stack (one push).                           //
//    The operation 'p_rw' multiplies the rewards of all automata in the range        //
// 'beg..end'. The result is stored on the top of the stack (one push).               //
//    The operation 'cs_rw' adds the rewards of all automata in the range 'beg..end'  //
// that the current state is equal to 'val'. The result is stored on the top of the   //
// stack (one push).                                                                  //
//    The operation 'p_rw' multiplies the rewards of all automata in the range        //
// 'beg..end' that the current state is equal to 'val'. The result is stored on the   //
// top of the stack (one push).                                                       //
//                                                                                    //
// Constant Token:          'cnst(val)'                                               //
// ==============                                                                     //
//    The operation 'cnst' performs the storage of a constant value ('val') on the    //
// top of the stack (one push).                                                       //
//                                                                                    //
//    General Principles:                                                             //
//                                                                                    //
//      - The reverse polish notation allows the function evaluation using a stack;   //
//      - The head of the list of tokens is located on position 0;                    //
//      - The tail of the list of tokens is located on position 'last'-1;             //
//      - All evaluations perform the operations expressed in the tokens starting     //
// from the head of the list;                                                         //
//      - The result of an evaluation is always a real value (type 'rp' in PEPS), in  //
// fact all variables in a function tree are real;                                    //
//      - Integer variables, like the automata identifiers or automata states, are    //
// also stored in a 'rp';                                                             //
//      - Booleans variables are also coded as 'rp' (0.0 is 'false' and 1.0 is        //
//  'true'). Other values are also considered as 'true', but the logical operators    //
//  always return 1.0 when the evaluation is 'true' and 0.0 otherwise;                //
//      - If the stack has more than one single value after the last operation is     //
// executed, the function is not well defined.                                        //
//                                                                                    //
// +-----------------------------------+--------------------------------------------+ //
// |       Examples of Function        |            Function Tree Object            | //
// +-----------------------------------+--------------------------------------------+ //
// | The state of the automaton 2      | st(2)  cnst(4)  gret                       | //
// | is greater than 4                 |                                            | //
// +-----------------------------------+--------------------------------------------+ //
// | The sum of the state of the       | st(0)  st(3)  plus  cnst(6)  less          | //
// | aut. 0 and 3 is less than 6       |                                            | //
// +-----------------------------------+--------------------------------------------+ //
// | The number of the automata (from  | cs_st(1,0,5)  cnst(7)  leq                 | //
// | the first to the sixth - 0 to 5)  |                                            | //
// | is less or equal than 7           |                                            | //
// +-----------------------------------+--------------------------------------------+ //
// | The function returns the reward   | rw(2)  cnst(3.6)  slash                    | //
// | of the current state of automaton |                                            | //
// | 2 divide by 3.6                   |                                            | //
// +-----------------------------------+--------------------------------------------+ //
// | The minimum between the reward of | rw(2)  rw(3)  min  rw(4)  min              | //
// | the state of automata 2, 3 and 4  |                                            | //
// +-----------------------------------+--------------------------------------------+ //
// | The function returns 3.4 if the   | cnst(3.4)  st(1)  cnst(3)  eq  times       | //
// | state of aut. 1 is equal to 3     |                            cnst(5.9)  plus | //
// | and returns 5.9 otherwise         |                                            | //
// +-----------------------------------+--------------------------------------------+ //
// | The product of the rewards of the | cp_rw(4,3,5) p_rw(0,2) plus                | //
// | automata in the state 4 (from     |                                            | //
// | fourth to sixth automata - 3 to 5)|                                            | //
// | plus the product of the rewards of|                                            | //
// | the first three (0 to 2) automata.|                                            | //
// +-----------------------------------+--------------------------------------------+ //
// | The average reward of the first   | s_rw(0,4) cnst(5) slash                    | //
// | five automata (aut. 0 to 4).      |                                            | //
// +-----------------------------------+--------------------------------------------+ //
// Note: a function tree object without the operators 'st', 'cs_st', 'rw', 's_rw',    //
// 'p_rw', 'cs_rw' and 'cp_rw' is constant, i.e., it returns always the same result.  //
// Such objects are used to code formal parameters of a SAN model. Those objects are  //
// destroyed (their references in the descriptor matrices are replaced by constants)  //
// when the SAN model is grouped or when a normalized descriptor (CND) is created.    //
//====================================================================================//

//====================================================================================//
//   Token of function trees                                                          //
//====================================================================================//
//struct token { oper op; rp val; int beg; int end; };
                // a function token (operation, parameter, begining and ending automata)
#include "peps.h"

class func_tree
{
public:
 inline func_tree();                                                // Empty Constructor
 inline func_tree(const int dim);                                   // Sized Constructor
 inline func_tree(const func_tree & t);                              // Copy Constructor
 inline ~func_tree();                                                      // Destructor

 inline void create();                                            // Empty Intialization
 inline void create(const int dim);                               // Sized Intialization
 inline void copy(const func_tree & t);                  // Copy and Sized Intialization
 inline void erase();                                                     // Destruction

 inline int  size() const;                               // The maximum number of tokens
 inline int  has() const;                                // The current number of tokens
 inline bool is_empty() const;                   // The current number of tokens is zero
        bool well_def(const bool verbose) const;
    // It verifies if 'this' is well defined ('verbose' cries out causes error messages)

        void param(aut_set & s) const;     // It fills 's' with the parameters of 'this'
        rp   eval(const aut_st & p) const;   // It evaluates 'this' for global state 'p'

        void tail(const token & t);                 // It puts 't' at the tail of 'this'
        void head(const token & t);                 // It puts 't' at the head of 'this'

 inline void reset();                             // 'mark' points to the head of 'this'
 inline void next();                        // 'mark' points to the next token of 'this'

 inline oper get_op() const;  // It returns the operation of the token pointed by 'mark'
 inline rp   get_val() const;   // It returns the operand of the token pointed by 'mark'
 inline int  get_beg() const;     // It returns the 'beg' of the token pointed by 'mark'
 inline int  get_end() const;     // It returns the 'end' of the token pointed by 'mark'


        //--------------------------------------------------------------------------//
        //     The next four functions perform concatenation of function trees.     //
        //--------------------------------------------------------------------------//
        // The concatenation operations allowed between a function and a constant   //
        // are the binary operations with associative property, namely:             //
        //    - 'plus' , 'minus' , 'times', 'slash', 'min', 'max', 'and' and 'or';  //
        //                                                                          //
        // The concatenation between two functions can be done to all binary        //
        // operations, namely:                                                      //
        //    - 'plus' , 'minus' , 'times', 'slash', 'min', 'max', 'and', 'or',     //
        //      'xor' , 'imp' , 'dimp' , 'eq' , 'neq' , 'gret' , 'less' , 'geq'     //
        //      and 'leq';                                                          //
        //--------------------------------------------------------------------------//
        void concat(const rp v, const oper o);
                           // It concatenates 'this' with constant 'v' and operation 'o'
        void concat(const func_tree & t, const oper o);
                // It concatenates two function trees ('this' and 't') and operation 'o'

        void create_cat(const func_tree & t, const rp v, const oper o);
             // It creates 'this' as the concatenation of 't' with 'v' and operation 'o'
        void create_cat(const func_tree & t1, const func_tree & t2, const oper o);
           // It creates 'this' as the concatenation of 't1' with 't2' and operation 'o'

        void show() const;              // It writes the function on the standard output
        void print(ofstream & fout) const;                // Print on 'fout' file stream
        void write(ofstream & fout) const;               // Output on 'fout' file stream
        void read(ifstream & fin);                       // Input from 'fin' file stream
        int  mem_use() const;                  // Memory space used by 'this' (in bytes)
        rp compile(const aut_st & p, const func_id f) const;
        //--------------------------------------------------------------------------//
        //                          Data Structures                                 //
        //--------------------------------------------------------------------------//
private: int     dimension;       // maximum number of tokens
         int     last;            // current number of tokens
         int     mark;            // pointed token (only used to read the list contents)
         token * list;            // the tokens
};
