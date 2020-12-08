//====================================================================================//
//                                                                                    //
//                                   State Structure                                  //
//                                                                                    //
//====================================================================================//
//  This File:   state.h                     Tool: C++                                //
//  Software:    PEPS                        Advisor: Paulof@inf.pucrs.br             //
//  Doc:                                                                              //
//====================================================================================//
//  Creation:    18/Jun/02                   by: lbrenner@inf.pucrs.br                //
//  Last Change: 18/Jun/02                   by: lbrenner@inf.pucrs.br                //
//====================================================================================//
//  This class is used to store the data obtained from SAN textual input file in the  //
// state form.                                                                        //
//====================================================================================//  

#ifndef _STATE_H_
#define _STATE_H_

#include "sanfile.h"

class STATE {
  public:
    STATE();
    ~STATE();
    void free();
    // input functions
    void put_name(char *_name); // put the state name
    void put_reward(double _reward); // put the state reward
    int put_n_trans(int _n_trans); // put the number of transitions
    void put_trans_dst(int _dst); // put the destin state of the transition
    void put_from_dst(int _dst); // put the destine state of the from transition
    int put_event_n_mst(int _trans, int _n_mst); // put the master events
    int put_event_n_sync(int _trans, int _n_sync); // put the synch events
    int put_event_n_local(int _trans, int _n_local); // put the local events    
    int put_event_n_mst(int _n_mst); // put the master events
    int put_event_n_sync(int _n_sync); // put the synch events
    int put_event_n_local(int _n_local); // put the local events    
    int put_mst_name(int _trans, int _mst, char *_name); // put the master event name
    int put_mst_rate(int _trans, int _mst, int type, char *_rate, int _i_rate); // put the mst event rate
    int put_mst_prob(int _trans, int _mst, int type, char *_prob, int _i_prob); // put the mst event prob
    int put_mst_name(char *_name); // put the master event name
    int put_mst_rate(int type, char *_rate, int _i_rate); // put the mst event rate
    int put_mst_prob(int type, char *_prob, int _i_prob); // put the mst event prob
    int put_sync_name(int _trans, int _sync, char *name); // put the master event name
    int put_sync_prob(int _trans, int _sync, int type, char *_prob, int _i_prob); // put the mst event prob
    int put_sync_name(char *name); // put the master event name
    int put_sync_prob(int type, char *_prob, int _i_prob); // put the mst event prob
    int put_local_name(int _trans, int _local, char *name); // put the master event name
    int put_local_rate(int _trans, int _local, int type, char *_rate, int _i_rate); // put the mst event rate
    int put_local_prob(int _trans, int _local, int type, char *_prob, int _i_prob); // put the mst event prob
    int put_local_name(char *name); // put the master event name
    int put_local_rate(int type, char *_rate, int _i_rate); // put the mst event rate
    int put_local_prob(int type, char *_prob, int _i_prob); // put the mst event prob

    int reset();
    
    //output functions
    void get_name(char *_name); // get the state name
    double get_reward(); // get the state reward
    int get_n_trans(); // get the number of transitions
    int get_trans_dst(int _trans); // get the destin state of the transition
    int get_event_n_mst(int _trans); // put the master events
    int get_event_n_sync(int _trans); // get the synch events
    int get_event_n_local(int _trans); // get the local events    
    void get_mst_name(int _trans, int _mst, char *name); // get the master event name
    int get_mst_rate(int _trans, int _mst, char *_rate, int *_i_rate); // get the mst event rate
    int get_mst_prob(int _trans, int _mst, char *_prob, int *_i_prob); // get the mst event prob
    void get_sync_name(int _trans, int _sync, char *name); // get the master event name
    int get_sync_prob(int _trans, int _sync, char *_prob, int *_i_prob); // get the mst event prob
    void get_local_name(int _trans, int _local, char *name); // get the master event name
    int get_local_rate(int _trans, int _local, char *_rate, int *_i_rate); // get the mst event rate
    int get_local_prob(int _trans, int _local, char *_prob, int *_i_prob); // get the mst event prob
  
  private:
    char name[MAX_STATE_NAME]; // state name
    double reward; // state reward
    int n_trans; // number of trans
    int ind_trans; // current trans
    TRANS *trans; // trans structure 
}; 

#endif
