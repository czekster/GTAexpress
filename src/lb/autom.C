//====================================================================================//
//                                                                                    //
//                                 Automaton Structure                                //
//                                                                                    //
//====================================================================================//
//  This File:   autom.C                     Tool: C++                                //
//  Software:    PEPS                        Advisor: Paulof@inf.pucrs.br             //
//  Doc:                                                                              //
//====================================================================================//
//  Creation:    18/Jun/02                   by: lbrenner@inf.pucrs.br                //
//  Last Change: 18/Jun/02                   by: lbrenner@inf.pucrs.br                //
//====================================================================================//
//  This class is used to store the data obtained from SAN textual input file in the  //
// automaton form.                                                                    //
//====================================================================================//  

#include "sanfile.h"

//------------------------------------------------------------------------------
// Contructor 
//------------------------------------------------------------------------------
AUTOMATON::AUTOMATON()
{
  strcpy(name,"");
  n_state = 0;
  state = NULL;
}

//------------------------------------------------------------------------------
// Destructor 
//------------------------------------------------------------------------------
AUTOMATON::~AUTOMATON()
{
  if(state)
    delete [] state;

}

//------------------------------------------------------------------------------
// free
//------------------------------------------------------------------------------
void AUTOMATON::free()
{
  for(int i=0; i<n_state; i++){
    state[i].free();
  }
  if(state)
    delete [] state;

}

//------------------------------------------------------------------------------
// put_name - put the automaton name
// i _name - automaton name
//------------------------------------------------------------------------------
void AUTOMATON::put_name(char *_name)
{
  strncpy(name,_name,MAX_AUTOMATON_NAME);
  name[MAX_AUTOMATON_NAME-1] = '\0';
}

//------------------------------------------------------------------------------
// put_n_state - put the number of state in automaton and alloc state structure
// i _n_state - number of states
//------------------------------------------------------------------------------
int AUTOMATON::put_n_state(int _n_state)
{
  n_state = _n_state;
  if(n_state>0){
    state = new STATE[n_state];
    if(!state){
      Programming_Error1(Parser_02);	
      return(0);
    }
    return(1);
  }
  return(0);
}

//------------------------------------------------------------------------------
// put_state_name - put the state name in a specific state
// i _state - state number
// i _name - state name
//------------------------------------------------------------------------------
int AUTOMATON::put_state_name(int _state, char *_name)
{
  int flag = 1;
  char aux[MAX_STATE_NAME];
  
  for(int i=0; i<n_state; i++){
    state[i].get_name(aux);	
    if(!strcmp(aux,_name))
      flag = 0;
  }	
  state[_state].put_name(_name);
  return(flag);
}

//------------------------------------------------------------------------------
// put_state_reward - put the state reward in a specific state
// i _state - state number
// i _reward - state reward
//------------------------------------------------------------------------------
void AUTOMATON::put_state_reward(int _state, double _reward)
{
  state[_state].put_reward(_reward);
}

//------------------------------------------------------------------------------
// put_state_n_trans - put the number of transitions in a state
// i _state - state number
// i _n_trans - number of transitions
//------------------------------------------------------------------------------
int AUTOMATON::put_state_n_trans(int _state, int _n_trans)
{
  state[_state].put_n_trans(_n_trans);
  return -1;//ERR_VISUAL_C++
}

//------------------------------------------------------------------------------
// put_trans_dst - this method put the destin state in a transition, if the destin
//                 state don't exist, a error is return (-1)
// i _state - state number
// i type - representation type(ID, ++, --, ==, +=, -=)
// i _dst - destin state
//------------------------------------------------------------------------------
int AUTOMATON::put_trans_dst(int _state, int type, char *_dst)
{
  char aux[MAX_STATE_NAME];
  char sname[MAX_STATE_NAME];
  int index, p;
  int is_replic=false;


  switch(type){
    // ID
    case 1: 
      index = -1;
      for(int i=0;i<n_state;i++){
        state[i].get_name(aux);	
        if(!strcmp(aux,_dst)){
          index = i;
        }
      }
      if(index>=0)
        state[_state].put_trans_dst(index);
      return(index);
    // ++
    case 2:
      if(_state>=(n_state-1))
        return(-1);
      index = 1;        
      state[_state+1].get_name(sname);
      break;
    // --
    case 3:
      if(_state<=0)
        return(-1);
      index = -1;
      state[_state-1].get_name(sname);
      break;
    // ==
    case 4:
      state[_state].put_trans_dst(_state);
      return(_state);
    // += or -=
    case 5:
      index = atoi(_dst);
      if(((_state+index)>=n_state) || ((_state+index)<0))
        return(-1);
      state[_state+index].get_name(sname);
      break;
    // error
    default:
      return(-1);
  }

  state[_state].get_name(aux);
  p = 0;
  while(aux[p]!='\0' && is_replic==false){
    if(aux[p]=='['){
      is_replic = true;
      aux[p] = '\0';
      sname[p] = '\0';
    }
    else{
      p++;
    }
  }
  if(!is_replic){
    return(-1);
  }
  if(!strcmp(sname,aux)){
    state[_state].put_trans_dst(_state+index);
    return(_state+index);
  }
  else{
    return(-1);
  }
}
//------------------------------------------------------------------------------
// put_from_dst - this method put the destin state in a transition, if the destin
//                 state don't exist, a error is return (-1)
// i _state - state number
// i _dst - destin state
//------------------------------------------------------------------------------
int AUTOMATON::put_from_dst(int _state, char *_dst)
{
  char aux[MAX_STATE_NAME];
  ///char sname[MAX_STATE_NAME];
  int index;

  index = -1;
  for(int i=0;i<n_state;i++){
    state[i].get_name(aux);	
    if(!strcmp(aux,_dst)){
      index = i;
    }
  }
  
  if(index>=0)
    state[_state].put_from_dst(index);
  return(index+1);
}

//------------------------------------------------------------------------------
// put_event_n_mst - put the number of master events
// i _state - state number
// i _trans - transition number
// i _n_mst - number of master events
//------------------------------------------------------------------------------
int AUTOMATON::put_event_n_mst(int _state, int _trans, int _n_mst)
{
  return(state[_state].put_event_n_mst(_trans, _n_mst));
}

//------------------------------------------------------------------------------
// put_event_n_sync - put the number of synchronized events
// i _state - state number
// i _trans - transition number
// i _n_sync - number of synchronized events
//------------------------------------------------------------------------------
int AUTOMATON::put_event_n_sync(int _state, int _trans, int _n_sync)
{
  return(state[_state].put_event_n_sync(_trans, _n_sync));
}

//------------------------------------------------------------------------------
// put_event_n_local - put the number of local events
// i _state - state number
// i _trans - transition number
// i _n_local - number of local events
//------------------------------------------------------------------------------
int AUTOMATON::put_event_n_local(int _state, int _trans, int _n_local)
{
  return(state[_state].put_event_n_local(_trans, _n_local));
}

//------------------------------------------------------------------------------
// put_event_n_mst - put the number of master events
// i _state - state number
// i _n_mst - number of master events
//------------------------------------------------------------------------------
int AUTOMATON::put_event_n_mst(int _state, int _n_mst)
{
  return(state[_state].put_event_n_mst(_n_mst));
}

//------------------------------------------------------------------------------
// put_event_n_sync - put the number of synchronized events
// i _state - state number
// i _n_sync - number of synchronized events
//------------------------------------------------------------------------------
int AUTOMATON::put_event_n_sync(int _state, int _n_sync)
{
  return(state[_state].put_event_n_sync(_n_sync));
}

//------------------------------------------------------------------------------
// put_event_n_local - put the number of local events
// i _state - state number
// i _n_local - number of local events
//------------------------------------------------------------------------------
int AUTOMATON::put_event_n_local(int _state, int _n_local)
{
  return(state[_state].put_event_n_local(_n_local));
}

//------------------------------------------------------------------------------
// put_mst_name - put the master event name if the name already exist, a error 
//                is returned but the name is writen
// i _state - state number
// i _trans - transition number
// i _mst - master events number
// i _name - event name
//------------------------------------------------------------------------------
int AUTOMATON::put_mst_name(int _state, int _trans, int _mst, char *_name)
{
  return(state[_state].put_mst_name(_trans, _mst, _name));
}

//------------------------------------------------------------------------------
// put_mst_rate - put the master event rate
// i _state - state number
// i _trans - transition number
// i _mst - master events number
// i type - representation type (0=value, 1=identifier)
// i _rate - rate value
// i _i_rate - rate identifier
//------------------------------------------------------------------------------
int AUTOMATON::put_mst_rate(int _state, int _trans, int _mst, int type, char *_rate, int _i_rate)
{
  return(state[_state].put_mst_rate(_trans, _mst, type, _rate, _i_rate));
}

//------------------------------------------------------------------------------
// put_mst_prob - put the master event probability
// i _state - state number
// i _trans - transition number
// i _mst - master events number
// i type - representation type (0=value, 1=identifier)
// i _prob - probability value
// i _i_prob - probability identifier
//------------------------------------------------------------------------------
int AUTOMATON::put_mst_prob(int _state, int _trans, int _mst, int type, char *_prob, int _i_prob)
{
  return(state[_state].put_mst_prob(_trans, _mst, type, _prob, _i_prob));
}

//------------------------------------------------------------------------------
// put_mst_name - put the master event name if the name already exist, a error 
//                is returned but the name is writen
// i _state - state number
// i _name - event name
//------------------------------------------------------------------------------
int AUTOMATON::put_mst_name(int _state, char *_name)
{
  return(state[_state].put_mst_name(_name));
}

//------------------------------------------------------------------------------
// put_mst_rate - put the master event rate
// i _state - state number
// i type - representation type (0=value, 1=identifier)
// i _rate - rate value
// i _i_rate - rate identifier
//------------------------------------------------------------------------------
int AUTOMATON::put_mst_rate(int _state, int type, char *_rate, int _i_rate)
{
  return(state[_state].put_mst_rate(type, _rate, _i_rate));
}

//------------------------------------------------------------------------------
// put_mst_prob - put the master event probability
// i _state - state number
// i type - representation type (0=value, 1=identifier)
// i _prob - probability value
// i _i_prob - probability identifier
//------------------------------------------------------------------------------
int AUTOMATON::put_mst_prob(int _state, int type, char *_prob, int _i_prob)
{
  return(state[_state].put_mst_prob(type, _prob, _i_prob));
}

//------------------------------------------------------------------------------
// put_sync_name - put the synchronized event name 
// i _state - state number
// i _trans - transition number
// i _sync - synchronized events number
// i _name - event name
//------------------------------------------------------------------------------
int AUTOMATON::put_sync_name(int _state, int _trans, int _sync, char *_name)
{
  return(state[_state].put_sync_name(_trans, _sync, _name));
}

//------------------------------------------------------------------------------
// put_sync_prob - put the synchronized event probability
// i _state - state number
// i _trans - transition number
// i _sync - synchronized events number
// i type - representation type (0=value, 1=identifier)
// i _prob - probability value
// i _i_prob - probability identifier
//------------------------------------------------------------------------------
int AUTOMATON::put_sync_prob(int _state, int _trans, int _sync, int type, char *_prob, int _i_prob)
{
  return(state[_state].put_sync_prob(_trans, _sync, type, _prob, _i_prob));
}

//------------------------------------------------------------------------------
// put_sync_name - put the synchronized event name 
// i _state - state number
// i _name - event name
//------------------------------------------------------------------------------
int AUTOMATON::put_sync_name(int _state, char *_name)
{
  return(state[_state].put_sync_name(_name));
}

//------------------------------------------------------------------------------
// put_sync_prob - put the synchronized event probability
// i _state - state number
// i type - representation type (0=value, 1=identifier)
// i _prob - probability value
// i _i_prob - probability identifier
//------------------------------------------------------------------------------
int AUTOMATON::put_sync_prob(int _state, int type, char *_prob, int _i_prob)
{
  return(state[_state].put_sync_prob(type, _prob, _i_prob));
}

//------------------------------------------------------------------------------
// put_local_name - put the local event name if the name already exist, a error 
//                is returned but the name is writen
// i _state - state number
// i _trans - transition number
// i _local - local events number
// i _name - event name
//------------------------------------------------------------------------------
int AUTOMATON::put_local_name(int _state, int _trans, int _local, char *_name)
{
  return(state[_state].put_local_name(_trans, _local, _name));
}

//------------------------------------------------------------------------------
// put_local_rate - put the local event rate
// i _state - state number
// i _trans - transition number
// i _local - local events number
// i type - representation type (0=value, 1=identifier)
// i _rate - rate value
// i _i_rate - rate identifier
//------------------------------------------------------------------------------
int AUTOMATON::put_local_rate(int _state, int _trans, int _local, int type, char *_rate, int _i_rate)
{
  return(state[_state].put_local_rate(_trans, _local, type, _rate, _i_rate));
}

//------------------------------------------------------------------------------
// put_local_prob - put the local event probability
// i _state - state number
// i _trans - transition number
// i _local - local events number
// i type - representation type (0=value, 1=identifier)
// i _prob - probability value
// i _i_prob - probability identifier
//------------------------------------------------------------------------------
int AUTOMATON::put_local_prob(int _state, int _trans, int _local, int type, char *_prob, int _i_prob)
{
  return(state[_state].put_local_prob(_trans, _local, type, _prob, _i_prob));
}

//------------------------------------------------------------------------------
// put_local_name - put the local event name if the name already exist, a error 
//                is returned but the name is writen
// i _state - state number
// i _name - event name
//------------------------------------------------------------------------------
int AUTOMATON::put_local_name(int _state, char *_name)
{
  return(state[_state].put_local_name(_name));
}

//------------------------------------------------------------------------------
// put_local_rate - put the local event rate
// i _state - state number
// i type - representation type (0=value, 1=identifier)
// i _rate - rate value
// i _i_rate - rate identifier
//------------------------------------------------------------------------------
int AUTOMATON::put_local_rate(int _state, int type, char *_rate, int _i_rate)
{
  return(state[_state].put_local_rate(type, _rate, _i_rate));
}

//------------------------------------------------------------------------------
// put_local_prob - put the local event probability
// i _state - state number
// i type - representation type (0=value, 1=identifier)
// i _prob - probability value
// i _i_prob - probability identifier
//------------------------------------------------------------------------------
int AUTOMATON::put_local_prob(int _state, int type, char *_prob, int _i_prob)
{
  return(state[_state].put_local_prob(type, _prob, _i_prob));
}

//------------------------------------------------------------------------------
// reset - reset indexes
//------------------------------------------------------------------------------
int AUTOMATON::reset()
{
  for(int i=0; i<n_state; i++){
    state[i].reset();
  }
  return(1);
}

//------------------------------------------------------------------------------
// get_name - get the automaton name
// o _name - automaton name
//------------------------------------------------------------------------------
void AUTOMATON::get_name(char *_name)
{
  strcpy(_name,name);
}

//------------------------------------------------------------------------------
// get_n_state - return the number of state in the automaton
//------------------------------------------------------------------------------
int AUTOMATON::get_n_state()
{
  return(n_state);
}

//------------------------------------------------------------------------------
// get_state_name - get the state name 
// i _state - state number
// o _name - state name
//------------------------------------------------------------------------------
void AUTOMATON::get_state_name(int _state, char *_name)
{
  state[_state].get_name(_name);
}

//------------------------------------------------------------------------------
// get_state_reward - return the state reward
// i _state - state number
//------------------------------------------------------------------------------
double AUTOMATON::get_state_reward(int _state)
{
  return(state[_state].get_reward());
}

//------------------------------------------------------------------------------
// get_state_n_trans - return the number of transitions
// i _state - state number
//------------------------------------------------------------------------------
int AUTOMATON::get_state_n_trans(int _state)
{
  return(state[_state].get_n_trans());
}

//------------------------------------------------------------------------------
// get_trans_dst - return the destin state of the transitions
// i _state - state number
// i _trans - transition number
//------------------------------------------------------------------------------
int AUTOMATON::get_trans_dst(int _state, int _trans)
{
  return(state[_state].get_trans_dst(_trans));
}

//------------------------------------------------------------------------------
// get_event_n_mst - return the number of master events
// i _state - state number
// i _trans - transition number
//------------------------------------------------------------------------------
int AUTOMATON::get_event_n_mst(int _state, int _trans)
{
  return(state[_state].get_event_n_mst(_trans));
}

//------------------------------------------------------------------------------
// get_event_n_sync - return the number of synchronized events
// i _state - state number
// i _trans - transition number
//------------------------------------------------------------------------------
int AUTOMATON::get_event_n_sync(int _state, int _trans)
{
  return(state[_state].get_event_n_sync(_trans));
}

//------------------------------------------------------------------------------
// get_event_n_local - return the number of local events
// i _state - state number
// i _trans - transition number
//------------------------------------------------------------------------------
int AUTOMATON::get_event_n_local(int _state, int _trans)
{
  return(state[_state].get_event_n_local(_trans));
}

//------------------------------------------------------------------------------
// get_mst_name - get the master event name 
// i _state - state number
// i _trans - transition number 
// i _mst - master event number 
// o _name - event name
//------------------------------------------------------------------------------
void AUTOMATON::get_mst_name(int _state, int _trans, int _mst, char *_name)
{
  state[_state].get_mst_name(_trans, _mst, _name);
}

//------------------------------------------------------------------------------
// get_mst_rate - get the master event rate
// i _state - state number
// i _trans - transition number
// i _mst - master event number
// o _rate - rate value
// o _i_rate - rate identifier
//------------------------------------------------------------------------------
int AUTOMATON::get_mst_rate(int _state, int _trans, int _mst, char *_rate, int *_i_rate)
{
  return(state[_state].get_mst_rate(_trans, _mst, _rate, _i_rate));
}

//------------------------------------------------------------------------------
// get_mst_prob - get the master event probability
// i _state - state number
// i _trans - transition number 
// i _mst - master event number
// o _prob - probability value
// o _i_prob - probability identifier
//------------------------------------------------------------------------------
int AUTOMATON::get_mst_prob(int _state, int _trans, int _mst, char *_prob, int *_i_prob)
{
  return(state[_state].get_mst_prob(_trans, _mst, _prob, _i_prob));
}

//------------------------------------------------------------------------------
// get_sync_name - get the synchronized event name 
// i _state - state number
// i _trans - transition number
// i _sync - synchronized event number
// o _name - event name
//------------------------------------------------------------------------------
void AUTOMATON::get_sync_name(int _state, int _trans, int _sync, char *_name)
{
  state[_state].get_sync_name(_trans, _sync, _name);
}

//------------------------------------------------------------------------------
// get_sync_prob - get the synchronized event probability
// i _state - state number
// i _trans - transition number
// i _sync - synchronized event number
// o _prob - probability value
// o _i_prob - probability identifier
//------------------------------------------------------------------------------
int AUTOMATON::get_sync_prob(int _state, int _trans, int _sync, char *_prob, int *_i_prob)
{
  return(state[_state].get_sync_prob(_trans, _sync, _prob, _i_prob));
}

//------------------------------------------------------------------------------
// get_local_name - get the local event name 
// i _state - state number
// i _trans - transition number
// i _local - local events number
// o _name - event name
//------------------------------------------------------------------------------
void AUTOMATON::get_local_name(int _state, int _trans, int _local, char *_name)
{
  state[_state].get_local_name(_trans, _local, _name);
}

//------------------------------------------------------------------------------
// get_local_rate - get the local event rate
// i _state - state number
// i _trans - transition number
// i _local - local events number
// o _rate - rate value
// o _i_rate - rate identifier
//------------------------------------------------------------------------------
int AUTOMATON::get_local_rate(int _state, int _trans, int _local, char *_rate, int *_i_rate)
{
  return(state[_state].get_local_rate(_trans, _local, _rate, _i_rate));
}

//------------------------------------------------------------------------------
// get_local_prob - get the local event probability
// i _state - state number
// i _trans - transition number
// i _local - local events number
// o _prob - probability value
// o _i_prob - probability identifier
//------------------------------------------------------------------------------
int AUTOMATON::get_local_prob(int _state, int _trans, int _local, char *_prob, int *_i_prob)
{
  return(state[_state].get_local_prob(_trans, _local, _prob, _i_prob));
}

