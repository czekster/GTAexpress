//====================================================================================//
//                                                                                    //
//                                   Network Structure                                //
//                                                                                    //
//====================================================================================//
//  This File:   net.C                       Tool: C++                                //
//  Software:    PEPS                        Advisor: Paulof@inf.pucrs.br             //
//  Doc:                                                                              //
//====================================================================================//
//  Creation:    18/Jun/02                   by: lbrenner@inf.pucrs.br                //
//  Last Change: 18/Jun/02                   by: lbrenner@inf.pucrs.br                //
//====================================================================================//
//  This class is used to store the data obtained from SAN textual input file in the  //
// network form.                                                                      //
//====================================================================================//  

#include "sanfile.h"

//------------------------------------------------------------------------------
// Contructor 
//------------------------------------------------------------------------------
NET::NET()
{
  strcpy(name,"");
  n_aut = 0;
  n_ident = 0;
  n_result = 0;
  n_e_table = 0;
  n_s_table = 0;  
  model = DISCRETE;
  automaton = NULL;
  e_table = NULL;
  s_table = NULL;
  reach.alloc(2);
}

//------------------------------------------------------------------------------
// Destructor 
//------------------------------------------------------------------------------
NET::~NET()
{
  if(automaton)
    delete [] automaton;

  for(int i=0; i<n_e_table; i++)
    if(e_table[i])
      delete [] e_table[i];
  if(e_table)
    delete [] e_table;

  for(int i=0; i<n_s_table; i++)
    if(s_table[i])
      delete [] s_table[i];
  if(s_table)
    delete [] s_table;

}

//------------------------------------------------------------------------------
// free
//------------------------------------------------------------------------------
void NET::free()
{
  for(int i=0; i<n_aut; i++){
    automaton[i].free();
  }
  if(automaton)
    delete [] automaton;
  for(int i=0; i<n_e_table; i++)
    if(e_table[i])
      delete [] e_table[i];
  if(e_table)
    delete [] e_table;

  for(int i=0; i<n_s_table; i++)
    if(s_table[i])
      delete [] s_table[i];
  if(s_table)
    delete [] s_table;
}

//------------------------------------------------------------------------------
// put_net_name - put the network name 
// i _name - network name
//------------------------------------------------------------------------------
void NET::put_net_name(char *_name)
{
  strncpy(name,_name,MAX_MODEL_NAME);
  name[MAX_MODEL_NAME-1] = '\0';
}

//------------------------------------------------------------------------------
// put_net_model - put the model type  
// i _model - model type
//------------------------------------------------------------------------------
void NET::put_net_model(int _model)
{
  model = _model;
}

//------------------------------------------------------------------------------
// put_net_n_aut - put the number of automaton and alloc automaton structure
// i _n_aut - number of automaton
//------------------------------------------------------------------------------
int NET::put_net_n_aut(int _n_aut)
{
  n_aut = _n_aut;
  if(n_aut>0){
    automaton = new AUTOMATON[n_aut];
    if(!automaton){
      Programming_Error1(Parser_202);
      return(0);
    }
    return(1);
  }
  return(0);
}

//------------------------------------------------------------------------------
// put_net_n_ident - put the number of identifiers and alloc identifiers 
//                  structure analizar se alloc os negativos ou nao
// i _n_ident - number of identifiers
//------------------------------------------------------------------------------
int NET::put_net_n_ident(int _n_ident)
{
  n_ident = _n_ident;
  if(!(ident.alloc(_n_ident))){
    return(0);
  }
  return(1);
}

//------------------------------------------------------------------------------
// put_net_n_result - put the number of results and alloc results structure
// i _n_result - number of results
//------------------------------------------------------------------------------
int NET::put_net_n_result(int _n_result)
{
  n_result = _n_result;
  if(!result.alloc(_n_result)){
    return(0);
  }
  return(1);
}

//------------------------------------------------------------------------------
// put_aut_name - put the automaton name in a specific automaton
// i _aut - automaton number
// i _name - automaton name
//------------------------------------------------------------------------------
int NET::put_aut_name(int _aut, char *_name)
{
  int flag = 1;
  char aux[MAX_AUTOMATON_NAME];
  
  for(int i=0; i<n_aut; i++){
    automaton[i].get_name(aux);	
    if(!strcmp(aux,_name))
      flag = 0;
  }	
  automaton[_aut].put_name(_name);
  return(flag);
}

//------------------------------------------------------------------------------
// put_aut_n_state - put the number of state in an automaton
// i _aut - automaton number
// i _n_state - number of state
//------------------------------------------------------------------------------
int NET::put_aut_n_state(int _aut, int _n_state)
{
  automaton[_aut].put_n_state(_n_state);
  return -1;//ERR_VISUAL_C++
}

//------------------------------------------------------------------------------
// put_state_name - put the state name in a specific state
// i _aut - automaton number
// i _state - state number
// i _name - state name
//------------------------------------------------------------------------------
int NET::put_state_name(int _aut, int _state, char *_name)
{
  return(automaton[_aut].put_state_name(_state, _name));
}

//------------------------------------------------------------------------------
// put_state_reward - put the state reward in a specific state
// i _aut - automaton number
// i _state - state number
// i _reward - state reward
//------------------------------------------------------------------------------
void NET::put_state_reward(int _aut, int _state, double _reward)
{
  automaton[_aut].put_state_reward(_state, _reward);
}

//------------------------------------------------------------------------------
// put_state_n_trans - put the number of transitions in a state
// i _aut - automaton number
// i _state - state number
// i _n_trans - number of transitions
//------------------------------------------------------------------------------
int NET::put_state_n_trans(int _aut, int _state, int _n_trans)
{
//  cout << _aut << "-" << _state << ": " << _n_trans << endl;
  return(automaton[_aut].put_state_n_trans(_state, _n_trans));
}

//------------------------------------------------------------------------------
// put_trans_dst - put the destin state in a transition
// i _aut - automaton number
// i _state - state number
// int type - representation type(ID, ++, --, ==, +=, -=)
// i _dst - destin state
//------------------------------------------------------------------------------
int NET::put_trans_dst(int _aut, int _state, int type, char *_dst)
{
  return(automaton[_aut].put_trans_dst(_state, type, _dst));
}

//------------------------------------------------------------------------------
// put_from_dst - put the destin state in a transition with tha from primitive
// i _aut - automaton number
// i _state - state number
// i _dst - destin state
//------------------------------------------------------------------------------
int NET::put_from_dst(int _aut, int _state, char *_dst)
{
  return(automaton[_aut].put_from_dst(_state, _dst));
}

//------------------------------------------------------------------------------
// put_event_n_mst - put the number of master events
// i _aut - automaton number
// i _state - state number
// i _trans - transition number
// i _n_mst - number of master events
//------------------------------------------------------------------------------
int NET::put_event_n_mst(int _aut, int _state, int _trans, int _n_mst)
{
  return(automaton[_aut].put_event_n_mst(_state, _trans, _n_mst));
}

//------------------------------------------------------------------------------
// put_event_n_sync - put the number of synchronized events
// i _aut - automaton number
// i _state - state number
// i _trans - transition number
// i _n_sync - number of synchronized events
//------------------------------------------------------------------------------
int NET::put_event_n_sync(int _aut, int _state, int _trans, int _n_sync)
{
  return(automaton[_aut].put_event_n_sync(_state, _trans, _n_sync));
}

//------------------------------------------------------------------------------
// put_event_n_local - put the number of local events
// i _aut - automaton number
// i _state - state number
// i _trans - transition number
// i _n_local - number of local events
//------------------------------------------------------------------------------
int NET::put_event_n_local(int _aut, int _state, int _trans, int _n_local)
{
  return(automaton[_aut].put_event_n_local(_state, _trans, _n_local));
}

//------------------------------------------------------------------------------
// put_event_n_mst - put the number of master events
// i _aut - automaton number
// i _state - state number
// i _n_mst - number of master events
//------------------------------------------------------------------------------
int NET::put_event_n_mst(int _aut, int _state, int _n_mst)
{
  return(automaton[_aut].put_event_n_mst(_state, _n_mst));
}

//------------------------------------------------------------------------------
// put_event_n_sync - put the number of synchronized events
// i _aut - automaton number
// i _state - state number
// i _n_sync - number of synchronized events
//------------------------------------------------------------------------------
int NET::put_event_n_sync(int _aut, int _state, int _n_sync)
{
  return(automaton[_aut].put_event_n_sync(_state, _n_sync));
}

//------------------------------------------------------------------------------
// put_event_n_local - put the number of local events
// i _aut - automaton number
// i _state - state number
// i _n_local - number of local events
//------------------------------------------------------------------------------
int NET::put_event_n_local(int _aut, int _state, int _n_local)
{
  return(automaton[_aut].put_event_n_local(_state, _n_local));
}

//------------------------------------------------------------------------------
// put_mst_name - put the master event name if the name already exist, a error 
//                is returned but the name is writen
// i _aut - automaton number
// i _state - state number
// i _trans - transition number
// i _mst - master events number
// i _name - event name
//------------------------------------------------------------------------------
int NET::put_mst_name(int _aut, int _state, int _trans, int _mst, char *_name)
{
  return(automaton[_aut].put_mst_name(_state, _trans, _mst, _name));
}

//------------------------------------------------------------------------------
// put_mst_rate - put the master event rate
// i _aut - automaton number
// i _state - state number
// i _trans - transition number
// i _mst - master events number
// i type - representation type (0=value, 1=identifier)
// i _rate - rate value
// i _i_rate - rate identifier
//------------------------------------------------------------------------------
int NET::put_mst_rate(int _aut, int _state, int _trans, int _mst, int type, char *_rate, int _i_rate)
{
  return(automaton[_aut].put_mst_rate(_state, _trans, _mst, type, _rate, _i_rate));
}

//------------------------------------------------------------------------------
// put_mst_prob - put the master event probability
// i _aut - automaton number
// i _state - state number
// i _trans - transition number
// i _mst - master events number
// i type - representation type (0=value, 1=identifier)
// i _prob - probability value
// i _i_prob - probability identifier
//------------------------------------------------------------------------------
int NET::put_mst_prob(int _aut, int _state, int _trans, int _mst, int type, char *_prob, int _i_prob)
{
  return(automaton[_aut].put_mst_prob(_state, _trans, _mst, type, _prob, _i_prob));
}

//------------------------------------------------------------------------------
// put_mst_name - put the master event name if the name already exist, a error 
//                is returned but the name is writen
// i _aut - automaton number
// i _state - state number
// i _name - event name
//------------------------------------------------------------------------------
int NET::put_mst_name(int _aut, int _state, char *_name)
{
  return(automaton[_aut].put_mst_name(_state, _name));
}

//------------------------------------------------------------------------------
// put_mst_rate - put the master event rate
// i _aut - automaton number
// i _state - state number
// i type - representation type (0=value, 1=identifier)
// i _rate - rate value
// i _i_rate - rate identifier
//------------------------------------------------------------------------------
int NET::put_mst_rate(int _aut, int _state, int type, char *_rate, int _i_rate)
{
  return(automaton[_aut].put_mst_rate(_state, type, _rate, _i_rate));
}

//------------------------------------------------------------------------------
// put_mst_prob - put the master event probability
// i _aut - automaton number
// i _state - state number
// i type - representation type (0=value, 1=identifier)
// i _prob - probability value
// i _i_prob - probability identifier
//------------------------------------------------------------------------------
int NET::put_mst_prob(int _aut, int _state, int type, char *_prob, int _i_prob)
{
  return(automaton[_aut].put_mst_prob(_state, type, _prob, _i_prob));
}

//------------------------------------------------------------------------------
// put_sync_name - put the synchronized event name 
// i _aut - automaton number
// i _state - state number
// i _trans - transition number
// i _sync - synchronized events number
// i _name - event name
//------------------------------------------------------------------------------
int NET::put_sync_name(int _aut, int _state, int _trans, int _sync, char *_name)
{
  return(automaton[_aut].put_sync_name(_state, _trans, _sync, _name));
}

//------------------------------------------------------------------------------
// put_sync_prob - put the synchronized event probability
// i _aut - automaton number
// i _state - state number
// i _trans - transition number
// i _sync - synchronized events number
// i type - representation type (0=value, 1=identifier)
// i _prob - probability value
// i _i_prob - probability identifier
//------------------------------------------------------------------------------
int NET::put_sync_prob(int _aut, int _state, int _trans, int _sync, int type, char *_prob, int _i_prob)
{
  return(automaton[_aut].put_sync_prob(_state, _trans, _sync, type, _prob, _i_prob));
}

//------------------------------------------------------------------------------
// put_sync_name - put the synchronized event name 
// i _aut - automaton number
// i _state - state number
// i _name - event name
//------------------------------------------------------------------------------
int NET::put_sync_name(int _aut, int _state, char *_name)
{
  return(automaton[_aut].put_sync_name(_state, _name));
}

//------------------------------------------------------------------------------
// put_sync_prob - put the synchronized event probability
// i _aut - automaton number
// i _state - state number
// i type - representation type (0=value, 1=identifier)
// i _prob - probability value
// i _i_prob - probability identifier
//------------------------------------------------------------------------------
int NET::put_sync_prob(int _aut, int _state, int type, char *_prob, int _i_prob)
{
  return(automaton[_aut].put_sync_prob(_state, type, _prob, _i_prob));
}

//------------------------------------------------------------------------------
// put_local_name - put the local event name if the name already exist, a error 
//                is returned but the name is writen
// i _aut - automaton number
// i _state - state number
// i _trans - transition number
// i _local - local events number
// i _name - event name
//------------------------------------------------------------------------------
int NET::put_local_name(int _aut, int _state, int _trans, int _local, char *_name)
{
  return(automaton[_aut].put_local_name(_state, _trans, _local, _name));
}

//------------------------------------------------------------------------------
// put_local_rate - put the local event rate
// i _aut - automaton number
// i _state - state number
// i _trans - transition number
// i _local - local events number
// i type - representation type (0=value, 1=identifier)
// i _rate - rate value
// i _i_rate - rate identifier
//------------------------------------------------------------------------------
int NET::put_local_rate(int _aut, int _state, int _trans, int _local, int type, char *_rate, int _i_rate)
{
  return(automaton[_aut].put_local_rate(_state, _trans, _local, type, _rate, _i_rate));
}

//------------------------------------------------------------------------------
// put_local_prob - put the local event probability
// i _aut - automaton number
// i _state - state number
// i _trans - transition number
// i _local - local events number
// i type - representation type (0=value, 1=identifier)
// i _prob - probability value
// i _i_prob - probability identifier
//------------------------------------------------------------------------------
int NET::put_local_prob(int _aut, int _state, int _trans, int _local, int type, char *_prob, int _i_prob)
{
  return(automaton[_aut].put_local_prob(_state, _trans, _local, type, _prob, _i_prob));
}

//------------------------------------------------------------------------------
// put_local_name - put the local event name if the name already exist, a error 
//                is returned but the name is writen
// i _aut - automaton number
// i _state - state number
// i _name - event name
//------------------------------------------------------------------------------
int NET::put_local_name(int _aut, int _state, char *_name)
{
  return(automaton[_aut].put_local_name(_state, _name));
}

//------------------------------------------------------------------------------
// put_local_rate - put the local event rate
// i _aut - automaton number
// i _state - state number
// i type - representation type (0=value, 1=identifier)
// i _rate - rate value
// i _i_rate - rate identifier
//------------------------------------------------------------------------------
int NET::put_local_rate(int _aut, int _state, int type, char *_rate, int _i_rate)
{
  return(automaton[_aut].put_local_rate(_state, type, _rate, _i_rate));
}

//------------------------------------------------------------------------------
// put_local_prob - put the local event probability
// i _aut - automaton number
// i _state - state number
// i type - representation type (0=value, 1=identifier)
// i _prob - probability value
// i _i_prob - probability identifier
//------------------------------------------------------------------------------
int NET::put_local_prob(int _aut, int _state, int type, char *_prob, int _i_prob)
{
  return(automaton[_aut].put_local_prob(_state, type, _prob, _i_prob));
}

//------------------------------------------------------------------------------
// put_ident_name - put the identifier name
// i _name - identifier name
//------------------------------------------------------------------------------
int NET::put_ident_name(char *_name)
{
  return(ident.put_name(_name)); 
}

//------------------------------------------------------------------------------
// put_ident_exp - put the identifier expression
// i _exp - identifier expression
//------------------------------------------------------------------------------
int NET::put_ident_exp(char *_exp)
{
  return(ident.put_exp(_exp)); 
}

//------------------------------------------------------------------------------
// put_result_name - put the result name
// i _name - result name
//------------------------------------------------------------------------------
int NET::put_result_name(char *_name)
{
  return(result.put_name(_name)); 
}

//------------------------------------------------------------------------------
// put_result_exp - put the result expression
// i _exp - result expression
//------------------------------------------------------------------------------
int NET::put_result_exp(char *_exp)
{
  return(result.put_exp(_exp)); 
}

//------------------------------------------------------------------------------
// put_reach_exp - put the reachability expression
// i _exp - reachability expression
//------------------------------------------------------------------------------
int NET::put_reach_exp(char *_exp)
{
  reach.put_name("reachability");
  return(reach.put_exp(_exp)); 
}

//------------------------------------------------------------------------------
// put_jit_name - put the jit name
// i _name - jit name
//------------------------------------------------------------------------------
int NET::put_jit_name(char *_name)
{
  return(jit.put_name(_name)); 
}

//------------------------------------------------------------------------------
// put_jit_exp - put the jit expression
// i _exp - jit expression
//------------------------------------------------------------------------------
int NET::put_jit_exp(char *_exp)
{
  return(jit.put_exp(_exp)); 
}

//------------------------------------------------------------------------------
// get_net_name - get the network name
// o _name - network name
//------------------------------------------------------------------------------
void NET::get_net_name(char *_name)
{
  strcpy(_name,name);
}

//------------------------------------------------------------------------------
// get_net_model - get the network model type
//------------------------------------------------------------------------------
int NET::get_net_model()
{
  return(model);
}

//------------------------------------------------------------------------------
// get_net_n_aut - get the number of automaton
//------------------------------------------------------------------------------
int NET::get_net_n_aut()
{
  return(n_aut);
}

//------------------------------------------------------------------------------
// get_net_n_ident - get the number of identifiers
//------------------------------------------------------------------------------
int NET::get_net_n_ident()
{
  return(n_ident);
}

//------------------------------------------------------------------------------
// get_net_n_result - get the number of results
//------------------------------------------------------------------------------
int NET::get_net_n_result()
{
  return(n_result);
}

//------------------------------------------------------------------------------
// get_aut_name - get the automaton name 
// i _aut - automaton number
// o _name - automaton name
//------------------------------------------------------------------------------
void NET::get_aut_name(int _aut, char *_name)
{
  automaton[_aut].get_name(_name);
}

//------------------------------------------------------------------------------
// get_aut_n_state - return the number of state in an automaton
// i _aut - automaton number
//------------------------------------------------------------------------------
int NET::get_aut_n_state(int _aut)
{
  return(automaton[_aut].get_n_state());
}

//------------------------------------------------------------------------------
// get_state_name - get the state name 
// i _aut - automaton number
// i _state - state number
// o _name - state name
//------------------------------------------------------------------------------
void NET::get_state_name(int _aut, int _state, char *_name)
{
  automaton[_aut].get_state_name(_state, _name);
}

//------------------------------------------------------------------------------
// get_state_reward - return the state reward
// i _aut - automaton number
// i _state - state number
//------------------------------------------------------------------------------
double NET::get_state_reward(int _aut, int _state)
{
  return(automaton[_aut].get_state_reward(_state));
}

//------------------------------------------------------------------------------
// get_state_n_trans - return the number of transitions
// i _aut - automaton number
// i _state - state number
//------------------------------------------------------------------------------
int NET::get_state_n_trans(int _aut, int _state)
{
  return(automaton[_aut].get_state_n_trans(_state));
}

//------------------------------------------------------------------------------
// get_trans_dst - return the destin state of the transition
// i _aut - automaton number
// i _state - state number
// i _trans - transition number
//------------------------------------------------------------------------------
int NET::get_trans_dst(int _aut, int _state, int _trans)
{
  return(automaton[_aut].get_trans_dst(_state,_trans));
}

//------------------------------------------------------------------------------
// get_event_n_mst - return the number of master events
// i _aut - automaton number
// i _state - state number
// i _trans - transition number
//------------------------------------------------------------------------------
int NET::get_event_n_mst(int _aut, int _state, int _trans)
{
  return(automaton[_aut].get_event_n_mst(_state, _trans));
}

//------------------------------------------------------------------------------
// get_event_n_sync - return the number of synchronized events
// i _aut - automaton number
// i _state - state number
// i _trans - transition number
//------------------------------------------------------------------------------
int NET::get_event_n_sync(int _aut, int _state, int _trans)
{
  return(automaton[_aut].get_event_n_sync(_state, _trans));
}

//------------------------------------------------------------------------------
// get_event_n_local - return the number of local events
// i _aut - automaton number
// i _state - state number
// i _trans - transition number
//------------------------------------------------------------------------------
int NET::get_event_n_local(int _aut, int _state, int _trans)
{
  return(automaton[_aut].get_event_n_local(_state, _trans));
}

//------------------------------------------------------------------------------
// get_mst_name - get the master event name 
// i _aut - automaton number
// i _state - state number
// i _trans - transition number 
// i _mst - master event number 
// o _name - event name
//------------------------------------------------------------------------------
void NET::get_mst_name(int _aut, int _state, int _trans, int _mst, char *_name)
{
  automaton[_aut].get_mst_name(_state, _trans, _mst, _name);
}

//------------------------------------------------------------------------------
// get_mst_rate - get the master event rate
// i _aut - automaton number
// i _state - state number
// i _trans - transition number
// i _mst - master event number
// o _rate - rate value
// o _i_rate - rate identifier
//------------------------------------------------------------------------------
int NET::get_mst_rate(int _aut, int _state, int _trans, int _mst, char *_rate, int *_i_rate)
{
  return(automaton[_aut].get_mst_rate(_state, _trans, _mst, _rate, _i_rate));
}

//------------------------------------------------------------------------------
// get_mst_prob - get the master event probability
// i _aut - automaton number
// i _state - state number
// i _trans - transition number 
// i _mst - master event number
// o _prob - probability value
// o _i_prob - probability identifier
//------------------------------------------------------------------------------
int NET::get_mst_prob(int _aut, int _state, int _trans, int _mst, char *_prob, int *_i_prob)
{
  return(automaton[_aut].get_mst_prob(_state, _trans, _mst, _prob, _i_prob));
}

//------------------------------------------------------------------------------
// get_sync_name - get the synchronized event name 
// i _aut - automaton number
// i _state - state number
// i _trans - transition number
// i _sync - synchronized event number
// o _name - event name
//------------------------------------------------------------------------------
void NET::get_sync_name(int _aut, int _state, int _trans, int _sync, char *_name)
{
  automaton[_aut].get_sync_name(_state, _trans, _sync, _name);
}

//------------------------------------------------------------------------------
// get_sync_prob - get the synchronized event probability
// i _aut - automaton number
// i _state - state number
// i _trans - transition number
// i _sync - synchronized event number
// o _prob - probability value
// o _i_prob - probability identifier
//------------------------------------------------------------------------------
int NET::get_sync_prob(int _aut, int _state, int _trans, int _sync, char *_prob, int *_i_prob)
{
  return(automaton[_aut].get_sync_prob(_state, _trans, _sync, _prob, _i_prob));
}

//------------------------------------------------------------------------------
// get_local_name - get the local event name 
// i _aut - automaton number
// i _state - state number
// i _trans - transition number
// i _local - local events number
// o _name - event name
//------------------------------------------------------------------------------
void NET::get_local_name(int _aut, int _state, int _trans, int _local, char *_name)
{
  automaton[_aut].get_local_name(_state, _trans, _local, _name);
}

//------------------------------------------------------------------------------
// get_local_rate - get the local event rate
// i _aut - automaton number
// i _state - state number
// i _trans - transition number
// i _local - local events number
// o _rate - rate value
// o _i_rate - rate identifier
//------------------------------------------------------------------------------
int NET::get_local_rate(int _aut, int _state, int _trans, int _local, char *_rate, int *_i_rate)
{
  return(automaton[_aut].get_local_rate(_state, _trans, _local, _rate, _i_rate));
}

//------------------------------------------------------------------------------
// get_local_prob - get the local event probability
// i _aut - automaton number
// i _state - state number
// i _trans - transition number
// i _local - local events number
// o _prob - probability value
// o _i_prob - probability identifier
//------------------------------------------------------------------------------
int NET::get_local_prob(int _aut, int _state, int _trans, int _local, char *_prob, int *_i_prob)
{
  return(automaton[_aut].get_local_prob(_state, _trans, _local, _prob, _i_prob));
}

//------------------------------------------------------------------------------
// get_ident_name - get the identifier name
// i _ident -  identifier number
// o _name - identifier name
//------------------------------------------------------------------------------
int NET::get_ident_name(int _ident, char *_name)
{
  return(ident.get_name(_ident, _name)); 
}

//------------------------------------------------------------------------------
// get_ident_name - get the identifier name
// i _name - identifier name
//------------------------------------------------------------------------------
int NET::get_ident_name(char *_name)
{
  return(ident.get_name(_name)); 
}

//------------------------------------------------------------------------------
// get_ident_exp - get the identifier expression
// i _ident -  identifier number
// o _exp - identifier expression
//------------------------------------------------------------------------------
int NET::get_ident_exp(int _ident, char *_exp)
{
  return(ident.get_exp(_ident, _exp)); 
}

//------------------------------------------------------------------------------
// get_ident_exp - get the identifier expression
// i _name -  identifier name
// o _exp - identifier expression
//------------------------------------------------------------------------------
int NET::get_ident_exp(char *_name, char *_exp)
{
  return(ident.get_exp(_name, _exp)); 
}

//------------------------------------------------------------------------------
// get_ident_exp - get the identifier expression
// i _exp - identifier expression
//------------------------------------------------------------------------------
int NET::get_ident_exp(char *_exp)
{
  return(ident.get_exp(_exp)); 
}

//------------------------------------------------------------------------------
// get_ident_n_exp - get the identifier expression
//------------------------------------------------------------------------------
int NET::get_ident_n_exp()
{
  return(ident.get_n_exp()); 
}

//------------------------------------------------------------------------------
// get_result_name - get the result name
// i _result -  result number
// o _name - result name
//------------------------------------------------------------------------------
int NET::get_result_name(int _result, char *_name)
{
  result.get_name(_result, _name); 
  return -1;//ERR_VISUAL_C++
}

//------------------------------------------------------------------------------
// get_result_exp - get the result expression
// i _result -  result number
// o _exp - result expression
//------------------------------------------------------------------------------
int NET::get_result_exp(int _result, char *_exp)
{
  return(result.get_exp(_result, _exp)); 
}

//------------------------------------------------------------------------------
// get_result_n_exp - get the result expression
//------------------------------------------------------------------------------
int NET::get_result_n_exp()
{
  return(result.get_n_exp()); 
}

//------------------------------------------------------------------------------
// get_reach_exp - get the reachability expression
// i _exp - reachability expression
//------------------------------------------------------------------------------
int NET::get_reach_exp(char *_exp)
{
  return(reach.get_exp(0, _exp)); 
}

//------------------------------------------------------------------------------
// get_jit_name - get the jit name
// i _jit -  jit number
// o _name - jit name
//------------------------------------------------------------------------------
int NET::get_jit_name(int _jit, char *_name)
{
  return(jit.get_name(_jit, _name)); 
}

//------------------------------------------------------------------------------
// get_jit_exp - get the jit expression
// i _jit -  jit number
// o _exp - jit expression
//------------------------------------------------------------------------------
int NET::get_jit_exp(int _jit, char *_exp)
{
  return(jit.get_exp(_jit, _exp)); 
}

//------------------------------------------------------------------------------
// get_name - get the automaton or state index
// i _name - automaton or state name
//------------------------------------------------------------------------------
int NET::get_name(char *_name)
{
  char aux[MAX_EXP_SIZE];
  
  for(int i=0;i<n_aut;i++){
    get_aut_name(i, aux);
    if(!strcmp(aux, _name))
      return(i);
    for(int j=0; j<get_aut_n_state(i); j++){
      get_state_name(i, j, aux);
      if(!strcmp(aux, _name))
        return(j);
    }
  }
  return(-1);
  
}

//------------------------------------------------------------------------------
// get_name - get the state index by automaton
// i _name - state name
// i i - automaton index
//------------------------------------------------------------------------------
int NET::get_name(char *_name, int i)
{
  char aux[MAX_EXP_SIZE];
  
  if(i>=0){
    for(int j=0; j<get_aut_n_state(i); j++){
      get_state_name(i, j, aux);
      if(!strcmp(aux, _name))
        return(j);
    }
  }
  return(-1);
}

//------------------------------------------------------------------------------
// get_name - get the state index in the automaton inteval
// i _name - state name
// i i - automaton index
// i j - automaton index
//------------------------------------------------------------------------------
int NET::get_name(char *_name, int i, int j)
{
  int flag, flag_n;
  char aux[MAX_EXP_SIZE];
  
  if(i>=0 && j<=n_aut){
    flag = flag_n = -1;
    for(int k=i; k<=j; k++){
      flag_n = -1;	
      for(int l=0; l<get_aut_n_state(i); l++){
        get_state_name(k, l, aux);
        if(!strcmp(aux, _name))
          flag_n = l;
      }
      if(flag_n==-1)
        return(0);
      if(flag==-1)
        flag = flag_n; 
      if(flag!=-1 && flag!=flag_n)
        return(-1); 
    }  
    return(1);
  }
  return(0);
}

//------------------------------------------------------------------------------
// rm_ident_cnst - remove constant
//------------------------------------------------------------------------------
int NET::rm_ident_cnst()
{
  return(ident.rm_cnst());
}

//------------------------------------------------------------------------------
// rm_reach_cnst - remove constant
//------------------------------------------------------------------------------
int NET::rm_reach_cnst()
{
  return(reach.rm_cnst());
}

//------------------------------------------------------------------------------
// rm_result_cnst - remove constant
//------------------------------------------------------------------------------
int NET::rm_result_cnst()
{
  return(result.rm_cnst());
}

//------------------------------------------------------------------------------
// reset - reset indexes
//------------------------------------------------------------------------------
int NET::reset()
{
  for(int i=0; i<n_aut; i++){
    automaton[i].reset();
  }
  return(1);
}
