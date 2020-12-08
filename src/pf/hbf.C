//====================================================================================//
//                                                                                    //
//                       Class Harwell-Boeing Format Matrix                           //
//                                                                                    //
//====================================================================================//
//  This File:   hbf.C                      Language: C++  (xlC and CC)               //
//  Software:    PEPS 2002                   Advisor: Brigitte.Plateau@imag.fr        //
//  Error Codes: 3600                                                                 //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change:  07/feb/04                        by: Ihab.Sbeity@imag.fr             //
//====================================================================================//
#include "peps.h"

     //--------------------------------------------------------------------------//
     //                          Static variable                                 //
     //--------------------------------------------------------------------------//
HBF HBF::hbf = HBF();

vector<info_gspair> HBF::rowelem;  // the element to store in the hbf matrix
                                   // (used by HBF::generateS)

     //--------------------------------------------------------------------------//
     //                      It returns the 'i,j' element                        //
     //--------------------------------------------------------------------------//
rp HBF::coord(const int i, const int j) const
{
#ifdef _PEPS_DEBUG_
  if ((i >= dimension) || (j >= dimension))
    Programming_Error("invalid coordinates", 3601)
#endif
 rp answer = zero;

 for (int k=columns[j]; k<columns[j+1]; k++)   // it searches the element in its column
   if (rows[k] == i) {
     answer = elements[k];
     break;
 }
 return(answer);
}
     //--------------------------------------------------------------------------//
     //                 It returns the largest element (in modulo)               //
     //--------------------------------------------------------------------------//
rp HBF::biggest() const
{
 rp answer = zero;

 for (int i=0; i<nz_number; i++)
   if (answer < absv(elements[i]))
     answer = absv(elements[i]);
 return(answer);
}
     //--------------------------------------------------------------------------//
     //                 It multiplies all matrix elements by 'f'                 //
     //--------------------------------------------------------------------------//
void HBF::scale(const rp f)
{
 for (int i=0; i<nz_number; i++)
   elements[i] *= f;
}
     //--------------------------------------------------------------------------//
     //          It multiplies all matrix elements by the largest element        //
     //--------------------------------------------------------------------------//
void HBF::scale()
{
 rp factor = biggest();
#ifdef _PEPS_DEBUG_
  if (absv(factor) < close_to_zero)
    Programming_Error("scaling a zero matrix", 3602)
  else
#endif
 factor = one / factor;
 scale(factor);
}

     //--------------------------------------------------------------------------//
     //                            Transpose the matrix                          //
     //--------------------------------------------------------------------------//
void HBF::transpose()
{
 rp  * new_elem;
 int * new_rows;
 int * new_cols;
 int j, k;
 ///int next;

 new_elem = new  rp[nz_number];
 new_rows = new int[nz_number];
 new_cols = new int[dimension+1];

 for (j=0; j<dimension+1; j++)                 // counts the elements by row
   new_cols[j] = 0;
 for (k=0; k<nz_number; k++)
   new_cols[rows[k]+1]++;
 for (j=1; j<dimension; j++)
   new_cols[j+1] += new_cols[j];

 for (j=0; j<dimension; j++)                   // copies the elements
   for (k=columns[j]; k<columns[j+1]; k++) {
     new_rows[new_cols[rows[k]]] = j;
     new_elem[new_cols[rows[k]]] = elements[k];
     new_cols[rows[k]]++;
   }
 for (j=dimension-1; j>=0; j--)
   new_cols[j+1] = new_cols[j];
 new_cols[0] = 0;

 delete[] elements;
 elements = new_elem;
 delete[] rows;
 rows     = new_rows;
 delete[] columns;
 columns  = new_cols;
}

     //--------------------------------------------------------------------------//
     //    Output on 'fout' file stream converting the row and columns indexes   //
     //--------------------------------------------------------------------------//
void HBF::convert(const peps_name file, const hbf_format format)
{
 ofstream fout;
 int      i;

 transpose();           // transpose the matrix - the result is always in c format
 if (format == C_format)
   Open_File(file, hbf_file, fout);
 else
   Open_File(file, dot_file, fout);
 fout << dimension << " " << nz_number << "\n";
 for (i=0; i<nz_number; i++)
   fout << elements[i] << "\n";
 if (format == C_format) {              // keep on 'c' format
   for (i=0; i<nz_number; i++)          //   (it have been converted on transpose)
     fout << rows[i]       << " ";
   fout << "\n";
   for (i=0; i<=dimension; i++)
     fout << columns[i]    << " ";
 }
 else {                                 // convert from 'c' to 'fortran' format
   for (i=0; i<nz_number; i++)          //   (adds one to each row and column
     fout << rows[i]+1     << " ";      //                                indexes)
   fout << "\n";
   for (i=0; i<=dimension; i++)
     fout << columns[i]+1  << " ";
 }
 fout << "\n";
 if (format == C_format)
   Close_File(file, hbf_file, fout);
 else
   Close_File(file, dot_file, fout);
 erase();
}
     //--------------------------------------------------------------------------//
     //                     Output on 'fout' file stream                         //
     //--------------------------------------------------------------------------//
void HBF::write() const
{
 ofstream fout;
 int      i;

 Open_File(file_name, hbf_file, fout);
 fout << dimension << " " << nz_number << "\n";
 for (i=0; i<nz_number; i++)
   fout << elements[i] << "\n";
 for (i=0; i<nz_number; i++)
   fout << rows[i]     << " ";
 fout << "\n";
 for (i=0; i<=dimension; i++)
   fout << columns[i]  << " ";
 fout << "\n";
 Close_File(file_name, hbf_file, fout);
}



     //--------------------------------------------------------------------------//
     //                     Input from 'fin' file stream                         //
     //--------------------------------------------------------------------------//
void HBF::read(const peps_name file)
{
 ifstream fin;
 int n, d, i;

 Open_File(file, hbf_file, fin);
 fin >> d;
 fin >> n;
 create(n, d);
 baptise(file);
 for (i=0; i<nz_number; i++)
   fin >> elements[i];
 for (i=0; i<nz_number; i++)
   fin >> rows[i];
 for (i=0; i<=dimension; i++)
   fin >> columns[i];
 Close_File(file_name, hbf_file, fin);
}
     //--------------------------------------------------------------------------//
     //              Memory space used by 'this' (in memory units)               //
     //--------------------------------------------------------------------------//
int HBF::mem_use() const
{
 int answer = 8;
 answer += nz_number * 8;
 answer += nz_number * 4;
 answer += (dimension+1) * 4;
 return(answer);
}

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
     //   Algorithm Principles:                                                  //
     //        - This function behaves like a complete aggregation of the main   //
     // descriptor. All automata of the model are aggregated into a single one   //
     // (all transitions become local to this single automata);                  //
     //        - The order of the resulting matrix will be reduced according to  //
     // the reachable state space size;                                          //
     //        - The numerical evaluation of each element of the resulting       //
     // matrixm is performed by the "evaluate a coordinate" member function of   //
     // the class descriptor ('rp dsc::coord(i,j);                               //
     //        - The descriptor may refers to a reduced state space (due to a    //
     // grouping process). In this, case its original information may be be      //
     // changed (different number and size of automata, remaining states, etc).  //
     //--------------------------------------------------------------------------//
     //   Algorithm: - the original state space is visited column by column;     //
     //                . if the column corresponds to a reachable state:         //
     //                  - for all elements in this column;                      //
     //                    . if its row corresponds to a reachable state:        //
     //                      - computes the transition rate;                     //
     //                      - store it in the temporary 'buf';                  //
     //                  - copies 'buf' vector in a sparse column structure;     //
     //              - copies all temporary sparse columns to 'this' structure;  //
     //--------------------------------------------------------------------------//
     //   Remark: this implementation performs three copies of each matrix       //
     // elements and requires a little bit less than twice the memory space of   //
     // the result. This waste of time and memory can be avoided if a good       //
     // estimation of the number of nonzero elements can be obtained.            //
     //--------------------------------------------------------------------------//
void HBF::generateF(const peps_name file)
{
 erase();

 //----------------------------------------------------- Original descriptor information
 int    o_ss;      // the product state space (original state space size)
 int    o_i, o_j;  // coordinates of the rows and columns of the product state space
 aut_st o_sizes;   // the size of each original automaton
 aut_st o_state;   // the state vector (corresponding to the state in 'o_i')

 o_ss = RSS::rss.orig_size();           // the product state space size
 FTB::ftb.get_domain(o_sizes);          // the automata sizes
 o_state.create(FTB::ftb.aut());        // the state vector
 o_state.reset();                       //     (allocation and reset)

 //------------------------------------------------------ Current descriptor information
 aut_st d_sizes;   // the size of each automata in the descriptor
 aut_st d_i;       // the state vector (used to index the rows of the descriptor)
 aut_st d_j;       // the state vector (used to index the columns of the descriptor)

 DSC::dsc.get_s(d_sizes);               // gets the automata sizes
 d_j.create(DSC::dsc.aut());            // the state vector for the rows (allocation)
 d_i.create(DSC::dsc.aut());            // the state vector for the columns (allocation)

 //-------------------------------------------------------- Resulting matrix information
 int    r_i, r_j;  // coordinates for the reachable state space (and resulting matrix)
 int    k;         // counter for nonzero elements of the resulting matrix

 dimension = RSS::rss.reach_size();     // the reachable state space size

 //--------------------------------------------------------------- Temporary information
 rp  ** t_elem;   // temporary array for the sparse columns nonzero elements
 int ** t_rows;   // temporary array for the row positions of each nonzero element
 int  * t_nzeros; // temporary size (number of nonzero elements) of each sparse column
 rp   * buf;      // a buffer to store the visited colum (non sparse)


 t_elem = new rp*[dimension];                       // alocates the temporary arrays
 t_rows = new int*[dimension];                      //   for the rows and nonzeros
 t_nzeros = new int[dimension];                     //   elements, the size of each
 buf = new rp[dimension];                           //   column and the buffer column


 // Ihab ------------------------- Get colone number to print

 //------------------------------------------------------------ "Visit all columns" loop
 for (o_j=0, d_j.reset(), r_j=0; o_j<o_ss; o_j++) { // for each original state       - j
   if (RSS::rss.remains(o_j)) {                     //   if it is in the descriptor  - j
     if (RSS::rss.has(o_j)) {                       //     if it is reachable        - j
       t_nzeros[r_j] = 0;                                 // init. the number of nonzero
                                                          // elements of this column

       //--------------------------------------------------------- "Visit all rows" loop
       for (o_i=0, d_i.reset(), r_i=0; o_i<o_ss; o_i++) { // for each original state - i
         if (RSS::rss.remains(o_i)) {                     //   if it is in the desc. - i
           if (RSS::rss.has(o_i)) {                       //     if it is reachable  - i

             //--------------------------------------------- Computes the matrix element
             if (absv(buf[r_i] = DSC::dsc.coord(d_i, d_j, o_state)) > close_to_zero) {
                                                             // stores it in 'buf'
               t_nzeros[r_j]++;                              // counts nonzeros
	       //cout << "r_i: "<< r_i <<" r_j: "<< r_j <<" val: "<< buf[r_i] <<"\n";
	     }
             //-------------------------------------------------------------------------

             r_i++;                                       //     advance reach state - i
           }
           d_i.next(d_sizes);                             //   advance desc. state   - i
         }
         o_state.next(o_sizes);                           // advance original stace  - i
       }
       //-------------------------------------------------------------------------------

       t_elem[r_j] = new rp[t_nzeros[r_j]];             // allocates sparse column 'r_j'
       t_rows[r_j] = new int[t_nzeros[r_j]];
       for (r_i=0, k=0; r_i<dimension; r_i++)           // it transfers the computed
         if (absv(buf[r_i]) > close_to_zero) {          //   column from 'buf' to the
           t_elem[r_j][k] = buf[r_i];                   //   temporary sparse column
           t_rows[r_j][k] = r_i;                        //   (vectors 't_elem' and
           k++;                                         //    't_rows')

	 }
       r_j++;                                       //     advance reachable state   - j
     }
     d_j.next(d_sizes);                             //   advance descriptor state    - j
   }
   cout << "\r                                      "
        << "\rGenerating Column " << o_j << "/" << o_ss << " (pss) "
        << r_j << "/" << dimension << " (rss)";                             cout.flush();
 }                                                  // advance original state        - j
 //-------------------------------------------------------------------------------------
 delete[] buf;
 cout << "\r                                      "
      << "\rGenerating Column " << o_j << "/" << o_ss << " (pss) "
        << r_j << "/" << dimension << " (rss)\nFinishing ... ";       cout.flush();


 //----------------------------------------------------- Copies sparse columns to 'this'
 nz_number = 0;
 for (r_j=0; r_j<dimension; r_j++)            // counts the total number of nonzeros
   nz_number += t_nzeros[r_j];
 elements  = new rp[nz_number];               // allocates 'this' structures
 rows      = new int[nz_number];
 columns   = new int[dimension+1];
 nz_number = 0;
 for (r_j=0; r_j<dimension; r_j++) {          // for each column
   columns[r_j] = nz_number;                     // stores its first element index
   for (k=0; k<t_nzeros[r_j]; k++) {             // copies its elements
     elements[nz_number] = t_elem[r_j][k];
     rows[nz_number] = t_rows[r_j][k];
     nz_number++;
   }
   delete[] t_elem[r_j];                         // erases its sparse column vectors
   delete[] t_rows[r_j];
 }
 columns[r_j] = nz_number;                    // stores the end of the last column index

 delete[] t_elem;                             // erases temporary structures
 delete[] t_rows;
 delete[] t_nzeros;
 baptise(file); write();                      // stores 'this' name and save it!

}



int HBF::LexToSan(int * info, int N, aut_st d_sizes)
{
int v=0;
int  j , k, p;
/*int* info;
info = new int[N];
k=0;
for(j=N-1;j>=0;j--) info[k++] = info1[j];
*/

for(j=0;j<N-1;j++)
{
 p=1;
 if(info[j]>0){
 for(k=j+1;k<N;k++) p*=d_sizes.info[k];
 v += info[j]*p;
 }
}
v += info[N-1];

return(v);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
///---------------------------------Here the sparce generation ----------------------------------------///
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////


void HBF::generate(const peps_name file)
{
erase();
 //------------------------------------------------------ Current Column information
 aut_st d_sizes;    // the size of each automata in the descriptor
 aut_st d_i;
 aut_st d_j;    // the state vector (used to index the Column)
 DSC::dsc.get_s(d_sizes);               // gets the automata sizes
 d_i.create(DSC::dsc.aut());            // the state vector for the column (allocation)
 d_j.create(DSC::dsc.aut());            // the state vector for the column (allocation)
 //-------------------------------------------------------- Resulting matrix information
 int nb_mat;  //  number of matrices(automatas)
 dimension = RSS::rss.reach_size();     // the reachable state space size
 nb_mat=d_i.dimension; // gets the number of automata
 int o_ss; //  product state space
 o_ss = RSS::rss.orig_size();           // the product state space size
 //--------------------------------------------------------------- Temporary information
 rp  * t_elem;  // temporary array for the value of elements of the column. it store no diagonal elements at each iteration
 int ** t_index_lex; // temporary matrix to stor lexicographic order of each temporary element t_elem
  rp * elem; // used to exchange with t_elem
 int ** index_lex; // used to exchange with t_index_lex

 int   t_nzeros; // temporary size (number of nonzero elements) of column at each iteration
 rp elemDiag;  // temporary  to sums up all diagonal elements
 rp valDiag; // temporary to store the value of diagonal element for contribution of the negative matrices.
 int actual_size; // it contains the maximum size of the Column before doing the next iteration
 int old_size; // it contains the maximum size of the Column after doing the next iteration
 int events;  // number of synchronized events;
 events = DSC::dsc.ev();
 //--------------------------------------------------------------- the sparce matrix Fields
 int ** rows; // it contains the row's number for each element of the Column of the sparce matirx
 rp ** elements; // it contains the elements of each Column of the sparce matirx
 int * lenght; // it contains the number of nozero element of each Columns of the matrix
 int st_index; // it contains the SAN index of the Column
 int * col; // Limits
 int col_id=0; // dimension!!!
 int col_nz;// it contains the number of nozero element of current Column
 //---------------------------------------------------------------- Other inetrmediate variables
 int index ; // global state index
 rp buf; // it contain the value of a temporary element
 int isIdentity; // booleen variable to specify the type of matrix (identitiy or not)
 int i; // matrix index
 int j; // local state index
 int k,l, t_nz,o, stop;
 int temp, emp=0;
 int nz_number=0;
 ofstream w_hbf;

 //-----------------------------------------------------------------------------------------------!
 //
 //                                          The Programm
 //
 //-----------------------------------------------------------------------------------------------!
rows = new int *[dimension];
elements = new rp *[dimension];
lenght = new int[dimension];
columns = new int [dimension];
col = new int[dimension+1];

/////////search the reachable state space index-----------------begin
d_j.reset();i=0;
for(o=0;o<o_ss;o++){
st_index = LexToSan(d_j.info,d_j.dimension,d_sizes);
if (RSS::rss.has(st_index)) columns[i++] =st_index;
d_j.next(d_sizes);
}
//////////////////////////////////////////////---------------------end

d_i.reset();
col[0] = 0;
 for(o=0;o<o_ss;o++){
	st_index = LexToSan(d_i.info,d_i.dimension,d_sizes);
	elemDiag= 0;
	if (RSS::rss.has(st_index)){
		col_elem = new rp[dimension];
		col_rows = new int[dimension];
		col_nz=0;
//---------------------------------------------------------------------- The Synchronization part Computition
		for(int e=0;e<events;e++){
 			i=0;
			t_nzeros = 0;
			actual_size = d_sizes.info[i];
 			t_elem = new rp[actual_size];
 			t_index_lex = new int * [actual_size];
 			for(k=0;k<actual_size;k++)  t_index_lex[k] = new int [i+1];
			for(j=0;j<d_sizes.info[i];j++){
	 			if (absv(buf = DSC::dsc.get_pos_sync_coord(e, i, j, d_i.info[i],d_i)) > close_to_zero) {
 						t_elem[t_nzeros]=buf;
						t_index_lex[t_nzeros][i] = j;
 						t_nzeros++;
						}
				}
 			for(i=1;i<nb_mat && t_nzeros!=0;i++){
				elem = new rp[t_nzeros];
 				index_lex = new int *[t_nzeros];
 				for(j=0;j<t_nzeros;j++){
					elem[j] = t_elem[j];
					index_lex[j] = new int [i];
					for(k=0;k<i;k++) index_lex[j][k] = t_index_lex[j][k];
 					}
				delete[] t_elem;
 				for(k=0;k<actual_size;k++) delete[] t_index_lex[k];
 				delete[] t_index_lex;

				old_size = actual_size;
 				actual_size = t_nzeros * d_sizes.info[i];
				t_nz =t_nzeros;

				t_elem = new rp[actual_size];
 				t_index_lex = new int *[actual_size];
 				for(k=0;k<actual_size;k++)	t_index_lex[k] = new int[i+1];

 				isIdentity = DSC::dsc.get_type(e,i);
 				if(isIdentity== Identity){
					for(k=0;k<t_nzeros;k++){
						t_elem[k] = elem [k];
						for(l=0;l<i;l++) t_index_lex[k][l] = index_lex[k][l];
						t_index_lex[k][i]=d_i.info[i];
						}
					}
				else{
					t_nzeros=0;
					for(k=0;k<t_nz;k++){
						for(j=0;j<d_sizes.info[i];j++){
 							if (absv(buf = DSC::dsc.get_pos_sync_coord(e, i, j, d_i.info[i],d_i)) > close_to_zero) {
								t_elem[t_nzeros]= elem[k] * buf;
								for(l=0;l<i;l++) t_index_lex[t_nzeros][l] = index_lex[k][l];
								t_index_lex[t_nzeros][i] = j ;
								t_nzeros++;
								}
							}
 						}
					}
				delete[] elem;
				for(k=0;k<t_nz;k++) delete[] index_lex[k];
				delete[] index_lex;
				}
			valDiag = DSC::dsc.get_neg_sync_coord(e, d_i, d_i);
			elemDiag += valDiag;
			if(t_nzeros >0){
				for(k=0;k<t_nzeros;k++){
					stop=0;
					i=0;
					index = LexToSan(t_index_lex[k], nb_mat,d_sizes) ;
					if (RSS::rss.has(index)){
						while((stop==0) &&(i<col_nz)){
							if(index == col_rows[i]) stop=1;
							else i++;
							}
						if(stop==0){
							col_elem[col_nz] =t_elem[k] ;
							col_rows[col_nz] = index;
							col_nz++;
							}
						else{
							col_elem[i] +=t_elem[k];
							}
						}
					}
				delete[] t_elem;
 				for(k=0;k<old_size;k++) delete[] t_index_lex[k];
				delete[] t_index_lex;
				}
			}

//-------------------------------------------------------------------- The Local part Computition
 		for(i=0;i<nb_mat && emp==0;i++){
  			if(DSC::dsc.get_type(i) != Empty) emp=1;
  			}
 		if(emp==1){
			for(i=0;i<nb_mat;i++){
  				temp=d_i.info[i];
  				for(j=0;j<d_sizes.info[i];j++){
					stop=0;
					k=0;
    					d_i.info[i]=j;
    					index=LexToSan(d_i.info,d_i.dimension,d_sizes);
 					if (RSS::rss.has(index)) {
    						if (absv(buf = DSC::dsc.get_local_coord(i,j,temp,d_i)) > close_to_zero) {
							 if(j==temp){
		     						elemDiag+=buf;
		     						}
                					else{
								while((stop==0) &&(k<col_nz)){
									if(index == col_rows[k]) stop=1;
									else k++;
									}
								if(stop==0){
									col_elem[col_nz] = buf;
									col_rows[col_nz] = index;
									col_nz++;
									}
								else{
									col_elem[k] +=buf;
									}
								}
							}
						}
  					}
				d_i.info[i]=temp;
				}
			}
		if(elemDiag!=0){
			col_elem[col_nz]=elemDiag;
			col_rows[col_nz]=st_index;
			col_nz++;
			}
		for(k=col_nz-1;k>0;k--){
			j = Find_Max(k);
			Permute(k,j);
			}


		rows[col_id] = new int [col_nz];
		elements[col_id] = new rp[col_nz];
		lenght[col_id] =col_nz;
		nz_number+= col_nz;
		for(k=0;k<col_nz;k++){
			elements[col_id][k] = col_elem[k];
			rows[col_id][k]=FindIndice(col_rows[k],dimension);
			}
		col_id++;
		col[col_id] = col[col_id - 1] + col_nz ;
		delete[] col_elem;
		delete[] col_rows;
		}
	d_i.next(d_sizes);
	}
baptise(file);
Open_File(file_name,hbf_file,w_hbf);
w_hbf <<dimension << " "  << nz_number<< "\n";

for(i=0;i<col_id;i++){
	for(j=0;j<lenght[i];j++)
	w_hbf<<elements[i][j] << "\n" ;
	}
for(i=0;i<col_id;i++){
	for(j=0;j<lenght[i];j++)
	w_hbf<<rows[i][j] << " " ;
	}
w_hbf <<"\n";
for(i=0;i<=col_id;i++)
 w_hbf<<col[i] << " " ;
w_hbf <<"\n";
Close_File(file,hbf_file,w_hbf);

for(i=0;i<col_id;i++) delete[] rows[i];
delete[] rows;
delete[] columns;
delete[] lenght;
delete[] col;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                        //
//                          Column's Ordering                                                             //
//                                                                                                        //
///////////////////////////////////////////////////////////////////////////////////////////////////////////
int HBF::Find_Max(int i)
{
int maxval = col_rows[0];
int max =0;
for(int j=1; j<=i; j++){
	if(col_rows[j]>maxval){
  		maxval = col_rows[j] ;
		max= j;
  		}
	}
return(max);
}

void HBF::Permute(int i, int j)
{
int r_temp;
rp e_temp;

	r_temp = col_rows[i];
	col_rows[i] = col_rows[j];
	col_rows[j] = r_temp;

	e_temp = col_elem[i];
	col_elem[i] = col_elem[j];
	col_elem[j] = e_temp;
}

//////////////////////// Quick Sort///// TO be verified !!!!!!!
/*
void HBF::Ordering(int g, int d)
{
 decoupage(g,d);
}

void HBF::decoupage(int g, int d)
{
int place;
int tbloc=1;
if((d-g)>=tbloc){
	choix_pivot(g,d);
	exploration(g,d,place);
	if((place-g)>(d-place)){
		decoupage(place+1,d);
		decoupage(g,place-1);
		}
	else{
		decoupage(g,place-1);
		decoupage(place+1,d);
		}
	}
}

void HBF::exploration(int g, int d, int &place)
{
int r_temp;
rp e_temp;
int i = g;
int j = d + 1;
int pivot = col_rows[g];

while(i<j){
	do
		{
		i+=1;
		}
	while(col_rows[i]<pivot);

	do
		{
		j-=1;
		}
	while(col_rows[j]>pivot);

	r_temp = col_rows[i];
	col_rows[i] = col_rows[j];
	col_rows[j] = r_temp;

	e_temp = col_elem[i];
	col_elem[i] = col_elem[j];
	col_elem[j] = e_temp;
	}
if(i>j){
	r_temp = col_rows[i];
	col_rows[i] = col_rows[j];
	col_rows[j] = r_temp;

	e_temp = col_elem[i];
	col_elem[i] = col_elem[j];
	col_elem[j] = e_temp;
	}
i-=1;
place = i;

r_temp = col_rows[place];
col_rows[place] = col_rows[g];
col_rows[g] = r_temp;

e_temp = col_elem[place];
col_elem[place] = col_elem[g];
col_elem[g] = e_temp;

}

void HBF::choix_pivot(int g, int d)
{
int indpivot;
int r_temp;
rp e_temp;
int milieu = (g+d)/2;

if(col_rows[g]<=col_rows[milieu]){
	if(col_rows[milieu]<=col_rows[d])
		indpivot = milieu;
	else{
		if(col_rows[d]<=col_rows[g])
			indpivot = g;
		else
			indpivot = d;
		}
	}
else{
	if(col_rows[d]<=col_rows[milieu])
		indpivot = milieu;
	else{
		if(col_rows[g]<=col_rows[d])
			indpivot = g;
		else
			indpivot = d;
		}
	}
r_temp = col_rows[indpivot];
col_rows[indpivot] = col_rows[g];
col_rows[g] = r_temp ;

e_temp = col_elem[indpivot];
col_elem[indpivot] = col_elem[g];
col_elem[g] = e_temp ;
}
*/
////////////////////////////////////////////////////////////////
int HBF::FindIndice(int val, int size)
{
int beg =0;
int end =size;
int midle = 0;
int find = 0;
while(find==0){
	midle = (beg + end)/2;
	if(columns[midle]== val) find=1;
	else{
		if((end - beg)<=1) break;
		else{
			if(columns[midle] > val) end = midle;
			else beg = midle;
			}
	 	}
	}
return midle;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
///------------------------------------------------- End ----------------------------------------------///
//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////




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
     //   Algorithm Principles:                                                  //
     //        - This function behaves like a complete aggregation of the main   //
     // descriptor. All automata of the model are aggregated into a single one   //
     // (all transitions become local to this single automata);                  //
     //        - The order of the resulting matrix will be reduced according to  //
     // the reachable state space size;                                          //
     //        - The numerical evaluation of each element of the resulting       //
     // matrixm is performed by the "evaluate a coordinate" member function of   //
     // the class descriptor ('rp dsc::coord(i,j);                               //
     //        - The descriptor may refers to a reduced state space (due to a    //
     // grouping process). In this, case its original information may be be      //
     // changed (different number and size of automata, remaining states, etc).  //
     //--------------------------------------------------------------------------//
     //   Algorithm: - the original state space is visited column by column;     //
     //                . if the column corresponds to a reachable state:         //
     //                  - for all elements in this column;                      //
     //                    . if its row corresponds to a reachable state:        //
     //                      - computes the transition rate;                     //
     //                      - store it in the temporary 'buf';                  //
     //                  - copies 'buf' vector in a sparse column structure;     //
     //              - copies all temporary sparse columns to 'this' structure;  //
     //--------------------------------------------------------------------------//
     //   Remark: this implementation performs three copies of each matrix       //
     // elements and requires a little bit less than twice the memory space of   //
     // the result. This waste of time and memory can be avoided if a good       //
     // estimation of the number of nonzero elements can be obtained.            //
     //--------------------------------------------------------------------------//

void HBF::Visit_Column(rp val, aut_id mark, int a, int nbaut, const aut_st d_sizes,
		       aut_st & d_i, aut_st & d_j, aut_st o_state, int r_i,
		       int & r_j, int & o_j)
{
  //cout << "debut Visit_Column, automate " << a << "\n";
  rp old_val = val;
  aut_id old_mark = mark;

  bool answer = true;
  int i, nright;
  gs_pair tmp;
  info_gspair tmp2;
  
  if (a<nbaut) {
    // loop on the states of automaton a
    while (answer) {
      //cout << "  o_j = " << o_j << "\n";

      if (RSS::rss.remains(o_j)) {            //   if it is in the descriptor  - j
	//if (RSS::rss.has(o_j)) {              //     if it is reachable        - j
	  
	  if (d_i.st(a) == d_j.st(a)) {         // if automaton 'a' does not move
	    // it increments the value with the diagonal element of 'a' if no
	    // automaton has moved, and visit the next automaton
	    if (mark == no_aut)
	      val += DSC::dsc.coord_loc(a, d_i, d_j, o_state);
	    Visit_Column(val, mark, a+1, nbaut, d_sizes,
			 d_i, d_j, o_state, r_i, r_j, o_j);
	  }
	  
	  else                                 // if automaton 'a' moves
	    if (mark == no_aut) {  // if it is the first automaton to move,
	      mark = a;            // mark it and visit next automaton
	      Visit_Column(val, mark, a+1, nbaut, d_sizes,
			   d_i, d_j, o_state, r_i, r_j, o_j);
	    }
	    else { // already 2 aut. moved -> no need to check the states of
          	   // the next automaton. We just count the number of states
	           // after a (original and reachable states) to skip them.
	      nright=1;
	      for (i=a; i<nbaut; i++)
		nright *= d_sizes.sz(i+1);

	      for (i=o_j; i<o_j+nright; i++)
		if (RSS::rss.has(i))
		  r_j ++;

	      o_j += nright;
	    }
	  //}
	
	  //else    // if it is not reachable, visit next state
	  //Visit_Column(val, mark, a+1, nbaut, d_sizes,
	  //	       d_i, d_j, o_state, r_i, r_j, o_j);
	
	val = old_val; 
	mark = old_mark;
	answer = d_j.next_aut(d_sizes, a); // Next state of automata a
      }
      else      // if it is not in the descriptor, visit next state
	Visit_Column(val, mark, a+1, nbaut, d_sizes,
		     d_i, d_j, o_state, r_i, r_j, o_j);
    }
  }
  
  else { // case of the last automaton
    while (answer) {
      //cout << "  o_j = " << o_j << "\n";
      if (RSS::rss.remains(o_j)) {            //   if it is in the descriptor  - j
	if (RSS::rss.has(o_j)) {              //     if it is reachable        - j

	  if (d_i.st(a) == d_j.st(a)) {       // if automaton 'a' does not move
	    // it increments the value with the diagonal element of 'a' if no
	    // automaton has moved, and visit the next automaton
	    if (mark == no_aut)
	      val += DSC::dsc.coord_loc(a, d_i, d_j, o_state);
	  }
	  else                                 // if automaton 'a' moves
	    if (mark == no_aut)    // if it is the first automaton to move,
	      mark = a;            // mark it and visit next automaton
	    else {
	      val = 0;
	      mark = no_aut;
	    }
	  
	  if (mark != no_aut)   // if only one automaton move, the answer is 
	    val = DSC::dsc.coord_loc(mark, d_i, d_j, o_state);  // the moving rate  
	  
	  
	  if (absv(val) > close_to_zero) {
	    // stocker le resultat r_i, r_j, answer
	    //cout << "r_i : " << r_i << "  r_j : " << r_j << " val : " << val << "\n";
	    tmp.first = r_i;                                  // the row
	    tmp.second = val;                                 // the element
	    tmp2.first = r_j;                                 // the column
	    tmp2.second = tmp;
	    
	    if (rowelem.size() == rowelem.capacity())
	      rowelem.reserve(rowelem.size() + dimension);
	    rowelem.push_back(tmp2);
	  }
	  r_j++;  // Next reachable state
	}
	answer = d_j.next_aut(d_sizes, a); // Next state of last automata
	val = old_val;
	mark = old_mark;
      }
      o_j++;
    }
  }
  //cout << "fin Visit_Column, automate " << a << "\n";
}

void HBF::Visit_Column_Evtp(rp val, int e, int a, int nbaut, const aut_st d_sizes,
		       aut_st & d_i, aut_st & d_j, aut_st o_state, int r_i,
		       int & r_j, int & o_j)
{
  //cout << "debut Visit_Column_Evtp, automate " << a << " - evt " << e << "\n";
  rp old_val = val;
  rp tmp_val;

  bool answer = true;
  int i, nright;
  gs_pair tmp;
  info_gspair tmp2;
  
  if (a<nbaut) {
    // loop on the states of automaton a
    while (answer) {
      //cout << "  o_j = " << o_j << "\n";

      if (RSS::rss.remains(o_j)) {            //   if it is in the descriptor  - j
	  
	tmp_val = DSC::dsc.coord_sync_p(e, a, d_i, d_j, o_state);
	if (absv(tmp_val) > close_to_zero) {    
	  val *= tmp_val;
	  Visit_Column_Evtp(val, e, a+1, nbaut, d_sizes,
			    d_i, d_j, o_state, r_i, r_j, o_j);
	}
	
	else { // value 0 -> no need to check the states of
	  // the next automaton. We just count the number of states
	  // after a (original and reachable states) to skip them.
	  nright=1;
	  for (i=a; i<nbaut; i++)
	    nright *= d_sizes.sz(i+1);
	  
	  for (i=o_j; i<o_j+nright; i++)
	    if (RSS::rss.has(i))
	      r_j ++;
	  
	  o_j += nright;
	}
	
	val = old_val; 
	answer = d_j.next_aut(d_sizes, a); // Next state of automata a
      }
      else      // if it is not in the descriptor, visit next state
	Visit_Column_Evtp(val, e, a+1, nbaut, d_sizes,
			  d_i, d_j, o_state, r_i, r_j, o_j);
    }
  }
  
  else { // case of the last automaton
    while (answer) {
      //cout << "  o_j = " << o_j << "\n";
      if (RSS::rss.remains(o_j)) {            //   if it is in the descriptor  - j
	if (RSS::rss.has(o_j)) {              //     if it is reachable        - j

	  tmp_val = DSC::dsc.coord_sync_p(e, a, d_i, d_j, o_state);
	  if (absv(tmp_val) > close_to_zero) {    
	    val *= tmp_val;
	    
	    if (absv(val) > close_to_zero) {
	      // stocker le resultat r_i, r_j, answer
	      //cout << "evtp : r_i : " << r_i << "  r_j : " << r_j << " val : " << val << "\n";
	      tmp.first = r_i;                                  // the row
	      tmp.second = val;                                 // the element
	      tmp2.first = r_j;                                 // the column
	      tmp2.second = tmp;
	      
	      if (rowelem.size() == rowelem.capacity())
		rowelem.reserve(rowelem.size() + dimension);
	      rowelem.push_back(tmp2);
	    }
	  }
	  r_j++;  // Next reachable state
	}
	answer = d_j.next_aut(d_sizes, a); // Next state of last automata
	val = old_val;
      }
      o_j++;
    }
  }
  //cout << "fin Visit_Column_Evtp, automate " << a << " - evt " << e << "\n";
}

void HBF::Visit_Column_Evtn(rp val, int e, int a, int nbaut, const aut_st d_sizes,
		       aut_st & d_i, aut_st & d_j, aut_st o_state, int r_i,
		       int & r_j, int & o_j)
{
  //cout << "debut Visit_Column_Evtn, automate " << a << " - evt " << e << "\n";
  rp old_val = val;
  rp tmp_val;

  bool answer = true;
  int i, nright;
  gs_pair tmp;
  info_gspair tmp2;
  
  if (a<nbaut) {
    // loop on the states of automaton a
    while (answer) {
      //cout << "  o_j = " << o_j << "\n";

      if (RSS::rss.remains(o_j)) {            //   if it is in the descriptor  - j
	
	tmp_val = DSC::dsc.coord_sync_n(e, a, d_i, d_j, o_state);
	if (absv(tmp_val) > close_to_zero) {    
	  val *= tmp_val;
	  Visit_Column_Evtn(val, e, a+1, nbaut, d_sizes,
			    d_i, d_j, o_state, r_i, r_j, o_j);
	}
	
	else { // value 0 -> no need to check the states of
	  // the next automaton. We just count the number of states
	  // after a (original and reachable states) to skip them.
	  nright=1;
	  for (i=a; i<nbaut; i++)
	    nright *= d_sizes.sz(i+1);
	  
	  for (i=o_j; i<o_j+nright; i++)
	    if (RSS::rss.has(i))
	      r_j ++;
	  
	  o_j += nright;
	  
	  val = 1;
	}
	
	val = old_val; 
	answer = d_j.next_aut(d_sizes, a); // Next state of automata a
      }
      else      // if it is not in the descriptor, visit next state
	Visit_Column_Evtn(val, e, a+1, nbaut, d_sizes,
			  d_i, d_j, o_state, r_i, r_j, o_j);
    }
  }
  
  else { // case of the last automaton
    while (answer) {
      //cout << "  o_j = " << o_j << "\n";
      if (RSS::rss.remains(o_j)) {            //   if it is in the descriptor  - j
	if (RSS::rss.has(o_j)) {              //     if it is reachable        - j

	  tmp_val = DSC::dsc.coord_sync_n(e, a, d_i, d_j, o_state);
	  if (absv(tmp_val) > close_to_zero) {   
	    val *= tmp_val;
	    
	    if (absv(val) > close_to_zero) {
	      // stocker le resultat r_i, r_j, answer
	      //cout << "evtn : r_i : " << r_i << "  r_j : " << r_j << " val : " << val << "\n";
	      tmp.first = r_i;                                  // the row
	      tmp.second = val;                                 // the element
	      tmp2.first = r_j;                                 // the column
	      tmp2.second = tmp;
	      
	      if (rowelem.size() == rowelem.capacity())
		rowelem.reserve(rowelem.size() + dimension);
	      rowelem.push_back(tmp2);
	    }
	  }
	  r_j++;  // Next reachable state
	}
	answer = d_j.next_aut(d_sizes, a); // Next state of last automata
	val = old_val;
      }
      o_j++;
    }
  }
  //cout << "fin Visit_Column_Evtn, automate " << a << " - evt " << e << "\n";
}


void HBF::generateS(const peps_name file)
{
 erase();

 //----------------------------------------------------- Original descriptor information
 int    o_ss;      // the product state space (original state space size)
 int    o_i, o_j;  // coordinates of the rows and columns of the product state space
 aut_st o_sizes;   // the size of each original automaton
 aut_st o_state;   // the state vector (corresponding to the state in 'o_i')

 o_ss = RSS::rss.orig_size();           // the product state space size
 FTB::ftb.get_domain(o_sizes);          // the automata sizes

 o_state.create(FTB::ftb.aut());        // the state vector
 o_state.reset();                       //     (allocation and reset)

 //------------------------------------------------------ Current descriptor information
 int    nbaut;     // the total number of automata
 aut_st d_sizes;   // the size of each automata in the descriptor 
 aut_st d_i;       // the state vector (used to index the rows of the descriptor)
 aut_st d_j;       // the state vector (used to index the columns of the descriptor)
 aut_id mark;      // the automaton that moved

 DSC::dsc.get_s(d_sizes);               // gets the automata sizes
 nbaut = d_sizes.size();                // the number of automata
 d_j.create(DSC::dsc.aut());            // the state vector for the rows (allocation)
 d_i.create(DSC::dsc.aut());            // the state vector for the columns (allocation)

 //-------------------------------------------------------- Resulting matrix information
 int    r_i, r_j;  // coordinates for the reachable state space (and resulting matrix)
 ///int    k;         // counter for nonzero elements of the resulting matrix
 int    e;         // counter for the events
 rp     elt;

 dimension = RSS::rss.reach_size();     // the reachable state space size
 
 vector<info_gspair> new_rowelem;
 vector<info_gspair>::const_iterator iter;
 info_gspair tmp2;

 vector<gs_pair> elem;
 vector<gs_pair>::const_iterator iter_b;
 gs_pair tmp;
 

 // rowelem = new vector<info_gspair>[DSC::dsc.ev()+1];
 rowelem.reserve(dimension);

 
 cout << "Exploration of local events ...\n";
 //------------------------------------------------------------------------------------
 // Local events
 //--------------------------------------------------------------- "Visit all rows" loop
 for (o_i=0, d_i.reset(), r_i=0; o_i<o_ss; o_i++) { // for each original state       - i
   if (RSS::rss.remains(o_i)) {                     //   if it is in the descriptor  - i
     if (RSS::rss.has(o_i)) {                       //     if it is reachable        - i
       //cout << "--------------- r_i = " << r_i << "\n";
       //------------------------------------------------------------"Visit all columns"
       d_j.reset(); o_j = 0; r_j = 0; mark = no_aut;
       Visit_Column(0, mark, 0, nbaut-1, d_sizes, d_i, d_j, o_state, r_i, r_j, o_j);

       r_i++;                                       //     advance reachable state   - i
     }
     d_i.next(d_sizes);                             //   advance descriptor state    - i
   }
   o_state.next(o_sizes);                           // advance original state        - i
 } 
 cout << "                 ... done\n";
 

 cout << "Exploration of synchronizing events ...\n";
 //------------------------------------------------------------------------------------
 // Synchronizing events
 //---------------------------------------------------------------------- For each event
 for (e=0; e<DSC::dsc.ev(); e++) {
   //cout << "evenement synchronise " << e << "\n";
   //------------------------------------------------------------- "Visit all rows" loop
   for (o_i=0, d_i.reset(), r_i=0; o_i<o_ss; o_i++) { // for each original state     - i
     if (RSS::rss.remains(o_i)) {                    //   if it is in the descriptor - i
       if (RSS::rss.has(o_i)) {                      //   if it is reachable         - i
	 //cout << "--------------- r_i = " << r_i << "\n";
	 //----------------------------------------------------------"Visit all columns"
	 d_j.reset(); o_j = 0; r_j = 0;
	 
	 Visit_Column_Evtp(1, e, 0, nbaut-1, d_sizes,
			   d_i, d_j, o_state, r_i, r_j, o_j);
	 
	 d_j.reset(); o_j = 0; r_j = 0;
	 Visit_Column_Evtn(1, e, 0, nbaut-1, d_sizes,
			   d_i, d_j, o_state, r_i, r_j, o_j);
	 
	 r_i++;                                     //     advance reachable state   - i
       }
       d_i.next(d_sizes);                           //   advance descriptor state    - i
     }
     o_state.next(o_sizes);                         // advance original state        - i
   }
 }
 cout << "                 ... done\n";
 

 // We sort rowelem : a stable sort is needed, so the rows are still in order.
 cout << "   ... sorting the elements...";
 stable_sort(rowelem.begin(), rowelem.end(), comp3());
 cout << "  done\n";

 //-------------------------------------------------------------------------------------
 // for the synchronizing events : make the sum of elements at same place)--------------
 if (DSC::dsc.ev() != 0) {
   cout << "  ... dealing with synchronizing events ... ";
   elem.reserve(dimension);
   new_rowelem.reserve(dimension);
   iter = rowelem.begin();
   r_j = 0;
   
   while (iter != rowelem.end()) {
     if ((*iter).first == r_j) {
       if (elem.size() == elem.capacity())
	 elem.reserve(elem.size() + dimension);
       elem.push_back((*iter).second);
       ++iter;
     }
     else { //changing of column
       sort(elem.begin(), elem.end(), comp());
       iter_b = elem.begin();
       r_i = 0; elt = 0;
       while (iter_b != elem.end()) {
	 if ((*iter_b).first == r_i) {
	   elt += (*iter_b).second;
	   ++iter_b;
	 }
	 else { //changing of row
	   if (absv(elt) > close_to_zero) {
	     if (new_rowelem.size() == new_rowelem.capacity())
	       new_rowelem.reserve(new_rowelem.size() + dimension);
	     tmp2.first = r_j;
	     tmp.first = r_i;
	     tmp.second = elt;
	     tmp2.second = tmp;
	     new_rowelem.push_back(tmp2);
	   }
	   r_i = (*iter_b).first;
	   elt = 0;
	 }
       }
       // last elem :
       if (absv(elt) > close_to_zero) {
	 if (new_rowelem.size() == new_rowelem.capacity())
	   new_rowelem.reserve(new_rowelem.size() + dimension);
	 tmp2.first = r_j;
	 tmp.first = r_i;
	 tmp.second = elt;
	 tmp2.second = tmp;
	 new_rowelem.push_back(tmp2);
       }
       r_i = (*iter_b).first;
       
       elem.erase(elem.begin(), elem.end());
       r_j = (*iter).first;
     }
   }
   
   // last column
   sort(elem.begin(), elem.end(), comp());
   iter_b = elem.begin();
   r_i = 0; elt = 0;
   while (iter_b != elem.end()) {
     if ((*iter_b).first == r_i) {
       elt += (*iter_b).second;
       ++iter_b;
     }
     else { //changing of row
       if (absv(elt) > close_to_zero) {
	 if (new_rowelem.size() == new_rowelem.capacity())
	   new_rowelem.reserve(new_rowelem.size() + dimension);
	 tmp2.first = r_j;
	 tmp.first = r_i;
	 tmp.second = elt;
	 tmp2.second = tmp;
	 new_rowelem.push_back(tmp2);
       }
       r_i = (*iter_b).first;
       elt = 0;
     }
   }
   // last elem :
   if (absv(elt) > close_to_zero) {
     if (new_rowelem.size() == new_rowelem.capacity())
       new_rowelem.reserve(new_rowelem.size() + dimension);
     tmp2.first = r_j;
     tmp.first = r_i;
     tmp.second = elt;
     tmp2.second = tmp;
     new_rowelem.push_back(tmp2);
   }
   r_i = (*iter_b).first;
   
   elem.erase(elem.begin(), elem.end());
   r_j = (*iter).first;
   
   rowelem = new_rowelem;
   cout << "  done\n";
 }
 

 //------------------------------------------------------------------------------------
 //----------------------------------------------------- Copies sparse columns to 'this'
 nz_number = rowelem.size();                 // counts the total number of nonzeros
 elements  = new rp[nz_number];                  // allocates 'this' structures
 rows      = new int[nz_number];
 columns   = new int[dimension+1];
 nz_number = 0; r_j = 0;
 
 iter = rowelem.begin();
 columns[r_j] = nz_number;                   // stores its first element index
 
 while (iter != rowelem.end()) {          // for each element
   //cout << "elt colonne : " << (*iter).first << "\n";
   while (r_j != (*iter).first) {
     r_j ++;
     columns[r_j] = nz_number;                   // stores its first element index
   }
   tmp = (*iter).second;
   elements[nz_number] = tmp.second;
   rows[nz_number] = tmp.first;
   nz_number++;
   ++iter;
 }
 while (r_j < dimension) {
   r_j ++;
   columns[r_j] = nz_number;                   // stores its first element index
 }
 

 // for (e=0; e<DSC::dsc.ev()+1; e++)
 rowelem.erase(rowelem.begin(), rowelem.end());
 
 baptise(file); write();                      // stores 'this' name and save it!
}


