//====================================================================================//
//                                                                                    //
//                              PEPS Solution Methods                                 //
//                                                                                    //
//====================================================================================//
//  This File:   solve.C                    Language: C++  (xlC and CC)               //
//  Software:    PEPS 2002                   Advisor: Brigitte.Plateau@imag.fr        //
//  Doc:         solve.tex                                                            //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 17/nov/04                        by: Leonardo.Brenner@imag.fr        //
//====================================================================================//
#include "peps.h"

//====================================================================================//
//  Power Method                                                                      //
//                                                                                    //
//    This function performs the power method to the infinitesimal generator stored   //
// into the global structure 'CND::cnd' (continuous-time normalized descriptor).      //
//                                                                                    //
//    The explicit input parameters are:                                              //
//    - 'buf'         the initial vector;                                             //
//    - 'p_type'      the type of precondition to be used                             //
//                                                                                    //
//    The implicit input parameters are:                                              //
//    - 'CND::cnd'               the infinitesimal generator                          //
//    - 'PRF::prf.it_type()'     fixed iterations or convergence test                 //
//    - 'PRF::prf.err_type()'    error test type (indiv./accum. - abs./relat.)        //
//    - 'PRF::prf.min_err()'     tolerance for convergence                            //
//    - 'PRF::prf.max_it()'      max. number of iterations                            //
//    - 'PRF::prf.verb()'        verbose mode                                         //
//    - 'PRF::prf.k_size()'      interval between convergence check (if verbose mode) //
//                                                                                    //
//    The implicit output parameter is:                                               //
//    - 'VCT::vct'               the resulting vector (solution)                      //
//====================================================================================//
bool Power_CND(VCT & buf, const precond_typ p_type)
{
 int   i, k;            // iteration counters
 Timer T;               // timer
 bool  abort = false;   // a flag to abort the iteration after the initial test
 bool  conv;            // the precision for convergence was reached
 rp    resid;           // the residue of the vector
 rp    acc;             // the sum of all elements of the vector
 rp    largest;         // the largest element of the vector
 int   lwhere;          // the position of the largest element of the vector
 rp    smallest;        // the smallest element of the vector
 int   swhere;          // the position of the smallest element of the vector

 bool  test_error       = (PRF::prf.it_type() != fix_it); // verify convergence
 e_typ error_test_type  = PRF::prf.err_type();            // error verification type
 int   limit            = PRF::prf.max_it();              // max. number of iterations
 rp    tol              = PRF::prf.min_err();             // tolerance for convergence
 bool  verbose          = PRF::prf.verb();                // verbose mode
 int   big_step         = (PRF::prf.k_size() / 2);        // half interval between info

 cout.setf(ios::scientific);
 cout.precision(peps_precision);
 
 CND::cnd.reset_aunf();
 if (PRF::prf.method() == meth_S) {  //only find cuts for the split method
    T.clear(); T.start();
    VCT::vct.setup_aunfs(); //discover the best cuts for the terms
    T.stop();
    CND::cnd.set_aunf_time(T.usertime());
 }

 VCT::vct.alloc_static();

 cout << "Iteration 0: "; cout.flush();
 T.clear(); T.start();
 VCT::vct.mult_CND(buf);

 T.stop();
 resid = VCT::vct.norm_inf();
 i = 1;
 conv = (resid < tol);

 VCT::vct.add(buf);

 acc = VCT::vct.sum();
 if ( (acc < one - round_zero) || (acc > one + round_zero) )            // initial test
   if  ( (acc < - PRF::prf.min_err()) || (acc > PRF::prf.min_err()) ) {
     cout << "\nWARNING: sum of residual vector = " << acc << " (it should be zero)"
          << "\nIteration 0: ";
   }
   else {
     cout << "\n*******************************************************************"
          << "\n***       the descriptor is not a infinitesimal generator       ***"
          << "\n*******************************************************************"
          << "\n                   Aborting the iterations"
          << "\n Too large sum of residual vector = " << acc  << " (it should be zero)\n";
     abort = true;
   }
 if (!abort) {
   if (p_type == no_precond)
     CND::cnd.add_identity();
   T.start();
   //-----------------------------------------------------------------------------------
   while ((i < limit) && !conv) {
     if (verbose) {
       VCT::vct.info(acc, largest, lwhere, smallest, swhere);
       cout << setiosflags(ios::scientific)
            << "largest: "  << largest  << " (" << lwhere << ") "
            << "smallest: " << smallest << " (" << swhere << ") ";
       if ( (acc < one - round_zero) || (acc > one + round_zero) )
         cout << " - sum: " << acc << " !!!\n";
       else
         cout << "\n";
     }
     for (k=0; (k<big_step && i<limit); k++) {
       cout << "\r                                      "
            << "\rIteration " << i << ": "; cout.flush();
       //======================
       switch (p_type) {
         case no_precond : buf.mult_CND(VCT::vct); 
                                                    break;
         case adtv       : buf.pre_adtv(VCT::vct); 
                           buf.norm_sum(VCT::vct);  break;
         case altn       : buf.pre_altn(VCT::vct); 
                           buf.norm_sum(VCT::vct);  break;
         case mult       : buf.pre_mult(VCT::vct); 
                           buf.norm_sum(VCT::vct);  break;
         case diag       : buf.pre_diag(VCT::vct); 
                           buf.norm_sum(VCT::vct);  break;
         case plyn       : buf.pre_plyn(VCT::vct); 
                           buf.norm_sum(VCT::vct);  break;
         case ipln       : buf.pre_ipln(VCT::vct); 
                           buf.norm_sum(VCT::vct);  break;
       }
       //======================
       i++;
       //********************************************************
       //****************** Operation included by Billy's request
        buf.normalize();
       //********************************************************
       if (test_error)
         if (conv = VCT::vct.test(buf, error_test_type, tol)) {
           VCT::vct.copy(buf);
           break;
         }
       if (i < limit) {
         cout << "\r                                      "
              << "\rIteration " << i << ": "; cout.flush();
         //======================
         switch (p_type) {
           case no_precond : VCT::vct.mult_CND(buf);
                                                      break;
           case adtv       : VCT::vct.pre_adtv(buf); 
                             VCT::vct.norm_sum(buf);  break;
           case altn       : VCT::vct.pre_altn(buf); 
                             VCT::vct.norm_sum(buf);  break;
           case mult       : VCT::vct.pre_mult(buf); 
                             VCT::vct.norm_sum(buf);  break;
           case diag       : VCT::vct.pre_diag(buf); 
                             VCT::vct.norm_sum(buf);  break;
           case plyn       : VCT::vct.pre_plyn(buf); 
                             VCT::vct.norm_sum(buf);  break;
           case ipln       : VCT::vct.pre_ipln(buf); 
                             VCT::vct.norm_sum(buf);  break;
         }
         //======================
         i++;
         //********************************************************
         //****************** Operation included by Billy's request
          VCT::vct.normalize();
         //********************************************************
         if (test_error)
           if (conv = VCT::vct.test(buf, error_test_type, tol))
             break;
       }
       else {
         VCT::vct.copy(buf);
       }
     }
   }
   //-----------------------------------------------------------------------------------
   T.stop();

   if (p_type == no_precond)
     CND::cnd.minus_identity();
   if (PRF::prf.it_type() == cnv_it) {        // Convergence verification
     buf.mult_CND(VCT::vct);                  //   One extra multiplication is made
     resid = buf.norm_inf();                  //     and the largest element is
   }                                          //     considered as the residue
   Notify_Time_Spend(T, "Power solution", i, resid);
   VCT::vct.write();
   Log_Out_Solution(T, i, Power, p_type, resid);
 }

 // cout << "vct a la fin de la mult :\n";
 // VCT::vct.debug();

 VCT::vct.desalloc_static();
 return(resid < PRF::prf.min_err());
}




//====================================================================================//
//  GMRES Method                                                                      //
//                                                                                    //
//    This function performs the gmres method to the infinitesimal generator stored   //
// into the global structure 'CND::cnd' (continuous-time normalized descriptor).      //
//                                                                                    //
//    The explicit input parameter is:                                                //
//    - 'p_type'      the type of precondition to be used                             //
//                                                                                    //
//    The implicit input parameters are:                                              //
//    - 'CND::cnd'               the infinitesimal generator                          //
//    - 'PRF::prf.it_type()'     fixed iterations or convergence test                 //
//    - 'PRF::prf.err_type()'    error test type (indiv./accum. - abs./relat.)        //
//    - 'PRF::prf.min_err()'     tolerance for convergence                            //
//    - 'PRF::prf.max_it()'      max. number of iterations                            //
//    - 'PRF::prf.verb()'        verbose mode                                         //
//    - 'PRF::prf.k_size()'      Krylov subspace size                                 //
//                                                                                    //
//    The implicit output parameter is:                                               //
//    - 'VCT::vct'               the resulting vector (solution)                      //
//====================================================================================//
bool GMRES_CND(const precond_typ p_type)
{
 int   i;               // iteration counter
 int   j, k, l;         // krylov subspace counters
 Timer T;               // timer
 bool  abort = false;   // a flag to abort the iteration after the initial test
 bool  conv;            // the precision for convergence was reached
 rp    resid;           // the residue of the vector
 rp    acc;             // the sum of all elements of the vector
 rp    largest;         // the largest element of the vector
 int   lwhere;          // the position of the largest element of the vector
 rp    smallest;        // the smallest element of the vector
 int   swhere;          // the position of the smallest element of the vector

 bool  test_error       = (PRF::prf.it_type() != fix_it); // verify convergence
 e_typ error_test_type  = PRF::prf.err_type();            // error verification type
 int   limit            = PRF::prf.max_it();              // max. number of iterations
 rp    tol              = PRF::prf.min_err();             // tolerance for convergence
 bool  verbose          = PRF::prf.verb();                // verbose mode
 int   restart          = PRF::prf.k_size();              // krylov subspace size

 VCT     * vv;                                      // vectors of the krylov subspace 
 full_mat  hh;                                      // Hessenberg matrix
 rp      * rhs;                                     // right hand side for the 'hh'
 rp      * c;                                       // indexes for the plane rotation
 rp      * s;                                       // indexes for the plane rotation
 rp      beta;                                      // normalizing factor for 'hh'
 rp      gamma;                                     // factor for plane rotation

 cout.setf(ios::scientific);
 cout.precision(peps_precision);
 //-------------------------------------------------- Allocation of temporary structures
 vv = new VCT[restart];                        // vectors of the krylov subspace
 for (j=0; j<restart; j++) {
   vv[j].create(VCT::vct.size(), VCT::vct.real_size());
   vv[j].get_CND();
 }
 hh.create(restart);                           // Hessenberg matrix
 hh.reset();
 
 rhs = new rp[restart];                        // right hand side of the 'hh' matrix
 c   = new rp[restart];                        // indexes for plane rotation
 s   = new rp[restart];                        // indexes for plane rotation

 CND::cnd.reset_aunf();
 if (PRF::prf.method() == meth_S) {  //only find cuts for the split method
    T.clear(); T.start();
    VCT::vct.setup_aunfs(); //discover the best cuts for the terms
    T.stop();
    CND::cnd.set_aunf_time(T.usertime());
 }

 VCT::vct.alloc_static();
 
 cout << "Verifying Descriptor: "; cout.flush();
 vv[0].mult_CND(VCT::vct);
 resid = vv[0].norm_inf();
 conv = (resid < tol);
 acc = vv[0].sum();
 i = 0;
 cout << "\r                                      "
      << "\rIteration " << i << ": "; cout.flush();
 if ( (acc < -round_zero) || (acc > round_zero) )                        // initial test
   if  ( (acc < - PRF::prf.min_err()) || (acc > PRF::prf.min_err()) ) {
     cout << "\nWARNING: sum of residual vector = " << acc << " (it should be zero)"
          << "\nIteration 0: ";
   }
   else {
     cout << "\n*******************************************************************"
          << "\n***       the descriptor is not a infinitesimal generator       ***"
          << "\n*******************************************************************"
          << "\n                   Aborting the iterations"
          << "\n Too large sum of residual vector = " << acc  << " (it should be zero)\n";
     abort = true;
   }
 if (!abort) {
   T.clear(); T.start();
   //-----------------------------------------------------------------------------------
   while ((i < limit) && !conv)           
   { if (verbose) {
       VCT::vct.info(acc, largest, lwhere, smallest, swhere);
       cout << setiosflags(ios::scientific)
            << "largest: "  << largest  << " (" << lwhere << ") "
            << "smallest: " << smallest << " (" << swhere << ") ";
       if ( (acc < one - round_zero) || (acc > one + round_zero) )
         cout << " - sum: " << acc << " !!!\n";
       else
         cout << "\n";
     }
     //======================
     switch (p_type) {
       case no_precond : vv[0].mult_CND(VCT::vct); break;
       case adtv       : vv[0].pre_adtv(VCT::vct); break;
       case altn       : vv[0].pre_altn(VCT::vct); break;
       case mult       : vv[0].pre_mult(VCT::vct); break;
       case diag       : vv[0].pre_diag(VCT::vct); break;
       case plyn       : vv[0].pre_plyn(VCT::vct); break;
       case ipln       : vv[0].pre_ipln(VCT::vct); break;
     }
     //======================
     i++;
     cout << "\r                                      "
          << "\rIteration " << i << ": "; cout.flush();
     resid = vv[0].norm2();                 // computes the residue
     if (test_error)
       if (resid < tol) {                   // if 'vv[0]' is the solution
         conv = true;                       // convergence found
         break;                             // avoid internal steps loop
       }
     vv[0].scale(-one/resid);
     rhs[0] = resid;
     //============================================================ internal GMRES steps
     for (j=1; j<restart; j++) {            // for all other vectors of the subspace
       //======================
       switch (p_type) {
         case no_precond : vv[j].mult_CND(vv[j-1]); break;
         case adtv       : vv[j].pre_adtv(vv[j-1]); break;
         case altn       : vv[j].pre_altn(vv[j-1]); break;
         case mult       : vv[j].pre_mult(vv[j-1]); break;
         case diag       : vv[j].pre_diag(vv[j-1]); break;
         case plyn       : vv[j].pre_plyn(vv[j-1]); break;
         case ipln       : vv[j].pre_ipln(vv[j-1]); break;
       }
       //======================
       i++;
       cout << "\r                                      "
            << "\rIteration " << i << ": "; cout.flush();
       //--------------------------------------------------------- modified gram-schimdt
       for (k=0; k<j; k++) {                    // for the calculated subspace '0..j-1'
         beta = vv[j].dot(vv[k]);                  // dot product: 'vv[j] . vv[k]'
         hh.coord(k,j-1, beta);                    // sets 'k,j-1' element of 'hh'
         vv[j].scale_add(vv[k], -beta);            // linear combination of
       }                                           //           'vv[j]' and 'vv[k]'
       beta = vv[j].norm2();                    // the second norm of 'vv[j]'
       hh.coord(j,j-1, beta);                   // sets 'j,j-1' element of 'hh'
       if (beta != zero)                        // if 'hh' is not singular
         vv[j].scale(one/beta);                    // orthonomalizes 'vv[j+1]'
       else                                     // if 'hh' is singular
         break;                                    // stops the loop (performs a restart
       //--------------------------------------------------------- factorization of 'hh'
       for (k=1; k<j; k++) {                    // for the calculated subspace '1..j-1'
         beta = hh.coord(k-1, j-1);                                  // norm. diagonal
         hh.coord(k-1, j-1, c[k-1] *  beta + s[k-1] * hh.coord(k,j-1)); // eliminates 
         hh.coord(k,   j-1, s[k-1] * -beta + c[k-1] * hh.coord(k,j-1)); // the row 'j-1'
       }
       //----------------------------------------------------------- next plane rotation
       gamma = sqrt(hh.coord(j-1,j-1) * hh.coord(j-1,j-1)            // computes the
                  + hh.coord(j,  j-1) * hh.coord(j,  j-1));          // rotation factor
       if (gamma == zero)                       // if 'hh' is singular
         break;                                    // stops the loop (performs a restart
       c[j-1]   = hh.coord(j-1,j-1) / gamma;    // computes rotation for 'vv[j-1]'
       s[j-1]   = hh.coord(j,  j-1) / gamma;    // computes rotation for 'vv[j]'
       rhs[j]   = -s[j-1] * rhs[j-1];           // normalizes the 'rhs' of 'vv[j]'
       rhs[j-1] =  c[j-1] * rhs[j-1];           // normalizes the 'rhs' of 'vv[j-1]'
       hh.coord(j-1,j-1,
          c[j-1] * hh.coord(j-1,j-1) + s[j-1] * hh.coord(j,j-1)); // rotates 'vv[j-1]'
       //-------------------------------------------------------------- convergence test
       resid = absv(rhs[j]);                     // computes the residue
       if (test_error)
         if (resid < tol) {                      // if 'vv[j]' is the solution
           conv = true;                             // convergence found
           break;                                   // exit internal steps loop
         }
     }
     //----------------------------------------------------- end of internal GMRES steps
     if (j == restart)                       // it forces 'j' to point to the last
       j = restart-2;                        //   computed 'vv' vector
     else
       j--;
     //=========================================================== compute 'hh' solution
     rhs[j] /= hh.coord(j,j);                // nomalizes the last position of 'rhs'
     for (k=1; k<=j; k++) {                  // for the other positions of 'rhs'
       beta = rhs[j-k];                         // performs a backward substituition
       for (l=(j-k)+1; l<=j; l++)
         beta -= hh.coord((j-k), l) * rhs[l];
       rhs[j-k] = beta / hh.coord(j-k, j-k);    // norm. the 'j-k'-th position of 'rhs'
     }
     for (k=0; k<=j; k++)                    // linear combination of all computed
       VCT::vct.scale_add(vv[k], rhs[k]);    //   vectors 'vv[k]' ('k = 0 .. j')
     VCT::vct.normalize();                   // normalize the resulting vector
     //---------------------------------------------------------------- convergence test
     resid = absv(rhs[j]);
     if (test_error)
       if (resid < tol)                      // if 'VCT::vct' is the solution
         conv = true;
   }
   //-----------------------------------------------------------------------------------
   T.stop();
   if (PRF::prf.it_type() == cnv_it) {        // Convergence verification
     vv[0].mult_CND(VCT::vct);                  //   One extra multiplication is made
     resid = vv[0].norm_inf();                  //     and the largest element is
   }                                            //     considered as the residue
   Notify_Time_Spend(T, "GMRES solution", i, resid);
   VCT::vct.write();
   Log_Out_Solution(T, i, Gmres, p_type, resid);
 }
 delete[] c;
 delete[] s;
 delete[] rhs;
 hh.erase();
 for (j=0; j<restart; j++)
   vv[j].erase();
 VCT::vct.desalloc_static();
 return(resid < PRF::prf.min_err());
}

//====================================================================================//
//  Arnoldi Method                                                                    //
//                                                                                    //
//    This function performs the arnoldi method to the infinitesimal generator stored //
// into the global structure 'CND::cnd' (continuous-time normalized descriptor).      //
//                                                                                    //
//    The explicit input parameter is:                                                //
//    - 'p_type'      the type of precondition to be used                             //
//                                                                                    //
//    The implicit input parameters are:                                              //
//    - 'CND::cnd'               the infinitesimal generator                          //
//    - 'PRF::prf.it_type()'     fixed iterations or convergence test                 //
//    - 'PRF::prf.err_type()'    error test type (indiv./accum. - abs./relat.)        //
//    - 'PRF::prf.min_err()'     tolerance for convergence                            //
//    - 'PRF::prf.max_it()'      max. number of iterations                            //
//    - 'PRF::prf.verb()'        verbose mode                                         //
//    - 'PRF::prf.k_size()'      Krylov subspace size                                 //
//                                                                                    //
//    The implicit output parameter is:                                               //
//    - 'VCT::vct'               the resulting vector (solution)                      //
//====================================================================================//
bool Arnoldi_CND(const precond_typ p_type)
{
 int   i;               // iteration counter
 int   j, k, l;         // krylov subspace counters
 Timer T;               // timer
 bool  abort = false;   // a flag to abort the iteration after the initial test
 bool  conv;            // the precision for convergence was reached
 rp    resid;           // the residue of the vector
 rp    acc;             // the sum of all elements of the vector
 rp    largest;         // the largest element of the vector
 int   lwhere;          // the position of the largest element of the vector
 rp    smallest;        // the smallest element of the vector
 int   swhere;          // the position of the smallest element of the vector

 bool  test_error       = (PRF::prf.it_type() != fix_it); // verify convergence
 e_typ error_test_type  = PRF::prf.err_type();            // error verification type
 int   limit            = PRF::prf.max_it();              // max. number of iterations
 rp    tol              = PRF::prf.min_err();             // tolerance for convergence
 bool  verbose          = PRF::prf.verb();                // verbose mode
 int   restart          = PRF::prf.k_size();              // krylov subspace size

 VCT     * vv;                                      // vectors of the krylov subspace 
 full_mat  hh;                                      // Hessenberg matrix
 rp      * rhs;                                     // right hand side for the 'hh'
 rp      * c;                                       // indexes for the plane rotation
 rp      * s;                                       // indexes for the plane rotation
 rp      beta;                                      // normalizing factor for 'hh'
 rp      gamma;                                     // factor for plane rotation

 cout.setf(ios::scientific);
 cout.precision(peps_precision);
 //-------------------------------------------------- Allocation of temporary structures

 vv = new VCT[restart];                        // vectors of the krylov subspace
 for (j=0; j<restart; j++) {
   vv[j].create(VCT::vct.size(), VCT::vct.real_size());
   vv[j].get_CND();
 }
 hh.create(restart);                           // Hessenberg matrix
 hh.reset();

 rhs = new rp[restart];                        // right hand side of the 'hh' matrix
 c   = new rp[restart];                        // indexes for plane rotation
 s   = new rp[restart];                        // indexes for plane rotation

 CND::cnd.reset_aunf();
 if (PRF::prf.method() == meth_S) {  //only find cuts for the split method
    T.clear(); T.start();
    VCT::vct.setup_aunfs(); //discover the best cuts for the terms
    T.stop();
    CND::cnd.set_aunf_time(T.usertime());
 }

 VCT::vct.alloc_static();
 
 cout << "Verifying Descriptor: "; cout.flush();
 vv[0].mult_CND(VCT::vct);
 resid = vv[0].norm_inf();
 conv = (resid < tol);
 acc = vv[0].sum();
 i = 0;
 cout << "\r                                      "
      << "\rIteration " << i << ": "; cout.flush();
 if ( (acc < -round_zero) || (acc > round_zero) )                        // initial test
   if  ( (acc < - PRF::prf.min_err()) || (acc > PRF::prf.min_err()) ) {
     cout << "\nWARNING: sum of residual vector = " << acc << " (it should be zero)"
          << "\nIteration 0: ";
   }
   else {
     cout << "\n*******************************************************************"
          << "\n***       the descriptor is not a infinitesimal generator       ***"
          << "\n*******************************************************************"
          << "\n                   Aborting the iterations"
          << "\n Too large sum of residual vector = " << acc  << " (it should be zero)\n";
     abort = true;
   }
 if (!abort) {
   T.clear(); T.start();
   //-----------------------------------------------------------------------------------
   while ((i < limit) && !conv)           
   { if (verbose) {
       VCT::vct.info(acc, largest, lwhere, smallest, swhere);
       cout << setiosflags(ios::scientific)
            << "largest: "  << largest  << " (" << lwhere << ") "
            << "smallest: " << smallest << " (" << swhere << ") ";
       if ( (acc < one - round_zero) || (acc > one + round_zero) )
         cout << " - sum: " << acc << " !!!\n";
       else
         cout << "\n";
     }
     //======================
     switch (p_type) {
       case no_precond : vv[0].mult_CND(VCT::vct); break;
       case adtv       : vv[0].pre_adtv(VCT::vct); break;
       case altn       : vv[0].pre_altn(VCT::vct); break;
       case mult       : vv[0].pre_mult(VCT::vct); break;
       case diag       : vv[0].pre_diag(VCT::vct); break;
       case plyn       : vv[0].pre_plyn(VCT::vct); break;
       case ipln       : vv[0].pre_ipln(VCT::vct); break;
     }
     //======================
     i++;
     cout << "\r                                      "
          << "\rIteration " << i << ": "; cout.flush();
     resid = vv[0].norm2();                 // computes the residue
     if (test_error)
       if (resid < tol) {                   // if 'vv[0]' is the solution
         conv = true;                       // convergence found
         break;                             // avoid internal steps loop
       }
     vv[0].scale(-one/resid);
     rhs[0] = resid;
     //========================================================== internal Arnoldi steps
     for (j=1; j<restart; j++) {            // for all other vectors of the subspace
       //======================
       switch (p_type) {
         case no_precond : vv[j].mult_CND(vv[j-1]); break;
         case adtv       : vv[j].pre_adtv(vv[j-1]); break;
         case altn       : vv[j].pre_altn(vv[j-1]); break;
         case mult       : vv[j].pre_mult(vv[j-1]); break;
         case diag       : vv[j].pre_diag(vv[j-1]); break;
         case plyn       : vv[j].pre_plyn(vv[j-1]); break;
         case ipln       : vv[j].pre_ipln(vv[j-1]); break;
       }
       //======================
       i++;
       cout << "\r                                      "
            << "\rIteration " << i << ": "; cout.flush();
       //--------------------------------------------------------- modified gram-schimdt
       for (k=0; k<j; k++) {                    // for the calculated subspace '0..j-1'
         beta = vv[j].dot(vv[k]);                  // dot product: 'vv[j] . vv[k]'
         hh.coord(k,j-1, beta);                    // sets 'k,j-1' element of 'hh'
         vv[j].scale_add(vv[k], -beta);            // linear combination of
       }                                           //           'vv[j]' and 'vv[k]'
       beta = vv[j].norm2();                    // the second norm of 'vv[j]'
       hh.coord(j,j-1, beta);                   // sets 'j,j-1' element of 'hh'
       if (beta != zero)                        // if 'hh' is not singular
         vv[j].scale(one/beta);                    // orthonomalizes 'vv[j+1]'
       else                                     // if 'hh' is singular
         break;                                    // stops the loop (performs a restart
       //--------------------------------------------------------- factorization of 'hh'
       for (k=1; k<j; k++) {                    // for the calculated subspace '1..j-1'
         beta = hh.coord(k-1, j-1);                                  // norm. diagonal
         hh.coord(k-1, j-1, c[k-1] *  beta + s[k-1] * hh.coord(k,j-1)); // eliminates 
         hh.coord(k,   j-1, s[k-1] * -beta + c[k-1] * hh.coord(k,j-1)); // the row 'j-1'
       }
       //-------------------------------------------------------------- convergence test
       if (hh.coord(j-1,j-1) != zero)            // computes the residue (if possible)
         resid = absv(hh.coord(j,j-1) * (rhs[j-1] / hh.coord(j-1,j-1)));
       if (test_error)
         if (resid < tol) {                      // if 'vv[j]' is the solution
           conv = true;                             // convergence found
           break;                                   // exit internal steps loop
         }
       //----------------------------------------------------------- next plane rotation
       if (j < restart-1) {                       // (not performed for the last vector)
         gamma = sqrt(hh.coord(j-1,j-1) * hh.coord(j-1,j-1)          // computes the
                    + hh.coord(j,  j-1) * hh.coord(j,  j-1));        // rotation factor
         if (gamma == zero)                        // if 'hh' is singular
           break;                                     // stops the loop (perf. a restart
         c[j-1]   = hh.coord(j-1,j-1) / gamma;     // computes rotation for 'vv[j-1]'
         s[j-1]   = hh.coord(j,  j-1) / gamma;     // computes rotation for 'vv[j]'
         rhs[j]   = -s[j-1] * rhs[j-1];            // normalizes the 'rhs' of 'vv[j]'
         rhs[j-1] =  c[j-1] * rhs[j-1];            // normalizes the 'rhs' of 'vv[j-1]'
         hh.coord(j-1,j-1,
            c[j-1] * hh.coord(j-1,j-1) + s[j-1] * hh.coord(j,j-1)); // rotates 'vv[j-1]'
       }
     }
     //--------------------------------------------------- end of internal Arnoldi steps
     if (j == restart)                       // it forces 'j' to point to the last
       j = restart-2;                        //   computed 'vv' vector
     else
       j--;
     //=========================================================== compute 'hh' solution
     rhs[j] /= hh.coord(j,j);                // nomalizes the last position of 'rhs'
     for (k=1; k<=j; k++) {                  // for the other positions of 'rhs'
       beta = rhs[j-k];                         // performs a backward substituition
       for (l=(j-k)+1; l<=j; l++)
         beta -= hh.coord(j-k, l) * rhs[l];
       rhs[j-k] = beta / hh.coord(j-k, j-k);    // norm. the 'j-k'-th position of 'rhs'
     }
     for (k=0; k<=j; k++)                    // linear combination of all computed
       VCT::vct.scale_add(vv[k], rhs[k]);    //   vectors 'vv[k]' ('k = 0 .. j')
     VCT::vct.normalize();                   // normalize the resulting vector
     //---------------------------------------------------------------- convergence test
     if (test_error)
       if (resid < tol)                      // if 'VCT::vct' is the solution
         conv = true;
   }
   //-----------------------------------------------------------------------------------
   T.stop();
   if (PRF::prf.it_type() == cnv_it) {        // Convergence verification
     vv[0].mult_CND(VCT::vct);                  //   One extra multiplication is made
     resid = vv[0].norm_inf();                  //     and the largest element is
   }                                            //     considered as the residue
   Notify_Time_Spend(T, "Arnoldi solution", i, resid);
   VCT::vct.write();
   Log_Out_Solution(T, i, Arnoldi, p_type, resid);
 }

 delete[] c;
 delete[] s;
 delete[] rhs;
 hh.erase();

 for (j=0; j<restart; j++)
   vv[j].erase();

 VCT::vct.desalloc_static();

 return(resid < PRF::prf.min_err());
}

//====================================================================================//
//  Power HBF                                                                         //
//====================================================================================//

bool Power_HBF(VCT & buf)
{
 int       i;
 int       half, rest;
 rp        error;
 rp        resid;
 i_typ     iteration_type;
 e_typ     error_test_type;
 Timer     T;

 HBF::hbf.scale();
 iteration_type  = PRF::prf.it_type();
 error_test_type = PRF::prf.err_type();
 half            = PRF::prf.max_it() / 2;
 rest            = PRF::prf.max_it() % 2;
 error           = PRF::prf.min_err();

 T.clear(); T.start();
 switch (iteration_type) {
   case fix_it : for (i=0; i<half; i++) {
                   VCT::vct.mult_HBF(buf);
                   buf.mult_HBF(VCT::vct);
                 }
                 if (rest == 0)
                   VCT::vct.copy(buf);
                 else
                   VCT::vct.mult_HBF(buf);
                 break;
   default     : VCT::vct.mult_HBF(buf);

     // cout << "Valeurs de vct apres la 1ere mult : \n";
     // VCT::vct.debug();
     // return false;
 
                 for (i=0; i<half; i++) {
                   if (VCT::vct.test(buf, error_test_type, error))
                     break;
                   buf.mult_HBF(VCT::vct);
                   VCT::vct.mult_HBF(buf);
                 }
 }
 T.stop();
   if (PRF::prf.it_type() == cnv_it) {        // Convergence verification
     buf.mult_HBF(VCT::vct);                  //   One extra multiplication is made
     resid = buf.test_hbf(VCT::vct, error_test_type); //     and the largest element is
   }                                                  //     considered as the residue
 Notify_Time_Spend(T, "HBF power solution", (i*2)+rest, resid);
 VCT::vct.write();
 // Log_Out_Solution(T, i, Hbf, 0, resid);

 // cout << "vct a la fin de la mult :\n";
 // VCT::vct.debug();

 return(resid < PRF::prf.min_err());
}


//===========================================================================================//
//  Standard Uniformization Method  - Transient solution                                     //
//                                                                                           //
//    This function performs the uniformization method to the infinitesimal generator stored //
// into the global structure 'CND::cnd' (continuous-time normalized descriptor).   	       //
// 	The Standard Uniformization is based on  the reduction of a Continuous-Time structure   //
// to a continuous-Time Normalized subordinaded to a Poisson Process.  		                //
//                                                                                           //
//    The explicit input parameters are:                                                     //
//    - 'buf'         the initial vector;                                                    //
//    - 'p_type'      the type of precondition to be used                                    //
//    - 'tm'	      the time                                                                 //
//                                                                                           //
//    The implicit input parameters are:                                                     //
//    - 'CND::cnd'               the infinitesimal generator                                 //
//    - 'PRF::prf.it_type()'     fixed iterations or convergence test                        //
//    - 'PRF::prf.err_type()'    error test type (indiv./accum. - abs./relat.)               //
//    - 'PRF::prf.min_err()'     tolerance for convergence                                   //
//    - 'PRF::prf.max_it()'      max. number of iterations                                   //
//    - 'PRF::prf.verb()'        verbose mode                                                //
//    - 'PRF::prf.k_size()'      interval between convergence check (if verbose mode)        //
//                                                                                           //
//    The implicit output parameter is:                                                      //
//    - 'VCT::vct'               the resulting vector (solution)                             //
//===========================================================================================//

bool Uniformization_CND(VCT & buf, rp _time)
{
 int   i;               // iteration counters
 ///int   j, k;
 Timer T;               // timer
 bool  abort = false;   // a flag to abort the iteration after the initial test
 bool  conv;            // the precision for convergence was reached
 rp    resid;           // the residue of the vector
 rp    acc;             // the sum of all elements of the vector
 rp    largest;         // the largest element of the vector
 int   lwhere;          // the position of the largest element of the vector
 rp    smallest;        // the smallest element of the vector
 int   swhere;          // the position of the smallest element of the vector
 rp    time;            // the time
 rp    bigest;          // the bigest diagonal element 
 rp    bigest_t;        // the bigest diagonal element times time
 rp    prod;
 rp    mbig;
 rp    sum;
 rp    sigma;
 int   maxit;           // max iterations in external loop
 VCT   aux;             // auxiliar vector

 bool  test_error       = (PRF::prf.it_type() != fix_it); // verify convergence
 e_typ error_test_type  = PRF::prf.err_type();            // error verification type
 rp    tol              = PRF::prf.min_err();             // tolerance for convergence
 bool  verbose          = PRF::prf.verb();                // verbose mode

 cout.setf(ios::scientific);
 cout.precision(peps_precision);

 CND::cnd.reset_aunf();
 if (PRF::prf.method() == meth_S) {  //only find cuts for the split method
    T.clear(); T.start();
    VCT::vct.setup_aunfs(); //discover the best cuts for the terms
    T.stop();
    CND::cnd.set_aunf_time(T.usertime());
 }

 VCT::vct.alloc_static();
 aux.create(VCT::vct.size(), VCT::vct.real_size());
 aux.get_CND();

 time = _time;
 bigest = 1/CND::cnd.n_factor();

 bigest_t = bigest * time;
 maxit = 1;
 if (bigest_t > 100){ // number of external iterations
   maxit = 1 + (int)(bigest_t/100);
   time = time/maxit;
 }  

 bigest_t = bigest * time;
 prod = exp(-bigest_t);
 mbig = 0;
 sum = prod;
 sigma = 1.0;
 while(!(sum > (1.0-tol))){ // number of internal iterations
   mbig++;
   sigma = sigma * (bigest_t/mbig);
   sum += sigma * prod;
 } 

 cout << "Iteration 0/" << (int)(maxit*mbig) << ": "; cout.flush();
 VCT::vct.mult_CND(buf);

 resid = VCT::vct.norm_inf();
 i = 1;
 conv = (resid < tol);

 VCT::vct.add(buf);

 acc = VCT::vct.sum();
 if ( (acc < one - round_zero) || (acc > one + round_zero) )            // initial test
   if  ( (acc < - PRF::prf.min_err()) || (acc > PRF::prf.min_err()) ) {
     cout << "\nWARNING: sum of residual vector = " << acc << " (it should be zero)"
          << "\nIteration 0: ";
   }
   else {
     cout << "\n*******************************************************************"
          << "\n***       the descriptor is not a infinitesimal generator       ***"
          << "\n*******************************************************************"
          << "\n                   Aborting the iterations"
          << "\n Too large sum of residual vector = " << acc  << " (it should be zero)\n";
     abort = true;
   }

 if(!abort){
   CND::cnd.add_identity();
   T.clear(); T.start();
   for(i=0; i<maxit; i++){ // external loop
     VCT::vct.copy(buf);
     if (verbose) {
       VCT::vct.info(acc, largest, lwhere, smallest, swhere);
       cout << setiosflags(ios::scientific)
            << "largest: "  << largest  << " (" << lwhere << ") "
            << "smallest: " << smallest << " (" << swhere << ") ";
       if ( (acc < one - round_zero) || (acc > one + round_zero) )
         cout << " - sum: " << acc << " !!!\n";
       else
         cout << "\n";
     }
     for(int k=1; k<=mbig; k++){ // internal loop
       cout << "\r                                                     "
            << "\rIteration " << (int)((i*mbig)+k) << "/" << (int)(maxit*mbig) << ": "; cout.flush();
       aux.mult_CND(VCT::vct);
       VCT::vct.scale_copy(aux,(bigest_t/k));
       buf.add(VCT::vct);
     }
     buf.scale(prod);
     buf.normalize();
   }
   T.stop(); 
   CND::cnd.minus_identity();
   time = time*maxit;

   if (PRF::prf.it_type() == cnv_it) {        // Convergence verification
     VCT::vct.mult_CND(buf);                  //   One extra multiplication is made
     resid = VCT::vct.norm_inf();             //     and the largest element is
   }                                          //     considered as the residue

   Notify_Time_Spend(T, "Uniformization solution", (int)(maxit*mbig), resid);
   VCT::vct.copy(buf);
   VCT::vct.write();
   Log_Out_Solution(T, (int)(maxit*mbig), Uniformization, no_precond, resid);
 }
 VCT::vct.desalloc_static();
 return(resid < PRF::prf.min_err());
}
