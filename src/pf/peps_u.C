//====================================================================================//
//                                                                                    //
//                                    PEPS Main                                       //
//                                                                                    //
//====================================================================================//
//  This File:   peps_u.C                   Language: C++  (xlC and CC)               //
//  Software:    PEPS 2002                   Advisor: Brigitte.Plateau@imag.fr        //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 21/oct/02                        by: paulof@inf.pucrs.br             //
//====================================================================================//
#include "peps.h"

//====================================================================================//
//   SAN compilation function prototype                                               //
//====================================================================================//
void Compile_Network(const peps_name san_level_name, const peps_name des_level_name);

//====================================================================================//
void Compile_SAN(bool aggr_too, bool norm_too)                  //   Main Operation   //
//====================================================================================//
{                          //     SAN model name in all directories handled by PEPS
 peps_name model_name;     // current directory (just the name, no paths, no extensions)
 peps_name san_level_name; // directory where the textual network  is      (./*.san)
 peps_name des_level_name; // directory to store the textual descriptor    (./des/*.des)
 peps_name dsc_level_name; // directory to store the internal descriptor   (./dsc/*.dsc)
 peps_name agg_level_name; // directory to store the aggregated descriptor (./agg/*.dsc)
 peps_name cnd_level_name; // directory to store the normalized descriptor (./cnd/*.cnd)

 Timer     T;              // timer to inform total time spent
 aut_st    agreg_map;      // aggregation map to perform aggregation
 bool      lu_too;         // the descriptor must be LU decomposed (generates a .lud)

 cout << "\nCompilation of a SAN model";
 T.clear();
 Ask_an_Existing_File_Name(san_level_name, san_file);
 Get_Only_the_File_Name(san_level_name, model_name);
 strcpy(des_level_name, "des/");  strcat(des_level_name, model_name);
 strcpy(dsc_level_name, "dsc/");  strcat(dsc_level_name, model_name);
 strcpy(agg_level_name, "agg/");
 strcpy(cnd_level_name, "cnd/");  
 if (Permission_to_Proceed(des_level_name, des_file)) {              // .san compilation
   T.start();
   cout << "Compile_Network\n";
   Compile_Network(san_level_name, des_level_name);
   T.stop();
   if (Permission_to_Proceed(dsc_level_name, dsc_file)) {            // .des compilation
     T.start();
     cout << "Compile_Function_Table\n";
     Compile_Function_Table        (des_level_name, dsc_level_name);
     cout << "Compile_Descriptor\n";
     Compile_Descriptor            (des_level_name, dsc_level_name);
     cout << "Compile_Reacheable_SS\n";
     Compile_Reacheable_SS         (des_level_name, dsc_level_name);
     cout << "Compile_Dictionary\n";
     Compile_Dictionary            (des_level_name, dsc_level_name);
     cout << "Compile_Integration_Function\n";
     Compile_Integration_Function  (des_level_name, dsc_level_name);
     T.stop();
     if (aggr_too) {                                                 // aggregation
       Ask_Agregation_Map(agreg_map);
       cout << "\nYou need a new file name to the aggregate model";
       Ask_a_File_Name(model_name, dsc_file);
       strcat(agg_level_name, model_name);
       strcat(cnd_level_name, model_name);
       if (Permission_to_Proceed(agg_level_name, dsc_file)) {
         T.start();
         DSC::dsc.aggregate(agreg_map, agg_level_name);
         T.stop();
       }
     }
     else
       strcat(cnd_level_name, model_name);
     if (norm_too)                                                   // normalization
       if (Permission_to_Proceed(cnd_level_name, cnd_file)) {
//          lu_too = Ask_an_Answer("Do you wish to generate an ILU Descriptor now");
         T.start();
//          if (lu_too)
//          LUD::lud.generate(cnd_level_name);
         CND::cnd.translate(cnd_level_name);
         T.stop();
//          if (lu_too) {
//            T.start();
//            LUD::lud.normalize(CND::cnd.n_factor());
//            LUD::lud.decompose();
//            T.stop();
//            LUD::lud.write();
//          }
         DSC::dsc.erase();
       }
   }
   Notify_Time_Spend(T, "compilation of a SAN model");
 }
}

//====================================================================================//
void Aggregate_DSC(bool norm_too)                               //   Main Operation   //
//====================================================================================//
{                          //     SAN model name in all directories handled by PEPS
 peps_name model_name;     // current directory (just the name, no paths, no extensions)
 peps_name agg_level_name; // directory to store the aggregated descriptor (./agg/*.dsc)
 peps_name cnd_level_name; // directory to store the normalized descriptor (./cnd/*.cnd)

 Timer     T;              // timer to inform total time spent
 aut_st    agreg_map;      // aggregation map to perform aggregation
//  bool      lu_too;         // the descriptor must be LU decomposed (generates a .lud)

 cout << "\nAggregation of a SAN descriptor";
 T.clear();
 if (Provide_DSC()) {                                                // aggregation
   DSC::dsc.name(agg_level_name);
   Get_Only_the_File_Name(agg_level_name, model_name);
   strcpy(agg_level_name, "agg/");
   strcpy(cnd_level_name, "cnd/");
   Ask_Agregation_Map(agreg_map);
   cout << "\nYou need a new file name to the aggregate model";
   Ask_a_File_Name(model_name, dsc_file);
   strcat(agg_level_name, model_name);
   strcat(cnd_level_name, model_name);
   if (Permission_to_Proceed(agg_level_name, dsc_file)) {
     T.start();
     DSC::dsc.aggregate(agreg_map, agg_level_name);
     T.stop();
   }
   if (norm_too)                                                   // normalization
     if (Permission_to_Proceed(cnd_level_name, cnd_file)) {
//        lu_too = Ask_an_Answer("Do you wish to generate an ILU Descriptor now");
       T.start();
//        if (lu_too)
//        LUD::lud.generate(cnd_level_name);
       CND::cnd.translate(cnd_level_name);
       T.stop();
//        if (lu_too) {
//          T.start();
//          LUD::lud.normalize(CND::cnd.n_factor());
//          LUD::lud.decompose();
//          T.stop();
//          LUD::lud.write();
//        }
       DSC::dsc.erase();
     }
   Notify_Time_Spend(T, "aggregation of a SAN descriptor");
 }
}

//====================================================================================//
void Normalize_DSC(bool use_aggr)                               //   Main Operation   //
//====================================================================================//
{
 peps_name model_name;     // current directory (just the name, no paths, no extensions)
 peps_name dsc_level_name; // directory where the descriptor came from (./[dsc,agg]/*.dsc)
 peps_name cnd_level_name; // directory to store the normalized descriptor (./cnd/*.cnd)

 Timer     T;
 bool      go_on, lu_too;

 cout << "\nNormalization of a SAN Descriptor\n";
 if (use_aggr)
   go_on = Provide_AGG();
 else
   go_on = Provide_DSC();
 if (go_on) {
   DSC::dsc.name(dsc_level_name);
   Get_Only_the_File_Name(dsc_level_name, model_name);
   strcpy(cnd_level_name, "cnd/"); strcat(cnd_level_name, model_name);
   if (Permission_to_Proceed(cnd_level_name, cnd_file)) {
//     lu_too = Ask_an_Answer("Do you wish to generate an ILU Descriptor now");
     T.clear();
     T.start();
//      if (lu_too)
//        LUD::lud.generate(name);
     CND::cnd.translate(cnd_level_name);
     T.stop();
//      if (lu_too) {
//        T.start();
//        LUD::lud.normalize(CND::cnd.n_factor());
//        LUD::lud.decompose();
//        T.stop();
//        LUD::lud.write();
//      }
//      if (lu_too)
//        Notify_Time_Spend(T, "descriptor normalization and ILU decomposition");
//      else
     DSC::dsc.erase();
   }
   Notify_Time_Spend(T, "normalization of a SAN descriptor");
 }
}

//====================================================================================//
void Edit_cnd()
//====================================================================================//
{
 peps_name name;
 aut_id    a, automata;
 ev_id     events;
 int       sss, global_i, global_j;
 aut_st    sizes;
 aut_st    local_i, local_j;
 rp        element;
 bool      continue_questions = true;

 if (Provide_CND()) {
   CND::cnd.name(name);
   automata = CND::cnd.aut();
   events   = CND::cnd.ev();
   sss      = CND::cnd.ss();
   cout << "\nVisualizing the elements of the Descriptor '" << name << ".cnd'\n"
        << "\nAutomata: " << automata << "   -   Events: " << events << "\n"
        <<   "State Space: " << sss << " states where " << RSS::rss.reach_size()
        <<   " are reachable\n"
        <<   "Normalizing factor: " << CND::cnd.n_factor()
        <<   " (largest element " << -1/CND::cnd.n_factor() << ")\n\n";
   CND::cnd.get_s(sizes);
   local_i.create(automata);
   local_j.create(automata);
   cout << "To visualize the elements of this descriptor you must enter the row and\n"
        << " column indexes of the global states in values from 0 to " << sss-1 << " (enter\n"
        << " values out of this range to stop the visualization). You can use vector\n"
        << " utilities option: 'inspect reachable state space' to find out the global\n"
        << " states indexes)\n\n";
   do { 
     cout << "Enter row index (0.." << sss-1 << "): ";
     cin >> global_i;
     if ((global_i < 0) || (global_i >= sss))
       continue_questions = false;
     else {
       local_i.st_rank(global_i, sizes);
       cout << "Local states for row index: [ ";
       for (a=0; a<automata; a++)
         cout << local_i.st(a) << " ";
       cout << "]\n";
       cout << "Enter column index (0.." << sss-1 << "): ";
       cin >> global_j;
       if ((global_i < 0) || (global_i >= sss))
         continue_questions = false;
       else {
         local_j.st_rank(global_j, sizes);
         cout << "Local states for column index: [ ";
         for (a=0; a<automata; a++)
           cout << local_j.st(a) << " ";
         cout << "]\n";
         element = CND::cnd.coord(local_i, local_j);
         cout << "Element value: " << element / CND::cnd.n_factor()
              << " (normalized: " << element << ")\n";
       }
     }
   } while(continue_questions);
 }
}

//====================================================================================//
void Compare_vct()
//====================================================================================//
{
 peps_name name;
 peps_name v_name;
 VCT       vec;
 rp        abs_err, rel_err;
 rp        big, sml;
 int       bpos, spos;
 rp        acc;
 

 cout.setf(ios::scientific);
 cout.precision(peps_precision);
 if (Provide_VCT()) {
   VCT::vct.name(name);
   cout << "\nCompare vector '" << name
        << ".vct' (" << VCT::vct.size() << "/"
	<< VCT::vct.real_size() << " states) with ";
   Ask_an_Existing_File_Name(v_name, vct_file);
   vec.read(v_name);
   if (vec.size() != VCT::vct.size())
     cout << "The vectors (" << name << ".vct and " << v_name
          << ".vct) do not have the same size!\n";
   else {
      abs_err = VCT::vct.compare(vec, abs_ind_err);
      VCT::vct.info(acc, big, bpos, sml, spos);
      rel_err = abs_err / sml;
      cout <<   "The maximum absolute error is: " << abs_err
           << "\nThe maximum relative error is: " << rel_err
           << "\nVector '" << name << ".vct' reachable states probability"
           << "\n   max.:  " << big << "   min.: " << sml;
      vec.info(acc, big, bpos, sml, spos);
      cout << "\nVector '" << v_name << ".vct' reachable states probability"
           << "\n   max.:  " << big << "   min.: " << sml << "\n";
   }
 }
}

//====================================================================================//
void Compute_Residue_vct()
//====================================================================================//
{
 peps_name v_name, d_name, vr_name;
 VCT       buf;
 rp        acc, big, sml;
 int       bpos, spos;

 cout.setf(ios::scientific);
 cout.precision(peps_precision);
 if (Provide_VCT_CND()) {
   VCT::vct.name(v_name);
   CND::cnd.name(d_name);
   if (VCT::vct.size() != CND::cnd.ss())
     cout << "\nThe vector " << v_name << ".vct and the descritpor "
          << d_name << ".cnd does not have the same size\n";
   else {
     cout << "\nComputing the residual of the solution '" << v_name
          << ".vct' (" << RSS::rss.reach_size() << "/" << VCT::vct.real_size()
          << " states) for the descriptor "
          << d_name << ".cnd (" << CND::cnd.aut() << " automata)\n"
          << "Please enter a probability vector name in which to store the residual\n";
     Ask_a_File_Name(vr_name, vct_file);
     buf.create(VCT::vct.size(), VCT::vct.real_size());
     buf.get_CND();
     buf.alloc_static();
     buf.baptise(vr_name);
     buf.mult_CND(VCT::vct);
     buf.write();
     buf.info(acc, big, bpos, sml, spos);
     if (absv(acc) > round_zero)
       cout << "The result (" << vr_name << ".vct) is not a residual vector\n"
            << "maybe " << d_name << ".cnd is not an infinitesimal generator\n"
            << "maybe " << v_name << ".vct is not a probability vector\n"
            << "or a program error occured!\n";
     else {
       cout << "The largest element in residual vector is " << big << " (" << bpos
            << "),\n" << "therefore the vector " << v_name << ".vct";
       if (big > PRF::prf.min_err())  cout << " is not ";
       else                           cout << " is ";
       cout << "the approximated solution\n"
            << "to the descritor " << d_name << ".cnd (smallest: " << sml
            << " (" << spos << ")\n";
     }
     buf.erase();
     buf.desalloc_static();
   }
 }
}

//====================================================================================//
void Look_Global_State_vct()
//====================================================================================//
{
 peps_name name_vec, name_aut, name_st;
 aut_st    sizes;
 aut_st    global_state;
 int       reduce_s;
 int       option;
 bool      go_on;
 aut_id    a;
 st_id     s;

 cout.setf(ios::scientific);
 cout.precision(peps_precision);
 if (go_on = Provide_VCT_CND()) {
   VCT::vct.name(name_vec);
   cout << "\nInspecting the probability of one global state of the vector "
        << name_vec << ".vct\n";
   CND::cnd.get_s(sizes);
   global_state.create(CND::cnd.aut());
   do {
     cout << "\nDo you wish to: 1) inspect a probability indicating its index"
          << "\n                2) inspect a probability indicating its local states vector"
          << "\n     (as always in PEPS, option '0' leaves the current menu)\n";
     cin >> option;
     switch (option) {
     case 1 : s = Ask_a_Global_State();
              global_state.st_rank(s, sizes);
              break;
     case 2 : s = Ask_a_Global_State(global_state);
              global_state.st_rank(s, sizes);
              break;
     default : go_on = false;
     }
     if (go_on) {
       cout << "-------------------------------------------------------------------------"
            << "\nGlobal State: " << s << ": ";
       reduce_s = RSS::rss.orig2rem(s);
       if (reduce_s == no_st)
         cout << zero << "\n[ ";
       else
         cout << VCT::vct.get(reduce_s) << " (after grouping: " << reduce_s << ")\n[ ";

       for (a=0; a<global_state.size(); a++)
         cout << global_state.st(a) << " ";

       if (RSS::rss.has(s))
         cout << "] is ";
       else
         if (RSS::rss.remains(s))
           cout << "] is un";
         else
           cout << "] was removed by grouping - and it is un";
       cout << "reachable\n-----------------"
            << "--------------------------------------------------------";

       /*
       position = RSS::rss.orig2rem(s);
       cout << "-------------------------------------------------------------------------"
            << "\nProbability of the global state in the " << position
            << "-th position is: ";
       if (position == no_st)  cout << zero;
       else                    cout << VCT::vct.get(position);
       cout << "\nThis global state corresponds to the following local states:\n[";
       for (a=0; a<global_state.size(); a++)
         cout << global_state.st(a) << " ";
       if (RSS::rss.has(s))
         cout << "] is ";
       else
         if (RSS::rss.remains(s))
           cout << "] is un";
         else
           cout << "] was removed by grouping - and it is un";
       cout << "reachable\nThis state was ranked in the original product state space in the "
            << s << "-th position"
            << "\n-------------------------------------------------------------------------";
       */
     }
   } while (go_on);
 }
}

//====================================================================================//
void Show_a_Slice_vct()
//====================================================================================//
{
 peps_name name;
 int    first, last, i;
 aut_id a;
 aut_st global_state, sizes;

 cout.setf(ios::scientific);
 cout.precision(peps_precision);
 if (Provide_VCT_CND()) {
   VCT::vct.name(name);
   cout << "\nVector '" << name
        << ".vct' (" << RSS::rss.reach_size() << "/" << VCT::vct.real_size()
	<< " states)\n\n";
   global_state.create(CND::cnd.aut());
   CND::cnd.get_s(sizes);
   do { first = 0;
        last  = VCT::vct.real_size()-1;
        Ask_a_Vector_Range(first, last);
        global_state.st_rank(first, sizes);
        for (i=first; i<last+1; i++,global_state.next(sizes)) {
          for (a=0; a<global_state.size(); a++)
            cout << global_state.st(a) << " ";
          cout << " : " << VCT::vct.get(i) << " (" << i << ")\n";
        }
        if (Ask_an_Answer("Do you need state name vs. number table")) {
          DCT::dct.show_states();
          DCT::dct.print();
        }
    } while (Ask_an_Answer("Do you wish to see more elements"));
 }
}

//====================================================================================//
void Show_Characteristics_vct()
//====================================================================================//
{
 peps_name name;
 int first, size, i;
 rp  sum, big, sml;
 int big_w, sml_w;

 cout.setf(ios::scientific);
 cout.precision(peps_precision);
 if (Provide_VCT()) {
   first = 0;
   size  = VCT::vct.size();
   VCT::vct.name(name);
   cout << "\nVector '" << name << ".vct' (" << RSS::rss.reach_size()
        << "/" << VCT::vct.real_size() << " states)";
   VCT::vct.info(sum, big, big_w, sml, sml_w);
   cout << "\nSum of all " << size << " elements: " << sum
        << "\n  - Largest  reachable element (" << big_w << "): " << big
        << "\n  - Smallest reachable element (" << sml_w << "): " << sml
        << "\nFirst elements: ";
   for (i=first; i<first+4; i++)
     cout << VCT::vct.get(i) << " ";
   cout << "\nLast elements: ";
   for (i=size-4; i<size; i++)
     cout << VCT::vct.get(i) << " ";
   cout << "\n";
 }
}

//====================================================================================//
void Integrate_Local_States()
//====================================================================================//
{
 peps_name name_vec, name_cnd, name_aut, name_st;
 int       product_size, remaining_size;
 int       i,            k;
 aut_st    state;
 aut_st    sizes;
 aut_id    a;
 st_id     s, first, last;
 rp        sum, acc;
 bool      accumulate;

 cout.setf(ios::scientific);
 cout.precision(peps_precision);
 if (Provide_VCT_CND()) {
   VCT::vct.name(name_vec);
   CND::cnd.name(name_cnd);
   // Verify integrity cnd vct rss ftb (same size)
   FTB::ftb.get_domain(sizes);
   state.create(FTB::ftb.aut());
   product_size   = FTB::ftb.ss();
   remaining_size = VCT::vct.real_size();
   cout << "\nIntegration of the vector '" << name_vec << ".vct' ("
        << remaining_size << " states) solution of '" << name_cnd << ".cnd' ("
        << product_size   << " states)\n";
   Ask_an_Automaton(a);
   Ask_Local_States(a, sizes.sz(a), s);
   cout << "\n";
   if (s != no_st) { first = s;
                     last  = s+1; }
   else {            accumulate = true;
                     acc        = zero;
                     first = 0;
                     last  = sizes.sz(a); }
   for (s=first; s<last; s++) {
     sum = 0;
     state.reset();
     for (i=0,k=0; i<product_size; i++,state.next(sizes))
       if (RSS::rss.remains(i)) {
         if (state.st(a) == s)
           sum += VCT::vct.get(k);
           k++;
       }
     DCT::dct.s_name(a, s, name_st);
     cout << "   " << sum << " for local state '" << name_st << "'\n";
     acc += sum;
   }
   DCT::dct.a_name(a, name_aut);
   if (accumulate)
     cout << " --------------------------\n"
          << "   " << acc << " (total for automaton '" << name_aut << "')\n";
 }
}

//====================================================================================//
void Integrate_Results(const bool write_log)
//====================================================================================//
{
 peps_name name_vec, name_cnd, name_inf, name_func;
 func_id   f;
 int       product_size, remaining_size;
 int       i, k, j;
 aut_st    state;
 aut_st    sizes;
 rp        sum, val;
 
 cout.setf(ios::scientific);
 cout.precision(peps_precision);
 if (Provide_VCT_CND()) {
   VCT::vct.name(name_vec);
   CND::cnd.name(name_cnd);
   INF::inf.name(name_inf);
   // Verify integrity cnd vct rss ftb inf (same size)
   INF::inf.get_domain(sizes);
   state.create(FTB::ftb.aut());
   product_size   = INF::inf.ss();
   remaining_size = VCT::vct.real_size();
   cout << "\nIntegration (" << name_inf << ".inf) of the vector '"
        << name_vec << ".vct' (" << remaining_size << " states) solution of '"
        << name_cnd << ".cnd' (" << product_size   << " states)\n";

   for (f=0; f<INF::inf.size(); f++) {
     sum = 0;
     state.reset();
     if (PRF::prf.vec_impl() == v_sparse) {  // sparse
       for (i=0,j=0,k=0; i<product_size; i++, state.next(sizes))
	 if (RSS::rss.remains(i)) {
	   if (RSS::rss.has(i)) {   // If the state is reachable, we take the value
	     val = VCT::vct.get(j); // in VCT::vct.vec
	     j++;                   // We go to the next element of vec
	     sum += val * INF::inf.eval(f, state); // Compute the result
	   }
	   k++;  // go to the next state
	 }
     }
     else {    // full
       for (i=0,k=0; i<product_size; i++, state.next(sizes))
	 if (RSS::rss.remains(i)) {
	   val = VCT::vct.get(k);
	   if (val != zero)
	     sum += val * INF::inf.eval(f, state);
	   k++;
	 }
     }
     DCT::dct.sf_name(f, name_func);
     cout << "Integration of function " << name_func << " : " << sum << "\n";
     if (write_log)
       Log_Out_a_Solution_Function(f, sum);
   }
 }
}

//====================================================================================//
void Analyse_Reachable_States()
//====================================================================================//
{
 peps_name name, output_name;
 ofstream  fout;
 int       first, last, i;
 aut_id    a;
 aut_st    global_state, sizes;
 bool      see_one;

 cout.setf(ios::scientific);
 cout.precision(peps_precision);
 if (Provide_VCT_CND()) {
   VCT::vct.name(name);
   cout << "\nVector '" << name
        << ".vct' (" << RSS::rss.reach_size() << "/" << VCT::vct.real_size()
	<< " states)\n\n";
   global_state.create(CND::cnd.aut());
   CND::cnd.get_s(sizes);
   do { first = 0;
        last  = VCT::vct.real_size()-1;
        see_one = false;
        Ask_a_Vector_Range(first, last);
        global_state.st_rank(first, sizes);
        for (i=first; i<last+1; i++,global_state.next(sizes)) {
	  if (RSS::rss.has(i)) {
            see_one = true;
            for (a=0; a<global_state.size(); a++)
              cout << global_state.st(a) << " ";
            cout << " : " << VCT::vct.get(i) << " (" << i << ")\n";
	  }
        }
        if (! see_one)
          cout << "No reachable state from state " << first << " to " << last << "\n";
        if (Ask_an_Answer("Do you wish to save ALL reachable states in a file")) {
          Ask_a_File_Name(output_name, rea_file);
          if (Permission_to_Proceed(output_name, rea_file)) {
            Open_File(output_name, rea_file, fout);
            fout << "\nVector '" << name << ".vct' ("
                 << RSS::rss.reach_size() << "/" << VCT::vct.real_size()
		 << " states)\n\n";
            global_state.reset();
            for (i=0; i<VCT::vct.real_size(); i++,global_state.next(sizes))
              if (RSS::rss.has(i)) {
                for (a=0; a<global_state.size(); a++)
                  fout << global_state.st(a) << " ";
                fout << " : " << VCT::vct.get(i) << " (" << i << ")\n";
              }
            Close_File(output_name, rea_file, fout);
          }
        }
    } while (Ask_an_Answer("Do you wish to see (on screen) more elements"));
 }
}

//====================================================================================//
void Import_MARCA_vct()
//====================================================================================//
{
 peps_name name_vec, name_cnd, name_marca_vec;
 ifstream  fin;
 ofstream  fout;
 int       product_size, remaining_size, reachable_size, marca_vct_size;
 int                     i,              k;
 rp        sum = 0;
 rp        buf;

 cout.setf(ios::scientific);
 cout.precision(peps_precision);
 if (Provide_CND()) {
   CND::cnd.name(name_cnd);
   product_size   = RSS::rss.orig_size();
   remaining_size = RSS::rss.size();
   reachable_size = RSS::rss.reach_size();
   cout << "Importing a vector from MARCA as solution of '" << name_cnd << "'\n";
   Ask_an_Existing_File_Name(name_marca_vec, dot_file);
   Ask_a_File_Name(name_vec, vct_file);
   if (Permission_to_Proceed(name_vec, vct_file)) {
     
     if (PRF::prf.vec_impl() == v_sparse)
       VCT::vct.create(reachable_size, remaining_size);
     else
       VCT::vct.create(remaining_size, remaining_size);
     
     VCT::vct.baptise(name_vec);
     VCT::vct.get_CND();
     Open_File(name_marca_vec, dot_file, fin);
     fin >> marca_vct_size;
     if (marca_vct_size != reachable_size)
       cout << "Size of Marca vector '" << name_marca_vec << "' and reachable "
            << "state space size of model in '" << name_cnd << ".cnd' mismatch!\n"
            << "****** Import Operation Aborted! ******\n";
     else {
       for (i=0; i<remaining_size; i++)
         if (RSS::rss.has(i)) {
           fin >> buf;
           sum += buf;
           VCT::vct.put(i, buf);
         }
         else
           VCT::vct.put(i, zero);
       Close_File(name_marca_vec, dot_file, fin);
       VCT::vct.write();
       cout << "Vector '" << name_marca_vec << "' (" << marca_vct_size << " states) "
            << "imported and saved to '" << name_vec << ".vct (" << remaining_size
            << " remaining states from " << product_size << " original states\n";
       if (absv(sum - one) > round_zero)
         cout << "** Warning: May not be a probability vector (sum " << sum << ")! **\n";
       else {
         Integrate_Results(false);
         // To Log out solution call it with 'true'
       }
     }
   }
 }
}

//====================================================================================//
void Generate_hbf()
//====================================================================================//
{                                // SAN model name in all directories handled by PEPS //
 peps_name model_name;     // current directory (just the name, no paths, no extensions)
 peps_name san_level_name; // directory where the .san is
 peps_name des_level_name; // directory where the textual descriptor is
 peps_name dsc_level_name; // directory where the internal format descriptor is
 peps_name matrix_name;    

 Timer     T;          // timer to inform total time spent

 T.clear();
 if (!DSC::dsc.is_empty() &&
          Ask_an_Answer("Do you want to use the descriptor already in memory")) {
   DSC::dsc.name(model_name);
   cout << "\nUsing " << model_name << " SAN model to sparse matrix (hbf) generation";
 }
 else {
   cout << "\nCompilation of SAN model and sparse matrix (hbf) generation";
   Ask_an_Existing_File_Name(san_level_name, san_file);
   Get_Only_the_File_Name(san_level_name, model_name);
   strcpy(des_level_name, "des/");  strcat(des_level_name, model_name);
   strcpy(dsc_level_name, "dsc/");  strcat(dsc_level_name, model_name);
   if (Permission_to_Proceed(des_level_name, des_file)) {             // .san compilation
     T.start();
     Compile_Network(san_level_name, des_level_name);
     T.stop();
     if (Permission_to_Proceed       (dsc_level_name, dsc_file)) {    // .des compilation
       T.start();
       Compile_Function_Table        (des_level_name, dsc_level_name);
       Compile_Descriptor            (des_level_name, dsc_level_name);
       Compile_Reacheable_SS         (des_level_name, dsc_level_name);
       Compile_Dictionary            (des_level_name, dsc_level_name);
       Compile_Integration_Function  (des_level_name, dsc_level_name);
       T.stop();
     }
   }
 }
 Ask_a_File_Name(matrix_name, hbf_file);                          // .hbf generation
 if (Permission_to_Proceed(matrix_name, hbf_file)) {
   cout << "\nGenerating the sparse matrix ...\n";
   T.start();
   HBF::hbf.generate(matrix_name);
   T.stop();
 }
 Notify_Time_Spend(T, "compilation of a SAN model and sparse matrix generation");
}

//====================================================================================//
void Convert_hbf()
//====================================================================================//
{
 peps_name original_name, new_name;

 Ask_an_Existing_File_Name(original_name, hbf_file);
 HBF::hbf.read(original_name);
 cout << "The matrix '" << original_name << ".hbf' is in ";
 switch (HBF::hbf.first_row()) {
 case 0  : cout << "the PEPS format (first column index is 0)\n";
           cout << "Converting to MARCA format (first row index is 1)\n";
	   HBF::hbf.convert("MATX_hb", F_format);
           break;
 case 1  : cout << "the MARCA format (first row index is 1)\n";
           Ask_a_File_Name(new_name, hbf_file);
           if (Permission_to_Proceed(new_name, hbf_file)) {
             cout << "Converting to PEPS format (first column index is 0)\n";
	     HBF::hbf.convert(new_name, C_format);
           } break;
 default : cout << "a unknown format\n ****** Conversion aborted! ****** \n\n";
 }
}

//====================================================================================//
void Power_hbf()
//====================================================================================//
{
 peps_name name;
 VCT       buf;

 if (Provide_HBF()) {
   HBF::hbf.name(name);
   cout << "\nSolution of the model '" << name << ".hbf' ("
        << HBF::hbf.size() << " nonzeros - " << HBF::hbf.order() << " states)\n";
   Ask_a_File_Name(name, vct_file);
   if (Permission_to_Proceed(name, vct_file)) {
     VCT::vct.create(HBF::hbf.order(),HBF::hbf.order());
     VCT::vct.baptise(name);
     if (PRF::prf.i_vec() != in_vec) {
       buf.create(HBF::hbf.order(),HBF::hbf.order());
       buf.init_hbf(PRF::prf.i_vec());
     }
     else
       do {
         Ask_an_Existing_File_Name(name, vct_file);
         buf.read(name);
       } while (buf.real_size() != VCT::vct.real_size());
     if (Power_HBF(buf))
       cout << "Convergence!\n";
   }
 }
}

//====================================================================================//
void State_hbf()
//====================================================================================//
{
 peps_name file_name;
 ofstream  fout;
 int       o_ss;         // the product state space (original state space size)
 int       o_pss;        // indice of the current global state (from 1 to #PSS - 1)
 int       o_rss;        // numbering from 0 to #RSS - 1
 aut_st    d_sizes;      // the size of each automata in the descriptor 
 aut_id    a;
 aut_st    global_state; // the state vector (corresponding to the state in 'o_j') 
 Timer     T;            // timer to inform total time spent

 cout << "\nState equivalence generation\n";
 T.clear();

 if (Provide_DSC()) {
   Ask_a_File_Name(file_name, dbg_file);                          // .hbf generation
   if (Permission_to_Proceed(file_name, dbg_file)) {
     cout << "\nGenerating the state equivalence ...\n";
     T.start();
     
     Open_File(file_name, dbg_file, fout);
     fout << "Equivalence de numerotation des etats : \n"
          << "   num de 0 a (#RSS - 1) -> num de 0 a (#PSS - 1) = [etats locaux]\n\n";
     
     o_ss = RSS::rss.orig_size();           // the product state space size     
     DSC::dsc.get_s(d_sizes);              // gets the automata sizes
     global_state.create(DSC::dsc.aut());
     
     //--------------------------------------------------- Loop on the reachable states
     for (o_pss=0, o_rss=0; o_pss<o_ss; o_pss++) { // for each original state
       if (RSS::rss.remains(o_pss)) {                   //   if it is in the descriptor
	 if (RSS::rss.has(o_pss)) {                     //     if it is reachable
	   global_state.st_rank(o_pss, d_sizes);
	   fout << o_rss << " -> " << o_pss <<  " = [ ";
	   for (a=0; a<global_state.size(); a++)
	     fout << global_state.st(a) << " ";
	   fout << "]\n";
	   o_rss += 1;
	 }
       }
     }
     
     Close_File(file_name, dbg_file, fout);
     
     T.stop();
   }
 }
 Notify_Time_Spend(T, "state equivalence generation");
}



//====================================================================================//
bool Start_Up_CND()
//====================================================================================//
{
 bool      answer = false;
 peps_name name, n;
 v_typ     initial_vector_type = PRF::prf.i_vec();

 if (Provide_CND()) {
   CND::cnd.name(name);
   cout << "\nSolution of the model '" << name << ".cnd' ("
        << CND::cnd.aut() << " automata - " << RSS::rss.reach_size()
        << "/" << RSS::rss.size() << " states)\n";
   Ask_a_File_Name(name, vct_file);
   if (Permission_to_Proceed(name, vct_file)) {
     if (initial_vector_type != in_vec) {
       if (PRF::prf.vec_impl() == v_sparse)
	 VCT::vct.create(RSS::rss.reach_size(), CND::cnd.ss());
       else
	 VCT::vct.create(CND::cnd.ss(), CND::cnd.ss());
       VCT::vct.get_CND();
       VCT::vct.init(initial_vector_type);
       VCT::vct.baptise(name);
     }
     else
       do {
         Ask_an_Existing_File_Name(name, vct_file);
         VCT::vct.read(name);
       } while (CND::cnd.ss() != VCT::vct.real_size());
     answer = true;
   }
 }
 
 return(answer);
}



//====================================================================================//
bool Start_Up_CND(VCT & buf)
//====================================================================================//
{
 bool      answer = false;
 peps_name name, n;
 v_typ     initial_vector_type = PRF::prf.i_vec();

 // cout << "debut start_up_cnd\n";

 if (Provide_CND()) {
   CND::cnd.name(name);
   cout << "\nSolution of the model '" << name << ".cnd' ("
        << CND::cnd.aut() << " automata - " << RSS::rss.reach_size()
        << "/" << RSS::rss.size() << " states)\n";
   Ask_a_File_Name(name, vct_file);
   VCT::vct.baptise(name);
   if (Permission_to_Proceed(name, vct_file)) {
     if (PRF::prf.vec_impl() == v_sparse)
       VCT::vct.create(RSS::rss.reach_size(), RSS::rss.size());
     else
       VCT::vct.create(RSS::rss.size(), RSS::rss.size());
     VCT::vct.get_CND();
     VCT::vct.baptise(name);
     //n = name;
     strcpy(n,name);
     
     if (initial_vector_type != in_vec) {
       if (PRF::prf.vec_impl() == v_sparse)
	 buf.create(RSS::rss.reach_size(), CND::cnd.ss());
       else
	 buf.create(CND::cnd.ss(), CND::cnd.ss());
       buf.init(initial_vector_type);
     }
     else
       do {
         Ask_an_Existing_File_Name(name, vct_file);
         buf.read(name);
       } while (buf.real_size() != VCT::vct.real_size());
     
     answer = true;
     buf.baptise(n);
   }
 }
 
 // cout << "fin start_up_cnd : buf.dimension = " << buf.size();
 // cout << " et buf.real_dim = " << buf.real_size() << " \n";
 
 //buf.name(n);
 //cout << "End of Start_Up_CND : buf name = " << n << ".vec \n";
 
 return(answer);
}


//====================================================================================//
bool Start_Up_CND_LUD()
//====================================================================================//
{
 bool      answer = false;
 peps_name name;
 v_typ     initial_vector_type = PRF::prf.i_vec();

 if (Provide_CND_LUD()) {
   CND::cnd.name(name);
   cout << "\nSolution of the model '" << name << ".cnd' ("
        << CND::cnd.aut() << " automata - " << RSS::rss.reach_size()
        << "/" << RSS::rss.size() << " states)\n";
   Ask_a_File_Name(name, vct_file);
   if (Permission_to_Proceed(name, vct_file)) {
     if (initial_vector_type != in_vec) {
       if (PRF::prf.vec_impl() == v_sparse)
	 VCT::vct.create(RSS::rss.reach_size(), CND::cnd.ss());
       else
	 VCT::vct.create(CND::cnd.ss(), CND::cnd.ss());
       VCT::vct.get_CND();
       VCT::vct.baptise(name);
       VCT::vct.init(initial_vector_type);
     }
     else
       do {
         Ask_an_Existing_File_Name(name, vct_file);
         VCT::vct.read(name);
       } while (CND::cnd.ss() != VCT::vct.real_size());
     answer = true;
   }
 }
 return(answer);
}

//====================================================================================//
bool Start_Up_CND_LUD(VCT & buf)
//====================================================================================//
{
 bool      answer = false;
 peps_name name;
 v_typ     initial_vector_type = PRF::prf.i_vec();

 if (Provide_CND_LUD()) {
   CND::cnd.name(name);
   cout << "\nSolution of the model '" << name << ".cnd' ("
        << CND::cnd.aut() << " automata - " << RSS::rss.reach_size()
        << "/" << RSS::rss.size() << " states)\n";
   Ask_a_File_Name(name, vct_file);
   VCT::vct.baptise(name);
   if (Permission_to_Proceed(name, vct_file)) {
     if (PRF::prf.vec_impl() == v_sparse)
       VCT::vct.create(RSS::rss.reach_size(), RSS::rss.size());
     else
       VCT::vct.create(RSS::rss.size(), RSS::rss.size());
     VCT::vct.get_CND();
     VCT::vct.baptise(name);
     if (initial_vector_type != in_vec) {
       if (PRF::prf.vec_impl() == v_sparse)
	 buf.create(RSS::rss.reach_size(), CND::cnd.ss());
       else
	 buf.create(CND::cnd.ss(), CND::cnd.ss());
       buf.init(initial_vector_type);
     }
     else
       do {
         Ask_an_Existing_File_Name(name, vct_file);
         buf.read(name);
       } while (buf.real_size() != VCT::vct.real_size());
     answer = true;
   }
 }
 return(answer);
}


//====================================================================================//
//====================================================================================//

//====================================================================================//
//                                    PEPS Main                                       //
//====================================================================================//
int main()
{
 bool      cont = true;
 peps_name name;
 VCT       buf;
 v_impl    vector_impl_type;

 cout.setf(ios::scientific);
 cout.precision(16);
 set_new_handler(Communicate_Out_of_Memory);
 do {
   switch (Welcome_Get_Peps_Option()) {
//====================================================================================//
//                      Compiling and Normalizing Operations                          //
//====================================================================================//
   case compile         : Compile_SAN(false, false);  break;
   case comp_aggr       : Compile_SAN(true,  false);  break;
   case comp_norm       : Compile_SAN(false, true);   break;
   case comp_aggr_norm  : Compile_SAN(true,  true);   break;
   case aggregate       : Aggregate_DSC(false);       break;
   case aggr_norm       : Aggregate_DSC(true);        break;
   case normalize1      : Normalize_DSC(false);       break;
   case normalize2      : Normalize_DSC(true);        break;
//====================================================================================//
//                                   Solve Operations                                 //
//====================================================================================//
   case power_std    :
     vector_impl_type = PRF::prf.vec_impl();
     if (Start_Up_CND(buf))
       if (Power_CND(buf, no_precond))
	 Integrate_Results(true);
     break;

   case power_diag   : if (Start_Up_CND(buf))      if (Power_CND(buf, diag))
                                                     Integrate_Results(true);
                       break;

   case arnoldi_std  : if (Start_Up_CND())         if (Arnoldi_CND(no_precond))
                                                     Integrate_Results(true);
                       break;

   case arnoldi_diag : if (Start_Up_CND())         if (Arnoldi_CND(diag))
                                                     Integrate_Results(true);
                       break;

   case gmres_std    : if (Start_Up_CND())         if (GMRES_CND(no_precond))
                                                     Integrate_Results(true);
                       break;

   case gmres_diag   : if (Start_Up_CND())         if (GMRES_CND(diag))
                                                     Integrate_Results(true);
                       break;

//====================================================================================//
//                                   Vector Operations                                //
//====================================================================================//
   case compare_vct       : Compare_vct();                         break;
   case look_global_vct   : Look_Global_State_vct();               break;
   case slice_vct         : Show_a_Slice_vct();                    break;
   case charac_vct        : Show_Characteristics_vct();            break;
   case residue_vct       : Compute_Residue_vct();                 break;
   case integrate_local   : Integrate_Local_States();              break;
   case integrate_results : Integrate_Results(false);              break;
   case analyse_reachable : Analyse_Reachable_States();            break;
   case import_marca_vct  : Import_MARCA_vct();                    break;

//====================================================================================//
//                                    HBF Operations                                  //
//====================================================================================//
   case generate_hbf : Generate_hbf();                            break;
   case power_hbf    : Power_hbf();                               break;
   case convert_hbf  : Convert_hbf();                             break;
   case state_hbf    : State_hbf();                               break;

//====================================================================================//
//                                    Load Operations                                 //
//====================================================================================//
   case read_dsc : Provide_DSC();                                 break;
   case read_cnd : Provide_CND();                                 break;
   case read_agg : Provide_AGG();                                 break;
   case read_vct : Provide_VCT();                                 break;
   case read_hbf : Provide_HBF();                                 break;

//====================================================================================//
//                                  Inspect Operations                                //
//====================================================================================//
   case see_dsc : See_DSC();                                       break;
   case see_rss : See_RSS();                                       break;
   case see_vct : See_VCT();                                       break;
   case see_hbf : See_HBF();                                       break;

//====================================================================================//
//                                   Other Operations                                 //
//====================================================================================//
   case credits     : Show_Credits();                              break;
   case goodbye     : cont = false;                                break;
   default          : Programming_Error("invalid PEPS option", 0000)
   }
 } while (cont);
 Say_Goodbye();
 return(0);
}
