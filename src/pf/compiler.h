//====================================================================================//
//                                                                                    //
//                           PEPS Compiler Declarations                               //
//                                                                                    //
//====================================================================================//
//  This File:   compiler.h                 Language: C++  (xlC and CC)               //
//  Software:    PEPS 3.1                    Advisor: Brigitte.Plateau@imag.fr        //
//  Error Codes: 7000                                                                 //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 30/jun/98                        by: Paulo.Fernandes@imag.fr         //
//====================================================================================//
//   This package contains the functions to compile (read textual files) of SAN       //
// descriptors, Tables of functions and Reachability Functions.                       //
//                                                                                    //
//   The three main functions perform the translation of the following structures:    //
//                                                                                    //
// +--------------+--------------+--------------------------------------------------+ //
// | Textual file | Compact File |                  Contents                        | //
// +--------------+--------------+--------------------------------------------------+ //
// |              |              | SAN Descriptor (model type, number of automata,  | //
// |  <name>.des  |  <name>.dsc  |   number of synch. events, size of each aut.     | //
// |              |              |   and transition matrices)                       | //
// +--------------+--------------+--------------------------------------------------+ //
// |              |              | Tables of functions (size of each aut., number   | //
// |  <name>.tft  |  <name>.ftb  |   of functions and reverse polish notation of    | //
// |              |              |   each function)                                 | //
// +--------------+--------------+--------------------------------------------------+ //
// |              |              | Integration Functions                            | //
// |  <name>.inf  |  <name>.ftb  |                                                  | //
// |              |              |                                                  | //
// +--------------+--------------+--------------------------------------------------+ //
// |              |              | Reachability Function (size of each aut. and     | //
// |  <name>.fct  |  <name>.rss  |   reverse polish notation of the reachability    | //
// |              |              |   function)                                      | //
// +--------------+--------------+--------------------------------------------------+ //
// |              |              | Dictionaire for external identifiers (name of    | //
// |  <name>.dic  |  <name>.dct  |   each automaton, each local state, each general | //
// |              |              |   function and each integration function)        | //
// +--------------+--------------+--------------------------------------------------+ //
//                                                                                    //
//====================================================================================//

void Compile_Descriptor(peps_name & t_name, peps_name & i_name);

void Compile_Function_Table(peps_name & t_name, peps_name & i_name);

void Compile_Integration_Function(peps_name & t_name, peps_name & i_name);

void Compile_Reacheable_SS(peps_name & t_name, peps_name & i_name);

void Compile_Dictionary(peps_name & t_name, peps_name & i_name);

//====================================================================================//
//   Four additional functions perform the compilation of particular structures       //
// inside the previous files:                                                         //
//                                                                                    //
// +-----------------+----------------+---------------------------------------------+ //
// |    Structure    |    Location    |                Contents                     | //
// +-----------------+----------------+---------------------------------------------+ //
// |  Square Matrix  |   .des files   | square matrices composed by real numbers    | //
// |                 |                |   and function identifiers                  | //
// +-----------------+----------------+---------------------------------------------+ //
// |  Function Tree  | .tft and .fct  | functions composed by operators and         | //
// |                 |     files      |   operands in reverse polish notation       | //
// +-----------------+----------------+---------------------------------------------+ //
// | Automata Sizes  | .des, .tft and | number of automata and the size of each     | //
// | Function Domain |   .fct files   |   automata                                  | //
// +-----------------+----------------+---------------------------------------------+ //
// |    Comments     | .des, .tft and | any text (even with line breaks) ended by   | //
// |                 |   .fct files   |   the special character '}'                 | //
// +-----------------+----------------+---------------------------------------------+ //
//                                                                                    //
//====================================================================================//

bool Read_SAN_Matrix(ifstream & fin, int order, san_mat & m);

bool Read_Function_Tree(ifstream & fin, func_tree & f);

bool Read_Aut_Sizes(ifstream & fin, aut_st & s);

bool Read_Comments(ifstream & fin);

//====================================================================================//
//                                                                                    //
//   General Rules for the Compilation Functions                                      //
//                   (imposed by the use of file streams I/O - 'ifstream' file types) //
//                                                                                    //
//    - all lexical tokens are separated by blank spaces;                             //
//    - line breaks are lexically equivalent to blank spaces;                         //
//    - the number of blank spaces (since if its not zero) is not important;          //
//    - comments are enclosed by accolades ' { <text> } ' which must be separated by  //
//         blank spaces;                                                              //
//    - nested comments are not allowed;                                              //
//    - comments can appear anywhere blank spaces can appear;                         //
//    - the sequence of the reserved words and all other lexical structures is        //
//         mandatory;                                                                 //
//                                                                                    //
//====================================================================================//
