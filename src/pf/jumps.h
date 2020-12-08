//====================================================================================//
//                                                                                    //
//                              Class Permutation Jumps                               //
//                                                                                    //
//====================================================================================//
//  This File:   jumps.h                    Language: C++  (xlC and CC)               //
//  Software:    PEPS 3.1                    Advisor: Brigitte.Plateau@imag.fr        //
//  Error Codes: 5200                                                                 //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 14/feb/00                        by: Paulo.Fernandes@imag.fr         //
//====================================================================================//
//     The Permutation Jumps objects contains informations to all automata of the SAN //
// model. The automata are numbered from 0 to N-1 (N is the total number of automata  //
// expressed in the variable 'dimension').                                            //
//                                                                                    //
//     For each automaton 'a' three information are stored:                           //
//                                                                                    //
//        . 'n_left[a]' is the size of the state space corresponding to all automata  //
//             ranked "before" automaton 'a';                                         //
//        . 'n_right[a]' is the size of the state space corresponding to all automata //
//             ranked "after" automaton 'a';                                          //
//        . 'n_jump[a]' is the size of the state space corresponding to all automata  //
//             ranked "after" automaton 'a' and the automaton 'a' itself;             //
//                                                                                    //
//      The terms "before" and "after" of the previous definitions are based on a     //
// specific order (expressed by an 'aut_list' object). If no order is specified the   //
// original order (automata from 0 to N-1) is considered. No matter which order is    //
// is considered the result (the 'jumps' object arrays) are indexed by the automata   //
// id's (and not by the automata rank in the order).                                  //
//                                                                                    //
//    Note: The total number of the automata must be the current number of automta in //
// the model and not the original number of automata (if the automata were grouped).  //
//                                                                                    //
//    Note: Permutation Jumps objects store the jumps used in the vector-descriptor   //
// multiplication algorithm (see the INRIA repport 2935 for more information about    //
// the algorithm). The implementation of this algorithm is in the function 'multiply' //
// of the class 'VCT' (files 'vct.h' and vct.C').                                     //
//====================================================================================//

class jumps
{
public:
 inline jumps();                                                    // Empty Constructor
 inline jumps(const int dim);                                       // Sized Constructor
 inline jumps(const jumps & j);                                      // Copy Constructor
 inline ~jumps();                                                          // Destructor

 inline void create();                                            // Empty Intialization
 inline void create(const int dim);                               // Sized Intialization
 inline void copy(const jumps & j);                      // Copy and Sized Intialization
 inline void erase();                                                     // Destruction

 inline bool is_empty() const;                       // The allocation has not been done
 inline int  size() const;                               // The total number of automata

        void fill(const aut_st & s);
                      // It computes the jumps with the sizes 's' and the original order
        void fill(const aut_st & s, const aut_list & l);
                           // It computes the jumps with the sizes 's' and the order 'l'
        void fill(const aut_id a, const aut_st & s, const aut_list & l);
            // It computes the jumps only for the aut. 'a' (sizes 's' and the order 'l')

 inline int nleft(const aut_id a) const;                        // The value 'n_left[a]'
 inline int nright(const aut_id a) const;                      // The value 'n_right[a]'
 inline int njump(const aut_id a) const;                        // The value 'n_jump[a]'

        void write(ofstream & fout) const;               // Output on 'fout' file stream
        void read(ifstream & fin);                       // Input from 'fin' file stream
        int  mem_use() const;                  // Memory space used by 'this' (in bytes)

        //--------------------------------------------------------------------------//
        //                          Data Structures                                 //
        //--------------------------------------------------------------------------//
private: int   dimension;            // the total number of automata of the model
         int * n_left;               // the 'n_left' value for the automata
         int * n_right;              // the 'n_right' value for the automata
         int * n_jump;               // the 'n_jump' value for the automata
};
