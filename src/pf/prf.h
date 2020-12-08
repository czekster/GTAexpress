//====================================================================================//
//                                                                                    //
//                             Preferences Structure                                  //
//                                                                                    //
//====================================================================================//
//  This File:   prf.h                      Language: C++  (xlC and CC)               //
//  Software:    PEPS 2002                   Advisor: Brigitte.Plateau@imag.fr        //
//  Error Codes: 6100                                                                 //
//====================================================================================//
//  Creation:     5/feb/97                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 27/feb/03                        by: Paulo.Fernandes@imag.fr         //
//====================================================================================//
//    This class is used to store the static variable 'PRF::prf' which contains most  //
// of the user choices in PEPS.                                                       //
//====================================================================================//

class PRF
{
public:
 inline PRF();                                                            // Constructor
 inline ~PRF();                                                            // Destructor
 inline void defaults();                                  // It puts the defaults values

 inline void baptise(const peps_name file);                    // It assigns a file name
 inline void name(peps_name file) const;                     // It returns the file name

 inline bool verb() const;                        // It returns the current verbose mode
 inline void verb(const bool m);                      // It sets the verbose mode to 'm'
 inline void sverb(peps_name n) const;                                   // string answer

 inline int  max_it() const;          // It returns the current maximum iteration number
 inline void max_it(const int i);         // It sets the maximum iteration number to 'i'

 inline i_typ it_type() const;                  // It returns the current iteration type
 inline void  it_type(const i_typ t);               // It sets the iteration type to 't'
 inline void  sit_type(peps_name n) const;                               // string answer

 inline rp   min_err() const;                    // It returns the current minimum error
 inline void min_err(const rp e);                    // It sets the minimum error to 'e'

 inline e_typ err_type() const;        // It returns the current error verification type
 inline void  err_type(const e_typ v);     // It sets the error verification type to 'v'
 inline void  serr_type(peps_name n) const;                              // string answer

 inline rp   lu_thrs() const;                     // It returns the current LU threshold
 inline void lu_thrs(const rp t);                     // It sets the LU threshold to 't'

 inline f_aprox lu_apx() const;     // It returns the current function aproximation type
 inline void    lu_apx(const f_aprox a);      // It sets the function aprox. type to 'a'
 inline void    slu_apx(peps_name n) const;                              // string answer

 inline lu_typ lu_type() const;          // It returns the current LU decomposition type
 inline void   lu_type(const lu_typ t);      // It sets the LU decomposition type to 't'
 inline void   slu_type(peps_name n) const;                              // string answer

 inline reg_typ reg_type() const;       // It returns the current matrix regulation type
 inline void    reg_type(const reg_typ t);  // It sets the matrix regulation type to 't'
 inline void    sreg_type(peps_name n) const;                            // string answer

 inline sol_typ sol_type() const;             // It returns the current LU solution type
 inline void    sol_type(const sol_typ t);        // It sets the LU solution type to 't'
 inline void    ssol_type(peps_name n) const;                            // string answer

 inline v_typ i_vec() const;               // It returns the current initial vector type
 inline void  i_vec(const v_typ v);            // It sets the initial vector type to 'v'
 inline void  si_vec(peps_name n) const;                                 // string answer

 inline m_typ method() const;       // It returns the current method to perform products
 inline void  method(const m_typ m);    // It sets the method to perform products to 'm'
 inline void  smethod(peps_name n) const;                                // string answer

 inline strat_typ strategy() const;// It returns the current strategy for 'cut' the terms
 inline void  strategy(const strat_typ m);    // It sets the strategy to 'cut' the terms
 inline void  sstrategy(peps_name n) const;                              // string answer
 
 inline v_impl vec_impl() const;         // It returns the type of vector implementation
 inline void   vec_impl(const v_impl v);       // It sets the type of vector impl to 'v'
 inline void   svec_impl(peps_name n) const;                             // string answer

 inline zin_impl z_impl() const;            // It returns the type of zin implementation
 inline void   z_impl(const zin_impl v);          // It sets the type of zin impl to 'v'
 inline void   sz_impl(peps_name n) const;                               // string answer

 inline int  k_size() const;         // It returns the current GMRES's Krylov space size
 inline void k_size(const int m);        // It sets the GMRES's Krylov space size to 'm'

 inline int  poly_k() const; // It returns the current polynomial size for aproximations
 inline void poly_k(const int m);// It sets the polynomial size for aproximations to 'm'

 inline bool is_partial_rss() const;  // It returns true if the san model specifies only
                                                        // partial reachability function
 inline void partial_rss();              // It sets partial reachability function (true)
 inline void full_rss();     // It sets non-partial (full) reachability function (false)

        void write() const;                              // Output on 'fout' file stream
        void read(const peps_name file);                 // Input from 'fin' file stream
        int  mem_use() const;                  // Memory space used by 'this' (in bytes)

        //--------------------------------------------------------------------------//
        //                             Static variable                              //
        //--------------------------------------------------------------------------//
        static PRF prf;

        //--------------------------------------------------------------------------//
        //                          Data Structures                                 //
        //--------------------------------------------------------------------------//
private: peps_name   file_name;    // the file name of this structure
         bool        verbose;      // Verbose mode
         int         max_iter;     // Maximum iteration number
         i_typ       iter_type;    // Iteration type
         rp          min_error;    // Minimum error
         e_typ       error_type;   // Error verification type
         rp          lu_thresh;    // Threshold for LU decomposition
         f_aprox     lu_aprox;     // LU funct. aproximation type
         lu_typ      lu_decomp;    // LU decomposition type
         reg_typ     lu_reg;       // LU matrix regularization type
         sol_typ     lu_sol;       // LU solution type
         v_typ       vec_type;     // Initial vector type
         v_impl      vector_impl;  // Vector Implementation
         zin_impl    the_zin_impl; // zin Implementation
         m_typ       meth_type;    // Vector-Descriptor Multiplication method
         strat_typ   strat_type;   // Strategy type to 'cut' the tensor term
         int         krylov;       // Krylov space size (GMRES)
         int         poly_size;    // Polynomial size for aproximations (precond.)
         bool        partial_RSS;  // Partial Reachability Function (informed by .san)
};
