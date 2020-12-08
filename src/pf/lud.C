//====================================================================================//
//                                                                                    //
//                       LU decomposed Descriptor Structure                           //
//                                                                                    //
//====================================================================================//
//  This File:   lud.C                      Language: C++  (xlC and CC)               //
//  Software:    PEPS 3.1                    Advisor: Brigitte.Plateau@imag.fr        //
//  Error Codes: 5500                                                                 //
//====================================================================================//
//  Creation:    22/jun/96                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 14/feb/00                        by: Paulo.Fernandes@imag.fr         //
//====================================================================================//
#include "peps.h"

     //--------------------------------------------------------------------------//
     //                             Static variable                              //
     //--------------------------------------------------------------------------//
LUD LUD::lud = LUD();

     //--------------------------------------------------------------------------//
     //                      Output on 'fout' file stream                        //
     //--------------------------------------------------------------------------//
void LUD::write() const
{
 ofstream fout;
 aut_id   a;
 ev_id    e;

 Open_File(file_name, lud_file, fout);
 fout << automata << " " << events << "\n";
 for (a=0; a<automata; a++)                        // l_valid
   fout << l_valid[a] << " ";
 fout << "\n";
 for (e=0; e<events; e++)                          // s_valid
   fout << s_valid[Mat_plus][e] << " ";
 fout << "\n";
 for (e=0; e<events; e++)
   fout << s_valid[Mat_minus][e] << " ";
 fout << "\n";
 sizes.write(fout);                                // sizes
 p_jumps.write(fout);                              // jumps
 for (a=0; a<automata; a++) {
   local_mat[a].write(fout);                       // local_mat
   local_mat2[a].write(fout);
 }
 for (e=0; e<events; e++)
   for (a=0; a<automata; a++) {
     synch_mat[e][a].write(fout);                  // synch_mat
     synch_adj[e][a].write(fout);                  // synch_adj
   }
 Close_File(file_name, lud_file, fout);
}
     //--------------------------------------------------------------------------//
     //                      Input from 'fin' file stream                        //
     //--------------------------------------------------------------------------//
void LUD::read(const peps_name file)
{
 ifstream fin;
 aut_id   a;
 ev_id    e;

 Open_File(file, lud_file, fin);
 fin >> a;
 fin >> e;
 create(a, e);
 baptise(file);
 for (a=0; a<automata; a++)                        // l_valid
   fin >> l_valid[a];
 for (e=0; e<events; e++)                          // s_valid
   fin >> s_valid[Mat_plus][e];
 for (e=0; e<events; e++)
   fin >> s_valid[Mat_minus][e];
 sizes.read(fin);                                  // sizes
 p_jumps.read(fin);                                // jumps
 for (a=0; a<automata; a++) {
   local_mat[a].read(fin);                         // local_mat
   local_mat2[a].read(fin);
 }
 for (e=0; e<events; e++)
   for (a=0; a<automata; a++) {
     synch_mat[e][a].read(fin);                    // synch_mat
     synch_mat2[e][a].read(fin);
     synch_adj[e][a].read(fin);                    // synch_adj
     synch_adj2[e][a].read(fin);
   }
 Close_File(file_name, lud_file, fin);
}
     //--------------------------------------------------------------------------//
     //              Memory space used by 'this' (in memory units)               //
     //--------------------------------------------------------------------------//
int LUD::mem_use() const
{
 aut_id a;
 ev_id  e;
 int    answer = 8;

 answer += sizes.mem_use();
 for (a=0; a<automata; a++) {
   answer += 4;
   answer += local_mat[a].mem_use();
 }
 for (e=0; e<events; e++) {
   for (a=0; a<automata; a++) {
     answer += synch_mat[e][a].mem_use();
     answer += synch_adj[e][a].mem_use();
     answer += 4;
   }
 }
 answer += 12;
 return(answer);
}

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
     //                       the 'this' eliminating the functional elements;    //
     //      - 'normalize'  - it normalizes the matrices (scaling) of 'this'     //
     //                       using 'f';                                         //
     //      - 'decompose'  - it decomposes the matrices of 'this';              //
     //                                                                          //
     //    Note: This operation is split in three distinct functions to allow    //
     // the evaluation of the largest element of the descriptor outside (using   //
     // the same factor of the normalized descriptor ('CND::cnd').               //
     //--------------------------------------------------------------------------//

     //--------------------------------------------------------------------------//
     //                         Main Function Generate                           //
     //--------------------------------------------------------------------------//
     //    This function copies the matrices of the descriptor 'DSC::dsc' to the //
     // matrices of 'this'. The functional elements of the matrices are replaced //
     // by constant elements according to the option expressed in the 'PRF::prf' //
     // (user preferences object) 'lu_aprox' (LU aproximation police).           //
     //                                                                          //
     //    The options are:                                                      //
     //                                                                          //
     //    - 'lu_user' : each function in the matrix is replaced by a evaluation //
     //                    according to a dialogue with the user;                //
     //    - 'lu_max'  : all functional elements are replaced by its maximum     //
     //                    value;                                                //
     //    - 'lu_min'  : all functional elements are replaced by its minimum     //
     //                    value;                                                //
     //    - 'lu_ave'  : all functional elements are replaced by its average     //
     //                    evaluation (arithmetic means);                        //
     //    - 'lu_zero' : all functional elements are replaced by zeros;          //
     //--------------------------------------------------------------------------//
void LUD::generate(const peps_name name)
{
 aut_id a;
 ev_id  e;
 f_aprox func_aprox = PRF::prf.lu_apx();

 erase();
 automata = DSC::dsc.aut();
 events   = DSC::dsc.ev();
 alloc();
 sizes.copy(DSC::dsc.sizes);                  // copies the sizes
 p_jumps.fill(sizes);                         // computes the permutation jumps
 for (a=0; a<automata; a++) {                 // copies the local matrices
   local_mat[a].copy_elim(func_aprox, DSC::dsc.local_mat[a],
                                      DSC::dsc.state_map, no_ev, a);
 }
 for (e=0; e<events; e++)                     // copies the synch. event matrices
   for (a=0; a<automata; a++) {
   synch_mat[e][a].copy_elim(func_aprox, DSC::dsc.synch_mat[e][a],
                                         DSC::dsc.state_map, e, a);
   synch_adj[e][a].copy_elim(func_aprox, DSC::dsc.synch_adj[e][a],
                                         DSC::dsc.state_map, e, a);
   }
 regular_type = PRF::prf.reg_type();
 strcpy(file_name, name);
}

     //--------------------------------------------------------------------------//
     //                         Main Function Normalize                          //
     //--------------------------------------------------------------------------//
     //    This function normalizes (scaling) all local matrices and the event   //
     // matrices containing the rates of each synchronizing event.               //
     //--------------------------------------------------------------------------//
void LUD::normalize(const rp f)
{
 ev_id  e;
 aut_id a;

 for (a=0; a<automata; a++) {
   local_mat[a].scale(f);
   local_mat2[a].copy(local_mat[a]); }
 for (e=0; e<events; e++) {
   synch_mat[e][DSC::dsc.get_sr(e)].scale(f);
   synch_adj[e][DSC::dsc.get_sr(e)].scale(f);
 }
 for (e=0; e<events; e++) {
   for (a=0; a<automata; a++) {
     synch_mat2[e][a].copy(synch_mat[e][a]);
     synch_adj2[e][a].copy(synch_adj[e][a]);
   }
 }
}

     //--------------------------------------------------------------------------//
     //                          Main Function Decompose                         //
     //--------------------------------------------------------------------------//
     //       This function performs the LU decomposition of all matrices.       //
     //--------------------------------------------------------------------------//
     //   A local term is valid if, and only if, the non-identity matrix has a   //
     // correct LU decomposition.                                                //
     //   A synchronizing event term is valid if, and only if, all matrices of   //
     // the tensor product have a correct LU decomposition.                      //
     //--------------------------------------------------------------------------//
void LUD::decompose()
{
 aut_id  a;
 ev_id   e;
 lu_typ  type   = PRF::prf.lu_type();  // the LU decomposition type
 rp      thresh = PRF::prf.lu_thrs();  // the LU decomposition uses a threshold (ILUTH)
 reg_typ reg    = PRF::prf.reg_type(); //                  and a matrix regulation type

 for (a=0; a<automata; a++) {                      // local terms
   switch (reg) {
   case shift_reg  : local_mat[a].add_diagonal(epsilon);    break;
   case wingl_reg  : local_mat[a].winglet();                break;
   case tychv_reg  : local_mat[a].tychonov(local_mat2[a]);
                     local_mat2[a].transpose();             break;
   }
   if (type == std_lu)
     l_valid[a] = local_mat[a].lu_decomp(thresh, reg);
   else
     l_valid[a] = local_mat[a].sor_decomp(thresh, reg);
 }
 for (e=0; e<events; e++) {
   s_valid[Mat_plus][e]  = false;    //  changed !!!!!!!  (it should be 'true')
   s_valid[Mat_minus][e] = false;    //  to look latter when it can be good ...
   for (a=0; a<automata; a++) {
     switch (reg) {
     case shift_reg : synch_mat[e][a].add_diagonal(epsilon);
                      synch_adj[e][a].add_diagonal(epsilon);    break;
     case wingl_reg : synch_mat[e][a].winglet();
                      synch_adj[e][a].winglet();                break;
     case tychv_reg : synch_mat[e][a].tychonov(synch_mat2[e][a]);
                      synch_mat2[e][a].transpose();
                      synch_adj[e][a].tychonov(synch_adj2[e][a]);
                      synch_adj2[e][a].transpose();             break;
     }
     if (type == std_lu) {
       s_valid[Mat_plus][e]  = s_valid[Mat_plus][e]
                               && synch_mat[e][a].lu_decomp(thresh, reg);
       s_valid[Mat_minus][e] = s_valid[Mat_minus][e]
                               && synch_adj[e][a].sor_decomp(thresh, reg);
     }
     else {
       s_valid[Mat_plus][e]  = s_valid[Mat_plus][e]
                               && synch_mat[e][a].lu_decomp(thresh, reg);
       s_valid[Mat_minus][e] = s_valid[Mat_minus][e]
                               && synch_adj[e][a].sor_decomp(thresh, reg);
     }
   }
 }
}
     //--------------------------------------------------------------------------//
     //                     Internal Function of allocation                      //
     //--------------------------------------------------------------------------//
void LUD::alloc()
{
 aut_id a;
 ev_id  e;

 sizes.create(automata);                                            // sizes
 p_jumps.create(automata);                                          // permutation jumps
 local_mat = new sp_mat[automata];                                  // local_mat
 local_mat2 = new sp_mat[automata];                                  // local_mat
 for (a=0; a<automata; a++) {
   local_mat[a].create();
   local_mat2[a].create(); }
 synch_mat = new sp_mat*[events];                                   // synch_mat
 synch_mat2 = new sp_mat*[events];
 for (e=0; e<events; e++) {
   synch_mat[e] = new sp_mat[automata];
   synch_mat2[e] = new sp_mat[automata];
   for (a=0; a<automata; a++) {
     synch_mat[e][a].create();
     synch_mat2[e][a].create(); }
 }
 synch_adj = new sp_mat*[events];                                   // synch_adj
 synch_adj2 = new sp_mat*[events];
 for (e=0; e<events; e++) {
   synch_adj[e] = new sp_mat[automata];
   synch_adj2[e] = new sp_mat[automata];
   for (a=0; a<automata; a++) {
     synch_adj[e][a].create();
     synch_adj2[e][a].create(); }
 }
 l_valid = new bool[automata];                                      // l_valid
 s_valid = new bool*[2];                                            // s_valid
 s_valid[Mat_plus]  = new bool[events];
 s_valid[Mat_minus] = new bool[events];
}
     //--------------------------------------------------------------------------//
     //                  Internal Function of transfer contents                  //
     //--------------------------------------------------------------------------//
void LUD::trans(const LUD & d)
{
 aut_id a;
 ev_id  e;

 sizes.copy(d.sizes);                                               // sizes
 p_jumps.copy(d.p_jumps);                                           // permutation jumps
 for (a=0; a<automata; a++) {                                        // local_mat
   local_mat[a].copy(d.local_mat[a]);
   local_mat2[a].copy(d.local_mat[a]); }
 for (e=0; e<events; e++)                                           // synch_mat
   for (a=0; a<automata; a++) {
     synch_mat[e][a].copy(d.synch_mat[e][a]);
     synch_mat2[e][a].copy(d.synch_mat[e][a]);
     synch_adj[e][a].copy(d.synch_adj[e][a]);
     synch_adj2[e][a].copy(d.synch_adj[e][a]);
   }
 for (a=0; a<automata; a++)                                         // l_valid
   l_valid[a] = d.l_valid[a];
 for (e=0; e<events; e++) {                                         // s_valid
   s_valid[Mat_plus][e]  = d.s_valid[Mat_plus][e];
   s_valid[Mat_minus][e] = d.s_valid[Mat_minus][e];
 }
}
     //--------------------------------------------------------------------------//
     //                    Internal Function of desallocation                    //
     //--------------------------------------------------------------------------//
void LUD::desal()
{
 aut_id a;
 ev_id  e;

 delete[] s_valid[Mat_plus];                                      // s_valid
 delete[] s_valid[Mat_minus];
 delete[] s_valid;
 delete[] l_valid;                                                // l_valid

 for (e=0; e<events; e++) {                                       // synch_mat
   for (a=0; a<automata; a++) {                                   //     &
     synch_mat[e][a].erase();                                     // synch_adj
     synch_mat2[e][a].erase();
     synch_adj[e][a].erase();
     synch_adj2[e][a].erase();
   }
   delete[] synch_mat[e];
   delete[] synch_mat2[e];
   delete[] synch_adj[e];
   delete[] synch_adj2[e];
 }
 delete[] synch_mat;
 delete[] synch_mat2;
 delete[] synch_adj;
 delete[] synch_adj2;
 for (a=0; a<automata; a++) {                                      // local_mat
   local_mat[a].erase();
   local_mat2[a].erase(); }
 delete[] local_mat;
 delete[] local_mat2;
 p_jumps.erase();                                                 // permutation jumps
 sizes.erase();                                                   // sizes
}
