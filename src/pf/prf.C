//====================================================================================//
//                                                                                    //
//                             Preferences Structure                                  //
//                                                                                    //
//====================================================================================//
//  This File:   prf.C                      Language: C++  (xlC and CC)               //
//  Software:    PEPS 3.1                    Advisor: Brigitte.Plateau@imag.fr        //
//  Error Codes: 6100                                                                 //
//====================================================================================//
//  Creation:     5/feb/97                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 27/feb/03                        by: Paulo.Fernandes@imag.fr         //
//====================================================================================//
#include "peps.h"

     //--------------------------------------------------------------------------//
     //                   static variable: The User Preferences                  //
     //--------------------------------------------------------------------------//
PRF PRF::prf = PRF();

     //--------------------------------------------------------------------------//
     //                      Output on 'fout' file stream                        //
     //--------------------------------------------------------------------------//
void PRF::write() const
{
 ofstream fout;

 Open_File(file_name, prf_file, fout);
 fout << verbose         << "\n";
 fout << max_iter        << "\n";
 fout << int(iter_type)  << "\n";
 fout << min_error       << "\n";
 fout << int(error_type) << "\n";
 fout << lu_thresh       << "\n";
 fout << int(lu_aprox)   << "\n";
 fout << int(lu_decomp)  << "\n";
 fout << int(lu_reg)     << "\n";
 fout << int(lu_sol)     << "\n";
 fout << int(vec_type)   << "\n";
 fout << int(vector_impl)<< "\n";
 fout << int(meth_type)  << "\n";
 fout << int(strat_type)  << "\n";
 fout << krylov          << "\n";
 fout << poly_size       << "\n";
 Close_File(file_name, prf_file, fout);
}
     //--------------------------------------------------------------------------//
     //                      Input from 'fin' file stream                        //
     //--------------------------------------------------------------------------//
void PRF::read(const peps_name file)
{
 ifstream fin;
 int      t;

 Open_File(file, prf_file, fin);
 baptise(file);
 fin >> verbose;
 fin >> max_iter;
 fin >> t;         iter_type  = i_typ(t);
 fin >> min_error;
 fin >> t;         error_type = e_typ(t);
 fin >> lu_thresh;
 fin >> t;         lu_aprox   = f_aprox(t);
 fin >> t;         lu_decomp  = lu_typ(t);
 fin >> t;         lu_reg     = reg_typ(t);
 fin >> t;         lu_sol     = sol_typ(t);
 fin >> t;         vec_type   = v_typ(t);
 fin >> t;         vector_impl = v_impl(t);
 fin >> t;         meth_type  = m_typ(t);
 fin >> krylov;
 fin >> poly_size;
 Close_File(file_name, prf_file, fin);
}
     //--------------------------------------------------------------------------//
     //              Memory space used by 'this' (in memory units)               //
     //--------------------------------------------------------------------------//
int PRF::mem_use() const
{
 return(52);
}
