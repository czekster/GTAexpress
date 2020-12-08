//====================================================================================//
//                                                                                    //
//                       Integration Functions Table Structure                        //
//                                                                                    //
//====================================================================================//
//  This File:   inf.h                       Language: C++  (xlC and CC)              //
//  Software:    PEPS 3.1                    Advisor: Brigitte.Plateau@imag.fr        //
//  Error Code:  4400                                                                 //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 14/feb/00                        by: Paulo.Fernandes@imag.fr         //
//====================================================================================//
//  This class is used to define the PEPS main structure Integration Functions Table  //
// ('INF::inf'). Each entry stores a function tree object corresponding to a function //
// used to integrate the results of a probability vector.                             //
//                                                                                    //
//  The variables 'dimension' and 'number' store, respectively, the maximum and the   //
// current number of functions in the table.                                          //
//                                                                                    //
//  The array field 'table' stores the function tree of each function                 //
//                                                                                    //
//  The functions in PEPS do not change their parameters after the automata grouping. //
// The set of reference of the function parameters (indexes of automata) is kept.     //
// For example, a given functional element in the automaton 1 depends on the state of //
// the automata 0 and 2, i.e., the function tree has the tokens 'aut(0)' and 'aut(2)'.//
// This function does not change after a grouping of the automata 0 and 2. The call   //
// of a function, however, can be done either with the parameters to the set of       //
// reference of the original model, either with the parameters to the current set of  //
// reference of the model. The state of a group of automata is equivalent to the      //
// individual states of each automata used in the grouping. In the example above,     //
// each state of the group correspond to individual states of automata 0 and 2. These //
// individual states are stored in a state map object (class 'state_map').            //
//                                                                                    //
//  The number of original automata of the model and their sizes are stored in the    //
// variable 'domain'.                                                                 //
//====================================================================================//

class INF
{
public:
 inline INF();                                                      // Empty Constructor
 inline INF(const int dim);                                         // Sized Constructor
 inline INF(const INF & t);                                          // Copy Constructor
 inline ~INF();                                                            // Destructor

 inline void create();                                            // Empty Intialization
 inline void create(const int dim);                               // Sized Intialization
 inline void copy(const INF & t);                        // Copy and Sized Intialization
 inline void erase();                                                     // Destruction

 inline bool is_empty() const;                // The current number of functions is zero
 inline bool is_full() const;                // There is no more place for new functions

 inline void baptise(const peps_name file);                    // It assigns a file name
 inline void name(peps_name file) const;                     // It returns the file name
 inline int  size() const;                            // The maximum number of functions
 inline int  has() const;                             // The current number of functions
 inline int  ss() const;                             // The original state space product

 inline func_id put(const func_tree & f);          // It Includes function 't' in 'this'
 inline void    get(const func_id f, func_tree & t) const;
                                   // It copies the function located in entry 'f' to 't'

        //--------------------------------------------------------------------------//
        // It fills 's' with the parameters of the function located in the entry 'f'//
        //--------------------------------------------------------------------------//
        //    For the first function the set of reference of the output parameter   //
        // 's' is given from 0 to the the original number of automata (size of the  //
        // variable 'domain').                                                      //
        //--------------------------------------------------------------------------//
 inline rp eval(const func_id f, const aut_st & s) const;

 inline void put_domain(const aut_st & s); // It puts the sizes of the original automata
 inline void get_domain(aut_st & s) const;
                                 // It Fills 's' with the original sizes of the automata

        void show(const func_id f);
                           // It writes the function in entry 'f' on the standard output
        void write() const;                              // Output on 'fout' file stream
        void read(const peps_name file);                 // Input from 'fin' file stream
        void print() const;     // It writes all functions entries on the 'inf.dbg' file
        int  mem_use() const;                  // Memory space used by 'this' (in bytes)

        //--------------------------------------------------------------------------//
        //                          Static variable                                 //
        //--------------------------------------------------------------------------//
        static INF inf;

        //--------------------------------------------------------------------------//
        //                          Data Structures                                 //
        //--------------------------------------------------------------------------//
private: peps_name   file_name;      // the file name of this structure
         int         dimension;      // the maximum number of functions
         int         number;         // the current number of functions
         func_tree * table;          // the table of function trees
         aut_st      domain;         // the size of each original automata


      //--------------------------------------------------------------------------//
      //                           Internal Functions                             //
      //--------------------------------------------------------------------------//
         void alloc();                            // Internal Function of allocation
         void desal();                         // Internal Function of desallocation
};
