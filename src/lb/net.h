//====================================================================================//
//                                                                                    //
//                                   Network Structure                                //
//                                                                                    //
//====================================================================================//
//  This File:   net.h                       Tool: C++                                //
//  Software:    PEPS                        Advisor: Paulof@inf.pucrs.br             //
//  Doc:                                                                              //
//====================================================================================//
//  Creation:    18/Jun/02                   by: lbrenner@inf.pucrs.br                //
//  Last Change: 18/Jun/02                   by: lbrenner@inf.pucrs.br                //
//====================================================================================//
//  This class is used to store the data obtained from SAN textual input file in the  //
// network form.                                                                      //
//====================================================================================//  

#ifndef _NET_H_
#define _NET_H_

#include "sanfile.h"

class NET {
  public:
    NET();
    ~NET();
    void free();
    // input functions
    void put_net_name(char *_name); // put the network name 
    void put_net_model(int _model); // put the model type
    int put_net_n_aut(int _n_aut); // put the number of automaton in network
    int put_net_n_ident(int _n_ident); // put the number of identifiers in network 
    int put_net_n_result(int _n_result); // put the number of results in network
    int put_aut_name(int _aut, char *_name); // put the automaton name
    int put_aut_n_state(int _aut, int _n_state); // put the number of state in an automaton
    int put_state_name(int _aut, int _state, char *_name); // put the state name
    void put_state_reward(int _aut, int _state, double _reward); // put the state reward
    int put_state_n_trans(int _aut, int _state, int _n_trans); // put the number of transitions 
    int put_trans_dst(int _aut, int _state, int type, char *_dst); // put destin state
    int put_from_dst(int _aut, int _state, char *_dst); // put from primitive destin state
    int put_event_n_mst(int _aut, int _state, int _trans, int _n_mst); // put the master events
    int put_event_n_sync(int _aut, int _state, int _trans, int _n_sync); // put the synch events
    int put_event_n_local(int _aut, int _state, int _trans, int _n_local); // put the local events    
    int put_event_n_mst(int _aut, int _state, int _n_mst); // put the master events
    int put_event_n_sync(int _aut, int _state, int _n_sync); // put the synch events
    int put_event_n_local(int _aut, int _state, int _n_local); // put the local events    

    int put_mst_name(int _aut, int _state, int _trans, int _mst, char *_name); 
    // put the master event name
    int put_mst_rate(int _aut, int _state, int _trans, int _mst, int type, char *_rate, int _i_rate); 
    // put the mst event rate
    int put_mst_prob(int _aut, int _state, int _trans, int _mst, int type, char *_prob, int _i_prob); 
    // put the mst event prob
    int put_mst_name(int _aut, int _state, char *_name); 
    // put the master event name
    int put_mst_rate(int _aut, int _state, int type, char *_rate, int _i_rate); 
    // put the mst event rate
    int put_mst_prob(int _aut, int _state, int type, char *_prob, int _i_prob); 
    // put the mst event prob

    int put_sync_name(int _aut, int _state, int _trans, int _sync, char *name); 
    // put the master event name
    int put_sync_prob(int _aut, int _state, int _trans, int _sync, int type, char *_prob, int _i_prob); 
    // put the mst event prob
    int put_sync_name(int _aut, int _state, char *name); 
    // put the master event name
    int put_sync_prob(int _aut, int _state, int type, char *_prob, int _i_prob); 
    // put the mst event prob

    int put_local_name(int _aut, int _state, int _trans, int _local, char *name); 
    // put the master event name
    int put_local_rate(int _aut, int _state, int _trans, int _local, int type, char *_rate, int _i_rate); 
    // put the mst event rate
    int put_local_prob(int _aut, int _state, int _trans, int _local, int type, char *_prob, int _i_prob); 
    // put the mst event prob
    int put_local_name(int _aut, int _state, char *name); 
    // put the master event name
    int put_local_rate(int _aut, int _state, int type, char *_rate, int _i_rate); 
    // put the mst event rate
    int put_local_prob(int _aut, int _state, int type, char *_prob, int _i_prob); 
    // put the mst event prob

    int put_ident_name(char *_name); // put the identifier name
    int put_ident_exp(char *_exp); // put the identifier expression
    int put_result_name(char *_name); // put the result name
    int put_result_exp(char *_exp); // put the result expression
    int put_reach_exp(char *_exp); // put the reachability expression
    int put_jit_name(char *_name); // put the jit name
    int put_jit_exp(char *_exp); // put the jit expression
    
    // output functions
    void get_net_name(char *_name); // get the network name 
    int get_net_model(); // get the model type
    int get_net_n_aut(); // get the number of automaton
    int get_net_n_ident(); // get the number of identifiers in network 
    int get_net_n_result(); // get the number of results in network
    void get_aut_name(int _aut, char *_name); // get the automaton name
    int get_aut_n_state(int _aut); // get the number of state in an automaton
    void get_state_name(int _aut, int _state, char *_name); // get the state name
    double get_state_reward(int _aut, int _state); // get the state reward
    int get_state_n_trans(int _aut, int _state); // get the number of transitions
    int get_trans_dst(int _aut, int _state, int _trans); // get the destin state
    int get_event_n_mst(int _aut, int _state, int _trans); // put the master events
    int get_event_n_sync(int _aut, int _state, int _trans); // get the synch events
    int get_event_n_local(int _aut, int _state, int _trans); // get the local events    
    void get_mst_name(int _aut, int _state, int _trans, int _mst, char *name); 
    // get the master event name
    int get_mst_rate(int _aut, int _state, int _trans, int _mst, char *_rate, int *_i_rate); 
    // get the mst event rate
    int get_mst_prob(int _aut, int _state, int _trans, int _mst, char *_prob, int *_i_prob); 
    // get the mst event prob
    void get_sync_name(int _aut, int _state, int _trans, int _sync, char *name); 
    // get the master event name
    int get_sync_prob(int _aut, int _state, int _trans, int _sync, char *_prob, int *_i_prob); 
    // get the mst event prob
    void get_local_name(int _aut, int _state, int _trans, int _local, char *name); 
    // get the master event name
    int get_local_rate(int _aut, int _state, int _trans, int _local, char *_rate, int *_i_rate); 
    // get the mst event rate
    int get_local_prob(int _aut, int _state, int _trans, int _local, char *_prob, int *_i_prob); 
    // get the mst event prob
    int get_ident_name(int _ident, char *_name); // get function name
    int get_ident_name(char *_name); // get function index by name
    int get_ident_exp(int _ident, char *_exp); // get the identifier expression
    int get_ident_exp(char *_name, char *_exp); // get expression by name
    int get_ident_exp(char *_exp); // get function index by expression
    int get_ident_n_exp(); // get the number of expression
    int get_result_name(int _result, char *_name); // get the result name
    int get_result_exp(int _result, char *_exp); // get the result expression
    int get_result_n_exp(); 
    int get_reach_exp(char *_exp); // get the reachability expression
    int get_jit_name(int _jit, char *_name); // get the jit name
    int get_jit_exp(int _jit, char *_exp); // get the jit expression
    int get_name(char *_name); // get automaton or state index
    int get_name(char *_name, int i);  // get state index in the automaton i
    int get_name(char *_name, int i, int j); // get state index in the automaton interval
    int rm_ident_cnst();
    int rm_reach_cnst();
    int rm_result_cnst();
    int reset();
    
    // write functions
    int put2file_des(char *path); // write positive and negativa matrices in file
    int put2file_des_d(char *path); // write positive and negativa matrices in file (discrete time)
    int put2file_dic(char *path); // write the data dictionary in file 
    int put2file_dic_d(char *path); // write the data dictionary in file (discrete time)
    int put2file_fct(char *path); // write reachability function in file
    int put2file_tft(char *path); // write functions in file
    int put2file_res(char *path); // write result functions in file
    int put2file_jit(char *path); // write just in time functions in file

    
  private:
    int count_events(); // count the number of synchronized events
    int get_event_name(int e, char *_name); // get the name of event i
    void put_local(fstream &fout, int _aut); // print local matrix
    int positive_event(fstream &fout, int i, char *_event); // print positive matrix
    int negative_event(fstream &fout, int i, char *_event, int m_s); // print negative matrix
    void print_events(fstream &fout, int _n);// print synchronized events

	int mount_s(int n_ev); // mount the compatible events (discrete time)
    int count_events_d(); // count the number of synchronized events (discrete time)
    void put_false_local(fstream &fout, int _aut); // print false local matrix (discrete time)
    void put_local_d(fstream &fout, int _aut); // print local matrix (discrete time)
    int positive_c_event(fstream &fout, int i, char *_event);
    // print positive matrix to complementar events (discrete time)
    int negative_c_event(fstream &fout, int i, char *_event, int m_s); 
    // print negative matrix to complementar events (discrete time)
    void print_events_d(fstream &fout, int _n);// print synchronized events (discrete time)
    int next_tok(char *buf, char *aux); // get the next token
    void put_func(fstream &fout, int i, char *aux);// put just in time functions in file

    char name[MAX_MODEL_NAME]; // network name
    int n_aut; // number of automaton
    int n_ident; // number of identifiers
    int n_result; // number of results
    int n_e_table; // number of synchonized events
    int n_s_table; // number of compatible events
    int model; // model type 
    int **e_table; // synchronized events table
    int **s_table; // compatible events table
    AUTOMATON *automaton; // automaton structure
    FUNC ident; //identifier structure
    FUNC jit; // just in time structure 
    FUNC reach; // reachability structure
    FUNC result; //result structure 
};

#endif
