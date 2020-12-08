//==============================================================================//
//                                                                              //
//                             Network Dictionary Class                         //
//                                                                              //
//==============================================================================//
//  This File:   ndic.h                   Tool: C++                             //
//  Software:    PEPS                     Advisor: Brigitte.Plateau@imag.fr     //
//  Doc:                                                                        //
//==============================================================================//
//  Creation:    30/Aug/05                by: Leonardo.Brenner@imag.fr          //
//  Last Change: 30/Aug/05                by: Leonardo.Brenner@imag.fr          //
//==============================================================================//
//  This class is used to store the network dictionary (automata and states     //
// identifiers).                                                                //
//  Each automaton has an index associated to an identifier. Each state has two //
// indices associated, one to the state identifier and other to the automaton   //
// where it appears.                                                            //
//==============================================================================// 
#ifndef _NDIC__H_
#define _NDIC__H_

#include "peps.h"

class NDIC_ {
  public:
    NDIC_();                                                       // constructor
    ~NDIC_();                                                       // destructor
    
   // procedures to include automata and states 
   int put_aut(char *_id, int _aut);                      // put a new automaton
   int put_stt(char *_id, int _aut, int _stt);                // put a new state
   
   // procedures to get information of automata and states
   int get_n_aut();                             // return the number of automata
   int get_n_stt(int _aut);          // return the number of states in automaton
   int is_aut(char *_id);                      // verify the automaton existence 
   int get_aut(char *_id);                         // return the automaton index
   int get_aut_rep(char *_id, int _pos);//return the automaton index with replic 
   int get_aut_name(int _aut, char *_name);       // get name to automaton index
   int is_stt(char *_id);                          // verify the state existence 
   int get_stt(int _aut, char *_id);                   // return the state index
   int get_stt_rep(int _aut, char *_id, int _pos);     // return the state index
                                                       //            with replic
   int get_stt_name(int _aut, int _stt, char *_name); // get name to state index
   
   int check_stt_in_aut(char *_id, int _faut, int _laut); // check the existence 
                                                // of state in a set of automata

   void print();    

  private:
   void resize();                            // alloc space for more identifiers
  
    char **id;                                                    // identifiers
    int   *type;                // type of the identifier, it can be:AUT, or STT
    int   *aut;                                               // automaton index
    int   *stt;                                                   // state index
    int    ind;                                             // used domain index
    int    n;                                          // total domain positions
    
};
#endif
