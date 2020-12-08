//====================================================================================//
//                                                                                    //
//                                Transition Structure                                //
//                                                                                    //
//====================================================================================//
//  This File:   trans.h                     Tool: C++                                //
//  Software:    PEPS                        Advisor: Paulof@inf.pucrs.br             //
//  Doc:                                                                              //
//====================================================================================//
//  Creation:    18/Jun/02                   by: lbrenner@inf.pucrs.br                //
//  Last Change: 18/Jun/02                   by: lbrenner@inf.pucrs.br                //
//====================================================================================//
//  This class is used to store the data obtained from SAN textual input file in the  //
// transition form.                                                                   //
//====================================================================================//  

#ifndef _TRANS_H_
#define _TRANS_H_

#include "sanfile.h"

class TRANS {
  public:
    TRANS();
    ~TRANS();
    void free();
    // input functions
    void put_dst(int _dst); // put the destin state index
    int put_n_mst(int _mst); // put the number of master events
    int put_n_sync(int _sync); // put the number of synchronized events 
    int put_n_local(int _local); // put the number of local events
    int put_mst_name(int _mst, char *_name); // put the master event name
    int put_mst_rate(int _mst, int type, char *_rate, int _i_rate); // put the mst event rate
    int put_mst_prob(int _mst, int type, char *_prob, int _i_prob); // put the mst event prob
    int put_mst_name(char *_name); // put the master event name
    int put_mst_rate(int type, char *_rate, int _i_rate); // put the mst event rate
    int put_mst_prob(int type, char *_prob, int _i_prob); // put the mst event prob

    int put_sync_name(int _sync, char *name); // put the master event name
    int put_sync_prob(int _sync, int type, char *_prob, int _i_prob); // put the mst event prob
    int put_sync_name(char *name); // put the master event name
    int put_sync_prob(int type, char *_prob, int _i_prob); // put the mst event prob

    int put_local_name(int _local, char *name); // put the master event name
    int put_local_rate(int _local, int type, char *_rate, int _i_rate); // put the mst event rate
    int put_local_prob(int _local, int type, char *_prob, int _i_prob); // put the mst event prob
    int put_local_name(char *name); // put the master event name
    int put_local_rate(int type, char *_rate, int _i_rate); // put the mst event rate
    int put_local_prob(int type, char *_prob, int _i_prob); // put the mst event prob
    int reset();
    
    // output functions  
    int get_dst(); // get the destin state index
    int get_n_mst(); // geut the number of master events
    int get_n_sync(); // get the number of synchronized events 
    int get_n_local(); // get the number of local events
    void get_mst_name(int _mst, char *name); // get the master event name
    int get_mst_rate(int _mst, char *_rate, int *_i_rate); // get the mst event rate
    int get_mst_prob(int _mst, char *_prob, int *_i_prob); // get the mst event prob
    void get_sync_name(int _sync, char *name); // get the master event name
    int get_sync_prob(int _sync, char *_prob, int *_i_prob); // get the mst event prob
    void get_local_name(int _local, char *name); // get the master event name
    int get_local_rate(int _local, char *_rate, int *_i_rate); // get the mst event rate
    int get_local_prob(int _local, char *_prob, int *_i_prob); // get the mst event prob

  private:
    int dst; // destin state number
    int n_mst; // number of master synchronized events
    int i_mst; // master index
    EVENT *mst; // master synchronized events structure 
    int n_sync; // number of slave synchronized events
    int i_sync; // synchronized index
    EVENT *sync; // slave synchronized events structure 
    int n_local; // number of local events
    int i_local; // local    
    EVENT *local; // local events structure
}; 

#endif
