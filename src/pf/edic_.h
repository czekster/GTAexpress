//==============================================================================//
//                                                                              //
//                             Event Dictionary Class                           //
//                                                                              //
//==============================================================================//
//  This File:   edic.h                   Tool: C++                             //
//  Software:    PEPS                     Advisor: Brigitte.Plateau@imag.fr     //
//  Doc:                                                                        //
//==============================================================================//
//  Creation:    30/Aug/05                by: Leonardo.Brenner@imag.fr          //
//  Last Change: 30/Aug/05                by: Leonardo.Brenner@imag.fr          //
//==============================================================================//
//  This class is used to store events information extracted from SAN textual   //
// input file.                                                                  //
//  Each event is defined by an identifier, a type, a rate, the master automaton//
// and a vector with the concerned automata.                                    //
//  The event type can be synchronizing (SYN) or local (LOC).                   //
//  The event rate is the function index associated to the event.               //
//  The master automaton is the first concerned automaton.                      //
//==============================================================================// 

#ifndef _EDIC__H_
#define _EDIC__H_

#include "peps.h"

class EDIC_ {
  public:
    EDIC_();                                                        // contructor 
    ~EDIC_();                                                       // destructor
    
    // procedures to input events informations
    int  put_ev(char *_id, int _type, int _rate);             // create an event
    void concerned_aut(char *_id, int _aut); // it say that an event is declared 
                                             //                  in an automaton
    void put_n_aut(int _n_aut);       // put the number of automata in the model
    

    // procedures to get events informations    
    int  get_n_ev();             // return the number of the events in the model
    int  get_ev(char *_id);                            // return the event index 
    void get_ev_id(int _ev, char *_id);              // get the event identifier
    int  get_ev_type(int _ev);                          // return the event type
    int  get_ev_rate(int _ev);                          // return the event rate 
    int  get_n_synch_ev();      // return the number of the synchronizing events
    int  get_real_synch_ev(int synch_ev);       // return the real synchronizing 
                                                //                   event index
    int  get_mst_aut(int _ev);              // return the master automaton index
    int  is_ev(char *_id);          // return true if the identifier is an event 
    int  is_concerned(int _ev, int _aut);          // return true if the _aut is 
                                                   // concerned by the event _ev
                                                   
    //  procedures to check automata concerned in local and synchronizing events
    void  check_ev();  // check if local events have only one automaton concerned 
                      //      and synchronizing events have two or more automata
                      //                                               concerned
    

    void print();      // print a readable version of the event dictionary class

  private:
    void resize();                            // alloc space for more identifiers
  
    char **id;                                                    // identifiers
    int   *type;                   // type of the event, it's can be: LOC or SYN
    int   *rate;                // index of the function associated to the event
    int  **c_aut;                       // set of automata involved by the event
    int    n_aut;                                          // number of automata
    int    ind;                                             // used events index
    int    n;                                          // total events positions
    
};
#endif
