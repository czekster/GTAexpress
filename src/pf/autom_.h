//==============================================================================//
//                                                                              //
//                                  Automaton Class                             //
//                                                                              //
//==============================================================================//
//  This File:   autom.h                  Tool: C++                             //
//  Software:    PEPS                     Advisor: Brigitte.Plateau@imag.fr     //
//  Doc:                                                                        //
//==============================================================================//
//  Creation:    30/Aug/05                by: Leonardo.Brenner@imag.fr          //
//  Last Change: 30/Aug/05                by: Leonardo.Brenner@imag.fr          //
//==============================================================================//
//  This class is used to model one automaton. It store the automaton name and  //
// the array of states.                                                         //
//==============================================================================// 
#ifndef _AUTOMATON__H_
#define _AUTOMATON__H_

#include "peps.h"

class AUTOMATON_ {
  public:
    AUTOMATON_();                                                  // constructor
    ~AUTOMATON_();                                                  // destructor
    
    // automaton procedures
    void put_name(char *_name);                        // put the automaton name
    void info_ev(int _n_ev);         // inform the number of events in the model
    void create_stt(int _n_stt);                       // create a set of states
      
    // procedure to manipulate states
    void   put_stt(char *_name, int _stt, double _rw);         // put state info
    double get_stt_reward(int _stt);                  // return the state reward
    
    // procedure to manipulate events
    void put_ev(int _stt, int _stto, int _ev, int _prob);      // put event info
    int  get_ev(int _stt, int _stto, int _ev);              // return event info
    
    void print();             // print a readable version of the automaton class
        
  private:
    char name[MAX_AUT_NAME];                                   // automaton name
    int n_stt;                                               // number of states
    STATE_ *stt;                                               // state structure
};

#endif
