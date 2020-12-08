//====================================================================================//
//                                                                                    //
//                               Function Structure                                   //
//                                                                                    //
//====================================================================================//
//  This File:   func.C                      Tool: C++                                //
//  Software:    PEPS                        Advisor: Paulof@inf.pucrs.br             //
//  Doc:                                                                              //
//====================================================================================//
//  Creation:    18/Jun/02                   by: lbrenner@inf.pucrs.br                //
//  Last Change: 18/Jun/02                   by: lbrenner@inf.pucrs.br                //
//====================================================================================//
//  This class is used to store the data obtained from SAN textual input file in the  //
// function form.                                                                     //
//====================================================================================//  

#include "sanfile.h"

extern NET *net;
//------------------------------------------------------------------------------
// Contructor 
//------------------------------------------------------------------------------
FUNC::FUNC()
{
  name = NULL;
  exp = NULL;
  n = 0;
  ind = 0;
}

//------------------------------------------------------------------------------
// Destructor 
//------------------------------------------------------------------------------
FUNC::~FUNC()
{
  for(int i=0;i<n;i++){
    if(name[i])
      delete[] name[i];
    if(exp[i])
     delete[] exp[i];  
  }

  if(name)
    delete[] name;
  if(exp)
    delete[] exp;
    
}

//------------------------------------------------------------------------------
// alloc - alloc the function space
// i _n - number of positions
//------------------------------------------------------------------------------
int FUNC::alloc(int _n)
{ 
  name = new char*[_n];
  exp = new char*[_n];
  
  if(name && exp){
    for(int i=0;i<_n;i++){
      name[i] = new char[MAX_FUNC_NAME];
      exp[i] = new char[MAX_EXP_SIZE];
      if(!(name[i] && exp[i])){
        Programming_Error1(Parser_202);  
        return(0);
      }
      else{
        strcpy(name[i],"");
        strcpy(exp[i],"");        
      }
    }
    n = _n;
    ind = 0;
    return(1);
  }
  else{
    Programming_Error1(Parser_202);  
    return(0);
  }
}

//------------------------------------------------------------------------------
// free - free the function space
//------------------------------------------------------------------------------
int FUNC::free()
{ 
  for(int i=0;i<n;i++){
    if(name[i])
      delete [] name[i];
    if(exp[i])
      delete [] exp[i];  
  }
  
  if(name)
    delete [] name;
  if(exp)
    delete [] exp;
  return 1;//ERR_VISUAL_C++
}

//------------------------------------------------------------------------------
// put_name - put the function name
// i _name - function name
//------------------------------------------------------------------------------
int FUNC::put_name(char *_name)
{
  if(ind==n){
    resize();
  }
  strncpy(name[ind],_name,MAX_FUNC_NAME);
  name[ind][MAX_FUNC_NAME-1] = '\0';
  return(ind-1);
}


//------------------------------------------------------------------------------
// put_exp - put the function expression
// i _exp - expression
//------------------------------------------------------------------------------
int FUNC::put_exp(char *_exp)
{
  if(ind==n){
    resize();
  }
  strncpy(exp[ind],_exp,MAX_EXP_SIZE);
  exp[ind][MAX_EXP_SIZE-1] = '\0';
  ind++;
  return(ind-1);
}

//------------------------------------------------------------------------------
// get_name - get the function name
// i i -  function index
// o _name - function name
//------------------------------------------------------------------------------
int FUNC::get_name(const int i, char *_name)
{
  if(i<ind){
    strncpy(_name,name[i],MAX_FUNC_NAME);
    _name[MAX_FUNC_NAME-1] = '\0';
  }
  else{
    return(0);
  }
  return(1);
}

//------------------------------------------------------------------------------
// get_name - get the function index by name
// i _name - function name
//------------------------------------------------------------------------------
int FUNC::get_name(char *_name)
{
  for(int i=0; i<ind; i++){
    if(!strcmp(_name,name[i])){
      return(i);
    }
  }
  return(-1);
}

//------------------------------------------------------------------------------
// get_exp - get the function expression
// i i - function index
// o _exp - expression
//------------------------------------------------------------------------------
int FUNC::get_exp(int i, char *_exp)
{
  if(i<ind){
    strcpy(_exp,exp[i]);
  }
  else{
    return(0);
  }
  return(1);
}

//------------------------------------------------------------------------------
// get_exp - get the function expression by name
// i _name - function name
// o _exp - expression
//------------------------------------------------------------------------------
int FUNC::get_exp(char *_name, char *_exp)
{
  for(int i=0; i<ind; i++){
    if(!strcmp(_name,name[i])){
      strcpy(_exp,exp[i]);
      return(i);
    }
  }
  return(-1);
}

//------------------------------------------------------------------------------
// get_exp - get the function index by expression
// i _exp - expression
//------------------------------------------------------------------------------
int FUNC::get_exp(char *_exp)
{
  for(int i=0; i<ind; i++){
    if(!strcmp(_exp,exp[i]))
      return(i);
  }
  return(-1);
}

//------------------------------------------------------------------------------
// get_n_exp - get the number of expression
//------------------------------------------------------------------------------
int FUNC::get_n_exp()
{
  return(ind);
}

//------------------------------------------------------------------------------
// exp_value - get the value of the expression 
// i i - expression index
// o _value - expression value
//------------------------------------------------------------------------------
int FUNC::exp_value(int i, int *_value)
{
  int p, flag;
  char aux[MAX_EXP_SIZE];
  
  if(i<ind){
    strcpy(aux, exp[i]);
    p = 0;
    flag = 1;
    while(aux[p]!='\0'){
      if(!(aux[p]>='0' && aux[p]<='9')){
        flag = 0;
      }
      p++;
    }
    if(flag){
      *_value=atoi(aux);
      return(1);
    }
  }
  return(0);
}

//------------------------------------------------------------------------------
// exp_value - get the value of the expression 
// i i - expression index
// o _value - expression value
//------------------------------------------------------------------------------
int FUNC::exp_value(int i, double *_value)
{
  int p, flag;
  char aux[MAX_EXP_SIZE];
  
  if(i<ind){
    strcpy(aux, exp[i]);
    p = 0;
    flag = 1;
    while(aux[p]!='\0'){
      if(!(aux[p]>='0' && aux[p]<='9')){
        if((aux[p]!='-' && aux[p]!='.' && aux[p]!='+' && aux[p]!='e' && aux[p]!='E')){
          flag = 0;
        }
      }
      p++;
    }
    if(flag){
      *_value=atof(aux);
      return(1);
    }
  }
  return(0);
}

//------------------------------------------------------------------------------
// rm_cnst - Remove constant values
//------------------------------------------------------------------------------
int FUNC::rm_cnst()
{
  for(int i=0; i<ind; i++){
    if(!rm_cnst(i))
      return(0); 
  }	
  return(1);
}

//------------------------------------------------------------------------------
// resize - alloc more space for functions
//------------------------------------------------------------------------------
int FUNC::resize()
{ 
  char **_name;
  char **_exp;


  n = n * 2;
  _name = new char*[n];
  _exp = new char*[n];
  

  if(_name && _exp){
    for(int i=0;i<n;i++){
      _name[i] = new char[MAX_FUNC_NAME];
      _exp[i] = new char[MAX_EXP_SIZE];
      if(!(_name[i] && _exp[i])){
        return(0);
      }
      else{
        strcpy(_name[i],"");
        strcpy(_exp[i],""); 
      }       
    }
    for(int i=0;i<ind;i++){
      strncpy(_name[i],name[i],MAX_FUNC_NAME);
      strncpy(_exp[i],exp[i],MAX_EXP_SIZE);
    }
    for(int i=0;i<ind;i++){
      if(name[i])
        delete[] name[i];
      if(exp[i])
        delete[] exp[i];
    }

    if(name)
      delete[] name;
    if(exp)
      delete[] exp;
    name = _name;
    exp = _exp;
    return(1);
  }
  else{
    return(0);
  }
}

//------------------------------------------------------------------------------
// rm_cnst - Remove constant values
// i i - expression index
//------------------------------------------------------------------------------
int FUNC::rm_cnst(int i)
{
  int p;
  char _name[MAX_FUNC_NAME];
  char *aux, tok[MAX_EXP_SIZE], exp_f[MAX_EXP_SIZE];
 
  aux = exp[i];
  exp_f[0] = '\0';
  
  while(aux[0]!='\0'){
    if(aux[0]=='@'){
      strcat(exp_f,"@");
      if(aux[1]=='$'){
        strcat(exp_f,"$");
        aux++;
      }
      aux+=2;  
      if((aux[0]>='a' && aux[0]<='z') || (aux[0]>='A' && aux[0]<='Z')){
        p = 0;
        while(aux[0]!=' '){
          tok[p]=aux[0];
          p++;
          aux++;
        }
        tok[p]='\0';
        if(net->get_name(tok)<0){
          get_name(i, _name);	
          Semantic_Error4(tok, Parser_110, _name);
          return(0);
        }
        sprintf(exp_f,"%s%d", exp_f, net->get_name(tok));
      }
    }
    else{
      if(aux[0]=='%'){
        strcat(exp_f,"%");	
        if(aux[1]=='$' || aux[1]=='#'){
          sprintf(exp_f,"%s%c", exp_f, aux[1]);
          aux++;
        }
        if(aux[1]=='S' || aux[1]=='P'){
          sprintf(exp_f,"%s%c", exp_f, aux[1]);
          aux++;
        }
        aux+=2;
        if((aux[0]>='a' && aux[0]<='z') || (aux[0]>='A' && aux[0]<='Z')){
          p = 0;
          while(aux[0]!=' '){
            tok[p]=aux[0];
            p++;
            aux++;
          }
          tok[p]='\0';
          if(net->get_name(tok)<0){
            get_name(i, _name);	
            Semantic_Error4(tok, Parser_110, _name);
            return(0);
          }
          sprintf(exp_f,"%s%d", exp_f, net->get_name(tok));
        }
      }
      else{
        if((aux[0]>='a' && aux[0]<='z') || (aux[0]>='A' && aux[0]<='Z')){
          p = 0;
          while(aux[0]!=' '){
            tok[p]=aux[0];
            p++;
            aux++;
          }
          tok[p]='\0';
          if(!(strcmp(tok,"min") && strcmp(tok,"max"))){
            strcat(exp_f,tok);
          }
          else{
            if(net->get_name(tok)<0){
              if(get_name(tok)>=0){
                get_exp(tok,tok);
                sprintf(exp_f,"%s%s", exp_f,tok);
              }
              else{
                if(net->get_ident_name(tok)>=0){
                  net->get_ident_exp(tok,tok);
                  sprintf(exp_f,"%s%s", exp_f,tok);
                }
                else{
                  get_name(i, _name);	
                  Semantic_Error4(tok, Parser_110, _name);
                  return(0);
                }
              }
            }
            else{
              sprintf(exp_f,"%s%d", exp_f, net->get_name(tok));
            }
          }
        }
      }
    }
    sprintf(exp_f,"%s%c",exp_f,aux[0]);
    aux++;  
  }

  strcpy(exp[i],exp_f);
  return(1);
}
