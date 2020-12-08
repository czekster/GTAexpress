//====================================================================================//
//                                                                                    //
//                            Class Probability Vector                                //
//                                                                                    //
//====================================================================================//
//  This File:   vct.h                      Language: C++  (xlC and CC)               //
//  Software:    PEPS 3.1                    Advisor: Brigitte.Plateau@imag.fr        //
//  Error Codes: 2300, 2400                                                           //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 14/jan/02                        by: Anne.Benoit@imag.fr             //
//====================================================================================//
//     This class is used to define probability vectors in general. It defines also   //
// the PEPS main structure probability vector (VCT::vct).                             //
//                                                                                    //
//     The variable 'dimension' stores the total number of states of the vector. The  //
// vector elements can be stored either in the array field 'vec', or with a sparse    //
// implementation.                                                                    //
// The boolean is_sparse says if we use the sparse implementation or not.             //
// They represent the probability of                                                  //
// each global state. These global states are sorted in a lexical order. The automata //
// list object 'permutation' defines the order of the "letters" in the "word" of this //
// lexical order (see example below). The automata sizes object 'sizes' stores the    //
// size of each automaton.                                                            //
//                                                                                    //
// Note: The permutation list must include all automata of the model.                 //
//                                                                                    //
// Note: The total number of states in the vector must be equal to the product of the //
// sizes of the automata ('dimension' == 'sizes.ss()').                               //
//                                                                                    //
// Example: to this two different 'permutation' lists, the vector elements will       //
// correspond to the following global states ('sizes' = { 2, 3, 4 }):                 //
//                                                                                    //
// +--------------------------------------+  +--------------------------------------+ //
// |     'permutation' = { 0, 1, 2 }      |  |     'permutation' = { 2, 0, 1 }      | //
// +----+-------+----+-------+----+-------+  +----+-------+----+-------+----+-------+ //
// |  0 | 0 0 0 |  8 | 0 2 0 | 16 | 1 1 0 |  |  0 | 0 0 0 |  8 | 0 2 1 | 16 | 1 1 2 | //
// |  1 | 0 0 1 |  9 | 0 2 1 | 17 | 1 1 1 |  |  1 | 0 1 0 |  9 | 1 0 1 | 17 | 1 2 2 | //
// |  2 | 0 0 2 | 10 | 0 2 2 | 18 | 1 1 2 |  |  2 | 0 2 0 | 10 | 1 1 1 | 18 | 0 0 3 | //
// |  3 | 0 0 3 | 11 | 0 2 3 | 19 | 1 1 3 |  |  3 | 1 0 0 | 11 | 1 2 1 | 19 | 0 1 3 | //
// |  4 | 0 1 0 | 12 | 1 0 0 | 20 | 1 2 0 |  |  4 | 1 1 0 | 12 | 0 0 2 | 20 | 0 2 3 | //
// |  5 | 0 1 1 | 13 | 1 0 1 | 21 | 1 2 1 |  |  5 | 1 2 0 | 13 | 0 1 2 | 21 | 1 0 3 | //
// |  6 | 0 1 2 | 14 | 1 0 2 | 22 | 1 2 2 |  |  6 | 0 0 1 | 14 | 0 2 2 | 22 | 1 1 3 | //
// |  7 | 0 1 3 | 15 | 1 0 3 | 23 | 1 2 3 |  |  7 | 0 1 1 | 15 | 1 0 2 | 23 | 1 2 3 | //
// +----+-------+----+-------+----+-------+  +----+-------+----+-------+----+-------+ //
//====================================================================================//
#ifndef __vct_h__
#define __vct_h__

#include "aunf.h"

class VCT
{
public: 
 inline VCT();                                                      // Empty Constructor
 inline VCT(const int dim, const int rdim);                         // Sized Constructor
 inline VCT(const VCT & v);                                          // Copy Constructor
 inline ~VCT();                                                            // Destructor

 inline void create();                                            // Empty Intialization
 inline void create(const int dim, const int rdim);               // Sized Intialization
 inline void copy(const VCT & v);                        // Copy and Sized Intialization
 inline void erase();                                                     // Destruction

 inline void baptise(const peps_name file);                    // It assigns a file name
 inline void name(peps_name file) const;                     // It returns the file name
 inline int  size() const;                        // The total number of vector elements
 inline int  real_size() const;                            // The total number of states
 inline int  aut() const;                         // The number of automata of the model
 inline bool is_empty() const;                     // The total number of states is zero
 inline bool is_not_permuted() const;     // The vector is in the original lexical order

 // Not for sparse implementation
 inline rp   get(const int s) const;                 // The probability of the state 's'
                                    // 's' is the index of the probability in the vector
 inline rp   get_pos(const int s) const;             // The probability of the state 's'
        // 's' is the position of the probability (it's not the same for sparse vectors)
 inline void put(const int s, const rp v);   // It puts probability 'v' to the state 's'

 inline void put_sizes(const aut_st & s);           // It sets the sizes of the automata
 inline void get_sizes(aut_st & s) const; // It fills 's' with the sizes of the automata
 inline void put_permut(const aut_list & l);  // It sets the permutation order of 'this'
 inline void get_permut(aut_list & l) const;
                                    // It fills 'l' with the permutation order of 'this'
 inline void get_CND();       // It sets the sizes of the automata and permutation order
                               // of 'this' according to the static structure 'CND::cnd'

        //----------------------------------------------------------------------------//
        //                      Basic Linear Algebra Functions                        //
        //----------------------------------------------------------------------------//
        //    The following nine functions perform some linear algebra functions to   //
        // the probability vectors. These functions do not look to the current        //
        // permutations of the vectors. Only the vector elements are concerned by     //
        // the following functions.                                                   //
        //----------------------------------------------------------------------------//
 inline void normalize();        // It normalizes the vector according to the first norm
 inline void scale(const rp f);   // It multiplies all vector elements by the factor 'f'
 inline void scale_copy(const VCT & v, const rp f);        // It performs 'this = v * f'
 inline void scale_add(const VCT & v, const rp f);        // It performs 'this += v * f'
 inline void add(const VCT & v);                              // It performs 'this += v'

 inline rp   norm1() const;            // It returns the sum of the elements (in modulo)
 inline rp   norm2() const;  // It returns the square root of sum of the elements square
 inline rp   norm_inf() const;             // It returns the biggest element (in modulo)

 inline rp   dot(const VCT & v) const;   // It returns the dot product of 'this' and 'v'
 inline rp   sum() const;               // It returns the sum of the elements (absolute)

        //----------------------------------------------------------------------------//
        //                             Test Functions                                 //
        //----------------------------------------------------------------------------//
 inline int  elem_under(const rp th) const;
              // It returns the number of elements under a threshold (smaller than 'th')
 inline bool verify() const;            // It verifies if the sum of the elements is 1.0
 inline rp   compare(const VCT & v, const e_typ err) const;
                            // It returns the error between two vectors ('this' and 'v')
 inline bool test(const VCT & v, const e_typ err, const rp e) const;
               // It returns 'true' if the error between two vectors is smaller than 'e'
 inline rp test_hbf(const VCT & v, const e_typ err) const;
               // It returns the error between two vectors
 inline void info(rp & acc, rp & big, int & bpos, rp & sml, int & spos) const;
      // It returns the vector sum, the largest and smallest elements and their position

 inline void debug();
 // It prints the values of the vector elements

 inline void change_positions(const VCT & v);


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
        void init(const v_typ o);

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
        void init_hbf(const v_typ o);

        //--------------------------------------------------------------------------//
        //       It permutes the vector elements acording to a lexical order        //
        //--------------------------------------------------------------------------//
        //   The vector elements are sorted according to a lexical order expressed  //
        // by the list of automata 'permutation'. This function changes from the    //
        // current order (expressed in the variables 'p_order' and 'p_jumps') to a  //
        // new order (expressed by the input parameters 's' and 'j'). The vector    //
        // elements are permuted acording to this new list.                         //
        //--------------------------------------------------------------------------//
        void permut(const aut_list & s, const jumps & j);

        //--------------------------------------------------------------------------//
        //          It copies one vector elements to another (permuting)            //
        //--------------------------------------------------------------------------//
        void becomes(const VCT & v, const aut_list & s, const jumps & j);

        //--------------------------------------------------------------------------//
        //           It adds one vector elements to another (permuting)             //
        //--------------------------------------------------------------------------//
        void operator += (const VCT & v);

        //--------------------------------------------------------------------------//
        //          It adds one vector elements to another (normalizing)            //
        //--------------------------------------------------------------------------//
        void norm_sum(const VCT & v);           // version being used
        void norm2_sum(const VCT & v);          // alternative version

        void write() const;                              // Output on 'fout' file stream
        void read(const peps_name file);                 // Input from 'fin' file stream
        int  mem_use() const;                  // Memory space used by 'this' (in bytes)

        //--------------------------------------------------------------------------//
        //                             Static variable                              //
        //--------------------------------------------------------------------------//
        static VCT vct;

        //--------------------------------------------------------------------------//
        //                        Static Variables Functions                        //
        //--------------------------------------------------------------------------//
        void alloc_static();      // It allocates and desallocates the internal static
        void desalloc_static();   // variables 't', 'z_in', 'z_out' and 'g_st' according
                                  // to the current values of the 'CND::cnd' structure

        //--------------------------------------------------------------------------//
        //                             Main Functions                               //
        //--------------------------------------------------------------------------//
        // Basic multiplications                                                    //
        //--------------------------------------------------------------------------//
        void mult_HBF(const VCT & v);    // Multiply by a HBF

        void mult_CND(const VCT & v);    // Multiply by a CND (switch to)
        void mult_CND_A(const VCT & v);  //       A Method
        void mult_CND_B(const VCT & v);  //       B Method
        void mult_CND_C(const VCT & v);  //       C Method
        void mult_CND_M(const VCT & v);  //       M Method
        void mult_CND_S(const VCT & v);  //       S Method (split)

        //--------------------------------------------------------------------------//
        // Basic solutions (multiplications by inverses)                            //
        //--------------------------------------------------------------------------//
        void solv_LUD();                 // Multiply 'this' by the inverse of a LUD term
        void solv_LUD(const VCT & v);    // Multiply 'v' by the inverse of a LUD term
        void solv_LUD_P(const VCT & v);  // Multiply 'v' by the inverse of a LUD term
                                         // (it accumulates the result with 'this')
        void solv_D();                   // Multiply by the inverse of the CND diagonal

        //--------------------------------------------------------------------------//
        // Preconditionned Multiplications (multiplications + solutions)            //
        //--------------------------------------------------------------------------//
                                         //       Precontionning Matrix Used        //
                                         //-----------------------------------------//
        void pre_altn(const VCT & v);    // one of the 'LUD::lud' terms
        void pre_adtv(const VCT & v);    // the aditive combination of 'LUD::lud' terms
        void pre_mult(const VCT & v);    // the multip. combination of 'LUD::lud' terms
        void pre_diag(const VCT & v);    // the diagonal of the descriptor 'CND::cnd'
        void pre_plyn(const VCT & v);    // the polynomial aprox. of the 'CND::cnd'
        void pre_ipln(const VCT & v);    // the polynomial aprox. of I - 'CND::cnd'
        void pre_cheb(const VCT & v);    // the chebyshev polynomial aprox.

        //--------------------------------------------------------------------------//
        // The next methods are used in the Split approach                          //
        //--------------------------------------------------------------------------//
        void setup_aunfs();
        void create_aunfs(const int e, const int cut);
        void split_const(san_mat & m, const int e, const aut_id a, const int cut, const aut_st & vsizes);
        void split_func(san_mat & m, const aut_st & vsizes, const int e, const aut_id a, const int cut, const st_map * sm, const aut_st & pgs);
        rp get_scalar(const int e, aut_st & gs, int *basein, int *baseout, aut_st *gs_aux, const int cut, jumps j);
        void print_aunfs();

        //--------------------------------------------------------------------------//
        //                          Data Structures                                 //
        //--------------------------------------------------------------------------//
private: peps_name file_name;          // the file name of this structure
         int       dimension;          // the total number of vector elements
         int       real_dim;           // the total number of states
         rp      * vec;                // vector elements
         int     * positions;          // positions of the states of RSS in the state
	                                   // space (used in sparse implementation)
    	 bool      is_sparse;          // says if we use the sparse implement. or not
         aut_list  p_order;            // the current permutation order of the vector
         jumps     p_jumps;            // the current permutation jumps of the vector
         aut_st    sizes;              // the size of each automaton

      //--------------------------------------------------------------------------//
      //                 Internal Functions of Multiply by a CND                  //
      //                                                                          //
      //   These seven routines multiply the vector 'v' by a normal factor which  //
      // the non identity matrix 'm' in the position (if necessary the state map  //
      // of the matrix 'm' is passed by the parameter 'sm').                      //
      //                                                                          //
      //   The particular characteristics of the functions are:                   //
      //                                                                          //
      //   'mcp'  - the normal factor is constant and the result is added to      //
      //            'this' (used by all methods);                                 //
      //   'mfip' - the normal factor is functional, the function evaluation is   //
      //            performed inside the inner most loop and the result is added  //
      //            to 'this' (used on method A);                                 //
      //   'mcs'  - the normal factor is constant and the result is stored in     //
      //            'this' (used by all methods);                                 //
      //   'mfis' - the normal factor is functional, the function evaluation is   //
      //            performed inside the inner most loop and the result is stored //
      //            in 'this' (used on method A);                                 //
      //   'mfos' - the normal factor is functional, the function evaluation is   //
      //            performed outside the inner most loop and the result is       //
      //            stored in 'this' (used on method B);                          //
      //   'mnps' - the normal factor is functional, the permutation of the input //
      //            vector is performed before this function is called, the       //
      //            function evaluation is performed outside the inner most loop  //
      //            and the result is stored in 'this' (used on method C);        //
      //                                                                          //
      // Obs.: The necessary information concerning the jumps and the orders of   //
      // the normal factor is taken from the internal structures of vector 'v',   //
      // namelly 'p_order', 'p_jumps' and 'sizes'. The only exception is the      //
      // function 'mnps', which receives two integers 'nleft' and 'nright' that   //
      // means the peridiciodity of function evaluations.                         //
      //                                                                          //
      // The boolean b in mfos and mnps says (for the sparse vector implement.)   //
      // if we must look for the non nul results (false) or if we already know    //
      // there positions (true)                                                   //
      //--------------------------------------------------------------------------//
      void mcp( san_mat & m,                    const VCT & v, const aut_id a);
      void mfip(san_mat & m, const st_map * sm, const VCT & v, const aut_id a);
      void mcs( san_mat & m,                    const VCT & v, const aut_id a);
      void mfis(san_mat & m, const st_map * sm, const VCT & v, const aut_id a);
      void mfos(san_mat & m, const st_map * sm, const VCT & v, const aut_id a,
		const bool b);
      void mnps(san_mat & m, const st_map * sm, const VCT & v, const aut_id a,
                const int nleft, const int nright, const aut_id pos, const bool b);

      //--------------------------------------------------------------------------//
      //              Internal Functions of Multiply by a LUD term                //
      //                                                                          //
      //   These functions compute the multiplication of a vector ('this') by a   //
      // normal factor of the 'term of the turn' which the non identity matrix is //
      // passed on the input paramter 'm'.                                        //
      //                                                                          //
      //   This functions has three versions, the first and the second versions   //
      // use assign the result to 'this', the third function adds the result to   //
      // the previous contents of 'this'. The first and the third versions use    //
      // 'this' as input. The second uses 'v' as input.                           //
      //--------------------------------------------------------------------------//
      void solv(const sp_mat & m, const sp_mat & m2,
                const int ni, const int nleft, const int nright, const int njump);
      void solv(const VCT v, const sp_mat & m, const sp_mat & m2, const int ni,
                const int nleft, const int nright, const int njump);
      void solv_P(const sp_mat & m, const sp_mat & m2, const VCT & v,
                  const int ni, const int nleft, const int nright, const int njump);

      //--------------------------------------------------------------------------//
      //      Static variables to the Internal Functions of Multiply by ...       //
      //                                                                          //
      //   The following variables are defined as static to reduce the cost       //
      // for allocation and release memory structures in the most sensible points //
      // of the PEPS solvers. All structures allocated and desallocated by the    //
      // 'alloc_static' and 'desalloc_static' previously defined.                 //
      //--------------------------------------------------------------------------//
static VCT    t;             // temporary vector to store the intermediary results
static rp   * z_in;          // input "slice" (a pointer to an array of vectors elements)
static rp   * v_in;          // input "slice" (a pointer to an array of vectors elements) for split
static rp   * z_out;         // output "slice" (a pointer to an array of vectors elements)
static aut_st g_st;          // a global state according to the current descriptor size
//static vector<vaunf> aunfs;  // the list of lists of aunfs (one list for each event e)
static aunfs_list *aunfs;
//static struct aunfs_ *aunfs;

static vector<gs_pair> * zins;       // array of input "slice" in sparse format
static vector<gs_pair> zin;          // input "slice" in sparse format
static vector<gs_pair> zouts;        // output "slice" in sparse format
//static int_pair * info_zin;          // some informations about the zin
//static int_pair * info_zout;         // some informations about the zout
static vector<info_pair> info2_zin;          // some informations about the zin
static vector<info_pair> info2_zout;         // some informations about the zout

static bool * used_z   ;             // the used zin or zout
static rp ** zinf;
static int * indice_r;
static int ** useful_f;
static int ** place;



      //--------------------------------------------------------------------------//
      //         Internal class 'comp' used to compare two gs_pairs               //
      //--------------------------------------------------------------------------//
 class comp{
   public :
     bool operator() (gs_pair x, gs_pair y) {
        return (x.first < y.first);
     }
 };  

 class comp2{
   public :
     bool operator() (info_pair x, info_pair y) {
        return (x.first < y.first);
     }
 };  




};

#endif
