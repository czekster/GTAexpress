//====================================================================================//
//                                                                                    //
//                               Event Dictionary                                     //
//                                                                                    //
//====================================================================================//
//  This File:   edic.h                      Tool: C++                                //
//  Software:    PEPS                        Advisor: Paulof@inf.pucrs.br             //
//  Doc:                                                                              //
//====================================================================================//
//  Creation:    30/Jan/03                   by: lbrenner@inf.pucrs.br                //
//  Last Change: 30/Jan/03                   by: lbrenner@inf.pucrs.br                //
//====================================================================================//
//  This class is used to store the event dictinary obtained from SAN textual input   //
// file in the function form.                                                         //
//====================================================================================//  

#ifndef _EDIC_H_
#define _EDIC_H_

#include "sanfile.h"

class EDIC {
  public:
    EDIC();  
    ~EDIC();
    
    // input functions
    int put_n_event(int _n_event); // put the number of events
    int put_event(char *_name, int _type, char *_rate, int _f_rate); // put event
    void put_mst(char *_name, int _aut); // put master flag

    // output functions
    int get_type(char *_name); // get event type
    int get_f_rate(char *_name);// get event rate
    int get_rate(char *_name, char *_rate); // get functional rate
    int get_mst(char *_name); // get master flag
        
  private:
    char **name; // event name
    int *type; // event type (0 - local, 1 - synchronized)
    char **rate; // event rate
    int *f_rate; // functional rate
    int n_event; // number of events
    int ind;
    int *mst; 
}; 

#endif
