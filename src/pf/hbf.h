//====================================================================================//
//                                                                                    //
//                       Class Harwell-Boeing Format Matrix                           //
//                                                                                    //
//====================================================================================//
//  This File:   hbf.h                      Language: C++  (xlC and CC)               //
//  Software:    PEPS 3.1                    Advisor: Brigitte.Plateau@imag.fr        //
//  Error Codes: 3600                                                                 //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 07/apr/01                        by: Anne.Benoit@imag.fr             //
//  Last Change: 07/feb/04                        by: Ihab.Sbeity@imag.fr             //
//====================================================================================//
//     This class is used to store the PEPS main structure "huge sparse matrix". This //
// structure is stored in the Harwell-Boeing Format where the nonzero elements of a   //
// same column are stored contiguously (traditional Harwell-Boeing format stores the  //
// row elements contiguously).                                                        //
//                                                                                    //
//     The static variable 'HBF::hbf' contains the current HBF sparse matrix.         //
//                                                                                    //
//     The structure has two delimiters: the number of nonzero elements ('nz_number') //
// and the order (number of rows and columns) of the matrix ('dimension').            //
//                                                                                    //
//     The matrix information is stored in three arrays:                              //
//                                                                                    //
//     - 'elements' the nonzero elements (an array of reals of size 'nz_number');     //
//     - 'rows'     the row position of each nonzero element (an array of integers of //
//                    size 'nz_number');                                              //
//     - 'columns'  the index (in the previous two arrays) of the beginning of each   //
//                    column (an array of integers of size 'dimension'+1);            //
//                                                                                    //
//------------------------------------------------------------------------------------//
//                                     Example                                        //
//                                                                                    //
// The matrix:    can be stored with:                                                 //
//                          nz_number: 8                                              //
// | -0.3  0.1  0.0  0.2 |  dimension: 4                                              //
// |  0.5 -0.8  0.0  0.3 |  elements: -0.3, 0.5,  0.1, 0.2, -0.8,    0.3, 0.2, -0.2   //
// |  0.0  0.0  0.0  0.0 |  rows:      0,   1,    0,   3,    1,      1,   0,   3      //
// |  0.0  0.2  0.0 -0.2 |  columns:   0,         2,              5, 5,             8 //
//                                                                                    //
// Note that the order of the nonzero elements of a column is not important.          //
//                                                                                    //
// Obs.: In order to exchange sparse matrices between PEPS and Marca, indexes from    //
//       1 to N also can be accepted (in PEPS the indexes are always from 0 to N-1).  //
//====================================================================================//

class HBF
{
public:
 inline HBF();                                                      // Empty Constructor
 inline HBF(const int n, const int d);                              // Sized Constructor
 inline HBF(const HBF & m);                                          // Copy Constructor
 inline ~HBF();                                                            // Destructor

 inline void create();                                            // Empty Intialization
 inline void create(const int n, const int d);                    // Sized Intialization
 inline void copy(const HBF & m);                        // Copy and Sized Intialization
 inline void erase();                                                     // Destruction

 inline void baptise(const peps_name file);                    // It assigns a file name
 inline void name(peps_name file) const;                     // It returns the file name
 inline int  size() const;                             // The number of nonzero elements
 inline int  order() const;                     // The matrix order (the number of rows)
 inline bool is_empty() const;                                      // The order is zero
 inline int  first_row() const;     // The index of the first row (indicates the format)

        bool lu_decomp(const rp th); // Replace 'this' by its Doolittle LU decomposition
        void lu_solve(const rp * x, rp * y) const;
                          // Solve 'x = y * this' ('this' are the Doolittle LU matrices)

        rp   coord(const int i, const int j) const;      // It returns the 'i,j' element
        rp   biggest() const;              // It returns the largest element (in modulo)
        void scale(const rp f);              // It multiplies all matrix elements by 'f'
        void scale();        // It multiplies all matrix elements by the largest element

        void transpose();                                            // transpose 'this'
        void convert(const peps_name file, const hbf_format format);
                  // Output on 'fout' file stream converting the row and columns indexes
        void write() const;                              // Output on 'fout' file stream
        void read(const peps_name file);                 // Input from 'fin' file stream
        int  mem_use() const;                  // Memory space used by 'this' (in bytes)

        //--------------------------------------------------------------------------//
        //                            Friend Functions                              //
        //--------------------------------------------------------------------------//
        //                                                                          //
        //                 Multiplication by a probability vector                   //
        //--------------------------------------------------------------------------//
        void friend VCT::mult_HBF(const VCT & v);

        //--------------------------------------------------------------------------//
        //                             Main Functions                               //
        //--------------------------------------------------------------------------//
        //                                                                          //
        //                     Generation from the Descriptor                       //
        //                                                                          //
        //    Input Parameters:                                                     //
        //                                                                          //
        //      - 'DSC::dsc'  the descriptor to be expanded;                        //
        //      - 'FTB::ftb'  the function table;                                   //
        //      - 'RSS::rss'  the reachable state space;                            //
        //      - 'name'      the file name of the new structure (the .hbf file);   //
        //                                                                          //
        //    Output Parameter:                                                     //
        //                                                                          //
        //      - 'this'      the sparse matrix corresponding to the descriptor;    //
        //                                                                          //
        //   This function generates a sparse Hardwell-Boeing format matrix (with   //
        // elements of the same column stored contiguously) corresponding to a      //
        // given SAN descriptor ('DSC::dsc').                                       //
        //                                                                          //
        //   IMPORTANT: for huge models this function may need HUGE memory space.   //
        // The memory allocation during of the elements generation is made by       //
        // individual calls of sparse vectors for each column. However, the final   //
        // storage of the matrix requires an extra temporary space to assembly the  //
        // sparse vectors of each column in a single vector. This function actually //
        // needs (at its final step) a memory space about twice the actual need to  //
        // store the matrix.                                                        //
        //--------------------------------------------------------------------------//
	void Visit_Column(rp val, aut_id mark, int a, int nbaut, const aut_st d_sizes,
			  aut_st & d_i, aut_st & d_j, aut_st o_state, int r_i,
			  int & r_j, int & o_j);
	void Visit_Column_Evtp(rp val, int e, int a, int nbaut,
			      const aut_st d_sizes, aut_st & d_i, aut_st & d_j,
			      aut_st o_state, int r_i, int & r_j, int & o_j);
	void Visit_Column_Evtn(rp val, int e, int a, int nbaut,
			      const aut_st d_sizes, aut_st & d_i, aut_st & d_j,
			      aut_st o_state, int r_i, int & r_j, int & o_j);
	void generateF(const peps_name file);
        void generateS(const peps_name file);
        void generate(const peps_name file); // sparce generation
	int   LexToSan(int * info, int N, aut_st d_sizes);
	//void exploration(int g, int d, int& place);
	//void choix_pivot(int g, int d);
	//void decoupage(int g, int d);
	//void fast_tri(rp * elem, int * rows, int g, int d);
	//void Ordering(int g, int d);
	int Find_Max(int i);
	void Permute(int i, int j);
	int FindIndice(int val, int size);

        //--------------------------------------------------------------------------//
        //                          Static variable                                 //
        //--------------------------------------------------------------------------//
        static HBF hbf;

	static vector<info_gspair> rowelem;  // the element to store in the hbf matrix
                                             // (used by HBF::generateS)

        //--------------------------------------------------------------------------//
        //                          Data Structures                                 //
        //--------------------------------------------------------------------------//
private: peps_name file_name;             // the file name of this structure
         int       dimension;             // the order of the matrix
         int       nz_number;             // the number of nonzero elements
         rp      * elements;              // the nonzero elements
         int     * rows;                  // the row positions of the nonzero elements
         int     * columns;               // the begining of each column
	 rp * col_elem;                   // used in the sparce matrix generation, as temporary to store the column elements values
         int * col_rows;		  // used in the sparce matrix generation ,as temporary to store the column elements indexes
};



      //--------------------------------------------------------------------------//
      //         Internal class 'comp' used to compare two gs_pair                //
      //--------------------------------------------------------------------------//
 class comp{
   public :
     bool operator() (gs_pair x, gs_pair y) {
        return (x.first < y.first);
     }
 };  

      //--------------------------------------------------------------------------//
      //         Internal class 'comp3' used to compare two info_gspair           //
      //--------------------------------------------------------------------------//
 class comp3{
   public :
     bool operator() (info_gspair x, info_gspair y) {
        return (x.first < y.first);
     }
 };  


