//====================================================================================//
//                                                                                    //
//                            Class Probability Vector                                //
//                                                                                    //
//====================================================================================//
//  This File:   vct.C                      Language: C++  (xlC and CC)               //
//  Software:    PEPS 3.1                    Advisor: Brigitte.Plateau@imag.fr        //
//  Error Codes: 2350                                                                 //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 13/feb/03                        by: Anne.Benoit@imag.fr             //
//====================================================================================//
#include "peps.h"

     //--------------------------------------------------------------------------//
     //                 static variable: The Probability Vector                  //
     //--------------------------------------------------------------------------//
VCT VCT::vct = VCT();

     //--------------------------------------------------------------------------//
     //               It initializes the states of 'this' - CND                  //
     //--------------------------------------------------------------------------//
     //    This function is used to initialize the elements of 'this' according  //
     // to one of the following options ('o'):                                   //
     //                                                                          //
     //   - 'eq_vec',      all reachable states are initialized with an uniform  //
     //                    distribution (equiprobable);                          //
     //   - 'ap_vec',      all reachable states are initialized with a           //
     //                    distribution proportional to the inverse of the       //
     //                    diagonal elements of the descriptor ('CND::cnd');     //
     //                                                                          //
     //   The size of the vector and the size of each automata is taken from the //
     // descriptor 'CND::cnd'. The field 'permutation' is set to the normal      //
     // order { 0, 1, ..., N-1 }.                                                //
     //                                                                          //
     // Note: this functions is used to prepare a vector to be multiplied by the //
     // continuous-time normalized descriptor (static variable 'CND::cnd').      //
     //--------------------------------------------------------------------------//
     // Implementation note: This function uses two types of coordinate for      //
     // global states. The coordinates using the variable 'i' concern the rank   //
     // of the global states in the original model. The coordinates using the    //
     // variable 'j' concern only the "existing" states. After a process of      //
     // grouping some global states can be eliminated, the static variable       //
     // 'RSS::rss' keeps the information of which global states are "existing",  //
     // i.e., were not eliminated. All coordinates to the functions of the class //
     // 'RSS::rss' must be in the coordinates of the original model. However the //
     // structures 'VCT::vct' and 'CND::cnd' only accepts the coordinates of the //
     // current model (the coordinates of the "remaining states - see 'rss.h').  //
     //--------------------------------------------------------------------------//
void VCT::init(const v_typ o)
{
#ifdef _PEPS_DEBUG_
  if (CND::cnd.ss() != RSS::rss.size())
    Programming_Error("incompatibility between RSS and CND", 2350)
#endif
 int i;             // counter for all states
 int j;             // counter for the remaining states
 int k;             // counter for the index in spvec
 rp  factor;        // a factor to normalize the vector elements
 rp  acc = zero;    // an accumulator to normalize the vector elements

 if (PRF::prf.vec_impl() == v_sparse)
   create(RSS::rss.reach_size(), CND::cnd.ss());      // allocates the vector elements
 else
   create(CND::cnd.ss(), CND::cnd.ss());              // allocates the vector elements

 p_order.create(CND::cnd.aut());       // allocates the 'p_order' list object
 p_order.put();                        // fills 'permutation' with 0, 1, ... N-1
 CND::cnd.get_oj(p_jumps);             // copy the original jumps
 CND::cnd.get_s(sizes);                // copy the size of the automata
 switch (o) {
   case eq_vec :
     factor = one / rp(RSS::rss.reach_size());    // computes the probability
     k=0;
     for (i=0, j=0; i<RSS::rss.orig_size(); i++)  // for all states
       if (RSS::rss.remains(i)) {                    // if it was not eliminated
         if (RSS::rss.has(i)) {                         // if it is reachable
	     vec[j] = factor;                           // puts the probability
	     if (is_sparse)
	       positions[j] = i;                        // puts the position
	     j++;
	 }
         else                                           // if it is not reachable
	   if (!is_sparse) {
	     vec[j] = zero;                             // puts zero
	     j++;                                       // the next remaining state
	   }
       }
     break;
   case ap_vec :
     k=0;
     for (i=0, j=0; i<RSS::rss.orig_size(); i++)  // for all states
       if (RSS::rss.remains(i)) {                    // if it was not eliminated
         if (RSS::rss.has(i)) {                         // if it is reachable
	   vec[j] = one / CND::cnd.get_diag(j);          // puts the inverse of the
	   acc += vec[j];                                // corresp. diagonal element
	                                                 // and add to the accumulator
	   if (is_sparse)
	       positions[j] = i;                         // puts the position
	   j++;
	 }
         else                                             // if it is not reachable
	   if (!is_sparse) {
	     vec[j] = zero;                               // puts zero
	     j++;                                         // the next remaining state
	   }
       }
     factor = one / acc;             // normalizes the vector
     for (j=0; j<dimension; j++)
       vec[j] *= factor;
     
 }
}

     //--------------------------------------------------------------------------//
     //               It initializes the states of 'this' - HBF                  //
     //--------------------------------------------------------------------------//
     //    This function is used to initialize the elements of 'this' according  //
     // to one of the following options:                                         //
     //                                                                          //
     //   - 'eq_vec',      all reachable states are initialized with an uniform  //
     //                    distribution (equiprobable);                          //
     //   - 'ap_vec',      all reachable states are initialized with a           //
     //                    distribution proportional to the inverse of the       //
     //                    diagonal elements of the HBF matrix ('HBF::hbf');     //
     //                                                                          //
     //   The size of the vector is to the HBF matrix order. The fields 'sizes'  //
     // and 'permutation' are erased.                                            //
     //                                                                          //
     // Note: this functions is used to prepare a vector to be multiplied by the //
     // Hardwell-Boeing Format sparse matrix (static variable 'HBF::hbf').       //
     //--------------------------------------------------------------------------//
     // Implementation note: The generation of HBF matrices in PEPS eliminates   //
     // all non-reachable states. Therefore, the order of the HBF matrix is the  //
     // number of reachable states.                                              //
     //--------------------------------------------------------------------------//
void VCT::init_hbf(const v_typ o)
{
 int i;             // a counter for the states
 rp  factor;        // a factor to normalize the vector elements
 rp  acc = zero;    // an accumulator to normalize the vector elements

 create(HBF::hbf.order(), HBF::hbf.order());  // allocates the vector elements
 switch (o) {
   case eq_vec :
     factor = one / rp(dimension);           // computes the probability
     for (i=0; i<dimension; i++)             // for all states
       vec[i] = factor;                         // puts the probability	 
     break;
   case ap_vec :
     for (i=0; i<dimension; i++) {           // for all states
       vec[i] = one / HBF::hbf.coord(i, i);     // puts the inverse of the
       acc += vec[i];                        // corresp. diagonal element
     }                                          // and add to the accumulator
     factor = one / acc;                  // normalizes the vector
     for (i=0; i<dimension; i++)
       vec[i] *= factor;
     break;
 }
}

     //--------------------------------------------------------------------------//
     //       It permutes the vector elements acording to a lexical order        //
     //--------------------------------------------------------------------------//
     //   The vector elements are sorted according to a lexical order expressed  //
     // by the list of automata 'permutation'. This function changes from the    //
     // current order (expressed in the variables 'p_order' and 'p_jumps') to a  //
     // new order (expressed by the input parameters 's' and 'j'). The vector    //
     // elements are permuted acording to this new list.                         //
     //--------------------------------------------------------------------------//
void VCT::permut(const aut_list & s, const jumps & j)
{
#ifdef _PEPS_DEBUG_
  if (!(s.is_full()))
    Programming_Error("incomplete list", 2351)
  if (s.size() != p_order.size())
    Programming_Error("different list sizes", 2352)
  if (j.size() != p_jumps.size())
    Programming_Error("different jump sizes", 2353)
#endif
      // rp     * new_vec;
      // rp     * fullvec;
      // int    * new_positions;
 aut_st   st;
 int      k;
 int      idx = 0;
 rp * new_vec;

 new_vec = new rp[dimension];
 
 st.create(sizes.size());
 st.reset();

 if (is_sparse) { // totally sparse
   vector<gs_pair> stl_vec;
   vector<gs_pair>::const_iterator iter;
   gs_pair stl_tmp;
   int * new_positions;
   new_positions = new int[dimension];
   stl_vec.reserve(dimension);

   int k2=0;
   for (k=0; k<real_dim; k++) {
     if (positions[k2]==k) {
       stl_tmp.first = idx;
       stl_tmp.second = vec[k2];
       stl_vec.push_back(stl_tmp);
       k2++;
     }
     else if (positions[k2]==-1) 
       k = real_dim;
     st.next(p_order, sizes, j, idx);
   }
   
   // stl_vec contains the new vector. We sort it and copy it
   // into vec and positions
   sort(stl_vec.begin(), stl_vec.end(), comp());
   iter = stl_vec.begin();
   k2=0;
   while (iter != stl_vec.end()) {
     new_positions[k2] = (*iter).first;
     new_vec[k2] = (*iter).second;
     ++iter; k2++;
   }
   
   // For the last case
   if (k2<dimension)
     new_positions[k2] = -1;
   
   delete[] positions;
   positions = new_positions;
 }
 
 else // full probability vectors
   for (int i=0; i<dimension; i++) {
     new_vec[idx] = vec[i];
     st.next(p_order, sizes, j, idx);
   }
 
 // always done
 p_order.copy(s);
 p_jumps.copy(j);
 delete[] vec;
 vec = new_vec;
 
}

     //--------------------------------------------------------------------------//
     //          It copies one vector elements to another (permuting)            //
     //--------------------------------------------------------------------------//
void VCT::becomes(const VCT & v, const aut_list & s, const jumps & j)
{
#ifdef _PEPS_DEBUG_
  if (dimension != v.dimension)
    Programming_Error("different sized vectors", 2354)
  if (!(s.is_full()))
    Programming_Error("incomplete list", 2355)
  if (s.size() != p_order.size())
    Programming_Error("different list sizes", 2356)
  if (j.size() != p_jumps.size())
    Programming_Error("different jump sizes", 2357)
#endif
 aut_st   st;
 int      k, k2;
 int      idx = 0;
 sizes.copy(v.sizes);
 st.create(sizes.size());
 st.reset();

 if (is_sparse) { // totally sparse
   vector<gs_pair> stl_vec;
   vector<gs_pair>::const_iterator iter;
   gs_pair stl_tmp;
   
   stl_vec.reserve(dimension);
   k2=0;
   for (k=0; k<real_dim; k++) {
     if (v.positions[k2]==k) {
       stl_tmp.first = idx;
       stl_tmp.second = v.vec[k2];
       stl_vec.push_back(stl_tmp);
       k2++;
     }
     st.next(v.p_order, sizes, j, idx);
   }
   
   // stl_vec contains the new vector. We sort it and copy it
   // into vec and positions
   sort(stl_vec.begin(), stl_vec.end(), comp());
   iter = stl_vec.begin();
   k2=0;
   while (iter != stl_vec.end()) {
     positions[k2] = (*iter).first;
     vec[k2] = (*iter).second;
     ++iter; k2++;
   }
 }
 else
   for (int i=0; i<dimension; i++) {
     vec[idx] = v.vec[i];
     st.next(v.p_order, sizes, j, idx);
   }
 
 // Always done
 p_order.copy(s);
 p_jumps.copy(j);
 
}

     //--------------------------------------------------------------------------//
     //           It adds one vector elements to another (permuting)             //
     //--------------------------------------------------------------------------//
void VCT::operator += (const VCT & v)
{
#ifdef _PEPS_DEBUG_
  if (dimension != v.dimension)
    Programming_Error("different sized vectors", 2358)
#endif
 aut_st   st;
 int      idx = 0;

 st.create(sizes.size());
 st.reset();
 
 if (is_sparse) { // totally sparse
   vector<gs_pair> stl_vec;
   vector<gs_pair>::const_iterator iter;
   gs_pair stl_tmp;
   
   stl_vec.reserve(dimension);
   int k2=0;
   for (int k=0; k<real_dim; k++) {
     if (v.positions[k2]==k) {
       stl_tmp.first = idx;
       stl_tmp.second = v.vec[k2];
       stl_vec.push_back(stl_tmp);
       k2++;
     }
     else if (v.positions[k2] == -1)
       k=real_dim; // get out of the loop
     
     st.next(v.p_order, sizes, p_jumps, idx);
   }
   
   // stl_vec contains the new vector. We sort it and copy it
   // into vec and positions
   sort(stl_vec.begin(), stl_vec.end(), comp());
   iter = stl_vec.begin();
   k2=0;
   while (iter != stl_vec.end()) {
     if (positions[k2] == -1)
       break;
     else if (positions[k2] < (*iter).first)
       k2++;
     else if (positions[k2] > (*iter).first)
       ++iter;
     else {
       vec[k2] += (*iter).second;
       ++iter;
       k2++;
     }
   }
 }

 else // full vector implementation
   for (int i=0; i<dimension; i++) {
     vec[idx] += v.vec[i];
     st.next(v.p_order, sizes, p_jumps, idx);
   }
 
}



     //--------------------------------------------------------------------------//
     //           It adds one vector elements to another (normalizing)           //
     //--------------------------------------------------------------------------//
     //             Second version - normalizes the biggest residue              //
     //--------------------------------------------------------------------------//
void VCT::norm2_sum(const VCT & v)
{
#ifdef _PEPS_DEBUG_
  if (dimension != v.dimension)
    Programming_Error("different sized vectors", 2359)
#endif
 int  k;
 rp   factor, min_factor = one;

 if (is_sparse)
   cout << "VCT::norm2_sum not yet implemented for sparse\n";
 
 else {
   
   for (k=0; k<dimension; k++)               // searches the minimal factor
     if ((v.vec[k] + vec[k]) < zero) {
       factor = v.vec[k] / -vec[k];             // if the result will be negative
       if (factor < min_factor)
	 min_factor = factor;
     }
     else
       if ((v.vec[k] + vec[k]) > one) {         // if the result will be bigger than one
	 factor = (one - v.vec[k]) / vec[k];
	 if (factor < min_factor)
	   min_factor = factor;
       }
   scale(min_factor);                        // normalizes the residue ('this')
   for (k=0; k<dimension; k++)               // it performs   'this = v + this'
     vec[k] = v.vec[k] + vec[k];
   if (factor != one)                        // if a normalization was performed
     cout << one / factor << " norm. factor (" << factor << ")\n";  // inform it!  
 } 
}

     //--------------------------------------------------------------------------//
     //           It adds one vector elements to another (normalizing)           //
     //--------------------------------------------------------------------------//
     //               First version - cut negatives and normalizes               //
     //--------------------------------------------------------------------------//
void VCT::norm_sum(const VCT & v)
{
#ifdef _PEPS_DEBUG_
  if (dimension != v.dimension)
    Programming_Error("different sized vectors", 2360)
#endif
 int  k;
 rp   acc = zero;
 rp   factor;
 rp   max_cut = zero;
 rp   total_cut = zero;
 int  cuts = 0;

 // if (is_sparse)
 //  cout << "VCT::norm_sum not yet implemented for sparse\n";

 // else {
   for (k=0; k<dimension; k++) {         // it performs   'this = v + this'
     vec[k] = v.vec[k] + vec[k];
     if (vec[k] < zero) {                // if one of 'this' elements becomes negative
       if (absv(vec[k]) > max_cut)
	 max_cut = absv(vec[k]);
       cuts++;
       total_cut += absv(vec[k]);
       vec[k] = zero;                     // "cuts" the element off (it becomes zero)
       //   vec[k] = v.vec[k];     or     // keeps the old value
       //   vec[k] = vec[k];       or     // keeps it negative (do nothing)
     }
     acc += vec[k];                       // computes the sum of the elements of 'this'
   }
   factor = one / acc;                    // it normalizes 'this'
   for (k=0; k<dimension; k++)
     vec[k] *= factor;
   if (max_cut > close_to_zero)       // if at least a significant "cut" was performed
     cout << cuts << " cuts (" << max_cut << " - " << total_cut << ")\n";  // inform it!
   // }
}

     //--------------------------------------------------------------------------//
     //                      Output on 'fout' file stream                        //
     //--------------------------------------------------------------------------//
void VCT::write() const
{
 ofstream fout;
 Open_File(file_name, vct_file, fout);
 fout << dimension << "\n";
 fout << real_dim << "\n";
 sizes.write(fout);
 p_order.write(fout);
 p_jumps.write(fout);
 int k=0;

 for (int i=0; i<dimension; i++)
   fout << vec[i] << "\n";
 
 Close_File(file_name, vct_file, fout);
}
     //--------------------------------------------------------------------------//
     //                      Input from 'fin' file stream                        //
     //--------------------------------------------------------------------------//
void VCT::read(const peps_name file)
{
 ifstream fin;
 int d, s;
// int indice, proba;
 
 Open_File(file, vct_file, fin);
 fin >> d;
 fin >> s;
 create(d,s);
 baptise(file);
 sizes.read(fin);
 p_order.read(fin);
 p_jumps.read(fin);

 if (is_sparse) {
   int j=0;
   for (d=0; d<real_dim; d++)
     if (RSS::rss.has(d)) {
       fin >> vec[j];
       positions[j] = d;
       j++;
     }
 }
 else
   for (d=0; d<dimension; d++)
     fin >> vec[d];
 

 Close_File(file_name, vct_file, fin);
}
     //--------------------------------------------------------------------------//
     //              Memory space used by 'this' (in memory units)               //
     //--------------------------------------------------------------------------//
int VCT::mem_use() const
{
  int answer = 4;
 
  // dimension is the size of RSS, it works for both sparse and full vectors
  answer += dimension * 12;
  answer += p_order.mem_use();
  answer += p_jumps.mem_use();
  answer += sizes.mem_use();

  return(answer);
}

     //--------------------------------------------------------------------------//
     //                       Static Variables Allocation                        //
     //--------------------------------------------------------------------------//
     //    It allocates the internal static variables 't', 'z_in', 'z_out' and   //
     //    'g_st' according to the current values of the 'CND::cnd' structure    //
     //--------------------------------------------------------------------------//
void VCT::alloc_static()
{

  int nr;
  //cout << " debut de VCT::alloc_static()\n";

  // creation and initialization of the temporary vector t (static)  
  if (PRF::prf.vec_impl() == v_sparse)
    t.create(RSS::rss.reach_size(), CND::cnd.ss());     
  else
    t.create(CND::cnd.ss(), CND::cnd.ss()); 
  
  t.get_CND();

  if (! is_sparse)
    z_out = new rp[CND::cnd.largest_aut()];   // allocation of the output "slice"
  
  else if (PRF::prf.z_impl() == zin_full) {
    nr = CND::cnd.ss() / CND::cnd.smallest_aut();
    zinf = new rp*[nr];
    indice_r = new int[nr];
    useful_f = new int*[nr];
    place = new int*[nr];

    used_z = new bool[nr];
    for (int i=0; i<nr; i++) {
      zinf[i] = new rp[CND::cnd.largest_aut()];
      useful_f[i] = new int[CND::cnd.largest_aut()];
      place[i] = new int[CND::cnd.largest_aut()];
    }
  }
  

  // We always need it (in mnps)
  z_in  = new rp[CND::cnd.largest_aut()];   // allocation of the input "slice"
  
  g_st.create(CND::cnd.aut());              // allocation of the global state (CND size)

  //cout << " fin de VCT::alloc_static()\n";

}

     //--------------------------------------------------------------------------//
     //                      Static Variables Desallocation                      //
     //--------------------------------------------------------------------------//
     //   It desallocates the internal static variables 't, z_in, z_out, g_st'   //
     //--------------------------------------------------------------------------//
void VCT::desalloc_static()
{

  //  cout << " VCT::desalloc_static()\n";
  
  t.erase();                                 // desallocates all temporary structures

  if (! is_sparse)
    delete[] z_out;
    
  else if (PRF::prf.z_impl() == zin_full) {
    for (int i=0; i<(CND::cnd.ss() / CND::cnd.smallest_aut()); i++) {
      delete[] zinf[i];
      delete[] useful_f[i];
      delete[] place[i];
    }
    delete[] zinf;
    delete[] indice_r;
    delete[] useful_f;
    delete[] place;
    delete[] used_z;
  }
  

  delete[] z_in;
  delete[] aunfs;

  g_st.erase();
}


