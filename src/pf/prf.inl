//====================================================================================//
//                                                                                    //
//                             Preferences Structure                                  //
//                                                                                    //
//====================================================================================//
//  This File:   prf_inl.h                  Language: C++  (xlC and CC)               //
//  Software:    PEPS 2002                   Advisor: Brigitte.Plateau@imag.fr        //
//  Error Codes: 6100                                                                 //
//====================================================================================//
//  Creation:     5/feb/97                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 13/feb/03                        by: Anne.Benoit@imag.fr             //
//====================================================================================//

     //--------------------------------------------------------------------------//
     //                              Constructor                                 //
     //--------------------------------------------------------------------------//
inline PRF::PRF()
{
 strcpy(file_name, "defaults");
 defaults();
}
     //--------------------------------------------------------------------------//
     //                              Destructor                                  //
     //--------------------------------------------------------------------------//
inline PRF::~PRF()
{
}
     //--------------------------------------------------------------------------//
     //                       It puts the defaults values                        //
     //--------------------------------------------------------------------------//
inline void PRF::defaults()
{
 verbose      = def_verbose;
 max_iter     = def_iter;
 iter_type    = def_i_typ;
 min_error    = def_err;
 error_type   = def_e_typ;
 lu_thresh    = def_thresh;
 lu_aprox     = def_f_aprox;
 lu_decomp    = def_lu_typ;
 lu_sol       = def_sol_typ;
 vec_type     = def_v_typ;
 the_zin_impl = def_zin_impl;
 vector_impl  = def_v_impl;
 meth_type    = def_m_typ;
 strat_type   = def_strat;
 krylov       = def_krylov;
 poly_size    = def_poly;
 partial_RSS  = def_partial;
}

     //--------------------------------------------------------------------------//
     //                          It assigns a file name                          //
     //--------------------------------------------------------------------------//
inline void PRF::baptise(const peps_name file)
{
 strcpy(file_name, file);
}
     //--------------------------------------------------------------------------//
     //                         It returns the file name                         //
     //--------------------------------------------------------------------------//
inline void PRF::name(peps_name file) const
{
 strcpy(file, file_name);
}

     //--------------------------------------------------------------------------//
     //                    It returns the current verbose mode                   //
     //--------------------------------------------------------------------------//
inline bool PRF::verb() const
{
 return(verbose);
}
     //--------------------------------------------------------------------------//
     //                      It sets the verbose mode to 'm'                     //
     //--------------------------------------------------------------------------//
inline void PRF::verb(const bool m)
{
 verbose = m;
}

     //--------------------------------------------------------------------------//
     //             It returns the current maximum iteration number              //
     //--------------------------------------------------------------------------//
inline int PRF::max_it() const
{
 return(max_iter);
}
     //--------------------------------------------------------------------------//
     //               It sets the maximum iteration number to 'i'                //
     //--------------------------------------------------------------------------//
inline void PRF::max_it(const int i)
{
 max_iter = i;
}

     //--------------------------------------------------------------------------//
     //                 It returns the current iteration type                    //
     //--------------------------------------------------------------------------//
inline i_typ PRF::it_type() const
{
 return(iter_type);
}
     //--------------------------------------------------------------------------//
     //                    It sets the iteration type to 't'                     //
     //--------------------------------------------------------------------------//
inline void PRF::it_type(const i_typ t)
{
 iter_type = t;
}

     //--------------------------------------------------------------------------//
     //                  It returns the current minimum error                    //
     //--------------------------------------------------------------------------//
inline rp PRF::min_err() const
{
 return(min_error);
}
     //--------------------------------------------------------------------------//
     //                    It sets the minimum error to 'e'                      //
     //--------------------------------------------------------------------------//
inline void PRF::min_err(const rp e)
{
 min_error = e;
}

     //--------------------------------------------------------------------------//
     //              It returns the current error verification type              //
     //--------------------------------------------------------------------------//
inline e_typ PRF::err_type() const
{
 return(error_type);
}
     //--------------------------------------------------------------------------//
     //                It sets the error verification type to 'v'                //
     //--------------------------------------------------------------------------//
inline void PRF::err_type(const e_typ v)
{
 error_type = v;
}

     //--------------------------------------------------------------------------//
     //                    It returns the current LU threshold                   //
     //--------------------------------------------------------------------------//
inline rp PRF::lu_thrs() const
{
 return(lu_thresh);
}
     //--------------------------------------------------------------------------//
     //                      It sets the LU threshold to 't'                     //
     //--------------------------------------------------------------------------//
inline void PRF::lu_thrs(const rp t)
{
 lu_thresh = t;
}

     //--------------------------------------------------------------------------//
     //             It returns the current function aproximation type            //
     //--------------------------------------------------------------------------//
inline f_aprox PRF::lu_apx() const
{
 return(lu_aprox);
}
     //--------------------------------------------------------------------------//
     //               It sets the function aproximation type to 'a'              //
     //--------------------------------------------------------------------------//
inline void PRF::lu_apx(const f_aprox a)
{
 lu_aprox = a;
}
     //--------------------------------------------------------------------------//
     //               It returns the current LU decomposition type               //
     //--------------------------------------------------------------------------//
inline lu_typ PRF::lu_type() const
{
 return(lu_decomp);
}
     //--------------------------------------------------------------------------//
     //                 It sets the LU decomposition type to 't'                 //
     //--------------------------------------------------------------------------//
inline void PRF::lu_type(const lu_typ t)
{
 lu_decomp = t;
}
     //--------------------------------------------------------------------------//
     //               It returns the current matrix regulation type              //
     //--------------------------------------------------------------------------//
inline reg_typ PRF::reg_type() const
{
 return(lu_reg);
}
     //--------------------------------------------------------------------------//
     //                 It sets the matrix regulation type to 't'                //
     //--------------------------------------------------------------------------//
inline void PRF::reg_type(const reg_typ t)
{
 lu_reg = t;
}
     //--------------------------------------------------------------------------//
     //                  It returns the current LU solution type                 //
     //--------------------------------------------------------------------------//
inline sol_typ PRF::sol_type() const
{
 return(lu_sol);
}
     //--------------------------------------------------------------------------//
     //                    It sets the LU solution type to 't'                   //
     //--------------------------------------------------------------------------//
inline void PRF::sol_type(const sol_typ t)
{
 lu_sol = t;
}

     //--------------------------------------------------------------------------//
     //                It returns the current initial vector type                //
     //--------------------------------------------------------------------------//
inline v_typ PRF::i_vec() const
{
 return(vec_type);
}
     //--------------------------------------------------------------------------//
     //                  It sets the initial vector type to 'v'                  //
     //--------------------------------------------------------------------------//
inline void PRF::i_vec(const v_typ v)
{
 vec_type = v;
}

     //--------------------------------------------------------------------------//
     //                It returns the current vector implementation type         //
     //--------------------------------------------------------------------------//
inline v_impl PRF::vec_impl() const
{
 return(vector_impl);
}
     //--------------------------------------------------------------------------//
     //                  It sets the type of vector implementation to 'v'        //
     //--------------------------------------------------------------------------//
inline void PRF::vec_impl(const v_impl v)
{
 vector_impl = v;
}
     //--------------------------------------------------------------------------//
     //                It returns the current zin implementation type            //
     //--------------------------------------------------------------------------//
inline zin_impl PRF::z_impl() const
{
 return(the_zin_impl);
}
     //--------------------------------------------------------------------------//
     //                  It sets the type of vector implementation to 'v'        //
     //--------------------------------------------------------------------------//
inline void PRF::z_impl(const zin_impl v)
{
 the_zin_impl = v;
}
     //--------------------------------------------------------------------------//
     //            It returns the current method to perform products             //
     //--------------------------------------------------------------------------//
inline m_typ PRF::method() const
{
 return(meth_type);
}
     //--------------------------------------------------------------------------//
     //               It sets the method to perform products to 'm'              //
     //--------------------------------------------------------------------------//
inline void PRF::method(const m_typ m)
{
 meth_type = m;
}
     //--------------------------------------------------------------------------//
     //            It returns the current method to perform products             //
     //--------------------------------------------------------------------------//
inline strat_typ PRF::strategy() const
{
 return(strat_type);
}
     //--------------------------------------------------------------------------//
     //               It sets the strategy to 'cut' the tensor terms             //
     //--------------------------------------------------------------------------//
inline void PRF::strategy(const strat_typ m)
{
 strat_type = m;
}
     //--------------------------------------------------------------------------//
     //            It returns the current GMRES's Krylov space size              //
     //--------------------------------------------------------------------------//
inline int PRF::k_size() const
{
 return(krylov);
}
     //--------------------------------------------------------------------------//
     //              It sets the GMRES's Krylov space size to 'm'                //
     //--------------------------------------------------------------------------//
inline void PRF::k_size(const int m)
{
 krylov = m;
}
     //--------------------------------------------------------------------------//
     //            It returns the current GMRES's Krylov space size              //
     //--------------------------------------------------------------------------//
inline int PRF::poly_k() const
{
 return(poly_size);
}
     //--------------------------------------------------------------------------//
     //              It sets the GMRES's Krylov space size to 'm'                //
     //--------------------------------------------------------------------------//
inline void PRF::poly_k(const int m)
{
 poly_size = m;
}
     //--------------------------------------------------------------------------//
     //             It returns true if the san model specifies only              //
     //--------------------------------------------------------------------------//
inline bool PRF::is_partial_rss() const
{
 return(partial_RSS);
}
     //--------------------------------------------------------------------------//
     //               It sets partial reachability function (true)               //
     //--------------------------------------------------------------------------//
inline void PRF::partial_rss()
{
 partial_RSS = true;
}
     //--------------------------------------------------------------------------//
     //              It sets partial reachability function (true)                //
     //--------------------------------------------------------------------------//
inline void PRF::full_rss()
{
 partial_RSS = false;
}

     //--------------------------------------------------------------------------//
     //                   It returns the current verbose mode                    //
     //--------------------------------------------------------------------------//
inline void PRF::sverb(peps_name n) const
{
 switch (verbose) {
   case true  : strcpy(n, "on");  break;
   case false : strcpy(n, "off"); break;
   //default    : Programming_Error("preference options must be updated", 6101) //redundant label (@CZ)
 }
}
     //--------------------------------------------------------------------------//
     //                  It returns the current iteration type                   //
     //--------------------------------------------------------------------------//
inline void PRF::sit_type(peps_name n) const
{
 switch (iter_type) {
   case fix_it : strcpy(n, "fixed iteration number"); break;
   case stb_it : strcpy(n, "stability test"); break;
   case cnv_it : strcpy(n, "convergence test"); break;
   default     : Programming_Error("preference options must be updated", 6102)
 }
}
     //--------------------------------------------------------------------------//
     //              It returns the current error verification type              //
     //--------------------------------------------------------------------------//
inline void PRF::serr_type(peps_name n) const
{
 switch (error_type) {
   case abs_ind_err : strcpy(n, "maximum of individual absolute errors");  break;
   case abs_acc_err : strcpy(n, "accumulated individual absolute error"); break;
   case rel_ind_err : strcpy(n, "maximum individual relative error");  break;
   default      : Programming_Error("preference options must be updated", 6103)
 }
}
     //--------------------------------------------------------------------------//
     //             It returns the current function aproximation type            //
     //--------------------------------------------------------------------------//
inline void PRF::slu_apx(peps_name n) const
{
 switch (lu_aprox) {
   case lu_user : strcpy(n, "dialogue with the user");  break;
   case lu_max  : strcpy(n, "maximum evaluation");      break;
   case lu_min  : strcpy(n, "minimum evaluation");      break;
   case lu_ave  : strcpy(n, "average evaluation");      break;
   case lu_zero : strcpy(n, "replace by zero");         break;
   default : Programming_Error("preference options must be updated", 6104)
 }
}
     //--------------------------------------------------------------------------//
     //               It returns the current LU decomposition type               //
     //--------------------------------------------------------------------------//
inline void PRF::slu_type(peps_name n) const
{
 switch (lu_decomp) {
   case std_lu : strcpy(n, "standard LU decomposition");  break;
   case sor_lu : strcpy(n, "SOR-like decomposition");     break;
   default : Programming_Error("preference options must be updated", 6105)
 }
}
     //--------------------------------------------------------------------------//
     //              It returns the current matrix regulation type               //
     //--------------------------------------------------------------------------//
inline void PRF::sreg_type(peps_name n) const
{
 switch (lu_reg) {
   case no_reg    : strcpy(n, "no regulation (keeping the matrix as it is)"); break;
   case shift_reg : strcpy(n, "shifting diagonal elements of an epsilon");    break;
   case wingl_reg : strcpy(n, "equal. diag. elements to the largest value");  break;
   case tychv_reg : strcpy(n, "multiply the matrix by its inverse");          break;
   case aprox_reg : strcpy(n, "replacing null diag. elements by an epsilon"); break;
   case one_reg   : strcpy(n, "replacing null diag. elements by one");        break;
   default : Programming_Error("preference options must be updated", 6105)
 }
}
     //--------------------------------------------------------------------------//
     //                  It returns the current LU solution type                 //
     //--------------------------------------------------------------------------//
inline void PRF::ssol_type(peps_name n) const
{
 switch (lu_sol) {
   case both_lu : strcpy(n, "solve both LU matrices"); break;
   case just_l  : strcpy(n, "solve only L matrices");  break;
   case just_u  : strcpy(n, "solve only U matrices");  break;
   default : Programming_Error("preference options must be updated", 6106)
 }
}
     //--------------------------------------------------------------------------//
     //               It returns the current initial vector type                 //
     //--------------------------------------------------------------------------//
inline void PRF::si_vec(peps_name n) const
{
 switch (vec_type) {
   case eq_vec : strcpy(n, "equiprobable"); break;
   case in_vec : strcpy(n, "any vector (read from a file");  break;
   case ap_vec : strcpy(n, "aproximated by the inverse of the diagonal");  break;
   default     : Programming_Error("preference options must be updated", 6107)
 }
}

     //--------------------------------------------------------------------------//
     //               It returns the current type of vector implementation       //
     //--------------------------------------------------------------------------//
inline void PRF::svec_impl(peps_name n) const
{
 switch (vector_impl) {
   case v_full : strcpy(n, "extended (E)"); break;
   case v_sparse : 
      if (the_zin_impl == zin_sparse) 
        strcpy(n, "fully reduced (FR)");
      else
	strcpy(n, "partially reduced (PR)");
      break;
   default     : Programming_Error("preference options must be updated", 6107)
 }
}
     //--------------------------------------------------------------------------//
     //               It returns the current type of zin implementation          //
     //--------------------------------------------------------------------------//
inline void PRF::sz_impl(peps_name n) const
{
 switch (the_zin_impl) {
   case zin_full   : strcpy(n, "full implementation"); break;
   case zin_sparse : strcpy(n, "sparse implementation");  break;
   default     : Programming_Error("preference options must be updated", 6107)
 }
}

     //--------------------------------------------------------------------------//
     //           It returns the current method to perform products              //
     //--------------------------------------------------------------------------//
inline void PRF::smethod(peps_name n) const
{
 switch (meth_type) {
   case meth_A : strcpy(n, "avoid permutation");             break;
   case meth_B : strcpy(n, "minimize permutations");         break;
   case meth_C : strcpy(n, "minimize function evaluations"); break;
   case meth_S : strcpy(n, "use the Split approach");        break;
   case meth_M : strcpy(n, "mixed method");                  break;
   default     : Programming_Error("preference options must be updated", 6108)
 }
}

     //--------------------------------------------------------------------------//
     //           It returns the strategy to 'cut' the tensor terms              //
     //--------------------------------------------------------------------------//
inline void PRF::sstrategy(peps_name n) const
{
 switch (strat_type) {
   case strat_A : strcpy(n, "identities to shuffle");             break;
   case strat_B : strcpy(n, "functional evaluations in shuffle");         break;
   case strat_C : strcpy(n, "functional evaluations in split"); break;
   case strat_D : strcpy(n, "functional evaluations in split, only identites to shuffle");        break;
   case strat_E : strcpy(n, "only functional definition and identities in shuffle");                  break;
   default     : Programming_Error("preference options must be updated", 6108)
 }
}

