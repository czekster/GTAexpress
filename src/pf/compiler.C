//====================================================================================//
//                                                                                    //
//                          PEPS Compiler Implementation                              //
//                                                                                    //
//====================================================================================//
//  This File:   compiler.C                 Language: C++  (xlC and CC)               //
//  Software:    PEPS 2002                   Advisor: Brigitte.Plateau@imag.fr        //
//  Error Codes: 7000                                                                 //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 21/oct/02                        by: paulof@inf.pucrs.br             //
//====================================================================================//
#include <stdlib.h>               // standard conversions (atof)
#include "peps.h"                 // basic definitions

//====================================================================================//
//  Errors in compilation process                                                     //
//====================================================================================//
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
//====================================================================================//
//  Read_Function_Tree                                                                //
//====================================================================================//
//                                                                                    //
//   This function reads from the input file 'fin' a function composed by a sequence  //
// of valid tokens (see list below). The tokens must be separated by blank spaces.    //
// the last token MUST be ';' (end function). Comments ('{ ... }' can appear in any   //
// place, but their separators ('{' and '}') MUST also be separated by blank spaces.  //
// See examples of valid functions in the file 'functree.h' (class 'functree').       //
//                                                                                    //
//   Valid tokens:                                                                    //
//                                                                                    //
//    '+'            'Plus'         addition                                          //
//    '-'            'Minus'        subtraction                                       //
//    '*'            'Times'        multiplication                                    //
//    '/'            'Slash'        division                                          //
//    'min'          'Mini'         minimum between two values                        //
//    'max'          'Maxi'         maximum between two values                        //
//    '=='           'Eq'           equality                                          //
//    '!=' '<>' '><' 'Neq'          inequality                                        //
//    '>'            'Gret'         greater than                                      //
//    '<'            'Less'         less than                                         //
//    '>=' '=>'      'Geq'          greater than or equal                             //
//    '<=' '=<'      'Leq'          less than or equal                                //
//    '&&'           'And'          logical 'and'                                     //
//    '||'           'Or'           logical 'or'                                      //
//    '^^'           'Xor'          logical exclusive 'or'                            //
//    '->'           'Imp'          logical implication                               //
//    '<->'          'Dimp'         logical double implication                        //
//    '!'            'Not'          logical 'not'                                     //
//    '@x'           'St(x)'        local state of automaton 'x'                      //
//    '@$x y z'      'Cs_st(x,y,z)' number of automata in 'y..z' with local state 'x' //
//    '%x'           'Rw(x)'        reward of local state of automaton 'x'            //
//    '%$ y z'       'S_rw(y,z)'    sum of rewards of local state of aut. in 'y..z'   //
//    '%# y z'       'P_rw(y,z)'    product of rewards of state of aut. in 'y..z'     //
//    '%Sx y z'      'Cs_rw(x,y,z)' same as 's_rw(y,z)' but only for aut. with st. 'x'//
//    '%Px y z'      'Cp_rw(x,y,z)' same as 'p_rw(y,z)' but only for aut. with st. 'x'//
//    'x'            'Cnst(x)'      constant value 'x'                                //
//    ';'            'end'          end function                                      //
//    '{ ... }'                     comments                                          //
//                                                                                    //
//   Obs.: The function must express a valid reverse polish notation expression. This //
// compilation returns 'true' if the sequence of tokens represent a semantically      //
// correct function (test performed by the function 'well_def' of the class 'functree'//
//  - see file 'functree.h').                                                         //
//====================================================================================//
bool Read_Function_Tree(ifstream & fin, func_tree & f)
{
 //token  line[2048];
 token  line[20000]; //just to support more size to the functions...
 char   buf[64];
 int    tok = 0;
 int    i;
 bool   cont = true;
 comp_errors error = err_0000;

 f.erase();
 fin >> buf;
 do {
   switch (buf[0]) {
   case '!' : if (buf[1] == '=') { line[tok].op = Neq;                      //  '!='
                                   line[tok].val = zero;
                                   if (buf[2] == '\0') { tok++; 
                                                         fin >> buf; }
                                   else error = err_7001; }
              else if (buf[1] == '\0') { line[tok].op = Not;                // '!'
                                         line[tok].val = zero;
                                         if (buf[2] == '\0') { tok++; 
                                                               fin >> buf; }
                                         else error = err_7001; }
                   else error = err_7003;
              break;
   case '%' : switch (buf[1]) {
              case '#' : line[tok].op = P_rw;                               // '%# y z'
                         if (buf[2] != '\0') error = err_7001;
                         else { line[tok].val = 0;
                                fin >> buf;
                                line[tok].beg = int(atof(buf));
                                fin >> buf;
                                line[tok].end = int(atof(buf));
                                tok++;
                                fin >> buf; }
                         break;
              case '$' : line[tok].op = S_rw;                               // '%$ y z'
                         if (buf[2] != '\0') error = err_7001;
                         else { line[tok].val = 0;
                                fin >> buf;
                                line[tok].beg = int(atof(buf));
                                fin >> buf;
                                line[tok].end = int(atof(buf));
                                tok++;
                                fin >> buf; }
                         break;
              case '0' : case '1' : case '2' :
              case '3' : case '4' : case '5' :
              case '6' : case '7' : case '8' :
              case '9' : line[tok].op = Rw;                                 // '%x'
                         line[tok].val = atof(buf+1);
                         tok++;
                         fin >> buf;
                         break;
              case 'P' : line[tok].op = Cp_rw;                              // '%Px y z'
                         if (buf[2] == '\0') error = err_7008;
                         else { line[tok].val = atof(buf+2);
                                fin >> buf;
                                line[tok].beg = int(atof(buf));
                                fin >> buf;
                                line[tok].end = int(atof(buf));
                                tok++;
                                fin >> buf; }
                         break;
              case 'S' : line[tok].op = Cs_rw;                              // '%Sx y z'
                         if (buf[2] == '\0') error = err_7008;
                         else { line[tok].val = atof(buf+2);
                                fin >> buf;
                                line[tok].beg = int(atof(buf));
                                fin >> buf;
                                line[tok].end = int(atof(buf));
                                tok++;
                                fin >> buf; }
                         break;
              default  : err_7001; }
              break;
   case '&' : if (buf[1] == '&') { line[tok].op = And;                      // '&&'
                                   line[tok].val = zero;
                                   if (buf[2] == '\0') { tok++; 
                                                         fin >> buf; }
                                   else error = err_7001; }
              else error = err_7004;
              break;
   case '*' : if (buf[1] == '\0') { line[tok].op = Times;                   // '*'
                                    line[tok].val = zero;
                                    tok++;
                                    fin >> buf; }
              else error = err_7001;
              break;
   case '+' : if (buf[1] == '\0') { line[tok].op = Plus;                    // '+'
                                    line[tok].val = zero;
                                    tok++;
                                    fin >> buf; }
              else { line[tok].op  = Cnst;                                  // 'x'
                     line[tok].val = atof(buf);
                     tok++;
                     fin >> buf; }
              break;
   case '-' : if (buf[1] == '\0') { line[tok].op = Minus;                   // '-'
                                    line[tok].val = zero;
                                    tok++;
                                    fin >> buf; }
              else if (buf[1] == '>') { line[tok].op = Imp;                 // '->'
                                        line[tok].val = zero;
                                        if (buf[2] == '\0') { tok++; 
                                                              fin >> buf; }
                                        else error = err_7001; }
                   else { line[tok].op  = Cnst;                             // '-x'
                          line[tok].val = atof(buf);
                          tok++;
                          fin >> buf; }
              break;
   case '.' : line[tok].op  = Cnst;                                         // '.x'
              line[tok].val = atof(buf);
              tok++;
              fin >> buf;
              break;
   case '/' : if (buf[1] == '\0') { line[tok].op = Slash;                   // '/'
                                    line[tok].val = zero;
                                    tok++;
                                    fin >> buf; }
              else error = err_7001;
              break;
   case '0' : case '1' : case '2' : case '3' : case '4' :                   // 'x'
   case '5' : case '6' : case '7' : case '8' :
   case '9' : line[tok].op  = Cnst;
              line[tok].val = atof(buf);
              tok++;
              fin >> buf;
              break;
   case ';' : if (buf[1] == '\0') { cont = false; }                         // ';'
              else error = err_7001;
              break;
   case '<' : switch (buf[1]) {
              case '\0' : line[tok].op = Less;                              // '<'
                          line[tok].val = zero;
                          tok++;
                          fin >> buf;
                          break;
              case '-'  : if (buf[2] == '>') { line[tok].op = Dimp;         // '<->'
                                               line[tok].val = zero;
                                               if (buf[3] == '\0') { tok++; 
                                                                     fin >> buf; }
                                               else error = err_7001; }
                          else error = err_7023;
                          break;
              case '='  : line[tok].op = Leq;                               // '<='
                          line[tok].val = zero;
                          if (buf[2] == '\0') { tok++; 
                                                fin >> buf; }
                          else error = err_7001;
                          break;
              case '>'  : line[tok].op = Neq;                               // '<>'
                          line[tok].val = zero;
                          if (buf[2] == '\0') { tok++; 
                                                fin >> buf; }
                          else error = err_7001;
                          break;
              default   : error = err_7001; }
              break;
   case '=' : switch (buf[1]) {
              case '\0' : error = err_7007;
                          break;
              case '='  : line[tok].op = Eq;                                // '=='
                          line[tok].val = zero;
                          if (buf[2] == '\0') { tok++; 
                                                fin >> buf; }
                          else error = err_7001;
                          break;
              case '<'  : line[tok].op = Leq;                               // '=<'
                          line[tok].val = zero;
                          if (buf[2] == '\0') { tok++; 
                                                fin >> buf; }
                          else error = err_7001;
                          break;
              case '>'  : line[tok].op = Geq;                               // '=>'
                          line[tok].val = zero;
                          if (buf[2] == '\0') { tok++; 
                                                fin >> buf; }
                          else error = err_7001;
                          break;
              default   : error = err_7001; }
              break;
   case '>' : switch (buf[1]) {
              case '\0' : line[tok].op = Gret;                              // '>'
                          line[tok].val = zero;
                          tok++;
                          fin >> buf;
                          break;
              case '='  : line[tok].op = Geq;                               // '>='
                          line[tok].val = zero;
                          if (buf[2] == '\0') { tok++; 
                                                fin >> buf; }
                          else error = err_7001;
                          break;
              case '<'  : line[tok].op = Neq;                               // '><'
                          line[tok].val = zero;
                          if (buf[2] == '\0') { tok++; 
                                                fin >> buf; }
                          else error = err_7001;
                          break;
              default   : error = err_7001; }
              break;
   case '@' : switch (buf[1]) {
              case '$' : line[tok].op = Cs_st;                              // '@$x y z'
                         if (buf[2] == '\0') error = err_7008;
                         else { line[tok].val = atof(buf+2);
                                fin >> buf;
                                line[tok].beg = int(atof(buf));
                                fin >> buf;
                                line[tok].end = int(atof(buf));
                                tok++;
                                fin >> buf; }
                         break;
              case '0' : case '1' : case '2' :
              case '3' : case '4' : case '5' :
              case '6' : case '7' : case '8' :
              case '9' : line[tok].op = St;                                 // '@x'
                         line[tok].val = atof(buf+1);
                         tok++;
                         fin >> buf;
                         break;
              default  : error = err_7008; }
              break;
   case '^' : if (buf[1] == '^') { line[tok].op = Or;                       // '^^'
                                   line[tok].val = zero;
                                   if (buf[2] == '\0') { tok++;
                                                         fin >> buf; }
                                   else error = err_7001; }
              else error = err_7006;
              break;
   case 'm' : if (buf[3] != '\0')
                error = err_7001;
              else
                if ((buf[1] == 'i') && (buf[2] == 'n')) {                   // 'min'
                  line[tok].op = Mini;
                  line[tok].val = zero;
                  tok++;
                  fin >> buf;
                }
                else
                  if ((buf[1] == 'a') && (buf[2] == 'x')) {                // 'max'
                    line[tok].op = Maxi;
                    line[tok].val = zero;
                    tok++;
                    fin >> buf;
                  }
                     else 
                       error = err_7002;
              break;
   case '{' : do { fin >> buf; } while (strcmp(buf, "}")); fin >> buf;      // '{ ... }'
              break;
   case '|' : if (buf[1] == '|') { line[tok].op = Or;                       // '||'
                                   line[tok].val = zero;
                                   if (buf[2] == '\0') { tok++;
                                                         fin >> buf; }
                                   else error = err_7001; }
              else error = err_7006;
              break;
   default  : error = err_7020;
   }
 } while ((error == err_0000) && (cont));
 switch (error) {
 case err_7001 : cerr << "Function compilation: no space between signs - token readed: '"
                      << buf << "'\n";
                 break;
 case err_7002 : cerr << "Function compilation: incomplete 'min' or 'max' sign\n";
                 break;
 case err_7003 : cerr << "Function compilation: incomplete 'not-equal' sign\n";
                 break;
 case err_7004 : cerr << "Function compilation: incomplete 'and' sign\n";
                 break;
 case err_7005 : cerr << "Function compilation: incomplete 'summation' definition\n";
                 break;
 case err_7006 : cerr << "Function compilation: incomplete 'or' or 'xor' sign\n";
                 break;
 case err_7007 : cerr << "Function compilation: incomplete 'equal' sign\n";
                 break;
 case err_7008 : cerr << "Function compilation: incomplete automaton state or reward"
                      << " definition\n";
                 break;
 case err_7020 : cerr << "Function Compilation: unexpected character or end of file\n";
                 break;
 case err_7023 : cerr << "Function Compilation: incomplete double implication sign\n";
                 break;
 case err_0000 : f.create(tok);
                 for (i=0; i<tok; i++)
                   f.tail(line[i]);
                 break;
 default       : Programming_Error("unexpected compilation error", 7000)
 }
 return(f.well_def(true) && !f.is_empty());
}

//====================================================================================//
//  Read_SAN_Matrix                                                                   //
//====================================================================================//
//                                                                                    //
//   This function reads from the input file 'fin' a square matrix composed by real   //
// numbers and function identifiers. The valid elements are numeric constants and     //
// function identifiers begining with the letter 'f' (capital 'F' is also valid) and  //
// an integer number. No blank space or comment can appear between the letter 'f' and //
// the function identifier. The elements of a same row are separated by blank spaces. //
// the rows are separated by ';' (row end). The end of the last row is marked by ']'  //
// (matrix end). Comments can appear in any place where a blank space can appear.     //
//                                                                                    //
//     Example:         ' -3  0  2  1 ;  0 -4  2  2 ;  1  0 -5  4 ; f1  0 f2 f0 ] '   //
//                                                                                    //
//                                                                                    //
//   Valid tokens:                                                                    //
//                                                                                    //
//    'x'            constant value 'x'                                               //
//    'Fx' 'fx'      function 'x'                                                     //
//    ';'            row end                                                          //
//    ']'            matrix end                                                       //
//    '{ ... }'      comments                                                         //
//                                                                                    //
// Obs.: the matrices must be square (the matrix size is the number elements in the   //
// first row.                                                                         //
//====================================================================================//
bool Read_SAN_Matrix(ifstream & fin, int order, san_mat & m)
{
 bool     answer = false;
 char     buf[32];
 int      square_order = order*order;
 int      i = 0, j = 0, index = 0;
 bool     cont = true;
 elem   * mat = new elem[square_order];
 comp_errors error = err_0000;

 m.erase();
 fin >> buf;
 do {
   switch (buf[0]) {
   case '+' : case '-' : case '.' : case '0' : case '1' : case '2' :        // 'x'
   case '3' : case '4' : case '5' : case '6' : case '7' : case '8' :
   case '9' : mat[index] = rp(atof(buf)); i++;             fin >> buf;
              break;
   case ';' : if (i < order) error = err_7012;                              // ';'
                        else { i = 0; j++; };              fin >> buf;
              break;
   case 'f' : mat[index] = func_id(atof(buf+1)); i++;      fin >> buf;      // 'fx'
              break;
   case ']' : if (i < order) error = err_7012;                              // ']'
                        else if (j < order-1) error = err_7014;
                                         else cont = false;
              break;
   case 'F' : mat[index] = func_id(atof(buf+1)); i++;      fin >> buf;      // 'F'
              break;
   case '{' : do { fin >> buf; } while (strcmp(buf, "}")); fin >> buf;      // '{ ... }'
              break;
   default  : error = err_7020;
   }
   if (j > order) error = err_7015;
   else if (i > order) error = err_7013;
        else index = (i*order) + j;
 } while ((error == err_0000) && (cont));
 switch (error) {
 case err_7012 : cerr << "Matrix Compilation: rows too short (misplaced ';')\n";
                 break;
 case err_7013 : cerr << "Matrix Compilation: rows too long (misplaced ';')\n";
                 break;
 case err_7014 : cerr << "Matrix Compilation: rows missing (misplaced ']')\n";
                 break;
 case err_7015 : cerr << "Matrix Compilation: too many rows (misplaced ']')\n";
                 break;
 case err_7020 : cerr << "Matrix Compilation: unexpected character or end of file\n";
                 break;
 case err_0000 : m.create(mat, order);
                 answer = true;
                 break;
 default       : Programming_Error("unexpected compilation error", 7000)
 }
 delete[] mat;
 return(answer);
}
//====================================================================================//
//  Read_Internal_Vector                                                              //
//====================================================================================//
//                                                                                    //
//   This function reads from the input file 'fin' an internal vector and puts it as  //
// the reachable state space (RSS::rss). The internal vector is stored as a bools     //
// object (bools class).                                                              //
//                                                                                    //
// Example: ' 128  30 7 31 24 ' a 128 elements vector stored in four words of 32 bits //
//====================================================================================//
bool Read_Internal_Vector(ifstream & fin, aut_st & s)
{
 bool     answer = false;
 bools    buf;

 RSS::rss.create(s);
 buf.read(fin);
 if (buf.size() == RSS::rss.size()) {
   answer = true;
   RSS::rss.put(buf);
 }
 return(answer);
}

//====================================================================================//
//  Read_Aut_Sizes                                                                    //
//====================================================================================//
//                                                                                    //
//   This function reads from the input file 'fin' the size of 'n' automata. The      //
// first information is the number of automata, then the size of each automata is     //
// read (all numbers are integers between '2' and 'max_integer'). Comments can appear //
// at any place, since the numbers are separated by blanks.                           //
//                                                                                    //
//     Example:      ' 4 { four automata } 8 8 10 10 { with sizes 8 8 10 and 10 } '   //
//====================================================================================//
bool Read_Aut_Sizes(ifstream & fin, aut_st & s)
{
 bool     answer = false;
 bool     cont   = true;
 char     buf[32];
 aut_id   automata = no_aut;
 int      size;
 aut_id   a = 0;
 comp_errors error = err_0000;

 fin >> buf;
 do {
   switch (buf[0]) {
   case '0' : case '1' : case '2' :
   case '3' : case '4' : case '5' :
   case '6' : case '7' : case '8' :
   case '9' : if (automata == no_aut) {                                     // number of
                automata = atoi(buf);                                       // automata
                if ( (automata < 2) || (automata > max_integer) )
                  error = err_7016;
                else {
                  s.create(automata);
                  a = 0;
                }
              }
              else {                                                        // size of
                size = atoi(buf);                                           // automaton
                if ( (size < 2) || (size > max_integer) )
                  error = err_7017;
                else {
                  s.put(a, size);
                  a++;
                }
              }
              if (a != automata)
                fin >> buf;
              break;
   case '{' : do { fin >> buf; } while (strcmp(buf, "}")); fin >> buf;      // comments
              break;
   default  : error = err_7020;
   }
 } while ( (error == err_0000) && (a != automata) );
 switch (error) {
 case err_7016 : cerr << "Automata Sizes Compilation: invalid number of automata ("
                      << automata << ")\n";
                 break;
 case err_7017 : cerr << "Automata Sizes Compilation: invalid automaton size (automaton "
                      << a << " - size " << size << ")\n";
                 break;
 case err_0000 : answer = !(s.is_empty());
                 break;
 case err_7020 : cerr << "Automata Sizes Compilation: unexpected character or end of"
                      << " file\n";
                 break;
 default       : Programming_Error("unexpected compilation error", 7000)
 }
 return(answer);
}

//====================================================================================//
//  Compile_Function_Table                                                            //
//====================================================================================//
//     This function reads from a file name called '<t_name>.tft' the following       //
// information:                                                                       //
//                                                                                    //
//     - the size of each automaton (domain of all functions);                        //
//     - the number of functions to put into the table;                               //
//     - for each function to put into the table:                                     //
//         - its internal identifier (an integer) and its formulae (a function tree   //
//             in reverse polish notation);                                           //
//     - the rewards for each state of each automaton:                                //
//         - the automaton and the state internal identifiers (two integers) and the  //
//             the reward associated to this state (a real number);                   //
//                                                                                    //
//     The structure of the declaration is:                                           //
//                                                                                    //
//     #domain <automata sizes>                                                       //
//     #functions <number of functions>                                               //
//     F<identifier> : <function tree>                                                //
//     ...        { as many functions as declared above }                             //
//                                                                                    //
//     Example:                                                                       //
//                                                                                    //
// {  Model for 3 queues example 02-03-05                                             //
//    lambda 1 = 1 unit   mu 1    = 3 units                                           //
//    lambda 2 = 2 unit   mu 2    = 4 units                                           //
//                        mu 31   = 5 units                                           //
//                        mu 32   = 6 units                                           //
//    func   f  = F2      mu 32 g = F0                                                //
//    func 1-f  = F3     -mu 32 g = F1 }                                              //
//                                                                                    //
// #domain 4                                                                          //
//    2 3 5 5                                                                         //
//                                                                                    //
// #functions 4                                                                       //
//                                                                                    //
// F0 : @2 0 ==  6 * ; { Function  mu 31 g ->  if A(31) = 0  then  6 units            //
//                                                           else  0 units }          //
// F1 : @2 0 == -6 * ; { Function -mu 31 g ->  if A(31) = 0  then -6 units            //
//                                                           else  0 units }          //
// F2 : %2 %3 + 9 < ;  { Function        f ->  if A(31) + A(32) < 9  then  1 unit     //
//                                                                   else  0 unit }   //
// F3 : %2 %3 + 9 >= ; { Function      1-f ->  if A(31) + A(32) >= 9 then  1 unit     //
//                                                                   else  0 unit }   //
//                                                                                    //
// #rewards                                                                           //
//                                                                                    //
//   0 0  :  0.5                                                                      //
//   0 1  :  1.0                                                                      //
//   1 0  :  1.25                                                                     //
//   1 1  :  1.5                                                                      //
//   1 2  :  2.5                                                                      //
//   2 0  :  0.0                                                                      //
//   2 1  :  3.5                                                                      //
//   2 2  :  7.0                                                                      //
//   2 3  : 15.0                                                                      //
//   2 4  : 20.0                                                                      //
//   3 0  :  1.75                                                                     //
//   3 1  :  2.0                                                                      //
//   3 2  :  4.0                                                                      //
//   3 3  :  8.0                                                                      //
//   3 4  : 16.0                                                                      //
//                                                                                    //
//    The result of this compilation is saved in the variable 'FTB::ftb' (see 'FTB'   //
// class in file 'ftb.h') and into a output file called '<i_name>.ftb'. If an error   //
// is found an error message is displayed in the standard error output ('cerr') and   //
// no result is stored (variable 'FTB::ftb' becomes empty).                           //
//====================================================================================//
void Compile_Function_Table(peps_name & t_name, peps_name & i_name)
{
 ifstream    fin;                 // input file
 char        buf[32];             // string buffer
 int         func_number;         // function number and function counter
 aut_st      sizes;               // functions domain
 aut_st      originals;           // id's of the original automata
 func_tree   f;                   // function been read
 func_id     id = 0;              // function id of function 'f'
 aut_id      a, b;                // counters for automata
 st_id       s, t;                // counters for automata states
 rp          rew;                 // buffer to read rewards

 comp_errors error = err_0000;    // error found ('err_0000' means no erros found)

 Open_File(t_name, tft_file, fin);
 do { fin >> buf; } while (strcmp(buf, "#domain"));
 if (Read_Aut_Sizes(fin, sizes)) {
   originals.create(sizes.size());                       originals.stamp();
   do { fin >> buf; } while (strcmp(buf, "#functions")); fin >> func_number;
   FTB::ftb.create(func_number);
   FTB::ftb.put_domain(sizes);
   FTB::ftb.put_currents(originals);
   FTB::ftb.baptise(i_name);
   if (func_number != 0) {                         // Read Functions
     do { fin >> buf;
          switch (buf[0]) {
            case 'f' :
            case 'F' : id = int(atof(buf+1));
                       fin >> buf;
                       if ((buf[0] != ':') || (buf[1] != '\0'))
                         error = err_7009;
                       else {
                         if (!(Read_Function_Tree(fin, f)))
                           error = err_7018;
                         else
                           if (f.is_empty())
                             error = err_7009;
                           else
                             if (id != FTB::ftb.put(f))
                               error = err_7010;
                       }
                       break;
            case '{' : do { fin >> buf; } while (strcmp(buf, "}")); break;
            default  : error = err_7020;
          }
     } while ((error == err_0000) && (id < func_number-1));
     if (id != (func_number-1))
       error = err_7019;
   }
   do { fin >> buf; } while (strcmp(buf, "#rewards"));  // Read Rewards
   for (a=0; a<sizes.size(); a++)
     for (s=0; s<sizes.sz(a); s++) {
       fin >> b;
       fin >> t;
       fin >> buf;
       if(buf[0] != ':' || buf[1] != '\0')
	 error = err_7022;
       fin >> rew;
       FTB::ftb.reward(b, t, rew);
     }
 }
 else
   error = err_7021;

 Close_File(t_name, tft_file, fin);
 switch (error) {
 case err_7009 : cerr << "Function Table compilation: invalid function definition\n";
                 FTB::ftb.erase();
                 break;
 case err_7010 : cerr << "Function Table compilation: non-contiguous function id\n";
                 FTB::ftb.erase();
                 break;
 case err_7018 : cerr << "Function Table compilation: function " << id+1
                      << "not well defined\n";
                 FTB::ftb.erase();
                 break;
 case err_7019 : cerr << "Function Table compilation: missing function " << id+1
                      << "\n";
                 FTB::ftb.erase();
                 break;
 case err_7020 : cerr << "Function Table compilation: unexpected char or end of file\n";
                 FTB::ftb.erase();
                 break;
 case err_7021 : cerr << "Function Table compilation: invalid function domain\n";
                 FTB::ftb.erase();
                 break;
 case err_7022 : cerr << "Function Table compilation: missing ':' in rewards\n";
                 FTB::ftb.erase();
                 break;
 case err_0000 : FTB::ftb.write();
                 break;
 default : Programming_Error("unexpected compilation error", 7000)
           FTB::ftb.erase();
 }
}

//====================================================================================//
//  Compile_Integration_Function                                                      //
//====================================================================================//
//     This function reads from a file name called '<t_name>.res' the following       //
// information:                                                                       //
//                                                                                    //
//     - the size of each automaton (domain of all functions);                        //
//     - the number of functions to put into the table;                               //
//     - for each function to put into the table:                                     //
//         - its internal identifier (an integer) and its formulae (a function tree   //
//             in reverse polish notation);                                           //
//                                                                                    //
//     The structure of the declaration is:                                           //
//                                                                                    //
//     #domain <automata sizes>                                                       //
//     #functions <number of functions>                                               //
//     F<identifier> : <function tree>                                                //
//     ...        { as many functions as declared above }                             //
//                                                                                    //
//     Example:                                                                       //
//                        to do                                                       //
//                                                                                    //
//    The result of this compilation is saved in the variable 'INF::inf' (see 'INF'   //
// class in file 'inf.h') and into a output file called '<i_name>.inf'. If an error   //
// is found an error message is displayed in the standard error output ('cerr') and   //
// no result is stored (variable 'INF::inf' becomes empty).                           //
//====================================================================================//
void Compile_Integration_Function(peps_name & t_name, peps_name & i_name)
{
 ifstream    fin;                 // input file
 char        buf[128];             // string buffer
 int         func_number;         // function number and function counter
 aut_st      sizes;               // the functions domain
 func_tree   f;                   // the function been read
 func_id     id = 0;              // the function id of function 'f'
 comp_errors error = err_0000;    // error found ('err_0000' means no erros found)

 Open_File(t_name, res_file, fin);
 do { fin >> buf; } while (strcmp(buf, "#domain"));
 if (Read_Aut_Sizes(fin, sizes)) {
   do { fin >> buf; } while (strcmp(buf, "#functions")); fin >> func_number;
   INF::inf.create(func_number);
   INF::inf.put_domain(sizes);
   INF::inf.baptise(i_name);
   if (func_number != 0) {
     do { fin >> buf;
          switch (buf[0]) {
            case 'f' :
            case 'F' : id = int(atof(buf+1));
                       fin >> buf;
                       if ((buf[0] != ':') || (buf[1] != '\0'))
                         error = err_7009;
                       else {
                         if (!(Read_Function_Tree(fin, f)))
                           error = err_7018;
                         else
                           if (f.is_empty())
                             error = err_7009;
                           else
                             if (id != INF::inf.put(f))
                               error = err_7010;
                       }
                       break;
            case '{' : do { fin >> buf; } while (strcmp(buf, "}")); break;
            default  : error = err_7020;
          }
     } while ((error == err_0000) && (id < func_number-1));
     if (id != (func_number-1))
       error = err_7019;
   }
 }
 else
   error = err_7021;

 Close_File(t_name, res_file, fin);
 switch (error) {
 case err_7009 : cerr << "Integration Functions compilation: invalid function definition\n";
                 INF::inf.erase();
                 break;
 case err_7010 : cerr << "Integration Functions compilation: non-contiguous function id\n";
                 INF::inf.erase();
                 break;
 case err_7018 : cerr << "Integration Functions compilation: function " << id+1
                      << "not well defined\n";
                 INF::inf.erase();
                 break;
 case err_7019 : cerr << "Integration Functions compilation: missing function " << id+1
                      << "\n";
                 INF::inf.erase();
                 break;
 case err_7020 : cerr << "Integ. Functions compilation: unexpected char or end of file\n";
                 INF::inf.erase();
                 break;
 case err_7021 : cerr << "Integration Functions compilation: invalid function domain\n";
                 INF::inf.erase();
                 break;
 case err_0000 : INF::inf.write();
                 break;
 default : Programming_Error("unexpected compilation error", 7000)
           INF::inf.erase();
 }
}

//====================================================================================//
//  Compile_Reachable_State_Space                                                     //
//====================================================================================//
//     This function reads from a file name called '<t_name>.fct' one the following   //
// information:                                                                       //
//                                                                                    //
//------------------------------------------------------------------------------------//
//   First Option                                                                     //
//   ============                                                                     //
//                                                                                    //
//     - the size of each automaton (domain of the reachability function);            //
//     - the reachability function formulae (a function tree in reverse polish        //
//             notation);                                                             //
//                                                                                    //
//     The structure of the declaration is:                                           //
//                                                                                    //
//     #domain <automata sizes>                                                       //
//     #function <function tree>                                                      //
//                                                                                    //
//     Example:                                                                       //
//                                                                                    //
// {  Model for 3 queues example 08-08-10                                             //
//    lambda 1 = 1 unit   mu 1    = 3 units                                           //
//    lambda 2 = 2 unit   mu 2    = 4 units                                           //
//                        mu 31   = 5 units                                           //
//                        mu 32   = 6 units                                           //
//    func   f  = F2      mu 32 g = F0                                                //
//    func 1-f  = F3     -mu 32 g = F1 }                                              //
//                                                                                    //
//     Example:                                                                       //
//                                                                                    //
// #domain 4                                                                          //
//   8 8 10 10                                                                        //
//                                                                                    //
// #function                                                                          //
// %2 %3 + 9 <= ; { if the sum of the states of automata 2 and 3 is less or equal 9 } //
//                                                                                    //
//    The result of this compilation is saved in a 'functree' object (variable 'f'),  //
// then the function 'f' is evaluated for all the domain in order to generate the     //
// vector of reachable states which is stored in the variable 'RSS::rss' (see 'RSS'   //
// class in file 'rss.h') and into a output file called '<i_name>.rss'. If an error   //
// is found an error message is displayed in the standard error output ('cerr') and   //
// no result is stored (variable 'RSS::rss' becomes empty).                           //
//                                                                                    //
//------------------------------------------------------------------------------------//
//    Second Option                                                                   //
//    =============                                                                   //
//                                                                                    //
//     - the size of each automaton (domain of the reachability function);            //
//     - the reachability vector in the internal format (a class bools object);       //
//                                                                                    //
//     The structure of the declaration is:                                           //
//                                                                                    //
//     #domain <automata sizes>                                                       //
//     #vector <bools>                                                                //
//                                                                                    //
//     Example: to do                                                                 //
//                                                                                    //
//====================================================================================//
void Compile_Reacheable_SS(peps_name & t_name, peps_name & i_name)
{
 ifstream    fin;
 char        buf[32];
 aut_st      sizes;
 ///func_id     id;
 func_tree   f;
 comp_errors error = err_0000;

 Open_File(t_name, fct_file, fin);
 do { fin >> buf; } while (strcmp(buf, "#domain"));
 if (Read_Aut_Sizes(fin, sizes)) {
   do { fin >> buf; } while (strcmp(buf, "#function") && strcmp(buf, "#vector"));
   if (!strcmp(buf, "#function")) {                        // First Option
     if (Read_Function_Tree(fin, f)) {                     //  function definition
       RSS::rss.generate(f, sizes);
       RSS::rss.baptise(i_name);
       RSS::rss.write();
     }
     else
       cerr << "Reachable State Space compilation: invalid function definition\n";
   }
   else {                                                  // Second Option
     if (Read_Internal_Vector(fin, sizes)) {               //  vector definition
       RSS::rss.baptise(i_name);
       RSS::rss.write();
     }
     else
       cerr << "Reachable State Space compilation: invalid vector definition\n";
   }
 }
 else
   cerr << "Reachable State Space compilation: invalid function domain\n";
 Close_File(t_name, fct_file, fin);
}

//====================================================================================//
//  Compile_Descriptor                                                                //
//====================================================================================//
void Compile_Descriptor(peps_name & t_name, peps_name & i_name)
{
 ifstream    fin;             // the file
 char        buf[32];         // buffer to read the file
 aut_id      a;               // number of automata
 ev_id       e;               // number of sync. events
 aut_st      sizes;           // the size of each automata
 san_mat     m;               // a buffer for the matrices
 st_map      sm;              // a buffer for the state maps
 aut_st      sms;             // a buffer for the simetrics
 int         i, j;            // counter for automata and events
 aut_id      mat_id;          // replicated matrices have the same mat_id
 aut_id      ticket = 0;      // ticket to generate mat_id's to non-replicated
 bool        error;           // a block (aut. sizes or matrix) could not be read

 Open_File(t_name, des_file, fin);
 do { fin >> buf; } while (strcmp(buf, "#type"));     fin >> i;
 do { fin >> buf; } while (strcmp(buf, "#automata")); fin >> a;
 do { fin >> buf; } while (strcmp(buf, "#events"));   fin >> e;
 if (i == 0)
   DSC::dsc.create(Discrete, a, e);
 else
   DSC::dsc.create(Continuous, a, e);
 DSC::dsc.baptise(i_name);
 DSC::dsc.o_baptise(i_name);
 do { fin >> buf; } while (strcmp(buf, "#sizes"));    sizes.read(fin);
 DSC::dsc.put_s(sizes);
 sms.create(a);
 do { fin >> buf; } while (strcmp(buf, "#locals"));
 for (i=0; i<a; i++) {
   do { fin >> buf; } while ((buf[0] != '[') && (buf[0] != 'R'));
   if (!(strcmp(buf, "["))) { error = !(Read_SAN_Matrix(fin, sizes.sz(i), m));
                              mat_id = ticket;
                              ticket++; }
                       else if (!(strcmp(buf, "REP"))) { fin >> mat_id;
                                                         DSC::dsc.get_mat(mat_id, m); }
                            else m.erase();
   DSC::dsc.put_mat(i, m);
   if (error)
     cerr << "Descriptor compilation: missing local matrix " << i << "\n";
   sm.create(sizes.sz(i), 1);
   sm.put(i, 0);
   sm.fill();
   DSC::dsc.put_sm(i, sm);
   sm.erase();
   sms.put(i, mat_id);
 }
 DSC::dsc.put_sms(sms);
 for (j=0; j<e; j++) {
   do { fin >> buf; } while (strcmp(buf, "#event"));  fin >> i;
   if (i != j)
     cerr << "Descriptor compilation: missing matrices of event " << j << "\n";
   else {
     do { fin >> buf; } while (strcmp(buf, "#positive"));
     for (i=0; i<a; i++) {
       do { fin >> buf; } while ((buf[0] != '[') && (buf[0] != 'R'));
       if (!(strcmp(buf, "[")))
         error = !(Read_SAN_Matrix(fin, sizes.sz(i), m));
       else if (!(strcmp(buf, "REP"))) {
              fin >> mat_id;
              DSC::dsc.get_mat(j, mat_id, Mat_plus, m);
            }
            else m.erase();
       DSC::dsc.put_mat(j, i, Mat_plus, m);
       if (error)
         cerr << "Descriptor compilation: missing matrix " << i 
              << " of event " << j << " (positive)\n";
       else
         m.erase();
     }
     do { fin >> buf; } while (strcmp(buf, "#negative"));
     DSC::dsc.put_sr(j, 0);
     for (i=0; i<a; i++) {
       do { fin >> buf; } while ((buf[0] != '[') && (buf[0] != 'R'));
       if (!(strcmp(buf, "[")))
         error = !(Read_SAN_Matrix(fin, sizes.sz(i), m));
       else if (!(strcmp(buf, "REP"))) {
              fin >> mat_id;
              DSC::dsc.get_mat(j, mat_id, Mat_minus, m);
            }
            else m.erase();
       DSC::dsc.put_mat(j, i, Mat_minus, m);
       if (error)
         cerr << "Descriptor compilation: missing matrix " << i 
              << " of event " << j << " (negative)\n";
       else {
         if (FTB::ftb.is_empty())
           { if (m.is_functional()) DSC::dsc.put_sr(j, i); 
             else if (m.has_negatives()) DSC::dsc.put_sr(j, i); }
         else
           { if (m.has_negatives()) DSC::dsc.put_sr(j, i); }
         m.erase();
       }
     }
   }
 }
 Close_File(t_name, des_file, fin);
 DSC::dsc.write();
}

//====================================================================================//
//  Compile_Dictionary                                                                //
//====================================================================================//
//    Example:                                                                        //
//                                                                                    //
//        #model      exemple_bidon                                                   //
//        #reach_func " @Bc + @Bv "                                                   //
//        #automata   3  4 4 4                                                        //
//        #events     2                                                               //
//        #functions  3                                                               //
//        #results    5                                                               //
//                                                                                    //
//        #automata   0 Br                                                            //
//                    1 Bc                                                            //
//                    2 Bv                                                            //
//                                                                                    //
//        #events   0 arrival                                                         //
//                  1 departure                                                       //
//                                                                                    //
//        #states  0 0 n0                                                             //
//                 0 1 n1                                                             //
//                 0 2 n2                                                             //
//                 0 3 n3                                                             //
//                 1 0 n0                                                             //
//                 1 1 n1                                                             //
//                 1 2 n2                                                             //
//                 1 3 n3                                                             //
//                 2 0 n0                                                             //
//                 2 1 n1                                                             //
//                 2 2 n2                                                             //
//                 2 3 n3                                                             //
//                                                                                    //
//       #functions  0 aut_r         " @Br <> 0 "                                     //
//                   1 new           " ( @Br <> 0 ) * 1 "                             //
//                   2 new           " ( @Br <> 0 ) * 1 * -1 "                        //
//                                                                                    //
//       #results  0 fv     " @Br <> 0 "                                              //
//                 1 fc     " @Br <> 0 "                                              //
//                 2 fr     " @Br <> 0 "                                              //
//                 3 g      " @Br <> 0 "                                              //
//                 4 h      " @Br <> 0 "                                              //
//====================================================================================//
void Compile_Dictionary(peps_name & t_name, peps_name & i_name)
{
 ifstream        fin;             // the file
 peps_name       buf;
 bool            reading = true;
 peps_name       model_name;
 peps_expr       expression;
 aut_st          sizes;
 aut_id          a, b;
 ev_id           e, d;
 st_id           s, t;
 func_id         g, i;

 Open_File(t_name, dic_file, fin);
 do { fin >> buf; } while (strcmp(buf, "#model"));
 fin >> model_name;
 DCT::dct.n_bapt(model_name);
 do { fin >> buf; } while (strcmp(buf, "#reach_func"));
 strcpy(expression,"\0");
 fin >> buf;
 if (strcmp(buf, "\""))
   Compilation_Error("External dictionary", "bad reachability function expression")
 else
   do { fin >> buf;
        if (strcmp(buf, "\"")) {
          strcat(expression, " ");
          strcat(expression, buf);
        }
        else reading = false;
   } while (reading);
 reading = true;
 DCT::dct.r_dexp(expression);
 do { fin >> buf; } while (strcmp(buf, "#automata"));
 if (Read_Aut_Sizes(fin, sizes)) {
   do { fin >> buf; } while (strcmp(buf, "#events"));    fin >> e;
   do { fin >> buf; } while (strcmp(buf, "#functions")); fin >> g;
   do { fin >> buf; } while (strcmp(buf, "#results"));   fin >> i;
   DCT::dct.create(sizes, e, g, i);
   DCT::dct.baptise(i_name);
   do { fin >> buf; } while (strcmp(buf, "#automata"));
   for (a=0; a<DCT::dct.aut(); a++) {
     fin >> b;
     fin >> buf;
     DCT::dct.a_bapt(b, buf);
   }
   do { fin >> buf; } while (strcmp(buf, "#events"));
   for (e=0; e<DCT::dct.ev(); e++) {
     fin >> d;
     fin >> buf;
     DCT::dct.e_bapt(d, buf);
   }
   do { fin >> buf; } while (strcmp(buf, "#states"));
   for (a=0; a<DCT::dct.aut(); a++)
     for (s=0; s<sizes.sz(a); s++) {
       fin >> b;
       fin >> t;
       fin >> buf;
       DCT::dct.s_bapt(b, t, buf);
     }
   do { fin >> buf; } while (strcmp(buf, "#functions"));
   for (g=0; g<DCT::dct.gfs(); g++) {
     fin >> i;
     fin >> buf;
     DCT::dct.gf_bapt(i, buf);
     strcpy(expression,"\0");
     fin >> buf;
     if (strcmp(buf, "\""))
       Compilation_Error("External dictionary", "bad general function expression")
     else
       do { fin >> buf;
            if (strcmp(buf, "\"")) {
              strcat(expression, " ");
              strcat(expression, buf);
            }
            else reading = false;
       } while (reading);
     reading = true;
     DCT::dct.gf_dexp(i, expression);
   }
   do { fin >> buf; } while (strcmp(buf, "#results"));
   for (i=0; i<DCT::dct.sfs(); i++) {
     fin >> g;
     fin >> buf;
     DCT::dct.sf_bapt(g, buf);
     strcpy(expression,"\0");
     fin >> buf;
     if (strcmp(buf, "\""))
       Compilation_Error("External dictionary", "bad result function expression")
     else
       do { fin >> buf;
            if (strcmp(buf, "\"")) {
              strcat(expression, " ");
              strcat(expression, buf);
            }
            else reading = false;
       } while (reading);
     reading = true;
     DCT::dct.sf_dexp(g, expression);
   }
   DCT::dct.write();
 }
 else
   cerr << "Dictionary compilation: invalid automata sizes definition\n";
 Close_File(t_name, dic_file, fin);
}

//====================================================================================//
//                                                                                    //
//====================================================================================//
