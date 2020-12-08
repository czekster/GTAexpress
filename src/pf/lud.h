//====================================================================================//
//                                                                                    //
//                       LU decomposed Descriptor Structure                           //
//                                                                                    //
//====================================================================================//
//  This File:   lud.h                      Language: C++  (xlC and CC)               //
//  Software:    PEPS 3.1                    Advisor: Brigitte.Plateau@imag.fr        //
//  Error Codes: 5500                                                                 //
//====================================================================================//
//  Creation:    22/jun/96                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 14/feb/00                        by: Paulo.Fernandes@imag.fr         //
//====================================================================================//
//     This structure is used to store a LU decomposition of the tensor product terms //
// of a given descriptor. Each matrix of the descriptor is decomposed and its LU      //
// compact representation ('sp_mat' object) is stored. The descriptor matrices have   //
// their functional elements eliminated before the LU decomposition (see member       //
// functions of the class 'sp_mat').                                                  //
//                                                                                    //
//     Analogously to the descriptor structure ('DSC' object) the arrays 'local_mat', //
// 'synch_mat' and 'synch_adj' store respectively the local matrices and the          //
// occurence and adjustement matrices of the synchronizing events.                    //
//                                                                                    //
//     The vector 'sizes' stores the size of each automata. Since the matrices stored //
// do not have functional elements, only one table of permutation jumps ('jumps'      //
// object) is necessary. This table corresponds to the automata ranked in the         //
// "normal" order (0, 1, 2, ...).                                                     //
//                                                                                    //
//     This structure is used as a preconditionner to the descriptor. According to    //
// the preconditionning approach only one tensor product term of the LU decomposed    //
// descriptor (called LUD) is used at a time. The variables 'syn_term', 'loc_term'    //
// and 'pm_term' define the "term of the turn". A value 'no_ev' in the variable       //
// 'syn_term' means that the term is a local, in such case the variable 'loc_term'    //
// indicates which one of the local terms is being pointed out. If the variable       //
// 'syn_term' indicates a synchronizing event, the variable 'pm_term' defines the     //
// occurence or the adjustement term. To change the "term of the turn" the member     //
// functions 'reset_turn' and 'next_turn' are used.                                   //
//                                                                                    //
//      During the decomposition of the descriptor matrices some terms can be marked  //
// as invalid (the matrices are not decomposable). This information is stored in the  //
// vectors of booleans 'l_valid' and 's_valid'.                                       //
//====================================================================================//

class LUD
{
public:
 inline LUD();                                                      // Empty Constructor
 inline LUD(const aut_id a, const ev_id e);                         // Sized Constructor
 inline LUD(const LUD & d);                                          // Copy Constructor
 inline ~LUD();                                                            // Destructor

 inline void create();                                            // Empty Intialization
 inline void create(const aut_id a, const ev_id e);               // Sized Intialization
 inline void copy(const LUD & d);                            // Copy Sized Intialization
 inline void erase();                                                     // Destruction

 inline void    baptise(const peps_name file);                 // It assigns a file name
 inline void    name(peps_name file) const;                  // It returns the file name
 inline int     size() const;                     // The number of automata of the model
 inline aut_id  aut() const;                      // The number of automata of the model
 inline ev_id   ev() const;                        // The number of synchronizing events
 inline int     ss() const;                                   // The state space product
 inline int     v_terms() const;                            // The number of valid terms
 inline bool    is_empty() const;                      // The number of automata is zero
 inline reg_typ regular() const;   // The regularization performed during the generation

        //--------------------------------------------------------------------------//
        //                     Term of the turn functions                           //
        //--------------------------------------------------------------------------//
 inline void reset_turn();     // It sets the "term of the turn" to the first valid term
 inline void next_turn();       // It sets the "term of the turn" to the next valid term

        void write() const;                              // Output on 'fout' file stream
        void read(const peps_name file);                 // Input from 'fin' file stream
        int  mem_use() const;                  // Memory space used by 'this' (in bytes)

        //--------------------------------------------------------------------------//
        //                             Friend functions                             //
        //--------------------------------------------------------------------------//
        //  Multiply 'this' (a probability vector) by the inverse of the LUD "term  //
        //                of the turn"   -   'this = this * LUD^-1'                 //
        //--------------------------------------------------------------------------//
        void friend VCT::solv_LUD();
        //--------------------------------------------------------------------------//
        //  Multiply 'this' (a probability vector) by the inverse of the LUD "term  //
        //                of the turn"   -   'this = v * LUD^-1'                    //
        //--------------------------------------------------------------------------//
        void friend VCT::solv_LUD(const VCT & v);
        //--------------------------------------------------------------------------//
        //  Multiply 'v' (a probability vector) by the inverse of the LUD "term of  //
        //        the turn", adding the result to 'this' - 'this = v * LUD^-1'      //
        //--------------------------------------------------------------------------//
        void friend VCT::solv_LUD_P(const VCT & v);

        //--------------------------------------------------------------------------//
        //                            Main functions                                //
        //--------------------------------------------------------------------------//
        //                                                                          //
        //                     Generation from the Descriptor                       //
        //                                                                          //
        //    Input Parameters:                                                     //
        //                                                                          //
        //      - 'DSC::dsc'  the descriptor to be decomposed;                      //
        //      - 'FTB::ftb'  the descriptor function table;                        //
        //      - 'name'      the file name of the new structure;                   //
        //      - 'f'         the uniformization factor (largest element in modulo  //
        //                      of the descriptor);                                 //
        //                                                                          //
        //    Output Parameter:                                                     //
        //                                                                          //
        //      - 'this'      the decomposed descriptor;                            //
        //                                                                          //
        //   This operation is performed by three distinct functions:               //
        //                                                                          //
        //      - 'generate'   - it copies the descriptor ('DSC::dsc') matrices to  //
        //                       'this', eliminating the functional elements;       //
        //      - 'normalize'  - it normalizes the matrices (scaling) of 'this'     //
        //                       using 'f';                                         //
        //      - 'decompose'  - it decomposes the matrices of 'this';              //
        //                                                                          //
        //    Note: This operation is split in three distinct functions to allow    //
        // the evaluation of the largest element of the descriptor outside (using   //
        // the same factor of the normalized descriptor ('CND::cnd').               //
        //--------------------------------------------------------------------------//
        void generate(const peps_name name);
        void normalize(const rp f);
        void decompose();

        //--------------------------------------------------------------------------//
        //                             Static variable                              //
        //--------------------------------------------------------------------------//
        static LUD lud;

        //--------------------------------------------------------------------------//
        //                             Data Structures                              //
        //--------------------------------------------------------------------------//
private: peps_name   file_name;        // the file name of this structure
         aut_id      automata;         // the number of automata
         ev_id       events;           // the number of synchronizing events
         aut_st      sizes;            // the size of each automaton
         reg_typ     regular_type;     // the type of regularization performed to matrices
         jumps       p_jumps;          // the permutation jumps of the automata
         sp_mat    * local_mat;        // the matrices of the local terms
         sp_mat    * local_mat2;       // the matrices of the local terms
         sp_mat   ** synch_mat;        // the matrices of the synchronizing terms
         sp_mat   ** synch_mat2;       // the matrices of the synchronizing terms
         sp_mat   ** synch_adj;        // the matrices of the synchronizing terms
         sp_mat   ** synch_adj2;       // the matrices of the synchronizing terms
         bool      * l_valid;          // the local term is valid
         bool     ** s_valid;          // the synchronizing event term (+ or -) is valid

         ev_id       syn_term;
         aut_id      loc_term;         // pointers to the "term of the turn"
         p_m         pm_term;

     //--------------------------------------------------------------------------//
     //                           Internal Functions                             //
     //--------------------------------------------------------------------------//
     inline bool turn_is_not_valid(); 
                          // It returns 'true' if the term of the turn is not valid
     void alloc();                               // Internal Function of allocation
     void trans(const LUD & d);         // Internal Function - copies 'd' on 'this'
     void desal();                            // Internal Function of desallocation
};
