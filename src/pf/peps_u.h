//====================================================================================//
//                                                                                    //
//                              PEPS Global Declarations                              //
//                                                                                    //
//====================================================================================//
//  This File:   peps.h                     Language: C++  (xlC and CC)               //
//  Software:    PEPS 3.1                    Advisor: Brigitte.Plateau@imag.fr        //
//====================================================================================//
//  Creation:     5/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 19/sep/01                        by: mauro.charao@zipmail.com.br     //
//====================================================================================//
#include <fstream.h>              // file streams (i/o)
#include <iostream.h>             // std i/o with streams (cin, cout, cerr)
#include <iomanip.h>              // i/o manipulators (setiosflags-scientific)
#include <string.h>               // string manipulation (strcpy, strcat)
#include <math.h>                 // absolute values (abs), square roots (sqrt)
#include <limits.h>               // maximum and minimum values to int, float, etc
#include <new.h>                  // handler for new commands (test for allocation)
#include <stdio.h>                // i/o standard of C language (used for the parser)
#include <stdlib.h>               // std function calls, e.g. exit(), atoi()

//====================================================================================//
//   Release Date      ******************** CHANGE THIS DATE FOR NEW RELEASES OF PEPS //
//====================================================================================//

#define __RELEASE_DATE__   "Sep 11 2001"

//====================================================================================//
//   Types of Files Handled by Peps                                                   //
//====================================================================================//

enum file_types  // all files handled by PEPS
     { san_file ,   // textual description of SAN
       des_file ,   // textual description of DEScriptor
       tft_file ,   // textual representation of Function Tables
       fct_file ,   // textual representation of a singular FunCTion
       dic_file ,   // textual representation of DICtionaires
       res_file ,   // textual representation of integration function (RESults)
       dtr_file ,   // textual output of DicTionaiRes
       rea_file ,   // textual output of REAchable state of a probability vector
       inf_file ,   // internal representation of INtegration Functions
       dct_file ,   // internal representation of DiCTionaires
       dsc_file ,   // internal representation of DeSCriptors
       cnd_file ,   // int. repr. of scaled Continuous-time Normalized Descriptors
       lud_file ,   // int. representation of LU decomposed Descriptors
       vct_file ,   // internal representation of probability VeCTors
       rss_file ,   // internal representation of Reachable State Spaces
       hbf_file ,   // int. representation of sparse matrices in a Harwell-Boeing Format
       ftb_file ,   // internal representation of Function TaBles
       prf_file ,   // user PReFerences for parameters (numerical precision, etc)
       ops_file ,   // messages of misfunctions of the PEPS software
       tim_file ,   // messages of TIMes spend in PEPS software
       dbg_file ,   // textual output (Readable) with DeBuGging information
       hlp_file ,   // help comments for PEPS
       dot_file ,   // names without extension
       err_file };  // unrecognized extension

//====================================================================================//
//   Automata Classes                                                     Error Codes //
//====================================================================================//

class aut_set;   // set of automata                       (array of booleans)  1100
class aut_list;  // list of automata       (array of automata identificators)  1200
class aut_st;    // automata states                     (global state vector)  1300
class d_graph;   // functional dependency graph          (matrix of booleans)  1400

//====================================================================================//
//   Vectors                                                              Error Codes //
//====================================================================================//

class bools;     // array of booleans                      (compact storage)   2100
class RSS;       // reachable state spaces                  (main structure)   2200
class VCT;       // probability vectors (basic functions)   (main structure)   2300
                 // probability vectors (multiply functions)                   2400
                 // probability vectors (solve functions)    (not yet splited) 2500

//====================================================================================//
//   Matrices Storage                                                     Error Codes //
//====================================================================================//

class elem;      // elements of matrices                (rates or functions)   3100
class elem_list; // list of elements                                           3200
class san_mat;   // generic matrices of elements                               3300
class sp_mat;    // sparse matrices with only constant elements                3400
class full_mat;  // full matrices with only constant elements                  3500
class HBF;       // Harwell-Boeing format sparse matrices   (main structure)   3600

//====================================================================================//
//   Functional Stuff                                                     Error Codes //
//====================================================================================//

class func_tree; // evaluation tree of a function  (reverse polish notation)   4100
class scr_ft;    // scratch function table       (buffer to store functions)   4200
class FTB;       // functions table                         (main structure)   4300
class INF;       // integration functions table             (main structure)   4400

//====================================================================================//
//   Descriptor Special Structures                                        Error Codes //
//====================================================================================//

class st_map;    // state maps (relation between reduced and original states)  5100
class jumps;     // permutation jumps table (table of nleft, nright and jump)  5200
class DSC;       // descriptors                              (main structure)  5300
class CND;       // scaled continuous-time descriptors       (main structure)  5400
class LUD;       // LU decomposed descriptors                (main structure)  5500

//====================================================================================//
//   User Interface                                                                   //
//====================================================================================//

class PRF;       // User Preferences                         (main structure)  6100

//====================================================================================//
//   Miscelaneous                                                                     //
//====================================================================================//

class Timer;    // general chronometers from pac++

//====================================================================================//
//   Basic Definitions                                                                //
//====================================================================================//
typedef double      rp;                        // a rate or a probability
const   rp          zero = 0.0;                // double sized constants (rp) to avoid
const   rp          one  = 1.0;                //   problems passing parameters
const   rp          two  = 2.0;
const   rp          epsilon = 1e-4;            // a small rate or probability
const   rp          close_to_zero = 1e-16;     // a very small rate or probability
const   rp          round_zero = 1e-12;        // a rather small rate or probability
const   rp          visual_zero = 1e-5;        // smallest value displayed as decimal
const   int         peps_precision = 16;       // precision to handle (i/o) rp values
const   int         max_integer = INT_MAX;     // maximum integer (signed)
const   int         min_integer = INT_MIN;     // minimum integer (signed)
const   rp          max_float =  1e+50;        // max. float (actually just a huge one)
const   rp          min_float = -1e+50;        // min. float (actually just a tiny one)
#ifdef _CC_COMPILER_
  typedef int         bool;                    // boolean variables
  const   bool        true  = 1;               //   gcc and other non-standart c++
  const   bool        false = 0;               //   compilers have built-in bool types
#endif
const   int         mem_unit = 1024;           // unit of memory space (Kbytes)
const   int         max_stack_size = 20;       // maximum size of the evaluation stack
                                               //    (for functional elements)

//====================================================================================//
//   Peps Name Definitions                                                            //
//====================================================================================//

const int max_peps_name = 128;          // Maximum size (actually the last 4 are ".ext")

typedef char peps_name[max_peps_name];  // User Definided Names for any Peps Entity

const int max_peps_expr = 1024;         // Maximum size for expressions

typedef char peps_expr[max_peps_expr];  // User Definided expressions

// const peps_name no_name = "\0";      // Default empty value for peps_name

//====================================================================================//
//   Identifiers Definitions                                                          //
//====================================================================================//
typedef int any_id;        // for identifiers of any kind
const any_id               no_id = -1;

typedef int aut_id;        // for automata (anywhere)
const aut_id               no_aut = -1;
const aut_id               fst_aut = 0;

typedef int st_id;         // for states in an automaton
const st_id                no_st = -1;
const st_id                fst_st = 0;

typedef int func_id;       // for functions (used for functional transitions)
const func_id              no_func = -1;
const func_id              fst_func = 0;

typedef int ev_id;         // for synchronizing events
const ev_id                no_ev = -1;
const ev_id                fst_ev = 0;

typedef int m_coord;       // for small matrices coordinates
const m_coord              no_coord = -1;
const m_coord              fst_m_coord = 0;  // the first coordinate (in matrices)

//====================================================================================//
//   Pre-defined Enumerations                                                         //
//====================================================================================//

enum md_type    { discrete , continuous };        // Discrete and Continuous time models

enum p_m        { mat_plus , mat_minus };               // Synchronizing events matrices

enum mat_typ    { sparse , binary , identity , empty };            // san matrices types

enum elem_type  { rate , func };                                 // types of san element

enum l_r        { left , right };                            // left and right subspaces

enum hbf_format { c_format , f_format };       // formats for 'c' or 'fortran' languages

enum oper       { s_plus , s_minus , s_times , s_slash ,   // function tokens operations
                  s_mini , s_maxi  ,
                  s_eq   , s_neq   , s_gret  , s_less , s_geq  , s_leq   ,
                  s_and  , s_or    , s_xor   , s_imp  , s_dimp , s_not   ,
                  s_st   , s_rw    , s_cs_st , s_s_rw , s_p_rw , s_cs_rw , s_cp_rw ,
                  s_cnst , s_no_oper };

//====================================================================================//
//   Descriptor Terms and Normal Factors Types                                        //
//------------------------------------------------------------------------------------//
//     Thes eenumerations are used to choose automatically the re-ordering algorithm  //
// to multiply a probability vector by a normal factor or tensor product term.        //
//====================================================================================//

enum term_typ { idnt_term ,   // The term is an identity matrix
                null_term ,   // The term is an empty matrix
                cnst_term ,   // The term does not have functional elements
                A_term    ,   // The term is suitable for the method A
                B_term    ,   // The term is suitable for the method B
                C_term    };  // The term is suitable for the method C

enum fact_typ { idnt_mat ,   // The normal factor is an identity matrix
                null_mat ,   // The normal factor is an empty matrix
                cnst_mat ,   // The normal factor has no functional element
                func_mat };  // The normal factor has functional elements

//====================================================================================//
//   User Preferences Enumerations                                                    //
//====================================================================================//
//  Error verification for convergence test:                                          //
//        - 'abs_ind_err'  maximum absolute error for each vector element             //
//        - 'abs_acc_err'  maximum absolute error for the sum of all vector elements  //
//        - 'rel_ind_err'  maximum relative error for each vector element             //
//------------------------------------------------------------------------------------//
enum e_typ  { abs_ind_err , abs_acc_err , rel_ind_err };
//====================================================================================//
//  Stop iterations control:                                                          //
//        - 'fix_it'     perform a fixed number of iterations        (no test at all) //
//        - 'stb_it'     perform the iterations until stability      (test to stop)   //
//        - 'cnv_it'     perform the iterations until stability and test convergence  //
//------------------------------------------------------------------------------------//
enum i_typ  { fix_it , stb_it , cnv_it };
//====================================================================================//
//  Functional elements aproximation policy:                                          //
//        - 'lu_user'      dialogues with user to each unsolved function              //
//        - 'lu_max'       replaces the function by its maximum evaluation            //
//        - 'lu_min'       replaces the function by its minimum evaluation            //
//        - 'lu_ave'       replaces the function by its average evaluation            //
//        - 'lu_zero'      replaces the function by zero                              //
//------------------------------------------------------------------------------------//
enum f_aprox { lu_user , lu_max , lu_min , lu_ave , lu_zero };
//====================================================================================//
//  LU Decomposition policy:                                                          //
//        - 'std_lu'       LU Doolitlle decomp.              A = L * U                //
//        - 'sor_lu'       SOR-like decomp.                  A = L + diag(L) * U      //
//------------------------------------------------------------------------------------//
enum lu_typ { std_lu , sor_lu };
//====================================================================================//
//  LU matrix regularization:                                                         //
//        - 'no_reg'      keepping the matrix as it is                                //
//        - 'shift_reg'   shifting diagonal elements of an epsilon                    //
//        - 'wingl_reg'   equalizing diagonal elements to the largest abs. value      //
//        - 'tychv_reg'   multiply the matrix by its inverse                          //
//        - 'aprox_reg'   replacing null diagonal elements by an epsilon              //
//        - 'one_reg'     replacing null diagonal elements by one                     //
//------------------------------------------------------------------------------------//
enum reg_typ { no_reg , shift_reg , wingl_reg , tychv_reg , aprox_reg , one_reg };
//====================================================================================//
//  LU solving option (in precontionning):                                            //
//        - 'both_lu'      solve with matrix L and U                                  //
//        - 'just_l'       solve with matrix L only                                   //
//        - 'just_u'       solve with matrix U only                                   //
//------------------------------------------------------------------------------------//
enum sol_typ  { both_lu , just_l , just_u };
//====================================================================================//
//  Choice of initial vector:                                                         //
//        - 'eq_vec'       equiprobable vector                                        //
//        - 'in_vec'       vector defined by an input file                            //
//        - 'ap_vec'       aproximated by the inverse of the descriptor diagonal      //
//------------------------------------------------------------------------------------//
enum v_typ  { eq_vec  , in_vec , ap_vec };
//====================================================================================//
//  Vector-Descriptor multiplication method:                                          //
//        - 'meth_A' do not change the automata order            (avoid permutations) //
//        - 'meth_B' change the order only of the product terms      (minimize perm.) //
//        - 'meth_C' change the order of the normal factors and product terms         //
//                                                             (minimize evaluations) //
//        - 'meth_M' uses different methods to each product term      (minimize both) //
//------------------------------------------------------------------------------------//
enum m_typ  { meth_A  , meth_B , meth_C , meth_M };
//====================================================================================//
//   Standard Solution Options                                                        //
//         - 'power'         power method                                             //
//         - 'arnoldi'       flexibe arnoldi method                                   //
//         - 'chebyshev'     polynomial chebyshev                                     //
//         - 'gmres'         flexibe gmres method                                     //
//------------------------------------------------------------------------------------//
enum solution_typ { power , arnoldi , gmres , chebyshev };
//====================================================================================//
//   Preconditionning Options    (matrix used as precodinner)                         //
//         - 'no_precond'    identity (no preconditionning)                           //
//         - 'adtv'          aditive combination of Ai terms                          //
//         - 'altn'          alternate use of Ai terms                                //
//         - 'mult'          multiplicative combination of Ai terms                   //
//         - 'diag'          inverse of the diagonal                                  //
//         - 'plyn'          polynomial approach of the inverse of A                  //
//         - 'ipln'          polynomial approach of the inverse of I - A              //
//         - 'cheb'          polynomial chebyshev                                     //
//------------------------------------------------------------------------------------//
enum precond_typ { no_precond , adtv , altn , mult , diag , plyn , ipln , cheb };

//====================================================================================//
//   User Preferences Defaults                                                        //
//====================================================================================//

const bool    def_verbose = true;          // Verbose mode
const int     def_iter    = 1000;          // Maximum iteration number
const i_typ   def_i_typ   = cnv_it;        // Iteration type
const rp      def_err     = 1e-10;         // Minimum error
const e_typ   def_e_typ   = abs_ind_err;   // Error verification type
const rp      def_thresh  = close_to_zero; // Threshold for LU decomposition
const f_aprox def_f_aprox = lu_ave;        // LU funct. aproximation type
const lu_typ  def_lu_typ  = std_lu;        // LU decomposition type
const sol_typ def_sol_typ = both_lu;       // LU solution type
const v_typ   def_v_typ   = eq_vec;        // Initial vector type
const m_typ   def_m_typ   = meth_M;        // Vector-Descriptor Multiplication method
const int     def_krylov  = 10;            // Krylov space size to GMRES
const int     def_poly    = 5;             // Size of the polynomial aproximation

//====================================================================================//
//   Global Inline functions                                                          //
//====================================================================================//

inline rp absv(const rp x) //===================== absolute values =====================
{
 if (x < zero)
   return(-x);
 return(x);
}

//====================================================================================//
//   Includes Peps Classes                                                Error Codes //
//====================================================================================//

#include "autset.h"    // set of automata (class aut_set)                 1100
#include "autlist.h"   // list of automata (class aut_list)               1200
#include "autst.h"     // state of automata (class aut_st)                1300
#include "bools.h"     // vector of booleans (class bools)                2100
#include "dgraph.h"    // dependency graph (class d_graph)                1400
#include "stmap.h"     // state map (class st_map)                        5100
#include "elem.h"      // basic san element (class elem)                  3100
#include "elemlist.h"  // list of san elements (class elem_list)          3200
#include "functree.h"  // evaluation tree of a function (class func_tree) 4100
#include "spmat.h"     // sparse matrices (class sp_mat)                  3400 *
#include "fullmat.h"   // full matrices (class full_mat)                  3500
#include "sanmat.h"    // san matrices (class san_mat)                    3300
#include "jumps.h"     // permutation jumps (class jumps)                 5200
#include "scrft.h"     // scratch function table (class scr_ft)           4200
#include "vct.h"       // probability vectors (class VCT)                 2300 2400
#include "lud.h"       // LU decomposed descriptors (class LUD)           5500 
#include "dsc.h"       // descriptors (class DSC)                         5300
#include "ftb.h"       // function table (class FTB)                      4300
#include "inf.h"       // integration functions table (class INF)         4400
#include "rss.h"       // reachable state space vector (class RSS)        2200
#include "cnd.h"       // cont.-time norm. descriptors (class CND)        5400
#include "hbf.h"       // sparse Harwell-Boeing format (class HBF)        3600
#include "dct.h"       // user names dictionary (class DCT)               6200
#include "prf.h"       // user preferences (class PRF)                    6100

//====================================================================================//
//   Includes Peps Packages                                               Error Codes //
//====================================================================================//

#include "solve.h"     // solution methods (solve.C)                      5600
#include "interf.h"    // user interface package                          6200
#include "compiler.h"  // compiler package (compiler.C)                   6300
#include "timer.h"     // timer definitions package (timer.C from pac++)

//====================================================================================//
//   Includes Peps Inlines                                                Error Codes //
//====================================================================================//

#include "autset.inl"    // set of automata (class aut_set)                 1100
#include "autlist.inl"   // list of automata (class aut_list)               1200
#include "autst.inl"     // state of automata (class aut_st)                1300
#include "bools.inl"     // vector of booleans (class bools)                2100
#include "dgraph.inl"    // dependency graph (class d_graph)                1400
#include "stmap.inl"     // state map (class st_map)                        5100
#include "elem.inl"      // basic san element (class elem)                  3100
#include "elemlist.inl"  // list of san elements (class elem_list)          3200
#include "functree.inl"  // evaluation tree of a function (class func_tree) 4100
//#include "spmat.inl"     // sparse matrices (class sp_mat)                  3400 *
#include "fullmat.inl"   // full matrices (class full_mat)                  3500
#include "sanmat.inl"    // san matrices (class san_mat)                    3300
#include "jumps.inl"     // permutation jumps (class jumps)                 5200
#include "scrft.inl"     // scratch function table (class scr_ft)           4200
#include "vct.inl"       // probability vectors (class VCT)                 2300 2400
#include "lud.inl"       // LU decomposed descriptors (class LUD)           5500
#include "dsc.inl"       // descriptors (class DSC)                         5300
#include "ftb.inl"       // function table (class FTB)                      4300
#include "inf.inl"       // integration functions table (class INF)         4400
#include "rss.inl"       // reachable state space vector (class RSS)        2200
#include "cnd.inl"       // cont.-time norm. descriptors (class CND)        5400
#include "hbf.inl"       // sparse Harwell-Boeing format (class HBF)        3600
#include "prf.inl"       // user preferences (class PRF)                    6100

