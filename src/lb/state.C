//====================================================================================//
//                                                                                    //
//                                   State Structure                                  //
//                                                                                    //
//====================================================================================//
//  This File:   state.C                     Tool: C++                                //
//  Software:    PEPS                        Advisor: Paulof@inf.pucrs.br             //
//  Doc:                                                                              //
//====================================================================================//
//  Creation:    18/Jun/02                   by: lbrenner@inf.pucrs.br                //
//  Last Change: 18/Jun/02                   by: lbrenner@inf.pucrs.br                //
//====================================================================================//
//  This class is used to store the data obtained from SAN textual input file in the  //
// state form.                                                                        //
//====================================================================================//  

#include "sanfile.h"

//------------------------------------------------------------------------------
// Contructor 
//------------------------------------------------------------------------------
STATE::STATE()
{
  strcpy(name,"");
  n_trans = 0;
  ind_trans = 0;
  reward = 0.0;
  trans = NULL;
   
}

//------------------------------------------------------------------------------
// Destructor 
//------------------------------------------------------------------------------
STATE::~STATE()
{
  if(trans)
    delete [] trans;
}

//------------------------------------------------------------------------------
// free
//------------------------------------------------------------------------------
void STATE::free()
{
  for(int i=0; i<n_trans; i++)
    trans[i].free();
  if(trans)
    delete [] trans;
}

//------------------------------------------------------------------------------
// put_name - put the state name
// i _name - state name
//------------------------------------------------------------------------------
void STATE::put_name(char *_name)
{
  strncpy(name,_name,MAX_STATE_NAME);
  name[MAX_STATE_NAME-1] = '\0';
}

//------------------------------------------------------------------------------
// put_reward - put the state reward
// i _reward - state reward
//------------------------------------------------------------------------------
void STATE::put_reward(double _reward)
{
  reward = _reward;
}

//------------------------------------------------------------------------------
// put_n_trans - put the number of trans in a state and alloc trans structure
// i _n_trans - number of trans
//------------------------------------------------------------------------------
int STATE::put_n_trans(int _n_trans)
{
  n_trans = _n_trans;
  if(n_trans){
    trans = new TRANS[n_trans];
    if(!trans){
      Programming_Error1(Parser_02);	
      return(0);
    }
    return(1);
  }
  return(0);
}

//------------------------------------------------------------------------------
// put_trans_dst - put the destin state of the transition
// i _trans - transition index
// i _dst - destin state index
//------------------------------------------------------------------------------
void STATE::put_trans_dst(int _dst)
{
  trans[ind_trans].put_dst(_dst);
  ind_trans++;
}

//------------------------------------------------------------------------------
// put_from_dst - put the destine state of the from primitiva
// i _dst - destin state index
//------------------------------------------------------------------------------
void STATE::put_from_dst(int _dst)
{
  trans[ind_trans].put_dst(_dst);
  ind_trans++;
}

//------------------------------------------------------------------------------
// put_event_n_mst - put the number of master events
// i _trans - transition number
// i _n_mst - number of master events
//------------------------------------------------------------------------------
int STATE::put_event_n_mst(int _trans, int _n_mst)
{
//  ind_trans = _trans;
  return(trans[_trans].put_n_mst(_n_mst));
}

//------------------------------------------------------------------------------
// put_event_n_sync - put the number of synchronized events
// i _trans - transition number
// i _n_sync - number of synchronized events
//------------------------------------------------------------------------------
int STATE::put_event_n_sync(int _trans, int _n_sync)
{
//  ind_trans = _trans;
  return(trans[_trans].put_n_sync(_n_sync));
}

//------------------------------------------------------------------------------
// put_event_n_local - put the number of local events
// i _trans - transition number
// i _n_local - number of local events
//------------------------------------------------------------------------------
int STATE::put_event_n_local(int _trans, int _n_local)
{
//  ind_trans = _trans;
  return(trans[_trans].put_n_local(_n_local));
}

//------------------------------------------------------------------------------
// put_event_n_mst - put the number of master events
// i _n_mst - number of master events
//------------------------------------------------------------------------------
int STATE::put_event_n_mst(int _n_mst)
{
//  ind_trans++;
  return(trans[ind_trans-1].put_n_mst(_n_mst));
}

//------------------------------------------------------------------------------
// put_event_n_sync - put the number of synchronized events
// i _n_sync - number of synchronized events
//------------------------------------------------------------------------------
int STATE::put_event_n_sync(int _n_sync)
{
//  ind_trans++;
  return(trans[ind_trans-1].put_n_sync(_n_sync));
}

//------------------------------------------------------------------------------
// put_event_n_local - put the number of local events
// i _n_local - number of local events
//------------------------------------------------------------------------------
int STATE::put_event_n_local(int _n_local)
{
//  ind_trans++;
  return(trans[ind_trans-1].put_n_local(_n_local));
}

//------------------------------------------------------------------------------
// put_mst_name - put the master event name if the name already exist, a error 
//                is returned but the name is writen
// i _trans - transition number
// i _mst - master event number
// i _name - event name
//------------------------------------------------------------------------------
int STATE::put_mst_name(int _trans, int _mst, char *_name)
{
//  ind_trans = _trans; 
  return(trans[_trans].put_mst_name(_mst, _name));
}

//------------------------------------------------------------------------------
// put_mst_rate - put the master event rate
// i _trans - transition number
// i _mst - master event number
// i type - representation type (0=value, 1=identifier)
// i _rate - rate value
// i _i_rate - rate identifier
//------------------------------------------------------------------------------
int STATE::put_mst_rate(int _trans, int _mst, int type, char *_rate, int _i_rate)
{
//  ind_trans = _trans; 
  return(trans[_trans].put_mst_rate(_mst, type, _rate, _i_rate));
}

//------------------------------------------------------------------------------
// put_mst_prob - put the master event probability
// i _trans - transition number
// i _mst - master event number
// i type - representation type (0=value, 1=identifier)
// i _prob - probability value
// i _i_prob - probability identifier
//------------------------------------------------------------------------------
int STATE::put_mst_prob(int _trans, int _mst, int type, char *_prob, int _i_prob)
{
//  ind_trans = _trans; 
  return(trans[_trans].put_mst_prob(_mst, type, _prob, _i_prob));
}

//------------------------------------------------------------------------------
// put_mst_name - put the master event name if the name already exist, a error 
//                is returned but the name is writen
// i _name - event name
//------------------------------------------------------------------------------
int STATE::put_mst_name(char *_name)
{
//  ind_trans++;  
  return(trans[ind_trans-1].put_mst_name(_name));
}

//------------------------------------------------------------------------------
// put_mst_rate - put the master event rate
// i type - representation type (0=value, 1=identifier)
// i _rate - rate value
// i _i_rate - rate identifier
//------------------------------------------------------------------------------
int STATE::put_mst_rate(int type, char *_rate, int _i_rate)
{
  return(trans[ind_trans-1].put_mst_rate(type, _rate, _i_rate));
}

//------------------------------------------------------------------------------
// put_mst_prob - put the master event probability
// i type - representation type (0=value, 1=identifier)
// i _prob - probability value
// i _i_prob - probability identifier
//------------------------------------------------------------------------------
int STATE::put_mst_prob(int type, char *_prob, int _i_prob)
{
  return(trans[ind_trans-1].put_mst_prob(type, _prob, _i_prob));
}

//------------------------------------------------------------------------------
// put_sync_name - put the synchronized event name 
// i _trans - transition number
// i _sync - synchronized event number
// i _name - event name
//------------------------------------------------------------------------------
int STATE::put_sync_name(int _trans, int _sync, char *_name)
{
//  ind_trans = _trans;
  return(trans[_trans].put_sync_name(_sync, _name));
}

//------------------------------------------------------------------------------
// put_sync_prob - put the synchronized event probability
// i _trans - transition number
// i _sync - synchronized event number
// i type - representation type (0=value, 1=identifier)
// i _prob - probability value
// i _i_prob - probability identifier
//------------------------------------------------------------------------------
int STATE::put_sync_prob(int _trans, int _sync, int type, char *_prob, int _i_prob)
{
//  ind_trans = _trans;
  return(trans[_trans].put_sync_prob(_sync, type, _prob, _i_prob));
}

//------------------------------------------------------------------------------
// put_sync_name - put the synchronized event name 
// i _name - event name
//------------------------------------------------------------------------------
int STATE::put_sync_name(char *_name)
{
//  ind_trans++;
  return(trans[ind_trans-1].put_sync_name(_name));
}

//------------------------------------------------------------------------------
// put_sync_prob - put the synchronized event probability
// i type - representation type (0=value, 1=identifier)
// i _prob - probability value
// i _i_prob - probability identifier
//------------------------------------------------------------------------------
int STATE::put_sync_prob(int type, char *_prob, int _i_prob)
{
  return(trans[ind_trans-1].put_sync_prob(type, _prob, _i_prob));
}

//------------------------------------------------------------------------------
// put_local_name - put the local event name if the name already exist, a error 
//                is returned but the name is writen
// i _trans - transition number
// i _local - local event number
// i _name - event name
//------------------------------------------------------------------------------
int STATE::put_local_name(int _trans, int _local, char *_name)
{
//  ind_trans = _trans;
  return(trans[_trans].put_local_name(_local, _name));
}

//------------------------------------------------------------------------------
// put_local_rate - put the local event rate
// i _trans - transition number
// i _local - local event number
// i type - representation type (0=value, 1=identifier)
// i _rate - rate value
// i _i_rate - rate identifier
//------------------------------------------------------------------------------
int STATE::put_local_rate(int _trans, int _local, int type, char *_rate, int _i_rate)
{
//  ind_trans = _trans;
  return(trans[_trans].put_local_rate(_local, type, _rate, _i_rate));
}

//------------------------------------------------------------------------------
// put_local_prob - put the local event probability
// i _trans - transition number
// i _local - local event number
// i type - representation type (0=value, 1=identifier)
// i _prob - probability value
// i _i_prob - probability identifier
//------------------------------------------------------------------------------
int STATE::put_local_prob(int _trans, int _local, int type, char *_prob, int _i_prob)
{
//  ind_trans = _trans;
  return(trans[_trans].put_local_prob(_local, type, _prob, _i_prob));
}

//------------------------------------------------------------------------------
// put_local_name - put the local event name if the name already exist, a error 
//                is returned but the name is writen
// i _name - event name
//------------------------------------------------------------------------------
int STATE::put_local_name(char *_name)
{
//  ind_trans++;
  return(trans[ind_trans-1].put_local_name(_name));
}

//------------------------------------------------------------------------------
// put_local_rate - put the local event rate
// i type - representation type (0=value, 1=identifier)
// i _rate - rate value
// i _i_rate - rate identifier
//------------------------------------------------------------------------------
int STATE::put_local_rate(int type, char *_rate, int _i_rate)
{
  return(trans[ind_trans-1].put_local_rate(type, _rate, _i_rate));
}

//------------------------------------------------------------------------------
// put_local_prob - put the local event probability
// i type - representation type (0=value, 1=identifier)
// i _prob - probability value
// i _i_prob - probability identifier
//------------------------------------------------------------------------------
int STATE::put_local_prob(int type, char *_prob, int _i_prob)
{
  return(trans[ind_trans-1].put_local_prob(type, _prob, _i_prob));
}

//------------------------------------------------------------------------------
// reset - reset indexes
//------------------------------------------------------------------------------
int STATE::reset()
{
  for(int i=0; i<n_trans; i++){
    trans[i].reset();
  }
  ind_trans = 0;
  return(1);
}

//------------------------------------------------------------------------------
// get_name - get the state name
// o _name - state name
//------------------------------------------------------------------------------
void STATE::get_name(char *_name)
{
  strcpy(_name,name);
}

//------------------------------------------------------------------------------
// get_reward - return the state reward
//------------------------------------------------------------------------------
double STATE::get_reward()
{
  return(reward);
}

//------------------------------------------------------------------------------
// get_n_trans - return the number of transition in the state
//------------------------------------------------------------------------------
int STATE::get_n_trans()
{
  return(n_trans);
}

//------------------------------------------------------------------------------
// get_trans_dst - return the destin state index  of the transition
// i _trans - transition index
//------------------------------------------------------------------------------
int STATE::get_trans_dst(int _trans)
{
  return(trans[_trans].get_dst());
}

//------------------------------------------------------------------------------
// get_event_n_mst - return the number of master events
// i _trans - transition number
//------------------------------------------------------------------------------
int STATE::get_event_n_mst(int _trans)
{
  return(trans[_trans].get_n_mst());
}

//------------------------------------------------------------------------------
// get_event_n_sync - return the number of synchronized events
// i _trans - transition number
//------------------------------------------------------------------------------
int STATE::get_event_n_sync(int _trans)
{
  return(trans[_trans].get_n_sync());
}

//------------------------------------------------------------------------------
// get_event_n_local - return the number of local events
// i _trans - transition number
//------------------------------------------------------------------------------
int STATE::get_event_n_local(int _trans)
{
  return(trans[_trans].get_n_local());
}

//------------------------------------------------------------------------------
// get_mst_name - get the master event name 
// i _trans - transition number
// i _mst - master event number
// o _name - event name
//------------------------------------------------------------------------------
void STATE::get_mst_name(int _trans, int _mst, char *_name)
{
  trans[_trans].get_mst_name(_mst, _name);
}

//------------------------------------------------------------------------------
// get_mst_rate - get the master event rate
// i _trans - transition number
// i _mst - master event number
// o _rate - rate value
// o _i_rate - rate identifier
//------------------------------------------------------------------------------
int STATE::get_mst_rate(int _trans, int _mst, char *_rate, int *_i_rate)
{
  return(trans[_trans].get_mst_rate(_mst, _rate, _i_rate));
}

//------------------------------------------------------------------------------
// get_mst_prob - get the master event probability
// i _trans - transition number
// i _mst - master event number
// o _prob - probability value
// o _i_prob - probability identifier
//------------------------------------------------------------------------------
int STATE::get_mst_prob(int _trans, int _mst, char *_prob, int *_i_prob)
{
  return(trans[_trans].get_mst_prob(_mst, _prob, _i_prob));
}

//------------------------------------------------------------------------------
// get_sync_name - get the synchronized event name 
// i _trans - transition number
// i _sync - synchronized event number
// o _name - event name
//------------------------------------------------------------------------------
void STATE::get_sync_name(int _trans, int _sync, char *_name)
{
  trans[_trans].get_sync_name(_sync, _name);
}


//------------------------------------------------------------------------------
// get_sync_prob - get the synchronized event probability
// i _trans - transition number
// i _sync - synchronized event number
// o _prob - probability value
// o _i_prob - probability identifier
//------------------------------------------------------------------------------
int STATE::get_sync_prob(int _trans, int _sync, char *_prob, int *_i_prob)
{
  return(trans[_trans].get_sync_prob(_sync, _prob, _i_prob));
}

//------------------------------------------------------------------------------
// get_local_name - get the local event name 
// i _trans - transition number
// i _local - local event number
// o _name - event name
//------------------------------------------------------------------------------
void STATE::get_local_name(int _trans, int _local, char *_name)
{
  trans[_trans].get_local_name(_local, _name);
}

//------------------------------------------------------------------------------
// get_local_rate - get the local event rate
// i _trans - transition number
// i _local - local event number
// o _rate - rate value
// o _i_rate - rate identifier
//------------------------------------------------------------------------------
int STATE::get_local_rate(int _trans, int _local, char *_rate, int *_i_rate)
{
  return(trans[_trans].get_local_rate(_local, _rate, _i_rate));
}

//------------------------------------------------------------------------------
// get_local_prob - get the local event probability
// i _trans - transition number
// i _local - local event number
// i _prob - probability value
// i _i_prob - probability identifier
//------------------------------------------------------------------------------
int STATE::get_local_prob(int _trans, int _local, char *_prob, int *_i_prob)
{
  return(trans[_trans].get_local_prob(_local, _prob, _i_prob));
}
