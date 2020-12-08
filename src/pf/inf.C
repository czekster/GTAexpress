//====================================================================================//
//                                                                                    //
//                       Integration Functions Table Structure                        //
//                                                                                    //
//====================================================================================//
//  This File:   inf.C                      Language: C++  (xlC and CC)               //
//  Software:    PEPS 3.1                    Advisor: Brigitte.Plateau@imag.fr        //
//  Error Code:  4450                                                                 //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 14/feb/00                        by: Paulo.Fernandes@imag.fr         //
//====================================================================================//
#include "peps.h"

     //--------------------------------------------------------------------------//
     //                   static variable: The Function Table                    //
     //--------------------------------------------------------------------------//
INF INF::inf = INF();
     //--------------------------------------------------------------------------//
     //         It writes the function in entry 'f' on the standard output       //
     //--------------------------------------------------------------------------//
void INF::show(const func_id f)
{
#ifdef _PEPS_DEBUG_
  if (f >= number)
    Programming_Error("index out of range", 4450)
#endif
 table[f].show();
}

     //--------------------------------------------------------------------------//
     //                      Output on 'fout' file stream                        //
     //--------------------------------------------------------------------------//
     //   Remark: the parameters of each function in the table are not saved.    //
     //--------------------------------------------------------------------------//
void INF::write() const
{
 ofstream fout;

 Open_File(file_name, inf_file, fout);
 fout << number << "\n";
 domain.write(fout);
 for (int i=0; i<number; i++)
   table[i].write(fout);
 Close_File(file_name, inf_file, fout);
}
     //--------------------------------------------------------------------------//
     //                      Input from 'fin' file stream                        //
     //--------------------------------------------------------------------------//
     //   Remark: since the parameters of each function in the table is not      //
     // saved, this function generates those parameters from the function        //
     // formulae.                                                                //
     //--------------------------------------------------------------------------//
void INF::read(const peps_name file)
{
 ifstream fin;

 Open_File(file, inf_file, fin);
 fin >> number;                  // gets the number of function entries
 create(number);                 // allocates 'this'
 baptise(file);
 domain.read(fin);               // gets the size of each original automata

 for (number=0; number<dimension; number++) {   // for all function entries
   table[number].read(fin);                        // reads the function formulae
 }                                                 //   set of reference of the
                                                   //   automata in the descriptor
 Close_File(file_name, inf_file, fin);
}
     //--------------------------------------------------------------------------//
     //          It writes all the function entries in the 'inf.dbg' file        //
     //--------------------------------------------------------------------------//
void INF::print() const
{
 peps_name table_name, model_name, buf_name;
 ofstream  fout;
 aut_id    a;
 func_id   f;

 Get_Only_the_File_Name(file_name, table_name);
 DCT::dct.n_name(model_name);
 Open_File("inf", dbg_file, fout);
 fout << "===============================================================\n"
      << "File inf.dbg\n"
      << "===============================================================\n"
      << table_name << ".inf -- A model with " << domain.size() << " orinal automata"
      << "\nUser name: '" << model_name << ".san')"
      << "\n------------\nGeneral Information\n------------"
      << "\nProduct state space:           " << RSS::rss.orig_size() << " states"
      << "\nState space after aggregation: " << RSS::rss.size()      << " states"
      << "\nReachable state space:         " << RSS::rss.reach_size() << " states"
      << "\nAutomata original sizes:           [ ";
 for (a=0; a<domain.size(); a++)
   fout << domain.sz(a) << " ";
 fout << "]\nCurrent Number of Results: " << number
      << " of " << dimension << " places\nSize of this Table: "
      << mem_use() / mem_unit << " Kbytes"
      << "\n===============================================================\n";
 for (f=0; f<number; f++) {
   DCT::dct.sf_name(f, buf_name);
   cout << buf_name << " (R" << f << "): ";
   table[f].show();
   cout << "\n";
 }
 Close_File("inf", dbg_file, fout);
}
     //--------------------------------------------------------------------------//
     //              Memory space used by 'this' (in memory units)               //
     //--------------------------------------------------------------------------//
int INF::mem_use() const
{
 int answer = 8;
 for (int i=0; i<number; i++) {
   answer += table[i].mem_use();
 }
 answer += domain.mem_use();
 return(answer);
}

     //--------------------------------------------------------------------------//
     //                       Internal Function of allocation                    //
     //--------------------------------------------------------------------------//
void INF::alloc()
{
 table = new func_tree[dimension];
 for (int i=0; i<dimension; i++) {
   table[i].create();
 }
}
     //--------------------------------------------------------------------------//
     //                     Internal Function of desallocation                   //
     //--------------------------------------------------------------------------//
void INF::desal()
{
 for (int i=0; i<dimension; i++) {
   //table[i].erase();
 }
 //delete[] table;
}
