//====================================================================================//
//                                                                                    //
//                                Events Structure                                    //
//                                                                                    //
//====================================================================================//
//  This File:   event.h                     Tool: C++                                //
//  Software:    PEPS                        Advisor: Paulof@inf.pucrs.br             //
//  Doc:                                                                              //
//====================================================================================//
//  Creation:    18/Jun/02                   by: lbrenner@inf.pucrs.br                //
//  Last Change: 18/Jun/02                   by: lbrenner@inf.pucrs.br                //
//====================================================================================//
//  This class is used to store the data obtained from SAN textual input file in the  //
// events form (local or synchronized).                                               //
//====================================================================================//  

#ifndef _EVENT_H_
#define _EVENT_H_

#include "sanfile.h"

class EVENT {
  public:
    EVENT();
    ~EVENT();
    // input functions
    void put_name(char *_name); // put the event name
    int put_rate(int type, char *_rate, int _i_rate); // put the event rate and type
    int put_prob(int type, char *_prob, int _i_prob); // put the event probability

    
    // output functions
    void get_name(char *name); // get the event name
    int get_rate(char *_rate, int *_i_rate); // get the event rate and type
    int get_prob(char *_prob, int *_i_prob); // get the event probability
  
  private:
    char name[MAX_EVENT_NAME]; // event name
    char rate[64]; // transition rate
    char prob[64]; // transition probability 
    int t_rate; // transition rate type
    int t_prob; // transition probability type
    int i_rate; // rate function identifier 
    int i_prob; // probability function identifier
}; 

#endif
