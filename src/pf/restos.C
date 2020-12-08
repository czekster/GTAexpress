/****************************************************************************************
****************************************************************************************
                               F R O M    P E P S  _ U . C
****************************************************************************************
****************************************************************************************/

//====================================================================================//
void Compile_Full_Network()
//====================================================================================//
{
 peps_name name;
 cout << "\nCompilation of a textual san\n";
 Ask_an_Existing_File_Name(name, san_file);
 if (Permission_to_Proceed(name, des_file))
   Compile_Network(name, name);
}

//====================================================================================//
void Compile_des()
//====================================================================================//
{
 peps_name name;
 cout << "\nCompilation of a textual descriptor\n";
 Ask_an_Existing_File_Name(name, des_file);
 if (Permission_to_Proceed(name, dsc_file))
   Compile_Descriptor(name, name);
}

//====================================================================================//
void Compile_tft()
//====================================================================================//
{
 peps_name name;
 cout << "\nCompilation of a textual function table\n";
 Ask_an_Existing_File_Name(name, tft_file);
 if (Permission_to_Proceed(name, ftb_file))
   Compile_Function_Table(name, name);
}

//====================================================================================//
void Generate_rss()
//====================================================================================//
{
 peps_name name;
 cout << "\nCompilation of a textual reachability function\n";
 Ask_an_Existing_File_Name(name, fct_file);
 if (Permission_to_Proceed(name, rss_file))
   Compile_Reacheable_SS(name, name);
}

//====================================================================================//
void Compile_dic()
//====================================================================================//
{
 peps_name name;
 cout << "\nCompilation of a textual dictionnary\n";
 Ask_an_Existing_File_Name(name, dic_file);
 if (Permission_to_Proceed(name, dct_file))
   Compile_Dictionary(name, name);
}

//====================================================================================//
void Compile_res()
//====================================================================================//
{
 peps_name name;
 cout << "\nCompilation of a textual table of integration functions (results)\n";
 Ask_an_Existing_File_Name(name, res_file);
 if (Permission_to_Proceed(name, inf_file))
   Compile_Integration_Function(name, name);
}

//====================================================================================//
void Agregate_dsc()
//====================================================================================//
{
 peps_name name;
 Timer     T;
 aut_st    agreg_map;

 if (Provide_DSC()) {
   DSC::dsc.name(name);
   cout << "\nAggregation of the Descriptor '" << name << ".dsc' ("
        << DSC::dsc.aut() << " automata - " << DSC::dsc.ev() << " events)\n";
   Ask_Agregation_Map(agreg_map);
   Ask_a_File_Name(name, dsc_file);
   if (Permission_to_Proceed(name, dsc_file)) {
     T.clear();
     T.start();
     DSC::dsc.aggregate(agreg_map, name);
     T.stop();
     Notify_Time_Spend(T, "aggregation of descriptor");
   }
 }
}

//====================================================================================//
void Generate_cnd()
//====================================================================================//
{
 peps_name name;
 Timer     T;
 bool      lu_too;

 if (Provide_DSC()) {
   DSC::dsc.name(name);
   cout << "\nNormalization of the Descriptor '" << name << ".dsc' ("
        << DSC::dsc.aut() << " automata - " << DSC::dsc.ev() << " events)\n";
   Ask_a_File_Name(name, cnd_file);
   if (Permission_to_Proceed(name, cnd_file)) {
     lu_too = Ask_an_Answer("Do you want to generate an ILU Descriptor now");
     T.clear();
     T.start();
     if (lu_too)
       LUD::lud.generate(name);
     CND::cnd.translate(name);
     T.stop();
     if (lu_too) {
       T.start();
       LUD::lud.normalize(CND::cnd.n_factor());
       LUD::lud.decompose();
       T.stop();
       LUD::lud.write();
     }
     if (lu_too)
       Notify_Time_Spend(T, "descriptor normalization and ILU decomposition");
     else
       Notify_Time_Spend(T, "descriptor normalization");
     DSC::dsc.erase();
   }
 }
}

//====================================================================================//
void Generate_lud()
//====================================================================================//
{
 peps_name name;
 Timer     T;

 if (Provide_DSC()) {
   DSC::dsc.name(name);
   cout << "\nLU decomposition of the Descriptor '" << name << ".dsc' ("
        << DSC::dsc.aut() << " automata - " << DSC::dsc.ev() << " events)\n";
   Ask_a_File_Name(name, lud_file);
   if (Permission_to_Proceed(name, lud_file)) {
     T.clear();
     T.start();
     LUD::lud.generate(name);
     T.stop();
     if (Exists_File(name, cnd_file) &&
         Exists_File(name, ftb_file) &&
         Exists_File(name, rss_file) ) {
       CND::cnd.read(name);
       FTB::ftb.read(name);
       RSS::rss.read(name);
       T.start();
       LUD::lud.normalize(CND::cnd.n_factor());
       LUD::lud.decompose();
       T.stop();
       Notify_Time_Spend(T, "LU decomposition of descriptor");
       LUD::lud.write();
       DSC::dsc.erase();
     }
     else
       cout << ":-( I'm sorry I can't find the file(s) '" << name
            << ".cnd' (ftb or rss)\n";
   }
 }
}


****************************************************************************************
****************************************************************************************
                              F R O M    I N T E R F _ U . C
****************************************************************************************
****************************************************************************************


//====================================================================================//
//  Provide DSC                                                                       //
//====================================================================================//
bool Provide_DSC()
{
 peps_name name;
 bool      answer = true;

 if (DSC::dsc.is_empty()) {
   cout << "No descriptor in memory.\n";
   Ask_an_Existing_File_Name(name, dsc_file);
   DSC::dsc.read(name);
 }
 else
   DSC::dsc.name(name);
 if ((RSS::rss.is_empty()) || (FTB::ftb.aut() == 0))
   if (Exists_File(name, ftb_file) && Exists_File(name, rss_file)) {
     RSS::rss.read(name);
     FTB::ftb.read(name);
   }
   else {
     cout << ":-( Cannot find the file <" << name << ".rss>, nor the file <"
           << name << ".ftb>!\n";
     answer = false;
   }
 if (DSC::dsc.ss() != RSS::rss.size()) {
   cout << "The files <" << name << ".dsc> and  <" << name
        << ".rss> are incompatible" << " (different state space size)\n";
   answer = false;
 }
 if (FTB::ftb.ss() != RSS::rss.orig_size()) {
   cout << "The files <" << name << ".ftb> and  <" << name
        << ".rss> are incompatible" << " (different product state space size)\n";
   answer = false;
 }
 if (!answer)
   cout << ":-( Your request is aborted, verify the integrity of your files\n";
 return(answer);
}
//====================================================================================//
//  Provide CND                                                                       //
//====================================================================================//
bool Provide_CND()
{
 peps_name name;
 bool      answer = true;

 if (CND::cnd.is_empty()) {
   cout << "No descriptor in memory.\n";
   Ask_an_Existing_File_Name(name, cnd_file);
   CND::cnd.read(name);
 }
 else
   CND::cnd.name(name);
 if ((RSS::rss.is_empty()) || (FTB::ftb.aut() == 0))
   if (Exists_File(name, ftb_file) && Exists_File(name, rss_file)) {
     RSS::rss.read(name);
     FTB::ftb.read(name);
   }
   else {
     cout << ":-( Cannot find the file <" << name << ".rss>, nor the file <"
           << name << ".ftb>!\n";
     answer = false;
   }
 if (CND::cnd.ss() != RSS::rss.size()) {
   cout << "The files <" << name << ".cnd> and  <" << name
        << ".rss> are incompatible" << " (different state space size)\n";
   answer = false;
 }
 if (FTB::ftb.ss() != RSS::rss.orig_size()) {
   cout << "The files <" << name << ".ftb> and  <" << name
        << ".rss> are incompatible" << " (different product state space size)\n";
   answer = false;
 }
 if (!answer)
   cout << ":-( Your request is aborted, verify the integrity of your files\n";
 return(answer);
}
//====================================================================================//
//  Provide CND LUD                                                                   //
//====================================================================================//
bool Provide_CND_LUD()
{
 peps_name name;
 bool      answer = true;

 if (CND::cnd.is_empty()) {
   cout << "No descriptor in memory.\n";
   Ask_an_Existing_File_Name(name, cnd_file);
   CND::cnd.read(name);
 }
 else
   CND::cnd.name(name);
 if ((RSS::rss.is_empty()) || (FTB::ftb.aut() == 0))
   if (Exists_File(name, ftb_file) && Exists_File(name, rss_file)) {
     RSS::rss.read(name);
     FTB::ftb.read(name);
   }
   else {
     cout << ":-( Cannot find the file <" << name << ".rss>, nor the file <"
           << name << ".ftb>!\n";
     answer = false;
   }
 if (CND::cnd.aut() != LUD::lud.aut()) {
   if (Exists_File(name, lud_file)) {
     LUD::lud.read(name);
   }
   else {
     cout << ":-( Cannot find the file <" << name << ".lud>!\n";
     answer = false;
   }
 }
 if (CND::cnd.ss()  != LUD::lud.ss()) {
   cout << "The files <" << name << ".cnd> and  <" << name
        << ".lud> are incompatible" << " (different state space size)\n";
   answer = false;
 }
 if (CND::cnd.ss() != RSS::rss.size()) {
   cout << "The files <" << name << ".cnd> and  <" << name
        << ".rss> are incompatible" << " (different state space size)\n";
   answer = false;
 }
 if (FTB::ftb.ss() != RSS::rss.orig_size()) {
   cout << "The files <" << name << ".ftb> and  <" << name
        << ".rss> are incompatible" << " (different product state space size)\n";
   answer = false;
 }
 if (!answer)
   cout << ":-( Your request is aborted, verify the integrity of your files\n";
 return(answer);
}
//====================================================================================//
//  Provide HBF                                                                       //
//====================================================================================//
bool Provide_HBF()
{
 peps_name name;

 if (HBF::hbf.is_empty()) {
   cout << "No HBF matrix in memory.\n";
   Ask_an_Existing_File_Name(name, hbf_file);
   HBF::hbf.read(name);
 }
 return(true);
}
//====================================================================================//
//  Provide VCT                                                                       //
//====================================================================================//
bool Provide_VCT()
{
 peps_name name;

 if (VCT::vct.is_empty()) {
   cout << "No Probability vector in memory.\n";
   Ask_an_Existing_File_Name(name, vct_file);
   VCT::vct.read(name);
 }
 return(true);
}
