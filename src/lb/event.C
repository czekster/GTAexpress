//====================================================================================//
//                                                                                    //
//                                Events Structure                                    //
//                                                                                    //
//====================================================================================//
//  This File:   event.C                     Tool: C++                                //
//  Software:    PEPS                        Advisor: Paulof@inf.pucrs.br             //
//  Doc:                                                                              //
//====================================================================================//
//  Creation:    18/Jun/02                   by: lbrenner@inf.pucrs.br                //
//  Last Change: 18/Jun/02                   by: lbrenner@inf.pucrs.br                //
//====================================================================================//
//  This class is used to store the data obtained from SAN textual input file in the  //
// events form (local or synchronized).                                               //
//====================================================================================//  

#include "sanfile.h"

//------------------------------------------------------------------------------
// Contructor 
//------------------------------------------------------------------------------
EVENT::EVENT()
{
  strcpy(name,"");
  strcpy(rate,"0");
  strcpy(prob,"1");
  t_rate = 0;
  t_prob = 0;
  i_rate = 1;
  i_prob = 1;
}

//------------------------------------------------------------------------------
// Destructor 
//------------------------------------------------------------------------------
EVENT::~EVENT()
{
//  to do
}

//------------------------------------------------------------------------------
// put_name - put the event name
// i _name - event name
//------------------------------------------------------------------------------
void EVENT::put_name(char *_name)
{
  strncpy(name,_name,MAX_EVENT_NAME);
  name[MAX_EVENT_NAME-1] = '\0';
}

//------------------------------------------------------------------------------
// put_rate - put the event rate and the representation type
// i type - representation type (0=value, 1=identifier)
// i _rate - rate value
// i _i_rate - rate identifier
//------------------------------------------------------------------------------
int EVENT::put_rate(int type, char *_rate, int _i_rate)
{
  if(type){
    i_rate = _i_rate;
    t_rate = type;
    return(1);
  }
  else{
    strcpy(rate,_rate);
    t_rate = type;
    return(1);
  }
  return(0);
}

//------------------------------------------------------------------------------
// put_prob - put the event probability and the representation type
// i type - representation type (0=value, 1=identifier)
// i _prob - probability value
// i _i_prob - probability identifier
//------------------------------------------------------------------------------
int EVENT::put_prob(int type, char *_prob, int _i_prob)
{
  if(type){
    i_prob = _i_prob;
    t_prob = type;
    return(1);
  }
  else{
    strcpy(prob,_prob);
    t_prob = type;
    return(1);
  }
  return(0);
}

//------------------------------------------------------------------------------
// get_name - get the event name
// o _name - event name
//------------------------------------------------------------------------------
void EVENT::get_name(char *_name)
{
  strcpy(_name,name);
}

//------------------------------------------------------------------------------
// get_rate - get the event rate and return the representation type
// o _rate - rate value
// o _i_rate - rate identifier
//------------------------------------------------------------------------------
int EVENT::get_rate(char *_rate, int *_i_rate)
{
  if(t_rate){
    *_i_rate = i_rate;
  }
  else{
    strcpy(_rate,rate);
  }
  return(t_rate);
}

//------------------------------------------------------------------------------
// get_prob - get the event probability and return the representation type
// o _prob - probability value
// o _i_prob - probability identifier
//------------------------------------------------------------------------------
int EVENT::get_prob(char *_prob, int *_i_prob)
{
  if(t_prob){
    *_i_prob = i_prob;
  }
  else{
    strcpy(_prob,prob);
  }
  return(t_prob);
}

