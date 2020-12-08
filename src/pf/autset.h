//====================================================================================//
//                                                                                    //
//                              Class Automata Subset                                 //
//                                                                                    //
//====================================================================================//
//  This File:   autset.h                   Language: C++  (xlC and CC)               //
//  Software:    PEPS 3.1                    Advisor: Brigitte.Plateau@imag.fr        //
//  Error Code:  1100                                                                 //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 14/feb/00                        by: Paulo.Fernandes@imag.fr         //
//====================================================================================//
//                                                                                    //
//     An automata subset is an array of booleans where entry 'i' is true if and only //
//  if the automaton 'i' belongs to the subset and false otherwise. The automata are  //
//  numbered from 0 to N-1.                                                           //
//                                                                                    //
//  Note: This class is used for subsets of a set of reference E. This set of         //
//  reference E can be the total set of automata of the model or a particular subset  //
//  of the total set of automata. The automata range is 0..'dimension'-1, i.e., a     //
//  subset of contiguous automata, starting by 0. The variable 'dimension' contains   //
//  the number of elements in E.                                                      //
//====================================================================================//

class aut_set
{
public:
 inline aut_set();                                                  // Empty Constructor
 inline aut_set(const int dim);                                     // Sized Constructor
 inline aut_set(const aut_set & s);                                  // Copy Constructor
 inline ~aut_set();                                                        // Destructor

 inline void create();                                            // Empty Intialization
 inline void create(const int dim);                               // Sized Intialization
 inline void copy(const aut_set & s);                    // Copy and Sized Intialization
 inline void complement(const aut_set & s);
                             // Copy and Sized Intialization of the complementary of 's'
 inline void copy(const aut_list & l);
                          // Copy and Sized Intialization of the members of the list 'l'
 inline void erase();                                                     // Destruction

 inline int  size() const;                              // Total number of automata in E
 inline int  inside() const;                              // Elements in E that are true
 inline bool is_empty() const;                             // 'this' is the empty subset
 inline bool is_full() const;                                    // 'this' is equal to E
 inline int  has() const;                                       // Cardinality of 'this'
 inline int  has_not() const;                    // Number of automata not in the subset
 inline bool has(const aut_id a) const;                // Automaton 'a' is in the subset
 inline bool has_not(const aut_id a) const;        // Automaton 'a' is not in the subset

 inline void put_in();                            // Includes all automata in the subset
 inline void take_out();                                     // Sets the subset to empty
 inline void put_in(const aut_id a);             // Includes automaton 'a' in the subset
 inline void take_out(const aut_id a);          // Removes automaton 'a' from the subset
 inline void put_in(const aut_st & s, const int x);
                            // Includes all automata of 's' with value 'v' in the subset

 inline void union_of(const aut_set & a, const aut_set & b);
                                                         // Union of subsets 'a' and 'b'
 inline void intersection(const aut_set & a, const aut_set & b);
                                                  // Intersection of subsets 'a' and 'b'
 inline void plus(const aut_set & s);                 // Union of subsets 'this' and 's'
 inline void minus(const aut_set & s);                 // Removes subset 's' from 'this'

        void next(const aut_list & l);
        // Changes the subset 'this' by the following subset in a list (see explanation)

        void write(ofstream & fout) const;               // Output on 'fout' file stream
        void read(ifstream & fin);                       // Input from 'fin' file stream
        int  mem_use() const;                  // Memory space used by 'this' (in bytes)

        //--------------------------------------------------------------------------//
        //                          Data Structures                                 //
        //--------------------------------------------------------------------------//
private: int    dimension;      // Total number of automata in the set of reference E
                                // (maximum cardinality of the subset)
         bool * set;            // array of booleans characterizing the subset
};                              // (true is in - false is out)

//   void next(const aut_list & l);                                       -- Explanation
   //------------------------------------------------------------------------------//
   //         Changes the subset 'this' by the following subset in a list          //
   //-------------------------------------------------------------+----------------//
   // The subsets of E are ordered. The order is defined by two   |   Example of   //
   // criteria. The first criterion is the cardinality of the     |  ordered list  //
   // subsets. For the second criterion, a subset is represented  |                //
   // by the list of its elements ranked according to the         | {}             //
   // automata list 'l'; For a given cardinality, the subsets are | { 2 }          //
   // ranked according to the lexical order. The function changes | { 1 }          //
   // 'this' by the following subset in the ordered list.         | { 3 }          //
   //                                                             | { 0 }          //
   // For example, with E = { 0, 1, 2, 3 } and l = { 2, 1, 3, 0 } | { 2, 1 }       //
   // the order list is represented in the column     ==>         | { 2, 3 }       //
   // If 'this' is { 1, 3 } the function changes it to { 1, 0 }.  | { 2, 0 }       //
   //                                                             | { 1, 3 }       //
   // Restriction: the elements of the automata list 'l' MUST be  | { 1, 0 }       //
   // included in E, but may be a strict subset of E, denoted by  | { 3, 0 }       //
   // El. In this case the order is computed on the subsets of El.| { 2, 1, 3 }    //
   // Then the subset 'this' MUST be included in El.              | { 2, 1, 0 }    //
   //                                                             | { 2, 3, 0 }    //
   // Remark: If 'this' is the last subset of the ordered list,   | { 1, 3, 0 }    //
   // function returns 'this' equal to the empty subset.          | { 2, 1, 3, 0 } //
   //-------------------------------------------------------------+----------------//
