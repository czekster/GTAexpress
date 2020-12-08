
#ifndef _PEPS_H_
#define _PEPS_H_

//====================================================================================//
//                                                                                    //
//                              PEPS Global Declarations                              //
//                                                                                    //
//====================================================================================//
//  This File:   peps.h                     Language: C++  (xlC and CC)               //
//  Software:    PEPS 2009                   Advisor: Brigitte.Plateau@imag.fr        //
//====================================================================================//
//  Creation:     5/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 17/mar/09                        by: ricardo.czekster@pucrs.br       //
//====================================================================================//
#include <stdlib.h>
#include <iostream>             // std i/o with streams (cin, cout, cerr)
#include <fstream>              // file streams (i/o)
#include <iomanip>              // i/o manipulators (setiosflags-scientific)
#include <string.h>               // string manipulation (strcpy, strcat)
#include <math.h>               // absolute values (abs), square roots (sqrt)
#include <limits.h>             // maximum and minimum values to int, float, etc
#include <new>                  // handler for new commands (test for allocation)
#include <stdio.h>              // i/o standard of C language (used for the parser)
#include <assert.h>

#if defined(WIN32)
  #include <windows.h>
  #define dlopen(a,b) LoadLibrary(a)
  #define find_dlsym(a,b,c) (a = (Funcadd*)GetProcAddress(b,c))
  #define dlsym(handle, func) GetProcAddress ((HMODULE)handle, func)
  #define dlclose(x) FreeLibrary((HMODULE)x)
  #define NO_DLERROR
  #define reg_file(x) 1
  #ifdef _WIN32_WCE
    #define dlsym(handle, func) GetProcAddress ((HMODULE)handle, TEXT (func))
  #else
    #define dlsym(handle, func) GetProcAddress ((HMODULE)handle, func)
  #endif
#else
  #include <dlfcn.h>              // handler for dinamic libraries (used for 'jit')
#endif

using namespace std;

//#define _PEPS_DEBUG_

// For the sparse implementation of the probability vector
#include <vector>               // vector probability storage in
#include <algorithm>               // for sort
#include <map>                 // algorithms to manipulate maps

//====================================================================================//
//   Release Date      ******************** CHANGE THIS DATE FOR NEW RELEASES OF PEPS //
//====================================================================================//

#define __RELEASE_DATE__   "Jan 15 2015"

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
       jit_file ,   // 'just in time' files (files created during PEPS running time
       obj_file ,   // 'object' (compiled jit's) files
       so__file ,   // 'shared object' (dinamic link libraries) files
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

class GRAMMAR;
class DOM;
class EDIC;
class NDIC;
class FUNC;
class AUTOMATON;
class STATE;

//end compiler peps 2007
class GRAMMAR_;
class DOM_;
class EDIC_;
class NDIC_;
class FUNC_;
class AUTOMATON_;
class STATE_;
//end compiler peps 2007

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
typedef pair<int, rp> gs_pair;            // a global state and its probability 
                                          // for the sparse implementation of vectors
typedef pair<int, int> int_pair;          // a pair of two integers
typedef pair<int, int_pair> info_pair;     // a pair of an integer and an int_pair
typedef pair<int, gs_pair> info_gspair;    // a pair of an integer and a gs_pair

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
const   int         mem_unit = 16384;           // unit of memory space (Kbytes)
const   int         max_stack_size = 20;       // maximum size of the evaluation stack
                                               //    (for functional elements)

//====================================================================================//
//   Peps Name Definitions                                                            //
//====================================================================================//

const int max_peps_name = 128;          // Maximum size (actually the last 4 are ".ext")

typedef char peps_name[max_peps_name];  // User Definided Names for any Peps Entity

const int max_peps_expr = 32768;         // Maximum size for expressions

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

enum md_type    { Discrete , Continuous };        // Discrete and Continuous time models

enum p_m        { Mat_plus , Mat_minus };               // Synchronizing events matrices

enum mat_typ    { Sparse , Binary , Identity , Empty };            // san matrices types

enum elem_type  { Rate , Func };                                 // types of san element

enum l_r        { Left , Right };                            // left and right subspaces

enum hbf_format { C_format , F_format };       // formats for 'c' or 'fortran' languages

/*
enum oper       { Plus , Minus , Times , Slash ,           // function tokens operations
                  Mini , Maxi  ,
                  Eq   , Neq   , Gret  , Less , Geq  , Leq   ,
                  And  , Or    , Xor   , Imp  , Dimp , Not   ,
                  St   , Rw    , Cs_st , S_rw , P_rw , Cs_rw , Cp_rw ,
                  Cnst , No_oper };
*/
enum oper       { Plus , Minus , Times , Slash ,           // function tokens operations
                  Mini , Maxi  , Mod   , Div  ,
                  Eq   , Neq   , Gret  , Less , Geq  , Leq   ,
                  And  , Or    , Xor   , Imp  , Dimp , Not   ,
                  Aut  , Sts   , Std   ,
                  St   , Rw    , Cs_st , S_rw , P_rw , Cs_rw , Cp_rw ,
                  Cnst , No_oper };

enum comp_errors { err_7001 ,     // no space between signs
                   err_7002 ,     // incomplete "min" or "max" sign
                   err_7003 ,     // incomplete "not-equal" sign
                   err_7004 ,     // incomplete "and" sign
                   err_7005 ,     // incomplete "summation" definition
                   err_7006 ,     // incomplete "or" or "xor" sign
                   err_7007 ,     // incomplete "equal" sign
                   err_7008 ,     // incomplete automaton state or reward definition
                   err_7009 ,     // invalid function definition
                   err_7010 ,     // non-contiguous function identifiers
                   err_7011 ,     // incomplete function definition
                   err_7012 ,     // rows too short (misplaced ";")
                   err_7013 ,     // rows too long (misplaced ";")
                   err_7014 ,     // rows missing (misplaced "]")
                   err_7015 ,     // too many rows (misplaced "]")
                   err_7016 ,     // invalid number of automata
                   err_7017 ,     // invalid automaton size
                   err_7018 ,     // bad function definition
                   err_7019 ,     // missing functions
                   err_7020 ,     // unexpected character or end of file
                   err_7021 ,     // invalid function domain
                   err_7022 ,     // missing ":" in rewards definition
                   err_7023 ,     // incomplete double implication sign
                   err_7024 ,     // 
                   err_0000 };    // no error found!


//============================================================================//
//   Token of function trees                                                  //
//============================================================================//
struct token { oper op; double val; int beg; int end; };

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
//  Choice of transient initial vector:                                               //
//        - 'equi_vec'       equiprobable vector          							//
//        - 'global_vec'       global state vector        							//
//        - 'local_vec'       local state vector          							//
//        - 'uset_vec'       vector defined by an input file                          //
//------------------------------------------------------------------------------------//
enum v_tran  { equi_vec,  global_vec, local_vec, user_vec };
//====================================================================================//
//  Choice of vector implementation                                                   //
//        - 'v_full' : full implementation of the probability vectors                 //
//        - 'v_sparse' : sparse implementation of the probability vectors             //
//------------------------------------------------------------------------------------//
enum v_impl  { v_full  , v_sparse };
//====================================================================================//
//  Choice of zin implementation (for vector/matrix multiplication                    //
//        - 'zin_full' : full implementation                                          //
//        - 'zin_sparse' : sparse implementation                                      //
//------------------------------------------------------------------------------------//
enum zin_impl  { zin_full  , zin_sparse};
//====================================================================================//
//  Vector-Descriptor multiplication method:                                          //
//        - 'meth_A' do not change the automata order            (avoid permutations) //
//        - 'meth_B' change the order only of the product terms      (minimize perm.) //
//        - 'meth_C' change the order of the normal factors and product terms         //
//                                                             (minimize evaluations) //
//        - 'meth_S' uses different methods to each pr. term using a split approach   //
//        - 'meth_M' uses different methods to each product term      (minimize both) //
//------------------------------------------------------------------------------------//
enum m_typ  { meth_A  , meth_B , meth_C , meth_S, meth_M };
//====================================================================================//
//   Standard Solution Options                                                        //
//         - 'Power'         power method                                             //
//         - 'Arnoldi'       flexibe arnoldi method                                   //
//         - 'Chebyshev'     polynomial chebyshev                                     //
//         - 'Gmres'         flexibe gmres method                                     //
//------------------------------------------------------------------------------------//
enum solution_typ { Power , Arnoldi , Gmres , Chebyshev , Uniformization};
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
//===========================================================================================================//
//  Strategy to 'cut' the tensor terms in the model (used in the Split method:                               //
//        - 'strat_A' identities to shuffle (only constant models)                                           //
//        - 'strat_B' functional evaluations in shuffle  (only functional models)                            //
//        - 'strat_C' functional evaluations in split  (only functional models)                              //
//        - 'strat_D' functional evaluations in split, only identities to shuffle  (only functional models)  //
//        - 'strat_E' only functional definition and identities in shuffle  (only functional models)         //
//-----------------------------------------------------------------------------------------------------------//
enum strat_typ  { strat_A  , strat_B , strat_C , strat_D , strat_E };

//====================================================================================//
//   User Preferences Defaults                                                        //
//====================================================================================//

const bool      def_verbose   = true;          // Verbose mode
const int       def_iter      = 1000;          // Maximum iteration number
const i_typ     def_i_typ     = cnv_it;        // Iteration type
const rp        def_err       = 1e-10;         // Minimum error
const e_typ     def_e_typ     = abs_ind_err;   // Error verification type
const rp        def_thresh    = close_to_zero; // Threshold for LU decomposition
const f_aprox   def_f_aprox   = lu_ave;        // LU funct. aproximation type
const lu_typ    def_lu_typ    = std_lu;        // LU decomposition type
const sol_typ   def_sol_typ   = both_lu;       // LU solution type
const v_typ     def_v_typ     = eq_vec;        // Initial vector type
const v_impl    def_v_impl    = v_full  ;      // Full vector implementation
const zin_impl  def_zin_impl = zin_sparse ;    // Sparse zin implementation
const m_typ     def_m_typ     = meth_A;        // Vector-Descriptor Multiplication method
const int       def_krylov    = 10;            // Krylov space size to GMRES
const int       def_poly      = 5;             // Size of the polynomial aproximation
const bool      def_partial   = false;         // Partial Reachability Function (from .san)
const strat_typ def_strat     = strat_A;       // Default Strategy to 'cut' the tensor terms

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

//PEPS 2007 COMPILER DIRECTIVES
#define MAX_ID_NAME 128
#define MAX_AUT_NAME 128
#define MAX_STT_NAME 128
#define MAX_EXP_SIZE 32768
/*
#define DOMAIN 1
#define CONSTANT 3
#define AUT_INTEGER 6
#define AUT_CONSTANT 7

*/
#define INTEGER 4
#define AUT_DOMAIN 8
#define EMPTY 0
#define INTERVAL 2
#define AUT 5

#define CONTINUOUS 0
#define DISCRETE 1

#define SYN 0
#define LOC 1

#define STT 0
#define FROM 1

#include "tk_defs_.h"
#include "grammar_.h"
#include "dom_.h"
#include "edic_.h"
#include "ndic_.h"
#include "func_.h"
#include "autom_.h"
#include "state_.h"
#include "error_msgs_.h"

//end of PEPS 2007 COMPILER DIRECTIVES

//====================================================================================//
//   Includes Peps Packages                                               Error Codes //
//====================================================================================//

#include "solve.h"     // solution methods (solve.C)                      5600
#include "interf.h"    // user interface package for UNIX version         6200
#include "compiler.h"  // compiler package (compiler.C)                   6300
#include "timer.h"     // timer definitions package (timer.C from pac++)

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

#include "aunf.h"        // definitions for AUNFs (Split)

#endif
