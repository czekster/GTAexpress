//====================================================================================//
//                                                                                    //
//                            Class Dependency Graph                                  //
//                                                                                    //
//====================================================================================//
//  This File:   dgraph.C                   Language: C++  (xlC and CC)               //
//  Software:    PEPS 3.1                    Advisor: Brigitte.Plateau@imag.fr        //
//  Error Code: 1400                                                                  //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 28/feb/02                        by: Paulo.Fernandes@imag.fr         //
//====================================================================================//
#include "peps.h"

     //--------------------------------------------------------------------------//
     //             Gets the automata belonging to the circuits of 'this'        //
     //--------------------------------------------------------------------------//
     //    Algorithm: Removes (logically) all nodes that do not have output or   //
     // do not have input arcs. Removing means that the node and its input and   //
     // output arcs are removed. Repeat this process until the graph is          //
     // unchanged. The remaining nodes belongs to circuits. A subset object (the //
     // output parameter 'c') is the subset of nodes that are not removed.       //
     //--------------------------------------------------------------------------//
void d_graph::get_circ(aut_set & c) const
{
 bool    changed;        // the subset 'c' has changed
 bool    no_out, no_in;  // a given node ('i') has no output arcs (resp. no input)
 aut_id  i, j;           // a pointer to the node being tested ('i') and its arc ('j')

 c.create(dimension);               // creates the subset 'c' object (output parameter)
 c.put_in();                        // puts all nodes in the subset 'c'
 do {                               // repeat until the subset 'c' is unchanged
   changed = false;
   for (i=0; i<dimension; i++)         // for all nodes in the subset 'c'
     if (c.has(i)) {
       no_out = true;
       no_in  = true;
       for (j=0; j<dimension; j++)            // verifies if there are output and
         if (c.has(j) && (i!=j)) {            // input arcs to nodes that still are
           if (graph[i][j]) no_out = false;   // in the subset 'c'
           if (graph[j][i]) no_in  = false;   // (except for the node 'i' itself)
         }
       if (no_out || no_in) {                 // if the node 'i' does not have such arcs
         c.take_out(i);                           // removes it from the subset 'c'
         changed = true;                          // the subset 'c' has changed
       }
     }
 } while (changed);                 // end of the 'repeat until the subset is unchanged'
}

     //--------------------------------------------------------------------------//
     //              Gets a minimal cutset of the circuits of 'this'             //
     //      and changes 'this' according to this cutset to make it acyclic      //
     //--------------------------------------------------------------------------//
     //    A minimal cutset of the circuit of the graph is the smallest subset   //
     // of nodes (automata) with the property that the graph "minus" the output  //
     // arcs of the nodes of the cutset, is acyclic.                             //
     //--------------------------------------------------------------------------//
     // Algorithm Principle: We assume that the number of automata is relatively //
     // small (less than 20). Thus a very simple algorithm is implemented here   //
     // using a naive and exhaustive search of the cutset: we test iteratively   //
     // if a cutset with one element can be found, then with two and so on.      //
     // Moreover, due to considerations related to tensor decomposition (see     //
     // lemma 5.8 of INRIA report 2935) the best decomposition is obtained with  //
     // a cutset such that, the product state space size of the automata in the  //
     // cutset is minimum.                                                       //
     //--------------------------------------------------------------------------//
     // Algorithm: - computes the nodes in the circuits and sort them according  //
     //            to their increasing sizes ('aut_list' object 'suspects');     //
     //            - initializes the possible cutset ('aut_set' object 't') as   //
     //            an empty subset;                                              //
     //            - initializes the variable 'test' with the graph ('this');    //
     //            - main loop:                                                  //
     //               . tests if 'test' has circuits:                            //
     //               . if it has circuits:                                      //
     //                   - 't' is not a cutset then tries another cutset ('t')  //
     //                   using the 'next' function (of the class 'aut_set') on  //
     //                   'test' with the list 'suspects' as ordering parameter; //
     //                    - make 'test' equal to 'this' minus the 't' output    //
     //                   arcs, i.e., sets the columns corresponding to the      //
     //                   automata in the cutset 't' to false;                   //
     //               . if it has no circuits:                                   //
     //                   - 't' is the "good" cutset;                            //
     //                   - 'test' is the new dependency graph, i.e., 'this'     //
     //                   minus the dependencies of the nodes of 't';            // 
     //--------------------------------------------------------------------------//
void d_graph::get_cutset(const aut_st & s, aut_set & t)
{
#ifdef _PEPS_DEBUG_
  if (s.size() != dimension)
    Programming_Error("bad get_cutset call", 1413)
#endif
 bool     not_found = true;  // the cutset was not found yet
 aut_set  circuit;           // the subset of automata belonging to circuits of 'this'
 aut_list suspects;          // the ordered version of 'circuit'
 d_graph  test;              // a copy of 'this' to be changed

 get_circ(circuit);          // finds the subset of automata belonging to circuits
 suspects.create(dimension); // allocates the list of 'suspects'
 suspects.put(circuit);      // puts all nodes of 'circuit' in 'suspects'
 suspects.sort(s);           // sorts the 'suspects' list according to the aut. sizes
 test.copy(*this);           // creates the copy of 'this'
 t.create(dimension);        // allocates the cutset (output parameter)
 t.take_out();               // make 't' an empty subset
 do {                        // main loop:
   if (test.has_circuits()) {   // if 'test' has circuits:
     t.next(suspects);             // tries another transversal ('t');
     test.copy(*this);             // makes 'test' equal to 'this'
     test.take(t);                 // sets the columns corresponding to 't'
   }                               // automata to false ('test' = 'this' minus 't')
   else {                       // if 'test' has no circuits:
     not_found = false;            // the cutset ('t') is found
     copy(test);                   // 'test' is the new dependency graph
   }
 } while (not_found);        // end of the 'main loop'
}

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
     //   3 the performance improves when the smaller automata are closer to the //
     // begining of the list (performance consideration presented in section 4.2 //
     // of the INRIA repport 2938).                                              //
     //--------------------------------------------------------------------------//
     // Restriction: the dependency graph 'this' must be acyclic before this     //
     // function is called.                                                      //
     //--------------------------------------------------------------------------//
     // Algorithm Principle: The automata are ranked into a list 'l' (output     //
     // parameter) in order to satisfy the three criteria above.                 //
     //                                                                          //
     // We define two properties for an automaton:                               //
     //                                                                          //
     //     - An automaton is a preferencial candidate to inclusion in 'l' when  //
     // it depends only on automata already in the 'l' (restriction 1) AND it    //
     // has functions (consideration 2);                                         //
     //                                                                          //
     //     -  An automaton is a simple candidate to inclusion in 'l' when it    //
     // depends only on automata already in 'l' (restriction 1) AND it is a      //
     // parameter for other automata (consideration 2);                          //
     //                                                                          //
     //   The ranking puts first the preferencial candidates (if any) then the   //
     // simple candidates. In case there are more than one candidates of the     //
     // same type, the automata with the smallest size (input parameter 's') is  //
     // chosen (consideration 3).                                                //
     //--------------------------------------------------------------------------//
     // Algorithm: - initializes 'to_put' with all automata (graph nodes);       //
     //            - initializes order 'l' (output parameter) as an empty list;  //
     //            - main loop:                                                  //
     //                . finds all preferencial and simple candidates to include //
     //                in 'l';                                                   //
     //                . includes in 'l' the smallest preferencial candidate or  //
     //                if not the smallest simple candidate or if not rest of    //
     //                the not included automata ('to_put');                     //
     //--------------------------------------------------------------------------//
     // Example: seven automata ( 0, 1, 2, 3, 4, 5, 6 ) where:                   //
     //                                                                          //
     //  - automaton 0 depends on automaton 2;           size = 6 states;        //
     //  - automaton 1 depends on automata 0 and 2;      size = 2 states;        //
     //  - automaton 2 is constant (no functions);       size = 3 states;        //
     //  - automaton 3 depends on automaton 4;           size = 4 states;        //
     //  - automaton 4 is also constant;                 size = 5 states;        //
     //  - automaton 5 is also constant;                 size = 4 states;        //
     //  - automaton 6 is also constant;                 size = 2 states;        //
     //                                                                          //
     //      +---------------------+--------------+---------+-----------+        //
     //      | main loop iteration | preferencial |  simple | includes  |        //
     //      +---------------------+--------------+---------+-----------+        //
     //      |       k = 0         |       -      | 2 and 4 |     2     |        //
     //      |       k = 1         |       0      |    4    |     0     |        //
     //      |       k = 2         |       1      |    4    |     1     |        //
     //      |       k = 3         |       -      |    4    |     4     |        //
     //      |       k = 4         |       3      |    -    |     3     |        //
     //      |       k = 5         |       -      |    -    |  5 and 6  |        //
     //      +---------------------+--------------+---------+-----------+        //
     //                                                                          //
     //    Resulting 'l' = { 2, 0, 1, 4, 3, 5, 6 }.                              //
     //--------------------------------------------------------------------------//
void d_graph::get_order(const aut_st & s, aut_list & l) const
{
#ifdef _PEPS_DEBUG_
  if (s.size() != dimension)
    Programming_Error("wrong number of automata", 1414)
  if (has_circuits())
    Programming_Error("getting order with cirduits", 1415)
#endif
 aut_set to_put;        // automata not yet in the list 'l'
 bool    param,         // the automaton 'i' is parameter of automton(a) not yet in 'l'
         no_dep,        // the automaton 'i' depends only on automata already in 'l'
         funct;         // the automaton 'i' has functions
 aut_id  preferencial,  // the automaton is the preferencial candidate to be included
                        // in 'l' at the 'k'-th iteration of the main loop
         candidate,     // the automaton is the simple candidate to be included in 'l'
                        // at the 'k'-th iteration of the main loop
         i, j, k;       // counters for the automata

 to_put.create(dimension);        // initializes 'to_put' with all automata
 to_put.put_in();
 l.create(dimension);             // initializes order 'l' as an empty list
 l.take_out();
 for (k=0; k<dimension; k++) {       // main loop
   preferencial = no_aut;
   candidate    = no_aut;
   for (i=0; i<dimension; i++) {        // for each automaton 'i'
     param  = false;
     no_dep = true;
     funct  = false;
     if (to_put.has(i)) {                  // not yet included
       for (j=0; j<dimension; j++) {
         if (to_put.has(j) && (i != j)) {     // verifies if automaton 'i'
           no_dep = no_dep && (!(graph[i][j]));  // depends on other non included aut.
           param  = param  || (graph[j][i]);     // is parameter of other non inc. aut.
         }
         funct = funct || (graph[i][j]);      // verifies if it has functions
       }
     }
     if (no_dep && funct)                     // if 'i' can be preferencial candidate
       if (preferencial == no_aut)               // if it is the first preferencial
         preferencial = i;
       else                                      // else
         if (s.st(i) < s.st(preferencial))          // test their sizes
           preferencial = i;
     if (param && no_dep)                     // if 'i' can be simple candidate
       if (candidate == no_aut)                  // if it is the first candidate
         candidate = i;
       else                                      // else
         if (s.st(candidate) > s.st(i))             // test their sizes
           candidate = i;
   }                                    // end of 'for each automaton 'i' loop
   if (preferencial != no_aut) {
     l.ins_tail(preferencial);          // if there is a preferencial candidate
     to_put.take_out(preferencial);     //   includes this automaton ('preferencial')
   }
   else if (candidate != no_aut) {
          l.ins_tail(candidate);        // else if there a simple candidate
          to_put.take_out(candidate);        // includes this automaton ('candidate')
        }
        else {                               // otherwise 
          l.fill_tail();                     //   includes all not yet included automata
          break;                             //   exit of the 'main loop' (all included)
        }
 }                                   // end of the 'main loop'
}

     //--------------------------------------------------------------------------//
     // Get an order to correctly handle the tensor product represented by 'this'//
     //--------------------------------------------------------------------------//
     //    The decomposition order to handle the automata of a tensor product    //
     // term is defined by one single rule:                                      //
     //                                                                          //
     //   - an automata with functions must preceed the automata that are its    //
     // parameters (decomposition in normal factor property);                    //
     //--------------------------------------------------------------------------//
     // Restriction: the dependency graph 'this' must be acyclic before this     //
     // function is called.                                                      //
     //--------------------------------------------------------------------------//
     // Algorithm Principle: It puts the automata from the last (completly       //
     // independent) to the first (mostly dependent).                            //
     //--------------------------------------------------------------------------//
     // Algorithm: - initializes 'to_put' with all automata (graph nodes);       //
     //            - initializes order 'l' (output parameter) as an empty list;  //
     //            - main loop:                                                  //
     //                . finds an automaton idepedent from the automata not yet  //
     // included (in the set 'to_put') and includes it in the head of the list;  //
     //--------------------------------------------------------------------------//
void d_graph::get_D_order(const aut_st & s, aut_list & l) const
{
#ifdef _PEPS_DEBUG_
  if (s.size() != dimension)
    Programming_Error("wrong number of automata", 1416)
  if (has_circuits())
    Programming_Error("getting D order with cirduits", 1417)
#endif
 aut_set to_put;        // automata not yet in the list 'l'
 bool    no_dep;        // the automaton 'i' do not depends of automata in 'to_put'
 aut_id  i, j, k;       // counters for the automata

 to_put.create(dimension);        // initializes 'to_put' with all automata
 to_put.put_in();
 l.create(dimension);             // initializes order 'l' as an empty list
 l.take_out();
 for (k=0; k<dimension; k++) {       // main loop
   for (i=0; i<dimension; i++) {        // for each automaton 'i'
     if (to_put.has(i)) {                  // not yet included (still in 'to_put')
       no_dep = true;
       for (j=0; j<dimension; j++) {
         if (to_put.has(j) && (i != j))       // verifies if automaton 'i'
           no_dep = no_dep && (!(graph[i][j]));  // do not depends on other 'to_put' aut.
       }
     }
     if (no_dep) {                     // if 'i' has no dependencies on 'to_put'
       l.ins_head(i);                      // insert automaton 'i' in the head of the list
       to_put.take_out(i);                 // remove automaton 'i' from the to_put
       i = dimension;                      // breaks the "for each automaton 'i' loop"
       no_dep = false;
     }
   }                                    // end of "for each automaton 'i' loop"
 }                                   // end of the "main loop"
}

     //--------------------------------------------------------------------------//
     //                      Output on 'fout' file stream                        //
     //--------------------------------------------------------------------------//
void d_graph::write(ofstream & fout) const
{
 aut_id i, j;

 fout << dimension << "\n";
 for (i=0; i<dimension; i++) {
   for (j=0; j<dimension; j++)
     fout << graph[i][j] << " ";
   fout << "\n";
 }
}
     //--------------------------------------------------------------------------//
     //                      Input from 'fin' file stream                        //
     //--------------------------------------------------------------------------//
void d_graph::read(ifstream & fin)
{
 aut_id i, j;

 fin >> i;
 create(i);
 for (i=0; i<dimension; i++)
   for (j=0; j<dimension; j++)
     fin >> graph[i][j];
}
     //--------------------------------------------------------------------------//
     //              Memory space used by 'this' (in memory units)               //
     //--------------------------------------------------------------------------//
int d_graph::mem_use() const
{
 int answer = 4;
 answer += dimension * dimension * 4;
 return(answer);
}
