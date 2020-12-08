//====================================================================================//
//                                                                                    //
//                               Dominion Structure                                   //
//                                                                                    //
//====================================================================================//
//  This File:   dom.C                       Tool: C++                                //
//  Software:    PEPS                        Advisor: Paulof@inf.pucrs.br             //
//  Doc:                                                                              //
//====================================================================================//
//  Creation:    30/Jan/03                   by: lbrenner@inf.pucrs.br                //
//  Last Change: 30/Jan/03                   by: lbrenner@inf.pucrs.br                //
//====================================================================================//
//  This class is used to store the dominion obtained from SAN textual input file in  //
// the function form.                                                                 //
//====================================================================================//  

#ifndef _DOM_H_
#define _DOM_H_

#include "sanfile.h"

class DOM {
  public:
    DOM();  
    ~DOM();
    int alloc(int _n); // alloc functions space
    int free();
    
    // input functions
    int put_name(char *_name); // put function name
    int put_dom(int _first, int _last); // put dominion limits

    // output functions
    int get_name(int i, char *_name); // get dominion name
    int get_name(char *_name);// get dominion index by name
    int get_first(int i); // get dominion limit (first)
    int get_first(char *_name); // get dominion limit (first) by name
    int get_last(int i); // get dominion limit (last)
    int get_last(char *_name); // get dominion limit (last) by name
    int get_dom_interv(int i, int *_first, int *_last); // get dominion interval
    int get_n_dom(); // get the number dominion
        
  private:
    int resize(); // alloc more space for dominions
        
    char **name; // dominion name
    int *first; // initial limit
    int *last; // final limit
    int ind; // total dominion index
    int n; // total dominion positions
}; 

#endif
