//====================================================================================//
//                                                                                    //
//                   Continuous-Time Normalized Descriptor Structure                  //
//                                                                                    //
//====================================================================================//
//  This File:   cnd_inl.h                  Language: C++  (xlC and CC)               //
//  Software:    PEPS 3.1                    Advisor: Brigitte.Plateau@imag.fr        //
//  Error Codes: 5400                                                                 //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 23/mar/01                        by: Anne.Benoit@imag.fr             //
//====================================================================================//

     //--------------------------------------------------------------------------//
     //                         Empty Constructor                                //
     //--------------------------------------------------------------------------//
inline CND::CND()
{
 strcpy(file_name, "\0");
 automata    = 0;
 events      = 0;
 state_space = 0;
 reach_space = 0;
}
     //--------------------------------------------------------------------------//
     //                         Sized Constructor                                //
     //--------------------------------------------------------------------------//
inline CND::CND(const aut_id a, const ev_id e, const int s, const int r)
{
 automata    = a;
 events      = e;
 state_space = s;
 reach_space = r;
 alloc();
}
     //--------------------------------------------------------------------------//
     //                          Copy Constructor                                //
     //--------------------------------------------------------------------------//
inline CND::CND(const CND & d)
{
 automata    = d.automata;
 events      = d.events;
 state_space = d.state_space;
 reach_space = d.reach_space;
 alloc();
 trans(d);
}
     //--------------------------------------------------------------------------//
     //                            Destructor                                    //
     //--------------------------------------------------------------------------//
inline CND::~CND()
{
 if (automata != 0) {
   desal();
   automata    = 0;
   events      = 0;
   state_space = 0;
   reach_space = 0;
 }
 strcpy(file_name, "\0");
}

     //--------------------------------------------------------------------------//
     //                         Empty Intialization                              //
     //--------------------------------------------------------------------------//
inline void CND::create()
{
 strcpy(file_name, "\0");
 automata    = 0;
 events      = 0;
 state_space = 0;
 reach_space = 0;
}
     //--------------------------------------------------------------------------//
     //                         Sized Intialization                              //
     //--------------------------------------------------------------------------//
inline void CND::create(const aut_id a, const ev_id e, const int s, const int r)
{
 erase();
 automata    = a;
 events      = e;
 state_space = s;
 reach_space = r;
 alloc(); 
}
     //--------------------------------------------------------------------------//
     //                     Copy and Sized Intialization                         //
     //--------------------------------------------------------------------------//
inline void CND::copy(const CND & d)
{
 erase();
 automata    = d.automata;
 events      = d.events;
 state_space = d.state_space;
 reach_space = d.reach_space;
 alloc();
 trans(d);
}
     //--------------------------------------------------------------------------//
     //                            Destruction                                   //
     //--------------------------------------------------------------------------//
inline void CND::erase()
{
 if (automata != 0) {
   desal();
   automata    = 0;
   events      = 0;
   state_space = 0;
   reach_space = 0;
 }
 strcpy(file_name, "\0");
}

     //--------------------------------------------------------------------------//
     //                       It assigns a file name                             //
     //--------------------------------------------------------------------------//
inline void CND::baptise(const peps_name file)
{
 strcpy(file_name, file);
}
     //--------------------------------------------------------------------------//
     //                   It assigns the original file name                      //
     //--------------------------------------------------------------------------//
inline void CND::o_baptise(const peps_name file)
{
 strcpy(orig_name, file);
}
     //--------------------------------------------------------------------------//
     //                       It returns the file name                           //
     //--------------------------------------------------------------------------//
inline void CND::name(peps_name file) const
{
 strcpy(file, file_name);
}
     //--------------------------------------------------------------------------//
     //                  It returns the original file name                       //
     //--------------------------------------------------------------------------//
inline void CND::o_name(peps_name file) const
{
 strcpy(file, orig_name);
}
     //--------------------------------------------------------------------------//
     //                     The total number of automata                         //
     //--------------------------------------------------------------------------//
inline int CND::size() const
{
 return(automata);
}
     //--------------------------------------------------------------------------//
     //                     The total number of automata                         //
     //--------------------------------------------------------------------------//
inline aut_id CND::aut() const
{
 return(automata);
}
     //--------------------------------------------------------------------------//
     //                The total number of synchronizing events                  //
     //--------------------------------------------------------------------------//
inline ev_id CND::ev() const
{
 return(events);
}
     //--------------------------------------------------------------------------//
     //                     The product state space size                         //
     //--------------------------------------------------------------------------//
inline int CND::ss() const
{
 return(state_space);
}
     //--------------------------------------------------------------------------//
     //                     The reachable state space size                       //
     //--------------------------------------------------------------------------//
inline int CND::rs() const
{
 return(reach_space);
}
     //--------------------------------------------------------------------------//
     //                        There is no automata                              //
     //--------------------------------------------------------------------------//
inline bool CND::is_empty() const
{
 return(automata == 0);
}
     //--------------------------------------------------------------------------//
     //                       The normalizing factor                             //
     //--------------------------------------------------------------------------//
inline rp CND::n_factor() const
{
 return(factor);
}
     //--------------------------------------------------------------------------//
     //                      The largest automata size                           //
     //--------------------------------------------------------------------------//
inline int CND::largest_aut() const
{
 return(sizes.biggest());
}
     //--------------------------------------------------------------------------//
     //                      The smallest automata size                          //
     //--------------------------------------------------------------------------//
inline int CND::smallest_aut() const
{
 return(sizes.smallest());
}

     //--------------------------------------------------------------------------//
     //                     The solution computation cost                        //
     //--------------------------------------------------------------------------//
inline rp CND::cost() const
{
 return(l_cost() + s_cost());
}

     //--------------------------------------------------------------------------//
     //                The computation cost of the local matrices                //
     //--------------------------------------------------------------------------//
inline rp CND::l_cost() const
{
 rp acc      = 0;
 rp prod_acc = 1;
 rp sum_acc  = 0;
 
 //local matrices cost
 for(int a=0; a<aut(); a++){
   prod_acc*=local_mat[a].size();	
   sum_acc+=((double)local_mat[a].nz()/(double)local_mat[a].size());
 }
 cout << prod_acc << endl;  
 cout << sum_acc << endl;  
 
 acc = prod_acc * sum_acc;
 
 return(acc);
}
     //--------------------------------------------------------------------------//
     //             The computation cost of the synchronized matrices            //
     //--------------------------------------------------------------------------//
inline rp CND::s_cost() const
{
 rp acc      = 0;
 rp prod_acc = 1;
 rp sum_acc  = 0;
 
 if(ev()==0){
   return(0);
 }

 //synchronized matrices cost
 for(int e=0; e<ev(); e++){
   prod_acc = 1;
   sum_acc  = 0;
   for(int a=0; a<aut(); a++){
     prod_acc*=synch_mat[e][a].size();	
     sum_acc+=((double)synch_mat[e][a].nz()/(double)synch_mat[e][a].size());
   }
   acc += prod_acc * sum_acc;  
 }

 acc += prod_acc;
 return(acc);
}

     //--------------------------------------------------------------------------//
     //                    The alternative computation cost                      //
     //--------------------------------------------------------------------------//
inline rp CND::acost() const
{
 return(l_acost() + s_acost());
}

     //--------------------------------------------------------------------------//
     //          The alternative computation cost of the local matrices          //
     //--------------------------------------------------------------------------//
inline rp CND::l_acost() const
{
 rp  acc      = 1;
 int big_aut  = 0; 
 int aut_i    = 0;

 for(int i=0; i<aut(); i++){
   if(local_mat[i].size() > big_aut){
     big_aut = local_mat[i].size();
     aut_i = i;
   }
 }
 
 //local matrices cost
 for(int a=0; a<aut(); a++){
   if(a!=aut_i)
     acc *= local_mat[a].nz();	
 }
 acc *= (2*aut()-3) + local_mat[aut_i].nz();
 
 return(acc);
}
     //--------------------------------------------------------------------------//
     //        The alternative computation cost of the synchronized matrices     //
     //--------------------------------------------------------------------------//
inline rp CND::s_acost() const
{
 rp  acc      = 0;
 rp  prod_acc = 1;
 int big_aut  = 0; 
 int aut_i    = 0;

 if(ev()==0){
   return(0);
 }

 for(int i=0; i<aut(); i++){
   if(synch_mat[0][i].size() > big_aut){
     big_aut = synch_mat[0][i].size();
     aut_i = i;
   }
   prod_acc *= synch_mat[0][i].size();	   
 }

 acc += prod_acc; // diagonal cost 
 
 //synchronized matrices cost
 for(int e=0; e<ev(); e++){
   prod_acc = 1;
   for(int a=0; a<aut(); a++){
     if(a!=aut_i)
       prod_acc *=synch_mat[e][a].nz();	
   }
   prod_acc *= (2*aut()-3) + synch_mat[e][aut_i].nz();
   acc+=prod_acc;
 }

 return(acc);
}

     //--------------------------------------------------------------------------//
     //                   It adds one to each diagonal element                   //
     //--------------------------------------------------------------------------//
inline void CND::add_identity()
{
 if (PRF::prf.vec_impl() == v_full)
   for (int i=0; i<state_space; i++)	
     diagonal[i] += one;	
 else
   for (int i=0; i<reach_space; i++)	
     diagonal[i] += one;

}
     //--------------------------------------------------------------------------//
     //                It subtracts one from each diagonal element               //
     //--------------------------------------------------------------------------//
inline void CND::minus_identity()
{
 if (PRF::prf.vec_impl() == v_full)
  for (int i=0; i<state_space; i++)
    diagonal[i] -= one;
 else
  for (int i=0; i<reach_space; i++)
    diagonal[i] -= one;
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
inline void CND::put_s(const aut_st & s)
{
#ifdef _PEPS_DEBUG_
  if (s.size() != automata)
    Programming_Error("wrong number of automata sizes", 5401)
#endif
 sizes.copy(s);
}
inline void CND::get_s(aut_st & s) const
{
 s.copy(sizes);
}
     //--------------------------------------------------------------------------//
     //                           The original jumps                             //
     //--------------------------------------------------------------------------//
inline void CND::get_oj(jumps & j) const
{
 j.copy(orig_jumps);
}

     //--------------------------------------------------------------------------//
     //                       The 'p'-th diagonal element                        //
     //--------------------------------------------------------------------------//
inline rp CND::get_diag(const int p)
{
 return(diagonal[p]);
}

     //--------------------------------------------------------------------------//
     //                       The local term type                                //
     //--------------------------------------------------------------------------//
inline term_typ CND::get_l_type(const int p)
{
 return(l_type[p]);
}
     //--------------------------------------------------------------------------//
     //                       The synch term type                                //
     //--------------------------------------------------------------------------//
inline term_typ CND::get_s_type(const int p)
{
 return(s_type[p]);
}
     //--------------------------------------------------------------------------//
     //                       The synch term type for the factor                 //
     //--------------------------------------------------------------------------//
inline fact_typ CND::get_sf_type(const int e, const int i)
{
 return(sf_type[e][i]);
}
     //--------------------------------------------------------------------------//
     //            The sanmat appointed by the event i and the position i        //
     //--------------------------------------------------------------------------//
inline san_mat CND::get_synch_mat(const int e, const int i)
{
 return(synch_mat[e][i]);
}
     //--------------------------------------------------------------------------//
     //            The state map                                                 //
     //--------------------------------------------------------------------------//
inline st_map *CND::get_state_map()
{
 return(state_map);
}
     //--------------------------------------------------------------------------//
     //            The s_D_order                                                 //
     //--------------------------------------------------------------------------//
inline aut_list CND::get_s_D_order(const int i) {
 return(s_D_order[i]);
}
     //-------------------------------------------------------------------------//
     //            Gets the functional cut                                       //
     //--------------------------------------------------------------------------//
inline int CND::get_fcut(const int i) {
 return (fcuts[i]);
}
     //--------------------------------------------------------------------------//
     //            The s_C_order                                                 //
     //--------------------------------------------------------------------------//
inline aut_list CND::get_s_C_order(const int e, const int a) {
 return(s_C_order[e][a]);
}
     //--------------------------------------------------------------------------//
     //            The s_B_order                                                 //
     //--------------------------------------------------------------------------//
inline aut_list CND::get_s_B_order(const int e) {
 return(s_B_order[e]);
}
