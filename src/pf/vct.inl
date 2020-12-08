//====================================================================================//
//                                                                                    //
//                            Class Probability Vector                                //
//                                                                                    //
//====================================================================================//
//  This File:   vct_inl.h                  Language: C++  (xlC and CC)               //
//  Software:    PEPS 3.1                    Advisor: Brigitte.Plateau@imag.fr        //
//  Error Codes: 2300                                                                 //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 13/feb/03                        by: Anne.Benoit@imag.fr             //
//====================================================================================//

     //--------------------------------------------------------------------------//
     //                          Empty Constructor                               //
     //--------------------------------------------------------------------------//
inline VCT::VCT()
{

// cout << "VCT::VCT()\n";

 dimension = 0;
 p_order.create();
 p_jumps.create();
 sizes.create();
 if (PRF::prf.vec_impl() == v_full)
	is_sparse = false;
 else
	is_sparse = true;
}
     //--------------------------------------------------------------------------//
     //                          Sized Constructor                               //
     //--------------------------------------------------------------------------//
inline VCT::VCT(const int dim, const int rdim)
{

// cout << "VCT::VCT(const int dim, const int size)\n";

 dimension = dim;
 real_dim = rdim;

 if (PRF::prf.vec_impl() == v_full)
	is_sparse = false;
 else
	is_sparse = true;
 
 vec = new rp[dimension];

 if (is_sparse)
   positions = new int[dimension];
 else
   positions = new int[1];

 p_order.create();
 p_jumps.create();
 sizes.create();
}
     //--------------------------------------------------------------------------//
     //                           Copy Constructor                               //
     //--------------------------------------------------------------------------//
inline VCT::VCT(const VCT & v)
{

 // cout << "VCT::VCT(const VCT & v)\n";

 dimension = v.dimension;
 real_dim = v.real_dim;

 if (v.is_sparse) {
   vec = new rp[dimension];
   positions = new int[dimension];
   for (int i=0; i<dimension; i++) {
     vec[i] = v.vec[i];
     positions[i] = v.positions[i];
   }
 }
 else {
   vec = new rp[dimension];
   positions = new int[1];
   for (int i=0; i<dimension; i++)
     vec[i] = v.vec[i];
 }
  
 p_order.copy(v.p_order);
 p_jumps.copy(v.p_jumps);
 sizes.copy(v.sizes);
 baptise(v.file_name);

}
     //--------------------------------------------------------------------------//
     //                              Destructor                                  //
     //--------------------------------------------------------------------------//
inline VCT::~VCT()
{

// cout << "VCT::~VCT()\n";

 if (dimension != 0) {
   delete[] positions;
   delete[] vec;
   dimension = 0;
 }
 p_order.erase();
 p_jumps.erase();
 sizes.erase();
 strcpy(file_name, "\0");
}

     //--------------------------------------------------------------------------//
     //                           Empty Intialization                            //
     //--------------------------------------------------------------------------//
inline void VCT::create()
{

// cout << "VCT::create()\n";

 dimension = 0;
 p_order.create();
 p_jumps.create();
 sizes.create();
 if (PRF::prf.vec_impl() == v_full)
	is_sparse = false;
 else
	is_sparse = true;
}
     //--------------------------------------------------------------------------//
     //                           Sized Intialization                            //
     //--------------------------------------------------------------------------//
inline void VCT::create(const int dim, const int rdim)
{

// cout << "VCT::create(dim : " << dim << ", rdim " << rdim << ")   - \n";

 if (PRF::prf.vec_impl() == v_full)
	is_sparse = false;
 else
	is_sparse = true;

// cout << is_sparse << "\n";

// if (dimension != dim) {
   erase();
   dimension = dim;
   real_dim = rdim;
   if (is_sparse) {
	vec = new rp[dimension];
	positions = new int[dimension];
   }
   else {
   	vec = new rp[dimension];
	positions = new int[1];
   }
// }

 p_order.erase();
 p_jumps.erase();
 sizes.erase();
}
     //--------------------------------------------------------------------------//
     //                       Copy and Sized Intialization                       //
     //--------------------------------------------------------------------------//
inline void VCT::copy(const VCT & v)
{

// cout << "VCT::copy(const VCT & v)\n";

 create(v.dimension, v.real_dim);
 if (v.is_sparse) {
   for (int i=0; i<dimension; i++) {	
     vec[i] = v.vec[i];
     positions[i] = v.positions[i];
   }
 }
 else {
   for (int i=0; i<dimension; i++)	
     vec[i] = v.vec[i];
 }

 p_order.copy(v.p_order);
 p_jumps.copy(v.p_jumps);
 sizes.copy(v.sizes);
 baptise(v.file_name);

}
     //--------------------------------------------------------------------------//
     //                            Destruction                                   //
     //--------------------------------------------------------------------------//
inline void VCT::erase()
{

// cout << "VCT::erase()\n";

 if (dimension != 0) {
   delete[] vec;
   delete[] positions;

   dimension = 0;
   strcpy(file_name, "\0");
 }
 p_order.erase();
 p_jumps.erase();
 sizes.erase();
}

     //--------------------------------------------------------------------------//
     //                          It assigns a file name                          //
     //--------------------------------------------------------------------------//
inline void VCT::baptise(const peps_name file)
{
 strcpy(file_name, file);
}
     //--------------------------------------------------------------------------//
     //                         It returns the file name                         //
     //--------------------------------------------------------------------------//
inline void VCT::name(peps_name file) const
{
 strcpy(file, file_name);
}
     //--------------------------------------------------------------------------//
     //                        The total number of vector elements               //
     //--------------------------------------------------------------------------//
inline int VCT::size() const
{
 return(dimension);
}
     //--------------------------------------------------------------------------//
     //                        The total number of states                        //
     //--------------------------------------------------------------------------//
inline int VCT::real_size() const
{
 return(real_dim);
}
     //--------------------------------------------------------------------------//
     //                    The number of automata of the model                   //
     //--------------------------------------------------------------------------//
inline int VCT::aut() const
{
 return(sizes.size());
}
     //--------------------------------------------------------------------------//
     //                    The total number of vector elements is zero           //
     //--------------------------------------------------------------------------//
inline bool VCT::is_empty() const
{
 return(dimension == 0);
}
     //--------------------------------------------------------------------------//
     //                The vector is in the original lexical order               //
     //--------------------------------------------------------------------------//
inline bool VCT::is_not_permuted() const
{
 bool answer = true;
 for (int a=0; a<p_order.size(); a++)
   if (p_order.pos(a) != a) {                // if 'p_order' is different of
     answer = false;                         // the original order (0, 1, ... N-1)
     break;                                  // returns 'false'
   }
 return(answer);
}

     //--------------------------------------------------------------------------//
     //                   The probability of the state 's'                       //
     //           's' is the index of the probability in the vector              //
     //--------------------------------------------------------------------------//
inline rp VCT::get(const int s) const
{


#ifdef _PEPS_DEBUG_
  if (s >= dimension)
    Programming_Error("index out of range", 2301)
#endif

return(vec[s]);

}
     //--------------------------------------------------------------------------//
     //                   The probability of the state 's'                       //
     //       's' is  the position of the probability                            //
     //       (it's not the same for sparse vectors)                             //
     //--------------------------------------------------------------------------//
inline rp VCT::get_pos(const int s) const
{


#ifdef _PEPS_DEBUG_
  if (s >= real_dim)
    Programming_Error("index out of range", 2301)
#endif


if (is_sparse) {
  for (int i=0; i<dimension; i++)
    if (positions[i] == s)
      return vec[i];

  return 0; // unreachable state
}
else
  return(vec[s]);

}
     //--------------------------------------------------------------------------//
     //             It puts the probability 'v' to the state 's'                 //
     //--------------------------------------------------------------------------//
inline void VCT::put(const int s, const rp v)
{


#ifdef _PEPS_DEBUG_
  if (s >= dimension)
    Programming_Error("index out of range", 2302)
#endif

/*
if (is_sparse) {
  cout << "put en creux ... foireux ! \n";
  int i=0;
}
else
*/

  vec[s] = v;

}

     //--------------------------------------------------------------------------//
     //                    It puts the sizes of the automata                     //
     //--------------------------------------------------------------------------//
inline void VCT::put_sizes(const aut_st & s)
{
#ifdef _PEPS_DEBUG_
  if (s.size() != CND::cnd.aut())
    Programming_Error("wrong number of automata", 2303)
  if (s.state_space() != CND::cnd.ss())
    Programming_Error("wrong automata sizes", 2304)
#endif
 sizes.copy(s);
}
     //--------------------------------------------------------------------------//
     //               It fills 's' with the sizes of the automata                //
     //--------------------------------------------------------------------------//
inline void VCT::get_sizes(aut_st & s) const
{
 s.copy(sizes);
}
     //--------------------------------------------------------------------------//
     //                  It puts the permutation order of 'this'                 //
     //--------------------------------------------------------------------------//
inline void VCT::put_permut(const aut_list & l)
{
#ifdef _PEPS_DEBUG_
  if (!(l.is_full()))
    Programming_Error("incomplete list", 2305)
#endif
 p_order.copy(l);
}
     //--------------------------------------------------------------------------//
     //             It fills 'l' with the permutation order of 'this'            //
     //--------------------------------------------------------------------------//
inline void VCT::get_permut(aut_list & l) const
{
 l.copy(p_order);
}

     //--------------------------------------------------------------------------//
     //        It puts the sizes of the automata and the permutation order       //
     //          of 'this' according to the static structure 'CND::cnd'          //
     //--------------------------------------------------------------------------//
inline void VCT::get_CND()
{
 aut_st s;
 jumps  j;

 CND::cnd.get_s(s);             // gets the sizes of the static structure 'CND::cnd'
 sizes.copy(s);
 p_order.create(sizes.size());  // creates 'permutation' as the original order
 p_order.put();                 //                                 (0, 1, ... N-1)
 CND::cnd.get_oj(j);            // gets the original jumps of the structure 'CND::cnd'
 p_jumps.copy(j);
}

     //--------------------------------------------------------------------------//
     //           It normalizes the vector according to the first norm           //
     //                   (the sum of the elements becomes 1.0)                  //
     //--------------------------------------------------------------------------//
inline void VCT::normalize()
{
 rp  factor;        // a factor to normalize the vector elements
 rp  acc = zero;    // an accumulator to normalize the vector elements
 int k;             // a counter for the vector elements
 
 for (k=0; k<dimension; k++)
   acc += vec[k];
 
 factor = one / acc;

 for (k=0; k<dimension; k++)
   vec[k] *= factor;

}
     //--------------------------------------------------------------------------//
     //            It multiplies all vector elements by the factor 'f'           //
     //--------------------------------------------------------------------------//
inline void VCT::scale(const rp f)
{

 for (int k=0; k<dimension; k++)
   vec[k] *= f;
}
     //--------------------------------------------------------------------------//
     //                    It performs 'this' = 'v' * 'f'                        //
     //--------------------------------------------------------------------------//
inline void VCT::scale_copy(const VCT & v, const rp f)
{
#ifdef _PEPS_DEBUG_
  if (dimension != v.dimension)
    Programming_Error("different vector sizes", 2306)
#endif
  if (is_sparse != v.is_sparse)
    cout << "VCT::scale_copy -> the vector have not the same implementation\n";

if (v.is_sparse)
 for (int k=0; k<dimension; k++) {
   vec[k] = v.vec[k] * f;
   positions[k] = v.positions[k];
 } 
else
 for (int k=0; k<dimension; k++)
   vec[k] = v.vec[k] * f;
}

     //--------------------------------------------------------------------------//
     //                    It performs 'this' += 'v' * 'f'                       //
     //--------------------------------------------------------------------------//
inline void VCT::scale_add(const VCT & v, const rp f)
{
#ifdef _PEPS_DEBUG_
  if (dimension != v.dimension)
    Programming_Error("different vector sizes", 2307)
#endif

//if (is_sparse)
//  cout << "scale_add : s'assurer que le RSS est le meme \n";

 for (int k=0; k<dimension; k++)
   vec[k] += v.vec[k] * f;

}

     //--------------------------------------------------------------------------//
     //                       It performs 'this' += 'v'                          //
     //--------------------------------------------------------------------------//
inline void VCT::add(const VCT & v)
{
#ifdef _PEPS_DEBUG_
  if (dimension != v.dimension)
    Programming_Error("different vector sizes", 2308)
#endif

//if (is_sparse)
//  cout << "add : s'assurer que le RSS est le meme \n";

 for (int k=0; k<dimension; k++)
   vec[k] += v.vec[k];
}

     //--------------------------------------------------------------------------//
     //       First Norm - It returns the sum of the elements (in modulo)        //
     //--------------------------------------------------------------------------//
inline rp VCT::norm1() const
{
 rp answer = zero;

 for (int k=0; k<dimension; k++)
   answer += absv(vec[k]);

 return(answer);
}
     //--------------------------------------------------------------------------//
     //  Second Norm - It returns the square root of sum of the elements square  //
     //--------------------------------------------------------------------------//
inline rp VCT::norm2() const
{
 rp answer = zero;

 for (int k=0; k<dimension; k++)
   answer += vec[k] * vec[k];

 return(sqrt(answer));
}
     //--------------------------------------------------------------------------//
     //         Infinity Norm - It returns the biggest element (in modulo)       //
     //--------------------------------------------------------------------------//
inline rp VCT::norm_inf() const
{
 rp answer = zero;

 for (int k=0; k<dimension; k++)
   if (answer < absv(vec[k]))
     answer = absv(vec[k]);

 return(answer);
}

     //--------------------------------------------------------------------------//
     //                It returns the dot product of 'this' and 'v'              //
     //--------------------------------------------------------------------------//
inline rp VCT::dot(const VCT & v) const
{
#ifdef _PEPS_DEBUG_
  if (dimension != v.dimension)
    Programming_Error("different vector sizes", 2309)
#endif
 rp answer = zero;

 for (int k=0; k<dimension; k++)
   answer += vec[k] * v.vec[k];

 return(answer);
}
     //--------------------------------------------------------------------------//
     //              It returns the sum of the elements (absolute)               //
     //--------------------------------------------------------------------------//
inline rp VCT::sum() const
{
 rp answer = zero;

 for (int k=0; k<dimension; k++)
   answer += vec[k];

 return(answer);
}
     //--------------------------------------------------------------------------//
     //              It returns the number of elements under a threshold         //
     //--------------------------------------------------------------------------//
inline int VCT::elem_under(const rp th) const
{
 int answer = 0;

 for (int k=0; k<dimension; k++)
   if (vec[k] < th)
     answer += 1;

 answer += real_dim - dimension;

 return(answer);
}

     //--------------------------------------------------------------------------//
     //            It verifies if the sum of the elements is 1.0                 //
     //--------------------------------------------------------------------------//
inline bool VCT::verify() const
{
 rp acc = zero;

 for (int k=0; k<dimension; k++)
   acc += vec[k];

 return(((acc < (one+close_to_zero)) && (acc > (one-close_to_zero))));
}
     //--------------------------------------------------------------------------//
     //         It returns the error between two vectors ('this' and 'v')        //
     //--------------------------------------------------------------------------//
inline rp VCT::compare(const VCT & v, const e_typ err) const
{
#ifdef _PEPS_DEBUG_
  if (v.size() != dimension)
    Programming_Error("different sizes in compare", 2310)
#endif
 rp answer = zero;
 rp diff;
 rp max    = zero;

 if (is_sparse != v.is_sparse)
   cout << "VCT::compare -> different implementation type for the vectors\n";

 for (int k=0; k<dimension; k++) {                // for all states
   diff = absv(vec[k] - v.vec[k]);                   // computes the absolute difference
   if (err == rel_ind_err)                           // if the precision is relative
     if (vec[k] > max)                                  // looks for the largest
       max = vec[k];                                    //   element
   if ((err == abs_ind_err) || (err == rel_ind_err)) // consider the error
     if (diff > answer)                                 // individually
       answer = diff;                                   // (keeping the biggest error)
   else                                                 //   or
     answer += diff;                                    // accumulated
 }

 if (err == rel_ind_err)                          // if the precision is relative
   answer /= max;                                    // consider the error relatively
 return(answer);
	
}

     //--------------------------------------------------------------------------//
     //  It returns 'true' if the error between two vectors is smaller than 'e'  //
     //--------------------------------------------------------------------------//
     // Obs.: This function assumes that the absolute error is always less       //
     // restritive than the relative error. This assumption is true if 'this'    //
     // is a probability vector.                                                 //
     //--------------------------------------------------------------------------//
inline bool VCT::test(const VCT & v, const e_typ err, const rp e) const
{
#ifdef _PEPS_DEBUG_
  if (v.size() != dimension)
    Programming_Error("different sizes in compare", 2311)
#endif
 bool answer = true;
 rp   error  = zero;
 rp   diff;
 rp   max    = zero;


 if (is_sparse != v.is_sparse)
   cout << "VCT::test -> different implementation type for the vectors\n";

 for (int k=0; k<dimension; k++) {                // for all states
   diff = absv(vec[k] - v.vec[k]);                   // computes the absolute difference
   if (err == rel_ind_err)                           // if the precision is relative
     if (vec[k] > max)                                  // looks for the largest
       max = vec[k];                                    //   element
   if ((err == abs_ind_err) || (err == rel_ind_err)) // consider the error 
     error = diff;                                      // individually
   else                                                 //   or
     error += diff;                                     // accumulated
   if (error > e) {                                  // if the absolute error already
     answer = false;                                 //   is bigger than 'e'
     break;                                              // stop the test (ret. false)
   }
 }

 if ((err == rel_ind_err) && (answer))   // if the precision is relative and the
                                         //   absolute error never was bigger than 'e'
   answer = error/max < e;                  // consider the error relatively

 return(answer);
}

     //--------------------------------------------------------------------------//
     //  It returns the maximum of absolute difference between 'this' and 'v'    //
     //--------------------------------------------------------------------------//
     // Obs.: This function assumes that the absolute error is always less       //
     // restritive than the relative error. This assumption is true if 'this'    //
     // is a probability vector.                                                 //
     //--------------------------------------------------------------------------//
inline rp VCT::test_hbf(const VCT & v, const e_typ err) const
{
#ifdef _PEPS_DEBUG_
  if (v.size() != dimension)
    Programming_Error("different sizes in compare", 2311)
#endif
 rp   error  = zero;
 rp   diff;
 rp   max    = zero;


 if (is_sparse != v.is_sparse)
   cout << "VCT::test_hbf -> different implementation type for the vectors\n";

 for (int k=0; k<dimension; k++) {                // for all states
   diff = absv(vec[k] - v.vec[k]);                   // computes the absolute difference
   if (err == rel_ind_err)                           // if the precision is relative
     if (vec[k] > max)                                  // looks for the largest
       max = vec[k];                                    //   element
   if ((err == abs_ind_err) || (err == rel_ind_err)) // consider the error 
     error = diff;                                      // individually
   else                                                 //   or
     error += diff;                                     // accumulated
 }

 if (err == rel_ind_err)                 // if the precision is relative,
   error = error/max;                    // consider the error relatively

 return(error);
}
     //--------------------------------------------------------------------------//
     // It returns the sum of all elements, the largest element and its position //
     //--------------------------------------------------------------------------//
inline void VCT::info(rp & acc, rp & big, int & bpos, rp & sml, int & spos) const
{
 rp elem;

 big = zero;
 sml = one;
 acc = 0;

 for (int i=0; i<dimension; i++) {
   acc += vec[i];
   elem = absv(vec[i]);
   if (big < elem) {
     if (is_sparse) bpos = positions[i];
     else           bpos = i;
     big = elem;
   }
   if ((elem > round_zero) && (sml > elem)) {
     if (is_sparse) spos = positions[i];
     else           spos = i;
     sml = elem;
   }
 }
}


inline void VCT::debug() {

 ofstream  fout;
 Open_File("vct", dbg_file, fout);

  for (int i=0; i<dimension; i++) {
//    if (is_sparse)
//      fout << "Position " << positions[i] << " : ";
    fout << "Probabilite " << vec[i] << "\n";
  }

//  cout << "\n";

 Close_File("vct", dbg_file, fout);

}


inline void VCT::change_positions(const VCT & v) {

  cout << "begin change_positions\n";

  int dim = v.size();
  //rp  new_vec[dim];
  //int new_pos[dim];
  rp  *new_vec = (rp*)malloc(sizeof(rp)*dim);
  int  *new_pos = (int*)malloc(sizeof(int)*dim);

  cout << "toto1\n";

  int j=0;
  for (int i=0;i<dim;i++) {
    new_pos[i]=v.positions[i];
    if (positions[j]!=-1) {
	if (positions[j]==v.positions[i]) {
	  new_vec[i]=vec[j];
	  j++;
    	}
    	else
	  new_vec[i]=0;
    }
    else { // the end of the vector is empty
      while (i<dim) {
	new_pos[i]=v.positions[i];
	new_vec[i]=0;
	i++;
      }
    }   
  }

  cout << "toto2\n";

  delete[] vec;
  delete[] positions;

  cout << "toto3\n";

  vec = new_vec;
  positions = new_pos;

  cout << "end change_positions\n";

}
