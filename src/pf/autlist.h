//====================================================================================//
//                                                                                    //
//                              Class Automata List                                   //
//                                                                                    //
//====================================================================================//
//  This File:   autlist.h                  Language: C++  (xlC and CC)               //
//  Software:    PEPS 3.1                    Advisor: Brigitte.Plateau@imag.fr        //
//  Error Codes: 1200                                                                 //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change:  7/feb/01                        by: Anne.Benoit@imag.fr             //
//====================================================================================//
//                                                                                    //
//     An automata list is an array of automata id's where entry 'p' is the id of the //
//  automaton in the p-th position of the list. Automata id's are numbers from 0 to   //
//  N-1 (N is the total number of automata of the model).                             //
//                                                                                    //
//  Note: This class is used for lists of automata id's in a set of reference E. This //
// set of reference E can be the total set of automata of the model or a particular   //
// subset of the total set of automata. The automata range is 0..'dimension'-1, i.e., //
// a subset of contiguous automata, starting by 0. The variable 'dimension' is the    //
// number of elements in E.                                                           //
//                                                                                    //
//  Restriction: The automata id's in a list cannot be replicated, i.e., an automaton //
// may appear only once in the list. Note that, due to this restriction the total     //
// number of automata in E is also the maximum number of automata in the list. The    //
// object automata list has a field 'dimension' for this maximum number and a field   //
// 'lsize' for its actual number of elements. For example, if E = { 0, 1, 2, 3, 4 }   //
// and 'this' = { 4, 2 }, the fields 'dimension' = 5 and the 'lsize' = 2. Therefore,  //
// no automaton id can be greater or equal than 'dimension'.                          //
//====================================================================================//

enum order_types { Normal , Reverse };                        // pre-defined list orders

class aut_list
{
public:
 inline aut_list();                                                 // Empty Constructor
 inline aut_list(const int dim);                                    // Sized Constructor
 inline aut_list(const aut_list & l);                                // Copy Constructor
 inline ~aut_list();                                                       // Destructor

 inline void create();                                            // Empty Intialization
 inline void create(const int dim);                               // Sized Intialization
 inline void copy(const aut_list & l);                   // Copy and Sized Intialization
 inline void erase();                                                     // Destruction

 inline int    size() const;                            // Total number of automata in E
 inline bool   is_empty() const;                              // No automata in the list
 inline bool   is_full() const;                          // All automata are in the list
 inline bool   operator == (const aut_list & l) const;         // 'this' is equal to 'l'
 inline bool   is_original() const;        // All automata are in the list from 0 to N-1
 inline bool   is_optimal(const d_graph & g) const;
                             // All automata are in the list and the functional automata
                                 // are preeceed only by its parameters (defined in 'g')

 inline int    has() const;                    // Current number of automata in the list
 inline int    has_not() const;   // Current number of automata in E and not in the list
 inline bool   has(const aut_id a) const;                // Automaton 'a' is in the list
 inline bool   has_not(const aut_id a) const;        // Automaton 'a' is not in the list
 inline aut_id biggest() const;                  // The biggest automaton id in the list

 inline aut_id aut(const int p) const;                 // The p-th automaton of the list
 inline int pos(const aut_id a) const;      // The position of automaton 'a' in the list

        void   before(const aut_id a, aut_set & s) const;
                                  // It fills 's' with the automata before automaton 'a'
        void   after(const aut_id a, aut_set & s) const;
                                   // It fills 's' with the automata after automaton 'a'

 inline void take_out();                        // It removes all automata from the list
 inline int get_cut();                   //It returns the preferencial cut value (split)
 inline void invert(int *f, int e, aut_list & o); //It inverts the list according to f[e]

        void sort(const aut_st & s);
                           // It sorts the list according to the increasing sizes in 's'
        void exchange(const int p, const int q);
                             // It exchanges the p-th and the q-th automaton of the list

        //--------------------------------------------------------------------------//
        //        The next three functions put all automata of E in the list        //
        //--------------------------------------------------------------------------//
        // Note: these functions erase previous contents of the list. However,      //
        // 'this' must already have been allocated ('dimension' is unchanged).      //
        //--------------------------------------------------------------------------//
        void put();                        // It puts the automata in the normal order
        void put(const order_types t);        // It puts the automata in the 't' order
        void put(const aut_st & s);
                      // It puts the automata according to the increasing sizes in 's'

        //--------------------------------------------------------------------------//
        //  The next three functions put the automata of the subset 's' in the list //
        //--------------------------------------------------------------------------//
        // Note: these functions erase previous contents of the list and alllocates //
        // 'this' according to the size of 's'.                                     //
        //--------------------------------------------------------------------------//
        void put(const aut_set & s);       // It puts the automata in the normal order
        void put(const aut_set & s, const order_types t);
                                             // It puts the automata in the 't' order
        void put(const aut_set & s, const aut_st & o);
                     // It puts the automata according to the increasing sizes in 's'

        //--------------------------------------------------------------------------//
        //        The next three functions insert automaton 'a' in the list         //
        //--------------------------------------------------------------------------//
        // Note: these functions replace previous contents of the list.             //
        //--------------------------------------------------------------------------//
        void ins_head(const aut_id a);// It puts the automaton at the head of the list
        void ins_tail(const aut_id a);// It puts the automaton at the tail of the list
        void insert(const int p, const aut_id a);
                             // It puts the automaton at the p-th position of the list

        //--------------------------------------------------------------------------//
        //      The next three functions insert a subset of automata in the list    //
        //--------------------------------------------------------------------------//
        // Note: these functions replace previous contents of the list.             //
        //--------------------------------------------------------------------------//
        void fill_head(); // It puts all automata of E not yet in the list at the head
        void fill_tail(); // It puts all automata of E not yet in the list at the tail
        void fill(const int p, const aut_set s);
                                     // It puts the automata of the subset 's' not yet
                                        // in the list starting from the p-th position

        //--------------------------------------------------------------------------//
        //     The next three functions move the automaton in the p-th position     //
        //--------------------------------------------------------------------------//
        // Note: these functions replace previous contents of the list.             //
        //--------------------------------------------------------------------------//
        void mov_head(const int p);  // It moves the automaton to the head of the list
        void mov_tail(const int p);  // It moves the automaton to the tail of the list
        void mov_cnst(const term_typ t, const int cut, const int e, const aut_list & l, int & r);
         // It moves the constant matrices in the shuffle side to the head of the list
        void mov_idnt(const term_typ t, const int c, const int e, const aut_list & l, int & r); 
                                     // It moves the identities matrices to the end and 
                                              //marks the new value for the cut (split)
        void mov_f_idnt(const term_typ t, const int c, const int e, const aut_list & l, int & r);
                                     // It moves the function and identities to the end and
                                              //marks the new value for the cut (split)
        void move(const int p, const int q);
                        // It moves the automaton to the q-th position of the list and
                                            // shifts the automata between 'p' and 'q'

        void write(ofstream & fout) const;               // Output on 'fout' file stream
        void read(ifstream & fin);                       // Input from 'fin' file stream
        int  mem_use() const;                  // Memory space used by 'this' (in bytes)

        //--------------------------------------------------------------------------//
        //                          Data Structures                                 //
        //--------------------------------------------------------------------------//
private: int      dimension;       // Total number of automata in the set of reference E
                                   // (maximum list size)
         int      lsize;           // current list size
         aut_id * list;            // array of automata id's (list information)
         int      cut;             // the preferencial cut value
};

