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
//  Last Change: 11/feb/99                        by: Paulo.Fernandes@imag.fr         //
//====================================================================================//

     //--------------------------------------------------------------------------//
     //                         Empty Constructor                                //
     //--------------------------------------------------------------------------//
inline DSC::DSC()
{
 strcpy(file_name, "\0");
 model_type = Discrete;
 automata   = 0;
 events     = 0;
}
     //--------------------------------------------------------------------------//
     //                         Sized Constructor                                //
     //--------------------------------------------------------------------------//
inline DSC::DSC(const md_type m, const aut_id a, const ev_id e)
{
 model_type = m;
 automata   = a;
 events     = e;
 alloc();
}
     //--------------------------------------------------------------------------//
     //                          Copy Constructor                                //
     //--------------------------------------------------------------------------//
inline DSC::DSC(const DSC & d)
{
 model_type = d.model_type;
 automata   = d.automata;
 events     = d.events;
 alloc();
 trans(d);
}
     //--------------------------------------------------------------------------//
     //                            Destructor                                    //
     //--------------------------------------------------------------------------//
inline DSC::~DSC()
{
 if (automata != 0) {
   desal();
   model_type = Discrete;
   automata   = 0;
   events     = 0;
 }
 strcpy(file_name, "\0");
}

     //--------------------------------------------------------------------------//
     //                         Empty Intialization                              //
     //--------------------------------------------------------------------------//
inline void DSC::create()
{
 strcpy(file_name, "\0");
 model_type = Discrete;
 automata   = 0;
 events     = 0;
}
     //--------------------------------------------------------------------------//
     //                         Sized Intialization                              //
     //--------------------------------------------------------------------------//
inline void DSC::create(const md_type m, const aut_id a, const ev_id e)
{
 erase();
 model_type = m;
 automata   = a;
 events     = e;
 alloc(); 
}
     //--------------------------------------------------------------------------//
     //                     Copy and Sized Intialization                         //
     //--------------------------------------------------------------------------//
inline void DSC::copy(const DSC & d)
{
 erase();
 model_type = d.model_type;
 automata   = d.automata;
 events     = d.events;
 alloc();
 trans(d);
}
     //--------------------------------------------------------------------------//
     //                            Destruction                                   //
     //--------------------------------------------------------------------------//
inline void DSC::erase()
{
 if (automata != 0) {
   desal();
   automata  = 0;
   events    = 0;
   strcpy(file_name, "\0");
 }
}

     //--------------------------------------------------------------------------//
     //                       It assigns a file name                             //
     //--------------------------------------------------------------------------//
inline void DSC::baptise(const peps_name file)
{
 strcpy(file_name, file);
}
     //--------------------------------------------------------------------------//
     //                   It assigns the original file name                      //
     //--------------------------------------------------------------------------//
inline void DSC::o_baptise(const peps_name file)
{
 strcpy(orig_name, file);
}
     //--------------------------------------------------------------------------//
     //                       It returns the file name                           //
     //--------------------------------------------------------------------------//
inline void DSC::name(peps_name file) const
{
 strcpy(file, file_name);
}
     //--------------------------------------------------------------------------//
     //                  It returns the original file name                       //
     //--------------------------------------------------------------------------//
inline void DSC::o_name(peps_name file) const
{
 strcpy(file, orig_name);
}
     //--------------------------------------------------------------------------//
     //                     The total number of automata                         //
     //--------------------------------------------------------------------------//
inline int DSC::size() const
{
 return(automata);
}
     //--------------------------------------------------------------------------//
     //                           The model type                                 //
     //--------------------------------------------------------------------------//
inline md_type DSC::type() const
{
 return(model_type);
}
     //--------------------------------------------------------------------------//
     //                     The total number of automata                         //
     //--------------------------------------------------------------------------//
inline aut_id DSC::aut() const
{
 return(automata);
}
     //--------------------------------------------------------------------------//
     //                The total number of synchronizing events                  //
     //--------------------------------------------------------------------------//
inline ev_id DSC::ev() const
{
 return(events);
}
     //--------------------------------------------------------------------------//
     //                     The product state space size                         //
     //--------------------------------------------------------------------------//
inline int DSC::ss() const
{
 return(sizes.state_space());
}
     //--------------------------------------------------------------------------//
     //                        There is no automata                              //
     //--------------------------------------------------------------------------//
inline bool DSC::is_empty() const
{
 return(automata == 0);
}
     //--------------------------------------------------------------------------//
     //                  The descriptor was already grouped                      //
     //--------------------------------------------------------------------------//
     // Implementation: A descriptor is non-grouped if all state maps are        //
     // elementaries.                                                            //
     //                                                                          //
     // Obs.: The state map of an automaton 'a' is elementary if it concerns     //
     // only the automaton 'a'.                                                  //
     //--------------------------------------------------------------------------//
inline bool DSC::is_grouped() const
{
 bool answer = false;

 for (aut_id a=0; a<automata; a++)
   if (!(state_map[a].is_singular()) || (state_map[a].get(0) != a)) {
     answer = true;
     break;
   }
 return(answer);
}

     //--------------------------------------------------------------------------//
     //                  The solution computational cost                         //
     //--------------------------------------------------------------------------//
inline rp DSC::cost() const
{
 return(l_cost() + s_cost());
}

     //--------------------------------------------------------------------------//
     //             The computational cost of the local matrices                 //
     //--------------------------------------------------------------------------//
inline rp DSC::l_cost() const
{
 rp acc      = 0;
 rp prod_acc = 1;
 rp sum_acc  = 0;
 
 //local matrices cost
 for(int a=0; a<aut(); a++){
   prod_acc *= local_mat[a].size();	
   sum_acc += ((double)local_mat[a].nz()/(double)local_mat[a].size());
 }
 
 acc = prod_acc * sum_acc;
 return(acc);
}
     //--------------------------------------------------------------------------//
     //           The computational cost of the synchronized matrices            //
     //--------------------------------------------------------------------------//
inline rp DSC::s_cost() const
{
 rp acc      = 0;
 rp prod_acc = 1;
 rp sum_acc  = 0;
 
 if(ev()==0){
   return(0);
 }

 //synchronized positive matrices cost
 for(int e=0; e<ev(); e++){
   prod_acc = 1;
   sum_acc  = 0;
   for(int a=0; a<aut(); a++){
     prod_acc *= synch_mat[e][a].size();	
     sum_acc += ((double)synch_mat[e][a].nz()/(double)synch_mat[e][a].size());
   }
   acc += prod_acc * sum_acc;  
 }

 //synchronized negative matrices cost
 for(int e=0; e<ev(); e++){
   prod_acc = 1;
   sum_acc  = 0;
   for(int a=0; a<aut(); a++){
     prod_acc *= synch_adj[e][a].size();	
     sum_acc += ((double)synch_adj[e][a].nz()/(double)synch_adj[e][a].size());
   }
   acc += prod_acc * sum_acc;  
 }
 
 return(acc);
}

     //--------------------------------------------------------------------------//
     //            The computational cost of the synchronized event              //
     //--------------------------------------------------------------------------//
inline rp DSC::s_cost(int s) const
{
 rp acc      = 0;
 rp prod_acc = 1;
 rp sum_acc  = 0;
 
 prod_acc = 1;
 sum_acc  = 0;
 for(int a=0; a<aut(); a++){
   prod_acc *= synch_mat[s][a].size();	
   sum_acc += ((double)synch_mat[s][a].nz()/(double)synch_mat[s][a].size());
 }
 acc += prod_acc * sum_acc;  

 prod_acc = 1;
 sum_acc  = 0;
 for(int a=0; a<aut(); a++){
   prod_acc *= synch_adj[s][a].size();	
   sum_acc += ((double)synch_adj[s][a].nz()/(double)synch_adj[s][a].size());
 }
 acc += prod_acc * sum_acc;  
  
 return(acc);
}


     //--------------------------------------------------------------------------//
     //                    The alternative computation cost                      //
     //--------------------------------------------------------------------------//
inline rp DSC::acost() const
{
 return(l_acost() + s_acost());
}

     //--------------------------------------------------------------------------//
     //          The alternative computation cost of the local matrices          //
     //--------------------------------------------------------------------------//
inline rp DSC::l_acost() const
{
 rp  acc      = 1;
 int big_aut  = 0; 
 int aut_i    = 0;

 for(int i=0; i<aut(); i++){
   if(local_mat[i].size() > big_aut){
     big_aut = local_mat[i].nz();
     aut_i = i;
   }
 }
 
 //local matrices cost
 for(int a=0; a<aut(); a++){
   if(a!=aut_i)
     acc *=local_mat[a].nz();	
 }
 acc *= ((aut()-2) + local_mat[aut_i].size() + local_mat[aut_i].nz());
 
 return(acc);
}
     //--------------------------------------------------------------------------//
     //        The alternative computation cost of the synchronized matrices     //
     //--------------------------------------------------------------------------//
inline rp DSC::s_acost() const
{
 rp  acc      = 0;
 rp  prod_acc = 1;
 int big_aut  = 0; 
 int aut_i    = 0;

 if(ev()==0){
   return(0);
 }

 
 //synchronized matrices cost
 for(int e=0; e<ev(); e++){
   prod_acc = 1;
   big_aut = 0;
   for(int i=0; i<aut(); i++){
     if(synch_mat[e][i].size() > big_aut){
       big_aut = synch_mat[e][i].nz();
       aut_i = i;
     }
   }
   for(int a=0; a<aut(); a++){
     if(a!=aut_i)
       prod_acc *= synch_mat[e][a].nz();	
   }
   prod_acc *= ((aut()-2) + synch_mat[e][aut_i].size() + synch_mat[e][aut_i].nz());
   acc += prod_acc;
 }

 //adjust synchronized matrices cost
 for(int e=0; e<ev(); e++){
   prod_acc = 1;
   big_aut = 0;
   for(int i=0; i<aut(); i++){
     if(synch_mat[e][i].size() > big_aut){
       big_aut = synch_mat[e][i].nz();
       aut_i = i;
     }
   }
   for(int a=0; a<aut(); a++){
     if(a!=aut_i)
       prod_acc *= synch_adj[e][a].nz();	
   }
   prod_acc *= ((aut()-2) + synch_adj[e][aut_i].size() + synch_adj[e][aut_i].nz());
   acc += prod_acc;
 }

 return(acc);
}

     //--------------------------------------------------------------------------//
     //        The alternative computation cost of the synchronized matrices     //
     //--------------------------------------------------------------------------//
inline rp DSC::s_acost(int s) const
{
 rp  acc      = 0;
 rp  prod_acc = 1;
 int big_aut  = 0; 
 int aut_i    = 0;

 if(ev()==0){
   return(0);
 }

 for(int i=0; i<aut(); i++){
   if(synch_mat[s][i].size() > big_aut){
     big_aut = synch_mat[s][i].nz();
     aut_i = i;
   }
 }
 
 prod_acc = 1;
 for(int a=0; a<aut(); a++){
   if(a!=aut_i)
     prod_acc *= synch_mat[s][a].nz();	
 }
 prod_acc *= ((aut()-2) + synch_mat[s][aut_i].size() + synch_mat[s][aut_i].nz());
 acc += prod_acc;

 prod_acc = 1;
 for(int a=0; a<aut(); a++){
   if(a!=aut_i)
     prod_acc *= synch_adj[s][a].nz();	
 }
 prod_acc *= ((aut()-2) + synch_adj[s][aut_i].size() + synch_adj[s][aut_i].nz());
 acc += prod_acc;

 return(acc);
}

     //--------------------------------------------------------------------------//
     //                           Access Functions                               //
     //--------------------------------------------------------------------------//
     //    The following member functions are used to access the internal        //
     // structures of the descriptor. The functions 'put' store the informations //
     // in the input parameter to the respective descriptor internal structure.  //
     // The functions 'get' fill the output parameter with the respective        //
     // descriptor internal structure.                                           //
     //--------------------------------------------------------------------------//

     //--------------------------------------------------------------------------//
     //                        The size of all automata                          //
     //--------------------------------------------------------------------------//
inline void DSC::put_s(const aut_st & s)
{
#ifdef _PEPS_DEBUG_
  if (s.size() != automata)
    Programming_Error("wrong number of automata sizes", 5304)
#endif
 sizes.copy(s);
}
inline void DSC::get_s(aut_st & s) const
{
 s.copy(sizes);
}

     //--------------------------------------------------------------------------//
     //                      The size of the automaton 'a'                       //
     //--------------------------------------------------------------------------//
inline void DSC::put_s(const aut_id a, const int s)
{
#ifdef _PEPS_DEBUG_
  if (a >= automata)
    Programming_Error("index out of range", 5305)
#endif
 sizes.put(a, s);
}
inline int DSC::get_s(const aut_id a) const
{
#ifdef _PEPS_DEBUG_
  if (a >= automata)
    Programming_Error("index out of range", 5306)
#endif
 return(sizes.sz(a));
}

     //--------------------------------------------------------------------------//
     //                 The local matrix of the automaton 'a'                    //
     //--------------------------------------------------------------------------//
inline void DSC::put_mat(const int a, const san_mat & m)
{
#ifdef _PEPS_DEBUG_
  if (a >= automata)
    Programming_Error("index out of range", 5307)
  if (m.size() != sizes.sz(a))
    Programming_Error("Wrong sized matrix", 5308)
#endif
 local_mat[a].copy(m);
}
inline void DSC::get_mat(const int a, san_mat & m) const
{
#ifdef _PEPS_DEBUG_
  if (a >= automata)
    Programming_Error("index out of range", 5309)
#endif
 m.copy(local_mat[a]);
}

     //--------------------------------------------------------------------------//
     //    The matrix of the event 'e' for the automaton 'a' of type 'pm'        //
     //--------------------------------------------------------------------------//
inline void DSC::put_mat(const ev_id e, const aut_id a, const p_m pm, const san_mat & m)
{
#ifdef _PEPS_DEBUG_
  if (a >= automata)
    Programming_Error("index out of range", 5310)
  if (e >= events)
    Programming_Error("index out of range", 5311)
  if (m.size() != sizes.sz(a))
    Programming_Error("wrong sized matrix", 5312)
#endif
 if (pm == Mat_plus)
   synch_mat[e][a].copy(m);
 else
   synch_adj[e][a].copy(m);
}
inline void DSC::get_mat(const ev_id e, const aut_id a, const p_m pm, san_mat & m) const
{
#ifdef _PEPS_DEBUG_
  if (a >= automata)
    Programming_Error("index out of range", 5313)
  if (e >= events)
    Programming_Error("index out of range", 5314)
#endif
 if (pm == Mat_plus)
   m.copy(synch_mat[e][a]);
 else
   m.copy(synch_adj[e][a]);
}

     //--------------------------------------------------------------------------//
     //                     The state map of automaton 'a'                       //
     //--------------------------------------------------------------------------//
inline void DSC::put_sm(const aut_id a, const st_map & s)
{
#ifdef _PEPS_DEBUG_
  if (a >= automata)
    Programming_Error("index out of range", 5315)
  if (s.row() != sizes.sz(a))
    Programming_Error("wrong sized state map", 5316)
#endif
 state_map[a].copy(s);
}
inline void DSC::get_sm(const int a, st_map & s) const
{
#ifdef _PEPS_DEBUG_
  if (a >= automata)
    Programming_Error("index out of range", 5317)
#endif
 s.copy(state_map[a]);
}

     //--------------------------------------------------------------------------//
     //              The automaton whith the rates for all events                //
     //--------------------------------------------------------------------------//
inline void DSC::put_sr(const aut_id * a)
{
 for (ev_id e=0; e<events; e++)
   sync_rates[e] = a[e];
}
inline void DSC::get_sr(aut_id * a) const
{
 for (ev_id e=0; e<events; e++)
   a[e] = sync_rates[e];
}

     //--------------------------------------------------------------------------//
     //              The automaton whith the rates for event 'e'                 //
     //--------------------------------------------------------------------------//
inline void DSC::put_sr(const ev_id e, const aut_id a)
{
#ifdef _PEPS_DEBUG_
  if (a >= automata)
    Programming_Error("index out of range", 5318)
  if (e >= events)
    Programming_Error("index out of range", 5319)
#endif
 sync_rates[e] = a;
}
inline aut_id DSC::get_sr(const ev_id e) const
{
#ifdef _PEPS_DEBUG_
  if (e >= events)
    Programming_Error("index out of range", 5320)
#endif
 return(sync_rates[e]);
}

     //--------------------------------------------------------------------------//
     //                  The symetric labels of all automata                     //
     //--------------------------------------------------------------------------//
inline void DSC::put_sms(const aut_st & s)
{
#ifdef _PEPS_DEBUG_
  if (s.size() != automata)
    Programming_Error("wrong sized ", 5321)
#endif
 symetric.copy(s);
}
inline void DSC::get_sms(aut_st & s) const
{
 s.copy(symetric);
}

     //--------------------------------------------------------------------------//
     //                  The symetric label of automaton 'a'                     //
     //--------------------------------------------------------------------------//
inline void DSC::put_sms(const aut_id a, const int s)
{
#ifdef _PEPS_DEBUG_
  if (a >= automata)
    Programming_Error("index out of range", 5322)
#endif
 symetric.stamp(a, s);
}
inline int DSC::get_sms(const aut_id a) const
{
#ifdef _PEPS_DEBUG_
  if (a >= automata)
    Programming_Error("index out of range", 5323)
#endif
 return(symetric.val(a));
}

     //--------------------------------------------------------------------------//
     //                  The type of automaton 'a'                     //
     //--------------------------------------------------------------------------//
inline mat_typ DSC::get_type(ev_id e, const aut_id  a){
return(synch_mat[e][a].tp());
}

inline mat_typ DSC::get_type(const aut_id  a){
return(local_mat[a].tp());
}
