#include "grammar_.h"

#ifndef _ERROR_MSGS__H_
#define _ERROR_MSGS__H_

/* //already defined in lb/error_msgs.h

// Domain Semantic Errors - CODE 100
#define Parser_100 "Identifier already defined"
#define Parser_101 "Domain has not an increasing order"
#define Parser_102 "was not declared"
#define Parser_103 "can not be evaluated in this moment. Try use a constant function"


// Function Semantic Errors - CODE 200
#define Parser_200 "Identifier already defined"
#define Parser_201 "does not occurs in all automata. Please verify that your function takes this into account"
#define Parser_202 "is not an automaton or a state identifier"
*/
#define Parser_203 "is a domain and it can not be used here"
#define Parser_204 "is not an automaton identifier"
#define Parser_205 "is not a replicated automaton identifier"
#define Parser_206 "is not a state identifier"
#define Parser_207 "can not be evaluated int this moment. Try use a constant function or a domain"
#define Parser_208 "is not a function or a domain identifier"
#define Parser_209 "A domain was already declared in this interval"
#define Parser_210 "It has elements that can not be used in reachability function"
#define Parser_211 "The function has elements that can not be used in result function"

// Event Semantic Errors - CODE 300
#define Parser_300 "was not declared"
#define Parser_301 "has elements that can not be used in event rate"
#define Parser_302 "Identifier already defined"
#define Parser_303 "is not a domain identifier"

// Network Semantic Errors - CODE 600
#define Parser_600 "is not a domain identifier"
#define Parser_601 "is not a state of this automaton"
#define Parser_602 "is not a domain nor a function identifier"
#define Parser_603 "function value must be constant"
#define Parser_604 "is not a function identifier"
#define Parser_605 "is not a description function. It can not be evaluated"
#define Parser_606 "has elements that can not be used in a probability function"
#define Parser_607 "is not a replicated state. You can not use \"++\", \"--\", \"==\""
#define Parser_608 "is not a declared event"

#endif
