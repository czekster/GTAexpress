//==============================================================================//
//                                                                              //
//                                    State Class                               //
//                                                                              //
//==============================================================================//
//  This File:   state.h                  Tool: C++                             //
//  Software:    PEPS                     Advisor: Brigitte.Plateau@imag.fr     //
//  Doc:                                                                        //
//==============================================================================//
//  Creation:    30/Aug/05                by: Leonardo.Brenner@imag.fr          //
//  Last Change: 30/Aug/05                by: Leonardo.Brenner@imag.fr          //
//==============================================================================//
//  This class is used to store a state with its transitions and events.        //
//  The state is defined by a name, a reward value, and a transition matrix. In //
// the transition matrix, each row represent a transition to one state, the     //
// number of rows is equal to the number of state in the automaton. The columns //
// represent the events. Each position (row, column) represent a transition to  //
// the state index row with the event index column.                             //
//  For exemple: one automaton with 3 states and 5 event in the model. The state//
// 1 has transtions to state 2 with event 1, 2, and 3 and to state 3 with event //
// 2 and 5. The transitions with event 2 have probabilities function 3 and 0    //
// associated to the transition state 2 and 3, respectively. The event 4 is not //
// declared in this state. The matrix to this state is following:               //
//   _|__1__2__3__4__5                                                          //
//   1|                                                                         //
//   2| -1 3 -1 -2 -2                                                           //
//   3| -2 0 -2 -2 -1                                                           //
//  In the matrix, the value -2 represent the no event occurrence in transition //
// The value -1 represent the event occurrence with probability 1 and the posi- //
// tive values represent a transition with probability functions. The value     //
// represent the function index. If no exist transition to a state, the row is  //
// NULL.                                                                        //
//==============================================================================// 
#ifndef _STATE__H_
#define _STATE__H_

#include "peps.h"

class STATE_ {
  public:
    STATE_();                                                      // constructor  
    ~STATE_();                                                      // destructor
    
    void create_tr(int _n_stt);                  // create the transition matrix
    void put_name(char *_name);                            // put the state name
    void put_reward(double _rw);                             // put reward value
    int  put_ev(int _stto, int _ev, int _prob);         // put event information
   
    void info_ev(int _n_ev);         // inform the number of events in the model

    double get_reward();                                  // return reward value
    int    get_ev(int _stto, int _ev);                  // get event information

    void print();                 // print a readable version of the state class
  private:
    char   name[MAX_STT_NAME];                                     // state name
    double rw;                                                   // reward value
    int    n_ev;                                 // number of event in the model
    int    n_stt;                            // number of state in the automaton
    int  **tr_ev;                                           // transition matrix
}; 

#endif
