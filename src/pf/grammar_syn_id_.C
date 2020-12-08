#include "grammar_.h"

//------------------------------------------------------------------------------
// id_blk - main identifier / domain function
//------------------------------------------------------------------------------
void GRAMMAR_::id_blk()
{
  char id[MAX_ID_NAME];
  char exp[MAX_EXP_SIZE];

  cout << " Compiling identifier block\n";
  if (get_tk() != TK_IDENTIFIERS)
    Syntactic_error("The symbol \"identifiers\" ");
  get_tk();
  while (TK == TK_ID){
    strcpy(id, tk);
    if (get_tk() != TK_ATTRIB)
      Syntactic_error("=");
    if (get_tk() == TK_LEFT_SQUARE_BRACKET){
      put_tk();
      dom_blk(id);
      if (tour == 1) {
        if (!dom.build_dom(id) || ident.is_func(id))
          Dom_semantic_error(Parser_100, id, 100);
        if (!dom.check_dom(id))
          Dom_semantic_error(Parser_101, id, 101);
       }
    }
    else{
      put_tk();
      exp_blk(id);
      if (tour == 1) 
        if (!func.put_id(id) || dom.is_dom(id))
          Function_semantic_error(Parser_200, id, 200);
      func.build_exp(id); 
      func.get_exp(id, exp);
      ident.put_exp(id, exp);
    }
    if (get_tk() != TK_SEMICOLON)
      Syntactic_error(";");
    get_tk();
  }
  put_tk();
}

//------------------------------------------------------------------------------
// dom_blk - main domain function
//------------------------------------------------------------------------------
void GRAMMAR_::dom_blk(char *_id)
{
  if (get_tk() != TK_LEFT_SQUARE_BRACKET)
    Syntactic_error("[");
  dom_blk2(_id);
  if (get_tk() != TK_RIGHT_SQUARE_BRACKET)
    Syntactic_error("]");
}

//------------------------------------------------------------------------------
// dom_blk2 - domain function to recursive calls
//------------------------------------------------------------------------------
void GRAMMAR_::dom_blk2(char *_id)
{
  dom_int(_id);
  list_dom(_id);
}

//------------------------------------------------------------------------------
// list_dom - provide a list structure in domains
//------------------------------------------------------------------------------
void GRAMMAR_::list_dom(char *_id)
{
  if (get_tk() == TK_COMMA){
    dom_blk2(_id);  
    return;
  }
  put_tk();
  return;
}

//------------------------------------------------------------------------------
// dom_int - internal structure in domain list, it can to be only one element or 
//           an interval
//------------------------------------------------------------------------------
void GRAMMAR_::dom_int(char *_id)
{
  char first[MAX_ID_NAME];
  char last[MAX_ID_NAME];
  
  get_tk();
  if (TK != TK_ID && TK != TK_INTEGER)
    Syntactic_error("An identifier or a number");
  if (TK == TK_ID){
    if (!ident.is_func(tk))
      Dom_semantic_error(Parser_102, _id, tk, 102);
    if (!ident.is_const(tk))
      Dom_semantic_error(Parser_103, _id, tk, 103);
    sprintf(first, "%d", ident.eval(tk));    
  }
  else
    strcpy(first, tk);  
 
  if (get_tk() == TK_DOTS){
    get_tk();
    if (TK != TK_ID && TK != TK_INTEGER)
      Syntactic_error("An identifier or a number");    
    if (TK == TK_ID){
      if (!ident.is_func(tk))
        Dom_semantic_error(Parser_102, _id, tk, 104);
      if (!ident.is_const(tk))
        Dom_semantic_error(Parser_103, _id, tk, 105);
      sprintf(last, "%d", ident.eval(tk));    
      if (tour == 1) 
        if (!dom.put_interv(first, last))
          Dom_semantic_error(Parser_101, _id, 106);
    }
    else
      if (tour == 1) 
        if (!dom.put_interv(first, tk))
          Dom_semantic_error(Parser_101, _id, 107);
  }
  else{
    if (tour == 1) dom.put_elem(first);
    put_tk();
  }
}

//------------------------------------------------------------------------------
// exp_blk - expression function use to recursive calls
//------------------------------------------------------------------------------
void GRAMMAR_::exp_blk(char *id_func)
{
  elem(id_func);
  exp(id_func);
}

//------------------------------------------------------------------------------
// exp_blk2 - expression function use to recursive calls in a list
//------------------------------------------------------------------------------
void GRAMMAR_::exp_blk2(char *id_func)
{
  exp_blk(id_func);
  list_blk(id_func);
}

//------------------------------------------------------------------------------
// list_blk - provide a list structure into expressions
//------------------------------------------------------------------------------
void GRAMMAR_::list_blk(char *id_func)
{
  if (get_tk() == TK_COMMA){
    exp_blk2(id_func);  
    return;
  }
  put_tk();
  return;
}

//------------------------------------------------------------------------------
// exp - provide the basic structure of an expression (element operator expression)
//------------------------------------------------------------------------------
void GRAMMAR_::exp(char *id_func)
{
  if (bin_op()){
    func.put_op(tk);
    elem(id_func);
    func.put_elem();
    exp(id_func);
  }
}

//------------------------------------------------------------------------------
// elem - define an element of an expression
//------------------------------------------------------------------------------
void GRAMMAR_::elem(char *id_func)
{
  char id[MAX_ID_NAME];
  char op[MAX_ID_NAME];
  char first[MAX_ID_NAME];
  char last[MAX_ID_NAME];
  int  i_first, i_last;
  static int aut_cur;

  switch (get_tk()){
    case TK_LEFT_PARENTHESIS: exp_blk(id_func); 
                              if (get_tk() != TK_RIGHT_PARENTHESIS)
                                Syntactic_error(")");
                              return;
    case TK_PLUS:             elem(id_func);
                              return;
    case TK_MINUS:            func.put_elem("-1");
                              func.put_op("*");
                              elem(id_func);
                              func.put_elem();
                              return;
    case TK_NOT:              func.put_op(tk);
                              elem(id_func);
                              func.put_elem();
                              return;
    case TK_MIN:              if (get_tk() != TK_LEFT_PARENTHESIS)
                                Syntactic_error("(");
                              exp_blk2(id_func);
                              if (get_tk() != TK_RIGHT_PARENTHESIS)
                                Syntactic_error(")");
                              func.put_elem("min");
                              return;
    case TK_MAX:              if (get_tk() != TK_LEFT_PARENTHESIS)
                                Syntactic_error("(");
                              exp_blk2(id_func);
                              if (get_tk() != TK_RIGHT_PARENTHESIS)
                                Syntactic_error(")");
                              func.put_elem("max");
                              return;
    case TK_REWARD:           aut_cur = id_aut(id, id_func);
                              if (tour == 1) sprintf(op, "\%%s", id);
                              else sprintf(op, "\%%d", ndic.get_aut(id));
                              func.put_elem(op);
                              return;
    case TK_STATE_NUM:        aut_cur = id_aut(id, id_func);
                              if (tour == 1) sprintf(op, "@%s", id);
                              else sprintf(op, "@%d", ndic.get_aut(id));
                              func.put_elem(op);
                              return;
    case TK_NBR:              switch (interval(first, last, id_func)){ 
                                case DOMAIN:   id_stt(id, id_func);
                                               cut_dom_id(first, last);
//                                               dom.get_interv(last, 0, i_first, i_last);
i_first = dom.get_first_pos(last);
i_last = dom.get_last_pos(last);
                                               if (tour == 1) sprintf(op, "@$%s %s[%d] %s[%d]", id, first, i_first, first, i_last);
                                               else {
                                                 i_first = ndic.get_aut_rep(first, i_first);
                                                 i_last  = ndic.get_aut_rep(first, i_last);
                                                 if (!ndic.check_stt_in_aut(id, i_first, i_last))
                                                   Function_semantic_warning(Parser_201, id_func, id, 201);
                                                 sprintf(op, "@$%d %d %d", ndic.get_stt(i_first, id), i_first, i_last);
                                               }
                                               func.put_elem(op);
                                               for (int i=1; i<dom.get_n_interv(last)-1; i++){
                                                 dom.get_interv(last, i, i_first, i_last);
                                                 if (tour == 1) sprintf(op, "@$%s %s[%d] %s[%d]", id, first, i_first, first, i_last);
                                                 else {
                                                   i_first = ndic.get_aut_rep(first, i_first);
                                                   i_last  = ndic.get_aut_rep(first, i_last);
                                                   if (!ndic.check_stt_in_aut(id, i_first, i_last))
                                                     Function_semantic_warning(Parser_201, id_func, id, 202);
                                                   sprintf(op, "@$%d %d %d", ndic.get_stt(i_first, id), i_first, i_last);
                                                 }
                                                 func.put_op("+");
                                                 func.put_elem(op);
                                               }
                                               return;
                                case INTERVAL: id_stt(id, id_func);
                                               if (tour == 1) sprintf(op,"@$%s %s %s", id, first, last);
                                               else{
                                                 i_first = ndic.get_aut(first);
                                                 i_last  = ndic.get_aut(last);
                                                 if (!ndic.check_stt_in_aut(id, i_first, i_last))
                                                   Function_semantic_warning(Parser_201, id_func, id, 203);
                                                 sprintf(op, "@$%d %d %d", ndic.get_stt(i_first, id), i_first, i_last);
                                               }
                                               func.put_elem(op);
                                               return;
                                case EMPTY:    id_stt(id, id_func);
                                               if (tour == 1) sprintf(op,"@$%s 0 %d", id, (n_aut-1)); 
                                               else
                                                 if (!ndic.check_stt_in_aut(id, 0, (n_aut-1)))
                                                   Function_semantic_warning(Parser_201, id_func, id, 204);
                                                 sprintf(op, "@$%d 0 %d", ndic.get_stt(0, id), (n_aut-1));
                                               func.put_elem(op);
                                               return;
                              }
    case TK_SUM_RW:           switch (interval1(first, last, id_func)){ 
                                case DOMAIN:   id_stat(id, id_func) == EMPTY ? strcpy(op, "%$") : strcpy(op, "%S");
                                               cut_dom_id(first, last);
                                               dom.get_interv(last, 0, i_first, i_last);
                                               if (tour == 1) sprintf(op, "%s%s %s[%d] %s[%d]", op,  id, first, i_first, first, i_last);
                                               else {
                                                 i_first = ndic.get_aut_rep(first, i_first);
                                                 i_last  = ndic.get_aut_rep(first, i_last);
                                                 if (!ndic.check_stt_in_aut(id, i_first, i_last))
                                                   Function_semantic_warning(Parser_201, id_func, id, 205);
                                                 sprintf(op, "%s%d %d %d", op, ndic.get_stt(i_first, id), i_first, i_last);
                                               }
                                               func.put_elem(op);
                                               for (int i=1; i<dom.get_n_interv(last)-1; i++){
                                                 dom.get_interv(last, i, i_first, i_last);
                                                 if (tour == 1) sprintf(op, "%s%s %s[%d] %s[%d]", op, id, first, i_first, first, i_last);
                                                 else {
                                                   i_first = ndic.get_aut_rep(first, i_first);
                                                   i_last  = ndic.get_aut_rep(first, i_last);
                                                   if (!ndic.check_stt_in_aut(id, i_first, i_last))
                                                     Function_semantic_warning(Parser_201, id_func, id, 206);
                                                   sprintf(op, "%s%d %d %d", op, ndic.get_stt(i_first, id), i_first, i_last);
                                                 }
                                                 func.put_op("+");
                                                 func.put_elem(op);
                                               }
                                               return;
                                case INTERVAL: id_stat(id, id_func) == EMPTY ? strcpy(op, "%$") : strcpy(op, "%S");
                                               if (tour == 1) sprintf(op,"%s%s %s %s", op, id, first, last);
                                               else{
                                                 i_first = ndic.get_aut(first);
                                                 i_last  = ndic.get_aut(last);
                                                 if (!ndic.check_stt_in_aut(id, i_first, i_last))
                                                   Function_semantic_warning(Parser_201, id_func, id, 207);
                                                 sprintf(op, "%s%d %d %d", op, ndic.get_stt(i_first, id), i_first, i_last);
                                               }
                                               func.put_elem(op); 
                                               return;
                              }
    case TK_PROD_RW:          switch (interval1(first, last, id_func)){ 
                                case DOMAIN:   id_stat(id, id_func) == EMPTY ? strcpy(op,"%#") : strcpy(op,"%P");
                                               cut_dom_id(first, last);
                                               dom.get_interv(last, 0, i_first, i_last);
                                               if (tour == 1) sprintf(op, "%s%s %s[%d] %s[%d]", op,  id, first, i_first, first, i_last);
                                               else {
                                                 i_first = ndic.get_aut_rep(first, i_first);
                                                 i_last  = ndic.get_aut_rep(first, i_last);
                                                 if (!ndic.check_stt_in_aut(id, i_first, i_last))
                                                   Function_semantic_warning(Parser_201, id_func, id, 208);
                                                 sprintf(op, "%s%d %d %d", op, ndic.get_stt(i_first, id), i_first, i_last);
                                               }
                                               func.put_elem(op);
                                               for (int i=1; i<dom.get_n_interv(last)-1; i++){
                                                 dom.get_interv(last, i, i_first, i_last);
                                                 if (tour == 1) sprintf(op, "%s%s %s[%d] %s[%d]", op, id, first, i_first, first, i_last);
                                                 else {
                                                   i_first = ndic.get_aut_rep(first, i_first);
                                                   i_last  = ndic.get_aut_rep(first, i_last);
                                                   if (!ndic.check_stt_in_aut(id, i_first, i_last))
                                                     Function_semantic_warning(Parser_201, id_func, id, 208);
                                                   sprintf(op, "%s%d %d %d", op, ndic.get_stt(i_first, id), i_first, i_last);
                                                 }
                                                 func.put_op("+");
                                                 func.put_elem(op);
                                               }
                                               return;
                                case INTERVAL: id_stat(id, id_func) == EMPTY ? strcpy(op,"%#") : strcpy(op,"%P");
                                               if (tour == 1) sprintf(op,"%s%s %s %s", op, id, first, last);
                                               else{
                                                 i_first = ndic.get_aut(first);
                                                 i_last  = ndic.get_aut(last);
                                                 if (!ndic.check_stt_in_aut(id, i_first, i_last))
                                                   Function_semantic_warning(Parser_201, id_func, id, 209);
                                                 sprintf(op, "%s%d %d %d", op, ndic.get_stt(i_first, id), i_first, i_last);
                                               }
                                               func.put_elem(op); 
                                               return;
                              }
    case TK_ID:               strcpy(op, tk);
                              switch (replic(id, id_func)){
                                case INTEGER: sprintf(op, "%s[%s]", op, id);
                                              if (tour == 2){
                                                if (ndic.is_aut(op))
                                                  sprintf(op, "%d", ndic.get_aut(op)); 
                                                else
                                                 if (ndic.is_stt(op))
                                                   sprintf(op, "%d", ndic.get_stt(aut_cur, op)); 
                                                 else
                                                   Function_semantic_error(Parser_202, id_func, op, 210);
                                              }
                                              func.put_elem(op);
                                              return;
                                case EMPTY:   if (ident.is_func(op)){
                                                func.cat_elem(op);
                                                return;
                                              }   
                                              else{
                                                if (tour == 2){
                                                  if (ndic.is_aut(op))
                                                    sprintf(op, "%d", ndic.get_aut(op)); 
                                                  else
                                                    if (ndic.is_stt(op))
                                                      sprintf(op, "%d", ndic.get_stt(aut_cur, op)); 
                                                    else
                                                      Function_semantic_error(Parser_202, id_func, op, 211);
                                                }
                                                func.put_elem(op);
                                                return;
                                              }
                                case DOMAIN: Function_semantic_error(Parser_202, id_func, op, 212);
                              }  
                              return;
    case TK_AUT:              func.put_elem("&a");
                              return;               
    case TK_STS:              func.put_elem("&s");
                              return; 
    case TK_STD:              func.put_elem("&d");
                              return; 
    case TK_REAL:  
    case TK_INTEGER:
    case TK_EXPONENT:         func.put_elem(tk);
                              return;
    default:                  Syntactic_error();
  }
}

int GRAMMAR_::id_stat(char *id, char *id_func)
{
  if (get_tk() == TK_ID){
    put_tk();
    id_stt(id, id_func);
    return(INTEGER);
  }
  id[0] = '\0';
  put_tk();
  return(EMPTY);
}

int GRAMMAR_::id_aut(char *_id, char *id_func)
{
  char rep[MAX_ID_NAME];
  
  if (get_tk() != TK_ID)
    Syntactic_error("An identifier");
  strcpy(_id, tk);
  switch(replic(rep, id_func)){
    case EMPTY:    break;
    case INTEGER:  sprintf(_id, "%s[%s]", _id, rep);
                   break;
    case DOMAIN:   Function_semantic_error(Parser_203, id_func, rep, 213);
                   break;
  }
  if (tour == 2){
    if (!ndic.is_aut(_id))
      Function_semantic_error(Parser_204, id_func, _id, 214);
    return(ndic.get_aut(_id));
  }
  return(0);
}

int GRAMMAR_::id_int_aut(char *_id, char *id_func)
{
  char rep[MAX_ID_NAME];
  char aux[MAX_ID_NAME];
  int  rep_type;
  
  if (get_tk() != TK_ID)
    Syntactic_error("An identifier");
  strcpy(_id, tk);
  switch(replic(rep, id_func)){
    case EMPTY:    rep_type = 0; 
                   break;
    case INTEGER:  rep_type = 0;
                   sprintf(_id, "%s[%s]", _id, rep);
                   break;
    case DOMAIN:   rep_type = 1;
                   if (tour == 1) 
                     sprintf(_id, "%s[%s]", _id, rep);
                   break;
  }
  if (tour == 2){
    if (!rep_type){
      if (!ndic.is_aut(_id))
        Function_semantic_error(Parser_204, id_func, _id, 215);
    }
    else{
      for (int i=0; i<dom.get_length(rep); i++){
        sprintf(aux, "%s[%d]", _id, dom.get_pos(rep, i));
        if (!ndic.is_aut(aux))
          Function_semantic_error(Parser_205, id_func, aux, 216);
      }
      sprintf(_id, "%s[%s]", _id, rep);   
    }
  }
  if(rep_type) 
    return(AUT_DOMAIN); 
  else 
    return(AUT);
}


void GRAMMAR_::id_stt(char *_id, char *id_func)
{
  char rep[MAX_ID_NAME];
  
  if (get_tk() != TK_ID)
    Syntactic_error("An identifier");
  strcpy(_id, tk);
  switch(replic(rep, id_func)){
    case EMPTY:    break;
    case INTEGER:  sprintf(_id, "%s[%s]", _id, rep);
                   break;
    case DOMAIN:   Function_semantic_error(Parser_203, id_func, rep, 217);
                   break;
  }
  if (tour == 2){
    if (!ndic.is_stt(_id))
      Function_semantic_error(Parser_206, id_func, _id, 218);
  }
}

int GRAMMAR_::replic(char *_rep, char *id_func)
{
  int type;

  if (get_tk() != TK_LEFT_SQUARE_BRACKET){
    put_tk();
    return(EMPTY);
  }
  get_tk();
  strcpy(_rep, tk);
  if (TK == TK_INTEGER)
    type = INTEGER;
  else
    if (TK == TK_ID)
      if (dom.is_dom(tk))
        type = DOMAIN;
      else
        if (ident.is_func(tk)){
          if(ident.is_const(tk)){
            type = INTEGER;
            sprintf(_rep, "%d", ident.eval(tk));
          }
          else
            Function_semantic_error(Parser_207, id_func, tk, 219);
        }
        else
          Function_semantic_error(Parser_208, id_func, tk, 220);
    else
      Syntactic_error("An identifier or a number"); 

  if (get_tk() != TK_RIGHT_SQUARE_BRACKET)
    Syntactic_error("]");
  return(type);
}

int GRAMMAR_::interval(char *_first, char *_last, char *id_func)
{
  if (get_tk() == TK_LEFT_SQUARE_BRACKET){
    put_tk();
    return(interval1(_first, _last, id_func));
  }
  _first[0] = '\0';
  _last[0]  = '\0';
  put_tk();
  return(EMPTY);
}

int GRAMMAR_::interval1(char *_first, char *_last, char *id_func)
{
  if (get_tk() != TK_LEFT_SQUARE_BRACKET)
    Syntactic_error("[");
  if (id_int_aut(_first, id_func) == AUT){
    if (get_tk() != TK_DOTS)
      Syntactic_error("..");
    if(id_int_aut(_last, id_func) != AUT)
      Function_semantic_error(Parser_203, id_func, _last, 221);      
  }
  else{
    if (get_tk() == TK_DOTS)
      Function_semantic_error(Parser_209, id_func, 222); 
    else
    	put_tk();
    _last[0] = '\0';
  }    
  if (get_tk() != TK_RIGHT_SQUARE_BRACKET)
    Syntactic_error("]");
  if (_last[0] == '\0') 
    return(DOMAIN);
  else 
    return(INTERVAL);  
}

//------------------------------------------------------------------------------
// bin_op - define a binary operator 
//------------------------------------------------------------------------------
int GRAMMAR_::bin_op()
{
  switch(get_tk()){
    case TK_AND:
    case TK_DIV:
    case TK_DIVI:
    case TK_DOUBLE:  
    case TK_EQUAL:
    case TK_MAJOR:
    case TK_MAJOR_EQUAL:
    case TK_MINOR:
    case TK_MINOR_EQUAL:
    case TK_MINUS:
    case TK_MOD:
    case TK_MULT:
    case TK_OR:
    case TK_PLUS:
    case TK_SIMPLE:
    case TK_UNEQUAL:
    case TK_XOR:          return(1);
    default:              put_tk();
                          return(0);

  }
}

