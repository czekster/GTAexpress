//====================================================================================//
//                                                                                    //
//                PEPS User Interface for UNIX versions Implementation                //
//                                                                                    //
//====================================================================================//
//  This File:   interf.h                   Language: C++  (xlC and CC)               //
//  Software:    PEPS 2003                   Advisor: Brigitte.Plateau@imag.fr        //
//  Doc:         interf.tex                                                           //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 11/nov/04                        by: Leonardo.Brenner@imag.fr        //
//====================================================================================//

//------------------------------------------------------------------------------------//
//                  Operations Performed by PEPS in the UNIX Version                  //
//------------------------------------------------------------------------------------//

enum peps_options {  // all operations that peps can do
//====================================================================================//
//------------------------------- Compiling Operations -------------------------------//
//====================================================================================//
        compile ,  // compile                          (san -> des -> dsc)
      comp_aggr ,  // compile and aggregate            (san -> des -> dsc -> agg)
      comp_norm ,  // compile and normalize            (san -> des -> dsc     ->    cnd)
      comp_2007 ,  // compile PEPS 2007 version  (because of no backwards compatibility)
 comp_aggr_norm ,  // compile, aggregate and normalize (san -> des -> dsc -> agg -> cnd)
 comp_lump_norm ,  // compile, lump and normalize      (san -> des -> dsc -> agg -> cnd)
      aggregate ,  // aggregate                                      (dsc -> agg)
      aggr_norm ,  // aggregate and normalize                        (dsc -> agg -> cnd)
     normalize1 ,  // normalize from a non-aggregated                (dsc     ->    cnd)
     normalize2 ,  // normalize from an aggregated                          (agg -> cnd)
//====================================================================================//
//--------------------------------- Solve operations ---------------------------------//
//================================================================== some unused =====//
         power_std  ,  // solution with Power method:
         power_adtv ,  //    - Partial Aditive precond.                         * unused
         power_mult ,  //    - Partial Multiplicative precond.                  * unused
         power_altn ,  //    - Partial Alternate precond.                       * unused
         power_diag ,  //    - Inverse Diagonal precond.
         power_plyn ,  //    - Polynomial Inverse precond.                      * unused
         power_ipln ,  //    - Polynom. Shift Inverse precond.                  * unused
       arnoldi_std  ,  // solution with Arnoldi method:
       arnoldi_adtv ,  //    - Partial Aditive precond.                         * unused
       arnoldi_mult ,  //    - Partial Multiplicative precond.                  * unused
       arnoldi_altn ,  //    - Partial Alternate precond.                       * unused
       arnoldi_diag ,  //    - Inverse Diagonal precond.
       arnoldi_plyn ,  //    - Polynomial Inverse precond.                      * unused
       arnoldi_ipln ,  //    - Polynom. Shift Inverse precond.                  * unused
         gmres_std  ,  // solution with GMRES method:
         gmres_adtv ,  //    - Partial Aditive precond.                         * unused
         gmres_mult ,  //    - Partial Multiplicative precond.                  * unused
         gmres_altn ,  //    - Partial Alternate precond.                       * unused
         gmres_diag ,  //    - Inverse Diagonal precond.
         gmres_plyn ,  //    - Polynomial Inverse precond.                      * unused
         gmres_ipln ,  //    - Polynom. Shift Inverse precond.                  * unused
  	   unifor_std ,  // solution with standard uniformization method.
//====================================================================================//
//--------------------------------- Vector operations --------------------------------//
//====================================================================================//
        compare_vct ,  // compare two vectors
    look_global_vct ,  // show the probability of a global state  (def. by local states)
          slice_vct ,  // show some elements of a probability vector     (def. by index)
         charac_vct ,  // show vector main characteristics      (largest, smallest, etc)
        residue_vct ,  // compute the residue of a vector   (according to a given model)
    integrate_local ,  // compute probability of local states
  integrate_results ,  // integrate result functions to a probability vector
  analyse_reachable ,  // analyse reachable state from the RSS vector
   import_marca_vct ,  // import a vector generated by MARCA
//====================================================================================//
//---------------------------------- Load operations ---------------------------------//
//====================================================================================//
           read_dsc ,  // load a DSC level model  ./dsc/name.(dsc, ftb, rss, dct, inf)
           read_cnd ,  // load a CND level model  ./cnd/name.(cnd, ftb, rss, dct, inf)
           read_agg ,  // load a AGG level model  ./agg/name.(dsc, ftb, rss, dct, inf)
           read_vct ,  // load a vector           ./name.(vct)
           read_hbf ,  // load a sparse matrix    ./name.(hbf)
//====================================================================================//
//--------------------------------- Inspect operations -------------------------------//
//====================================================================================//
            see_dsc ,  // inspect the DSC level     ./dsc/name.(dsc, ftb, rss, dct, inf)
                       // or inspect the CND level  ./cnd/name.(cnd, ftb, rss, dct, inf)
                       // or inspect the AGG level  ./agg/name.(dsc, ftb, rss, dct, inf)
	    see_rss ,  // inspect a reachable state space ./cnd/name.rss
            see_vct ,  // inspect a vector          ./name.vct
            see_hbf ,  // inspect a sparse matrix   ./name.hbf
//====================================================================================//
//---------------------------------- HBF operations ----------------------------------//
//================================================================== some unused =====//
       generate_hbf ,  // full aggregation of dsc into a hbf file
          power_hbf ,  // solution with Power method                            * unused
        convert_hbf ,  // convert a hbf from PEPS to MARCA format or vice-versa
          state_hbf ,  // generate the state numbering of a SAN
//====================================================================================//
//---------------------------------- Other operations --------------------------------//
//====================================================================================//
            credits ,  // show PEPS credits
            nothing ,  // do nothing
            goodbye }; // exit PEPS tool

//------------------------------------------------------------------------------------//
//                Interface Functions used by PEPS in the UNIX Version                //
//------------------------------------------------------------------------------------//

void Communicate_Out_of_Memory();

void Open_File(const peps_name file, const file_types ft, ifstream & f_tmp);

void Open_File(const peps_name file, const file_types ft, ofstream & f_tmp);

void Append_File(const peps_name file, const file_types ft, ofstream & f_tmp);

void Close_File(const peps_name file, const file_types ft, ifstream & f_tmp);

void Close_File(const peps_name file, const file_types ft, ofstream & f_tmp);

bool Exists_File(const peps_name file, const file_types ft);

void Get_Current_Dir(peps_name & name);

void Create_Temporary_Directories();

void Remove_Temporary_Directories();

void Remove_Probability_Vectors();

void Remove_Tim_Files();

void Ask_a_File_Name(peps_name & name, const file_types ft);

void Ask_an_Existing_File_Name(peps_name & name, const file_types ft);

bool Permission_to_Proceed(const peps_name name, const file_types ft);

void Get_Only_the_File_Name(const peps_name full_name, peps_name & just_the_file_name);

bool Provide_DSC();

bool Provide_AGG();

bool Provide_CND();

bool Provide_CND_LUD();

bool Provide_HBF();

bool Provide_VCT();

bool Provide_VCT_CND();

void See_DSC();

void See_RSS();

void See_VCT();

void See_HBF();

void Generate_a_New_FCT();

bool Ask_an_Answer(const peps_name name);

v_typ   Ask_an_Initial_Vector_Type();

v_tran Ask_a_Transient_Initial_Vector_Type();

v_impl  Ask_a_Vector_Implementation();

zin_impl  Ask_a_Zin_Implementation();

i_typ   Ask_an_Iteration_Type();

e_typ   Ask_a_Precision_Type();

f_aprox Ask_a_LU_Function_Aproximation_Type();

lu_typ  Ask_a_LU_Decomposition_Type();

reg_typ Ask_a_Matrix_Regulation_Type();

sol_typ Ask_a_LU_Solution_Type();

rp    Ask_an_Error();

rp    Ask_an_Error(int & half, int & rest);

m_typ Ask_a_Product_Method();

strat_typ Ask_a_Strategy_Method();

rp    Ask_a_Threshold();

int   Ask_a_Krylov_Space_Size();

int   Ask_a_Polynomial_Size();

void Ask_Agregation_Map(aut_st & s);

void Ask_Partial_State(aut_st & p, const aut_set & s,
                       const func_id f, const ev_id e, const aut_id a);

int  Ask_an_Iteration_Number();

int  Ask_an_Iteration_Number(int & half, int & rest);

void Ask_a_Vector_Range(int & i, int & f);

void Ask_an_Automaton(aut_id & a);

int Ask_a_Global_State();

int Ask_a_Global_State(aut_st & states);

void Ask_a_Local_State(const aut_id a, const st_id size, st_id & s);

void Ask_Local_States(const aut_id a, const st_id size, st_id & s);

int Ask_Time_Transient_Solution();

void Show_User_Preferences();

void Change_User_Preferences();

void Notify_Time_Spend(const Timer T,
                       const peps_name op, const int iteration, const rp resid);

void Notify_Time_Spend(const Timer T, const peps_name op);

void Notify_Time_Spend(const Timer T, const peps_name name,
                       const peps_name op, const int iteration, const rp resid);

void Notify_Time_Spend(const Timer T, const peps_name name, const peps_name op);

void Log_Out_Solution(const Timer T, const int iteration,
                      const solution_typ s, const precond_typ p,
                      const rp resid);

void Log_Out_a_Solution_Function(const func_id f, const rp v);

void Show_Credits();

void Say_Goodbye();

void Say_Sorry();

peps_options Welcome_Get_Peps_Option();

void Show_Structures();

void Preferences();

//------------------------------------------------------------------------------------//
//                       Macro definition of error messages                           //
//------------------------------------------------------------------------------------//

#define Programming_Error(text, number) \
        { cerr <<  "Programming Error - " << text << " (**** " << number << " ****)\n" \
               << "(file '" << __FILE__ << "' - line " << __LINE__ << ")\n" \
               << "****************************************************************\n"; \
        exit(1); }

#define Programming_Warning(text, number) \
        { cerr <<  "Programming Warning - " << text << " (++++ " << number << " ++++)\n" \
               << "(file '" << __FILE__ << "' - line " << __LINE__ << ")\n"; }

#define Compilation_Error(structure, text) \
        { cerr <<  structure << " Compilation Error - " << text << "\n" \
               << "****************************************************************\n"; \
        exit(1); }

#define Bad_Function_Definition(text, number) \
        { cerr <<  "Bad Function Definition - " << text << " (F" << number << ")\n" \
               << "****************************************************************\n"; \
        exit(1); }

#define Bad_Function_Semantics(text) \
        { cerr <<  "Bad Function Semantic Definition - " << text << "\n" \
               << "****************************************************************\n"; \
        exit(1); }

#define Function_Stack_Overflow(max_stack_size, max_top) \
       { cerr << "************* Function Evaluation Internal Error *************\n\n" \
              << "This function evaluation exceeds the maximum stack size. You can\n" \
              << "change the function in order to keep the evaluation within its\n" \
              << "limit (" << max_stack_size << "), or re-compile PEPS changing the" \
              << " constant 'max_stack_size'\n" \
              << "value in the file 'peps.h' (the maximum value need by this function\n" \
              << "is: " << max_top << "\n" \
              << "****************************************************************\n"; \
       exit(1); }
