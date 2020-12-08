//====================================================================================//
//                                                                                    //
//                              Class Automata States                                 //
//                                                                                    //
//====================================================================================//
//  This File:   autst.h                    Language: C++  (xlC and CC)               //
//  Software:    PEPS 2002                   Advisor: Brigitte.Plateau@imag.fr        //
//  Error Codes: 1300                                                                 //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 07/nov/02                        by: paulof@inf.pucrs.br             //
//====================================================================================//
//                                                                                    //
//     An automata state is a vector of information concerning each automaton of a    //
//  set of reference E. The entry 'i' of the vector contains the information relative //
//  to automaton 'i'. The possible use of this information (an integer) is:           //
//        - size of the automaton 'i';                                                //
//        - individual state of automaton 'i';                                        //
//        - any other integer coded information (e.g. membership of a group);         //
//                                                                                    //
//  Note: This set of reference E can be the total set of automata of the model or a  //
//  particular subset of the total set of automata. The automata range is             //
//  0..'dimension'-1, i.e., a subset of contiguous automata, starting by 0. The       //
//  variable 'dimension' contains the number of elements in E.                        //
//                                                                                    //
//  Note: If the set of reference E is the total set of automata of the model and the //
//  state of each individual automaton is represented, the variable defines a global  //
//  state of the SAN model.                                                           //
//====================================================================================//

class aut_st
{
public:
 inline aut_st();                                                   // Empty Constructor
 inline aut_st(const int dim);                                      // Sized Constructor
 inline aut_st(const aut_st & s);                                    // Copy Constructor
 inline ~aut_st();                                                         // Destructor

 inline void create();                                           // Empty Initialization
 inline void create(const int dim);                              // Sized Initialization
 inline void copy(const aut_st & s);                    // Copy and Sized Initialization
 inline void erase();                                                     // Destruction

 inline int  size() const;                              // Total number of automata in E
 inline bool is_empty() const;         // Total number of automata in E is equal to zero

        void write(ofstream & fout) const;               // Output on 'fout' file stream
        void read(ifstream & fin);                       // Input from 'fin' file stream
        int  mem_use() const;                  // Memory space used by 'this' (in bytes)

//----- Functions used only for automata sizes

 inline int  sz(const aut_id a) const;             // Gets the size of the automaton 'a'
 inline void put(const aut_id a, const int s);     // Puts the size of the automaton 'a'

        int  state_space() const;                  // Calculates the product state space
        int  state_space(const aut_set & s) const;
                           // Calculates the product state space for the automata in 's'

//----- Functions used only for automata states

 inline int * st_addr() const;                // Returns the pointer to the states array
 inline st_id st(const aut_id a) const;                // The state of the automaton 'a'

        //------------------------------------------------------------+-------------//
        //        The lexical order is used to rank the states        | 0, 0, 0, 0  //
        //                                                            | 0, 0, 0, 1  //
        //   Each automata state space is coded from 0 to its size-1. | 0, 0, 1, 0  //
        //  These sizes are stored in the input parameter 's' (type   | 0, 0, 1, 1  //
        //  'aut_st') of all 'next' and 'rank' functions).            | 0, 1, 0, 0  //
        //                                                            | 0, 1, 0, 1  //
        //  - The 'reset' functions assign to 'this' the first state  | 0, 1, 1, 0  //
        //  of the lexical order, i.e., 'this' [0, ... , 0];          | 0, 1, 1, 1  //
        //  - The 'next' functions replace the state in 'this' by its | 1, 0, 0, 0  //
        //  successor (if 'this' is the last state the function       | 1, 0, 0, 1  //
        //  returns the first state);                                 | 1, 0, 1, 0  //
        //  - The 'rank' functions give the rank of 'this' in the     | 1, 0, 1, 1  //
        //  lexical order (0 is the first state);                     | 1, 1, 0, 0  //
        //  - The 'st_rank' function puts in 'this' the lexical order | 1, 1, 0, 1  //
        //  corresponding to the rank 'r';                            | 1, 1, 1, 0  //
        //                                                            | 1, 1, 1, 1  //
        //    For example, if we have four automata of size 2, the    L_____________//
        //  lexical order is presented in the column: ---------->>                  //
        //                                                                          //
        //   - The 'next_aut' functions replace the state in 'this' by the state    //
        //   where the automaton 'a' state is incremented by 1. It returns true if  //
        //   the increment is valid, and false elsewhere.                           //
        //--------------------------------------------------------------------------//

 inline void reset();                                            // Resets all automata
 inline void next(const aut_st & s);                // Replaces 'this' by its successor
 inline void next_lex(const aut_st & s);                // Replaces 'this' by its successor as lexico order
 inline void reset(const aut_id a, const l_r lr);
           // Resets partially the automata before (left) or after (right) automaton 'a'
 inline void next(const aut_st & s, const aut_id a, const l_r lr); 
                  // Replaces the left (or right) side of 'a' in 'this' by its successor
        int  rank(const aut_st & s) const;               // The rank of the global state
        void st_rank(const int r, const aut_st & s);// Sets 'this' according to rank 'r'

 inline bool next_aut(const aut_st & s, int a);

        //------------------------------------------------------------+-------------//
        //     Other lexical orders may be used to rank the states    | 0, 0, 0, 0  //
        //        (defined using a list 'l' of automata in E)         | 0, 0, 1, 0  //
        //                                                            | 1, 0, 0, 0  //
        //   In the following five functions, the positions of the    | 1, 0, 1, 0  //
        // automata are given by the list 'l' ('l' must have the same | 0, 1, 0, 0  //
        // dimension as 'this'). The lexical order is taken according | 0, 1, 1, 0  //
        // to the list 'l'.                                           | 1  1  0  0  //
        //                                                            | 1, 1, 1, 0  //
        // For the example above, if 'l' is { 3, 1, 0, 2 }, the new   | 0, 0, 0, 1  //
        // lexical order is represented in the column: ---------->>   | 0, 0, 1, 1  //
        //                                                            | 1, 0, 0, 1  //
        //   If 'l' is strictly included in the set of reference E,   | 1, 0, 1, 1  //
        // 'l' behaves as a mask on E, which means that the function  | 0, 1, 0, 1  //
        // works only on the automata in 'l'. The lexical order is    | 0, 1, 1, 1  //
        // now defined on a subset of states where the automata in    | 1  1  0  1  //
        // E-'l' are constant.                                        | 1, 1, 1, 1  //
        //------------------------------------------------------------+-------------//

 inline void reset(const aut_list & l);           // Resets the automata of the list 'l'
 inline void next(const aut_list & l, const aut_st & s);
               // Replaces 'this' by its successor in the lexical order defined with 'l'
 inline void reset(const aut_list & l, const int a, const l_r lr);
             // Resets the automata before (left) or after (right) the 'a'-th aut. in 'l'
 inline void next(const aut_list & l, const aut_st & s, const int a, const l_r lr);
         // Replaces 'this' by its successor in a lexical order defined with the sublist
                    // of 'l' before (left) or after (right) the 'a'-th automaton in 'l'
        int rank(const aut_list & l, const aut_st & s) const;
                             // The rank of the state according to the new lexical order

        //--------------------------------------------------------------------------//
        //     It replaces the state of 'this' by its successor in the lexical      //
        //   order defined using a list 'l' of automata in E an recalculates the    //
        //     rank 'r' in the lexical order defined by the 'jumps' object 'j'      //
        //--------------------------------------------------------------------------//
 inline void next(const aut_list & l, const aut_st & s, const jumps & j, int & r);

        //--------------------------------------------------------------------------//
        // It verifies if a group of global states has at least one reacheable state//
        //--------------------------------------------------------------------------//
        //   Given a state vector x of automata in E and a subset s of E, denote    //
        // St(x,s) the set of states y such that y(i) = x(i) for all automata i in  //
        // E-s. The function returns false if St(x,s) is included in the            //
        // non-reacheable state space and true otherwise.                           //
        //                                                                          //
        //   In the 'verify' function, the state vector x is coded in 'this' and    //
        // the s subset is the input parameter 's'. The reacheable state space is   //
        // the static variable 'RRS::rss' and the size of the automata is taken     //
        // from the static variable 'FTB::ftb' (y(i) domain when i belongs to s).   //
        //                                                                          //
        //--------------------------------------------------------------------------//
        bool verify(const aut_set & s);

        //   Functions usefull for any automata integer coded information           //

 inline void stamp(const aut_id a, const int v);  // Puts the value 'v' to automaton 'a'
 inline void stamp();                                   // Initialize 'info[a]' with 'a'

 inline int  val(const aut_id a) const;               // Gets the value of automaton 'a'
 inline int  biggest() const;                        // Gets the biggest value in 'this'
 inline int  smallest() const;                      // Gets the smallest value in 'this'

 inline bool has(const int v) const;                       // The value 'v' is in 'this'
 inline int  count_rep(const int v, aut_id f, aut_id l) const;
                                                      // The times that 'v' is in 'this'
        int  count_groups() const;    // Counts the number of different values in 'this'
        bool contigous() const;        // Verifies if the values in 'this' are contigous
        void sort(aut_id f, aut_id l);                    // Sort the automata by states
        void create_map(const int i, const int sz); 
        bool is_sorted(aut_id f, aut_id l);
        
        //--------------------------------------------------------------------------//
        //                          Data Structures                                 //
        //--------------------------------------------------------------------------//
public: int   dimension;        // Total number of automata in E
         int * info;             // automata information (sizes, states or integer info)
       
         //--------------------------------------------------------------------------//
         //                           Internal Functions                             //
         //--------------------------------------------------------------------------//
         void quick_sort(int i, int s);    
};


