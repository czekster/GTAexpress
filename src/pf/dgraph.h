//====================================================================================//
//                                                                                    //
//                            Class Dependency Graph                                  //
//                                                                                    //
//====================================================================================//
//  This File:   dgraph.h                   Language: C++  (xlC and CC)               //
//  Software:    PEPS 3.1                    Advisor: Brigitte.Plateau@imag.fr        //
//  Error Code: 1400                                                                  //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 24/feb/02                        by: Paulo.Fernandes@imag.fr         //
//====================================================================================//
//                                                                                    //
//     A dependency graph is a matrix of booleans where entry 'i,j' is true if and    //
//  only if the automaton 'i' has at least one functional element that depends on the //
//  state of the automaton 'j'. The automata are numbered from 0 to N-1, where N is   //
//  the total number of automata in the model (variable 'dimension').                 //
//                                                                                    //
//  Note: Each tensor product term of the descriptor has a corresponding dependency   //
//  graph. In case of reduction of the descriptor (by automata grouping) the          //
//  dependency graph must follow the reduction, i.e., the number of automata (N) is   //
//  the actual number of groups and not the original number of automata (before       //
//  grouping).                                                                        //
//                                                                                    //
//  Note: Although this class is always used for dependency graphs with all automata  //
//  of the model ('dimension' is equal to N) 'd_graph' objects with a different set   //
//  of reference (a set of contigous automata id's starting from 0 as subset E in     //
//  other automata classes) can be expressed.                                         //
//====================================================================================//

class d_graph
{
public:
 inline d_graph();                                                  // Empty Constructor
 inline d_graph(const int dim);                                     // Sized Constructor
 inline d_graph(const d_graph & g);                                  // Copy Constructor
 inline ~d_graph();                                                        // Destructor

 inline void create();                                            // Empty Intialization
 inline void create(const int dim);                               // Sized Intialization
 inline void copy(const d_graph & g);                    // Copy and Sized Intialization
 inline void erase();                                                     // Destruction

 inline int  size() const;                            // Number of automata in the model

 inline bool is_empty() const;                       // The allocation has not been done
 inline bool is_clique() const;                                 // The graph is a clique
 inline bool has_no_dep() const;                        // The graph has no dependencies

 inline bool has_circuits() const;                             // The graph has circuits

 inline bool param_of(const aut_id a, const aut_id b) const;  // 'a' is parameter of 'b'
 inline bool dep_on(const aut_id a, const aut_id b) const;         // 'a' depends on 'b'
 inline void param_of(const aut_id a, aut_set & s) const;
                     // Fills 's' with the automata that are parameters of automaton 'a'
 inline void dep_on(const aut_id a, aut_set & s) const;
                            // Fills 's' with the automata that depends on automaton 'a'

 inline void put(const aut_id a, const aut_id b);        // Puts that 'a' depends on 'b'
 inline void take(const aut_id a, const aut_id b);
                                                 // Puts that 'a' does not depend on 'b'
 inline void put(const aut_id a, const aut_set & s);
                                                  // Puts that 'a' depends on subset 's'
 inline void take(const aut_id a);    // Puts that 'a' does not depend on any other aut.
 inline void take(const aut_set & s); // Puts that no automata depend on the subset 's'
 inline void take();                             // Builds a graph with no dependencies

        void get_circ(aut_set & c) const;
                                // Gets the automata belonging to the circuits of 'this'

        //--------------------------------------------------------------------------//
        //           Gets a minimal cutset of the circuits of 'this'                //
        //     and changes 'this' according to this cutset to make it acyclic       //
        //--------------------------------------------------------------------------//
        //    A minimal cutset of the circuit of the graph is the smallest subset   //
        // of nodes (automata) with the property that the graph "minus" the output  //
        // arcs of the nodes of the cutset, is acyclic.                             //
        //--------------------------------------------------------------------------//
        void get_cutset(const aut_st & s, aut_set & t);

        //--------------------------------------------------------------------------//
        // Get an order to efficiently handle the tensor product repres. by 'this'  //
        //--------------------------------------------------------------------------//
        //    The best order to handle the automata of a tensor product term is     //
        // based on one restriction and two performance concerns:                   //
        //                                                                          //
        //   1 an automata with functions must be preceeded by the automata that    //
        // are its parameters (restriction presented in the lemma 5.6 of the INRIA  //
        // repport no 2935);                                                        //
        //                                                                          //
        //   2 the automata with functions are more efficiently treated if they are //
        // closer to the begining of the list (performance consideration presented  //
        // in section 4.2 of the INRIA repport 2938);                               //
        //                                                                          //
        //   3 the performance improves when smaller automata are closer to the     //
        // begining of the list (performance consideration presented in section 4.2 //
        // of the INRIA repport 2938).                                              //
        //--------------------------------------------------------------------------//
        // Restriction: the dependency graph 'this' must be acyclic before this     //
        // function is called.                                                      //
        //--------------------------------------------------------------------------//
        void get_order(const aut_st & s, aut_list & l) const;
        //--------------------------------------------------------------------------//
        // Get an order to correctly handle the tensor product represented by 'this'//
        //--------------------------------------------------------------------------//
        //    The best order to handle the automata of a tensor product term is     //
        // based on one restriction and two performance concerns:                   //
        //                                                                          //
        //   1 an automata with functions must preceed the automata that are its    //
        // parameters (decomposition of normal factors property);                   //
        //                                                                          //
        //   2 the automata with functions are more efficiently treated if they are //
        // closer to the begining of the list (performance consideration presented  //
        // in section 4.2 of the INRIA repport 2938);                               //
        //                                                                          //
        //   3 the performance improves when smaller automata are closer to the     //
        // begining of the list (performance consideration presented in section 4.2 //
        // of the INRIA repport 2938).                                              //
        //--------------------------------------------------------------------------//
        // Restriction: the dependency graph 'this' must be acyclic before this     //
        // function is called.                                                      //
        //--------------------------------------------------------------------------//
        void get_D_order(const aut_st & s, aut_list & l) const; 

        void write(ofstream & fout) const;               // Output on 'fout' file stream
        void read(ifstream & fin);                       // Input from 'fin' file stream
        int  mem_use() const;                  // Memory space used by 'this' (in bytes)

        //--------------------------------------------------------------------------//
        //                          Data Structures                                 //
        //--------------------------------------------------------------------------//
private: int     dimension;      // number of automata in the model (N)
         bool ** graph;          // graph information (ij means that i depends on j)
};

