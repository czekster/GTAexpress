//====================================================================================//
//                                                                                    //
//                                Transition Structure                                //
//                                                                                    //
//====================================================================================//
//  This File:   trans.C                     Tool: C++                                //
//  Software:    PEPS                        Advisor: Paulof@inf.pucrs.br             //
//  Doc:                                                                              //
//====================================================================================//
//  Creation:    18/Jun/02                   by: lbrenner@inf.pucrs.br                //
//  Last Change: 18/Jun/02                   by: lbrenner@inf.pucrs.br                //
//====================================================================================//
//  This class is used to store the data obtained from SAN textual input file in the  //
// transition form.                                                                   //
//====================================================================================//  

#include "sanfile.h"

//------------------------------------------------------------------------------
// Contructor 
//------------------------------------------------------------------------------
TRANS::TRANS()
{
  dst = -1;
  n_mst = 0;
  n_sync = 0;
  n_local = 0;
  i_mst = 0;
  i_sync = 0;
  i_local = 0;
  mst = NULL;
  sync = NULL;
  local = NULL;
}

//------------------------------------------------------------------------------
// Destructor 
//------------------------------------------------------------------------------
TRANS::~TRANS()
{
 if(mst)
   delete [] mst;
 if(sync)
   delete [] sync;
 if(local)
   delete [] local;
}

//------------------------------------------------------------------------------
// free
//------------------------------------------------------------------------------
void TRANS::free()
{
 if(mst)
   delete [] mst;
 if(sync)
   delete [] sync;
 if(local)
   delete [] local;
}

//------------------------------------------------------------------------------
// put_dst - put the destin state
// i _dst - state destin
//------------------------------------------------------------------------------
void TRANS::put_dst(int _dst)
{
  dst = _dst;
}

//------------------------------------------------------------------------------
// put_n_mst - put the number of master events and alloc event structure
// i _n_mst - number of master events
//------------------------------------------------------------------------------
int TRANS::put_n_mst(int _n_mst)
{
  n_mst = _n_mst;
  if(n_mst>0){
    mst = new EVENT[n_mst];
    if(!mst){
      Programming_Error1(Parser_02);	
      return(0);
    }
    return(1);
  }
  return(1);
}

//------------------------------------------------------------------------------
// put_n_sync - put the number of synchronized events and alloc event structure
// i _n_sync - number of synchronized events
//------------------------------------------------------------------------------
int TRANS::put_n_sync(int _n_sync)
{
  n_sync = _n_sync;
  if(n_sync>0){
    sync = new EVENT[n_sync];
    if(!sync){
      Programming_Error1(Parser_02);	
      return(0);
    }
    return(1);
  }
  return(1);
}

//------------------------------------------------------------------------------
// put_n_local - put the number of local events and alloc event structure
// i _n_local - number of local events
//------------------------------------------------------------------------------
int TRANS::put_n_local(int _n_local)
{
  n_local = _n_local;
  if(n_local>0){
    local = new EVENT[n_local];
    if(!local){
      Programming_Error1(Parser_02);	
      return(0);
    }
    return(1);
  }
  return(1);
}

//------------------------------------------------------------------------------
// put_mst_name - put the master event name if the name already exist, a error 
//                is returned but the name is writen
// i _mst - master event number
// i _name - event name
//------------------------------------------------------------------------------
int TRANS::put_mst_name(int _mst, char *_name)
{
  int flag = 1;
  char aux[MAX_EVENT_NAME];
  
  for(int i=0; i<n_mst; i++){ // search the name in master events
    mst[i].get_name(aux);	
    if(!strcmp(aux,_name))
      flag = 0;
  }	

  for(int i=0; i<n_sync; i++){ // search the name in syncrhonized events
    sync[i].get_name(aux);	
    if(!strcmp(aux,_name))
      flag = 0;
  }	

  for(int i=0; i<n_local; i++){ // search the name in local events
    local[i].get_name(aux);	
    if(!strcmp(aux,_name))
      flag = 0;
  }	

  mst[_mst].put_name(_name);
  i_mst = _mst;
  return(flag);
}

//------------------------------------------------------------------------------
// put_mst_rate - put the master event rate
// i _mst - master event number
// i type - representation type (0=value, 1=identifier)
// i _rate - rate value
// i _i_rate - rate identifier
//------------------------------------------------------------------------------
int TRANS::put_mst_rate(int _mst, int type, char *_rate, int _i_rate)
{
  mst[_mst].put_rate(type, _rate, _i_rate);
  i_mst = _mst;
  return(1);
}

//------------------------------------------------------------------------------
// put_mst_prob - put the master event probability
// i _mst - master event number
// i type - representation type (0=value, 1=identifier)
// i _prob - probability value
// i _i_prob - probability identifier
//------------------------------------------------------------------------------
int TRANS::put_mst_prob(int _mst, int type, char *_prob, int _i_prob)
{
  mst[_mst].put_prob(type, _prob, _i_prob);
  i_mst = _mst;
  return(1);
}

//------------------------------------------------------------------------------
// put_mst_name - put the master event name if the name already exist, a error 
//                is returned but the name is writen
// i _name - event name
//------------------------------------------------------------------------------
int TRANS::put_mst_name(char *_name)
{
  int flag = 1;
  char aux[MAX_EVENT_NAME];
  
  for(int i=0; i<n_mst; i++){ // search the name in master events
    mst[i].get_name(aux);	
    if(!strcmp(aux,_name))
      flag = 0;
  }	

  for(int i=0; i<n_sync; i++){ // search the name in syncrhonized events
    sync[i].get_name(aux);	
    if(!strcmp(aux,_name))
      flag = 0;
  }	

  for(int i=0; i<n_local; i++){ // search the name in local events
    local[i].get_name(aux);	
    if(!strcmp(aux,_name))
      flag = 0;
  }	

  mst[i_mst].put_name(_name);
  i_mst++;
  return(flag);
}

//------------------------------------------------------------------------------
// put_mst_rate - put the master event rate
// i type - representation type (0=value, 1=identifier)
// i _rate - rate value
// i _i_rate - rate identifier
//------------------------------------------------------------------------------
int TRANS::put_mst_rate(int type, char *_rate, int _i_rate)
{
  mst[i_mst-1].put_rate(type, _rate, _i_rate);
  return(1);
}

//------------------------------------------------------------------------------
// put_mst_prob - put the master event probability
// i type - representation type (0=value, 1=identifier)
// i _prob - probability value
// i _i_prob - probability identifier
//------------------------------------------------------------------------------
int TRANS::put_mst_prob(int type, char *_prob, int _i_prob)
{
  mst[i_mst-1].put_prob(type, _prob, _i_prob);
  return(1);
}

//------------------------------------------------------------------------------
// put_sync_name - put the synchronized event name if the name already exist, a error 
//                is returned but the name is writen
// i _sync - synchronized event number
// i _name - event name
//------------------------------------------------------------------------------
int TRANS::put_sync_name(int _sync, char *_name)
{
  int flag = 1;
  char aux[MAX_EVENT_NAME];
  
  for(int i=0; i<n_mst; i++){ // search the name in master events
    mst[i].get_name(aux);	
    if(!strcmp(aux,_name))
      flag = 0;
  }	

  for(int i=0; i<n_sync; i++){ // search the name in syncrhonized events
    sync[i].get_name(aux);	
    if(!strcmp(aux,_name))
      flag = 0;
  }	
  for(int i=0; i<n_local; i++){ // search the name in local events
    local[i].get_name(aux);	
    if(!strcmp(aux,_name))
      flag = 0;
  }	

  sync[_sync].put_name(_name);
  i_sync = _sync;
  return(flag);
}

//------------------------------------------------------------------------------
// put_sync_prob - put the synchronized event probability
// i _sync - synchronized event number
// i type - representation type (0=value, 1=identifier)
// i _prob - probability value
// i _i_prob - probability identifier
//------------------------------------------------------------------------------
int TRANS::put_sync_prob(int _sync, int type, char *_prob, int _i_prob)
{
  i_sync = _sync;
  return(sync[_sync].put_prob(type, _prob, _i_prob));
}

//------------------------------------------------------------------------------
// put_sync_name - put the synchronized event name if the name already exist, a error 
//                is returned but the name is writen
// i _name - event name
//------------------------------------------------------------------------------
int TRANS::put_sync_name(char *_name)
{
  int flag = 1;
  char aux[MAX_EVENT_NAME];
  
  for(int i=0; i<n_mst; i++){ // search the name in master events
    mst[i].get_name(aux);	
    if(!strcmp(aux,_name))
      flag = 0;
  }	

  for(int i=0; i<n_sync; i++){ // search the name in syncrhonized events
    sync[i].get_name(aux);	
    if(!strcmp(aux,_name))
      flag = 0;
  }	
  for(int i=0; i<n_local; i++){ // search the name in local events
    local[i].get_name(aux);	
    if(!strcmp(aux,_name))
      flag = 0;
  }	
 
  sync[i_sync].put_name(_name);
  i_sync++;
  return(flag);
}

//------------------------------------------------------------------------------
// put_sync_prob - put the synchronized event probability
// i type - representation type (0=value, 1=identifier)
// i _prob - probability value
// i _i_prob - probability identifier
//------------------------------------------------------------------------------
int TRANS::put_sync_prob(int type, char *_prob, int _i_prob)
{
  return(sync[i_sync-1].put_prob(type, _prob, _i_prob));
}

//------------------------------------------------------------------------------
// put_local_name - put the local event name if the name already exist, a error 
//                is returned but the name is writen
// i _local - local event number
// i _name - event name
//------------------------------------------------------------------------------
int TRANS::put_local_name(int _local, char *_name)
{
  int flag = 1;
  char aux[MAX_EVENT_NAME];
  
  for(int i=0; i<n_mst; i++){ // search the name in master events
    mst[i].get_name(aux);	
    if(!strcmp(aux,_name))
      flag = 0;
  }	

  for(int i=0; i<n_sync; i++){ // search the name in syncrhonized events
    sync[i].get_name(aux);	
    if(!strcmp(aux,_name))
      flag = 0;
  }	

  for(int i=0; i<n_local; i++){ // search the name in local events
    local[i].get_name(aux);	
    if(!strcmp(aux,_name))
      flag = 0;
  }	

  local[_local].put_name(_name);
  i_local = _local;
  return(flag);
}

//------------------------------------------------------------------------------
// put_local_rate - put the local event rate
// i _local - local event number
// i type - representation type (0=value, 1=identifier)
// i _rate - rate value
// i _i_rate - rate identifier
//------------------------------------------------------------------------------
int TRANS::put_local_rate(int _local, int type, char *_rate, int _i_rate)
{
  local[_local].put_rate(type, _rate, _i_rate);
  i_local = _local;
  return(1);
}

//------------------------------------------------------------------------------
// put_local_prob - put the local event probability
// i _local - local event number
// i type - representation type (0=value, 1=identifier)
// i _prob - probability value
// i _i_prob - probability identifier
//------------------------------------------------------------------------------
int TRANS::put_local_prob(int _local, int type, char *_prob, int _i_prob)
{
  local[_local].put_prob(type, _prob, _i_prob);
  i_local = _local;  
  return(1);
}

//------------------------------------------------------------------------------
// put_local_name - put the local event name if the name already exist, a error 
//                is returned but the name is writen
// i _name - event name
//------------------------------------------------------------------------------
int TRANS::put_local_name(char *_name)
{
  int flag = 1;
  char aux[MAX_EVENT_NAME];
  
  for(int i=0; i<n_mst; i++){ // search the name in master events
    mst[i].get_name(aux);	
    if(!strcmp(aux,_name))
      flag = 0;
  }	

  for(int i=0; i<n_sync; i++){ // search the name in syncrhonized events
    sync[i].get_name(aux);	
    if(!strcmp(aux,_name))
      flag = 0;
  }	

  for(int i=0; i<n_local; i++){ // search the name in local events
    local[i].get_name(aux);	
    if(!strcmp(aux,_name))
      flag = 0;
  }	

  local[i_local].put_name(_name);
  i_local++;
  return(flag);
}

//------------------------------------------------------------------------------
// put_local_rate - put the local event rate
// i type - representation type (0=value, 1=identifier)
// i _rate - rate value
// i _i_rate - rate identifier
//------------------------------------------------------------------------------
int TRANS::put_local_rate(int type, char *_rate, int _i_rate)
{
  local[i_local-1].put_rate(type, _rate, _i_rate);
  return(1);
}

//------------------------------------------------------------------------------
// put_local_prob - put the local event probability
// i type - representation type (0=value, 1=identifier)
// i _prob - probability value
// i _i_prob - probability identifier
//------------------------------------------------------------------------------
int TRANS::put_local_prob(int type, char *_prob, int _i_prob)
{
  local[i_local-1].put_prob(type, _prob, _i_prob);
  return(1);
}

//------------------------------------------------------------------------------
// reset - reset indexes
//------------------------------------------------------------------------------
int TRANS::reset()
{
  i_mst = 0;
  i_sync = 0;
  i_local = 0;
  return(1);
}

//------------------------------------------------------------------------------
// get_dst - return the destin state
//------------------------------------------------------------------------------
int TRANS::get_dst()
{
  return(dst);
}

//------------------------------------------------------------------------------
// get_n_mst - return the number of master events
//------------------------------------------------------------------------------
int TRANS::get_n_mst()
{
  return(n_mst);
}

//------------------------------------------------------------------------------
// get_n_sync - return the number of synchronized events
//------------------------------------------------------------------------------
int TRANS::get_n_sync()
{
  return(n_sync);
}

//------------------------------------------------------------------------------
// get_n_loc - return the number of local events
//------------------------------------------------------------------------------
int TRANS::get_n_local()
{
  return(n_local);
}

//------------------------------------------------------------------------------
// get_mst_name - get the master event name
// i _mst - master event number
// o _name - event name
//------------------------------------------------------------------------------
void TRANS::get_mst_name(int _mst, char *_name)
{
  return(mst[_mst].get_name(_name));
}

//------------------------------------------------------------------------------
// get_mst_rate - get the master event rate, the representation type is returned
// i _mst - master event number
// o _rate - rate value
// o _i_rate - rate identifier
//------------------------------------------------------------------------------
int TRANS::get_mst_rate(int _mst, char *_rate, int *_i_rate)
{
  return(mst[_mst].get_rate(_rate, _i_rate));
}

//------------------------------------------------------------------------------
// get_mst_prob - get the master event probability, the representation type is 
//                returned
// i _mst - master event number
// o _prob - probability value
// o _i_prob - probability identifier
//------------------------------------------------------------------------------
int TRANS::get_mst_prob(int _mst, char *_prob, int *_i_prob)
{
  return(mst[_mst].get_prob(_prob, _i_prob));
}

//------------------------------------------------------------------------------
// get_sync_name - get the synchronized event name
// i _sync - synchronized event number
// o _name - event name
//------------------------------------------------------------------------------
void TRANS::get_sync_name(int _sync, char *_name)
{
  return(sync[_sync].get_name(_name));
}

//------------------------------------------------------------------------------
// get_sync_prob - get the synchronized event probability, the representation 
//                 type is returned
// i _sync - synchronized event number
// o _prob - probability value
// o _i_prob - probability identifier
//------------------------------------------------------------------------------
int TRANS::get_sync_prob(int _sync, char *_prob, int *_i_prob)
{
  return(sync[_sync].get_prob(_prob, _i_prob));
}

//------------------------------------------------------------------------------
// get_local_name - get the local event name
// i _local - local event number
// o _name - event name
//------------------------------------------------------------------------------
void TRANS::get_local_name(int _local, char *_name)
{
  return(local[_local].get_name(_name));
}

//------------------------------------------------------------------------------
// get_local_rate - get the local event rate, the representation type is returned
// i _local - local event number
// o _rate - rate value
// o _i_rate - rate identifier
//------------------------------------------------------------------------------
int TRANS::get_local_rate(int _local, char *_rate, int *_i_rate)
{
  return(local[_local].get_rate(_rate, _i_rate));
}

//------------------------------------------------------------------------------
// get_local_prob - get the local event probability, the representation type is 
//                returned
// i _local - local event number
// o _prob - probability value
// o _i_prob - probability identifier
//------------------------------------------------------------------------------
int TRANS::get_local_prob(int _local, char *_prob, int *_i_prob)
{
  return(local[_local].get_prob(_prob, _i_prob));
}

