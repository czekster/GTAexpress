//====================================================================================//
//                                                                                    //
//                               Class Full Matrix                                    //
//                                                                                    //
//====================================================================================//
//  This File:   fullmat.h                  Language: C++  (xlC and CC)               //
//  Software:    PEPS 3.1                    Advisor: Brigitte.Plateau@imag.fr        //
//  Error Codes: 3500                                                                 //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 14/feb/00                        by: Paulo.Fernandes@imag.fr         //
//====================================================================================//
//     This class is used to store square matrices in the full format. The variable   //
// 'dimension' gives the order (number of rows and columns) of the matrix.            //
//====================================================================================//

class full_mat
{
public:
 inline full_mat();                                                 // Empty Constructor
 inline full_mat(const int d);                                      // Sized Constructor
 inline full_mat(const full_mat & m);                                // Copy Constructor
 inline ~full_mat();                                                       // Destructor
 
 inline void create();                                            // Empty Intialization
 inline void create(const int d);                                 // Sized Intialization
 inline void copy(const full_mat & m);                   // Copy and Sized Intialization
 inline void erase();                                                     // Destruction

 inline int  size() const;                                    // The order of the matrix
 inline int  od() const;                                      // The order of the matrix
 inline int  nz() const;                              // The number of non-zero elements
 inline bool is_empty() const;                                      // The order is zero
 inline bool has_negatives() const;                       // There are negative elements

 inline rp   coord(const int i, const int j) const;             // It returns an element
 inline void coord(const int i, const int j, const rp v);        // It stores an element

 inline void reset();                                    // It sets all elements to zero
 inline void scale(const rp f);                               // It performs 'this *= f'
        void reduce(const bools & v);
                             // It reduce 'this' keeping only the row and columns in 'v'

        //--------------------------------------------------------------------------//
        //  Solution of the linear system 'x = y this' considering only the first   //
        //           'k' rows and columns (the answer is the vector 'y')            //
        //--------------------------------------------------------------------------//
        void solve(const rp * x, rp * y, const int k) const;

        void write(ofstream & fout) const;               // Output on 'fout' file stream
        void read(ifstream & fin);                       // Input from 'fin' file stream
        int  mem_use() const;                  // Memory space used by 'this' (in bytes)

        //--------------------------------------------------------------------------//
        //                          Data Structures                                 //
        //--------------------------------------------------------------------------//
private: int     dimension;      // order of the matrix (number of rows and columns)
         rp   ** mat;            // two-dimensional array with the elements
};

