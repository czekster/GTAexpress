//====================================================================================//
//                                                                                    //
//                               Function Structure                                   //
//                                                                                    //
//====================================================================================//
//  This File:   func.h                      Tool: C++                                //
//  Software:    PEPS                        Advisor: Paulof@inf.pucrs.br             //
//  Doc:                                                                              //
//====================================================================================//
//  Creation:    18/Jun/02                   by: lbrenner@inf.pucrs.br                //
//  Last Change: 18/Jun/02                   by: lbrenner@inf.pucrs.br                //
//====================================================================================//
//  This class is used to store the data obtained from SAN textual input file in the  //
// function form.                                                                     //
//====================================================================================//  

#ifndef _FUNC_H_
#define _FUNC_H_

#include "sanfile.h"

class FUNC {
  public:
    FUNC();  
    ~FUNC();
    int alloc(int _n); // alloc functions space
    int free();
    
    // input functions
    int put_name(char *_name); // put function name
    int put_exp(char *_exp); // put function expression

    // output functions
    int get_name(int i, char *_name); // get function name
    int get_name(char *_name);// get function index by name
    int get_exp(int i, char *_exp); // get function expression
    int get_exp(char *_name, char *_exp); // get function expression by name
    int get_exp(char *_exp); // get expression index
    int get_n_exp(); // get the number expressions
    int exp_value(int i, int *_value); // get the function value
    int exp_value(int i, double *_value); // the same but to double value
    int rm_cnst();
        
  private:
    int resize(); // alloc more space for functions
    int rm_cnst(int i);
        
    char **name; // function name
    char **exp; // function expression
    int ind; // total functions index
    int n; // total functions positions
}; 

#endif
