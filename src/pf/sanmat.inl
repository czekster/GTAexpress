//====================================================================================//
//                                                                                    //
//                                Class SAN Matrix                                    //
//                                                                                    //
//====================================================================================//
//  This File:   sanmat.inl                 Language: C++  (xlC and CC)               //
//  Software:    PEPS 2002                   Advisor: Brigitte.Plateau@imag.fr        //
//  Error Codes: 3300                                                                 //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change:  7/nov/02                        by: paulof@inf.pucrs.br             //
//====================================================================================//

     //--------------------------------------------------------------------------//
     //                            Empty Constructor                             //
     //--------------------------------------------------------------------------//
inline san_mat::san_mat()
{
 type      = Identity;
 dimension = 0;
 nz_number = 0;
}
     //--------------------------------------------------------------------------//
     //                            Sized Constructor                             //
     //--------------------------------------------------------------------------//
inline san_mat::san_mat(const int n, const int d, const mat_typ t)
{
#ifdef _PEPS_DEBUG_
  if (d == 0)
    Programming_Error("creation of zero dimension matrix", 3301)
#endif
 type      = t;
 dimension = d;
 nz_number = n;
 alloc();
}
     //--------------------------------------------------------------------------//
     //                             Copy Constructor                             //
     //--------------------------------------------------------------------------//
inline san_mat::san_mat(const san_mat & m)
{
 type      = m.type;
 dimension = m.dimension;
 nz_number = m.nz_number;
 alloc();
 trans(m);
}
     //--------------------------------------------------------------------------//
     //                                Destructor                                //
     //--------------------------------------------------------------------------//
inline san_mat::~san_mat()
{
 if (dimension != 0) {
   desal();
   type      = Identity;
   dimension = 0;
   nz_number = 0;
 }
}
     //--------------------------------------------------------------------------//
     //                           Empty Intialization                            //
     //--------------------------------------------------------------------------//
inline void san_mat::create()
{
 type      = Identity;
 dimension = 0;
 nz_number = 0;
}
     //--------------------------------------------------------------------------//
     //                           Sized Intialization                            //
     //--------------------------------------------------------------------------//
inline void san_mat::create(const int n, const int d, const mat_typ t)
{
#ifdef _PEPS_DEBUG_
  if (d == 0)
    Programming_Error("creation of zero dimension matrix", 3302)
#endif
 erase();
 type      = t;
 dimension = d;
 nz_number = n;
 alloc();
}
     //--------------------------------------------------------------------------//
     //                       Copy and Sized Intialization                       //
     //--------------------------------------------------------------------------//
inline void san_mat::copy(const san_mat & m)
{
 create(m.nz_number, m.dimension, m.type);
 trans(m);
}
     //--------------------------------------------------------------------------//
     //                               Destruction                                //
     //--------------------------------------------------------------------------//
inline void san_mat::erase()
{
 if (dimension != 0) {
   desal();
   type      = Identity;
   dimension = 0;
   nz_number = 0;
 }
}
     //--------------------------------------------------------------------------//
     //                         The order of the matrix                          //
     //--------------------------------------------------------------------------//
inline int san_mat::size() const
{
 return(dimension);
}
     //--------------------------------------------------------------------------//
     //                     The number of non-zero elements                      //
     //--------------------------------------------------------------------------//
inline int san_mat::nz() const
{
 return(nz_number);
}
     //--------------------------------------------------------------------------//
     //                             The matrix type                              //
     //--------------------------------------------------------------------------//
inline mat_typ san_mat::tp() const
{
 return(type);
}
     //--------------------------------------------------------------------------//
     //                            The order is zero                             //
     //--------------------------------------------------------------------------//
inline bool san_mat::is_empty() const
{
 return(dimension == 0);
}
     //--------------------------------------------------------------------------//
     //   It evaluates all functional elements of 'this' according to state 's'  //
     //--------------------------------------------------------------------------//
     //  Traditional Implementation: it uses the FTB::ftb (functree) evaluation  //
     //--------------------------------------------------------------------------//
     // Obs.: the reference set of 's' is the current number of automata.        //
     //--------------------------------------------------------------------------//
     // Implementation notes:                                                    //
     //   - only sparse matrices can have functional elements;                   //
     //   - the input parameter 's' has the local states of all automata but     //
     // automaton 'a'. The local state of the automaton 'a' is taken from the    //
     // row index of the matrix ('this').                                        //
     //--------------------------------------------------------------------------//
inline void san_mat::eval(aut_st & s, const aut_id a, const st_map * m)
{
#ifdef _PEPS_DEBUG_
  if (s.size() != FTB::ftb.g_aut())
    Programming_Error("wrong sized parameters", 3307)
  if (a >= CND::cnd.aut())
    Programming_Error("invalid automaton id", 3308)
  if (m[a].size() != dimension)
    Programming_Error("wrong state map", 3309)
#endif

 if (type == Sparse)
   for (int i=0; i<nz_number; i++)   // to all nonzero elements of a sparse matrix
     if (elements[i].is_func()) {       // if its a functional one
       s.put(a, rows[i]);                  // it puts in the position 'a' of 's'
                                           // its current local state (row index)
       elements[i].f_eval(s, m);           // and evaluates the element
     }
}
     //--------------------------------------------------------------------------//
     //   It evaluates all functional elements of 'this' according to state 's'  //
     //--------------------------------------------------------------------------//
     //  Just-In-Time Implementation: it uses the 'jit' compiled functions       //
     //--------------------------------------------------------------------------//
     // Obs.: the reference set of 's' is the current number of automata.        //
     //--------------------------------------------------------------------------//
     // Implementation notes:                                                    //
     //   - only sparse matrices can have functional elements;                   //
     //   - the input parameter 's' has the local states of all automata but     //
     // automaton 'a'. The local state of the automaton 'a' is taken from the    //
     // row index of the matrix ('this').                                        //
     //--------------------------------------------------------------------------//
inline void san_mat::jit_eval(aut_st & s, const aut_id a, const st_map * m)
{
#ifdef _PEPS_DEBUG_
  if (s.size() != FTB::ftb.g_aut())
    Programming_Error("wrong sized parameters", 3308)
  if (a >= CND::cnd.aut())
    Programming_Error("invalid automaton id", 3309)
  if (m[a].size() != dimension)
    Programming_Error("wrong state map", 3310)
#endif

 if (type == Sparse)
   for (int i=0; i<nz_number; i++)   // to all nonzero elements of a sparse matrix
     if (elements[i].is_func()) {       // if its a functional one
       s.put(a, rows[i]);                  // it puts in the position 'a' of 's'
                                           // its current local state (row index)
       elements[i].jit_f_eval(s, m);       // and evaluates the element
     }
}
     //--------------------------------------------------------------------------//
     //              It performs the product 'v_out = v_in * this'               //
     //--------------------------------------------------------------------------//
inline void san_mat::left_mult(rp v_out[], const rp v_in[]) const
{
 int j, k;          // counters for columns and nonzero elements

 switch (type) {
   case Sparse   : for (j=0; j<dimension; j++) {
                     v_out[j] = zero;
                     for (k=columns[j]; k<columns[j+1]; k++)
                       v_out[j] += elements[k].get() * v_in[rows[k]];
                   } break;
   case Binary   : for (j=0; j<dimension; j++) {
                     v_out[j] = zero;
                     for (k=columns[j]; k<columns[j+1]; k++)
                       v_out[j] += v_in[rows[k]];
                   } break;
   case Identity : for (j=0; j<dimension; j++)
                     v_out[j] = v_in[j];
                   break;
   case Empty    : for (j=0; j<dimension; j++)
                     v_out[j] = zero;
 }

// cout << "fin de la mult plein : \n";
// for (j=0; j<dimension; j++) 
//   cout << "      v_out[" << j << "] = " << v_out[j] << "\n";
}

     //--------------------------------------------------------------------------//
     //              It performs the product 'v_out = v_in * this'               //
     //--------------------------------------------------------------------------//
inline void san_mat::sparse_left_mult2(vector<gs_pair> & v_out,
			               const vector<gs_pair> v_in,
                                       int s, int m) const
{
 int j, k;          // counters for columns and nonzero elements
 rp tmp;
 gs_pair pair_tmp;
 vector<gs_pair>::const_iterator i;  // iterator to traverse the v_in vector


 //cout << "begin sparse_left_mult2 : capacity " << v_out.capacity() << "\n";

 switch (type) {
   case Sparse   : for (j=0; j<dimension; j++) {
                     i = v_in.begin(); 
                     tmp = zero;
		     k=columns[j];
		     while (i!=v_in.end() && k<columns[j+1]) {
			if ((*i).first == rows[k]) {
                          tmp += elements[k].get() * (*i).second;
			  ++i; ++k;
          		}
			else if ((*i).first > rows[k])
			  k++;
			else
                          ++i;
		     }
	
		     if (tmp!=0) { // We add it into v_out
//cout << "elt non nul : " << tmp <<  " en " << s + j*m << "\n";
			pair_tmp.first = s + j*m;
			pair_tmp.second = tmp;
			if (v_out.size() == v_out.capacity())
			  v_out.reserve(v_out.size()+1);
			v_out.push_back(pair_tmp);
                     }
                   } break;

   case Binary   : for (j=0; j<dimension; j++) {
                     tmp = zero;
		     i = v_in.begin();
		     k=columns[j];
		     while (i!=v_in.end() && k<columns[j+1]) {
			if ((*i).first == rows[k]) {
                          tmp += (*i).second;
			  ++i; ++k;
          		}
			else if ((*i).first > rows[k])
			  k++;
			else    
                          ++i;
		     }
	
		     if (tmp!=0) { // We add it into v_out
//cout << "elt non nul\n";
			pair_tmp.first = s + j*m;
			pair_tmp.second = tmp;
			if (v_out.size() == v_out.capacity())
			  v_out.reserve(v_out.size()+1);
         		v_out.push_back(pair_tmp);
                     }
                   } break;

   case Identity : for (j=0; j<dimension; j++) {
    		     i = v_in.begin();
		     while (i!=v_in.end()) {
		       if ((*i).first < j)
			 ++i;
                       else if ((*i).first == j) {
                          tmp = (*i).second;
			  break;
		          }
		       else { // (*i).first > j
			  tmp = 0;
 			  break;
		       }
                     }
		     if (tmp!=0) { // We add it into v_out
//cout << "elt non nul\n";
			pair_tmp.first = s + j*m;
			pair_tmp.second = tmp;
			if (v_out.size() == v_out.capacity())
			  v_out.reserve(v_out.size()+1);
		        v_out.push_back(pair_tmp);
                     }
                   }
                   break;
    case Empty    : break;;
 }

//cout << "end sparse_left_mult2\n";
}

     //--------------------------------------------------------------------------//
     //              It performs the product 'v_out = v_in * this'               //
     //--------------------------------------------------------------------------//
inline void san_mat::sparse_left_mult2(vector<gs_pair> & v_out,
			               const rp *v_in,
                                       int base) const
{
 int j, k;          // counters for columns and nonzero elements
 rp tmp;
 gs_pair pair_tmp;

 switch (type) {
   case Sparse   : for (j=0; j<dimension; j++) {
	             tmp = zero;
                     for (k=columns[j]; k<columns[j+1]; k++) 
                        tmp += elements[k].get() * v_in[rows[k]];
	
		     if (tmp!=0) { // We add it into v_out
//cout << "j = " << j << " : elt non nul : " << tmp <<  " en " << base + j << "\n";
			pair_tmp.first = base + j;
			pair_tmp.second = tmp;
			if (v_out.size() == v_out.capacity())
			  v_out.reserve(v_out.size()+1);
			v_out.push_back(pair_tmp);
                     }
                   } break;

   case Binary   : for (j=0; j<dimension; j++) {
	             tmp = zero;
                     for (k=columns[j]; k<columns[j+1]; k++) 
                        tmp += v_in[rows[k]];
	
		     if (tmp!=0) { // We add it into v_out
			pair_tmp.first = base + j;
			pair_tmp.second = tmp;
			if (v_out.size() == v_out.capacity())
			  v_out.reserve(v_out.size()+1);
			v_out.push_back(pair_tmp);
                     }
                   } break;

   case Identity : for (j=0; j<dimension; j++) {
		     tmp = v_in[j];
		     if (tmp!=0) { // We add it into v_out
			pair_tmp.first = base + j;
			pair_tmp.second = tmp;
			if (v_out.size() == v_out.capacity())
			  v_out.reserve(v_out.size()+1);
			v_out.push_back(pair_tmp);
                     }
                   }
                   break;
    case Empty    : break;;
 }

}

     //--------------------------------------------------------------------------//
     //              It performs the product 'v_out = v_in[] * this[j][]'        //
     //--------------------------------------------------------------------------//
inline rp san_mat::sparse_elem_left_mult(const rp v_in[], int j) const
{
 int k;          // counter for nonzero elements of the matrix	
 rp v_out;

//cout << "debut de la mult : j = " << j << "\n";

 switch (type) {
   case Sparse   : v_out = zero;
                   for (k=columns[j]; k<columns[j+1]; k++)
                     v_out += elements[k].get() * v_in[rows[k]];
                   break;
   case Binary   : v_out = zero;
                   for (k=columns[j]; k<columns[j+1]; k++)
                     v_out += v_in[rows[k]];
                   break;
   case Identity : v_out = v_in[j];
                   break;
   case Empty    : v_out = zero;
 }

//cout << "fin de la mult normale, v_out = " << v_out << "\n";

 return v_out;

}

inline rp san_mat::sparse_elem_left_mult2(const vector<gs_pair> v_in, int j) const
{
 int k;          // counter for nonzero elements of the matrix	
 rp v_out;
 vector<gs_pair>::const_iterator i;  // iterator to traverse the v_in vector

 switch (type) {
   case Sparse   : v_out = zero;
		   i = v_in.begin();
		   k=columns[j];
		   while (i!=v_in.end() && k<columns[j+1]) {
			if ((*i).first == rows[k]) {
                          v_out += elements[k].get() * (*i).second;
			  ++i; ++k;
          		}
			else if ((*i).first > rows[k])
			  k++;
			else
                          ++i;
		   }
                    
                   break;
   case Binary   : v_out = zero;
		   i = v_in.begin();
		   k=columns[j];
		   while (i!=v_in.end() && k<columns[j+1]) {
			if ((*i).first == rows[k]) {
                          v_out += (*i).second;
			  ++i; ++k;
          		}
			else if ((*i).first > rows[k])
			  k++;
			else    
                          ++i;
		   }
                   break;
   case Identity : i = v_in.begin();
		   while (i!=v_in.end()) {
		     if ((*i).first < j)
			++i;
                     else if ((*i).first == j) {
                        v_out = (*i).second;
			break;
		     }
		     else break; // (*i).first > j
                   }
                   break;
   case Empty    : v_out = zero;
 }

 //cout << "fin de la mult en sparse, v_out = " << v_out << "\n";

 return v_out;

}
