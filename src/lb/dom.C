//====================================================================================//
//                                                                                    //
//                               Dominion Structure                                   //
//                                                                                    //
//====================================================================================//
//  This File:   dom.C                       Tool: C++                                //
//  Software:    PEPS                        Advisor: Paulof@inf.pucrs.br             //
//  Doc:                                                                              //
//====================================================================================//
//  Creation:    30/Jan/03                   by: lbrenner@inf.pucrs.br                //
//  Last Change: 30/Jan/03                   by: lbrenner@inf.pucrs.br                //
//====================================================================================//
//  This class is used to store the dominion obtained from SAN textual input file in  //
// the function form.                                                                 //
//====================================================================================//  

#include "sanfile.h"

//------------------------------------------------------------------------------
// Contructor 
//------------------------------------------------------------------------------
DOM::DOM()
{
  name = NULL;
  first = NULL;
  last = NULL;
  n = 0;
  ind = 0;
}

//------------------------------------------------------------------------------
// Destructor 
//------------------------------------------------------------------------------
DOM::~DOM()
{
  for(int i=0;i<n;i++){
    if(name[i])
      delete[] name[i];
  }

  if(name)
    delete[] name;
  if(first)
    delete[] first;
  if(last)
    delete[] last;
    
}

//------------------------------------------------------------------------------
// alloc - alloc the function space
// i _n - number of positions
//------------------------------------------------------------------------------
int DOM::alloc(int _n)
{ 
  name = new char*[_n];
  first = new int[_n];
  last = new int[_n];
  
  if(name && first && last){
    for(int i=0;i<_n;i++){
      name[i] = new char[MAX_FUNC_NAME];
      if(!name[i]){
        Programming_Error1(Parser_202);  
        return(0);
      }
      else{
        strcpy(name[i],"");
        first[i] = 0;
        last[i] = 0;
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
int DOM::free()
{ 
  for(int i=0;i<n;i++){
    if(name[i])
      delete [] name[i];
  }
  
  if(name)
    delete [] name;
  if(first)
    delete [] first;
  if(last)
    delete [] last;
  return 1;//ERR_VISUAL_C++
}

//------------------------------------------------------------------------------
// put_name - put the dominion name
// i _name - dominion name
//------------------------------------------------------------------------------
int DOM::put_name(char *_name)
{
  for(int i=0; i<ind; i++){
    if(!strcmp(name[i],_name))
      return(-1);
  }
  if(ind==n){
    resize();
  }
  strncpy(name[ind],_name,MAX_FUNC_NAME);
  name[ind][MAX_FUNC_NAME-1] = '\0';
  return(ind-1);
}

//------------------------------------------------------------------------------
// put_dom - put the dominion limits
// i _first - initial limit
// i _last - final limit
//------------------------------------------------------------------------------
int DOM::put_dom(int _first, int _last)
{
  if(_first>_last){
    return(-1);
  }
  if(ind==n){
    resize();
  }
  first[ind] = _first;
  last[ind] = _last;
  ind++;
  return(ind-1);
}

//------------------------------------------------------------------------------
// get_name - get the dominion name
// i i -  dominion index
// o _name - dominion name
//------------------------------------------------------------------------------
int DOM::get_name(int i, char *_name)
{
  if(i<ind){
    strcpy(_name,name[i]);
  }
  else{
    return(0);
  }
  return(1);
}

//------------------------------------------------------------------------------
// get_name - get the dominion index by name
// i _name - dominion name
//------------------------------------------------------------------------------
int DOM::get_name(char *_name)
{
  for(int i=0; i<ind; i++){
    if(!strcmp(_name,name[i])){
      return(i);
    }
  }
  return(-1);
}

//------------------------------------------------------------------------------
// get_first - get the initial limit
// i i - dominion index
//------------------------------------------------------------------------------
int DOM::get_first(int i)
{
  if(i<ind){
    return(first[i]);
  }
  else{
    return(0);
  }
}

//------------------------------------------------------------------------------
// get_first - get the initial limit by name
// i _name - dominion name
//------------------------------------------------------------------------------
int DOM::get_first(char *_name)
{
  for(int i=0; i<ind; i++){
    if(!strcmp(_name,name[i])){
      return(first[i]);
    }
  }
  return(-1);
}
//------------------------------------------------------------------------------
// get_last - get the final limit
// i i - dominion index
//------------------------------------------------------------------------------
int DOM::get_last(int i)
{
  if(i<ind){
    return(last[i]);
  }
  else{
    return(0);
  }
}

//------------------------------------------------------------------------------
// get_last - get the final limit by name
// i _name - dominion name
//------------------------------------------------------------------------------
int DOM::get_last(char *_name)
{
  for(int i=0; i<ind; i++){
    if(!strcmp(_name,name[i])){
      return(last[i]);
    }
  }
  return(-1);
}

//------------------------------------------------------------------------------
// get_dom_interv - get the dominion interval
// i i - dominion index
// o _first - initial limit
// o _last - final limit
//------------------------------------------------------------------------------
int DOM::get_dom_interv(int i, int *_first, int *_last)
{
  if(i<ind){
    *_first = first[i];
    *_last = last[i];
    return(1);
  }
  return(-1);
}

//------------------------------------------------------------------------------
// get_n_dom - get the number of dominions
//------------------------------------------------------------------------------
int DOM::get_n_dom()
{
  return(ind);
}

//------------------------------------------------------------------------------
// resize - alloc more space for dominions
//------------------------------------------------------------------------------
int DOM::resize()
{ 
  char **_name;
  int *_first;
  int *_last;


  n = n * 2;
  _name  = new char*[n];
  _first = new int[n];
  _last  = new int[n];
  

  if(_name && _first && _last){
    for(int i=0;i<n;i++){
      _name[i] = new char[MAX_FUNC_NAME];
      if(!_name[i]){
        return(0);
      }
      else{
        strcpy(_name[i],"");
        _first[i] = 0;
        _last[i] = 0;
      }       
    }
    for(int i=0;i<ind;i++){
      strncpy(_name[i],name[i],MAX_FUNC_NAME);
      _first[i] = first[i];
      _last[i] = last[i];
    }
    for(int i=0;i<ind;i++){
      if(name[i])
        delete[] name[i];
    }

    if(name)
      delete[] name;
    if(first)
      delete[] first;
    if(last)
      delete[] last;
      
    name = _name;
    first = _first;
    last = _last;
    return(1);
  }
  else{
    return(0);
  }
}

