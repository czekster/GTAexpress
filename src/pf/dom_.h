//==============================================================================//
//                                                                              //
//                                    Domain Class                              //
//                                                                              //
//==============================================================================//
//  This File:   dom.h                    Tool: C++                             //
//  Software:    PEPS                     Advisor: Brigitte.Plateau@imag.fr     //
//  Doc:                                                                        //
//==============================================================================//
//  Creation:    30/Aug/05                by: Leonardo.Brenner@imag.fr          //
//  Last Change: 30/Aug/05                by: Leonardo.Brenner@imag.fr          //
//==============================================================================//
//  This class is used to store the domain data obtained from SAN textual input //
// file.                                                                        //
//  A domain is a list of intervals. A domain can a simples interval ([0..3]),  //
// a list of number ([0, 1, 2, 3, 5, 7, 8, 9]), or a combination of simples     //
// intervals and number ([0..3], 5, [7..9]). The domain order must to increase. //
//==============================================================================// 
#ifndef _DOM__H_
#define _DOM__H_

#include "peps.h"

class DOM_ {
  public:
    DOM_();                                                        // constructor
    ~DOM_();                                                        // destructor

    // procedures to manipulate the temporary domain 
    int  build_dom(char *_id);   // build a domain based in the temporary domain    
    void put_elem(int _elem);    // put a single element in the temporary domain
    void put_elem(char *_elem);   
    int  put_interv(int _first, int _last);            // put an interval in the
    int  put_interv(char *_first, char *_last);         //      temporary domain

    // procedures to manipulate the domains
    int get_interv(char *_id, int pos, int _first, int _last); // get the pos-th 
                                                              // domain interval
    int get_n_interv(char *_id);                  // get the number of intervals 
    int get_length(char *_id);                     // get the number of elements
    int get_first_pos(char *_id);                       // get the first element
    int get_last_pos(char *_id);                         // get the last element
    int get_pos(char *_id, int pos);                   // get the pos-th element
    int is_dom(char *_id);                 // return true if the _id is a domain    
    int check_dom(char *_id);               // check the increasing domain order      

    void print();            // print a readable version of the domain structure

  private:
    void resize();                               // alloc space for more domains
  
    // main domain structure
    char **id;                                              // domain identifier
    int  **elem;                                       // elements of the domain
    int   *length;                                              // domain length
    int  **interv;           // position where start each interval in the vector
    int   *n_interv;                                        //number of intevals
    int    ind;                                             // used domain index
    int    n;                                          // total domain positions
    
    // temporary structure to store a domain in construction
    int   *t_elem;           // temporary vector used for domain in construction
    int    t_length;           // temporary length of the domain in construction
    int    t_n_int; // current number of intervals of the domain in construction

};
#endif
