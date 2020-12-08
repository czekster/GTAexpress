//====================================================================================//
//                                                                                    //
//                                 Class Booleans                                     //
//                                                                                    //
//====================================================================================//
//  This File:   bools.h                    Language: C++  (xlC and CC)               //
//  Software:    PEPS 3.1                    Advisor: Brigitte.Plateau@imag.fr        //
//  Error Codes: 2100                                                                 //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 14/feb/00                        by: Paulo.Fernandes@imag.fr         //
//====================================================================================//
//                                                                                    //
//   This class implements array of booleans optimizing the storage space. The user   //
// may see this class objects as standard arrays of one word for each array element,  //
// but the implementation uses each bit of the machine word to store the array        //
// elements. In the current implementation up to 32 array elements are coded in each  //
// word.                                                                              //
//                                                                                    //
//   Note: This class belongs to PEPS 3.1 code, but it is general enough to be used   //
// in any other C++ program. In such a case, only the compilation flag '_PEPS_DEBUG_' //
// should be changed and some definitions explicited at the end of this file should   //
// be done.                                                                           //
//====================================================================================//

//---------------------------- Machine Dependent Constants ---------------------------//

const int          word_size = 32;          // number of bits of the machine word
const unsigned int all_zero  = 0U;          // all bits of the word are zero
const unsigned int all_one   = 4294967295U; // all bits of the word are one

class bools
{
public:
 inline bools();                                                    // Empty Constructor
 inline bools(const int dim);                                       // Sized Constructor
 inline bools(const bools & v);                                      // Copy Constructor
 inline ~bools();                                                          // Destructor

 inline void create();                                            // Empty Intialization
 inline void create(const int dim);                               // Sized Intialization
 inline void copy(const bools & v);                      // Copy and Sized Intialization
 inline void erase();                                                     // Destruction

 inline int  size() const;                                     // The boolean array size
 inline bool is_full() const;                                   // All booleans are true
 inline bool is_empty() const;                                 // All booleans are false
 inline int  has() const;                                   // The number of true values

 inline void put();                                        // Puts true in all positions
 inline void take();                                      // Puts false in all positions
 inline void assign(const bool v);        // Puts the boolean value 'v' in all positions

 inline void put(const int p);                          // Puts true at the position 'p'
 inline void take(const int p);                        // Puts false at the position 'p'
 inline void assign(const bool v, const int p);
                                       // Puts the boolean value 'v' at the position 'p'

 inline bool operator [] (const int p) const;   // The boolean value of the position 'p'

        void write(ofstream & fout) const;               // Output on 'fout' file stream
        void read(ifstream & fin);                       // Input from 'fin' file stream
        int  mem_use() const;                  // Memory space used by 'this' (in bytes)

#ifdef _PEPS_DEBUG_
        void dump();                             // Debugging option to dump the content
#endif

        //--------------------------------------------------------------------------//
        //                          Data Structures                                 //
        //--------------------------------------------------------------------------//
private: int            dimension;       // number of values
         int            words;           // number of words to store the values
         unsigned int * values;          // the values stored in a compact form

  static unsigned int  mask0010[word_size];  // a mask with 0000000001 , 0000000010 ...
  static unsigned int  mask1101[word_size];  // a mask with 1111111110 , 1111111101 ...
  static unsigned int  mask0011[word_size];  // a mask with 0000000001 , 0000000011 ...
  static unsigned int  mask1100[word_size];  // a mask with 1111111110 , 1111111100 ...
};


     //==========================================================================//
     //              Definitions to use this class outside of PEPS               //
     //==========================================================================//

     //--------------------------------------------------------------------------//
     //                           Basic definitions                              //
     //--------------------------------------------------------------------------//
     //                                                                          //
     //   The basic definitions are taken form the file 'peps.h'.                //
     //                                                                          //
     //   The non-PEPS users can use this class taking only the files 'bools.h'  //
     // and 'bools.C'. Removing the line '#include "peps.h"'.                    //
     //                                                                          //
     //   The PEPS users MUST keep these basic definitions as remarks, because   //
     // they are already defined in the file 'peps.h' (used by all PEPS classes).//
     //--------------------------------------------------------------------------//
     //#include <fstream.h>               // file streams (i/o)
     //
     //typedef int         bool;          // boolean variables
     //const   bool        true  = 1;     //    gcc and other non-standart c++
     //const   bool        false = 0;     //    compilers have built-in bool types
     // 
     //#include "bools.h"                 // class prototype
     //--------------------------------------------------------------------------//
     //                         Error Message Macro                              //
     //--------------------------------------------------------------------------//
     //                                                                          //
     //   The macro of error message is taken form the file 'interf.h'.          //
     //                                                                          //
     //   The PEPS users MUST keep this definition as remarks, because it is     //
     // already defined in the file 'interf.h' (used by all PEPS classes).       //
     //--------------------------------------------------------------------------//
     //#define Programming_Error(text, number) \
     //        cerr <<  "Programming Error - " << text << " (**** " << number
     //             << " ****)\n" << "(file 'bools.h' - line " << __LINE__ << ")\n";
     //--------------------------------------------------------------------------//
