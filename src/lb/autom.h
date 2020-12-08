//====================================================================================//
//                                                                                    //
//                                 Automaton Structure                                //
//                                                                                    //
//====================================================================================//
//  This File:   autom.h                       Tool: C++                                //
//  Software:    PEPS                        Advisor: Paulof@inf.pucrs.br             //
//  Doc:                                                                              //
//====================================================================================//
//  Creation:    18/Jun/02                   by: lbrenner@inf.pucrs.br                //
//  Last Change: 18/Jun/02                   by: lbrenner@inf.pucrs.br                //
//====================================================================================//
//  This class is used to store the data obtained from SAN textual input file in the  //
// automaton form.                                                                      //
//====================================================================================//  

#ifndef _AUTOMATON_H_
#define _AUTOMATON_H_

#include "sanfile.h"

class AUTOMATON {
  public:
    AUTOMATON();
    ~AUTOMATON();
    void free();
    
    // input functions
    void put_name(char *_name); // put the automaton name
    int put_n_state(int _n_state); // put the number of states
    int put_state_name(int _state, char *_name); // put the state name
    void put_state_reward(int _state, double _reward); // put the state reward
    int put_state_n_trans(int _state, int _n_trans); // put the number of transitions 
    int put_trans_dst(int _state, int type, char *_dst); 
    // put the state destin in a trans
    int put_from_dst(int _state, char *_dst); 
    // put the from primitive destin in a trans

    int put_event_n_mst(int _state, int _trans, int _n_mst); // put the master events
    int put_event_n_sync(int _state, int _trans, int _n_sync); // put the synch events
    int put_event_n_local(int _state, int _trans, int _n_local); // put the local events    
    int put_event_n_mst(int _state, int _n_mst); // put the master events
    int put_event_n_sync(int _state, int _n_sync); // put the synch events
    int put_event_n_local(int _state, int _n_local); // put the local events    

    int put_mst_name(int _state, int _trans, int _mst, char *_name); 
    // put the master event name
    int put_mst_rate(int _state, int _trans, int _mst, int type, char *_rate, int _i_rate); 
    // put the mst event rate
    int put_mst_prob(int _state, int _trans, int _mst, int type, char *_prob, int _i_prob); 
    // put the mst event prob
    int put_mst_name(int _state, char *_name); 
    // put the master event name
    int put_mst_rate(int _state, int type, char *_rate, int _i_rate); 
    // put the mst event rate
    int put_mst_prob(int _state, int type, char *_prob, int _i_prob); 
    // put the mst event prob

    int put_sync_name(int _state, int _trans, int _sync, char *name); 
    // put the master event name
    int put_sync_prob(int _state, int _trans, int _sync, int type, char *_prob, int _i_prob); 
    // put the mst event prob
    int put_sync_name(int _state, char *name); 
    // put the master event name
    int put_sync_prob(int _state, int type, char *_prob, int _i_prob); 
    // put the mst event prob

    int put_local_name(int _state, int _trans, int _local, char *name); 
    // put the master event name
    int put_local_rate(int _state, int _trans, int _local, int type, char *_rate, int _i_rate); 
    // put the mst event rate
    int put_local_prob(int _state, int _trans, int _local, int type, char *_prob, int _i_prob); 
    // put the mst event prob
    int put_local_name(int _state, char *name); 
    // put the master event name
    int put_local_rate(int _state, int type, char *_rate, int _i_rate); 
    // put the mst event rate
    int put_local_prob(int _state, int type, char *_prob, int _i_prob); 
    // put the mst event prob
    
    int reset();
    
    
    //output functions
    void get_name(char *_name); // get the automaton name
    int get_n_state(); // get the number of states
    void get_state_name(int _state, char *_name); // get the state name
    double get_state_reward(int _state); // get the state reward
    int get_state_n_trans(int _state); // get the number of transitions
    int get_trans_dst(int _state, int _trans); // get the destin state of the transitions
    int get_event_n_mst(int _state, int _trans); // put the master events
    int get_event_n_sync(int _state, int _trans); // get the synch events
    int get_event_n_local(int _state, int _trans); // get the local events    
    void get_mst_name(int _state, int _trans, int _mst, char *name); 
    // get the master event name
    int get_mst_rate(int _state, int _trans, int _mst, char *_rate, int *_i_rate); 
    // get the mst event rate
    int get_mst_prob(int _state, int _trans, int _mst, char *_prob, int *_i_prob); 
    // get the mst event prob
    void get_sync_name(int _state, int _trans, int _sync, char *name); 
    // get the master event name
    int get_sync_prob(int _state, int _trans, int _sync, char *_prob, int *_i_prob); 
    // get the mst event prob
    void get_local_name(int _state, int _trans, int _local, char *name); 
    // get the master event name
    int get_local_rate(int _state, int _trans, int _local, char *_rate, int *_i_rate); 
    // get the mst event rate
    int get_local_prob(int _state, int _trans, int _local, char *_prob, int *_i_prob); 
    // get the mst event prob
    
  private:
    char name[MAX_AUTOMATON_NAME]; // automaton name
    int n_state; // number of states
    STATE *state; // state structure
    
};

#endif
