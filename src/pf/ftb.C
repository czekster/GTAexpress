//====================================================================================//
//                                                                                    //
//                             Functions Table Structure                              //
//                                                                                    //
//====================================================================================//
//  This File:   ftb.C                      Language: C++  (xlC and CC)               //
//  Software:    PEPS 3.1                    Advisor: Brigitte.Plateau@imag.fr        //
//  Error Code:  4350                                                                 //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 17/feb/03                        by: lbrenner@inf.pucrs.br           //
//====================================================================================//
#include "peps.h"

     //--------------------------------------------------------------------------//
     //                   static variable: The Function Table                    //
     //--------------------------------------------------------------------------//
FTB FTB::ftb = FTB();

     //--------------------------------------------------------------------------//
     //             It Creates 'this' according to the scratch 'sft'             //
     //--------------------------------------------------------------------------//
void FTB::create(scr_ft & sft)
{
 int     orig_aut =  domain.size();             // original number of automata
 int     curr_aut =  currents.count_groups();   // current number of automata
 aut_id  a;                                     // counter for original automata
 aut_set orig_params;                           // buffer to the original parameters

 orig_params.create(orig_aut);
 dimension = sft.has();
 alloc();
 for (number=0; number<dimension; number++) {
   sft.get(number, table[number]);
   param[number].create(curr_aut);
   param[number].take_out();
   table[number].param(orig_params);
   for (a=0; a<orig_aut; a++)
     if (orig_params.has(a))
       param[number].put_in(currents.val(a));
 }
}

     //--------------------------------------------------------------------------//
     //        Initializes all rewards with the internal id of the state         //
     //--------------------------------------------------------------------------//
void FTB::init_reward()
{
#ifdef _PEPS_DEBUG_
  if (domain.is_empty())
    Programming_Error("reward definition for unknown domain", 4350)
#endif
 aut_id a;
 st_id  s;

 for (a=0; a<domain.size(); a++)
   for (s=0; s<domain.sz(a); s++)
     reward_table[a][s] = s;
}

     //--------------------------------------------------------------------------//
     //   It verifies if a function becomes constant in the process of grouping  //
     //--------------------------------------------------------------------------//
     //   Denote: P   the set of parameters of the function in the entry 'f';    //
     //           S   the set of automata whose states are known;                //
     //           SP  the intersection of P and S;                               //
     //           U   the set of automata whose states are unknown;              //
     //           UP  the intersection of P and U;                               //
     //           R   the set of automata state values for the automata in SP;   //
     //                                                                          //
     //   This function tests if the function 'f' is a constant on R.            //
     //                                                                          //
     //   Input parameters: 's' the set S denoted above;                         //
     //                     'v' the set R (restricted to automata in SP);        //
     //                                                                          //
     //   All the possible values in R of the function are computed (changes the //
     // states of the automata in UP). If the same evaluation is found, it       //
     // returns 'true' and the constant value is stored in 'c'. Otherwise, it    //
     // returns 'false'.                                                         //
     //                                                                          //
     // Note: the maximum size of 's' and the size of 'v' must be equal to the   //
     // number of automata in the original model. It means that in 'v' only the  //
     // values of the automata in 's' are relevant.                              //
     //                                                                          //
     // Obs.: This function (called in the grouping process) assumes that the    //
     // automata being grouped is being grouped for the first time.              //
     //--------------------------------------------------------------------------//
bool FTB::eval(const func_id f, const aut_st & v, const aut_set & s, rp & c) const
{
#ifdef _PEPS_DEBUG_
  if (f >= number)
    Programming_Error("index out of range", 4351)
  if (v.size() != domain.size())
    Programming_Error("bad evaluation call", 4352)
  if (s.size() != domain.size())
    Programming_Error("bad evaluation call", 4353)
#endif
 bool     answer = true;      // the answer (the function is constant)
 aut_set  up(s.size());       // the automata in UP ('aut_set' object)
 aut_list lst;                // the automata in UP ('aut_list' object)
 int      p_ss;               // the number of global states in R
 aut_st   st;                 // the global states in R

 st.copy(v);                        // puts the known values in g_st
 table[f].param(up);                // initializes 'up' with P
 up.minus(s);                       // removes S ('s') from 'up'
 p_ss = domain.state_space(up);     // computes the number of states in UP
 lst.put(up);                       // creates a 'aut_list' object with automata in UP
 st.reset(lst);                     // reset the automata of UP
 c = table[f].eval(st);
 for (int i=1; i<p_ss; i++) {          // for all the states in UP
   st.next(lst, domain);
   if (c != table[f].eval(st)) {          // verifies if they have the same evaluation
     answer = false;                      // if not, answer = false (stops the process) 
     break;                               // else keeps the answer ('true')
   }
 }                                  // if all states in UP were evaluated with the same
 return(answer);                    //    result, returns 'true'
}

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
void FTB::eval(const func_id f, const aut_st & v, const aut_set & s,
               rp & min, rp & ave, rp & max) const
{
#ifdef _PEPS_DEBUG_
  if (f >= number)
    Programming_Error("index out of range", 4354)
  if (v.size() != domain.size())
    Programming_Error("bad evaluation call", 4355)
  if (s.size() != domain.size())
    Programming_Error("bad evaluation call", 4356)
#endif
 aut_set  up(s.size());       // the automata in UP ('aut_set' object)
 aut_list lst;                // the automata in UP ('aut_list' object)
 int      p_ss;               // the number of global states in R
 aut_st   st;                 // the global states in R
 rp       buf;                // a buffer to evaluations
 rp       acc = zero;         // an accumulator to compute the average

 st.copy(v);                        // puts the known values in g_st
 table[f].param(up);                // initializes 'up' with P
 up.minus(s);                       // removes S ('s') from 'up'
 p_ss = domain.state_space(up);     // computes the number of states in UP
 lst.put(up);                       // creates a 'aut_list' object with automata in UP
 st.reset(lst);                     // reset the automata of UP
 buf = table[f].eval(st);           // initializes the output parameters
 min = buf;
 max = buf;
 for (int i=1; i<p_ss; i++) {          // for all the states in UP
   st.next(lst, domain);
   buf = table[f].eval(st);
   if (absv(min) > absv(buf))             // looks for the smallest
     min = buf;
   if (absv(max) < absv(buf))             // looks for the biggest
     max = buf;
   acc += buf;                            // accumulates for the average
 }
 ave = acc / p_ss;                        // computes the average
}


     //--------------------------------------------------------------------------//
     //     It evaluates the function in entry 'f' to any nonzero evaluation     //
     //--------------------------------------------------------------------------//
     // Note: Remember that each entry of a rate matrix must be either always    //
     // positive or always negative (if diagonal element).                       //
     //                                                                          //
     // Obs.: This function assumes that the automata being grouped is being     //
     // grouped for the first time.                                              //
     //--------------------------------------------------------------------------//
rp FTB::nonzero_eval(const func_id f) const
{
#ifdef _PEPS_DEBUG_
  if (f >= number)
    Programming_Error("index out of range", 4357)
  if (is_grouped())
    Programming_Error("nonzero_eval call over a already grouped model", 4358);
#endif
 rp       answer;              // a nonzero evaluation
 aut_list lst;                 // the parameters of 'f' ('aut_list' object)
 aut_st   st(domain.size());   // global state
 int      p_ss;                // the number of global state with different evaluations

 st.reset();                         // resets the global state
 lst.put(param[f]);                  // creates a 'aut_list' with the parameters of 'f'
 p_ss = domain.state_space(param[f]);
 for (int i=0; i<p_ss; i++) {        // for all global states with different evaluation
   answer = table[f].eval(st);          // evaluates the function located in entry 'f'
   if (answer != zero)                  // if the result is nonzero
     break;                                // stops the process
   else                                 // else
     st.next(lst, domain);                 // changes 'g_st' to the next global state
 }
#ifdef _PEPS_DEBUG_
  if (answer == zero)
    Programming_Warning("function with constant zero value", 4359)
#endif
 return(answer);
}

     //--------------------------------------------------------------------------//
     //         It writes all the function entries on the standard output        //
     //--------------------------------------------------------------------------//
void FTB::show()
{
 func_id f;

 cout << "Function Table (" << number << " functions with " << dimension << " places)\n";
 for (f=0; f<number; f++) {
   cout << "F" << f << ": ";
   table[f].show();
   cout << "\n";
 }
}
     //--------------------------------------------------------------------------//
     //         It writes the function in entry 'f' on the standard output       //
     //--------------------------------------------------------------------------//
void FTB::show(const func_id f)
{
#ifdef _PEPS_DEBUG_
  if (f >= number)
    Programming_Error("index out of range", 4360)
#endif
 table[f].show();
}
     //--------------------------------------------------------------------------//
     //         It prints the function in entry 'f' on 'fout' file stream        //
     //--------------------------------------------------------------------------//
void FTB::print(const func_id f, ofstream & fout) const
{
#ifdef _PEPS_DEBUG_
  if (f >= number)
    Programming_Error("index out of range", 4360)
#endif
 table[f].print(fout);
}
     //--------------------------------------------------------------------------//
     //          It prints all the function entries in the 'ftb.dbg' file        //
     //--------------------------------------------------------------------------//
void FTB::print() const
{
 peps_name table_name, model_name, buf_name;
 ofstream  fout;
 aut_st    bufsizes;
 aut_id    a;
 func_id   f;

 Get_Only_the_File_Name(file_name, table_name);
 DCT::dct.n_name(model_name);
 Open_File("ftb", dbg_file, fout);
 fout << "===============================================================\n"
      << "File ftb.dbg\n"
      << "===============================================================\n"
      << table_name << ".ftb -- A model with " << domain.size()
      << " original automata (" << currents.size() << " after aggregation)"
      << "\nUser name: '" << model_name << ".san')"
      << "\n------------\nGeneral Information\n------------"
      << "\nProduct state space:           " << RSS::rss.orig_size() << " states"
      << "\nState space after aggregation: " << RSS::rss.size()      << " states"
      << "\nReachable state space:         " << RSS::rss.reach_size() << " states"
      << "\nAutomata original sizes:                [ ";
 for (a=0; a<domain.size(); a++)
   fout << domain.sz(a) << " ";
 fout << "]\nAutomata attribution after aggregation: [ ";
 for (a=0; a<currents.size(); a++)
   fout << currents.sz(a) << " ";
 CND::cnd.get_s(bufsizes);
 fout << "]\nAutomata sizes after aggregation:       [ ";
 for (a=0; a<bufsizes.size(); a++)
   fout << bufsizes.sz(a) << " ";
 fout << "]\nCurrent Number of Functions: " << number
      << " of " << dimension << " places\nSize of this Table: "
      << mem_use() / mem_unit << " Kbytes"
      << "\n===============================================================\n";
 for (f=0; f<number; f++) {
   DCT::dct.gf_name(f, buf_name);
   fout << buf_name << " (F" << f << "): ";
   table[f].print(fout);
   fout << "\n";
 }
 Close_File("ftb", dbg_file, fout);
}
     //--------------------------------------------------------------------------//
     //                      Output on 'fout' file stream                        //
     //--------------------------------------------------------------------------//
     //   Remark: the parameters of each function in the table are not saved.    //
     //--------------------------------------------------------------------------//
void FTB::write() const
{
 ofstream fout;
 aut_id   a;
 st_id    s;

 Open_File(file_name, ftb_file, fout);
 fout << number << "\n";
 domain.write(fout);
 currents.write(fout);
 for (int i=0; i<number; i++)
   table[i].write(fout);
 for (a=0; a<domain.size();a++) {                // gets its reward_table
   for (s=0; s<domain.sz(a); s++)
   	fout << reward_table[a][s] << " ";
   fout << "\n";
 }
 Close_File(file_name, ftb_file, fout);
}
     //--------------------------------------------------------------------------//
     //                      Input from 'fin' file stream                        //
     //--------------------------------------------------------------------------//
     //   Remark: since the parameters of each function in the table are not     //
     // saved, this function generates those parameters from the function        //
     // formulae. Also this function allocates the reward table as soon as the   //
     // number of original automata is known.                                    //
     //--------------------------------------------------------------------------//
void FTB::read(const peps_name file)
{
 ifstream fin;
 aut_id   a;                          // counter for original automata
 st_id    s;
 aut_set  orig_params;                // buffer to store the parameters of each function
 int      orig_aut;                   // number of original automata
 int      curr_aut;                   // number of descriptor automata (groups)

 Open_File(file, ftb_file, fin);
 fin >> number;                  // gets the number of function entries
 create(number);                 // allocates 'this'
 baptise(file);

 domain.read(fin);               // gets the size of each original automata
 reward_table = new rp*[domain.size()];     // allocates the reward table
 for (a=0; a<domain.size(); a++)
   reward_table[a] = new rp[domain.sz(a)];
 orig_aut = domain.size();       // gets the number of original automata
 currents.read(fin);             // gets the the current group of each original automata
 curr_aut = currents.count_groups(); // gets the number of descriptor automata (groups)

 g_st.create(domain.size());     // allocates the buffer to the global state
 orig_params.create(orig_aut);   // allocates the buffer to store the parameters of
                                 //   each function entry

 for (number=0; number<dimension; number++) {   // for all function entries
   table[number].read(fin);                        // reads the function formulae
   table[number].param(orig_params);               // gets its parameters
   param[number].create(curr_aut);                 // translates the parameters of
   for (a=0; a<orig_aut; a++)                      //   'orig_params' (expressed in
     if (orig_params.has(a))                       //   the set of reference of the
       param[number].put_in(currents.val(a));      //   original automata) to the
 }                                                 //   set of reference of the
                                                   //   automata in the descriptor
 for (a=0; a<domain.size();a++)
   for (s=0; s<domain.sz(a); s++)                  // gets its reward_table
     fin >> reward_table[a][s];

 Close_File(file_name, ftb_file, fin);
}
     //--------------------------------------------------------------------------//
     //                It puts the 'just in time' functions in file              //
     //--------------------------------------------------------------------------//
void FTB::write_jit()
{
  int i, j;                   // counter for the functions in the table
  peps_name jit_file_name="\0";    // jit file name in the ./jit directory
  peps_name buf;              // buffer to assembly textual arguments

  ofstream fout;              // the peps_jit.C output file
//  void *handle=NULL;

  Get_Current_Dir(jit_file_name);                   // generate the peps_jit.C file
#ifdef WIN32
  strcat(jit_file_name, "./jit/peps_jit");
#else
  strcat(jit_file_name, "/jit/peps_jit");
#endif
  Open_File(jit_file_name, jit_file, fout);
  fout << "typedef double rp;\n extern \"C\" {\n";
  for(i=0; i<domain.size(); i++){
    fout << "rp t_r" << i << "[" << domain.sz(i) <<  "] ={";
    for(j=0; j<domain.sz(i)-1; j++){
      fout << reward(i,j) << ", ";
    }
    fout << reward(i,domain.sz(i)-1) << "};\n";
  }
  fout << endl;
  for(i=0; i<size(); i++){
    fout << "rp peps_jit_F" << i << "(const int * states)\n{\n";
    put_func(fout, i);
    fout << "}\n";
  }
  fout << "}\n";
  Close_File(jit_file_name, jit_file, fout);

#if defined(__MINGW32__)
  system("mingw32-g++.exe -fPIC -c -o jit/peps_jit.o jit/peps_jit.C");       // compile peps_jit.C
  system("mingw32-g++.exe -shared -o jit/peps_jit.so jit/peps_jit.o"); // generate .so library
#else
  system("g++ -c -fPIC -o jit/peps_jit.o jit/peps_jit.C");       // compile peps_jit.C
  system("g++ -shared -o jit/peps_jit.so jit/peps_jit.o"); // generate .so library
#endif
  strcpy(buf, jit_file_name);
  strcat(buf, ".so");
  if(handle)
    dlclose(handle);
  handle = dlopen (buf, RTLD_LAZY | RTLD_GLOBAL);    // load the dinamic library
  if (handle==0) {
    cerr << "**** Cannot load dynamic library " << buf << " ****\n";
  }

  if (jit_mat)                                             // if the calls have been
    delete[] jit_mat;                                      //    allocated delete it
//  jit_mat = new (rp(*)(const int *))[size()];              // allocation of calls
  jit_mat = (rp (**)(const int *)) calloc(size(), sizeof(rp (*)(const int *)));
  for (i=0; i<size(); i++) {
    //    strcpy(buf, "peps_jit_F");
    //    itoa(aux, i);
    //    strcat(buf, aux);
    sprintf(buf, "peps_jit_F%d",i);
    jit_mat[i] = (rp(*)(const int *))dlsym(handle, buf);
  }

  //  rp (*fe1[2])(const aut_st &p);
  // fe1[0] = (rp(*)(const aut_st &p))dlsym(handle, "F0");
  // fe1[1] = (rp(*)(const aut_st &p))dlsym(handle, "F2");
}

     //--------------------------------------------------------------------------//
     //              Memory space used by 'this' (in memory units)               //
     //--------------------------------------------------------------------------//
int FTB::mem_use() const
{
 int answer = 8;
 for (int i=0; i<number; i++) {
   answer += table[i].mem_use();
   answer += param[i].mem_use();
 }
 answer += domain.mem_use();
 answer += currents.mem_use();
 answer += g_st.mem_use();
#ifdef _PEPS_DEBUG_
 answer += 4 * number;
#endif
 return(answer);
}
#ifdef _PEPS_DEBUG_

     //--------------------------------------------------------------------------//
     //        It resets the counters for the number of evaluation calls         //
     //--------------------------------------------------------------------------//
void FTB::reset_trace()
{
 for (int i=0; i<dimension; i++)
   trace[i] = 0;
}
     //--------------------------------------------------------------------------//
     //         It prints the counters for the number of evaluation calls        //
     //--------------------------------------------------------------------------//
void FTB::print_trace()
{
 int j;

 cout << "Trace functions\n";
 for (int i=0; i<number; i++) {
   cout << "F" << i << ": ";
   table[i].reset();
   for (j=0; j<4; j++) {
     cout << "(" << int(table[i].get_op()) << "," << int(table[i].get_val()) << ") ";
     table[i].next();
   }
   cout << ": " << trace[i] << "\n";
 }
}
#endif

     //--------------------------------------------------------------------------//
     //                       Internal Function of allocation                    //
     //--------------------------------------------------------------------------//
void FTB::alloc()
{
 table = new func_tree[dimension];
 param = new aut_set[dimension];
 for (int i=0; i<dimension; i++) {
   table[i].create();
   param[i].create();
 }
#ifdef _PEPS_DEBUG_
 trace = new int[dimension];
#endif
}
     //--------------------------------------------------------------------------//
     //                     Internal Function of desallocation                   //
     //--------------------------------------------------------------------------//
void FTB::desal()
{
 for (int i=0; i<dimension; i++) {
   table[i].erase();
   param[i].erase();
 }
 delete[] table;
 delete[] param;
#ifdef _PEPS_DEBUG_
 delete[] trace;
#endif
}
     //--------------------------------------------------------------------------//
     //                    It creates the just in time functions                 //
     //--------------------------------------------------------------------------//
void FTB::put_func(ofstream & fout, int f) const
{ 
  char stack[max_stack_size][max_peps_name];
  int index, count=0;

  index = 0;
  for (int i=0; i<table[f].has(); i++) {
   switch (table[f].get_op()){
   case Neq    : fout << "  " << stack[index-2] << " = rp(" << stack[index-2] 
                      << " != " << stack[index-1] << ");\n";           	
                 index--;
                 break;
   case Not    : fout << "  " << stack[index-1] << " = rp(!" << stack[index-1] << ");\n";           	
                 break;
   case P_rw   : fout << "  rp var" << count << " = t_r" << table[f].get_beg() 
                      << "[states[" << table[f].get_beg() << "]];\n";
                 for(int i=table[f].get_beg()+1; i<=table[f].get_end(); i++){
                   fout << "  var" << count << " *= t_r" << i << "[states[" << i << "]];\n";
                 }
                 sprintf(stack[index], "var%d", count);
                 count++;
                 index++;
                 break;
   case S_rw   : fout << "  rp var" << count << " = t_r" << table[f].get_beg() 
                      << "[states[" << table[f].get_beg() << "]];\n";
                 for(int i=table[f].get_beg()+1; i<=table[f].get_end(); i++){
                   fout << "  var" << count << " += t_r" << i << "[states[" << i << "]];\n";
                 }
                 sprintf(stack[index], "var%d", count);
                 count++;
                 index++;
                 break;
   case Rw     : fout << "  rp var" << count << " = t_r" << table[f].get_val() << "[states[" 
                      << table[f].get_val() << "]];\n";
                 sprintf(stack[index], "var%d", count);
                 index++;
                 count++;
                 break;
   case Cp_rw  : fout << "  rp var" << count << " = 1.0;\n";
                 for(int i=table[f].get_beg(); i<=table[f].get_end(); i++){
                   fout << "  if(" << table[f].get_val() << " == states[" << i << "])\n" 
                        << "    var" << count << " *= t_r" << i << "[states[" 
                        << i << "]];\n";
                 }
                 sprintf(stack[index], "var%d", count);
                 count++;
                 index++;
                 break;
   case Cs_rw  : fout << "  rp var" << count << " = 0.0;\n";
                 for(int i=table[f].get_beg(); i<=table[f].get_end(); i++){
                   fout << "  if(" << table[f].get_val() << " == states[" << i << "])\n" 
                        << "    var" << count << " += t_r" << i << "[states[" 
                        << i << "]];\n";
                 }
                 sprintf(stack[index], "var%d", count);
                 count++;
                 index++;
                 break;
   case And    : fout << "  " << stack[index-2] << " = rp(" << stack[index-2] 
                      << " && " << stack[index-1] << ");\n";           	
                 index--;
                 break;
   case Times  : fout << "  " << stack[index-2] << " = " << stack[index-2] << " * " 
                      << stack[index-1] << ";\n";
                 index--;
                 break; 
   case Plus   : fout << "  " << stack[index-2] << " = " << stack[index-2] << " + " 
                      << stack[index-1] << ";\n";
                 index--;
                 break; 
   case Slash  : fout << "  " << stack[index-2] << " = " << stack[index-2] << " / " 
                      << stack[index-1] << ";\n";
                 index--;
                 break;
   case Minus  : fout << "  " << stack[index-2] << " = " << stack[index-2] << " - " 
                      << stack[index-1] << ";\n";
                 index--;
                 break;
   case Imp    : fout << "  " << stack[index-2] << " = ( " << stack[index-2] 
                      << " && " << stack[index-1] << ") || !(" << stack[index-2]
                      << ");\n";
                 break;
   case Cnst   : fout << "  rp var" << count << " = " << table[f].get_val() << ";\n";
                 sprintf(stack[index],"var%d",count);
                 count++;
                 index++;
                 break;
   case Less   : fout << "  " << stack[index-2] << " = rp(" << stack[index-2] 
                      << " < " << stack[index-1] << ");\n";           	
                 index--;  
                 break;
   case Dimp   : fout << "  " << stack[index-2] << " = ( " << stack[index-2] 
                      << " && " << stack[index-1] << ") || (!(" 
                      << stack[index-2] << ") && !(" << stack[index-1] << "));\n";
                 break;
   case Leq    : fout << "  " << stack[index-2] << " = rp(" << stack[index-2] 
                      << " <= " << stack[index-1] << ");\n";           	
                 index--;  
                 break;
   case Eq     : fout << "  " << stack[index-2] << " = rp(" << stack[index-2] 
                      << " == " << stack[index-1] << ");\n";           	
                 index--;  
                 break;
   case Gret   : fout << "  " << stack[index-2] << " = rp(" << stack[index-2] 
                      << " > " << stack[index-1] << ");\n";           	
                 index--;  
                 break;
   case Geq    : fout << "  " << stack[index-2] << " = rp(" << stack[index-2] 
                      << " >= " << stack[index-1] << ");\n";           	
                 index--;  
                 break;
   case St     : fout << "  rp var" << count << " = states[" << (int)table[f].get_val() << "];\n";
                 sprintf(stack[index], "var%d", count);
                 index++;
                 count++;
                 break;
   case Cs_st  : fout << "  rp var" << count << " = 0;\n  for(int i="  << table[f].get_beg() 
                      << "; i<=" << table[f].get_end() << "; i++){\n    if(states[i]==" 
                      << (int)table[f].get_val() << "){\n" << "      var" << count << "++;\n    }\n  }\n";
                 sprintf(stack[index], "var%d", count);
                 count++;
                 index++;
                 break;
   case Xor    : fout << "  " << stack[index-2] << " = rp(" << stack[index-2] << " xor " 
                      << stack[index-1] << ");\n";
                 index--;  
                 break;
   case Mini   : fout << "  if(" << stack[index-1] << " < " << stack[index-2] << "){\n"
                      << "    " << stack[index-2] << " = " << stack[index-1] << ";\n  }\n";
                 index--;
                 break;
   case Maxi   : fout << "  if(" << stack[index-1] << " > " << stack[index-2] << "){\n"
                      << "    " << stack[index-2] << " = " << stack[index-1] << ";\n  }\n";
                 index--;
                 break;
   case Or     : fout << "  " << stack[index-2] << " = rp(" << stack[index-2] 
                      << " || " << stack[index-1] << ");\n";           	
                 index--;  
                 break;
   }
   table[f].next();
  }
  fout << "  return(" << stack[0] << ");\n";  
}

