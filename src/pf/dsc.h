//====================================================================================//
//                                                                                    //
//                                Descriptor Structure                                //
//                                                                                    //
//====================================================================================//
//  This File:   dsc.h                      Language: C++  (xlC and CC)               //
//  Software:    PEPS 3.1                    Advisor: Brigitte.Plateau@imag.fr        //
//  Error Codes: 5300                                                                 //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 13/may/01                        by: lbrenner@inf.pucrs.br           //
//====================================================================================//
//                                                                                    //
// IMPORTANT: This descriptor structure can ONLY be used for continuous-time models.  //
//                                                                                    //
//====================================================================================//
//     This class is used to store the PEPS main structure "descriptor". The static   //
// variable 'DSC::dsc' contains the current SAN descriptor.                           //
//                                                                                    //
//     Altough the descriptor structure is planned to be used also for discrete-time  //
// models, only continuous-time models are currently handled by PEPS. Extensions to   //
// the PEPS code can use other values than 'continuous' for the enumeration private   //
// variable 'model_type'. The inclusion of discrete-time models implies the change of //
// ALMOST ALL member functions of this class.                                         //
//                                                                                    //
//     A descriptor is a structure storing the matrices of the sum of N + 2E tensor   //
// products, where N is the number of automata and E is the number of synchronizing   //
// events of the model (this information is stored in the private variables           //
// 'automata' and 'events').                                                          //
//                                                                                    //
//     The first N terms of the model correspond to the local terms. Each local term  //
// is represented only by its non-identity matrix. The local terms matrices are       //
// stored in the array 'local_mat'. These matrices are indexed from 0 to N-1, which   //
// are the internal id's of the automata.                                             //
//                                                                                    //
//     Each synchronizing event has two tensor product terms with N matrices each.    //
// The first tensor product term contains:                                            //
//     - one matrix with the rates of occurence of the event (strictly positive)      //
//         for the automaton which is master of the synchronizing event;              //
//     - some matrices (at least one) with the routing probabilities for the slave    //
//         automata (other automata concerned by the synchronizing event);            //
//     - some identity matrices (eventually none) for the automata not concerned by   //
//         the synchronizing event.                                                   //
//                                                                                    //
//     The second tensor product term contains diagonal matrices such that the sum of //
// the two terms is a generator (the sum of row elements is zero).                    //
//                                                                                    //
//     The two-dimensional arrays 'synch_mat' and 'synch_adj' store respectively the  //
// matrices of both terms. In these two arrays the first coordinate is the internal   //
// id of the event (from 0 to 'event'-1), the second coordinate is the automaton      //
// internal id (from 0 to 'automata'-1). For example, 'synch_mat[2][4]' is the        //
// occurence matrix of the fifth automaton (its internal id is '4') of the third      //
// event (id = 2).                                                                    //
//                                                                                    //
//      Additionally, 'sync_rates' (an array of automata id's) indicates the master   //
// automaton of each synchronizing event (which matrix has the rates of the event).   //
//                                                                                    //
//      The array 'symetric' contains integer labels for each automaton. Identical    //
// (replicated) automata have identical labels, which identify their type. This       //
// information can be used to improve the manipulation of identical automata.         //
//                                                                                    //
//      In the process of grouping (see main function 'aggregation') a new name is    //
// given to the structure, the original name of the structure is stored in the        //
// private variable 'orig_name'. Also in the process of grouping, the matrices        //
// corresponding to the grouped automata are built. One of the effects of this        //
// grouping process is that each state of the resulting grouped automaton corresponds //
// to a combination of the states of each component. For example, if two automata     //
// with sizes 4 and 3 are grouped the resulting automaton (with size 12) will have    //
// its states corresponding to:                                                       //
//                                                                                    //
//     +-----------+-----------+-----------+-----------+-----------+-----------+      //
//     | resulting | component | resulting | component | resulting | component |      //
//     | automaton | automata  | automaton | automata  | automaton | automata  |      //
//     +-----------+-----+-----+-----------+-----+-----+-----------+-----+-----+      //
//     |     0     |   0 |   0 |     4     |   1 |   1 |     8     |   2 |   2 |      //
//     |     1     |   0 |   1 |     5     |   1 |   2 |     9     |   3 |   0 |      //
//     |     2     |   0 |   2 |     6     |   2 |   0 |    10     |   3 |   1 |      //
//     |     3     |   1 |   0 |     7     |   2 |   1 |    11     |   3 |   2 |      //
//     +-----------+-----+-----+-----------+-----+-----+-----------+-----+-----+      //
//                                                                                    //
//     This kind of table with the correspondence between the states is called "state //
// map" and it is implemented by the class 'st_map' (file "stmap.h"). Another effect  //
// of the grouping process is the possible elimination of non-reachable states of the //
// resulting automaton. In this case the state map will be reduced by the elimination //
// of the non-reacheable entries.                                                     //
//                                                                                    //
//     The state map of each automaton of the descriptor is stored in the private     //
// array variable 'state_map'. Note that for all purposes, non-grouped descriptors    //
// can be handled exactly as already grouped descriptors. Non-grouped descriptors     //
// have state maps with elementary tables (state 0 corresponds to state 0, state 1    //
// corresponds to state 1, etc).                                                      //
//====================================================================================//

class DSC
{
public:
 inline DSC();                                                      // Empty Constructor
 inline DSC(const md_type m, const aut_id a, const ev_id e);        // Sized Constructor
 inline DSC(const DSC & d);                                          // Copy Constructor
 inline ~DSC();                                                            // Destructor

 inline void create();                                            // Empty Intialization
 inline void create(const md_type m, const aut_id a, const ev_id e);
                                                                  // Sized Intialization
 inline void copy(const DSC & d);                        // Copy and Sized Intialization
 inline void erase();                                                     // Destruction

 inline void    baptise(const peps_name file);                  // It assigns a file name
 inline void    o_baptise(const peps_name file);    // It assigns the original file name
 inline void    name(peps_name file) const;                   // It returns the file name
 inline void    o_name(peps_name file) const;       // It returns the original file name
 inline int     size() const;                             // The total number of automata
 inline md_type type() const;                                             // The model type
 inline aut_id  aut() const;                              // The total number of automata
 inline ev_id   ev() const;                  // The total number of synchronizing events
 inline int     ss() const;                               // The product state space size
 inline bool    is_empty() const;                                  // There is no automata
 inline bool    is_grouped() const;                // The descriptor was already grouped
 inline rp      cost() const;                          // The solution computational cost
 inline rp      l_cost() const;    // The solution computational cost to local matrices
 inline rp      s_cost() const;    // The solution computational cost to synch matrices
 inline rp      s_cost(int s) const;// The solution computational cost to synch matrices
 inline rp      acost() const;                        // The solution computational cost
 inline rp      l_acost() const;  // The solution computational cost to local matrices
 inline rp      s_acost() const;  // The solution computational cost to synch matrices
 inline rp      s_acost(int s) const;// The solution computational cost to synch matrices
        int nz();                  // It counts how much nonzero elements would have the
                                    // sparse matrix equivalent to the descritpor 'this'

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

        //--------------------------------------------------------------------------//
        //                    Evaluation of a transition rate                       //
        //--------------------------------------------------------------------------//
        //    This function also computes the numeric value of the transition rate  //
        // corresponding to a transition from the global state 'i' to the global    //
        // state 'j'. The coordinates 'i' and 'j' are vectors where each entry is   //
        // the state of a group. Additionally, this function also receives the set  //
        // of parameters ('s') necessary to evaluate the functional elements of the //
        // descriptor. Usually the information in 'i' and 's' is equivalent. The    //
        // difference between this two vectors is their set of reference. The input //
        // parameter 'i' refers to the current descriptor number of automata (after //
        // grouping), while the input parameter 's' refers to the original number   //
        // of automata (before grouping).                                           //
        //                                                                          //
        // Obs.: The parameter 's' could be calculated from the input parameter row //
        // coordinate ('i') and the descriptor state maps.                          //
        //--------------------------------------------------------------------------//
        rp coord(const aut_st & i, const aut_st & j, const aut_st & s);
        rp get_local_coord(aut_id a, int i, int j, const aut_st & s);
        rp get_pos_sync_coord(int e, int a, int i, int j, const aut_st & s);
        rp get_neg_sync_coord(int e,  aut_st d_i, const aut_st & s);
    	rp get_sync_coord(int  i, int j, const aut_st & s);


	// Returns for an automaton and an event the coordinate in the local matrix,
	// the positive synchronizing matrix, or the negative sync. matrix.
        rp coord_loc(aut_id a, const aut_st & i, const aut_st & j, const aut_st & s);
        rp coord_sync_p(ev_id e, aut_id a,
			const aut_st & i, const aut_st & j, const aut_st & s);
        rp coord_sync_n(ev_id e, aut_id a,
			const aut_st & i, const aut_st & j, const aut_st & s);

        void write() const;                              // Output on 'fout' file stream
        void read(const peps_name file);                 // Input from 'fin' file stream
        void print() const;               // Prints a readable version in 'dsc.dbg' file
        int  mem_use() const;                  // Memory space used by 'this' (in bytes)

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
        //                      The size of the automaton 'a'                       //
        //--------------------------------------------------------------------------//
 inline void put_s(const aut_id a, const int s);
 inline int get_s(const aut_id a) const;
        //--------------------------------------------------------------------------//
        //                  The local matrix of the automaton 'a'                   //
        //--------------------------------------------------------------------------//
 inline void put_mat(const int a, const san_mat & m);
 inline void get_mat(const int a, san_mat & m) const;
        //--------------------------------------------------------------------------//
        //      The matrix of the event 'e' for the automaton 'a' of type 'pm'      //
        //--------------------------------------------------------------------------//
        //   Obs.: enumerated type 'p_m' indicates whether a matrix contains the    //
        // rates or probabilities of occurence ('plus') or is the normalization     //
        // matrix ('minus').                                                        //
        //--------------------------------------------------------------------------//
 inline void put_mat(const ev_id e, const aut_id a, const p_m pm, const san_mat & m);
 inline void get_mat(const ev_id e, const aut_id a, const p_m pm, san_mat & m) const;
        //--------------------------------------------------------------------------//
        //                     The state map of automaton 'a'                       //
        //--------------------------------------------------------------------------//
 inline void put_sm(const aut_id a, const st_map & s);
 inline void get_sm(const aut_id a, st_map & s) const;
        //--------------------------------------------------------------------------//
        //              The automaton with the rates for all events                 //
        //--------------------------------------------------------------------------//
 inline void put_sr(const aut_id * a);
 inline void get_sr(aut_id * a) const;
        //--------------------------------------------------------------------------//
        //              The automaton with the rates for event 'e'                  //
        //--------------------------------------------------------------------------//
 inline void put_sr(const ev_id e, const aut_id a);
 inline aut_id get_sr(const ev_id e) const;
        //--------------------------------------------------------------------------//
        //                    The type labels of all automata                       //
        //--------------------------------------------------------------------------//
 inline void put_sms(const aut_st & s);
 inline void get_sms(aut_st & s) const;
        //--------------------------------------------------------------------------//
        //                    The type of automata a                       //
        //--------------------------------------------------------------------------//
 inline mat_typ get_type(ev_id e, const aut_id  a);
 inline mat_typ get_type(const aut_id  a);
        //--------------------------------------------------------------------------//
        //                    The type label of automaton 'a'                       //
        //--------------------------------------------------------------------------//
 inline void put_sms(const aut_id a, const int s);
 inline int get_sms(const aut_id a) const;

        //--------------------------------------------------------------------------//
        //                            Friend Functions                              //
        //--------------------------------------------------------------------------//
        //                                                                          //
        //                   LU decomposition of a descriptor                       //
        //--------------------------------------------------------------------------//
        void friend LUD::generate(const peps_name name);

        //--------------------------------------------------------------------------//
        //                             Main Functions                               //
        //--------------------------------------------------------------------------//
        //                                                                          //
        //                      Aggregation of the Descriptor                       //
        //                                                                          //
        //    Input and Output Parameters:                                          //
        //                                                                          //
        //      - 'this'      before: the descriptor to be aggregated;              //
        //                    after:  the aggregated descriptor;                    //
        //      - 'FTB::ftb'  before: the old function table;                       //
        //                    after:  the new function table;                       //
        //      - 'RSS::rss'  before: the old (non-reduced) reacheable state space; //
        //                    after:  the new (reduced) reacheable state space;     //
        //                                                                          //
        //    Only Input Parameters:                                                //
        //                                                                          //
        //      - 's'         an aggregation map, i.e., an array of automata labels //
        //                    where automata with the same label will be grouped    //
        //                    into a single automaton;                              //
        //      - 'name'      the file name of the new structures (that are saved   //
        //                    after grouping);                                      //
        //                                                                          //
        //   Restrictions: the aggregation map MUST contain contiguous labels from  //
        // 0 to M-1, where M is the number of different labels in the aggregation   //
        // map. This restriction allows the definition the new descriptor with M    //
        // grouped automata and the use of the labels as the internal id's of the   //
        // groups. Note that, the grouping process can also change the ranking      //
        // order of the automata.                                                   //
        //                                                                          //
        //   Note: there are no restrictions to create groups with only one         //
        // automaton. In this case, the automaton will not change.                  //
        //--------------------------------------------------------------------------//
        void aggregate(const aut_st & s, const peps_name name);
        void aggregate_replics(const aut_st & s, const peps_name name);


        //--------------------------------------------------------------------------//
        //                          Static variable                                 //
        //--------------------------------------------------------------------------//
        static DSC dsc;

        //--------------------------------------------------------------------------//
        //                          Data Structures                                 //
        //--------------------------------------------------------------------------//
private: peps_name   file_name;        // the file name of this structure
         peps_name   orig_name;        // the file name of the original structure
         md_type     model_type;       // discrete or continuous-time model
         aut_id      automata;         // the number of automata
         ev_id       events;           // the number of synchronizing events
         aut_st      sizes;            // the size of each automaton
         san_mat   * local_mat;        // the matrices of the local terms
         san_mat  ** synch_mat;        // the + matrices of the synchronizing terms
         san_mat  ** synch_adj;        // the - matrices of the synchronizing terms
         st_map    * state_map;        // the corresponding state map to each automaton
         aut_id    * sync_rates;       // where are the rates of the synch. events
         aut_st      symetric;         // array of automata types

      //--------------------------------------------------------------------------//
      //            Internal Functions of the Main Function Agregation            //
      //--------------------------------------------------------------------------//

      void make_a_group(const aut_st & s, const aut_id g, DSC & d) const;
      void make_a_replic_group(const aut_st & s, const aut_id g, DSC & d) const;
      //--------------------------------------------------------------------------//
      // It builds the tensor sum of the local matrices and the tensor product of //
      //  the sync. matrices for the grouped automaton 'g'. The automata used as  //
      //      components are coded with label 'g' in the aggregation map 's'.     //
      //--------------------------------------------------------------------------//

      void reduce_a_group(const aut_id g);
      //--------------------------------------------------------------------------//
      //           It reduces all matrices of an automaton according to           //
      //                     the reachable state space                            //
      //--------------------------------------------------------------------------//

      void eliminate_events();
      //--------------------------------------------------------------------------//
      //         It eliminates the synchronizing events that become local         //
      //                       to a grouped automaton                             //
      //--------------------------------------------------------------------------//

      void solve_functions();
      //--------------------------------------------------------------------------//
      //          It evaluates the functional elements that are constants         //
      //--------------------------------------------------------------------------//

      //--------------------------------------------------------------------------//
      //                           Internal Functions                             //
      //--------------------------------------------------------------------------//
      void alloc();                               // Internal Function of allocation
      void trans(const DSC & d);             // Internal Function copy 'd' on 'this'
      void desal();                            // Internal Function of desallocation
};
