//====================================================================================//
//                                                                                    //
//                                Descriptor Structure                                //
//                                                                                    //
//====================================================================================//
//  This File:   dsc.C                      Language: C++  (xlC and CC)               //
//  Software:    PEPS 3.1                    Advisor: Brigitte.Plateau@imag.fr        //
//  Error Codes: 5300                                                                 //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 28/feb/02                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 22/apr/02                        by: Anne.Benoit@imag.fr             //
//  Last Change: 01/oct/03                        by: lbrenner@inf.pucrs.br           //
//====================================================================================//
//====================================================================================//
#include "peps.h"

     //--------------------------------------------------------------------------//
     //                          Static variable                                 //
     //--------------------------------------------------------------------------//
DSC DSC::dsc = DSC();

     //--------------------------------------------------------------------------//
     //            It counts how much nonzero elements would have the            //
     //             sparse matrix equivalent to the descritpor 'this'            //
     //--------------------------------------------------------------------------//
     //  This procedure generates all elements of reachable states of the sparse //
     // matrix equivalent to 'this' (resulting matrix) exactly like the function //
     // 'generate' of the 'HBF' class (file 'hbf.h'). The only difference is     //
     // that this function do not store the computed elements of the resulting   //
     // matrix.                                                                  //
     //                                                                          //
     // Note: this function works for all models (grouped or not).               //
     //--------------------------------------------------------------------------//
int DSC::nz()
{
 //----------------------------------------------------- Original descriptor information
 int    o_ss;      // the product state space (original state space size)
 int    o_i, o_j;  // coordinates of the rows and columns of the product state space
 aut_st o_sizes;   // the size of each original automaton
 aut_st o_state;   // the state vector (corresponding to the state in 'o_i')

 o_ss = RSS::rss.orig_size();           // the product state space size
 FTB::ftb.get_domain(o_sizes);          // the automata sizes
 o_state.create(FTB::ftb.aut());        // the state vector
 o_state.reset();                       //     (allocation and reset)

 //------------------------------------------------------ Current descriptor information
 aut_st d_i;       // the state vector (used to index the rows of the descriptor)
 aut_st d_j;       // the state vector (used to index the columns of the descriptor)

 d_j.create(automata);            // the state vector for the rows (allocation)
 d_i.create(automata);            // the state vector for the columns (allocation)

 //-------------------------------------------------------- Resulting matrix information
 int answer = 0;         // counter for nonzero elements of the resulting matrix

 //------------------------------------------------------------ "Visit all columns" loop
 for (o_j=0, d_j.reset(); o_j<o_ss; o_j++) {        // for each original state       - j
   if (RSS::rss.remains(o_j)) {                     //   if it is in the descriptor  - j
     if (RSS::rss.has(o_j))                         //     if it is reachable        - j
       //--------------------------------------------------------- "Visit all rows" loop
       for (o_i=0, d_i.reset(); o_i<o_ss; o_i++) {        // for each original state - i
         if (RSS::rss.remains(o_i)) {                     //   if it is in the desc. - i
           if (RSS::rss.has(o_i))                         //     if it is reachable  - i
             //--------------------------------------------- Computes the matrix element
             if (absv(coord(d_i, d_j, o_state)) > close_to_zero)
               answer++;                                      // counts nonzero elements
             //-------------------------------------------------------------------------
           d_i.next(sizes);                               //   advance desc. state   - i
         }
         o_state.next(o_sizes);                           // advance original stace  - i
       }
       //-------------------------------------------------------------------------------
     d_j.next(sizes);                               //   advance descriptor state    - j
   }
 }                                                  // advance original state        - j
 //-------------------------------------------------------------------------------------
 return(answer);
}

     //--------------------------------------------------------------------------//
     //                    Evaluation of a transition rate                       //
     //--------------------------------------------------------------------------//
     //    This function computes the numeric value of the transition rate       //
     // corresponding to a transition from the global state 'i' to the global    //
     // state 'j'. The coordinates 'i' and 'j' are vectors where each entry is   //
     // the state of a group.                                                    //
     //                                                                          //
     // Obs.: The input parameters 'i' and 'j' must be passed according to the   //
     // current descriptor automata (the size of the vectors 'i' and 'j' must be //
     // equal to the private variable 'automata' and the state maps are used to  //
     // obtain the global state according to the original number of automata.    //
     //--------------------------------------------------------------------------//
     // Algorithm: - Computes the original global state corresponding to 'i'     //
     //              (used for function evaluation);                             //
     //            - Computes the entry 'i,j' for the local transitions;         //
     //            - Computes the entry 'i,j' for the synch. transitions;        //
     //--------------------------------------------------------------------------//
rp DSC::coord(const aut_st & i, const aut_st & j)
{
#ifdef _PEPS_DEBUG_
  if ((i.size() != automata) || (j.size() != automata))
    Programming_Error("wrong global state size", 5301)
#endif

 aut_st state;             // original global state corresponding to 'i'
 aut_id a;                 // counter for the descriptor automata
 ev_id  e;                 // counter for the descriptor events
 rp     answer = 0;        // accumulator for the result
 rp     ev_plus, ev_minus; // temporaries to store the entry 'i,j' for the event 'e'
 aut_id mark = no_aut;     // the automaton that moves in the transition 'i' to 'j'
                           // if no automata moves or more than one moves this variable
                           // has the value 'no_aut'

 //----------------------------- Computes the original global state corresponding to 'i'
 state.create(FTB::ftb.aut());    // it creates an original global state vector
 for (a=0; a<automata; a++)       // for all automata of the descriptor
   state_map[a].get(i.st(a), state); // it gets the corresp. states from its 'state_map'

 //---------------------------------- Computes the entry 'i,j' for the local transitions
 for (a=0; a<automata; a++) {                       // for all local terms
   if (i.st(a) == j.st(a))                             // if automaton 'a' does not move
     answer += local_mat[a].coord(i.st(a), j.st(a), state); // it increments the answer
                                                            // with the diagonal
                                                            // element of 'a'
   else                                                // if automaton 'a' moves
     if (mark == no_aut)                                  // if it is the first aut.
       mark = a;                                             // to move mark it!
     else {                                               // if another already moved
       mark = no_aut;                                        // the answer is zero
       answer = zero;                                        // (for the local part)
       break;                                                // stops the loop
     }
 }
 if (mark != no_aut)                                 // if only one automaton moves
   answer = local_mat[mark].coord(i.st(mark), j.st(mark), state);  // the answer is its
                                                                   // moving rate

 //--------------------------------- Computes the entry 'i,j' for the synch. transitions
 for (e=0; e<events; e++) {             // for each synchronizing event 'e'
   ev_plus  = 1;                           // accumulator for the matrices +
   ev_minus = 1;                           // accumulator for the matrices -
   for (a=0; a<automata; a++) {            // iterative multiplication for all automata
     ev_plus  *= synch_mat[e][a].coord(i.st(a), j.st(a), state);
     ev_minus *= synch_adj[e][a].coord(i.st(a), j.st(a), state);
   }
   answer += (ev_plus + ev_minus);         // it adds both temporary accumulators
 }                                         // to the answer
 return(answer);
}

     //--------------------------------------------------------------------------//
     //                    Evaluation of a transition rate                       //
     //--------------------------------------------------------------------------//
     //    This function also computes the numeric value of the transition rate  //
     // corresponding to a transition from the global state 'i' to the global    //
     // state 'j'. The coordinates 'i' and 'j' are vectors where each entry is   //
     // the state of a group. Additionally, this function also receives the set  //
     // of parameters ('s') necessary to evaluate the functional elements of the //
     // descriptor. Usually the information in 'i' and 's' is equivalent. The    //
     // difference between this two vectors is their set of reference. The input //
     // parameter 'i' refers to the current descriptor number of automata (after //
     // grouping), while the input parameter 's' refers to the original number   //
     // of automata (before grouping).                                           //
     //                                                                          //
     // Obs.: The parameter 's' could be calculated from the input parameter row //
     // coordinate ('i') and the descriptor state maps.                          //
     //--------------------------------------------------------------------------//
     // Algorithm: - Computes the entry 'i,j' for the local transitions;         //
     //            - Computes the entry 'i,j' for the synch. transitions;        //
    //--------------------------------------------------------------------------//
rp DSC::coord(const aut_st & i, const aut_st & j, const aut_st & s)
{
ev_id  e;
 aut_id a;
 rp     answer = 0;        // accumulator for the result
 rp     ev_plus, ev_minus;
/*
#ifdef _PEPS_DEBUG_
  if ((i.size() != automata) || (j.size() != automata))
    Programming_Error("wrong global state size", 5302)
  if (s.size() != FTB::ftb.aut())
    Programming_Error("wrong parameters size", 5303)
#endif

 aut_id a;                 // counter for the descriptor automata
 ev_id  e;                 // counter for the descriptor events
 rp     answer = 0;        // accumulator for the result
 rp     ev_plus, ev_minus; // temporaries to store the entry 'i,j' for the event 'e'
 aut_id mark = no_aut;     // the automaton that moves in the transition 'i' to 'j'
                           // if no automata moves or more than one moves this variable
                           // has the value 'no_aut'

 //---------------------------------- Computes the entry 'i,j' for the local transitions
 for (a=0; a<automata; a++) {                       // for all local terms
   if (i.st(a) == j.st(a))                             // if automaton 'a' does not move
     answer += local_mat[a].coord(i.st(a), j.st(a), s);     // it increments the answer
                                                            // with the diagonal
                                                            // element of 'a'
   else                                                // if automaton 'a' moves
     if (mark == no_aut)                                  // if it is the first aut.
       mark = a;                                             // to move mark it!
     else {                                               // if another already moved
       mark = no_aut;                                        // the answer is zero
       answer = zero;                                        // (for the local part)
       break;                                                // stops the loop
     }
 }
 if (mark != no_aut)                                 // if only one automaton moves
   answer = local_mat[mark].coord(i.st(mark), j.st(mark), s);      // the answer is its
                                                                   // moving rate
*/
 //--------------------------------- Computes the entry 'i,j' for the synch. transitions
 for (e=0; e<events; e++) {             // for each synchronizing event 'e'
   ev_plus  = 1;                           // accumulator for the matrices +
   ev_minus = 1;                           // accumulator for the matrices -
   for (a=0; a<automata; a++) {            // iterative multiplication for all automata
     ev_plus  *= synch_mat[e][a].coord(i.st(a), j.st(a), s);
     ev_minus *= synch_adj[e][a].coord(i.st(a), j.st(a), s);
   }
   answer += (ev_plus + ev_minus);         // it adds both temporary accumulators
 }                                         // to the answer
 return(answer);
}


rp DSC::get_sync_coord(int  i, int j, const aut_st & s)
{
ev_id  e;
 aut_id a;
 rp     answer = 0;        // accumulator for the result
 rp     ev_plus, ev_minus;

 //--------------------------------- Computes the entry 'i,j' for the synch. transitions
 for (e=0; e<events; e++) {             // for each synchronizing event 'e'
   ev_plus  = 1;                           // accumulator for the matrices +
   ev_minus = 1;                           // accumulator for the matrices -
   for (a=0; a<automata; a++) {            // iterative multiplication for all automata
     ev_plus  *= synch_mat[e][a].coord(i, j, s);
     ev_minus *= synch_adj[e][a].coord(i, j, s);
   }
   answer += (ev_plus + ev_minus);         // it adds both temporary accumulators
 }                                         // to the answer
 return(answer);
}


     //--------------------------------------------------------------------------//
     //    Return the value of the local matrix of automaton 'a' corresponding   //
     //         to a transition from global state 'i' to global state 'j'        //
     //--------------------------------------------------------------------------//
rp DSC::coord_loc(aut_id a, const aut_st & i, const aut_st & j, const aut_st & s)
{
#ifdef _PEPS_DEBUG_
  if ((i.size() != automata) || (j.size() != automata))
    Programming_Error("wrong global state size", 5302)
  if (s.size() != FTB::ftb.aut())
    Programming_Error("wrong parameters size", 5303)
#endif
      
 return(local_mat[a].coord(i.st(a), j.st(a), s));
 
}
     //--------------------------------------------------------------------------//
     //  Return the value of the positive synchronizing matrix of automaton 'a'  //
     //  corresponding to a transition from global state 'i' to global state 'j' //
     //--------------------------------------------------------------------------//
rp DSC::coord_sync_p(ev_id e, aut_id a, const aut_st & i, const aut_st & j,
		     const aut_st & s)
{
#ifdef _PEPS_DEBUG_
  if ((i.size() != automata) || (j.size() != automata))
    Programming_Error("wrong global state size", 5302)
  if (s.size() != FTB::ftb.aut())
    Programming_Error("wrong parameters size", 5303)
#endif

      /*
      cout << " coord_sync_p : e = " << e << " - a = " << a
	   << " - i = " << i.st(a) << " - j = " << j.st(a)
	   << " - coord = " << synch_mat[e][a].coord(i.st(a), j.st(a), s) << "\n";
      */
      return(synch_mat[e][a].coord(i.st(a), j.st(a), s));

}
     //--------------------------------------------------------------------------//
     //    //
     //   //
     //--------------------------------------------------------------------------//
rp DSC::get_local_coord(aut_id a, int i, int j, const aut_st & s)
{
#ifdef _PEPS_DEBUG_
  if ((i != automata) || (j != automata))
    Programming_Error("wrong global state size", 5302)
  if (s.size() != FTB::ftb.aut())
    Programming_Error("wrong parameters size", 5303)
#endif

      /*
      cout << " coord_sync_p : e = " << e << " - a = " << a
	   << " - i = " << i.st(a) << " - j = " << j.st(a)
	   << " - coord = " << synch_mat[e][a].coord(i.st(a), j.st(a), s) << "\n";
      */
      return(local_mat[a].coord(i, j, s));

}
     //--------------------------------------------------------------------------//
     //    //
     //   //
     //--------------------------------------------------------------------------//
rp DSC::get_pos_sync_coord(int e, int a, int i, int j, const aut_st & s)
{
#ifdef _PEPS_DEBUG_
  if ((i != automata) || (j != automata))
    Programming_Error("wrong global state size", 5302)
  if (s.size() != FTB::ftb.aut())
    Programming_Error("wrong parameters size", 5303)
#endif                                // to the answer
 return(synch_mat[e][a].coord(i, j, s));
}


     //--------------------------------------------------------------------------//
     //    //
     //   //
     //--------------------------------------------------------------------------//
rp DSC::get_neg_sync_coord(int e, aut_st d_i, const aut_st & s)
{
#ifdef _PEPS_DEBUG_
  //if ((i.size() != automata) || (j.size() != automata))
  //  Programming_Error("wrong global state size", 5302)
  if (s.size() != FTB::ftb.aut())
    Programming_Error("wrong parameters size", 5303)
#endif
 rp v=1;
 int n=d_i.dimension;
 for(int i=0;i<n;i++)
  v *= synch_adj[e][i].coord(d_i.info[i], d_i.info[i], s);
 return(v);
}



     //--------------------------------------------------------------------------//
     //  Return the value of the negative synchronizing matrix of automaton 'a'  //
     //  corresponding to a transition from global state 'i' to global state 'j' //
     //--------------------------------------------------------------------------//
rp DSC::coord_sync_n(ev_id e, aut_id a, const aut_st & i, const aut_st & j,
		     const aut_st & s)
{

#ifdef _PEPS_DEBUG_
  if ((i.size() != automata) || (j.size() != automata))
    Programming_Error("wrong global state size", 5302)
  if (s.size() != FTB::ftb.aut())
    Programming_Error("wrong parameters size", 5303)
#endif
	 
      /* 
      cout << " coord_sync_n : e = " << e << " - a = " << a 
	   << " - i = " << i.st(a) << " - j = " << j.st(a)
	   << " - coord = " << synch_adj[e][a].coord(i.st(a), j.st(a), s) << "\n";
      */
      return(synch_adj[e][a].coord(i.st(a), j.st(a), s));

}
     //--------------------------------------------------------------------------//
     //                     Output on 'fout' file stream                         //
     //--------------------------------------------------------------------------//
void DSC::write() const
{
 ofstream fout;
 aut_id   a;
 ev_id    e;

 Open_File(file_name, dsc_file, fout);
 fout << int(model_type) << " " << automata << " " << events << "\n"
      << orig_name << "\n";
 sizes.write(fout);
 for (a=0; a<automata; a++)
   local_mat[a].write(fout);
 for (e=0; e<events; e++)
   for (a=0; a<automata; a++)
     synch_mat[e][a].write(fout);
 for (e=0; e<events; e++)
   for (a=0; a<automata; a++)
     synch_adj[e][a].write(fout);
 for (a=0; a<automata; a++)
   state_map[a].write(fout);
 for (e=0; e<events; e++)
   fout << sync_rates[e] << " ";
 fout << "\n";
 symetric.write(fout);
 Close_File(file_name, dsc_file, fout);
}
     //--------------------------------------------------------------------------//
     //                     Input from 'fin' file stream                         //
     //--------------------------------------------------------------------------//
void DSC::read(const peps_name file)
{
 ifstream fin;
 int      m;
 aut_id   a;
 ev_id    e;

 Open_File(file, dsc_file, fin);
 fin >> m;
 fin >> a;
 fin >> e;
 create(md_type(m), a, e);
 baptise(file);
 fin >> orig_name;
 sizes.read(fin);
 for (a=0; a<automata; a++)
   local_mat[a].read(fin);
 for (e=0; e<events; e++)
   for (a=0; a<automata; a++)
     synch_mat[e][a].read(fin);
 for (e=0; e<events; e++)
   for (a=0; a<automata; a++)
     synch_adj[e][a].read(fin);
 for (a=0; a<automata; a++)
   state_map[a].read(fin);
 for (e=0; e<events; e++)
   fin >> sync_rates[e];
 symetric.read(fin);
 Close_File(file_name, dsc_file, fin);
}
     //--------------------------------------------------------------------------//
     //                      Print on 'fout' file stream                         //
     //--------------------------------------------------------------------------//
void DSC::print() const
{
 peps_name descriptor_name, model_name, bufname;
 ofstream  fout;
 aut_st    bufsizes;
 aut_id    a;
 ///aut_id    aux;
 ev_id     e;
 ///int       s;

 Provide_DSC();

 Get_Only_the_File_Name(file_name, descriptor_name);
 DCT::dct.n_name(model_name);
 Open_File("dsc", dbg_file, fout);
 fout << "===============================================================\n"
      << "File dsc.dbg\n"
      << "===============================================================\n"
      << descriptor_name << ".dsc -- A model with " << automata
      << " automata and " << events << " events"
      << "\nUser name: '" << model_name << "' (file '" << orig_name << ".san)"
      << "\n------------\nGeneral Information\n------------"
      << "\nProduct state space:           " << RSS::rss.orig_size() << " states"
      << "\nState space after aggregation: " << RSS::rss.size()      << " states"
      << "\nReachable state space:         " << RSS::rss.reach_size() << " states"
      << "\nAutomata sizes:                   [ ";
 FTB::ftb.get_domain(bufsizes);
 for (a=0; a<bufsizes.size(); a++)
   fout << bufsizes.sz(a) << " ";
 fout << "]\nAutomata sizes after aggregation: [ ";
 for (a=0; a<sizes.size(); a++)
   fout << sizes.sz(a) << " ";
 fout << "]\nSimetric Labels for the automata: [ ";
 for (a=0; a<symetric.size(); a++)
   fout << symetric.val(a) << " ";
 fout << "]\nCurrent Number of Functions: " << FTB::ftb.size()
      << "\nSize of the Descriptor: " << mem_use() / mem_unit << " Kbytes"
      << "\n===============================================================\n";
 fout << "\n------------\nLocal Matrices\n------------\n";
 for (a=0; a<automata; a++) {                // local matrices
   DCT::dct.a_name(a, bufname);
   fout << "Automaton: " << bufname << "\n";
   local_mat[a].write(fout);
 }
 if (events != 0) {
   fout << "\n------------\nSynch Events Matrices\n------------\n";
   for (e=0; e<events; e++) {                 // sync. matrices
     for (a=0; a<automata; a++) {
       DCT::dct.e_name(e, bufname);
       fout << "Event " << bufname;
       DCT::dct.a_name(a, bufname);
       fout << " Automaton: " << bufname << "+ (";
       if (sync_rates[e] == a)
         fout << "master";
       else
         fout << "slave)";
       fout << ")\n++++++++++++++++++++++++++++++++++++++\n";
       synch_mat[e][a].write(fout);
     }
     for (a=0; a<automata; a++) {
       DCT::dct.e_name(e, bufname);
       fout << "Event " << bufname;
       DCT::dct.a_name(a, bufname);
       fout << " Automaton: " << bufname << "- (";
       if (sync_rates[e] == a)
         fout << "master";
       else
         fout << "slave";
       fout << ")\n--------------------------------------\n";
       synch_adj[e][a].write(fout);
     }
   }
 }
 fout << "\n------------\nState Maps\n------------";
 for (a=0; a<automata; a++) {                // automata state maps
       DCT::dct.a_name(a, bufname);
       fout << " Automaton: " << bufname << "\n";
   state_map[a].write(fout);
 }
 Close_File("dsc", dbg_file, fout);
}



     //--------------------------------------------------------------------------//
     //              Memory space used by 'this' (in memory units)               //
     //--------------------------------------------------------------------------//
int DSC::mem_use() const
{
 aut_id a;
 ev_id  e;
 int    answer = 12;

 answer += sizes.mem_use();
 answer += symetric.mem_use();
 for (a=0; a<automata; a++) {
   answer += state_map[a].mem_use();
   answer += local_mat[a].mem_use();
 }
 for (e=0; e<events; e++) {
   for (a=0; a<automata; a++) {
     answer += synch_mat[e][a].mem_use();
     answer += synch_adj[e][a].mem_use();
     answer += 4;
   }
 }
 return(answer);
}


     //--------------------------------------------------------------------------//
     //                             Main Functions                               //
     //--------------------------------------------------------------------------//
     //                                                                          //
     //                      Aggregation of the Descriptor                       //
     //                                                                          //
     //    Input and Output Parameters:                                          //
     //                                                                          //
     //      - 'this'      before: the descriptor to be aggregated;              //
     //                    after:  the aggregated descriptor;                    //
     //      - 'FTB::ftb'  before: the old function table;                       //
     //                    after:  the new function table;                       //
     //      - 'RSS::rss'  before: the old (non-reduced) reachable state space;  //
     //                    after:  the new (reduced) reachable state space;      //
     //                                                                          //
     //    Only Input Parameters:                                                //
     //                                                                          //
     //      - 's'         an aggregation map, i.e., an array of automata labels //
     //                    where automata with the same label will be grouped    //
     //                    into a single automaton;                              //
     //      - 'name'      the file name of the new structures (that are saved   //
     //                    after grouping);                                      //
     //                                                                          //
     //   Restrictions: the aggregation map MUST contain contiguous labels from  //
     // 0 to M-1, where M is the number of different labels in the aggregation   //
     // map. This restriction allows the definition the new descriptor with M    //
     // grouped automata and the use of the labels as the internal id's of the   //
     // groups. Note that, the grouping process can also change the ranking      //
     // order of the automata.                                                   //
     //                                                                          //
     //   Note: there are no restrictions to create groups with only one         //
     // automaton. In this case, the automaton will not change.                  //
     //--------------------------------------------------------------------------//
void DSC::aggregate(const aut_st & s, const peps_name name)
{
#ifdef _PEPS_DEBUG_
  if (is_grouped())
    Programming_Error("aggregation of an already grouped descriptor", 5324)
  if (s.size() != automata)
    Programming_Error("wrong aggregation map size", 5325)
  if (!(s.contigous()) || (s.smallest() != 0))
    Programming_Error("bad aggregation map", 5326)
#endif

 DSC d;       // temporary object to store the new descriptor
 int groups;  // number of groups (new number of automata)
 int g;       // counter for groups

 groups = s.count_groups();            // it computes the new number of automata
 scr_ft::sft.create(200);              // it sets the dimension the scratch func. table
 FTB::ftb.put_currents(s);             // it saves aggreg. map into the functions table
 d.create(model_type, groups, events); // it creates the new descriptor object
 get_sr(d.sync_rates);                 // it copies the original information concerning
 get_sms(d.symetric);                  // the synch. rates and symetries

 for (g=0; g<groups; g++)              // for all new automata matrices
   make_a_group(s, g, d);                 // it computes the tensor sum and products
                                          // (new functions are stored in 'scr_ft::sft')

 FTB::ftb.create(scr_ft::sft);         // it copies the scratch into the func. table

 for (g=0; g<groups; g++)              // for all new automata
   d.reduce_a_group(g);                   // it reduces its state space

 d.solve_functions();                  // it attempts to evaluate all functions

 d.print();
 copy(d);                              // it copies the temporary descriptor 'd'
 d.erase();                            // into 'this' structure

 scr_ft::sft.create(200);              // resets the scratch functions table
 eliminate_events();                   // it attempts to eliminate synch. events
 FTB::ftb.create(scr_ft::sft);         // it copies the scratch into the func. table

 strcpy(file_name, orig_name);         // it saves the original name of the descriptor
 strcpy(file_name, name); write();     // it baptises and writes the new descriptor
 RSS::rss.baptise(name);  RSS::rss.write();    // and the related structures
 FTB::ftb.baptise(name);  FTB::ftb.write();    // reacheable state space and func. table
}

     //--------------------------------------------------------------------------//
     //                             Main Functions                               //
     //--------------------------------------------------------------------------//
     //                                                                          //
     //                  Aggregation replics of the Descriptor                   //
     //                                                                          //
     //    Input and Output Parameters:                                          //
     //                                                                          //
     //      - 'this'      before: the descriptor to be aggregated;              //
     //                    after:  the aggregated descriptor;                    //
     //      - 'FTB::ftb'  before: the old function table;                       //
     //                    after:  the new function table;                       //
     //      - 'RSS::rss'  before: the old (non-reduced) reachable state space;  //
     //                    after:  the new (reduced) reachable state space;      //
     //                                                                          //
     //    Only Input Parameters:                                                //
     //                                                                          //
     //      - 's'         an aggregation map, i.e., an array of automata labels //
     //                    where automata with the same label will be grouped    //
     //                    into a single automaton;                              //
     //      - 'name'      the file name of the new structures (that are saved   //
     //                    after grouping);                                      //
     //                                                                          //
     //   Restrictions: the aggregation map MUST contain contiguous labels from  //
     // 0 to M-1, where M is the number of different labels in the aggregation   //
     // map. This restriction allows the definition the new descriptor with M    //
     // grouped automata and the use of the labels as the internal id's of the   //
     // groups. Note that, the grouping process can also change the ranking      //
     // order of the automata.                                                   //
     //                                                                          //
     //   Note: there are no restrictions to create groups with only one         //
     // automaton. In this case, the automaton will not change.                  //
     //--------------------------------------------------------------------------//
void DSC::aggregate_replics(const aut_st & s, const peps_name name)
{
#ifdef _PEPS_DEBUG_
  if (is_grouped())
    Programming_Error("aggregation of an already grouped descriptor", 5324)
  if (s.size() != automata)
    Programming_Error("wrong aggregation map size", 5325)
  if (!(s.contigous()) || (s.smallest() != 0))
    Programming_Error("bad aggregation map", 5326)
#endif

 DSC d;       // temporary object to store the new descriptor
 int groups;  // number of groups (new number of automata)
 int g;       // counter for groups

 groups = s.count_groups();            // it computes the new number of automata
 scr_ft::sft.create(200);              // it sets the dimension the scratch func. table
 FTB::ftb.put_currents(s);             // it saves aggreg. map into the functions table
 d.create(model_type, groups, events); // it creates the new descriptor object
 get_sr(d.sync_rates);                 // it copies the original information concerning
 get_sms(d.symetric);                  // the synch. rates and symetries

 for (g=0; g<groups; g++)              // for all new automata matrices
   make_a_replic_group(s, g, d);       // it creates the new local matrix

 FTB::ftb.create(scr_ft::sft);         // it copies the scratch into the func. table
// d.print();

 for (g=0; g<groups; g++)              // for all new automata
   d.reduce_a_group(g);                   // it reduces its state space

 d.solve_functions();                  // it attempts to evaluate all functions

 copy(d);                              // it copies the temporary descriptor 'd'
 d.erase();                            // into 'this' structure

 scr_ft::sft.create(200);              // resets the scratch functions table
 eliminate_events();                   // it attempts to eliminate synch. events
 FTB::ftb.create(scr_ft::sft);         // it copies the scratch into the func. table
 
 strcpy(file_name, orig_name);         // it saves the original name of the descriptor
 strcpy(file_name, name); write();     // it baptises and writes the new descriptor
 RSS::rss.baptise(name);  RSS::rss.write();    // and the related structures
 FTB::ftb.baptise(name);  FTB::ftb.write();    // reacheable state space and func. table
}

      //--------------------------------------------------------------------------//
      //            Internal Functions of the Main Function Agregation            //
      //--------------------------------------------------------------------------//
      // It builds the tensor sum of the local matrices and the tensor product of //
      //  the sync. matrices for the grouped automaton 'g'. The automata used as  //
      //      components are coded with label 'g' in the aggregation map 's'.     //
      //--------------------------------------------------------------------------//
      // Algorithm: - It identifies the automata to be grouped;                   //
      //            - It computes the tensor sum of the local matrices;           //
      //            - For each synch. event:                                      //
      //                = It computes the tensor product the synch. matrices;     //
      //            - It generates the corresponding state map;                   //
      //            - It generates a new 'symetric' array;                        //
      //--------------------------------------------------------------------------//
void DSC::make_a_group(const aut_st & s, const aut_id g, DSC & d) const
{
#ifdef _PEPS_DEBUG_
  if (!(s.has(g)))
    Programming_Error("creation of an empty group", 5327)
  if (g >= d.automata)
    Programming_Error("creation of an invalid group", 5328)
  if (s.size() != automata)
    Programming_Error("bad aggregation map", 5329)
#endif

 aut_set   group(automata);        // the automata to be grouped (in a subset)
 aut_list  g_list(automata);       // the automata to be grouped (in a list)
 int       n;                      // the number of automata in the group
 aut_st    g_size;                 // the size of each automaton to be grouped
 san_mat * mats;                   // the matrices to be grouped
 aut_id    a;                      // counter for the automata
 ev_id     e;                      // counter for the synch. events

 //----------------------------------------------- Identifies the automata to be grouped
 group.take_out();                         // component automata in a subset object
 group.put_in(s, g);
 g_list.take_out();                        // component automata in a list object
 g_list.put(group);
 n = group.has();                          // number of component automata
 g_size.create(n);                         // the size of each component automata
 for (a=0; a<n; a++)
   g_size.put(a, local_mat[g_list.aut(a)].size());
 d.sizes.put(g, g_size.state_space());     // the state space of the grouped automaton

 //--------------------------- allocation of temporary matrices (one for each component)
 mats = new san_mat[n];

 //------------------------------------ It computes the tensor sum of the local matrices
 if (n == 1)                                // if there is only one component
   d.local_mat[g].assign(local_mat[g_list.aut(0)]); // just copy it!
 else {                                     // if there is more than one component
   for (a=0; a<n; a++)                         // it copies the local matrices
     mats[a].copy(local_mat[g_list.aut(a)]);   // and perform the tensor sum of
   d.local_mat[g].t_sum(mats, n);              // the components
 }

 //---------------------------------- It computes the tensor product the synch. matrices
 for (e=0; e<events; e++) {                      // for all events
   if (n == 1) {                                    // if there is only one component
     d.synch_mat[e][g].assign(synch_mat[e][g_list.aut(0)]);   // just copy it!
     d.synch_adj[e][g].assign(synch_adj[e][g_list.aut(0)]);
   }
   else {                                           // if there is more than one comp.
     for (a=0; a<n; a++)                               // it copies the + matrices
       mats[a].copy(synch_mat[e][g_list.aut(a)]);      // and perform the tensor product
     d.synch_mat[e][g].t_prod(mats, n);                // of the components
     for (a=0; a<n; a++)
       mats[a].copy(synch_adj[e][g_list.aut(a)]);      // Idem for the - matrices
     d.synch_adj[e][g].t_prod(mats, n);
   }
 }
 //----------------------------------------------- Generates the corresponding state map
 d.state_map[g].create(d.local_mat[g].size(), n); // allocates the state map table
 d.state_map[g].put(group);                       // stores the components id's
 d.state_map[g].fill(g_size);                     // generates the state map contents
 //-----------------------------------------------------------------------------------//
 // Important: It generates a state map in a lexical order combination of the component
 // states. The aggregation can not be applied to already grouped descriptors because
 // of this decision. Further PEPS versions could remove this restriction by replacing
 // the code above by a functions that computes a state map as combination of the state
 // maps of the components. 
 //-----------------------------------------------------------------------------------//

 //------------------------------------------------- It generates a new 'symetric' array
 d.symetric.create(d.automata);
 d.symetric.stamp();           // generation of a different symetric label to each group
 //-----------------------------------------------------------------------------------//
 // Important: It looses the old symetric information. Further PEPS versions could 
 // remove this restriction by replacing the code above by a function that computes the
 // new symetric array ('d.symetric') based on the old one ('symetric').
 //-----------------------------------------------------------------------------------//

 //------------------------------------------------- desallocation of temporary matrices
 for (a=0; a<n; a++)
   mats[a].erase();
 delete[] mats;
}


      //--------------------------------------------------------------------------//
      //            Internal Functions of the Main Function Agregation            //
      //--------------------------------------------------------------------------//
      // Algorithm: - It identifies the automata to be grouped;                   //
      //            - It computes the descritor of the local matrices;           //
      //            - For each synch. event:                                      //
      //                = It computes the descritor the synch. matrices;     //
      //            - It generates the corresponding state map;                   //
      //            - It generates a new 'symetric' array;                        //
      //--------------------------------------------------------------------------//
void DSC::make_a_replic_group(const aut_st & s, const aut_id g, DSC & d) const
{
#ifdef _PEPS_DEBUG_
  if (!(s.has(g)))
    Programming_Error("creation of an empty group", 5327)
  if (g >= d.automata)
    Programming_Error("creation of an invalid group", 5328)
  if (s.size() != automata)
    Programming_Error("bad aggregation map", 5329)
#endif

 aut_set   group(automata);        // the automata to be grouped (in a subset)
 aut_list  g_list(automata);       // the automata to be grouped (in a list)
 int       n;                      // the number of automata in the group
 int       rep;                    // the number of repetions 
 int       g_state_space;          // the size of the grouped automaton
 int       active;                 // flag to synchronized event
 aut_st    g_size;                 // the size of each automaton to be grouped
 aut_st    map;                    // the autmata states to the state map
 aut_st    map2;
 elem      t_rate(Rate,0.0,no_id); // the transition rate (local and synchronized)
 elem    **mat;                    // the grouped matrix
 aut_id    a;                      // counter for the automata
 ev_id     e;                      // counter for the synch. events
 st_id     dst;                    // transition destine

 //----------------------------------------------- Identifies the automata to be grouped
 group.take_out();                         // component automata in a subset object
 group.put_in(s, g);
 g_list.take_out();                        // component automata in a list object
 g_list.put(group);
 n = group.has();                          // number of component automata
 g_size.create(n);                         // the size of each component automata


 for (a=0; a<n; a++)
   g_size.put(a, local_mat[g_list.aut(a)].size());
 
 g_state_space = 1;
 if(n >= local_mat[g_list.aut(0)].size()){
   for(int i=(n+local_mat[g_list.aut(0)].size()-1); i>n; i--)
     g_state_space *= i;	
   for(int i=(local_mat[g_list.aut(0)].size()-1); i>1; i--)
     g_state_space /= i;
 }
 else{
   for(int i=(n+local_mat[g_list.aut(0)].size()-1); i>=local_mat[g_list.aut(0)].size(); i--)
     g_state_space *= i;	
   for(int i=n; i>1; i--)
     g_state_space /= i;
 }

 d.sizes.put(g, g_state_space);            // the state space of the grouped automaton

 //--------------------------- allocation of temporary matrix 
 mat = new elem*[g_state_space];
 for(int i=0; i<g_state_space; i++){
   mat[i] = new elem[g_state_space];
   for(int j=0; j<g_state_space; j++)
     mat[i][j].put(Rate, 0.0, no_id);
 }  

 //----------------------------------------------- Generates the corresponding state map
 d.state_map[g].create(g_state_space, n);               // allocates the state map table
 d.state_map[g].put(group);                                // stores the components id's
 map.create(n); 
 map2.create(FTB::ftb.aut()); 
 for(long unsigned int i=0, j=0; i<g_size.state_space(); i++){ 
   map.create_map(i, local_mat[g_list.aut(0)].size());
   for(int k=0; k<n; k++)
     map2.stamp(g_list.aut(k),map.st(k));
   if(map2.is_sorted(g_list.aut(0),g_list.aut(n-1))){ 
     d.state_map[g].put(j,map2);
     j++;
   }
   else{
     RSS::rss.reduce(map2, group);                       // it takes the global states 
   }
 }

 scr_ft::sft.create(FTB::ftb);   
 //------------------------------------ It computes the tensor sum of the local matrices
 if (n == 1)                                           // if there is only one component
   d.local_mat[g].assign(local_mat[g_list.aut(0)]);                     // just copy it!
 else {                                           // if there is more than one component
   for(int i=0; i<g_state_space; i++){
     d.state_map[g].get(i,map2);
     for(int l=0, j=0; j<n; j++){ 
       rep = map2.count_rep(l,g_list.aut(0),g_list.aut(n-1));
       if(rep!=0){
         for(int k=0; k<local_mat[g_list.aut(0)].size(); k++){
           if((local_mat[g_list.aut(0)].coord(l,k)!=zero) && (l!=k)){
             map2.stamp(g_list.aut(j),k); 
             dst = d.state_map[g].get(map2); 
             mat[i][dst] = local_mat[g_list.aut(0)].coord(l, k) * rep;             
             d.state_map[g].get(i,map2); 
           }
         }
       }
       j += rep-1;	
       l++;
     }  
   }

   FTB::ftb.create(scr_ft::sft);         // it copies the scratch into the func. table
   scr_ft::sft.create(FTB::ftb);   
   for(int i=0; i<g_state_space; i++){
     mat[i][i].put(Rate,0.0,no_id);
     t_rate.add(mat[i],g_state_space);  
     FTB::ftb.create(scr_ft::sft);        // it copies the scratch into the func. table         
     scr_ft::sft.create(FTB::ftb);
     mat[i][i] = t_rate * -1.0;
     FTB::ftb.create(scr_ft::sft);       // it copies the scratch into the func. table
     scr_ft::sft.create(FTB::ftb);
   }   
   FTB::ftb.create(scr_ft::sft);         // it copies the scratch into the func. table   
   d.local_mat[g].create(mat, g_state_space); 
 } 
 
 for(int i=0; i<g_state_space; i++){
   for(int j=0; j<g_state_space; j++){
     mat[i][j] = 0.0;
   } 
 }   

 scr_ft::sft.create(FTB::ftb);   

 //---------------------------------- It computes the tensor product the synch. matrices
 for (e=0; e<events; e++) {                                            // for all events
   if (n == 1) {                                       // if there is only one component
     d.synch_mat[e][g].assign(synch_mat[e][g_list.aut(0)]);             // just copy it!
     d.synch_adj[e][g].assign(synch_adj[e][g_list.aut(0)]);
   }
   else {                                             // if there is more than one comp.
     for(int i=0; i<g_state_space; i++)
       mat[i][i] = 0.0;
     for(int i=0; i<g_state_space; i++){ 
       d.state_map[g].get(i,map2);  
       for(int l=0, j=0; j<n; j++){ 
         rep = map2.count_rep(l,g_list.aut(0),g_list.aut(n-1));
         active = 0;
         t_rate = 1.0;
         if(rep!=0){
           for(int k=0; k<synch_mat[e][g_list.aut(0)].size(); k++){
             if(synch_mat[e][g_list.aut(0)].coord(l,k)!=zero){ 
               for(int r=0; r<rep; r++){
                 map2.stamp(g_list.aut(j)+r,k);
                 t_rate *= synch_mat[e][g_list.aut(j)+r].coord(l,k);
               }
               active = 1;
               break;
             }
           }
           if(active == 0){
             break;
           }
         }
         j += rep-1;	
         l++;
       }
       if(active == 1){
         dst = d.state_map[g].get(map2); 
         mat[i][dst] = t_rate;
       }
     }

     FTB::ftb.create(scr_ft::sft);         // it copies the scratch into the func. table
     scr_ft::sft.create(FTB::ftb);   
     d.synch_mat[e][g].create(mat, g_state_space); 
     for(int i=0; i<g_state_space; i++){
       t_rate.add(mat[i],g_state_space);  
       FTB::ftb.create(scr_ft::sft);      // it copies the scratch into the func. table         
       scr_ft::sft.create(FTB::ftb);
       mat[i][i] = t_rate;
       for(int j=0; j<g_state_space; j++){
         if(i!=j){
           mat[i][j] = 0.0;
         }
       } 
     }   
     d.synch_adj[e][g].create(mat, g_state_space); 
   }
 }

 //------------------------------------------------- It generates a new 'symetric' array
 d.symetric.create(d.automata);
 d.symetric.stamp();           // generation of a different symetric label to each group
 //-----------------------------------------------------------------------------------//
 // Important: It looses the old symetric information. Further PEPS versions could 
 // remove this restriction by replacing the code above by a function that computes the
 // new symetric array ('d.symetric') based on the old one ('symetric').
 //-----------------------------------------------------------------------------------//

 //------------------------------------------------- desallocation of temporary matrices
 for(int i=0; i<g_state_space; i++)
   if(mat[i])
     delete[] mat[i];
 if(mat)
   delete[] mat; 
}

      //--------------------------------------------------------------------------//
      //            Internal Functions of the Main Function Agregation            //
      //--------------------------------------------------------------------------//
      //          It reduces all matrices and the state map of an automaton       //
      //                  according to the reachable state space                  //
      //--------------------------------------------------------------------------//
      // Algorithm Principle: A state 's' of a grouped automaton 'g' is not       //
      // reachable iff ALL global states "containing" 's' (i.e., whose projection //
      // on 'g' is equal to 's') are unreachable. To verify is 's' is reachable   //
      // or not the algorithm generates all global states containing 's'. For     //
      // that purpose, the subset 'to_change' contains all automata that are      //
      // parameters of the reachability function and are components of 'g'.       //
      //--------------------------------------------------------------------------//
      // Algorithm: - Computes automata 'to_change';                              //
      //            - Defines which states must be kept;                          //
      //            - Reduces the structures concerning the automaton 'g';        //
      //--------------------------------------------------------------------------//
void DSC::reduce_a_group(const aut_id g)
{
#ifdef _PEPS_DEBUG_
  if (g >= automata)
    Programming_Error("reduction of an invalid group", 5330)
#endif

 aut_set to_change;   // the subset of automata that must be changed
 aut_set known;       // the subset of automata states known (the grouped automata)
 bools   to_keep;     // the states to keep
 aut_st  s;           // global state (partially known)
 int     i;           // counter for the states of the automata
 ///int ai;
 ev_id   e;           // counter for the synchronizing events

 //------------------------------------------------------- Computes automata 'to_change'
 RSS::rss.get_dep(to_change);        // it gets the parameters of the reachability func.
 known.create(FTB::ftb.aut());       // it gets the grouped automata (id's according to
 known.take_out();                   //   the original number of automata)
 state_map[g].get(known);
 to_change.minus(known);             // it takes automata in 'g' out

 //--------------------------------------------------- Defines which states must be kept
 to_keep.create(state_map[g].size());       // it allocates the 'to_keep' vector
 to_keep.take();
 s.create(FTB::ftb.aut());                  // it allocates the global state
 s.reset();
 for (i=0; i<state_map[g].size(); i++) {    // for all states of automaton 'g'
   state_map[g].get(i, s);                     // it gets its corresp. original states
   if (s.verify(to_change))                    // if at least one of the global states
                                                  // containing 's' is reachable
     to_keep.put(i);                              // it keeps the state 'i'
   else                                        // else
     RSS::rss.reduce(s, known);                   // it takes the global states 
 }                                                // containing 's' from 'RSS::rss'

 //--------------------------------- Reduces the structures concerning the automaton 'g'
 state_map[g].reduce(to_keep);               // it reduces the state map
 local_mat[g].reduce(to_keep);               // it reduces the local matrix
 for (e=0; e<events; e++) {                  // for all events
   synch_mat[e][g].reduce(to_keep);             // it reduces the synch. matrices
   synch_adj[e][g].reduce(to_keep);
 }
 sizes.put(g, to_keep.has());                // it saves the new automaton size 
}
      //--------------------------------------------------------------------------//
      //            Internal Functions of the Main Function Agregation            //
      //--------------------------------------------------------------------------//
      //          It eliminates synchronizing events that become local            //
      //                      (to a grouped automaton)                            //
      //--------------------------------------------------------------------------//
      // Algorithm Priciple: All event that has just one non-identity matrix at   //
      // each tensor product factor is no longer synchronizing.                   //
      //--------------------------------------------------------------------------//
      // Algorithm: - If a sync. event can become local to an automaton, choose   //
      //                this automaton as the event destination (if an event can  //
      //                not become local, its destination is 'no_aut');           //
      //            - For each event which can be eliminated, it adds the sync.   //
      //                matrices to the local matrix of the destination group;    //
      //            - Removes the eliminated events matrices;                     //
      //--------------------------------------------------------------------------//
void DSC::eliminate_events()
{
 ev_id   * dest;        // destination of each synch. event
 san_mat * m_plus;      // temporaries for the matrices to be added to local matrices
 san_mat * m_minus;
 int       n;           // number of matrices to be added to local matrices
 aut_id  * new_sr;      // new sync. rates
 aut_id    a;           // counter for the automata
 ev_id     e;           // counter for the old events (all events)
 ev_id     new_e = 0;   // counter for the new events (only the non-eliminated events)

 dest   = new ev_id[events];
 new_sr = new aut_id[events];

 //--------------------------------------------------- Chooses a sync. event destination
 for (e=0; e<events; e++) {                     // For all events:
   dest[e] = no_aut;                               // it seraches for the first
   for (a=0; a<automata; a++)                      // non-identity matrix for
     if ((synch_mat[e][a].tp() != Identity) ||     // event 'e' + and - matrices
         (synch_adj[e][a].tp() != Identity)) {
       dest[e] = a;                                  // the first non-identity matrix
       break;                                        // becomes the event destination
     }
   for (a++; a<automata; a++)                      // if another non-identity matrix is
     if ((synch_mat[e][a].tp() != Identity) ||     // found, destination is 'no_aut'
         (synch_adj[e][a].tp() != Identity)) {
       dest[e] = no_aut;
       break;
     }
 }
 //------------------------------------ Adds the sync. matrices events to a local matrix
 for (a=0, n=0; a<automata; a++) {         // For all local matrices:
   for (e=0; e<events; e++)                   // it counts how many events
     if (dest[e] == a)                        //   have this destination
       n++;
   if (n != 0) {                           // if at least one event has this destination
     m_plus  = new san_mat[n];                // it allocates the temporary matrices
     m_minus = new san_mat[n];
     for (e=0, n=0; e<events; e++)            // it copies the matrices to add
       if (dest[e] == a) {
         m_plus[n].copy(synch_mat[e][a]);
         m_minus[n].copy(synch_adj[e][a]);
         n++;
       }
     local_mat[a].add(m_plus, m_minus, n);    // it adds the event matrices (creating
                                              //   new functions in the 'scrft::sft')
     delete[] m_plus;                         // it desallocates the temporary matrices
     delete[] m_minus;
   }
   else                                    // if no event has this destination
     local_mat[a].assign();                   // it copies the local matrix (copying
                                              //   the function in the 'scrft::sft')
 }
 //------------------------------------------------- Removes the events with destination
 for (e=0; e<events; e++)                    // For all events:
   if (dest[e] == no_aut) {                     // if it has no destination
     if (e != new_e)                               // if a previous event has been
       for (a=0; a<automata; a++) {                   // removed, it copies its matrices
         synch_mat[new_e][a].assign(synch_mat[e][a]); // (using "assign" to insert
         synch_adj[new_e][a].assign(synch_adj[e][a]); //  the new func. id's into scr_ft)
       }
     else                                          // if the matrices remain in the same
       for (a=0; a<automata; a++) {                   // place, it just inserts the new
         synch_mat[new_e][a].assign();                // func. id's into scr_ft using
         synch_adj[new_e][a].assign();                // "assign"
       }
     new_sr[new_e] = FTB::ftb.g_aut(sync_rates[e]); // it changes its sync_rates
     new_e++;                                       //   according to the new aut. id's
   }
 for (e=new_e; e<events; e++)                // after all non-eliminated events where
   for (a=0; a<automata; a++) {              //   relocated
     synch_mat[e][a].erase();                // it erases the rest of the event matrices
     synch_adj[e][a].erase();
 }

 events = new_e;                            // it saves the new number of events
 delete[] sync_rates;                       // it saves the new 'sync_rates'
 sync_rates = new_sr;
 delete[] dest;
}

      //--------------------------------------------------------------------------//
      //            Internal Functions of the Main Function Agregation            //
      //--------------------------------------------------------------------------//
      //          It evaluates the functional elements that are constants         //
      //--------------------------------------------------------------------------//
      //  Note: A functional element is a constant (after grouping), iff it is    //
      // evaluated with the same value on ALL state space.                        //
      //--------------------------------------------------------------------------//
void DSC::solve_functions()
{
 aut_id a;
 ev_id  e;

 for (a=0; a<automata; a++)                      // for all local matrices
   local_mat[a].solve_functions(state_map[a]);      // it attempts to evaluate functions
 for (e=0; e<events; e++)                        // for all synch. events matrices
   for (a=0; a<automata; a++) {
     synch_mat[e][a].solve_functions(state_map[a]); // it attempts to evaluate functions
     synch_adj[e][a].solve_functions(state_map[a]);
   }
}


      //--------------------------------------------------------------------------//
      //                    Internal Function of allocation                       //
      //--------------------------------------------------------------------------//
void DSC::alloc()
{
 aut_id a;
 ev_id  e;

 sizes.create(automata);                                             // sizes
 local_mat = new san_mat[automata];                                  // local_mat
 for (a=0; a<automata; a++)
   local_mat[a].create();
 synch_mat = new san_mat*[events];                                   // synch_mat
 synch_adj = new san_mat*[events];                                   // synch_adj
 for (e=0; e<events; e++) {
   synch_mat[e] = new san_mat[automata];
   for (a=0; a<automata; a++)
     synch_mat[e][a].create();
   synch_adj[e] = new san_mat[automata];
   for (a=0; a<automata; a++)
     synch_adj[e][a].create();
 }
 state_map = new st_map[automata];                                   // state_maps
 for (a=0; a<automata; a++)
   state_map[a].create();
 sync_rates = new aut_id[events];                                    // sync_rates
 symetric.create(automata);                                          // symetric
}

      //--------------------------------------------------------------------------//
      //                Internal Function of tranfer contents                     //
      //--------------------------------------------------------------------------//
void DSC::trans(const DSC & d)
{
 aut_id a;
 ev_id  e;

 sizes.copy(d.sizes);                                                   // sizes
 for (a=0; a<automata; a++)                                             // local_mat
   local_mat[a].copy(d.local_mat[a]);
 for (e=0; e<events; e++)                                               // synch_mat
   for (a=0; a<automata; a++) {
     synch_mat[e][a].copy(d.synch_mat[e][a]);
     synch_adj[e][a].copy(d.synch_adj[e][a]);
   }
 for (a=0; a<automata; a++)                                             // state_maps
   state_map[a].copy(d.state_map[a]);
 for (e=0; e<events; e++)                                               // sync_rates
   sync_rates[e] = d.sync_rates[e];
 symetric.copy(d.symetric);                                             // symetric
}


      //--------------------------------------------------------------------------//
      //                  Internal Function of desallocation                      //
      //--------------------------------------------------------------------------//
void DSC::desal()
{
 aut_id a;
 ev_id  e;

 symetric.erase();                                                   // symetric
 delete[] sync_rates;                                                // sync_rates
 for (a=0; a<automata; a++)                                          // state_maps
   state_map[a].erase();
 delete[] state_map;
 for (e=0; e<events; e++) {                                          // synch_mat
   for (a=0; a<automata; a++) {                                      // synch_adj
     synch_mat[e][a].erase();
     synch_adj[e][a].erase();
   }
   delete[] synch_mat[e];
   delete[] synch_adj[e];
 }
 delete[] synch_mat;
 delete[] synch_adj;
 for (a=0; a<automata; a++)                                          // local_mat
   local_mat[a].erase();
 delete[] local_mat;
 sizes.erase();                                                      // sizes
}

