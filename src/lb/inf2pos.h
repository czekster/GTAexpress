//============================================================================//
//                                                                            //
//                 INFix TO POStfix translator Structure                      //
//                                                                            //
//============================================================================//
//  This File: inf2pos.h             Language: C++                            //
//  Software:  PEPS 3.1                Advisor: Brigitte.Plateau@imag.fr      //
//============================================================================//
//  Creation: 12/05/97                 by: Roberta.Jungblut-Hessel@imag.fr    //
//============================================================================//
// PEPS works with expressions in the reverse polish notation, while the user //
// works with infix operators. This module converts the expressions at the    //
// infix operator form to postfix operator form.                              //
// Infix operator: the arithmetic operator is placed between the operands on  //
// which it performs some operation.                                          //
// Postfix operator: the arithmetic operator is placed after the operands     //
// upon which it performs some operation.                                     //
// Reverse polish notation: language in which all operators are postfix       //
// operators.                                                                 //
//============================================================================//  
#ifndef _INF2POS_H_
#define _INF2POS_H_

#include "sanfile.h"

class inf2pos
{  
  public:
    inf2pos();                                           // Constructor
    ~inf2pos();                                           // Destructor
    void reset();                                // To reset the indexes
    void put_elem(char *elem);   // To put an element in the func vector
    void put_elem();   
         // To put an operator (in the correct order) in the func vector
    void put_op(char *o);   // To store temporarily an operator, this is 
                            // necessary to make translation  
    void get_func(char *fnc); 
            // To return the function already translated to the exterior
 private:
    char *func[FUNCTION];     // Vector of strings to store the function
    char *op[FUNCTION];  
                 // Vector of strings to store temporarily the operators
    int ind_func;                    // Index to control the func vector
    int ind_op;                        // Index to control the op vector
};


#endif
