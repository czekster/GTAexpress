//====================================================================================//
//                                                                                    //
//                        Reacheable State Space Structure                            //
//                                                                                    //
//====================================================================================//
//  This File:   rss.C                      Language: C++  (xlC and CC)               //
//  Software:    PEPS 2002                   Advisor: Brigitte.Plateau@imag.fr        //
//  Error Codes: 2200                                                                 //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 13/feb/03                        by: Anne.Benoit@imag.fr             //
//====================================================================================//
#include "peps.h"

     //--------------------------------------------------------------------------//
     //                          Static variable                                 //
     //--------------------------------------------------------------------------//
RSS RSS::rss = RSS();

      //--------------------------------------------------------------------------//
      //  orig2rem                                         Coordinates Conversion //
      //--------------------------------------------------------------------------//
      //   This routines converts a coordinate according to the original product  //
      // state space to the remaining state space.                                //
      //--------------------------------------------------------------------------//
int RSS::orig2rem(const int s) const
{
 int o;        // original state space
 int g = 0;    // remaining state space

 if (remains(s))
   for (o=0; o<s; o++) {
     if (remains(o))
       g++;
   }
 else
   g = no_st;
 return(g);
}
      //--------------------------------------------------------------------------//
      //  orig2reach                                       Coordinates Conversion //
      //--------------------------------------------------------------------------//
      //   This routines converts a coordinate according to the original product  //
      // to the reachable state space.                                            //
      //--------------------------------------------------------------------------//
int RSS::orig2reach(const int s) const
{
 int o;        // original state space
 int r = 0;    // reachable state space

 if (has(s))
   for (o=0; o<s; o++) {
     if (has(o))
       r++;
   }
 else
   r = no_st;
 return(r);
}
      //--------------------------------------------------------------------------//
      //  rem2orig                                         Coordinates Conversion //
      //--------------------------------------------------------------------------//
      //   This routines converts a coordinate according to the remaining product //
      // state space to the original state space.                                 //
      //--------------------------------------------------------------------------//
int RSS::rem2orig(const int s) const
{
 int o;        // original state space
 int g = 0;    // remaining state space

 for (o=0; g<s; o++) {
   if (remains(o))
     g++;
 }
 return(o);
}
      //--------------------------------------------------------------------------//
      //  rem2reach                                        Coordinates Conversion //
      //--------------------------------------------------------------------------//
      //   This routines converts a coordinate according to the remaining product //
      // to the reachable state space.                                            //
      //--------------------------------------------------------------------------//
int RSS::rem2reach(const int s) const
{
 int o;        // original state space
 int g = 0;    // remaining state space
 int r = 0;    // reachable state space

 for (o=0; g<s; o++) {
   if (remains(o)) {
     g++;
     if (has(o))
       r++;
   }
 }
 if (!has(o))
   r = no_st;
 return(r);
}
      //--------------------------------------------------------------------------//
      //  reach2orig                                       Coordinates Conversion //
      //--------------------------------------------------------------------------//
      //   This routines converts a coordinate according to the reachable state   //
      // space to the original state space.                                       //
      //--------------------------------------------------------------------------//
int RSS::reach2orig(const int s) const
{
 int o;        // original state space
 int r = 0;    // reachable state space

 for (o=0; r<s; o++) {
   if (has(o))
     r++;
 }
 return(r);
}
      //--------------------------------------------------------------------------//
      //  reach2rem                                       Coordinates Conversion //
      //--------------------------------------------------------------------------//
      //   This routines converts a coordinate according to the reachable state   //
      // space to the remaining state space.                                      //
      //--------------------------------------------------------------------------//
int RSS::reach2rem(const int s) const
{
 int o;        // original state space
 int g = 0;    // remaining state space
 int r = 0;    // reachable state space

 for (o=0; r<s; o++) {
   if (remains(o)) {
     g++;
     if (has(o))
       r++;
   }
 }
 return(g);
}
     //--------------------------------------------------------------------------//
     //         It eliminates all global states such that the automata in        //
     //          the subset 's' are defined by the internal states in 'v'        //
     //--------------------------------------------------------------------------//
     //  Restriction: this function should NEVER eliminate reachable states.     // 
     //--------------------------------------------------------------------------//
     // Algorithm Principle: Let P be the set of internal states of a subset S   //
     //  of automata. This function eliminates from 'this' all global states     //
     //  where the projection on the automata of S is equal to P. The input      //
     //  parameter 'v' has the internal states of P for the automata in S        //
     //  (defined by the input parameter 's').                                   //
     //--------------------------------------------------------------------------//
     // Algorithm: - It identifies the automata not in 's' (called 'sbar');      //
     //            - For the states of the automata in 'sbar'                    //
     //               . it eliminates the global states containing P;            //
     //--------------------------------------------------------------------------//
void RSS::reduce(const aut_st & v, const aut_set & s)
{
#ifdef _PEPS_DEBUG_
  if (v.size() != FTB::ftb.aut())
    Programming_Error("wrong sized parameters", 2251)
  if (s.size() != FTB::ftb.aut())
    Programming_Error("wrong sized parameters", 2252)
#endif
 aut_st   sizes;         // the size of each original automata (from 'FTB::ftb')
 aut_set  sbar;          // the automata not in 's' (automata subset object)
 aut_list sbar_list;     // the automata not in 's' (automata list object)
 int      sub_size;      // product state space of the automata in 'sbar'
 aut_st   g_st(v);       // a buffer for global states (initialized with 'v')
 int      rank;          // a buffer for the rank of the global state 'g_st'

 //------------------------------------------- It defines the not in 's' automata subset
 FTB::ftb.get_domain(sizes);              // gets the size of all automata
 sbar.create(s.size());                   // puts the complement of 's' in 'sbar'
 sbar.complement(s);
 sbar_list.create(s.size());              // puts 'sbar' in a list object
 sbar_list.put(sbar);
 sub_size = sizes.state_space(sbar);      // computes the state space size of
                                          // the automata in 'sbar'
 //----------------------------------------------- It eliminates all states containing P
 g_st.reset(sbar_list);                   // resets the state of the automata in 'sbar'
 for (int i=0; i<sub_size; i++) {         // for all state space of the aut. in 'sbar'
   rank = g_st.rank(sizes);                  // gets the global state rank
   if (remain[rank]) {                       // if this global state was not eliminated
     remain.take(rank);                        // removes it
     dimension--;                              // (there is one remaining state less)
     if (states[rank]) {
       states.take(rank);
       reachable--;
     }
   }
   g_st.next(sbar_list, sizes);               // pass to the next global state in
 }                                            // 'sbar' product state space
}

     //--------------------------------------------------------------------------//
     //                 It creates a fully-rechable state space                  //
     //--------------------------------------------------------------------------//
void RSS::generate()
{
 remain.put();                           // mark all states as remaining
 states.put();                           // mark all states as reachable
 reachable = dimension;
 f_parameters.create(FTB::ftb.aut());    // the reachability function has no parameters
 f_parameters.take_out();
}
     //--------------------------------------------------------------------------//
     //      It marks all states with nonzero evaluation of 'f' as reachable     //
     //      ('s' contains the size of each automaton - the function domain)     //
     //           The vector 'remain' is filled with 'true' values.              //
     //--------------------------------------------------------------------------//
void RSS::generate(const func_tree & f, const aut_st & s)
{
#ifdef _PEPS_DEBUG_
  if (s.size() != FTB::ftb.aut())
    Programming_Error("wrong sized parameter", 2254)
#endif
 aut_st g_st(s.size());              // a buffer to visit all global states

 create(s.state_space());            // allocates 'this' data structures
 f_parameters.create(s.size());      // fills the 'f_parameters' subset
 f.param(f_parameters);              //   with the parameters of 'f'
 reachable = 0;
 g_st.reset();
 for (int i=0; i<dimension; i++) {   // for all the state space (all global states)
   if (f.eval(g_st) != zero) {          // if 'f' has a nonzero evaluation
     states.put(i);                        // marks this global state as reachable
     reachable++;
   }
   else                                 // if 'f' has a zero evaluation
     states.take(i);                       // marks this global state as unreachable
   g_st.next(s);                        // changes to the next global state
 }
}
     //--------------------------------------------------------------------------//
     //                    Change of a reachable state space                     //
     //--------------------------------------------------------------------------//
     //   The next function fills 'this' with a reachable state space putting as //
     // reachable only the states where the probability vector VCT::vct has      //
     // nonzero values.                                                          //
     //--------------------------------------------------------------------------//
void RSS::change_according_to_VCT()
{
#ifdef _PEPS_DEBUG_
  if (originals != dimension)
    Programming_Error("changing an already reduced rss", 2255)
  if (dimension != VCT::vct.size())
    Programming_Error("changing a rss without the correct vct", 2256)
#endif
 reachable = 0;
 for (int i=0; i<dimension; i++)     // for all the state space (all global states)
   if (VCT::vct.get(i) >= close_to_zero) { // if VCT::vct has a nonzero probability
     states.put(i);                        // marks this global state as reachable
     reachable++;
   }
}
      //--------------------------------------------------------------------------//
      //                    Change of a reachable state space                     //
      //--------------------------------------------------------------------------//
      //   The next function fills 'this' with a reachable state space putting as //
      // reachable the elements of a boolean vector (bools class object).         //
      //--------------------------------------------------------------------------//
void RSS::put(const bools v)
{
#ifdef _PEPS_DEBUG_
  if (v.size() != dimension)
    Programming_Error("wrong sized parameters", 2257)
  if (originals != dimension)
    Programming_Error("invalid operation over reduced rss structure", 2258)
  if (reachable != 0)
    Programming_Error("rss structure not ready to be reset", 2259)
#endif

  int i;      // a counter to pass by all states

  states.copy(v);                 // copy the vector of reachable states
  f_parameters.put_in();          // assume that it depends on all automata
  for (i=0; i<dimension; i++)     // count the reachable states
    if (has(i))                   //   (non optimized version)
      reachable++;                //      ...
}
     //--------------------------------------------------------------------------//
     //                  Dump the vector on 'fout' file stream                   //
     //--------------------------------------------------------------------------//
void RSS::dump_vector(ofstream & fout) const
{
 states.write(fout);
}
     //--------------------------------------------------------------------------//
     //                     Output on 'fout' file stream                         //
     //--------------------------------------------------------------------------//
void RSS::write() const
{
 ofstream fout;

 Open_File(file_name, rss_file, fout);
 fout << dimension << " " << originals << " " << reachable << "\n";
 f_parameters.write(fout);
 states.write(fout);
 remain.write(fout);
 Close_File(file_name, rss_file, fout);
}
     //--------------------------------------------------------------------------//
     //                     Input from 'fin' file stream                         //
     //--------------------------------------------------------------------------//
void RSS::read(const peps_name file)
{
 ifstream fin;
 int      d;

 Open_File(file, rss_file, fin);
 fin >> d;
 create(d);
 fin >> originals;
 fin >> reachable;
 f_parameters.read(fin);
 states.read(fin);
 remain.read(fin);
 baptise(file);
 Close_File(file_name, rss_file, fin);
}
     //--------------------------------------------------------------------------//
     //               Print a readable version in 'rss.dbg' file                 //
     //--------------------------------------------------------------------------//
void RSS::print() const
{
 peps_name vector_name, model_name;
 ofstream  fout;
 int       d;
 int       false_reach   = 0;
 int       false_unreach = 0;
 int       false_unreach_elim = 0;

 Provide_CND();
 if (VCT::vct.is_empty()) {
   cout << "\nYou need a probability vector to print the RSS.";
   Ask_an_Existing_File_Name(vector_name, vct_file);
   VCT::vct.read(vector_name);
 }

 Get_Only_the_File_Name(file_name, vector_name);
 DCT::dct.n_name(model_name);
 Open_File("rss", dbg_file, fout);
 fout << "===============================================================\n"
      << "File rss.dbg\n"
      << "===============================================================\n"
      << vector_name << ".inf -- The results for a model called '" << model_name << ".san')"
      << "\n------------\nGeneral Information\n------------"
      << "\nProduct state space:           " << originals << " states"
      << "\nState space after aggregation: " << dimension << " states"
      << "\nReachable state space:         " << reachable << " states"
      << "\nSize of this Structure: " << mem_use() / mem_unit << " Kbytes"
      << "\n===============================================================\n";
 for (d=0; d<originals; d++) {
   fout << "state " << d << ": ";
   if (remains(d)) {
     fout << "remains ";
     if (has(d)) {
       fout << "(reachable ";
       if (VCT::vct.get_pos(d) < close_to_zero) {
         fout << "but seems with null probability)\n";
         false_reach++;
       }
       else
         fout << ")\n";
     }
     else {
       fout << "(unreachable ";
       if (VCT::vct.get_pos(d) > close_to_zero) {
         fout << "but seems with non-null probability)\n";
         false_unreach++;
       }
       else
         fout << ")\n";
     }
   }
   else {
     fout << "was eliminated (unreachable ";
     if (VCT::vct.get_pos(d) > close_to_zero) {
       fout << "but seems with non-null probability)\n";
       false_unreach_elim++;
     }
     else
       fout << ")\n";
   }
 }
 fout << "\n===============================================================\n"
      << "Reachability function seems ";
 if ((false_reach == 0) && (false_unreach == 0) && (false_unreach_elim == 0))
   fout << "well-defined";
 else
   fout << "incorrect"
        << "\n---------------------------------------------------------------\n"
        << "Reachable states with null probability: " << false_reach
        << "\nUnreachable states with non-null probability: " << false_unreach
        << "\nEliminated states with non-null probability: " << false_unreach_elim
        << "\n---------------------------------------------------------------\n"
        << "States incorrectly eliminated (due to bad reachability function)\n"
        << "      avoid the correct solution of aggregated models!!";
 fout << "\n===============================================================\n";

 Close_File("rss", dbg_file, fout);
}
     //--------------------------------------------------------------------------//
     //              Memory space used by 'this' (in memory units)               //
     //--------------------------------------------------------------------------//
int RSS::mem_use() const
{
 int answer = 12;
 answer += f_parameters.mem_use();
 answer += remain.mem_use();
 answer += states.mem_use();
 return(answer);
}
