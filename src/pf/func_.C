//==============================================================================//
//                                                                              //
//                              Function Class                                  //
//                                                                              //
//==============================================================================//
//  This File:   func.C                   Tool: C++                             //
//  Software:    PEPS                     Advisor: Brigitte.Plateau@imag.fr     //
//  Doc:                                                                        //
//==============================================================================//
//  Creation:    30/Aug/05                by: Leonardo.Brenner@imag.fr          //
//  Last Change: 30/Aug/05                by: Leonardo.Brenner@imag.fr          //
//==============================================================================//
//  This class is used to store and manipulate the function obtained from SAN   //
// textual input file.                                                          //
//==============================================================================//  
#include "peps.h"

//------------------------------------------------------------------------------
// Contructor 
//------------------------------------------------------------------------------
FUNC_::FUNC_()
{
  id       = NULL;
  expr     = NULL;
  ind      = 0;
  n        = 0;
  
  func     = NULL;
  op       = NULL;
  ind_func = 0; 
  ind_op   = 0; 
  n_func   = 0;
  n_op     = 0;

  dimension = 0;
  last      = 0;
  mark      = 0;
}

//------------------------------------------------------------------------------
// Destructor 
//------------------------------------------------------------------------------
FUNC_::~FUNC_()
{
  for (int i=0; i<ind; i++){
    if (id[i])
      delete[] id[i];
    if (expr[i])
     delete[] expr[i];  
  }

  for (int i=0; i<ind_func; i++)
    if (func[i])
      delete[] func[i];

  for (int i=0; i<ind_op; i++)
    if (op[i])
      delete[] op[i];
  
  
  if (id)
    delete[] id;
  if (expr)
    delete[] expr;
  if (func)
    delete[] func;
  if (op)
    delete[] op;
    

  ind      = 0;    
  n        = 0;
  ind_func = 0;
  ind_op   = 0;
  n_func   = 0;
  n_op     = 0;
}

//------------------------------------------------------------------------------
// put_id - put a new function identifier
// in: _id - identifier
//------------------------------------------------------------------------------
int FUNC_::put_id(char *_id)
{
  for (int i=0; i<ind; i++)
    if (!strcmp(id[i], _id))
      return(0);

  if (ind >= n)
    resize();
  id[ind]   = new char[MAX_ID_NAME];
  expr[ind]  = new char[MAX_EXP_SIZE];
  strcpy(id[ind],_id);  
  ind++;
  return(1);
}

//------------------------------------------------------------------------------
// build_exp - put a new function expression
// in: _id - identifier
//------------------------------------------------------------------------------
int FUNC_::build_exp(char *_id)
{
  for (int i=0; i<ind; i++){
    if (!strcmp(id[i], _id)){
      strcpy(expr[i], func[0]);
      for (int j=1; j<ind_func; j++){
        strcat(expr[i]," ");
        strcat(expr[i],func[j]);
      }
      for (int j=0; j<ind_func; j++)
        delete [] func[j];
      for (int j=0; j<ind_op; j++)
        delete [] op[j];
      ind_func = 0;
      ind_op = 0;
      return(1);
    }
  }

  if (ind >= n)
    resize();
  id[ind]  = new char[MAX_ID_NAME];
  expr[ind] = new char[MAX_EXP_SIZE];
  strcpy(id[ind], _id);
  strcpy(expr[ind], func[0]);
  for(int j=1; j<ind_func; j++){
    strcat(expr[ind]," ");
    strcat(expr[ind],func[j]);
  }
  for (int j=0; j<ind_func; j++)
    delete [] func[j];
  for (int j=0; j<ind_op; j++)
    delete [] op[j];
  ind_func = 0;
  ind_op = 0;
  ind++;
  return(1);
}

//------------------------------------------------------------------------------
// put_exp - put a new function expression
// in: _id  - identifier
//     _exp - expression
//------------------------------------------------------------------------------
int FUNC_::put_exp(char *_id, char *_exp)
{
  for (int i=0; i<ind; i++){
    if (!strcmp(id[i], _id)){
      strcpy(expr[i], _exp);
      return(1);
    }
  }

  if (ind >= n)
    resize();
  id[ind]  = new char[MAX_ID_NAME];
  expr[ind] = new char[MAX_EXP_SIZE];
  strcpy(id[ind], _id);
  strcpy(expr[ind], _exp);
  ind++;
  return(1);
}

//------------------------------------------------------------------------------
// get_id - get the function index
// in: _id  - identifier
//------------------------------------------------------------------------------
int FUNC_::get_id(char *_id)
{
  for (int i=0; i<ind; i++)
    if (!strcmp(id[i], _id))
      return(i);
  return(-1);
}

//------------------------------------------------------------------------------
// get_name - get the function identifier
// in:  _func - function index
// out: _id   - identifier
//------------------------------------------------------------------------------
void FUNC_::get_name(int _func, char *_id)
{
  strcpy(_id, id[_func]);
}

//------------------------------------------------------------------------------
// get_exp - get the function expression
// in:  _id  - identifier
// out: _exp - function expression
//------------------------------------------------------------------------------
int FUNC_::get_exp(char *_id, char *_exp)
{
  for (int i=0; i<ind; i++)
    if (!strcmp(id[i], _id)){
      strcpy(_exp, expr[i]);
      return(i);
    }
  return(-1);
}

//------------------------------------------------------------------------------
// get_exp - get the function expression
// in:  _func  - function index
// out: _exp - function expression
//------------------------------------------------------------------------------
void FUNC_::get_exp(int _func, char *_exp)
{
  strcpy(_exp, expr[_func]);
}

//------------------------------------------------------------------------------
// get_exp_to_file - get the function expression to file (remove description words)
// in:  _func  - function index
// out: _exp - function expression
//------------------------------------------------------------------------------
int FUNC_::get_exp_to_file(int _func, char *_exp)
{

  strcpy(_exp, expr[_func]);
  for (int i=0; i<MAX_EXP_SIZE; i++){
    switch(expr[_func][i]){
      case '&' : if (expr[_func][i+1] == 'a' || expr[_func][i+1] == 's' || expr[_func][i+1] == 'd')
                   return(0);
      case '\0': return(1);
    }
  }
}

//------------------------------------------------------------------------------
// get_n_func - get the number of function
//------------------------------------------------------------------------------
int FUNC_::get_n_func()
{
  return(ind);
}

//------------------------------------------------------------------------------
// has_identical - if the _exp is identical to another expression then function 
///                index is returned else -1 is returned
// in: _exp - function expression
//------------------------------------------------------------------------------
int FUNC_::has_identical(char *_exp)
{
  for (int i=0; i<ind; i++)
    if (!strcmp(expr[i], _exp))
      return(i);
  return(-1);
}

//------------------------------------------------------------------------------
// put_elem - put a new element
// in: _elem - new element
//------------------------------------------------------------------------------
void FUNC_::put_elem(char *_elem)
{
  if (ind_func >= n_func)
    resize_stack();
  func[ind_func] = new char[MAX_EXP_SIZE];
  strcpy(func[ind_func],_elem);
  ind_func++;
}

//------------------------------------------------------------------------------
// put_elem - put a new element
//------------------------------------------------------------------------------
void FUNC_::put_elem()
{
  if (ind_func >= n_func)
    resize_stack();
  func[ind_func] = new char[MAX_EXP_SIZE];
  strcpy(func[ind_func], op[ind_op-1]);
  ind_func++;
  ind_op--;
  delete[] op[ind_op];
}

//------------------------------------------------------------------------------
// put_op - put a new operator
// in: _op - new operator
//------------------------------------------------------------------------------
void FUNC_::put_op(char *_op)
{
  if (ind_op >= n_op)
    resize_stack();
  op[ind_op] = new char[MAX_ID_NAME];
  strcpy(op[ind_op], _op);
  ind_op++;
}

//------------------------------------------------------------------------------
// cat_elem - concat a previous function
// in: _id - function identifier
//------------------------------------------------------------------------------
void FUNC_::cat_elem(char *_id)
{
  if (ind_func >= n_func)
    resize_stack();
  func[ind_func] = new char[MAX_EXP_SIZE];
  for (int i=0; i<ind; i++)
    if (!strcmp(id[i], _id))
     strcpy(func[ind_func],expr[i]);
  ind_func++;
}

//------------------------------------------------------------------------------
// is_func - return true if the identifier is a function
// in: _id -  identifier
//------------------------------------------------------------------------------
int FUNC_::is_func(char *_id)
{
  for (int i=0; i<ind; i++)
    if (!strcmp(id[i], _id))
      return(1);
  return(0);
}

//------------------------------------------------------------------------------
// is_const - return true if the identifier is a constant function
// in: _id -  identifier
//------------------------------------------------------------------------------
int FUNC_::is_const(char *_id)
{
  int ind_id = -1;
  
  for (int i=0; i<ind; i++)
    if (!strcmp(id[i], _id)){
      ind_id = i;  
      break;
    }
  if (ind_id < 0)
    return(-1);
  for (int i=0; i<MAX_EXP_SIZE; i++){
    switch(expr[ind_id][i]){
      case '&' : if (expr[ind_id][i+1] != 'a' && expr[ind_id][i+1] != 's' && expr[ind_id][i+1] != 'd') break;
      case '%' :
      case '@' : 
      case '#' : return(0); 
      case '\0': return(1);
    }
  }
  return(1);
}

//------------------------------------------------------------------------------
// is_descr - return true if the identifier is a description function
// in: _id -  identifier
//------------------------------------------------------------------------------
int FUNC_::is_descr(char *_id)
{
  int ind_id = -1;
  
  for (int i=0; i<ind; i++)
    if (!strcmp(id[i], _id)){
      ind_id = i;  
      break;
    }
  if (ind_id < 0)
    return(-1);
  for (int i=0; i<MAX_EXP_SIZE; i++){
    switch(expr[ind_id][i]){
      case '%' :
      case '@' : 
      case '#' : return(0); 
      case '\0': return(1);
    }
  }
  return(1);
}

//------------------------------------------------------------------------------
// check_rate - return true if the identifier is a function that can be used as 
//              an event rate
// in: _id -  identifier
//------------------------------------------------------------------------------
int FUNC_::check_rate(char *_id)
{
  int ind_id = -1;
  
  for (int i=0; i<ind; i++)
    if (!strcmp(id[i], _id)){
      ind_id = i;  
      break;
    }

  if (ind_id < 0)
    return(0);

  for (int i=0; i<MAX_EXP_SIZE; i++){
    switch(expr[ind_id][i]){
      case '&' : if (expr[ind_id][i+1] == 'a' || expr[ind_id][i+1] == 's' || expr[ind_id][i+1] == 'd') 
                   return(0);
      case '\0': return(1);
    }
  }
  return(1);
}

//------------------------------------------------------------------------------
// check_reach - return true if the function reachability is correct
// in: _id -  identifier
//------------------------------------------------------------------------------
int FUNC_::check_reach(char *_id) 
{
  int ind_id = -1;
  
  for (int i=0; i<ind; i++)
    if (!strcmp(id[i], _id)){
      ind_id = i;  
      break;
    }
  if (ind_id < 0)
    return(0);
 
  for (int i=0; i<MAX_EXP_SIZE; i++){
    switch(expr[ind_id][i]){
      case '&' : if (expr[ind_id][i+1] == 'a' || expr[ind_id][i+1] == 's' || expr[ind_id][i+1] == 'd') 
                   return(0);
      case '\0': return(1);
    }
  }
  return(1);
}

//------------------------------------------------------------------------------
// eval - evaluate a function
// in: _id - identifier
//------------------------------------------------------------------------------
int FUNC_::eval(char *_id)
{
  int ind_id = -1;
  int ok     = 0;
  
  for (int i=0; i<ind; i++)
    if (!strcmp(id[i], _id)){
      ind_id = i;  
      break;
    }

  for (int i=0; i<MAX_EXP_SIZE; i++){
    switch(expr[ind_id][i]){
      case '&' : if (expr[ind_id][i+1] != 'a' && expr[ind_id][i+1] != 's' && expr[ind_id][i+1] != 'd') break;
      case '%' :
      case '@' : 
      case '#' : cerr << " Semantical Error - (** \"" <<  _id << "\" is not a description function. It can not be evaluated **). Error 700\n";
                 exit(0);
      case '\0': ok = 1;
                 break;
    }
    if (ok) 
      break;
  }
  return(eval_func(ind_id, 0, 0, 0));
}

//------------------------------------------------------------------------------
// eval - evaluate a function
// in: _id  - identifier
//     _aut - current automaton
//     _stt - current state
//------------------------------------------------------------------------------
int FUNC_::eval(char *_id, int _aut, int _stt)
{
  int ind_id = -1;
  int ok = 0;
  
  for (int i=0; i<ind; i++)
    if (!strcmp(id[i], _id)){
      ind_id = i;  
      break;
    }

  for (int i=0; i<MAX_EXP_SIZE; i++){
    switch(expr[ind_id][i]){
      case '&' : if (expr[ind_id][i+1] != 'd') break;
      case '%' :
      case '@' : 
      case '#' : cerr << " Semantical Error - (** \"" <<  _id << "\" is not a description function. It can not be evaluated **). Error 701\n";
                 exit(0);
      case '\0': ok = 1;
                 break;
    }
    if (ok) 
      break;
  }
  return(eval_func(ind_id, _aut, _stt, 0));
}

//------------------------------------------------------------------------------
// eval - evaluate a function
// in: _id   - identifier
//     _aut  - current automaton
//     _stt  - current state
//     _sttd - target state
//------------------------------------------------------------------------------
int FUNC_::eval(char *_id, int _aut, int _stt, int _sttd)
{
  int ind_id = -1;
  int ok = 0;
  
  for (int i=0; i<ind; i++)
    if (!strcmp(id[i], _id)){
      ind_id = i;  
      break;
    }

  for (int i=0; i<MAX_EXP_SIZE; i++){
    switch(expr[ind_id][i]){
      case '%' :
      case '@' : 
      case '#' : cerr << " Semantical Error - (** \"" <<  _id << "\" is not a description function. It can not be evaluated **). Error 702\n";
                 exit(0);
      case '\0': ok = 1;
                 break;
    }
    if (ok) 
      break;
  }
  return(eval_func(ind_id, _aut, _stt, _sttd));
}

//------------------------------------------------------------------------------
// print - print a readable version of the model structure
//------------------------------------------------------------------------------
void FUNC_::print()
{ 
  cout << "\tFunction Table\n";
  cout << "Allocated positions: " << n << endl;
  cout << "Used positions: " << ind << endl << endl;
  for (int i=0; i<ind; i++){
    cout << i << "\t" << id[i] << "\t" << expr[i] << endl;
  }
}

//------------------------------------------------------------------------------
// create_func_tree - create a function tree to be evaluated
// in: _i - function index
//------------------------------------------------------------------------------
int FUNC_::create_func_tree(int _i)
{ 

 token  line[2048];
 char   buf[32];
 int    tok = 0;
 int    i = 0;
 int    j;
 bool   cont = true;
 comp_errors error = err_0000;

 erase_func_tree();
 do {
   while (expr[_i][i] == ' ') i++;
   j = 0;
   while (expr[_i][i] != ' ' && expr[_i][i] != '\0'){
     buf[j] = expr[_i][i];
     j++; i++;
   }
   buf[j] = '\0';
   switch (buf[0]) {
   case '!' : if (buf[1] == '=') { line[tok].op = Neq;                      //  '!='
                                   line[tok].val = 0.0;
                                   if (buf[2] == '\0') { tok++; }
                                   else error = err_7001; }
              else if (buf[1] == '\0') { line[tok].op = Not;                // '!'
                                         line[tok].val = 0.0;
                                         if (buf[2] == '\0') { tok++; }
                                         else error = err_7001; }
                   else error = err_7003;
              break;
   case '&' : switch (buf[1]) {
               case '&' : line[tok].op = And;                      // '&&'
                          line[tok].val = 0.0;
                          if (buf[2] == '\0') { tok++; }
                          else error = err_7001; 
                          break;
               case 'a' : line[tok].op = Aut;                      // '&a'
                          line[tok].val = 0.0;
                          if (buf[2] == '\0') { tok++; }
                          else error = err_7001; 
                          break;
               case 's' : line[tok].op = Sts;                      // '&s'
                          line[tok].val = 0.0;
                          if (buf[2] == '\0') { tok++; }
                          else error = err_7001; 
                          break;
               case 'd' : line[tok].op = Std;                      // '&d'
                          line[tok].val = 0.0;
                          if (buf[2] == '\0') { tok++; }
                          else error = err_7001; 
                          break;
               default  : error = err_7004;
              }
              break;
   case '*' : if (buf[1] == '\0') { line[tok].op = Times;                   // '*'
                                    line[tok].val = 0.0;
                                    tok++; }
              else error = err_7001;
              break;
   case '+' : if (buf[1] == '\0') { line[tok].op = Plus;                    // '+'
                                    line[tok].val = 0.0;
                                    tok++; }
              else { line[tok].op  = Cnst;                                  // 'x'
                     line[tok].val = atof(buf);
                     tok++; }
              break;
   case '-' : if (buf[1] == '\0') { line[tok].op = Minus;                   // '-'
                                    line[tok].val = 0.0;
                                    tok++;}
              else if (buf[1] == '>') { line[tok].op = Imp;                 // '->'
                                        line[tok].val = 0.0;
                                        if (buf[2] == '\0') { tok++; }
                                        else error = err_7001; }
                   else { line[tok].op  = Cnst;                             // '-x'
                          line[tok].val = atof(buf);
                          tok++; }
              break;
   case '.' : line[tok].op  = Cnst;                                         // '.x'
              line[tok].val = atof(buf);
              tok++;
              break;
   case '/' : if (buf[1] == '\0') { line[tok].op = Slash;                   // '/'
                                    line[tok].val = 0.0;
                                    tok++; }
              else error = err_7001;
              break;
   case '0' : case '1' : case '2' : case '3' : case '4' :                   // 'x'
   case '5' : case '6' : case '7' : case '8' :
   case '9' : line[tok].op  = Cnst;
              line[tok].val = atof(buf);
              tok++;
              break;
   case '\0': cont = false;                          // ';'
              break;
   case '<' : switch (buf[1]) {
              case '\0' : line[tok].op = Less;                              // '<'
                          line[tok].val = 0.0;
                          tok++;
                          break;
              case '-'  : if (buf[2] == '>') { line[tok].op = Dimp;         // '<->'
                                               line[tok].val = 0.0;
                                               if (buf[3] == '\0') { tok++; }
                                               else error = err_7001; }
                          else error = err_7023;
                          break;
              case '='  : line[tok].op = Leq;                               // '<='
                          line[tok].val = 0.0;
                          if (buf[2] == '\0') { tok++;  }
                          else error = err_7001;
                          break;
              case '>'  : line[tok].op = Neq;                               // '<>'
                          line[tok].val = 0.0;
                          if (buf[2] == '\0') { tok++; }
                          else error = err_7001;
                          break;
              default   : error = err_7001; }
              break;
   case '=' : switch (buf[1]) {
              case '\0' : error = err_7007;
                          break;
              case '='  : line[tok].op = Eq;                                // '=='
                          line[tok].val = 0.0;
                          if (buf[2] == '\0') { tok++; }
                          else error = err_7001;
                          break;
              case '<'  : line[tok].op = Leq;                               // '=<'
                          line[tok].val = 0.0;
                          if (buf[2] == '\0') { tok++;  }
                          else error = err_7001;
                          break;
              case '>'  : line[tok].op = Geq;                               // '=>'
                          line[tok].val = 0.0;
                          if (buf[2] == '\0') { tok++; }
                          else error = err_7001;
                          break;
              default   : error = err_7001; }
              break;
   case '>' : switch (buf[1]) {
              case '\0' : line[tok].op = Gret;                              // '>'
                          line[tok].val = 0.0;
                          tok++;
                          break;
              case '='  : line[tok].op = Geq;                               // '>='
                          line[tok].val = 0.0;
                          if (buf[2] == '\0') { tok++;  }
                          else error = err_7001;
                          break;
              case '<'  : line[tok].op = Neq;                               // '><'
                          line[tok].val = 0.0;
                          if (buf[2] == '\0') { tok++; }
                          else error = err_7001;
                          break;
              default   : error = err_7001; }
              break;
   case '^' : if (buf[1] == '^') { line[tok].op = Or;                       // '^^'
                                   line[tok].val = 0.0;
                                   if (buf[2] == '\0') { tok++; }
                                   else error = err_7001; }
              else error = err_7006;
              break;
   case 'm' : if (buf[3] != '\0')
                error = err_7001;
              else
                if ((buf[1] == 'i') && (buf[2] == 'n')) {                   // 'min'
                  line[tok].op = Mini;
                  line[tok].val = 0.0;
                  tok++;
                }
                else
                  if ((buf[1] == 'a') && (buf[2] == 'x')) {                // 'max'
                    line[tok].op = Maxi;
                    line[tok].val = 0.0;
                    tok++;
                  }
                  else
                    if ((buf[1] == 'o') && (buf[2] == 'd')) {                // 'mod'
                      line[tok].op = Mod;
                      line[tok].val = 0.0;
                      tok++;
                    }
                    else
                      error = err_7002;
              break;
   case 'd' : if (buf[3] != '\0')
                error = err_7001;
              else
                if ((buf[1] == 'i') && (buf[2] == 'v')) {                   // 'div'
                  line[tok].op = Div;
                  line[tok].val = 0.0;
                  tok++;
                }
                else
                  error = err_7002;
              break;
   case '{' : do { while (expr[_i][i] == ' ') i++;
                   j = 0;
                   while (expr[_i][i] != ' ' && expr[_i][i] != '\0'){
                     buf[j] = expr[_i][i];
                     j++; i++;
                   }
                   buf[j] = '\0';
              } while (strcmp(buf, "}")); 
              while (expr[_i][i] == ' ') i++;
              j = 0;
              while (expr[_i][i] != ' ' && expr[_i][i] != '\0'){
                buf[j] = expr[_i][i];
                j++; i++;
              }
              buf[j] = '\0'; // '{ ... }'
              break;
   case '|' : if (buf[1] == '|') { line[tok].op = Or;                       // '||'
                                   line[tok].val = 0.0;
                                   if (buf[2] == '\0') { tok++; }
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
 case err_0000 : if (tok > 0)
                   list = new token[tok];
                 dimension = tok;
                 for (i=0; i<tok; i++){
                   list[last] = line[i];
                   last++;
                 }
                 break;
// default       : Programming_Error("unexpected compilation error", 7000)
 }
 return(1);
}

//------------------------------------------------------------------------------
// erase_func_tree - delete the last function tree created
//------------------------------------------------------------------------------
void FUNC_::erase_func_tree()
{
 if (dimension != 0) {
   delete[] list;
   dimension = 0;
   last      = 0;
   mark      = 0;
 }
}

//------------------------------------------------------------------------------
// eval_func - evaluate a function
// in: _i  - function index
//     aut - current automaton
//     sts - current state
//     std - target state
//------------------------------------------------------------------------------
int FUNC_::eval_func(int _i, int aut, int sts, int std)
{
 double stack[40];  
 int    top = -1;   

 create_func_tree(_i);
 for (int i=0; i<last; i++) {
   switch(list[i].op) {
   case Plus    : top--; stack[top] += stack[top+1];
                  break;
   case Minus   : top--; stack[top] -= stack[top+1];
                  break;
   case Times   : top--; stack[top] *= stack[top+1];
                  break;
   case Slash   : top--; stack[top] = exp(log(stack[top])-log(stack[top+1])); //stack[top] /= stack[top+1];
                  break;
   case Div     : top--; stack[top] =  (int) stack[top] / stack[top+1];
                  break;
   case Mod     : top--; stack[top] = (int)stack[top] % (int)stack[top+1];
                  break;
   case Mini    : top--; if (stack[top] > stack[top+1])
                           stack[top] = stack[top+1];
                  break;
   case Maxi    : top--; if (stack[top] < stack[top+1])
                           stack[top] = stack[top+1];
                  break;
   case Eq      : top--; stack[top] = double(stack[top] == stack[top+1]);
                  break;
   case Neq     : top--; stack[top] = double(stack[top] != stack[top+1]);
                  break;
   case Gret    : top--; stack[top] = double(stack[top] > stack[top+1]);
                  break;
   case Less    : top--; stack[top] = double(stack[top] < stack[top+1]);
                  break;
   case Geq     : top--; stack[top] = double(stack[top] >= stack[top+1]);
                  break;
   case Leq     : top--; stack[top] = double(stack[top] <= stack[top+1]);
                  break;
   case And     : top--; stack[top] = stack[top] && stack[top+1];
                  break;
   case Or      : top--; stack[top] = stack[top] || stack[top+1];
                  break;
   case Xor     : top--; stack[top] = ( !(stack[top]) &&   stack[top+1]  ) ||
                                        ( stack[top]  && !(stack[top+1]) );
                  break;
   case Imp     : top--; stack[top] = ( stack[top] && stack[top+1] ) || !(stack[top]);
                  break;
   case Dimp    : top--; stack[top] = ( stack[top] && stack[top+1] ) ||
                                      ( !(stack[top]) && !(stack[top+1]) );
                  break;
   case Not     : stack[top] = !(stack[top]) ;
                  break;
   case Cnst    : top++; stack[top] = list[i].val;
                  break;                 
   case Aut     : top++; stack[top] = aut;
                  break;   
   case Sts     : top++; stack[top] = sts; 
                  break;
   case Std     : top++; stack[top] = std;      
                  break;   
   }
 }
 if (stack[0] != (int)stack[0])
   cerr << " Semantical Warning - Function \"" << id[_i] << "\" (** The function result was truncated. Please, verify your function **). Error 703\n";
 return(stack[0]);
}

//------------------------------------------------------------------------------
// resize - alloc space for more functions
//------------------------------------------------------------------------------
void FUNC_::resize()
{ 
  char **_id;
  char **_exp;

  n = n + 20;
  _id  = new char*[n];
  _exp = new char*[n];
  
  for (int i=0; i<ind; i++){ 
    _id[i]  = new char[MAX_ID_NAME];
    _exp[i] = new char[MAX_EXP_SIZE];

    strcpy(_id[i],id[i]);
    strcpy(_exp[i],expr[i]);
  }
    // delete old structures
  for (int i=0; i<ind; i++){
    if (id[i])
      delete[] id[i];
    if (expr[i])
      delete[] expr[i];
  }
  if (id)
    delete[] id;
  if (expr)
    delete[] expr;
      
  id  = _id;
  expr = _exp;
}


//------------------------------------------------------------------------------
// resize_stack - alloc space for more stack functions or operators
//------------------------------------------------------------------------------
void FUNC_::resize_stack()
{ 
  char **_func;
  char **_op;

  if (n_func <= ind_func){
    n_func = n_func + 20;
    _func  = new char*[n_func];
    // alloc new strutures and copy identifiers
    for (int i=0; i<ind_func; i++){ 
      _func[i] = new char[MAX_EXP_SIZE];
      strncpy(_func[i],func[i],MAX_EXP_SIZE);
    }
    // delete old structures
    for (int i=0; i<ind_func; i++)
      if (func[i])
        delete[] func[i];
    if (func)
      delete[] func;
    func = _func;
  }

  if (n_op <= ind_op){
    n_op = n_op + 20;
    _op  = new char*[n_op];
    // alloc new strutures and copy identifiers
    for (int i=0; i<ind_op; i++){ 
      _op[i] = new char[MAX_ID_NAME];
      strncpy(_op[i],op[i],MAX_ID_NAME);
    }
    // delete old structures
    for (int i=0; i<ind_op; i++)
      if (op[i])
        delete[] op[i];
    if (op)
      delete[] op;
    op = _op;
  }
}

