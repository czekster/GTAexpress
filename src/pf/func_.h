//==============================================================================//
//                                                                              //
//                              Function Class                                  //
//                                                                              //
//==============================================================================//
//  This File:   func.h                   Tool: C++                             //
//  Software:    PEPS                     Advisor: Brigitte.Plateau@imag.fr     //
//  Doc:                                                                        //
//==============================================================================//
//  Creation:    30/Aug/05                by: Leonardo.Brenner@imag.fr          //
//  Last Change: 30/Aug/05                by: Leonardo.Brenner@imag.fr          //
//==============================================================================//
//  This class is used to store and manipulate the function obtained from SAN   //
// textual input file.                                                          //
//==============================================================================//  
#ifndef _FUNC__H_
#define _FUNC__H_

#include "peps.h"

//==============================================================================//
// operations                                                                   //
//==============================================================================//
/*
enum oper       { Plus , Minus , Times , Slash ,     // function tokens operations
                  Mini , Maxi  , Mod   , Div   ,
                  Eq   , Neq   , Gret  , Less , Geq  , Leq   ,
                  And  , Or    , Xor   , Imp  , Dimp , Not   ,
                  Aut  , Sts   , Std   , Cnst , No_oper };
*/

//==============================================================================//
//  Errors in compilation process                                               //
//==============================================================================//
/*
enum comp_errors { err_7001 ,     // no space between signs
                   err_7002 ,     // incomplete "min" or "max" sign
                   err_7003 ,     // incomplete "not-equal" sign
                   err_7004 ,     // incomplete "and" sign
                   err_7005 ,     // incomplete "summation" definition
                   err_7006 ,     // incomplete "or" or "xor" sign
                   err_7007 ,     // incomplete "equal" sign
                   err_7008 ,     // incomplete automaton state or reward definition
                   err_7009 ,     // invalid function definition
                   err_7010 ,     // non-contiguous function identifiers
                   err_7011 ,     // incomplete function definition
                   err_7012 ,     // rows too short (misplaced ";")
                   err_7013 ,     // rows too long (misplaced ";")
                   err_7014 ,     // rows missing (misplaced "]")
                   err_7015 ,     // too many rows (misplaced "]")
                   err_7016 ,     // invalid number of automata
                   err_7017 ,     // invalid automaton size
                   err_7018 ,     // bad function definition
                   err_7019 ,     // missing functions
                   err_7020 ,     // unexpected character or end of file
                   err_7021 ,     // invalid function domain
                   err_7022 ,     // missing ":" in rewards definition
                   err_7023 ,     // incomplete double implication sign
                   err_7024 ,     // 
                   err_0000 };    // no error found!
*/

//============================================================================//
//   Token of function trees                                                  //
//============================================================================//
//struct token { oper op; double val; int beg; int end; };
        // a function token (operation, parameter, begining and ending automata)

class FUNC_ {
  public:
    FUNC_();                                                       // constructor
    ~FUNC_();                                                       // destructor
    
    // put and get procedures to expression and identifiers
    int  put_id(char *_id);  
    int  build_exp(char *_id); 
    int  put_exp(char *_id, char *_exp);  
    int  get_id(char *_id);
    void get_name(int _func, char *_id);
    int  get_exp(char *_id, char *_exp);
    void get_exp(int _func, char *_exp);
    int  get_exp_to_file(int _func, char *_exp);
    int  get_n_func();
    int  has_identical(char *_exp);

   // put procedures to elements and operators
    void put_elem(char *_elem); 
    void put_elem();   
    void put_op(char *_op); 
    void cat_elem(char *_elem);
    
    // procedures to evaluations
    int is_func(char *_id);
    int is_const(char *_id);
    int is_descr(char *_id);
    int check_rate(char *_id);
    int check_reach(char *_id);
    int eval(char *_id);
    int eval(char *_id, int _aut, int _stt);
    int eval(char *_id, int _aut, int _stt, int _sttd);

    void print();

  private:
    int create_func_tree(int _i);
    void erase_func_tree();
    int eval_func(int _i, int aut, int sts, int std);

    void resize();                            // alloc space for more functions
    void resize_stack();
     
    char **id;                                           // function identifier
    char **expr;                                         // function expression
    int    ind;                                         // used functions index
    int    n;                                      // total functions positions
    
    char **func;                     // Vector of strings to store the function
    char **op;          // Vector of strings to store temporarily the operators
    int    ind_func;                        // Index to control the func vector
    int    ind_op;                            // Index to control the op vector
    int    n_func;                           // total stack functions positions
    int    n_op;                             // total stack operators positions

    int    dimension;                               // maximum number of tokens
    int    last;                                    // current number of tokens
    int    mark;         // pointed token (only used to read the list contents)
    token *list;                                                  // the tokens

}; 
#endif
