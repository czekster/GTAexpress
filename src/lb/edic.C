//====================================================================================//
//                                                                                    //
//                               Event Dictionary                                     //
//                                                                                    //
//====================================================================================//
//  This File:   edic.h                      Tool: C++                                //
//  Software:    PEPS                        Advisor: Paulof@inf.pucrs.br             //
//  Doc:                                                                              //
//====================================================================================//
//  Creation:    30/Jan/03                   by: lbrenner@inf.pucrs.br                //
//  Last Change: 30/Jan/03                   by: lbrenner@inf.pucrs.br                //
//====================================================================================//
//  This class is used to store the event dictinary obtained from SAN textual input   //
// file in the function form.                                                         //
//====================================================================================//  

#include "sanfile.h"

//------------------------------------------------------------------------------
// Contructor 
//------------------------------------------------------------------------------
EDIC::EDIC()
{
  name = NULL;
  type = NULL;
  rate = NULL;
  f_rate = NULL;
  n_event = 0;
  ind = 0;
  mst = NULL;
}

//------------------------------------------------------------------------------
// Destructor 
//------------------------------------------------------------------------------
EDIC::~EDIC()
{
  for(int i=0;i<n_event;i++){
    if(name[i])
      delete[] name[i];
    if(rate[i])
      delete[] rate[i];
  }

  if(name)
    delete[] name;
  if(type)
    delete[] type;
  if(rate)
    delete[] rate;
  if(f_rate)
    delete[] f_rate;
  if(mst)
    delete[] mst;
  n_event = 0;
  ind = 0;
}

//------------------------------------------------------------------------------
// put_n_event - put the number of events
// i _n_event - number of events
//------------------------------------------------------------------------------
int EDIC::put_n_event(int _n_event)
{
  n_event = _n_event;
  ind = 0;
  if(n_event>0){
    name = new char*[n_event];
    type = new int[n_event];
    rate = new char*[n_event];
    f_rate = new int[n_event];
    mst = new int[n_event];

    if(name && type && rate && mst && f_rate){
      for(int i=0; i<n_event; i++){
		name[i] = new char[MAX_EVENT_NAME];
		rate[i] = new char[64];
		if(name[i] && rate[i]){
		  strcpy(name[i],"");
		  strcpy(rate[i],"");
		  type[i] = -1;
		  f_rate[i] = 0;
		  mst[i] = -1;
		}      
        else{
          Programming_Error1(Parser_02);	
          return(0);
        }
      }
      return(1);
    }
    else{
      Programming_Error1(Parser_02);	
      return(0);
    }
  }
  return(0);
}

//------------------------------------------------------------------------------
// put_event - put the event
// i _name - event name
// i _type - event type
// i _rate - event rate
// i _f_rate - identifier rate
//------------------------------------------------------------------------------
int EDIC::put_event(char *_name, int _type, char *_rate, int _f_rate)
{
  strncpy(name[ind],_name,MAX_EVENT_NAME);
  name[ind][MAX_EVENT_NAME-1] = '\0';
  strncpy(rate[ind],_rate,64);
  rate[ind][63] = '\0';
  type[ind] = _type;
  f_rate[ind] = _f_rate;
  mst[ind] = -1;
  ind++;
  return(ind-1);
}

//------------------------------------------------------------------------------
// put_mst - Put master flag
// i _name - event name
// i _aut - master automaton 
//------------------------------------------------------------------------------
void EDIC::put_mst(char *_name, int _aut)
{
  for(int i=0; i<n_event; i++){
    if(!strcmp(name[i],_name)){
      mst[i] = _aut;
    }
  }
}

//------------------------------------------------------------------------------
// get_type - get the event type
// i _name - event name
//------------------------------------------------------------------------------
int EDIC::get_type(char *_name)
{
  for(int i=0; i<n_event; i++){
    if(!strcmp(name[i],_name)){
      return(type[i]);
    }
  }
  return(-1);
}

//------------------------------------------------------------------------------
// get_f_rate - get the event rate (identifier)
// i _name - event name
//------------------------------------------------------------------------------
int EDIC::get_f_rate(char *_name)
{
  for(int i=0; i<n_event; i++){
    if(!strcmp(name[i],_name)){
      return(f_rate[i]);
    }
  }
  return(-1);
}

//------------------------------------------------------------------------------
// get_rate - get the event rate 
// i _name - event name
// o _rate - event rate
//------------------------------------------------------------------------------
int EDIC::get_rate(char *_name, char *_rate)
{
  for(int i=0; i<n_event; i++){
    if(!strcmp(name[i],_name)){
      strcpy(_rate,rate[i]);
      return(0);
    }
  }
  return(-1);
}

//------------------------------------------------------------------------------
// get_mst - get the master flag
// i _name - event name
//------------------------------------------------------------------------------
int EDIC::get_mst(char *_name)
{
  for(int i=0; i<n_event; i++){
    if(!strcmp(name[i],_name)){
      return(mst[i]);
    }
  }
  return(-1);
}

