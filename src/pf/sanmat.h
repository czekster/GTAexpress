//====================================================================================//
//                                                                                    //
//                                Class SAN Matrix                                    //
//                                                                                    //
//====================================================================================//
//  This File:   sanmat.h                   Language: C++  (xlC and CC)               //
//  Software:    PEPS 2002                   Advisor: Brigitte.Plateau@imag.fr        //
//  Error Codes: 3300                                                                 //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change:  7/nov/02                        by: paulof@inf.pucrs.br             //
//====================================================================================//
//     This class is used to store the matrices of the descriptor structures. Each    //
// matrix can be stored according to one of the four formats:                         //
//                                                                                    //
//     - empty:    if all elements are equal to zero;                                 //
//     - identity: all diagonal elements are equal to one;                            //
//     - binary:   all nonzero elements are equal to one (and it is not an idenity);  //
//     - sparse:   all matrices that are not of the previous types;                   //
//                                                                                    //
//     The matrix type is stored in the internal variable 'type'. For all matrix      //
// types the variables 'dimension' and 'nz_number' store the order of the matrix and  //
// its number of nonzero elements. The other structures are used according to the     //
// matrix type.                                                                       //
//                                                                                    //
//    - sparse: the matrices are stored in the Harwell-Boeing Format (hbf) where the  //
// nonzero elements of a same column are stored contiguously (traditional hbf stores  //
// the row elements contiguously). The matrix is stored in three arrays (see class    //
// 'HBF' (file 'hbf.h') to get more information about the format):                    //
//         - 'elements' the nonzero elements (an array of 'elem' objects of size      //
//           'nz_number' - see class 'elem' - file 'elem.h');                         //
//         - 'rows' the row position of each nonzero element (an array of integers of //
//           size 'nz_number');                                                       //
//         - 'columns'  the index (in the previous two arrays) of the beginning of    //
//           each column plus the total number of nonzero elements (an array of       //
//           integers of size 'dimension'+1);                                         //
//                                                                                    //
//    - binary: the matrices are store in a format similar to the hbf, but since all  //
// nonzero elements are equal to one the array 'elements' is not used;                //
//                                                                                    //
//    - identity: the identity matrices do not use other data structures;             //
//                                                                                    //
//    - empty: the empty matrices do not use other data structures;                   //
//====================================================================================//

class san_mat
{
public:
 inline san_mat();                                                  // Empty Constructor
 inline san_mat(const int n, const int d, const mat_typ t);         // Sized Constructor
 inline san_mat(const san_mat & m);                                  // Copy Constructor
 inline ~san_mat();                                                        // Destructor
 
 inline void create();                                            // Empty Intialization
 inline void create(const int n, const int d, const mat_typ t);   // Sized Intialization
        void create(const elem m[], const int d);   // Initialization from a full matrix
        void create(elem **m, const int d);   // Initialization from a full matrix

 inline void copy(const san_mat & m);                    // Copy and Sized Intialization
 inline void erase();                                                     // Destruction

 inline int     size() const;                                 // The order of the matrix
 inline int     nz() const;                            // The number of nonzero elements
 inline mat_typ tp() const;                                           // The matrix type
 inline bool    is_empty() const;                                   // The order is zero
        bool    is_functional() const;                  // There are functional elements
        bool    is_clique(const aut_id a) const;
                      // The union of the arguments of all functional elements of 'this'
                                     // contains at least all automata but automaton 'a'
        bool    has_negatives() const;                    // There are negative elements
        bool    has_diag() const;                         // There are diagonal elements
        void    params(aut_set & s) const;               // The parameters of the matrix
                     // Obs.: the reference set of 's' is the current number of automata

        elem coord(const int i, const int j) const;      // It returns the element 'i,j'
        rp   coord(const int i, const int j, const aut_st & s) const;
                                       // It returns the evaluation of the element 'i,j'
                     // Obs.: the reference set of 's' is the current number of automata

        //--------------------------------------------------------------------------//
        //   It evaluates all functional elements of 'this' according to state 's'  //
        //--------------------------------------------------------------------------//
        // Obs.: the reference set of 's' is the current number of automata.        //
        //--------------------------------------------------------------------------//
 inline void eval(aut_st & s, const aut_id a, const st_map * m);          // traditional
        rp evaluation(int k);
 inline void jit_eval(aut_st & s, const aut_id a, const st_map * m); // 'jit' evaluation

        void scale(const rp f);                               // It performs 'this *= f'
        void solve_functions(const st_map & m);
            // It tries to eliminate functionnal elements according to the state map 'm'
                              // Obs.: this function is called during a grouping process

        void cut_diag();                             // It removes the diagonal elements
        void reduce(const bools & v);          // It removes rows and columns not in 'v'
        void declassify();                  // It changes the type of 'this' to 'sparse'
        void classify();            // It tries to change the type of 'this' either from
                                  // 'sparse' to 'empty', 'identity' or 'binary', either
                                               // from 'binary' to 'empty' or 'identity'

        //--------------------------------------------------------------------------//
        //              It performs the product 'v_out = v_in * this'               //
        //--------------------------------------------------------------------------//
 inline void left_mult(rp v_out[], const rp v_in[]) const;
 inline void sparse_left_mult2(vector<gs_pair> & v_out,
			       const vector<gs_pair> v_in,
			       int s, int m) const;
 inline void sparse_left_mult2(vector<gs_pair> & v_out,
			       const rp *v_in,
			       int base) const;
 
        //--------------------------------------------------------------------------//
        //              It performs the product of the vector v_in and the          //
        //         column 'column' of the matrix 'this'. The result is returned.    //
        //--------------------------------------------------------------------------//
 inline rp sparse_elem_left_mult(const rp v_in[], int column) const;
 inline rp sparse_elem_left_mult2(const vector<gs_pair> v_in, int column) const;
 

        //--------------------------------------------------------------------------//
        //  The next five functions perform operations in the matrix which imply    //
        // changes of the functional element identifiers from the old reference set //
        // (indexes in the 'FTB::ftb' object) to a new reference set (indexes in    //
        // the 'scrt::sft'). These operations are implemented by the operators of   //
        // the class 'elem' (file 'elem.h').                                        //
        //--------------------------------------------------------------------------//
        //                     It only modifies the func. id's                      //
        //--------------------------------------------------------------------------//
        void assign();
        //--------------------------------------------------------------------------//
        //           It copies 'm' to 'this' and modifies the func. id's            //
        //--------------------------------------------------------------------------//
        void assign(const san_mat & m);
        //--------------------------------------------------------------------------//
        //           It performs 'this' = tensor product of 'n' matrices            //
        //                        and modif. the func. id's                         //
        //--------------------------------------------------------------------------//
        void t_prod(const san_mat mat[], const int n);
        //--------------------------------------------------------------------------//
        //             It performs 'this' = tensor sum of 'n' matrices              //
        //                        and modif. the func. id's                         //
        //--------------------------------------------------------------------------//
        void t_sum(const san_mat mat[], const int n);
        //--------------------------------------------------------------------------//
        //                It performs 'this' += sum of '2n' matrices                //
        //                        and modif. the func. id's                         //
        //--------------------------------------------------------------------------//
        void add(const san_mat matp[], const san_mat matm[], const int n);

        void print(ofstream & fout) const;                // Print on 'fout' file stream
        void print() const;                              // Print on 'cout' stream
        void write(ofstream & fout) const;               // Output on 'fout' file stream
        void write() const;                              // Output on  cout
        void read(ifstream & fin);                       // Input from 'fin' file stream
        int  mem_use() const;                  // Memory space used by 'this' (in bytes)

	// *** RGP ***
	int get_elem_lin(const int k);
	int get_elem_col(const int k);
	rp get_elem_value(const int k, const aut_st & s) const;
//	inline void add_identity();
//	inline void minus_identity();

        //--------------------------------------------------------------------------//	
        //                            Friend Functions                              //
        //--------------------------------------------------------------------------//
        //                                                                          //
        // It copies a san_mat eliminating the function according to a given policy //
        //--------------------------------------------------------------------------//
        void friend sp_mat::copy_elim(const f_aprox t,
                                      const san_mat & m, const st_map * s,
                                      const ev_id e,     const aut_id a);

        //--------------------------------------------------------------------------//
        //                          Data Structures                                 //
        //--------------------------------------------------------------------------//
private: mat_typ   type;           // the matrix type
         int       dimension;      // the order of the matrix
         int       nz_number;      // the number of nonzero elementss
         elem    * elements;       // the nonzero elements
         m_coord * rows;           // the row positions of the nonzero elements
         m_coord * columns;        // the begining of each column

         //--------------------------------------------------------------------------//
         //                           Internal Functions                             //
         //--------------------------------------------------------------------------//
         void alloc();                               // Internal Function of allocation
         void trans(const san_mat & m);         // Internal Function copy 'm' on 'this'
         void desal();                            // Internal Function of desallocation
};

