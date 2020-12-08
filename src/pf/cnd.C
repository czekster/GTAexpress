//====================================================================================//
//                                                                                    //
//                   Continuous-Time Normalized Descriptor Structure                  //
//                                                                                    //
//====================================================================================//
//  This File:   cnd.C                      Language: C++  (xlC and CC)               //
//  Software:    PEPS 2002                   Advisor: Brigitte.Plateau@imag.fr        //
//  Error Codes: 5450                                                                 //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 07/nov/02                        by: paulof@inf.pucrs.br             //
//====================================================================================//
#include "peps.h"

     //--------------------------------------------------------------------------//
     //                          Static variable                                 //
     //--------------------------------------------------------------------------//
CND CND::cnd = CND();
int  *CND::fcuts;

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
rp CND::coord(const aut_st & i, const aut_st & j)
{
#ifdef _PEPS_DEBUG_
  if ((i.size() != automata) || (j.size() != automata))
    Programming_Error("wrong global state size", 5450)
#endif
 
 aut_st state;             // original global state corresponding to 'i'
 aut_id a;                 // counter for the descriptor automata
 ev_id  e;                 // counter for the descriptor events
 rp     answer = 0;        // accumulator for the result
 rp     ev_plus;           // temporary to store the entry 'i,j' for the event 'e'
 bool   d_element = true;  // 'true' if it is an diagonal element
 aut_id mark = no_aut;     // the automaton that moves in the transition 'i' to 'j'
                           // if no automata moves or more than one moves this variable
                           // has the value 'no_aut'

 //----------------------------- Computes the original global state corresponding to 'i'
 state.create(FTB::ftb.aut());    // it creates an original global state vector
 for (a=0; a<automata; a++)       // for all automata of the descriptor
   state_map[a].get(i.st(a), state); // it gets the corresp. states from its 'state_map'
 
 //---------------------------------- Computes the entry 'i,j' for the local transitions
 for (a=0; a<automata; a++) {                       // for all local terms
   if (i.st(a) != j.st(a))                             // if automaton 'a' does move
     if (mark == no_aut) {                                // if it is the first aut.
       mark = a;                                             // to move mark it and
       d_element = false;                                     // it is not an diagonal
     }                                                        // element of the descriptor
     else {                                               // if another already moved
       mark = no_aut;                                        // the answer is zero
       answer = zero;                                        // (for the local part)
       break;                                                // stops the loop
     }
 }
 if (mark != no_aut)                                 // if only one automaton moves
   answer = local_mat[mark].coord(i.st(mark), j.st(mark), state);  // the answer is its
                                                                   // moving rate
 if (d_element)                                      // if no automata moves
   answer = diagonal[i.rank(sizes)];                               // the answer is a
                                                                   // diagonal element
 //--------------------------------- Computes the entry 'i,j' for the synch. transitions
 for (e=0; e<events; e++) {             // for each synchronizing event 'e'
   ev_plus  = 1;                           // accumulator for the matrices +
   for (a=0; a<automata; a++) {            // iterative multiplication for all automata
     ev_plus  *= synch_mat[e][a].coord(i.st(a), j.st(a), state);
   }
   answer += ev_plus;                      // it adds temporary accumulator
 }                                         //   to the answer
 return(answer);
}

     //--------------------------------------------------------------------------//
     //                     Output on 'fout' file stream                         //
     //--------------------------------------------------------------------------//
void CND::write() const
{
 ofstream fout;
 aut_id   a;
 ev_id    e;
 int      s;

 Open_File(file_name, cnd_file, fout);
 fout << automata << " " << events << " "   // dimensions
      << state_space << " " << reach_space << "\n";
 fout << orig_name << "\n";                 // original name
 fout << factor << "\n";                    // normalizing factor
 sizes.write(fout);                         // automata sizes
 symetric.write(fout);                      // automata labels
 orig_jumps.write(fout);                    // original jumps
 for (a=0; a<automata; a++)                 // automata state maps
   state_map[a].write(fout);

 for (a=0; a<automata; a++) {
   fout << int(l_type[a])  << "\n";         // local tensor sum term types
   fout << int(lf_type[a]) << "\n";         // local normal factor types
   l_B_order[a].write(fout);                // local normal factor orders (B method)
   l_B_jumps[a].write(fout);                // local normal factor jumps  (B method)
   l_C_order[a].write(fout);                // local normal factor orders (C method)
   l_C_jumps[a].write(fout);                // local normal factor jumps  (C method)
   fout << l_C_left[a]  << " "              // local normal factor nleft  (C method)
        << l_C_right[a] << " "              // local normal factor nright (C method)
        << l_C_fst[a]   << "\n";            // local normal factor first  (C method)
 }
 for (e=0; e<events; e++) {  
   fout << int(s_type[e]) << "\n";          // sync. tensor product term types
   s_B_order[e].write(fout);                // sync. tensor product orders
   s_B_jumps[e].write(fout);                // sync. tensor product jumps
   for (a=0; a<automata; a++) {
     fout << int(sf_type[e][a]) << "\n";       // sync. normal factor types
     s_C_order[e][a].write(fout);              // sync. normal factor orders
     s_C_jumps[e][a].write(fout);              // sync. normal factor jumps
     fout << s_C_left[e][a]  << " "            // sync. normal factor nleft  (C method)
          << s_C_right[e][a] << " "            // sync. normal factor nright (C method)
          << s_C_fst[e][a]   << "\n";          // sync. normal factor first  (C method)
   }
   s_D_order[e].write(fout);                // sync. tensor product decomp. orders
 }
 for (a=0; a<automata; a++)                 // local matrices
   local_mat[a].write(fout);
 for (e=0; e<events; e++)                   // sync. matrices
   for (a=0; a<automata; a++)
     synch_mat[e][a].write(fout);
 if (PRF::prf.vec_impl() == v_full) 
   for (s=0; s<state_space; s++)              // diagonal elements
     fout << diagonal[s] << "\n";
 else
   for (s=0; s<reach_space; s++)              // diagonal elements
     fout << diagonal[s] << "\n";
 Close_File(file_name, cnd_file, fout);
}
     //--------------------------------------------------------------------------//
     //                     Input from 'fin' file stream                         //
     //--------------------------------------------------------------------------//
void CND::read(const peps_name file)
{
 ifstream fin;
 aut_id   a;
 ev_id    e;
 int      s, r;

 Open_File(file, cnd_file, fin);
 fin >> a;                                  // dimensions
 fin >> e;
 fin >> s;
 fin >> r;
 create(a, e, s, r);
 baptise(file);
 fin >> orig_name;                          // original name
 fin >> factor;                             // normalizing factor
 sizes.read(fin);                           // automata sizes
 symetric.read(fin);                        // automata labels
 orig_jumps.read(fin);                      // original jumps
 for (a=0; a<automata; a++)                 // automata state maps
   state_map[a].read(fin);

 for (a=0; a<automata; a++) {
   fin >> s; l_type[a]  = term_typ(s);      // local tensor sum term types
   fin >> s; lf_type[a] = fact_typ(s);      // local normal factor types
   l_B_order[a].read(fin);                  // local normal factor orders (B method)
   l_B_jumps[a].read(fin);                  // local normal factor jumps  (B method)
   l_C_order[a].read(fin);                  // local normal factor orders (C method)
   l_C_jumps[a].read(fin);                  // local normal factor jumps  (C method)
   fin >> l_C_left[a];                      // local normal factor nleft  (C method)
   fin >> l_C_right[a];                     // local normal factor nright (C method)
   fin >> l_C_fst[a];                       // local normal factor first  (C method)
 }
 for (e=0; e<events; e++) {  
   fin >> s; s_type[e]  = term_typ(s);      // sync. tensor product term types
   s_B_order[e].read(fin);                  // sync. tensor product orders
   s_B_jumps[e].read(fin);                  // sync. tensor product jumps
   for (a=0; a<automata; a++) {
     fin >> s; sf_type[e][a] = fact_typ(s);    // sync. normal factor types
     s_C_order[e][a].read(fin);                // sync. normal factor orders
     s_C_jumps[e][a].read(fin);                // sync. normal factor jumps
     fin >> s_C_left[e][a];                    // sync. normal factor nleft  (C method)
     fin >> s_C_right[e][a];                   // sync. normal factor nright (C method)
     fin >> s_C_fst[e][a];                     // sync. normal factor first  (C method)
   }
   s_D_order[e].read(fin);                  // sync. tensor product decomp. orders
 }
 for (a=0; a<automata; a++)                 // local matrices
   local_mat[a].read(fin);
 for (e=0; e<events; e++)                   // sync. matrices
   for (a=0; a<automata; a++)
     synch_mat[e][a].read(fin);

 if (PRF::prf.vec_impl() == v_full)
   for (s=0; s<state_space; s++)              // diagonal elements
     fin >> diagonal[s];
 else
   for (s=0; s<reach_space; s++)              // diagonal elements
     fin >> diagonal[s];
 Close_File(file_name, cnd_file, fin);
}
     //--------------------------------------------------------------------------//
     //                      Print on 'fout' file stream                         //
     //--------------------------------------------------------------------------//
void CND::print() const
{
 peps_name descriptor_name, model_name, bufname;
 ofstream  fout;
 aut_st    bufsizes;
 aut_id    a, aux;
 ev_id     e;
 ///int       s;

  Provide_CND();

Get_Only_the_File_Name(file_name, descriptor_name);
 DCT::dct.n_name(model_name);
 FTB::ftb.get_domain(bufsizes);
 Open_File("cnd", dbg_file, fout);
 fout << "===============================================================\n"
      << "File cnd.dbg\n"
      << "===============================================================\n"
      << descriptor_name << ".cnd -- A model with " << bufsizes.size()
      << " original automata (" << automata << " after aggregation)\n"
      << descriptor_name << ".cnd -- A model with " << events
      << " original events (" << events << " after span)"
      << "\nUser given name: '" << model_name << "' (file '" << orig_name << ".san)"
      << "\n------------\nGeneral Information\n------------"
      << "\nProduct state space:                     " << RSS::rss.orig_size() << " states"
      << "\nState space after aggregation:           " << RSS::rss.size()      << " states"
      << "\nReachable state space:                   " << RSS::rss.reach_size() << " states"
      << "\nComputational cost (using diagonal):     " 
      << "\n  Local matrices:                        " << CND::l_cost() << " flops"
      << "\n  Synchronized matrices:                 " << CND::s_cost() << " flops"
      << "\n                                         " << "----------------------"
      << "\n                                         " << CND::cost() << " flops"
      << "\nAlternative comp. cost (using diagonal): " 
      << "\n  Local matrices:                        " << CND::l_acost() << " flops"
      << "\n  Synchronized matrices:                 " << CND::s_acost() << " flops"
      << "\n                                         " << "----------------------"
      << "\n                                         " << CND::acost() << " flops"
      << "\nAutomata sizes:                           [ ";
 for (a=0; a<bufsizes.size(); a++)
   fout << bufsizes.sz(a) << " ";
 fout << "]\nAutomata attribution after aggregation:   [ ";
 FTB::ftb.get_currents(bufsizes);
 for (a=0; a<bufsizes.size(); a++)
   fout << bufsizes.sz(a) << " ";
 fout << "]\nAutomata sizes after aggregation:         [ ";
 for (a=0; a<sizes.size(); a++)
   fout << sizes.sz(a) << " ";
 fout << "]\nSymetric labels for the current automata: [ ";
 for (a=0; a<symetric.size(); a++)
   fout << symetric.val(a) << " ";
 fout << "]\nCurrent Number of Functions: " << FTB::ftb.size()
      << "\nSize of the Normalized Descriptor: "
      << mem_use() / mem_unit << " Kbytes"
      << "\nNormalizing Factor: " << factor
      << "\n===============================================================\n";
 fout << "Types and Orders of the Local Terms\n------------";
 // ----------------------------------------------------------------------------//
 FTB::ftb.get_currents(bufsizes);
 for (a=0; a<automata; a++) {
   fout << "\nGroup " << a << " - Original Automata: < ";
   for (aux=0; aux<bufsizes.size(); aux++)   // for all original automata
     if (a == bufsizes.sz(aux)) {            //   if it belongs to the group "a"
       DCT::dct.a_name(aux, bufname);        //     prints its name
        fout << bufname << " ";
     }
   fout << ">\nCurrent local matrix type: ";
   switch (l_type[a]) {
     case idnt_term : fout << "identity";
                      if (lf_type[a] != idnt_mat)
                        fout << "\n** term and matrix types mismatch - PEPS programming error **";
                      break;
     case null_term : fout << "empty";
                      if (lf_type[a] != null_mat)
                        fout << "\n** term and matrix types mismatch - PEPS programming error **";
                      break;
     case cnst_term : fout << "non-functional matrix";
                      if (lf_type[a] != cnst_mat)
                        fout << "\n** term and matrix types mismatch - PEPS programming error **";
                      break;
     case A_term    : fout << "functional and fully dependable (method A)";
                      if (lf_type[a] != func_mat)
                        fout << "\n** term and matrix types mismatch - PEPS programming error **";
                      break;
     case B_term    : fout << "functional and partially dependable (method B)";
                      if (lf_type[a] != func_mat)
                        fout << "\n** term and matrix types mismatch - PEPS programming error **";
                      break;
     case C_term    : 
     default        : fout << "\n** unexpectable type - PEPS programming error **";
   }
   fout << "\n - Order for this term (normal factor) using multiplication method B:"
        << "\n---------------------------------------------------------------------\n";
   for (aux=0; aux<l_B_order[a].has(); aux++) {
     fout << "Current Group " << l_B_order[a].aut(aux) << " <====== ";
     if (a == l_B_order[a].aut(aux)) {
       switch (l_type[a]) {
         case idnt_term : fout << "Identity";   break;
         case null_term : fout << "Null";       break;
         case cnst_term : fout << "Constant";   break;
         default        : fout << "Functional";
       }
     }
     else
       fout << "Identity";
     fout << " Matrix\n";
   }
 }
 if (events != 0) {
   fout << "\n------------\nTypes and Orders of the Synchronized Terms\n------------";
   for (e=0; e<events; e++) {  
     DCT::dct.e_name(e, bufname);
     fout << "\nSynchronized Event " << bufname << " term type: ";
     switch (s_type[e]) {
       case idnt_term : fout << "identity (product of only identity matrices)";   break;
       case null_term : fout << "empty (one empty matrix in the product)";        break;
       case cnst_term : fout << "constant term (no functional matrices)";         break;
       case B_term    : fout << "functional and in an optimal order (method B)";  break;
       case C_term    : fout << "functional and needing re-ordering (method C)";  break;
  //   case A_term    : 
       default        : fout << "\n** unexpectable type - PEPS programming error **";
     }
     DCT::dct.e_name(e, bufname);
     fout << "\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n"
          << "Decomposition Order for event " << bufname << " for all methods"
          << "\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
     for (aux=0; aux<s_D_order[e].has(); aux++) {
       fout << "Current Group " << s_D_order[e].aut(aux) << " <====== ";
       switch (sf_type[e][s_D_order[e].aut(aux)]) {
         case idnt_mat : fout << "Identity";   break;
         case null_mat : fout << "Null";       break;
         case cnst_mat : fout << "Constant";   break;
         default       : fout << "Functional";
       }
       fout << " Matrix\n";
     }
     fout << "\nOrder for event " << bufname << " term using multiplication method B:"
          << "\n------------------------------------------------------------\n";
     for (aux=0; aux<s_B_order[e].has(); aux++) {
       fout << "Current Group " << s_B_order[e].aut(aux) << " <====== ";
       switch (sf_type[e][s_D_order[e].aut(aux)]) {
         case idnt_mat : fout << "Identity";   break;
         case null_mat : fout << "Null";       break;
         case cnst_mat : fout << "Constant";   break;
         default       : fout << "Functional";
       }
       fout << " Matrix\n";
     }
     for (a=0; a<automata; a++) {
       DCT::dct.a_name(a, bufname);
       fout << "\nOrder for the " << a << "-th normal factor using multiplication method C:"
            << "\n---------------------------------------------------------------\n";
       for (aux=0; aux<s_C_order[e][a].has(); aux++) {
         fout << "Current Group " << s_C_order[e][a].aut(aux) << " <====== ";
         if (a == s_C_order[e][a].aut(aux))
           switch (sf_type[e][a]) {
             case idnt_mat : fout << "Identity";   break;
             case null_mat : fout << "Null";       break;
             case cnst_mat : fout << "Constant";   break;
             default       : fout << "Functional";
           }
         else
           fout << "Identity";
         fout << " Matrix\n";
       }
     }
   }
 }
 fout << "\n------------\nLocal Matrices\n------------\n";
 FTB::ftb.get_currents(bufsizes);
 for (a=0; a<automata; a++) {                // local matrices
   fout << "Group " << a << " - Original Automata: < ";
   for (aux=0; aux<bufsizes.size(); aux++)   // for all original automata
     if (a == bufsizes.sz(aux)) {            //   if it belongs to the group "a"
       DCT::dct.a_name(aux, bufname);        //     prints its name
        fout << bufname << " ";
     }
   fout << ">\n";
   local_mat[a].print(fout);
 }
 if (events != 0) {
   fout << "\n------------\nSynch Events Matrices\n------------\n";
   for (e=0; e<events; e++)                   // sync. matrices
     for (a=0; a<automata; a++) {
       DCT::dct.e_name(e, bufname);
       // include the verification of which event it corresponds ...
       fout << "Event " << bufname << " Group " << a << " - Original Automata: < ";
       for (aux=0; aux<bufsizes.size(); aux++)   // for all original automata
         if (a == bufsizes.sz(aux)) {            //   if it belongs to the group "a"
           DCT::dct.a_name(aux, bufname);        //     prints its name
           fout << bufname << " ";
       }
       fout << ">\n";
       synch_mat[e][a].print(fout);
     }
 }
 // fout << "\n------------\nJumps\n------------";
 // orig_jumps.write(fout);                    // original jumps
 // fout << "\n------------\nState Maps\n------------";
 // for (a=0; a<automata; a++)                 // automata state maps
 //   state_map[a].write(fout);
 // fout << "\n------------\nDiagonal Elements\n------------";
 // for (s=0; s<state_space; s++)              // diagonal elements
 //   fout << diagonal[s] << "\n";
 Close_File("cnd", dbg_file, fout);
}
     //--------------------------------------------------------------------------//
     //                 Print on 'fout' file stream (diagonal)                   //
     //--------------------------------------------------------------------------//
void CND::dprint() const
{
 peps_name descriptor_name, model_name;
 ///peps_name bufname;
 ofstream  fout;
 aut_st    bufsizes;
 aut_id    a;
 ///aut_id    aux;
 ///ev_id     e;
 int       s;

  Provide_CND();

Get_Only_the_File_Name(file_name, descriptor_name);
 DCT::dct.n_name(model_name);
 FTB::ftb.get_domain(bufsizes);
 Open_File("diag", dbg_file, fout);
 fout << "===============================================================\n"
      << "File diag.dbg\n"
      << "===============================================================\n"
      << descriptor_name << ".cnd -- A model with " << bufsizes.size()
      << " original automata (" << automata << " after aggregation)\n"
      << descriptor_name << ".cnd -- A model with " << events
      << " original events (" << events << " after span)"
      << "\nUser given name: '" << model_name << "' (file '" << orig_name << ".san)"
      << "\n------------\nGeneral Information\n------------"
      << "\nProduct state space:           " << RSS::rss.orig_size() << " states"
      << "\nState space after aggregation: " << RSS::rss.size()      << " states"
      << "\nReachable state space:         " << RSS::rss.reach_size() << " states"
      << "\nAutomata sizes:                           [ ";
 for (a=0; a<bufsizes.size(); a++)
   fout << bufsizes.sz(a) << " ";
 fout << "]\nAutomata attribution after aggregation:   [ ";
 FTB::ftb.get_currents(bufsizes);
 for (a=0; a<bufsizes.size(); a++)
   fout << bufsizes.sz(a) << " ";
 fout << "]\nAutomata sizes after aggregation:         [ ";
 for (a=0; a<sizes.size(); a++)
   fout << sizes.sz(a) << " ";
 fout << "]\nSymetric labels for the current automata: [ ";
 for (a=0; a<symetric.size(); a++)
   fout << symetric.val(a) << " ";
 fout << "]\nCurrent Number of Functions: " << FTB::ftb.size()
      << "\nSize of the Normalized Descriptor: "
      << mem_use() / mem_unit << " Kbytes"
      << "\nNormalizing Factor: " << factor;
 fout << "\n------------\nDiagonal Elements\n------------\n"
      << "[pos]       value        (original value)\n";

 if (PRF::prf.vec_impl() == v_full)
   for (s=0; s<state_space; s++)              // diagonal elements
     //   fout << "[" << s << "] " << diagonal[s] << " (" << diagonal[s] / factor << ")\n";
     fout << "[" << s << "] " << diagonal[s] << " ("
	  << rp(int(((diagonal[s] / factor)-0.005)*100))/100 << ")\n";
 else
   for (s=0; s<reach_space; s++)              // diagonal elements
     fout << "[" << s << "] " << diagonal[s] << " ("
	  << rp(int(((diagonal[s] / factor)-0.005)*100))/100 << ")\n";
 
 Close_File("diag", dbg_file, fout);
}

     //--------------------------------------------------------------------------//
     //              Memory space used by 'this' (in memory units)               //
     //--------------------------------------------------------------------------//
int CND::mem_use() const
{
 aut_id a;
 ev_id  e;
 int    answer = 20;

 answer += sizes.mem_use();
 answer += symetric.mem_use();
 answer += orig_jumps.mem_use();
 for (a=0; a<automata; a++) {
   answer += state_map[a].mem_use();
   answer += 4;
   answer += 4;
   answer += l_B_order[a].mem_use();
   answer += l_B_jumps[a].mem_use();
   answer += l_C_order[a].mem_use();
   answer += l_C_jumps[a].mem_use();
   answer += 4;
   answer += 4;
   answer += 4;
   answer += local_mat[a].mem_use();
 }
 for (e=0; e<events; e++) {
   answer += 4;
   answer += s_B_order[e].mem_use();
   answer += s_B_jumps[e].mem_use();
   for (a=0; a<automata; a++) {
     answer += 4;
     answer += s_C_order[e][a].mem_use();
     answer += s_C_jumps[e][a].mem_use();
     answer += 4;
     answer += 4;
     answer += 4;
     answer += synch_mat[e][a].mem_use();
   }
   answer += s_D_order[e].mem_use();
 }
 if (PRF::prf.vec_impl() == v_full)
   answer += state_space * 8;
 else
   answer += reach_space * 8;
   
 return(answer);
}

     //--------------------------------------------------------------------------//
     //                             Main Functions                               //
     //--------------------------------------------------------------------------//
     //                                                                          //
     //                        Normalizes the Descriptor                         //
     //                                                                          //
     //    Only Input Parameters:                                                //
     //                                                                          //
     //    - 'name'     the file name of the new structures (that are saved      //
     //                    after grouping);                                      //
     //    - 'DSC::dsc' the descriptor to be normalized;                         //
     //                                                                          //
     //    Input and Output Parameters:                                          //
     //                                                                          //
     //    - 'FTB::ftb' before: the old function table (referred by 'DSC::dsc'); //
     //                 after:  the new function table (referred by 'this');     //
     //                                                                          //
     //    Only Output Parameters:                                               //
     //                                                                          //
     //    - 'this'     the normalized descriptor (usually 'CND::cnd');          //
     //--------------------------------------------------------------------------//
void CND::translate(const peps_name name)
{
#ifdef _PEPS_DEBUG_
  if (DSC::dsc.type() != Continuous)
    Programming_Error("only continuous-time descriptors are accepted", 5451)
#endif
 create(DSC::dsc.aut(), DSC::dsc.ev(), DSC::dsc.ss(), RSS::rss.reach_size());
 copy_structures();
 generate_diagonal();
 scr_ft::sft.create(FTB::ftb.size());
 process_matrices();
 FTB::ftb.create(scr_ft::sft);
 scr_ft::sft.erase();
 generate_order_jumps();
 classify_factors();
 if (PRF::prf.method() == meth_S)
   reorganize_orders();
 strcpy(file_name, name);  write();
 FTB::ftb.baptise(name);   FTB::ftb.write();
 RSS::rss.baptise(name);   RSS::rss.write();
 FTB::ftb.write_jit();
}

      //--------------------------------------------------------------------------//
      //            Internal Functions of the Main Function Agregation            //
      //--------------------------------------------------------------------------//
      //        It copies the basic structures of the descriptor 'DSC::dsc'       //
      //                           WITHOUT ANY CHANGES                            //
      //--------------------------------------------------------------------------//
void CND::copy_structures()
{
 peps_name buf;
 aut_id a;
 ev_id  e;

 DSC::dsc.o_name(buf);                                   // original name
 strcpy(orig_name, buf);
 DSC::dsc.get_s(sizes);                                  // automata sizes
 for (a=0; a<automata; a++)
   DSC::dsc.get_mat(a, local_mat[a]);                    // local matrices
 for (e=0; e<events; e++)
   for (a=0; a<automata; a++){
     DSC::dsc.get_mat(e, a, Mat_plus, synch_mat[e][a]);  // synch. positive matrices
   }
 for (a=0; a<automata; a++)
   DSC::dsc.get_sm(a, state_map[a]);                     // state maps
 DSC::dsc.get_sms(symetric);                             // automata labels
}

      //--------------------------------------------------------------------------//
      //            Internal Functions of the Main Function Agregation            //
      //--------------------------------------------------------------------------//
      //  It generates all diagonal elements and computes the normalizing factor  //
      //--------------------------------------------------------------------------//
void CND::generate_diagonalF()
{
/* aut_st g_st;          // state vector to evaluate functions (current state space)
 rp     largest = 0;   // largest (in modulo) diagonal element of reachable states
 int    i;             // counter for the current global states

 g_st.create(automata);                     // creates the vector for local states
 g_st.reset();
 for (i=0; i<state_space; i++) {            // for all current global states
   diagonal[i] = DSC::dsc.coord(g_st, g_st);   // computes the diagonal element
   if (diagonal[i] < largest)                  // keeps the largest in modulo
     largest = diagonal[i];                    // (the largest is negative)
   g_st.next(sizes);                           // pass to the next global state
 }
 factor = -1 / largest;                     // computes the normalizing factor
 for (i=0; i<state_space; i++)              // scales all diagonal elements
   diagonal[i] *= factor;
*/

  aut_st g_st;          // state vector to evaluate functions (current state space)
  rp     largest = 0;   // largest (in modulo) diagonal element of reachable states
  int    o_i, c_i = 0;  // counter for the global states (originals and current)
  int    orig_state_space;  // original (product) state space

  orig_state_space = RSS::rss.orig_size();   // gets the original product state space
  g_st.create(automata);                     // creates the vector for local states
  g_st.reset();
  for (o_i=0; o_i<orig_state_space; o_i++) { // for all global states in the
                                             //   original product state space
    if (RSS::rss.remains(o_i)) {                // if this original is not eliminated
      diagonal[c_i] = DSC::dsc.coord(g_st, g_st);  // computes the diagonal element
      if (RSS::rss.has(o_i) ||                     // if this orig. state is reachable
          PRF::prf.is_partial_rss())               //     or if working on partial
	                                           //     reachable (look to all states)
	if (diagonal[c_i] < largest)                  // keeps the largest in modulo
	  largest = diagonal[c_i];                    // (the largest is negative)
      c_i++;                                       // pass to the next not eliminated
      g_st.next(sizes);                            //   global state
    }
  }
  factor = -1 / largest;                     // computes the normalizing factor
  for (c_i=0; c_i<state_space; c_i++) {      // scales all diagonal elements
    diagonal[c_i] *= factor;
  }
}

void CND::generate_diagonalS()
{
 aut_st g_st;          // state vector to evaluate functions (current state space)
 ///rp     diag;
 rp largest = 0;   // largest (in modulo) diagonal element of reachable states
 int    o_i, r_i = 0;  // counter for the global states (originals and reachable)
 int    orig_state_space;  // original (product) state space

 orig_state_space = RSS::rss.orig_size();   // gets the original product state space
 g_st.create(automata);                     // creates the vector for local states
 g_st.reset();

 for (o_i=0; o_i<orig_state_space; o_i++) { // for all global states in the
                                            //   original product state space
   if (RSS::rss.remains(o_i)) {                // if this original is not eliminated
     if (RSS::rss.has(o_i)) {                     // if this orig. state is reachable
       diagonal[r_i] = DSC::dsc.coord(g_st, g_st);   // computes the diagonal element
       if (diagonal[r_i] < largest)                  // keeps the largest in modulo
	 largest = diagonal[r_i];                    // (the largest is negative)
       r_i++;                                       // pass to the next reachable
     }
     g_st.next(sizes);                            // pass to the next global state
   }
 }
 factor = -1 / largest;                     // computes the normalizing factor
 for (r_i=0; r_i<reach_space; r_i++) {      // scales all diagonal elements
   diagonal[r_i] *= factor;
 }

 /*
 for (j=0; j<state_space; j++) {                       // for all current global states
   if (RSS::rss.has(j)) {
     diagonal[i] = DSC::dsc.coord(g_st, g_st);   // computes the diagonal element
     if (diagonal[i] < largest)                  // keeps the largest in modulo
       largest = diagonal[i];                    // (the largest is negative)
     i++;
   }
   else {
     diag = DSC::dsc.coord(g_st, g_st);
     if (diag < largest)
       largest = diag;
   }   
   g_st.next(sizes);                           // pass to the next global state
 }
 factor = -1 / largest;                     // computes the normalizing factor
 for (i=0; i<reach_space; i++)              // scales all diagonal elements
   diagonal[i] *= factor;
 */
}


void CND::generate_diagonal()
{
  if (PRF::prf.vec_impl() == v_full)
    generate_diagonalF();
  else
    generate_diagonalS();
}

      //--------------------------------------------------------------------------//
      //            Internal Functions of the Main Function Agregation            //
      //--------------------------------------------------------------------------//
      //       It scales one matrix of each tensor product term and removes       //
      //               the diagonal elements of the local matrices                //
      //--------------------------------------------------------------------------//
      // Obs.: If the descriptor ('DSC::dsc') was not grouped some functional     //
      // elements can (possibly) be replaced by constants. If a grouping process  //
      // was already done the number of functions is already the minimal.         //
      //--------------------------------------------------------------------------//
void CND::process_matrices()
{
 aut_id a;
 ev_id  e;

 for (a=0; a<automata; a++) {                   // for all local matrices
   local_mat[a].cut_diag();                        // removes diagonal elements
   if (!(FTB::ftb.is_grouped()))                   // if the descriptor was not grouped
     local_mat[a].solve_functions(state_map[a]);      // it tries to eliminate functions
   local_mat[a].scale(factor);                     // scales local matrices
 }
 
 for (e=0; e<events; e++)                       // for all synch. matrices
   for (a=0; a<automata; a++) {                    // if the descriptor was not grouped
     if (!(FTB::ftb.is_grouped()))                    // it tries to eliminate functions
       synch_mat[e][a].solve_functions(state_map[a]);
     if (a == DSC::dsc.get_sr(e))                  // scales the matrix of the master
       synch_mat[e][a].scale(factor);              //   automaton of the event 'e'
     else                                          // and just copies the matrices
       synch_mat[e][a].assign();                   //   of the slave automata
   }
}

      //--------------------------------------------------------------------------//
      //            Internal Functions of the Main Function Agregation            //
      //--------------------------------------------------------------------------//
      //   It verifies the cycles, finds the transversals, compute orders (for    //
      //     all multiplication methods) and calculates permutation jumps to      //
      //                          each computed order                             //
      //--------------------------------------------------------------------------//
void CND::generate_order_jumps()
{
 d_graph dgraph;
 aut_set dep;
 aut_id  a;
 ev_id   e;

 orig_jumps.fill(sizes);
 dgraph.create(automata);
 for (a=0; a<automata; a++) {
   dgraph.take();
   local_mat[a].params(dep);
   dep.take_out(a);
   dgraph.put(a, dep);
   dgraph.get_order(sizes, l_B_order[a]);
   l_B_jumps[a].fill(sizes, l_B_order[a]);

   l_C_order[a].create(dep.size());
   l_C_order[a].put(dep);
   l_C_order[a].fill_tail();
   l_C_order[a].mov_tail(l_C_order[a].pos(a));
   l_C_jumps[a].fill(sizes, l_C_order[a]);
   l_C_left[a]  = sizes.state_space(dep);
   l_C_right[a] = (state_space / l_C_left[a]) / sizes.sz(a);
   l_C_fst[a] = dep.has();
 }
 for (e=0; e<events; e++) {
   dgraph.take();
   for (a=0; a<automata; a++) {
     synch_mat[e][a].params(dep);
     dgraph.put(a, dep);
   }
   dgraph.get_order(sizes, s_B_order[e]);
   fcuts[e] = s_B_order[e].get_cut(); //for this case, I will need the last index, to know where to cut the tensor term for split
   s_B_jumps[e].fill(sizes, s_B_order[e]);
   dgraph.get_D_order(sizes, s_D_order[e]);
 }

 for (e=0; e<events; e++) {
   for (a=0; a<automata; a++) {
     synch_mat[e][a].params(dep);
     dep.take_out(a);
     s_C_order[e][a].create(dep.size());
     s_C_order[e][a].put(dep);
     s_C_order[e][a].fill_tail();
     s_C_order[e][a].mov_tail(s_C_order[e][a].pos(a));
     s_C_jumps[e][a].fill(sizes, s_C_order[e][a]);
     s_C_left[e][a]  = sizes.state_space(dep);
     s_C_right[e][a] = (state_space / s_C_left[e][a]) / sizes.sz(a);
     s_C_fst[e][a] = dep.has();
   }
 }
};

      //--------------------------------------------------------------------------//
      //      This method will reorder the s_B_order variable to use in the Split //
      //--------------------------------------------------------------------------//
void CND::reorganize_orders() const {
    // This is basically a heuristic to reorganize the orders in the s_B_order structure. If fully functional
    //   the method will just let shuffle deal with it; if it is a B_term, then it will a. call shuffle for all
    //   functional evaluations or b. call sparse for all functional evaluations. Lastly, if the term is a constant
    //   term, then it will order the matrices according to the following: most identities to shuffle and cut in the
    //   last constant matrix.
    for (int e = 0; e < CND::cnd.ev(); e++) {
        int type = CND::cnd.get_s_type(e);
        switch (CND::cnd.get_s_type(e)) {
            case idnt_term : //cout << "identity";
                break;
            case null_term : //cout << "empty";
                break;
            case cnst_term : { //cout << "constant";
               //it doesn't matter the strategy: for split, it is always good to send idnts to shuffle (if there are memory)
               //let the most identities to shuffle and tries to cut in the last constant matrix...
               aut_list l;
               int r = 0;
               l.create(automata);
               l.mov_idnt(CND::cnd.get_s_type(e), fcuts[e], e, s_B_order[e], r);
               fcuts[e] = r; // the new cut, only identities to shuffle
               s_B_order[e].copy(l);
               s_B_jumps[e].fill(sizes, s_B_order[e]);
            }
                break;
            case A_term    : //cout << "functional and fully dependable (method A)";
                //let only shuffle to be called for this term...
                fcuts[e] = 0;
                break;
            case B_term    : { //cout << "functional and partially dependable (method B)";
                switch(PRF::prf.strategy()) {      // switch strategy to deal with constant terms in the split approach: A
                   case strat_B: // let shuffle deals with the functional elements
                      if (fcuts[e] != 0) {
                         aut_list l;
                         l.create(automata);
                         l.invert(fcuts, e, s_B_order[e]); //puts dependendable matrices to shuffle and non-dependable to sparse
                         fcuts[e] = abs(fcuts[e] - automata); //this cut value is the limit
                         s_B_order[e].copy(l);
                      }
                      break;
                   case strat_A: // if strategy A was selected for FUNCTIONAL terms, another one must me used, such as strategy C for example...
                   case strat_C:
                      //split is set by default with this option when it decides WHERE to cut the tensor term (look method generate_order_jumps)
                      s_B_jumps[e].fill(sizes, s_B_order[e]);
                      break;
                   case strat_D: // evals in split, sending all constant matrices in the shuffle part to the sparse part
                      if (fcuts[e] != 0) {
                         aut_list l;
                         int r = 0;
                         l.create(automata);
                         l.copy(s_B_order[e]);
                         l.mov_cnst(CND::cnd.get_s_type(e), fcuts[e], e, l, r); //move constant matrices to sparse (leave only identities to shuffle)
                         fcuts[e] = r;
                         s_B_order[e].copy(l);
                      }
                      break;
                   case strat_E: // only functional definition and identities in the shuffle part
                      if (fcuts[e] != 0) {
                         aut_list l;
                         int r = 0;
                         l.create(automata);
                         l.invert(fcuts, e, s_B_order[e]); //puts dependendable matrices to shuffle and non-dependable to sparse
                         fcuts[e] = abs(fcuts[e] - automata); //this cut value is the limit
                         l.mov_f_idnt(CND::cnd.get_s_type(e), fcuts[e], e, l, r);
                         fcuts[e] = r;
                         s_B_order[e].copy(l);
                      }
                      break;
                }
                s_B_jumps[e].fill(sizes, s_B_order[e]);
            }
                break;
            case C_term    :
            default        : //cout << endl << "** unexpectable type - PEPS programming error **";
                break;
        }
    }
}

      //--------------------------------------------------------------------------//
      //            Internal Functions of the Main Function Agregation            //
      //--------------------------------------------------------------------------//
      //      It chooses a multiplication method to each tensor product term      //
      //--------------------------------------------------------------------------//
void CND::classify_factors()
{
 aut_id  a;
 ev_id   e;
 bool    all_identity;
 bool    one_null;
 bool    all_constant;
 bool    optimal_order;
 aut_set param;
 aut_set before;

 for (a=0; a<automata; a++)
   if (!(local_mat[a].is_functional()))
     if (local_mat[a].nz() == 0) {
       l_type[a]  = null_term;
       lf_type[a] = null_mat;
     }
     else
       if (local_mat[a].tp() == Identity) {
         l_type[a]  = idnt_term;
         lf_type[a] = idnt_mat;
       }
       else {
         l_type[a]  = cnst_term;
         lf_type[a] = cnst_mat;
       }
   else if (local_mat[a].is_clique(a)) {
          l_type[a]  = A_term;
          lf_type[a] = func_mat;
        }
        else {
          l_type[a]  = B_term;
          lf_type[a] = func_mat;
        }

 param.create(automata);
 before.create(automata);
 for (e=0; e<events; e++) {
   all_identity  = true;
   one_null      = false;
   all_constant  = true;
   optimal_order = true;
   for (a=0; a<automata; a++)
     if (!(synch_mat[e][a].is_functional()))
       if (synch_mat[e][a].nz() == 0) {
         sf_type[e][a] = null_mat;
         one_null = true;
         all_identity  = false;
       }
       else
         if (synch_mat[e][a].tp() == Identity)
           sf_type[e][a] = idnt_mat;
         else {
           sf_type[e][a] = cnst_mat;
           all_identity  = false;
         }
     else {
       all_identity = false;
       all_constant = false;
       synch_mat[e][a].params(param);
       s_B_order[e].before(a, before);
       before.minus(param);
       if (!(before.is_empty()))
         optimal_order = false;
       sf_type[e][a] = func_mat;
     }
   if (one_null)
     s_type[e] = null_term;
   else if (all_identity)
          s_type[e] = idnt_term;
        else if (all_constant)
               s_type[e] = cnst_term;
             else if (optimal_order)
                    s_type[e] = B_term;
                  else
                    s_type[e] = C_term;
 }
};

      //--------------------------------------------------------------------------//
      //                    Internal Function of allocation                       //
      //--------------------------------------------------------------------------//
void CND::alloc()
{
 aut_id   a;
 ev_id    e;

 sizes.create(automata);                    // automata sizes
 symetric.create(automata);                 // automata labels
 orig_jumps.create(automata);               // original jumps
 state_map = new st_map[automata];          // automata state_maps
 for (a=0; a<automata; a++)
   state_map[a].create();

 local_mat  = new san_mat[automata];        // local matrices
 l_type     = new term_typ[automata];       // local tensor sum term types
 lf_type    = new fact_typ[automata];       // local normal factor types
 l_B_order  = new aut_list[automata];       // local normal factor orders (B method)
 l_B_jumps  = new jumps[automata];          // local normal factor jumps  (B method)
 l_C_order  = new aut_list[automata];       // local normal factor orders (C method)
 l_C_jumps  = new jumps[automata];          // local normal factor jumps  (C method)
 l_C_left   = new int[automata];            // local normal factor nleft  (C method)
 l_C_right  = new int[automata];            // local normal factor nright (C method)
 l_C_fst    = new int[automata];            // local normal factor first  (C method)
 for (a=0; a<automata; a++) {
   local_mat[a].create();
   l_B_order[a].create(automata);
   l_B_jumps[a].create(automata);
   l_C_order[a].create(automata);
   l_C_jumps[a].create(automata);
 }
 s_type     = new term_typ[events];         // sync. tensor product term types
 s_B_order  = new aut_list[events];         // sync. tensor product term orders
 s_B_jumps  = new jumps[events];            // sync. tensor product term jumps
 synch_mat  = new san_mat*[events];            // sync. matrices
 sf_type    = new fact_typ*[events];           // sync. normal factor types
 s_C_order  = new aut_list*[events];           // sync. normal factor orders
 s_C_jumps  = new jumps*[events];              // sync. normal factor jumps
 s_C_left   = new int*[events];                // sync. normal factor nleft  (C method)
 s_C_right  = new int*[events];                // sync. normal factor nright (C method)
 s_C_fst    = new int*[events];                // sync. normal factor first  (C method)
 s_D_order  = new aut_list[events];         // sync. tensor product term decomp. orders
 for (e=0; e<events; e++) {
   s_B_order[e].create(automata);
   s_B_jumps[e].create(automata);
   synch_mat[e] = new san_mat[automata];
   sf_type[e]   = new fact_typ[automata];
   s_C_order[e] = new aut_list[automata];
   s_C_jumps[e] = new jumps[automata];
   s_C_left[e]   = new int[automata];
   s_C_right[e]  = new int[automata];
   s_C_fst[e]    = new int[automata];
   for (a=0; a<automata; a++) {
     synch_mat[e][a].create();
     s_C_order[e][a].create(automata);
     s_C_jumps[e][a].create(automata);
   }
   s_D_order[e].create(automata);
 }

 if (PRF::prf.vec_impl() == v_full)
   diagonal = new rp[state_space];            // diagonal elements
 else
   diagonal = new rp[reach_space];

 fcuts = new int[CND::cnd.ev()];            // allocation of the functional cuts (Split)
}

void CND::add_aunf(aunf *vaunfs, int size) {
  int sz = 0;
  for (int i = 0; i < size; i++) {
      sz += sizeof(vaunfs[i]);
  }
  mem_aunfs += sz;
  tot_aunfs += size;
}

void CND::reset_aunf() {
  tot_aunfs = 0;
}

int CND::get_tot_aunfs() {
  return (tot_aunfs);
}

int CND::get_mem_aunfs() {
  return (mem_aunfs);
}

double CND::get_aunf_time() {
  return (aunf_time);
}

void CND::set_aunf_time(const double val) {
  aunf_time = val;
}

      //--------------------------------------------------------------------------//
      //                Internal Function of tranfer contents                     //
      //--------------------------------------------------------------------------//
void CND::trans(const CND & d)
{
 aut_id   a;
 ev_id    e;
 int      s;

 factor = d.factor;

 sizes.copy(d.sizes);                       // automata sizes
 symetric.copy(d.symetric);                 // automata labels
 orig_jumps.copy(d.orig_jumps);             // original jumps
 for (a=0; a<automata; a++)                 // automata state maps
   state_map[a].copy(d.state_map[a]);

 for (a=0; a<automata; a++) {
   local_mat[a].copy(d.local_mat[a]);       // local matrices
   l_type[a] = d.l_type[a];                 // local tensor sum term types
   lf_type[a] = d.lf_type[a];               // local normal factor types
   l_B_order[a].copy(d.l_B_order[a]);       // local normal factor orders (B method)
   l_B_jumps[a].copy(d.l_B_jumps[a]);       // local normal factor jumps  (B method)
   l_C_order[a].copy(d.l_C_order[a]);       // local normal factor orders (C method)
   l_C_jumps[a].copy(d.l_C_jumps[a]);       // local normal factor jumps  (C method)
   l_C_left[a]  = d.l_C_left[a];            // local normal factor nleft  (C method)
   l_C_right[a] = d.l_C_right[a];           // local normal factor nright (C method)
   l_C_fst[a]   = d.l_C_fst[a];             // local normal factor first  (C method)
 }
 for (e=0; e<events; e++) {
   s_type[e] = d.s_type[e];                 // sync. tensor product term types
   s_B_order[e].copy(d.s_B_order[e]);       // sync. tensor product term orders
   s_B_jumps[e].copy(d.s_B_jumps[e]);       // sync. tensor product term jumps
   for (a=0; a<automata; a++) {
     synch_mat[e][a].copy(d.synch_mat[e][a]);  // sync. matrices
     sf_type[e][a] = d.sf_type[e][a];          // sync. normal factor types
     s_C_order[e][a].copy(d.s_C_order[e][a]);  // sync. normal factor orders
     s_C_jumps[e][a].copy(d.s_C_jumps[e][a]);  // sync. normal factor jumps
     s_C_left[e][a]  = d.s_C_left[e][a];       // sync. normal factor nleft  (C method)
     s_C_right[e][a] = d.s_C_right[e][a];      // sync. normal factor nright (C method)
     s_C_fst[e][a]   = d.s_C_fst[e][a];        // sync. normal factor first  (C method)
   }
   s_D_order[e].copy(d.s_D_order[e]);       // sync. tensor product term decomp. orders
 }
 if (PRF::prf.vec_impl() == v_full)
   for (s=0; s<state_space; s++)              // diagonal elements
     diagonal[s] = d.diagonal[s];
 else
   for (s=0; s<reach_space; s++)              // diagonal elements
     diagonal[s] = d.diagonal[s];
}

      //--------------------------------------------------------------------------//
      //                  Internal Function of desallocation                      //
      //--------------------------------------------------------------------------//
void CND::desal()
{
 aut_id   a;
 ev_id    e;

 sizes.erase();                             // automata sizes
 symetric.erase();                          // automata labels
 orig_jumps.erase();                        // original jumps
 for (a=0; a<automata; a++)                 // automata state maps
   state_map[a].erase();

 for (a=0; a<automata; a++) {
   local_mat[a].erase();
   l_B_order[a].erase();
   l_B_jumps[a].erase();
   l_C_order[a].erase();
   l_C_jumps[a].erase();
 }
 delete[] local_mat;                        // local matrices
 delete[] l_type;                           // local tensor sum term types
 delete[] lf_type;                          // local normal factor types
 delete[] l_B_order;                        // local normal factor orders
 delete[] l_B_jumps;                        // local normal factor jumps
 delete[] l_C_order;                        // local normal factor orders
 delete[] l_C_jumps;                        // local normal factor jumps
 delete[] l_C_left;                         // local normal factor nleft
 delete[] l_C_right;                        // local normal factor nright
 delete[] l_C_fst;                          // local normal factor first

 for (e=0; e<events; e++) {
   s_B_order[e].erase();
   s_B_jumps[e].erase();
   for (a=0; a<automata; a++) {
     synch_mat[e][a].erase();
     s_C_order[e][a].erase();
     s_C_jumps[e][a].erase();
   }
   s_D_order[e].erase();
   delete[] synch_mat[e];
   delete[] sf_type[e];
   delete[] s_C_order[e];
   delete[] s_C_jumps[e];
   delete[] s_C_left[e];
   delete[] s_C_right[e];
   delete[] s_C_fst[e];
 }
 delete[] s_type;                           // sync. tensor product term types
 delete[] s_B_order;                        // sync. tensor product term orders
 delete[] s_B_jumps;                        // sync. tensor product term jumps
 delete[] synch_mat;                        // sync. matrices
 delete[] sf_type;                          // sync. normal factor types
 delete[] s_C_order;                        // sync. normal factor orders
 delete[] s_C_jumps;                        // sync. normal factor jumps
 delete[] s_C_left;                         // sync. normal factor nleft
 delete[] s_C_right;                        // sync. normal factor nright
 delete[] s_C_fst;                          // sync. normal factor first
 delete[] s_D_order;                        // sync. tensor product term decomp. orders

 delete[] fcuts;
}

