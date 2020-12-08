//====================================================================================//
//                                                                                    //
//                             Functions Table Structure                              //
//                                                                                    //
//====================================================================================//
//  This File:   ftb.inl                    Language: C++  (xlC and CC)               //
//  Software:    PEPS 3.1                    Advisor: Brigitte.Plateau@imag.fr        //
//  Error Code:  4300                                                                 //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 07/nov/02                        by: paulof@inf.pucrs.br             //
//====================================================================================//

     //--------------------------------------------------------------------------//
     //                          Empty Constructor                               //
     //--------------------------------------------------------------------------//
inline FTB::FTB()
{
 strcpy(file_name, "\0");
 dimension = 0;
 number    = 0;
 domain.create();
 currents.create();
 g_st.create();
}
     //--------------------------------------------------------------------------//
     //                          Sized Constructor                               //
     //--------------------------------------------------------------------------//
inline FTB::FTB(const int dim)
{
 dimension = dim;
 number    = 0;
 alloc();
 domain.create();
 currents.create();
 g_st.create();
}
     //--------------------------------------------------------------------------//
     //                           Copy Constructor                               //
     //--------------------------------------------------------------------------//
inline FTB::FTB(const FTB & t)
{
 dimension = t.dimension;
 number    = t.number;
 for (int i=0; i<dimension; i++) {
  table[i].copy(t.table[i]);
  param[i].copy(t.param[i]);
 }
 domain.copy(t.domain);
 currents.copy(t.currents);
 g_st.create(domain.size());
}
     //--------------------------------------------------------------------------//
     //                              Destructor                                  //
     //--------------------------------------------------------------------------//
inline FTB::~FTB()
{
 if (dimension != 0 ) {
   desal();
   dimension = 0;
   number    = 0;
 }
 if (!(domain.is_empty())) {
   for (aut_id a=0; a<domain.size(); a++)
     delete[] reward_table[a];
   delete reward_table;
 }
 domain.erase();
 currents.erase();
 g_st.erase();
 strcpy(file_name, "\0");
}

     //--------------------------------------------------------------------------//
     //                           Empty Intialization                            //
     //--------------------------------------------------------------------------//
inline void FTB::create()
{
 strcpy(file_name, "\0");
 dimension = 0;
 number    = 0;
 domain.create();
 currents.create();
 g_st.create();
}
     //--------------------------------------------------------------------------//
     //                           Sized Intialization                            //
     //--------------------------------------------------------------------------//
inline void FTB::create(const int dim)
{
 erase();
 dimension = dim;
 number    = 0;
 alloc();
 domain.create();
 currents.create();
 g_st.create();
}
     //--------------------------------------------------------------------------//
     //                       Copy and Sized Intialization                       //
     //--------------------------------------------------------------------------//
inline void FTB::copy(const FTB & t)
{
 if (dimension != t.dimension) {
   erase();
   dimension = t.dimension;
   alloc();
 }
 number = t.number;
 for (int i=0; i<dimension; i++) {
  table[i].copy(t.table[i]);
  param[i].copy(t.param[i]);
 }
 domain.copy(t.domain);
 currents.copy(t.currents);
 g_st.create(domain.size());
}
     //--------------------------------------------------------------------------//
     //                            Destruction                                   //
     //--------------------------------------------------------------------------//
inline void FTB::erase()
{
 if (dimension != 0) {
   desal();
   dimension = 0;
   number = 0;
 }
 if (!(domain.is_empty())) {
   for (aut_id a=0; a<domain.size(); a++)
     delete[] reward_table[a];
   delete reward_table;
 }
 domain.erase();
 currents.erase();
 g_st.erase();
 strcpy(file_name, "\0");
}


     //--------------------------------------------------------------------------//
     //               The current number of functions is zero                    //
     //--------------------------------------------------------------------------//
inline bool FTB::is_empty() const
{
 return(number == 0);
}
     //--------------------------------------------------------------------------//
     //               There is no more place for new functions                   //
     //--------------------------------------------------------------------------//
inline bool FTB::is_full() const
{
 return(dimension == number);
}
     //--------------------------------------------------------------------------//
     //                  The model has already been grouped                      //
     //--------------------------------------------------------------------------//
inline bool FTB::is_grouped() const
{
#ifdef _PEPS_DEBUG_
  if ((domain.size() == 0) || (currents.size() == 0))
    Programming_Error("is_grouped test over a non-initialized table", 4301)
#endif
 return(domain.size() != currents.count_groups());
}

     //--------------------------------------------------------------------------//
     //                          It assigns a file name                          //
     //--------------------------------------------------------------------------//
inline void FTB::baptise(const peps_name file)
{
 strcpy(file_name, file);
}
     //--------------------------------------------------------------------------//
     //                         It returns the file name                         //
     //--------------------------------------------------------------------------//
inline void FTB::name(peps_name file) const
{
 strcpy(file, file_name);
}
     //--------------------------------------------------------------------------//
     //                     The maximum number of functions                      //
     //--------------------------------------------------------------------------//
inline int FTB::size() const
{
 return(dimension);
}
     //--------------------------------------------------------------------------//
     //                     The current number of functions                      //
     //--------------------------------------------------------------------------//
inline int FTB::has() const
{
 return(number);
}
     //--------------------------------------------------------------------------//
     //                     The number of original automata                      //
     //--------------------------------------------------------------------------//
inline int FTB::aut() const
{
 return(currents.size());
}
     //--------------------------------------------------------------------------//
     //                     The current number of automata                       //
     //--------------------------------------------------------------------------//
inline int FTB::g_aut() const
{
 return(currents.count_groups());
}
     //--------------------------------------------------------------------------//
     //                     The original state space product                     //
     //--------------------------------------------------------------------------//
inline int FTB::ss() const
{
 return(domain.state_space());
}
     //--------------------------------------------------------------------------//
     //                     The original state space product                     //
     //--------------------------------------------------------------------------//
inline aut_id FTB::g_aut(const aut_id a) const
{
 return(currents.val(a));
}

     //--------------------------------------------------------------------------//
     //         The reward of the state 's' of the original automaton 'a'        //
     //--------------------------------------------------------------------------//
inline rp FTB::reward(const aut_id a, const st_id s) const
{
#ifdef _PEPS_DEBUG_
  if (domain.is_empty())
    Programming_Error("reward definition for unknown domain", 4302)
  if (a >= domain.size())
    Programming_Error("index out of range", 4303)
  if (s >= domain.sz(a))
    Programming_Error("index out of range", 4304)
#endif
 return(reward_table[a][s]);
}
     //--------------------------------------------------------------------------//
     // It puts 'r' as the reward of the state 's' of the original automaton 'a' //
     //--------------------------------------------------------------------------//
inline void FTB::reward(const aut_id a, const st_id s, const rp r)
{
#ifdef _PEPS_DEBUG_
  if (domain.is_empty())
    Programming_Error("reward definition for unknown domain", 4305)
  if (a >= domain.size())
    Programming_Error("index out of range", 4306)
  if (s >= domain.sz(a))
    Programming_Error("index out of range", 4307)
#endif
 reward_table[a][s] = r;
}

     //--------------------------------------------------------------------------//
     //                   It Includes function 't' in 'this'                     //
     //--------------------------------------------------------------------------//
inline func_id FTB::put(const func_tree & t)
{
#ifdef _PEPS_DEBUG_
  if (number == dimension)
    Programming_Error("FTB structure full", 4308)
  if (!(t.well_def(true)))
    Programming_Error("Inclusion of a spurious function", 4309)
  if (currents.size() == 0)
    Programming_Error("Inclusion of a function in a non-initialized table", 4310)
#endif
 int     orig_aut =  domain.size();             // original number of automata
 int     curr_aut =  currents.count_groups();   // current number of automata
 aut_id  a;                                     // counter for original automata
 aut_set orig_params;                           // buffer to the original parameters

 table[number].copy(t);
 orig_params.create(orig_aut);
 param[number].create(curr_aut);
 t.param(orig_params);
 for (a=0; a<orig_aut; a++)
   if (orig_params.has(a))
     param[number].put_in(currents.val(a));
 number++;
 return(number-1);
}
     //--------------------------------------------------------------------------//
     //            It copies the function located in entry 'f' to 't'            //
     //--------------------------------------------------------------------------//
inline void FTB::get(const func_id f, func_tree & t) const
{
#ifdef _PEPS_DEBUG_
  if (f >= number)
    Programming_Error("index out of range", 4311)
#endif
 t.copy(table[f]);
}

     //--------------------------------------------------------------------------//
     //      It fills 's' with the parameters of the function located in the     //
     //          entry 'f' according to the original number of automata          //
     //--------------------------------------------------------------------------//
inline void FTB::params(const func_id f, aut_set & s) const
{
#ifdef _PEPS_DEBUG_
  if (f >= number)
    Programming_Error("index out of range", 4312)
  if (s.size() != domain.size())
    Programming_Error("bad parameters call", 4313)
#endif
 table[f].param(s);
}
     //--------------------------------------------------------------------------//
     //      It fills 's' with the parameters of the function located in the     //
     //          entry 'f' according to the current number of automata           //
     //--------------------------------------------------------------------------//
inline void FTB::g_params(const func_id f, aut_set & s) const
{
#ifdef _PEPS_DEBUG_
  if (f >= number)
    Programming_Error("index out of range", 4314)
  if (s.size() != currents.count_groups())
    Programming_Error("bad parameters call", 4315)
#endif
 s.copy(param[f]);
}

     //--------------------------------------------------------------------------//
     //         It evaluates the function in entry 'f' for a global state        //
     //--------------------------------------------------------------------------//
     //    For the first function the global state is directly given by the      //
     // input parameter 's'. In this case the size of 's' must be equal to the   //
     // original number of automata (size of the variable 'domain').             //
     //--------------------------------------------------------------------------//
inline rp FTB::eval(const func_id f, const aut_st & s) const
{
#ifdef _PEPS_DEBUG_
  if (f >= number)
    Programming_Error("index out of range", 4316)
  if (s.size() != domain.size())
    Programming_Error("wrong global state size", 4317)
  trace[f]++;
#endif
 return(table[f].eval(s));
}
     //--------------------------------------------------------------------------//
     //         It evaluates the function in entry 'f' for a global state        //
     //--------------------------------------------------------------------------//
     //    For the second function the global state is given the individual      //
     // state of each current automata. The number of current automata is given  //
     // by the number of different labels in the variable 'current'. To obtain   //
     // the global state according to the original number of automata the array  //
     // of state maps 'm' is used.                                               //
     //--------------------------------------------------------------------------//
inline rp FTB::g_eval(const func_id f, const aut_st & s, const st_map * m)
{
#ifdef _PEPS_DEBUG_
  if (f >= number)
    Programming_Error("index out of range", 4318)
  if (s.size() != currents.count_groups())
    Programming_Error("wrong global state size", 4319)
  trace[f]++;
#endif
 rp answer;

 if (is_grouped()) {
   for (aut_id a=0; a<s.size(); a++)
     m[a].get(s.st(a), g_st);
   answer = table[f].eval(g_st);
 }
 else
   answer = table[f].eval(s);
 return(answer);
}
     //--------------------------------------------------------------------------//
     //         It evaluates the function in entry 'f' for a global state        //
     //--------------------------------------------------------------------------//
     //    For the third function the global state is also given (like in the    //
     // previous function - g_eval). But in this thrid function (jit_g_eval) the //
     // evaluation is made using functions compiled 'just-in-time'.              //
     //--------------------------------------------------------------------------//
inline rp FTB::jit_g_eval(const func_id f, const aut_st & s, const st_map * m)
{
#ifdef _PEPS_DEBUG_
  if (f >= number)
    Programming_Error("index out of range", 4320)
  if (s.size() != currents.count_groups())
    Programming_Error("wrong global state size", 4321)
  trace[f]++;
#endif
 rp answer;

 if (is_grouped()) {
   for (aut_id a=0; a<s.size(); a++)
     m[a].get(s.st(a), g_st);
   answer = jit_mat[f](g_st.st_addr());            //***********************
 }
 else {
   answer = jit_mat[f](s.st_addr());               //***********************
 }
 return(answer);
}
     //--------------------------------------------------------------------------//
     //                 It puts the sizes of the original automata               //
     //--------------------------------------------------------------------------//
inline void FTB::put_domain(const aut_st & s)
{
 domain.copy(s);
 reward_table = new rp*[domain.size()];     // allocates the reward table
 for (aut_id a=0; a<domain.size(); a++)
   reward_table[a] = new rp[domain.sz(a)];
 g_st.create(domain.size());                // allocates the buffer 'g_st'
}
     //--------------------------------------------------------------------------//
     //            It Fills 's' with the original sizes of the automata          //
     //--------------------------------------------------------------------------//
inline void FTB::get_domain(aut_st & s) const
{
 s.copy(domain);
}
     //--------------------------------------------------------------------------//
     //           It puts the current positions of the original automata         //
     //--------------------------------------------------------------------------//
inline void FTB::put_currents(const aut_st & c)
{
  currents.copy(c);
}
     //--------------------------------------------------------------------------//
     //     It Fills 'c' with the current positions of the original automata     //
     //--------------------------------------------------------------------------//
inline void FTB::get_currents(aut_st & c) const
{
 c.copy(currents);
}
