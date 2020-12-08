//====================================================================================//
//                                                                                    //
//                                    AUNF Structure                                  //
//                                                                                    //
//====================================================================================//
//  This File:   aunf.h                      Language: C++  (xlC and CC)              //
//  Software:    PEPS 3.1                    Advisor: Brigitte.Plateau@imag.fr        //
//  Error Code:  unassigned                                                           //
//====================================================================================//
//====================================================================================//

#ifndef __AUNF_H__
#define __AUNF_H__

// Struct to store an AUNF (without STL) - 11/05/2010

// basic aunf structure
struct aunf_struct {
   double scalar;
   int basein;
   int baseout;
   aut_st pgs; // partial global state - store only part of a global state
};

// main typedef for an aunf
typedef struct aunf_struct aunf;

// list of aunfs
struct aunfs_ {
   int size;
   aunf *vaunf;
};

// main typedef for the list of aunfs
typedef struct aunfs_ aunfs_list;


#endif
