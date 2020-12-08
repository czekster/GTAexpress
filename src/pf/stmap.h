//====================================================================================//
//                                                                                    //
//                                Class State Map                                     //
//                                                                                    //
//====================================================================================//
//  This File:   stmap.h                    Language: C++  (xlC and CC)               //
//  Software:    PEPS 3.1                    Advisor: Brigitte.Plateau@imag.fr        //
//  Error Codes: 5100                                                                 //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 28/jun/97                        by: Paulo.Fernandes@imag.fr         //
//====================================================================================//
//   In the process of grouping, a subset of the automata can be aggregated into a    //
// single group. After the grouping, a reduction of the number of states of a group   //
// can be performed. A state map object is associated to each group (called "grouped  //
// automaton") to keep the following information:                                     //
//                                                                                    //
//   - the number of automata composing the grouped automaton (variable 'automata');  //
//   - the size of the grouped automaton (variable 'rows');                           //
//   - which automata were grouped to generate this grouped automaton (array variable //
//      'position');                                                                  //
//   - which original local states corresponds to each state of the grouped automaton //
//      (array variable 'states');                                                    //
//                                                                                    //
//   The array variable 'position' stores one automaton id for each original          //
// automaton that has been grouped. Obviously the set of reference of those automata  //
// is the set of reference of the original model (from '0' to 'N-1', where 'N' is the //
// original number of automata of the model). Remind that grouping can only be done   //
// from the original model (see 'DSC' class - file 'dsc.h').                          //
//                                                                                    //
//   The 'states' table has one entries to each state of the grouped automaton. In    //
// each one of those entries, there is the vector of local states equivalent to the   //
// state of the grouped automaton. The number of local states is equal to the number  //
// of original automata that have been grouped.                                       //
//                                                                                    //
// Obs.: Automata that represent one single automaton (non-grouped) are called        //
// 'singular' automata. The states of singular automata correspond to only one        //
// original local state and its values are '0', '1', '2', and so on.                  //
//====================================================================================//

class st_map
{
public:
 inline st_map();                                                   // Empty Constructor
 inline st_map(const int r, const aut_id a);                        // Sized Constructor
 inline st_map(const st_map & m);                                    // Copy Constructor
 inline ~st_map();                                                         // Destructor

 inline void create();                                            // Empty Intialization
 inline void create(const int r, const aut_id a);                 // Sized Intialization
 inline void copy(const st_map & m);                     // Copy and Sized Intialization
 inline void erase();                                                     // Destruction

 inline int  size() const;              // The number of states of the grouped automaton
 inline int  aut() const;                // The number of automata that has been grouped
 inline int  row() const;               // The number of states of the grouped automaton
 inline bool is_empty() const;     // The grouped automaton has no states ('row() == 0')
 inline bool is_singular() const;    // The grouped automaton is singular ('aut() == 1')

        void put(const aut_set & s);
                    // It puts the automata of 's' as the automata that has been grouped
        void put(const aut_id a, const int p);
                  // It puts automaton 'a' as the 'p'-th automaton that has been grouped

        void fill();
              // It fills the array 'states' with a singular order ('0', '1', and so on)
        void fill(const aut_st & s); // It fills the array 'states' with a lexical order
                          // (the parameter 's' has the size of each original automaton)

        //--------------------------------------------------------------------------//
        //  It puts the local states of 's' in the entry 'r' of the array 'states'  //
        //--------------------------------------------------------------------------//
        // Obs.: the size of the state vector 's' is the original number of         //
        // automata. Only the local states of the original automata used by 'this'  //
        // are stored as original states corresponding the state 'r'.               //
        //--------------------------------------------------------------------------//
        void put(const st_id r, const aut_st & s);


 inline void   get(aut_set & s) const;
                                // It fills 's' with the automata that have been grouped
 inline aut_id get(const int p) const;
                                   // It gets the 'p'-th automaton that has been grouped

        //--------------------------------------------------------------------------//
        //     It gets the local states of the entry 'r' of the array 'states'      //
        //--------------------------------------------------------------------------//
        // Obs.: the size of the output parameter 's' is the original number of     //
        // automata. Only the local states of the original automata used by 'this'  //
        // are filled with the original states corresponding the state 'r'.         //
        //--------------------------------------------------------------------------//
 inline void   get(const st_id r, aut_st & s) const;

        //--------------------------------------------------------------------------//
        //           It gets the grouped state of the original states 's'           //
        //--------------------------------------------------------------------------//
 inline int   get(aut_st & s);

        void reduce(const bools & v); 
                     // It reduces the state map keeping only the states that are in 'v'

        void write() const;                                    // Output on stand stdout
        void write(ofstream & fout) const;               // Output on 'fout' file stream
        void read(ifstream & fin);                       // Input from 'fin' file stream
        int  mem_use();                        // Memory space used by 'this' (in bytes)

        //--------------------------------------------------------------------------//
        //                          Data Structures                                 //
        //--------------------------------------------------------------------------//
private: int      rows;     // the number of states of the grouped automaton
         aut_id   automata; // the number of automata that has been grouped
         aut_id * position; // which automata were grouped into this grouped automaton
         st_id ** states;   // which original local states corresponds to each state of
                            //    the grouped automaton

         //--------------------------------------------------------------------------//
         //                           Internal Functions                             //
         //--------------------------------------------------------------------------//
         void alloc();                               // Internal Function of allocation
         void trans(const st_map & m);         // Internal Function of tranfer contents
         void desal();                            // Internal Function of desallocation
};

