//====================================================================================//
//                                                                                    //
//                              Class Sparse Matrix                                   //
//                                                                                    //
//====================================================================================//
//  This File:   spmat.h                    Language: C++  (xlC and CC)               //
//  Software:    PEPS 3.1                    Advisor: Brigitte.Plateau@imag.fr        //
//  Doc:         mat.tex                                                              //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 14/feb/00                        by: Paulo.Fernandes@imag.fr         //
//====================================================================================//

class sp_mat
{
public: sp_mat();                                                   // Empty Constructor
        sp_mat(const int n, const int d);                           // Sized Constructor
        sp_mat(const sp_mat & m);                                    // Copy Constructor
        ~sp_mat();                                                         // Destructor
 
        void create();                                                  // Intialization
        void create(const int n, const int d);                           // Construction
        void copy(const sp_mat & m);                                             // Copy
        void erase();                                                     // Destruction

        int  size() const;                               // Give the order of the matrix
        int  od() const;                                 // Give the order of the matrix
        int  nz() const;                         // Give the number of non-zero elements
        bool is_empty() const;                                               // Is Empty
        bool has_negatives() const;                       // There are negative elements

        rp   coord(const int i, const int j) const;                 // Give a coordenate
        void scale(const rp f);                                               // Scaling
        void reduce(const bools & v);              // Keep only the row and columns in v


        // Copies a san_mat eliminating the function according to a given police
        void copy_elim(const f_aprox t, const san_mat & m, const st_map * s,
                                        const ev_id e,     const aut_id a);

        void add_diagonal(const rp v);          // Adds diagonal elements with value 'v'
        void winglet();                         // Puts diagonal elements with value 'v'
        void tychonov(const sp_mat & m);                   // this = (m * m^T) + epsilon

        //--------------------------------------------------------------------------//
        //              It performs the product 'v_out = v_in * this'               //
        //--------------------------------------------------------------------------//
        void left_mult(rp v_out[], const rp v_in[]) const;

        void transpose();                                           // Transposes 'this'
        bool sor_decomp(const rp th, const reg_typ t);
                             // Replaces 'this' by its SOR-like (L D^-1 U) decomposition
        bool lu_decomp(const rp th, const reg_typ t);
                                    // Replaces 'this' by its Doolittle LU decomposition


        void LU_solve(const rp * x, rp * y) const;   // Chooses one of the options below
        void lu_solve(const rp * x, rp * y) const;
                          // Solve 'x = y * this' ('this' are the Doolittle LU matrices)
        void l_solve(const rp * x, rp * y) const;      // The same but just for L matrix
        void u_solve(const rp * x, rp * y) const;      // The same but just for U matrix

        void write(ofstream & fout) const;                             // Write (output)
        void read(ifstream & fin);                                       // Read (input)
        int  mem_use() const;                  // Memory space used by 'this' (in bytes)

private: int    dimension;      // order of the matrix
         int    nz_number;      // number of non-zeros
         rp   * elements;       // the elements that need to be stored
         int  * rows;           // the row positions that need to be stored
         int  * columns;        // the column positions that need to be stored
         int  * diag;           // the column positions marking the diagonal elements
};

