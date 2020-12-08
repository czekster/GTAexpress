//====================================================================================//
//                                                                                    //
//                             Functions Table Structure                              //
//                                                                                    //
//====================================================================================//
//  This File:   ftb.h                      Language: C++  (xlC and CC)               //
//  Software:    PEPS 2002                   Advisor: Brigitte.Plateau@imag.fr        //
//  Error Code:  4300                                                                 //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 17/feb/03                        by: lbrenner@inf.pucrs.br           //
//====================================================================================//
//  This class is used to define the PEPS main structure Function Table ('FTB::ftb'). //
// Each entry stores a function tree object corresponding to functional elements of   //
// the descriptor matrices.                                                           //
//                                                                                    //
//  The variables 'dimension' and 'number' store, respectively, the maximum and the   //
// current number of functions in the table.                                          //
//                                                                                    //
//  Two array fields ('table' and 'param' respectively) store the function tree of    //
// each function and their parameters (an automata subset object).                    //
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
// variable 'domain'. The variable 'currents' stores the current group id of each     //
// original automata. After grouping, a group id has the same type than an automaton  //
// id.                                                                                //
//                                                                                    //
//  The reward of each state of each original automata is also stored into this       //
// structure ('FTB::ftb'). The two-dimensional array 'reward_table' has in the entry  //
// 'reward_table[a][s]' the reward associated to the state 's' of the automaton 'a'.  //
// The allocation of this array is performed automatically when the size of each      //
// original automata is informed (function 'put_domain'). The size of the array       //
// 'domain' different from zero is used as a flag to indicates if the 'reward_table'  //
// is allocated.                                                                      //
//====================================================================================//

class FTB
{
public:
 inline FTB();                                                      // Empty Constructor
 inline FTB(const int dim);                                         // Sized Constructor
 inline FTB(const FTB & t);                                          // Copy Constructor
 inline ~FTB();                                                            // Destructor

 inline void create();                                            // Empty Intialization
 inline void create(const int dim);                               // Sized Intialization
        void create(scr_ft & sft);   // It Creates 'this' according to the scratch 'sft'
 inline void copy(const FTB & t);                        // Copy and Sized Intialization
 inline void erase();                                                     // Destruction

 inline bool is_empty() const;                // The current number of functions is zero
 inline bool is_full() const;                // There is no more place for new functions
 inline bool is_grouped() const;                   // The model has already been grouped

 inline void baptise(const peps_name file);                    // It assigns a file name
 inline void name(peps_name file) const;                     // It returns the file name
 inline int  size() const;                            // The maximum number of functions
 inline int  has() const;                             // The current number of functions
 inline int  aut() const;                             // The original number of automata
 inline int  g_aut() const;                            // The current number of automata
 inline int  ss() const;                             // The original state space product

 inline rp   reward(const aut_id a, const st_id s) const;
                            // The reward of the state 's' of the original automaton 'a'
 inline void reward(const aut_id a, const st_id s, const rp r);
             // It puts 'r' as the reward of the state 's' of the original automaton 'a'
        void init_reward(); // Initializes all rewards with the internal id of the state

 inline aut_id g_aut(const aut_id a) const;    // The current group of the automaton 'a'

 inline func_id put(const func_tree & f);          // It Includes function 't' in 'this'
 inline void    get(const func_id f, func_tree & t) const;
                                   // It copies the function located in entry 'f' to 't'

        //--------------------------------------------------------------------------//
        // It fills 's' with the parameters of the function located in the entry 'f'//
        //--------------------------------------------------------------------------//
        //    For the first function the set of reference of the output parameter   //
        // 's' is given from 0 to the the original number of automata (size of the  //
        // variable 'domain').                                                      //
        //                                                                          //
        //    For the second function the set of reference of the output parameter  //
        // 's' is given from 0 to the the current number of automata (number of     //
        // groups in the variable 'currents').                                      //
        //--------------------------------------------------------------------------//
 inline void params(const func_id f, aut_set & s) const;
 inline void g_params(const func_id f, aut_set & s) const;

        //--------------------------------------------------------------------------//
        //         It evaluates the function in entry 'f' for a global state        //
        //--------------------------------------------------------------------------//
        //    For the first function the global state is directly given by the      //
        // input parameter 's'. In this case the size of 's' must be equal to the   //
        // original number of automata (size of the variable 'domain').             //
        //                                                                          //
        //    For the second function the global state is given the individual      //
        // state of each current automata. The number of current automata is given  //
        // by the number of different labels in the variable 'current'. To obtain   //
        // the global state according to the original number of automata the array  //
        // of state maps 'm' is used.                                               //
        //                                                                          //
        //    For the third function the global state is also given (like in the    //
        // previous function - g_eval). But in this thrid function (jit_g_eval) the //
        // evaluation is made using functions compiled 'just-in-time'.              //
        //--------------------------------------------------------------------------//
 inline rp eval(const func_id f, const aut_st & s) const;
 inline rp g_eval(const func_id f, const aut_st & s, const st_map * m);
 inline rp jit_g_eval(const func_id f, const aut_st & s, const st_map * m);

        //--------------------------------------------------------------------------//
        //   It verifies if a function becomes constant in the process of grouping  //
        //--------------------------------------------------------------------------//
        //   Denote: P   the set of parameters of the function in the entry 'f';    //
        //           S   the set of automata whose states are known;                //
        //           U   the set of automata whose states are unknown;              //
        //           SP  the intersection of P and S;                               //
        //           UP  the intersection of P and U;                               //
        //           R   the set of automata state values for the automata in SP;   //
        //                                                                          //
        //   This function tests if the function 'f' is a constant on R.            //
        //                                                                          //
        //   Input parameters: 's' the set S denoted above;                         //
        //                     'v' the set R;                                       //
        //                                                                          //
        //   All the evaluations of the function are computed for all possible      //
        // values of the arguments (P) having the constant value R on SP. If the    //
        // same evaluation is found, it returns 'true' and the constant value is    //
        // stored in 'c'. Otherwise, it returns 'false'.                            //
        //                                                                          //
        // Note: the maximum size of 's' and the size of 'v' must be equal to the   //
        // number of automata in the original model. It means that in 'v' only the  //
        // values of the automata in 's' are relevant.                              //
        //--------------------------------------------------------------------------//
        bool eval(const func_id f, const aut_st & v, const aut_set & s, rp & c) const;

        //--------------------------------------------------------------------------//
        //                It computes a function limits and average                 //
        //--------------------------------------------------------------------------//
        //   Denote: P   the set of parameters of the function in the entry 'f';    //
        //           S   the set of automata whose states are known;                //
        //           U   the set of automata whose states are unknown;              //
        //           SP  the intersection of P and S;                               //
        //           UP  the intersection of P and U;                               //
        //           R   the set of automata state values for the automata in SP;   //
        //                                                                          //
        //   This function computes the possible values on R, returning the biggest //
        // the smallest evaluations.                                                //
        //                                                                          //
        //   Input parameters: 's' the set S denoted above;                         //
        //                     'v' the set R;                                       //
        //                                                                          //
        //   All the evaluations of the function are computed for all possible      //
        // values of the arguments (P) having the constant value R on SP. The       //
        // biggest value is returned on 'max', the smallest on 'min', and the       //
        // average on 'ave'.                                                        //
        //                                                                          //
        // Note: the maximum size of 's' and the size of 'v' must be equal to the   //
        // number of automata in the original model. It means that in 'v' only the  //
        // values of the automata in 's' are relevant.                              //
        //--------------------------------------------------------------------------//
        void eval(const func_id f, const aut_st & v, const aut_set & s,
                  rp & min, rp & ave, rp & max) const;

        //--------------------------------------------------------------------------//
        //      It returns a nonzero evaluation to the function in entry 'f'        //
        //--------------------------------------------------------------------------//
        //                                                                          //
        //   If the function has only zero evaluations a warning message is issued  //
        // and the function returns '0'. This function is used, for example, to     //
        // test the sign of a term.                                                 //
        //                                                                          //
        // Note: Remember that each entry of a rate matrix must be either always    //
        // positive or always negative (if diagonal element).                       //
        //--------------------------------------------------------------------------//
        rp nonzero_eval(const func_id f) const;

 inline void put_domain(const aut_st & s); // It puts the sizes of the original automata
 inline void get_domain(aut_st & s) const;
                                 // It Fills 's' with the original sizes of the automata
 inline void put_currents(const aut_st & c);
                                // It puts the current group id of the original automata
 inline void get_currents(aut_st & c) const;
                      // It Fills 'c' with the current group id of the original automata

        void show();           // It writes all functions entries on the standard output
        void show(const func_id f);
                           // It writes the function in entry 'f' on the standard output
        void print() const;     // It writes all functions entries on the 'ftb.dbg' file
        void print(const func_id f, ofstream & fout) const;
                            // It prints the function in entry 'f' on 'fout' file stream
        void write() const;                              // Output on 'fout' file stream
        void read(const peps_name file);                 // Input from 'fin' file stream
        void write_jit();                   // Output 'just in time' functions in a file
        int  mem_use() const;                  // Memory space used by 'this' (in bytes)

#ifdef _PEPS_DEBUG_
        void reset_trace(); // It resets the counters for the number of evaluation calls
        void print_trace(); // It prints the counters for the number of evaluation calls
#endif

        //--------------------------------------------------------------------------//
        //                          Static variable                                 //
        //--------------------------------------------------------------------------//
        static FTB ftb;

        //--------------------------------------------------------------------------//
        //                          Data Structures                                 //
        //--------------------------------------------------------------------------//
private: peps_name   file_name;      // the file name of this structure
         int         dimension;      // the maximum number of functions
         int         number;         // the current number of functions
         func_tree * table;          // the table of function trees
         aut_set   * param;          // the table of parameters of each function tree
         aut_st      domain;         // the size of each original automata
         rp       ** reward_table;   // the reward of each state of each original aut.
         aut_st      currents;       // the current group id of each original automata
                                     // (initially automaton 'i' is 'i')
         aut_st      g_st;           // a buffer to global states
                                     // (to evaluate global states of grouped models)
         rp        (**jit_mat)(const int *);
                           // the magical jit matrix to call fast function eval
         void      * handle;         // the handle to load dynamic library 
         
#ifdef _PEPS_DEBUG_
         int       * trace;          // number of evaluations of each function;
#endif

      //--------------------------------------------------------------------------//
      //                           Internal Functions                             //
      //--------------------------------------------------------------------------//
         void alloc();                            // Internal Function of allocation
         void desal();                         // Internal Function of desallocation
         void put_func(ofstream & fout, int f) const;
                      // Internal Function to put 'just in time' functions in a file
};
