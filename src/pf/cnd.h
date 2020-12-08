//====================================================================================//
//                                                                                    //
//                   Continuous-Time Normalized Descriptor Structure                  //
//                                                                                    //
//====================================================================================//
//  This File:   cnd.h                      Language: C++  (xlC and CC)               //
//  Software:    PEPS 3.1                    Advisor: Brigitte.Plateau@imag.fr        //
//  Error Codes: 5400                                                                 //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 24/feb/02                        by: Paulo.Fernandes@imag.fr         //
//====================================================================================//
//     This structure is used to store  the PEPS main structure "continuous-time      //
// normalized descriptor". The static variable 'CND::cnd' contains the current SAN    //
// "continuous-time normalized descriptor.                                            //
//                                                                                    //
//     This structure is obtained by the normalization of a descriptor (object 'DSC'  //
// see file 'dsc.h'). This normalization transforms the matrices of the descriptor    //
// scaling one matrix of each tensor product. The normalization factor used is the    //
// largest element (in modulo) of the generator corresponding to the non-normalized   //
// descriptor ('DSC::dsc').                                                           //
//                                                                                    //
//     The normalized descriptor structure also contains special structures to avoid  //
// waste of time during the vector descriptor multiplication (see class 'VCT'         //
// multiply procedures).                                                              //
//                                                                                    //
//     The basic structures of the normalized descriptor are:                         //
//                                                                                    //
//       - the number of automata (variable 'automata');                              //
//       - the number of synchronizing events (variable 'events');                    //
//       - the state space size (variable 'state_space');                             //
//       - the size of each automata ('aut_st' object 'sizes');                       //
//       - the local matrices (array 'local_mat');                                    //
//       - the + matrices of the synchronizing events (array 'synch_mat');            //
//       - the diagonal elements of the corresponding matrix (array 'diagonal');      //
//       - the state maps (see 'st_map' class) of each automaton (array 'state_map'); //
//       - the labels to identify replicated automata ('aut_st' object 'symetric');   //
//                                                                                    //
//     Note that since the diagonal elements are numerically stored in the array      //
// 'diagonal', the negative matrices of the synchronizing events do not need to be    //
// kept. For the same reason the diagonal elements of the local matrices are removed. //
//                                                                                    //
//     The structures to improve the vector descriptor multiplication are:            //
//                                                                                    //
//      - the permutation jumps for the automata taken in their original order, i.e., //
//  first automaton 0, then automanaton 1, etc. ('jumps' object - see class 'jumps' - //
//  'orig_jumps');                                                                    //
//                                                                                    //
//      - the information used to choose automatically a multiplication method (see   //
//  file 'peps.h'):                                                                   //
//         - the tensor product term is an identity, empty, constant or suitable for  //
//  the functional methods A, B or C (arrays 'l_type' and 's_type', repectively for   //
//  local and synchronizing terms);                                                   //
//         - the normal factor is an identity, empty, constant or functional (arrays  //
//  'lf_type' and 'sf_type', repectively for local and synchronizing normal factors); //
//                                                                                    //
//      - the information needed to use multiplication method B:                      //
//         - to each tensor product term, the order to handle the matrices and the    //
// corresponding permutation jumps ('l_B_order' and 'l_B_jumps' to local terms and    //
// 's_B_order' and 's_B_jumps' to synchronizing terms);                               //
//                                                                                    //
//      - the information needed to use multiplication method C:                      //
//         - to each normal factor, the order to handle the matrices and the          //
// corresponding permutation jumps ('l_C_order' and 'l_C_jumps' to local terms and    //
// 's_C_order' and 's_C_jumps' to synchronizing terms);                               //
//         - to each normal factor, the state space for the automata that are the     //
// parameters of the normal factor, the state space for the automata that are not     //
// parameters of the normal factor, the position of the first automaton that is not   //
// parameters of the normal factor ('l_C_left', 'l_C_right' and 'l_C_fst' for local   //
// normal factors and 's_C_left', 's_C_right' and 's_C_fst' for synchronizing normal  //
// factors);                                                                          //
//                                                                                    //
//      - information of the correct decomposition order of tensor products of the    //
// synchronized events:                                                               //
//         - to each synchronized event it is the order the normal factors should be  //
// multiplied to respect the decomposition in normal factors property ('s_D_order');  //
//                                                                                    //
//    Tensor product terms that are constant or where the method A is employed do not //
// need special information. Those cases use the original permutation jumps (object   //
// 'orig_jumps').                                                                     //
// Obs.: the normalizing factor used to generate the normalized descriptor is stored  //
// in the variable 'factor'. This information is not used latter, but it can be very  //
// usefull in debuging.                                                               //
//====================================================================================//

#ifndef __CND_H__
#define __CND_H__

class CND
{
public: CND();                                                      // Empty Constructor
        CND(const aut_id a, const ev_id e, const int s,
	    const int rs);                                          // Sized Constructor
        CND(const CND & d);                                          // Copy Constructor
        ~CND();                                                            // Destructor

 inline void create();                                            // Empty Intialization
 inline void create(const aut_id a, const ev_id e, const int s,
                    const int rs);                                // Sized Intialization
 inline void copy(const CND & d);                        // Copy and Sized Intialization
 inline void erase();                                                     // Destruction

 inline void    baptise(const peps_name file);                 // It assigns a file name
 inline void    o_baptise(const peps_name file);   // It assigns the original file name
 inline void    name(peps_name file) const;                  // It returns the file name
 inline void    o_name(peps_name file) const;      // It returns the original file name
 inline int     size() const;                            // The total number of automata
 inline aut_id  aut() const;                             // The total number of automata
 inline ev_id   ev() const;                 // The total number of synchronizing events
 inline int     ss() const;                              // The product state space size
 inline int     rs() const;                            // The reachable state space size
 inline bool    is_empty() const;                                 // There is no automata
 inline rp      n_factor() const;                              // The normalizing factor
 inline int     largest_aut() const;                        // The largest automata size
 inline int     smallest_aut() const;                      // The smallest automata size
 inline rp      cost() const;                        // The solution computational cost
 inline rp      l_cost() const;  // The solution computational cost to local matrices
 inline rp      s_cost() const;  // The solution computational cost to synch matrices
 inline rp      acost() const;                       // The solution computational cost
 inline rp      l_acost() const; // The solution computational cost to local matrices
 inline rp      s_acost() const; // The solution computational cost to synch matrices

 inline void add_identity();                     // It adds one to each diagonal element
 inline void minus_identity();            // It subtracts one from each diagonal element

        //--------------------------------------------------------------------------//
        //                           Access Functions                               //
        //--------------------------------------------------------------------------//
        //    The following member functions are used to access the internal        //
        // structures of the descriptor. The functions 'put' store the information  //
        // in the input parameter into the respective descriptor internal structure.//
        // The functions 'get' fill the output parameter with the respective        //
        // descriptor internal structure.                                           //
        //--------------------------------------------------------------------------//
        //                        The size of all automata                          //
        //--------------------------------------------------------------------------//
 inline void put_s(const aut_st & s);             
 inline void get_s(aut_st & s) const;
        //--------------------------------------------------------------------------//
        //                           The original jumps                             //
        //--------------------------------------------------------------------------//
 inline void get_oj(jumps & j) const;
        //--------------------------------------------------------------------------//
        //                       The 'p'-th diagonal element                        //
        //--------------------------------------------------------------------------//
 inline rp get_diag(const int p);

        //--------------------------------------------------------------------------//
        //                       Returns the l_type (local matrices)                //
        //--------------------------------------------------------------------------//
 inline term_typ get_l_type(const int p);
        //--------------------------------------------------------------------------//
        //                       Returns the s_type (synch matrices)                //
        //--------------------------------------------------------------------------//
 inline term_typ get_s_type(const int p);
        //--------------------------------------------------------------------------//
        //                      Returns the sf_type (synch matrices) for the factor //
        //--------------------------------------------------------------------------//
 inline fact_typ get_sf_type(const int e, const int i);
        //--------------------------------------------------------------------------//
        //     Returns the sanmat appointed by the event i and the position i       //
        //--------------------------------------------------------------------------//
 inline san_mat get_synch_mat(const int e, const int i);
        //--------------------------------------------------------------------------//
        //     Returns the state map                                                //
        //--------------------------------------------------------------------------//
 inline st_map *get_state_map();
        //--------------------------------------------------------------------------//
        //     Returns the s_D_order                                                //
        //--------------------------------------------------------------------------//
 inline aut_list get_s_D_order(const int i);
        //--------------------------------------------------------------------------//
        //     Returns the s_C_order                                                //
        //--------------------------------------------------------------------------//
 inline aut_list get_s_C_order(const int e, const int a);
        //--------------------------------------------------------------------------//
        //     Returns the s_B_order                                                //
        //--------------------------------------------------------------------------//
 inline aut_list get_s_B_order(const int e);
        //--------------------------------------------------------------------------//
        //     Sets the cut value                                                   //
        //--------------------------------------------------------------------------//
 inline void set_cut(const int i, const int cut);
        //--------------------------------------------------------------------------//
        //     Returns the cut value                                                //
        //--------------------------------------------------------------------------//
 inline int get_cut(const int i);
        //--------------------------------------------------------------------------//
        //     Returns the functional cut value                                     //
        //--------------------------------------------------------------------------//
 inline int get_fcut(const int i);

        //--------------------------------------------------------------------------//
        //                    Evaluation of a transition rate                       //
        //--------------------------------------------------------------------------//
        //    This function computes the numeric value of the transition rate       //
        // corresponding to a transition from the global state 'i' to the global    //
        // state 'j'. The coordinates 'i' and 'j' are vectors where each entry is   //
        // the state of a group.                                                    //
        //                                                                          //
        // Obs.: The input parameters 'i' and 'j' must be passed according to the   //
        // current descriptor automata. The size of the vectors 'i' and 'j' must be //
        // equal to the private variable 'automata' and the state maps are used to  //
        // obtain the global state according to the original number of automata.    //
        //--------------------------------------------------------------------------//
        rp coord(const aut_st & i, const aut_st & j);

        void write() const;                              // Output on 'fout' file stream
        void read(const peps_name file);                 // Input from 'fin' file stream
        void print() const;               // Output a readable version in 'cnd.dbg' file
        void dprint() const;              // Output diagonal elements in 'diag.dbg' file
        int  mem_use() const;                  // Memory space used by 'this' (in bytes)

        //--------------------------------------------------------------------------//
        //                            Friend Functions                              //
        //--------------------------------------------------------------------------//
        //                                                                          //
        //                Multiplication by a probability vector                    //
        //--------------------------------------------------------------------------//
        void friend VCT::mult_CND(const VCT & v);
        void friend VCT::mult_CND_A(const VCT & v);
        void friend VCT::mult_CND_B(const VCT & v);
        void friend VCT::mult_CND_C(const VCT & v);
        void friend VCT::mult_CND_M(const VCT & v);

        //--------------------------------------------------------------------------//
        //                    Friend Methods used in Split                          //
        //--------------------------------------------------------------------------//
        void friend VCT::mult_CND_S(const VCT & v);
        void friend VCT::split_const(san_mat & m, const int e, const aut_id a, const int cut, const aut_st & vsizes);
        void friend VCT::split_func(san_mat & m, const aut_st & vsizes, const int e, const aut_id a, const int cut, const st_map * sm, const aut_st & pgs);
        rp friend VCT::get_scalar(const int e, aut_st & gs, int *basein, int *baseout, aut_st *gs_aux, const int cut, jumps j);
        void friend VCT::create_aunfs(const int e, const int cut);

        //--------------------------------------------------------------------------//
        //                            Friend Functions                              //
        //--------------------------------------------------------------------------//
        //                                                                          //
        //   Multiplication of the inverse of the diagonal by a probability vector  //
        //--------------------------------------------------------------------------//
        void friend VCT::solv_D();

        //--------------------------------------------------------------------------//
        //                             Main Functions                               //
        //--------------------------------------------------------------------------//
        //                                                                          //
        //                        Normalizes the Descriptor                         //
        //                                                                          //
        //    Only Input Parameters:                                                //
        //                                                                          //
        //    - 'name'     the file name of the new structures (that are saved      //
        //                    after grouping);                                      //
        //    - 'DSC::dsc' the descriptor to be normalized;                         //
        //                                                                          //
        //    Input and Output Parameters:                                          //
        //                                                                          //
        //    - 'FTB::ftb' before: the old function table (referred by 'DSC::dsc'); //
        //                 after:  the new function table (referred by 'this');     //
        //                                                                          //
        //    Only Output Parameters:                                               //
        //                                                                          //
        //    - 'this'     the normalized descriptor (usually 'CND::cnd');          //
        //--------------------------------------------------------------------------//
        void translate(const peps_name name);

        //--------------------------------------------------------------------------//
        //                          Static variable                                 //
        //--------------------------------------------------------------------------//
        static CND cnd;

        //--------------------------------------------------------------------------//
        //                      Methods used in Split                               //
        //--------------------------------------------------------------------------//
      void add_aunf(aunf *vaunfs, int size);     //increment the number of aunfs
	   int get_tot_aunfs();                       //return the total number of aunfs
      int get_mem_aunfs();                       //return the total memory of aunfs
		void set_aunf_time(const double val);  //increment the time to generate aunfs
	   double get_aunf_time();    //return the total time to generate the aunfs list
	   void reset_aunf();                                //reset the number of aunfs
      void reorganize_orders() const;         //reorganizes the s_B_order for split

        //--------------------------------------------------------------------------//
        //                          Data Structures                                 //
        //--------------------------------------------------------------------------//
private: peps_name   file_name;   // the file name of this structure
         peps_name   orig_name;   // the file name of the original structure

         aut_id      automata;    // the number of automata
         ev_id       events;      // the number of synchronizing events
         int         state_space; // the state space size
         int         reach_space; // the reachable state space size
		   int         tot_aunfs;   // the total number of AUNFs
		   int         mem_aunfs;   // the total number of AUNFs
         double      aunf_time;   // the time to generate the AUNFs

         rp          factor;      // the factor used to normalize the descriptor

         aut_st      sizes;       // the size of each automaton
         aut_st      symetric;    // array of automata labels (replications map)
         jumps       orig_jumps;  // the jumps of the automata in the original order
         st_map    * state_map;   // the correspond state map to each automaton

         term_typ  * l_type;      // the local terms types
         fact_typ  * lf_type;     // the local normal factors types
         aut_list  * l_B_order;   // the order to handle local normal factors (B method)
         jumps     * l_B_jumps;   // the permutation jumps to the 'l_B_order'
         aut_list  * l_C_order;   // the order to handle local normal factors (C method)
         jumps     * l_C_jumps;   // the permutation jumps to the 'l_C_order'
         int       * l_C_left;    // the state space for param. of local normal factors
         int       * l_C_right;   // the state space for non param. of local norm. fact.
         int       * l_C_fst;     // the position of the first non paramameter

         term_typ  * s_type;      // the synchronizing terms types
         aut_list  * s_B_order;   // the order to handle synchronizing terms (B method)
         jumps     * s_B_jumps;   // the permutation jumps to the 's_order'
         fact_typ ** sf_type;     // the synchronizing normal factors types
         aut_list ** s_C_order;   // the order to handle sync. normal factors (C method)
         jumps    ** s_C_jumps;   // the permutation jumps to the 's_C_order'
         int      ** s_C_left;    // the state space for param. of sync. normal factors
         int      ** s_C_right;   // the state space for non param. of sync. norm. fact.
         int      ** s_C_fst;     // the position of the first non parameter
         aut_list  * s_D_order;   // the order to decompose the normal factors
	                   // (this D order should be used to all methods: A, B and C)

         san_mat   * local_mat;   // the matrices of the local terms
         san_mat  ** synch_mat;   // the + matrices of the synchronizing terms
         //san_mat  ** synch_adj; // the - matrices of the synchronizing terms
         rp        * diagonal;    // the diagonal elements of the descriptor

         static int  * fcuts;     // functional cuts (dependencies)

      //--------------------------------------------------------------------------//
      //            Internal Functions of the Main Function Agregation            //
      //--------------------------------------------------------------------------//

      void copy_structures();
      //--------------------------------------------------------------------------//
      //        It copies the basic structures of the descriptor 'DSC::dsc'       //
      //                           WITHOUT ANY CHANGES                            //
      //--------------------------------------------------------------------------//

	  void generate_diagonalF();
      void generate_diagonalS();
      void generate_diagonal();
      //--------------------------------------------------------------------------//
      //  It generates all diagonal elements and computes the normalizing factor  //
      //--------------------------------------------------------------------------//

      void process_matrices();
      //--------------------------------------------------------------------------//
      //       It scales one matrix of each tensor product term and removes       //
      //               the diagonal elements of the local matrices                //
      //--------------------------------------------------------------------------//

      void generate_order_jumps();
      //--------------------------------------------------------------------------//
      //   It verifies the cycles, finds the transversals, compute orders (for    //
      //     all multiplication methods) and calculates permutation jumps to      //
      //                          each computed order                             //
      //--------------------------------------------------------------------------//

      void classify_factors();
      //--------------------------------------------------------------------------//
      //      It chooses a multiplication method to each tensor product term      //
      //--------------------------------------------------------------------------//

      //--------------------------------------------------------------------------//
      //                           Internal Functions                             //
      //--------------------------------------------------------------------------//
      void alloc();                               // Internal Function of allocation
      void trans(const CND & d);            // Internal Function of tranfer contents
      void desal();                            // Internal Function of desallocation

};

#endif
